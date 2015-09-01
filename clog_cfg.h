#ifndef __CLOG_CFG_H__
#define __CLOG_CFG_H__ 1

#include "clog_lex.h"

typedef enum {
    CLOG_LEVEL_ALL,
    CLOG_LEVEL_TRACE,
    CLOG_LEVEL_DEBUG,
    CLOG_LEVEL_INFO,
    CLOG_LEVEL_NOTICE,
    CLOG_LEVEL_WARN,
    CLOG_LEVEL_ERROR,
    CLOG_LEVEL_ALERT,
    CLOG_LEVEL_EMERG,
    CLOG_LEVEL_FATAL,
    CLOG_LEVEL_OFF
} CLOG_LEVEL;

#endif /* __CLOG_CFG_H__ */
