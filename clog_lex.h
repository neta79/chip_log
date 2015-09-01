#ifndef __CLOG_LEX_H__
#define __CLOG_LEX_H__ 1

#include "clog_lex_fwd.h"

typedef struct {
    clog_str_st buf;
    clog_token_typeid type;
    unsigned int lineno;
    unsigned int colno;
} token_t;

struct clog_configlexer_st {
    const clog_ch *begin;
    const clog_ch *end;
    const clog_ch *p;
    unsigned int lineno;
    unsigned int colno;
    unsigned int ctr;
    token_t t;
};

typedef struct clog_configlexer_st clog_configlexer_st;


#endif /* __CLOG_LEX_H__ */
