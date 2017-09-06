#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/netfilter_ipv4/ipt_dstmatch.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <asm/uaccess.h>

static DECLARE_MUTEX(dstmatch_mutex);

//#undef printk
#define MAX_BUFFER_SIZE (32 * 1024)

extern u32 ipstr_to_u32 (char *ipstr);


#define MAX_POOL	10
struct dstmatch_entry
{
	unsigned int	sip;
	unsigned int	eip;
};

struct dstmatch_pool
{
	char	name[30];
	struct dstmatch_entry *entry;
	int 	ecount;
	int	refcnt;
};

static struct dstmatch_pool dmentry[MAX_POOL] = {{"",NULL,0}, };

static int make_ip_entries(struct dstmatch_pool *dpool, char *buffer)
{
	char tmpbuf[20];
	int n, tblen = 0;
	struct dstmatch_entry *entry;

	dpool->ecount = 0;
	for ( n=0; n<MAX_BUFFER_SIZE; n++)
	{
		if (buffer[n] == 0x00)
			break;
		if (buffer[n] == '\n')
			dpool->ecount++;
	}

	dpool->entry = kmalloc(sizeof(struct dstmatch_entry)*dpool->ecount, GFP_KERNEL);
	entry = dpool->entry;

	for ( n=0; n<MAX_BUFFER_SIZE; n++)
	{
		if (buffer[n] == 0x00)
			break;
		else if (buffer[n] == '\n')
		{
			if (tblen)
			{
				tmpbuf[tblen] = '\0';
				entry->eip = ipstr_to_u32(tmpbuf);
				if (!entry->sip) entry->sip = entry->eip;
				entry++;
				tblen = 0;
			}

			continue;
		}
		else if (buffer[n] == '~')
		{
			tmpbuf[tblen] = '\0';
			entry->sip = ipstr_to_u32(tmpbuf);
			tblen = 0;
		}
		else
			tmpbuf[tblen++] = buffer[n];
	}

	return 0;
}


static int make_dstmatch_pool(const char *name)
{
	struct file  *fp;
	mm_segment_t orgfs;
	char fname[128], *fbuf;
	int orgfsuid, orgfsgid, retval, n;
	int rc = 1;

	down(&dstmatch_mutex);

	/* check a slot */
	for (n = 0; n < MAX_POOL; n++)
	{
		if (!strcmp(dmentry[n].name, name))
		{
			dmentry[n].refcnt = 1;
			rc = 1;
			goto out;
		}
	}

	/* find a empty slot */
	for (n = 0; n < MAX_POOL; n++)
		if ((dmentry[n].entry == NULL) && (dmentry[n].name[0] == '\0'))
			break;

	if (n >= MAX_POOL) 
	{
		rc = 0;
		goto out;
	}

	// Save uid and gid used for filesystem access.
	// Set user and group to 0 (root)
	orgfsuid = current->fsuid;
	orgfsgid = current->fsgid;
	current->fsuid=current->fsgid = 0; 
	orgfs = get_fs();
	set_fs(KERNEL_DS);


	sprintf(fname,"/etc/dstmatch/%s", name);
	fp = filp_open(fname, O_RDONLY, 0);
	if (IS_ERR(fp))
	{
		printk("--> Error %ld opening %s\n", -PTR_ERR(fname), fname);
		rc = 0;
	}
	else 
	{

		fbuf = kmalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
		if ((fbuf != NULL) && (fp->f_op && fp->f_op->read))
		{
			memset(fbuf, 0x00, MAX_BUFFER_SIZE);
			retval = fp->f_op->read(fp, fbuf, MAX_BUFFER_SIZE, &fp->f_pos);
			if (retval < 0)
			{
				printk("--> Read %s error %d\n", fname, -retval);
				rc = 0;
			}
			else
			{
				make_ip_entries(&dmentry[n], fbuf);
				strcpy(dmentry[n].name, name);
				dmentry[n].refcnt = 0;
			}
			kfree(fbuf);
		}	

		filp_close(fp, NULL);
	}

#if 0
	{
		struct dstmatch_entry *entry;
		int i;

		entry = dmentry[n].entry;
		for (i=0; i<dmentry[n].ecount; i++, entry++)
		{
			printk("[%d] %08x ~ %08x \n", i, entry->sip, entry->eip);
		}
	}
#endif

	set_fs(orgfs);
	current->fsuid = orgfsuid;
	current->fsgid = orgfsgid;
out :
	up(&dstmatch_mutex);

	return rc;
}

