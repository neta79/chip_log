#ifndef __CLOG_FWD_H__
#define __CLOG_FWD_H__ 1

typedef enum {
    CLOG_LEVEL_ALL    ,
    CLOG_LEVEL_TRACE  ,
    CLOG_LEVEL_DEBUG  ,
    CLOG_LEVEL_INFO   ,
    CLOG_LEVEL_NOTICE ,
    CLOG_LEVEL_WARNING,
    CLOG_LEVEL_ERROR  ,
    CLOG_LEVEL_ALERT  ,
    CLOG_LEVEL_FATAL  ,
    CLOG_LEVEL_NONE
} CLOG_LEVEL;


struct clog_record_s;
typedef struct clog_record_s *clog_record;

struct clog_logger_s;
typedef struct clog_logger_s *clog_logger;

struct clog_handler_s;
typedef struct clog_handler_s *clog_handler;

#endif /* __CLOG_FWD_H__ */
