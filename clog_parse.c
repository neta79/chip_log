#include "clog_parse.h"
#include "clog_lex.h"

struct configparser_t {
    clog_str buf;
    configlexer_t *l;
};

void configparser_init(configparser_t *p, clog_str buf)
{
    p->buf = buf;
    p->l = configlexer_init(buf);
}

void configparser_free(configparser_t *p)
{
    configlexer_free(p->l);
}