static int destroy_dstmatch_pool(const char *name)
{
	int n;

	down(&dstmatch_mutex);

	for (n = 0; n < MAX_POOL; n++)
	{
		if (!strcmp(dmentry[n].name, name))
		{
			if (dmentry[n].refcnt == 0) 
			{
				printk("--- destroy %s tree found.  (%d:%s)\n", name, n, dmentry[n].name);
				if (dmentry[n].entry) kfree(dmentry[n].entry);
				dmentry[n].entry = NULL;
				dmentry[n].name[0] = '\0';
				dmentry[n].ecount = 0;
			}
			dmentry[n].refcnt = 0;
			break;
		}
	}

	up(&dstmatch_mutex);

	return 1;
}

static int binary_search(struct dstmatch_pool *dpool, unsigned int target)
{
	struct dstmatch_entry *entry = dpool->entry;
	int low, mid, high;

	if (!entry) return 0;

	low = 0;
	high = dpool->ecount-1;

	while (low <= high) 
	{
		mid = (high + low) / 2; 

		if (target >  entry[mid].eip)
			low = mid + 1; 
		else if (target <  entry[mid].sip)
			high = mid - 1; 
		else	
		{
			//printk("Got it : %08x ~ %08x\n", entry[mid].sip, entry[mid].eip);
			return 1;
		}
	}
	return 0;
}

static int
match(const struct sk_buff *skb,
      const struct net_device *in,
      const struct net_device *out,
      const void *matchinfo,
      int offset,
      const void *hdr,
      u_int16_t datalen,
      int *hotdrop)
{
	const struct ipt_dstmatch_info *info = matchinfo;
	const struct iphdr *iph = skb->nh.iph;
	int n, rc = 0;

//	printk("--> dstmatch : %s \n", info->name);

	for (n = 0; n < MAX_POOL; n++)
	{
		if (!strcmp(dmentry[n].name, info->name))
		{
			rc = binary_search(&dmentry[n], ntohl(iph->daddr));
			break;
		}
	}

	return rc;
}

static int
checkentry(const char *tablename,
           const struct ipt_ip *ip,
           void *matchinfo,
           unsigned int matchsize,
           unsigned int hook_mask)
{
	const struct ipt_dstmatch_info *info = matchinfo;

	if (matchsize != IPT_ALIGN(sizeof(struct ipt_dstmatch_info)))
		return 0;
	if (info->name[0] == '\0')
		return 0;

	return make_dstmatch_pool(info->name);
}

static void
destroy(void *matchinfo, unsigned int matchsize)
{
	const struct ipt_dstmatch_info *info = matchinfo;
	int n;

	if (matchsize != IPT_ALIGN(sizeof(struct ipt_dstmatch_info)))
		return;

	destroy_dstmatch_pool(info->name);	

#if 0
	for (n = 0; n < MAX_POOL; n++)
		printk("[%d] N:%s T:%s \n", n, dmentry[n].name, (dmentry[n].entry)?"Pool":"Null");
#endif
}

static int proc_read(char *buffer, char **start, off_t offset, int length, int *eof, void *data)
{
	char *p = buffer;
	struct dstmatch_entry *entry = NULL;
	int len, n, k;

	for (n = 0; n < MAX_POOL; n++)
	{
		entry = dmentry[n].entry;
		if (!entry) continue;

		p += sprintf(p, "---------------------\n");
		p += sprintf(p, "%s\n", dmentry[n].name);
		p += sprintf(p, "---------------------\n");
		for (k=0; k<dmentry[n].ecount; k++, entry++)
			p += sprintf(p,"%u.%u.%u.%u ~ %u.%u.%u.%u\n", NIPQUAD(entry->sip), NIPQUAD(entry->eip));
	}

	len = p - buffer;
	if( len <= offset+length ) *eof = 1;
	*start = buffer + offset;
	len -= offset;
	if( len > length ) len = length;
	if( len < 0 ) len = 0;

	return len;
}

static struct ipt_match dstmatch_match
= { { NULL, NULL }, "dstmatch", &match, &checkentry, &destroy, THIS_MODULE };

static int __init init(void)
{
	struct proc_dir_entry *proc_entry;
	int i;

	for (i = 0; i < MAX_POOL; i++)
	{
		dmentry[i].entry = NULL;
		dmentry[i].name[0] = '\0';
		dmentry[i].refcnt = 0;
	}

	proc_entry = create_proc_entry("net/ipt_dstmatch", 0, 0);
	if( proc_entry != NULL )
	{
		proc_entry->read_proc = &proc_read;
	}

	return ipt_register_match(&dstmatch_match);
}

static void __exit fini(void)
{
	ipt_unregister_match(&dstmatch_match);
}

module_init(init);
module_exit(fini);
