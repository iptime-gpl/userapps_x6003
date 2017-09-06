#ifndef _IPT_DSTMATCH_H
#define _IPT_DSTMATCH_H

#define IPT_DSTMATCH_NAME_LEN	32 

struct xt_dstmatch_info {
    char name[IPT_DSTMATCH_NAME_LEN];
    u_int32_t options;
    u_int32_t invert;
};

#endif
