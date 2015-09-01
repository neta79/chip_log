#ifndef __CLOG_PRS_H__
#define __CLOG_PRS_H__ 1

#include "clog_lex.h"
#include "clog_prs_fwd.h"

typedef clog_configparser_result (*clog_configparser_stage) (clog_configparser p);

struct clog_configparser_st {
    clog_configlexer l;
    clog_configparser_stage stage;
    int param_level;
    int got_comma;
    int got_param;
};



#endif /* __CLOG_PRS_H__ */
