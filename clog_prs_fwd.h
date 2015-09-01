#ifndef __CLOG_PRS_FWD_H__
#define __CLOG_PRS_FWD_H__ 1

#include "clog_lex_fwd.h"
#include "clog_cfg.h"

struct clog_configparser_st;
typedef struct clog_configparser_st clog_configparser_st;
typedef struct clog_configparser_st *clog_configparser;

typedef enum {
    CLOG_CONFIG_CONTINUE,
    CLOG_CONFIG_END,
    CLOG_CONFIG_LABEL_START,
    CLOG_CONFIG_LABEL_TOK,
    CLOG_CONFIG_LABEL_END,
    CLOG_CONFIG_PAYLOAD_START,
    CLOG_CONFIG_PAYLOAD_KEY,
    CLOG_CONFIG_PAYLOAD_VALUE_START,
    CLOG_CONFIG_PAYLOAD_PARAM_START,
    CLOG_CONFIG_PAYLOAD_PARAM,
    CLOG_CONFIG_PAYLOAD_PARAM_END,
    CLOG_CONFIG_PAYLOAD_VALUE_END,
    CLOG_CONFIG_PAYLOAD_END,
    CLOG_CONFIG_ERR_UNEXPECTED_TOKEN
} clog_configparser_result;

DLL_LOCAL clog_configparser clog_configparser_init(clog_configlexer l);
DLL_LOCAL void clog_configparser_free(clog_configparser p);
DLL_LOCAL clog_configparser_result clog_configparser_next(clog_configparser p);

#endif /* __CLOG_PRS_FWD_H__ */
