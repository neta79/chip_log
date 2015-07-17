#ifndef __CLOG_LOG_H__
#define __CLOG_LOG_H__ 1

#include <clog_fwd.h>

struct clog_logger_s {
    unsigned int  config_key;
    CLOG_LEVEL    level;
    const char   *name;
};

#endif /* __CLOG_LOG_H__ */
