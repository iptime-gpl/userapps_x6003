/* This is a module which is used for setting the NFMARK field of an skb. */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/proc_fs.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_mark.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/rtnetlink.h>


#include <net/checksum.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_ecache.h>

extern int check_internal_subnet(u32 ipaddr);

#define WAN1_TABLE	 201
#define WAN2_TABLE	 202
#define DEFAULT_RTID_TAG 255


#ifdef CONFIG_NETFILTER_XT_TARGET_RTMARK_MODULE
#define CONFIG_NETFILTER_XT_RTMARK_WRR
#endif

#ifdef CONFIG_NETFILTER_XT_RTMARK_WRR

#define IP_WRR_TIMEOUT   10*60*HZ /* 10 min , tvsec (10 = 1sec) */
struct ip_wrr_info
{
	unsigned int     rtid;
	unsigned long    timeout;	
};

static int wan1_weight[2]  = {0,0}; /* { set_value, operation_value } */
static int wan2_weight[2]  = {0,0};
static int ip_wrr = 0;
static struct ip_wrr_info ip_wrr_check[256] = {{0,0},};
static struct timer_list ip_wrr_timer;
static int ip_wrr_interval = IP_WRR_TIMEOUT/2;

static void ip_wrr_checker(unsigned long dummy);

#endif

static int wan1_status  = 0;
static int wan2_status  = 0;


static int proc_read_rtmark(char *buffer, char **start, off_t offset, int length, int *eof, void *data)
{
        char *p = buffer;
        int len;

        p +=  sprintf(p, "%d %d", wan1_status, wan2_status);

        len = p - buffer;
        if( len <= offset+length ) *eof = 1;
        *start = buffer + offset;
        len -= offset;
        if( len > length ) len = length;
        if( len < 0 ) len = 0;

        return len;
}

static int proc_write_rtmark(struct file *file, const char *buffer, u_long count, void *data)
{
	wan1_status = (buffer[0] == '0') ? 0 : 1;
	wan2_status = (buffer[2] == '0') ? 0 : 1;
	return count;
}

static int my_atoi(const char *s)
{
    int val = 0;

    for (;; s++) {
        switch (*s) {
            case '0'...'9':
                val = 10*val+(*s-'0');
                break;
            default:
                return val;
        }
    }
}

static int kill_ct(struct nf_conn *i, void *data)
{
	u32  ipaddr=0;

	ipaddr = my_atoi((char *)data);
	//printk("==> %08x %08x \n", ipaddr , i->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip);
	return (ipaddr == i->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip);
}

static int proc_write_cleanup(struct file *file, const char *buffer, u_long count, void *data)
{
	struct net *net;
	char buf[256];

	memcpy(buf, buffer, count);
	buf[count]=0x0;
	rtnl_lock();
	for_each_net(net)
		nf_ct_iterate_cleanup(net, kill_ct, (void *)buf);
	rtnl_unlock();
	return count;
}

#ifdef CONFIG_NETFILTER_XT_RTMARK_WRR
static char * __my_strtok;
static char * my_strtok(char * s,const char * ct)
{
        char *sbegin, *send;

        sbegin  = s ? s : __my_strtok;
        if (!sbegin)
        {
                return NULL;
        }

        sbegin += strspn(sbegin,ct);
        if (*sbegin == '\0')
        {
                __my_strtok = NULL;
                return( NULL );
        }

        send = strpbrk( sbegin, ct);
        if (send && *send != '\0')
                *send++ = '\0';

        __my_strtok = send;

        return (sbegin);
}

static int proc_read_wrr(char *buffer, char **start, off_t offset, int length, int *eof, void *data)
{
        char *p = buffer;
        int len;

        p +=  sprintf(p, "%d %d \n", wan1_weight[0], wan2_weight[0]);

        len = p - buffer;
        if( len <= offset+length ) *eof = 1;
        *start = buffer + offset;
        len -= offset;
        if( len > length ) len = length;
        if( len < 0 ) len = 0;

        return len;
}

static int proc_write_wrr(struct file *file, const char *buffer, u_long count, void *data)
{
        char buf[32], *buf2;

        memcpy(buf, buffer, count);
        buf[count]=0x0;

        buf2 = my_strtok(buf, " ");
        wan1_weight[0] = my_atoi(buf2);
        buf2 = my_strtok(NULL, " ");
        wan2_weight[0] = my_atoi(buf2);

        return count;
}

static int proc_read_ipwrr(char *buffer, char **start, off_t offset, int length, int *eof, void *data)
{
        char *p = buffer;
        int len, idx;

	for(idx=1; idx < 255; idx++)
	{
		if (ip_wrr_check[idx].rtid)
			p += sprintf(p, "[%d] %d (%ul)\n", 
				idx, 
				ip_wrr_check[idx].rtid,
				(long)(ip_wrr_check[idx].timeout - jiffies));
	}

        p +=  sprintf(p, "IP WRR %s \n", (ip_wrr) ? "ON" : "OFF");

        len = p - buffer;
        if( len <= offset+length ) *eof = 1;
        *start = buffer + offset;
        len -= offset;
        if( len > length ) len = length;
        if( len < 0 ) len = 0;

        return len;
}

