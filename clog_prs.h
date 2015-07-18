#ifndef __CLOG_PARSE_H__
#define __CLOG_PARSE_H__ 1

#include "clog_str.h"

struct configparser_t;
typedef struct configparser_t configparser_t;

DLL_LOCAL void configparser_init(configparser_t *p, clog_str buf);
DLL_LOCAL void configparser_free(configparser_t *p);

#endif /* __CLOG_PARSE_H__ */
