#ifndef __CLOG_HDL_H__
#define __CLOG_HDL_H__ 1

#include <clog_fwd.h>

struct clog_handler_s {
    struct clog_handler_s *next;
    const char *format_begin;
    unsigned int format_len;
};

#endif /* __CLOG_HDL_H__ */
