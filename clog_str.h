#ifndef __CLOG_STR_H__
#define __CLOG_STR_H__ 1

#include "clog_basics.h"

typedef struct {
    const clog_ch *data;
    clog_len len;
} clog_str_st;

typedef clog_str_st *clog_str;

typedef struct {
    clog_str_st buf;
    clog_str_st tok;
    clog_ch ch;
    const clog_ch *p;
} clog_strsplit_st;

typedef clog_strsplit_st *clog_strsplit;

DLL_LOCAL void clog_str_init(clog_str s);
DLL_LOCAL void clog_str_init_c_str(clog_str s, const char *o);
DLL_LOCAL void clog_str_init_buf(clog_str s, const clog_ch *buf, clog_len size);
DLL_LOCAL void clog_str_substr(clog_str out, const clog_str s, clog_len start, clog_len len);
DLL_LOCAL int clog_str_find(clog_str s, clog_str sub);
DLL_LOCAL int clog_str_find_ranged(clog_str s, clog_str sub, clog_len start, clog_len stop);
DLL_LOCAL int clog_str_find_ch(clog_str s, clog_ch ch);
DLL_LOCAL int clog_str_find_ch_ranged(clog_str s, clog_ch ch, clog_len start, clog_len stop);
DLL_LOCAL void clog_str_split_init(clog_strsplit ctx, clog_str s, clog_ch ch);
DLL_LOCAL int clog_str_split_next(clog_strsplit ctx);
DLL_LOCAL int clog_str_cmq(clog_str a, clog_str b);
DLL_LOCAL int clog_str_eq(clog_str a, clog_str b);

DLL_LOCAL unsigned int clog_chattr(clog_ch ch);

#define clog_isprint(ch) (clog_chattr(ch) & 0x01)
#define clog_iseol(ch)   (clog_chattr(ch) & 0x02)
#define clog_isalpha(ch) (clog_chattr(ch) & 0x04)
#define clog_isdigit(ch) (clog_chattr(ch) & 0x08)
#define clog_isspace(ch) (clog_chattr(ch) & 0x10)
#define clog_ishex(ch)   (clog_chattr(ch) & 0x20)
#define clog_isupper(ch) (clog_chattr(ch) & 0x40)
#define clog_islower(ch) (clog_chattr(ch) & 0x80)

#endif /* __CLOG_STR_H__ */