static int proc_write_ipwrr(struct file *file, const char *buffer, u_long count, void *data)
{
	if (buffer[0] != '0')
	{
		ip_wrr = 1;
		init_timer(&ip_wrr_timer);
		ip_wrr_timer.function = ip_wrr_checker;
		ip_wrr_timer.expires = jiffies + ip_wrr_interval;
		add_timer(&ip_wrr_timer);
	}
	else
	{
		ip_wrr = 0;
		del_timer(&ip_wrr_timer);
	}

        return count;
}
#endif /* CONFIG_IP_NF_RTMARK_WRR */

static u32 init_rtmark_proc(void)
{
	struct proc_dir_entry *parent_entry, *proc_entry;

	proc_entry = create_proc_entry("rtmark_status", 0, 0);
	if( proc_entry != NULL )
	{
		proc_entry->read_proc = &proc_read_rtmark;
		proc_entry->write_proc = &proc_write_rtmark;
	}

	//proc_entry = create_proc_entry("cleanup", 0, parent_entry);
	proc_entry = create_proc_entry("rtmark_cleanup", 0, 0);
	if( proc_entry != NULL )
	{
		proc_entry->write_proc = &proc_write_cleanup;
	}

#ifdef CONFIG_NETFILTER_XT_RTMARK_WRR
	//proc_entry = create_proc_entry("wrr", 0, parent_entry);
	proc_entry = create_proc_entry("rtmark_wrr", 0, 0);
	if( proc_entry != NULL )
	{
		proc_entry->read_proc = &proc_read_wrr;
		proc_entry->write_proc = &proc_write_wrr;
	}

	//proc_entry = create_proc_entry("ipwrr", 0, parent_entry);
	proc_entry = create_proc_entry("rtmark_ipwrr", 0, 0);
	if( proc_entry != NULL )
	{
		proc_entry->read_proc = &proc_read_ipwrr;
		proc_entry->write_proc = &proc_write_ipwrr;
	}
#endif /* CONFIG_IP_NF_RTMARK_WRR */

	return 0;
}

#ifdef CONFIG_NETFILTER_XT_RTMARK_WRR
static void ip_wrr_checker(unsigned long dummy)
{
	int idx;

	for(idx=0; idx < 256; idx++)
	{
#if 0
		if (ip_wrr_check[idx].rtid)
			printk("->[%d], %ul, %ul, %s \n", 
				idx, jiffies , ip_wrr_check[idx].timeout,
				((long)(jiffies - ip_wrr_check[idx].timeout) > 0) ? "Dead" : "Live");
#endif

		if (ip_wrr_check[idx].rtid && ((long)(jiffies - ip_wrr_check[idx].timeout) > 0))
		{
			ip_wrr_check[idx].rtid = 0;
			ip_wrr_check[idx].timeout = 0;
		}
	}

	mod_timer(&ip_wrr_timer, jiffies + ip_wrr_interval);
}
#endif

static unsigned int target(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_mark_tginfo2 *markinfo = par->targinfo;
	int rtmark = markinfo->mark;
	struct iphdr *iph = ip_hdr(skb);
#ifdef CONFIG_NETFILTER_XT_RTMARK_WRR
	int idx;
#endif


	/* except Twin IP and user-defined LS from WRR */
	if (rtmark > DEFAULT_RTID_TAG) 
	{
		if (!check_internal_subnet(iph->saddr))
			return XT_CONTINUE;

		rtmark -= DEFAULT_RTID_TAG;
#ifdef CONFIG_NETFILTER_XT_RTMARK_WRR
		idx = ntohl(iph->saddr) & 0xff;

		if (wan1_weight[0] && wan2_weight[0])
		{
			if (ip_wrr && ip_wrr_check[idx].rtid)
			{
				rtmark = ip_wrr_check[idx].rtid;
			} 
			else 
			{
				if (wan1_weight[0] > wan1_weight[1])
				{
					wan1_weight[1]++;
					if (wan1_weight[0] == wan1_weight[1])
						wan2_weight[1] = 0;
					rtmark = WAN1_TABLE;
				}
				else
				{
					wan2_weight[1]++;
					if (wan2_weight[0] == wan2_weight[1])
						wan1_weight[1] = 0;
					rtmark = WAN2_TABLE;
				}
			}

			if(ip_wrr)
			{
				ip_wrr_check[idx].rtid = rtmark;
				ip_wrr_check[idx].timeout = jiffies + IP_WRR_TIMEOUT;
			}
		}
#endif
	}

	if (rtmark == WAN1_TABLE && !wan1_status && wan2_status)
		skb->mark = WAN2_TABLE;
	else if (rtmark == WAN2_TABLE && wan1_status && !wan2_status)
		skb->mark = WAN1_TABLE;
	else
		skb->mark = rtmark;

	return XT_CONTINUE;
}

static int checkentry(const struct xt_tgchk_param *par)
{
	return 0;
}

static struct xt_target ipt_mark_reg = { 
	.name   	= "RTMARK", 
	.revision 	= 0,
	.family         = NFPROTO_UNSPEC,
	.checkentry     = checkentry,
	.target         = target,
	.targetsize     = sizeof(struct xt_mark_tginfo2),
	.me         	= THIS_MODULE,
};

static int __init init(void)
{
	if (xt_register_target(&ipt_mark_reg))
		return -EINVAL;
	init_rtmark_proc();

#ifdef CONFIG_NETFILTER_XT_RTMARK_WRR
	memset((char *)ip_wrr_check, 0, sizeof(struct ip_wrr_info) * 256);
#endif

	return 0;
}

static void fini(void)
{
	xt_unregister_target(&ipt_mark_reg);
	del_timer(&ip_wrr_timer);
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");
