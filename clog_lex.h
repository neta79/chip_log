#ifndef __CLOG_LEX_H__
#define __CLOG_LEX_H__ 1

#include "clog_str.h"

struct configlexer_t;
typedef struct configlexer_t configlexer_t;

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
    T_NEWLINE               ,
    T_REMARK                ,
    T_END                   ,
    ERR_UNKNOWN_TOKEN       = 0x80,
    ERR_UNTERMINATED_QUOTE  = 0x81
} token_typeid;

DLL_LOCAL configlexer_t *configlexer_init(clog_str config);
DLL_LOCAL void           configlexer_free(configlexer_t *l);
DLL_LOCAL void           configlexer_reset(configlexer_t *l);
DLL_LOCAL int            configlexer_isover(configlexer_t *l);
DLL_LOCAL token_typeid   configlexer_next(configlexer_t *l);
DLL_LOCAL token_typeid   configlexer_type(configlexer_t *l);
DLL_LOCAL const char*    configlexer_typename(configlexer_t *l);
DLL_LOCAL int            configlexer_lineno(configlexer_t *l);
DLL_LOCAL int            configlexer_colno(configlexer_t *l);
DLL_LOCAL clog_str_st    configlexer_token(configlexer_t *l);
DLL_LOCAL clog_str_st    configlexer_at(configlexer_t *l);
DLL_LOCAL int            configlexer_iserror(configlexer_t *l);
DLL_LOCAL const char*    configlexer_geterror(configlexer_t *l);

typedef struct {
    const char *name;
} __clog_logger_t;

#endif /* __CLOG_LEX_H__ */
