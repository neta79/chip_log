#ifndef __CLOG_LEX_FWD_H__
#define __CLOG_LEX_FWD_H__ 1

#include "clog_str.h"

struct clog_configlexer_st;
typedef struct clog_configlexer_st *clog_configlexer;

typedef enum {
    T_IDENT                 ,
    T_NUMBER                ,
    T_QUOTE                 ,
    T_OPENPAR               ,
    T_CLOSEPAR              ,
    T_OPENBR                ,
    T_CLOSEBR               ,
    T_COMMA                 ,
    T_PERIOD                ,
    T_EQUAL                 ,
    T_SEMICOLON             ,
    T_PERCENT               ,
    T_DOLLAR                ,
    T_BKSLASH               ,
    T_ASTERISK              ,
    T_NEWLINE               ,
    T_REMARK                ,
    T_END                   ,
    ERR_UNKNOWN_TOKEN       = 0x80,
    ERR_UNTERMINATED_QUOTE  = 0x81
} clog_token_typeid;

DLL_LOCAL clog_configlexer    clog_configlexer_init(clog_str config);
DLL_LOCAL void                clog_configlexer_free(clog_configlexer l);
DLL_LOCAL void                clog_configlexer_reset(clog_configlexer l);
DLL_LOCAL int                 clog_configlexer_isover(clog_configlexer l);
DLL_LOCAL clog_token_typeid   clog_configlexer_next(clog_configlexer l);
DLL_LOCAL clog_token_typeid   clog_configlexer_type(clog_configlexer l);
DLL_LOCAL const char*         clog_configlexer_typename(clog_configlexer l);
DLL_LOCAL int                 clog_configlexer_lineno(clog_configlexer l);
DLL_LOCAL int                 clog_configlexer_colno(clog_configlexer l);
DLL_LOCAL clog_str_st         clog_configlexer_token(clog_configlexer l);
DLL_LOCAL clog_str_st         clog_configlexer_at(clog_configlexer l);
DLL_LOCAL int                 clog_configlexer_iserror(clog_configlexer l);
DLL_LOCAL const char*         clog_configlexer_geterror(clog_configlexer l);

typedef struct {
    const char *name;
} __clog_logger_t;

#endif /* __CLOG_LEX_FWD_H__ */
