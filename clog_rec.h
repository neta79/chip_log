#ifndef __CLOG_REC_H__
#define __CLOG_REC_H__ 1

#include <clog_fwd.h>


struct clog_record_s {
    CLOG_LEVEL   level;
    const char  *logger_name;
    const char  *file_name;
    unsigned int line_no;
    const char  *func_name;
    const char  *msg_begin;
    const char  *msg_end;
    void        *msg_args;
};


#endif /* __CLOG_REC_H__ */

