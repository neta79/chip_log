#include "clog_lex.h"
#include <stdio.h>



int main()
{
    const char config[] = "        \n"
        "root.level = name                 \n"
        "root.format = fmtstring           \n"
        "root.sink = sinkname              \n"
        "name1, name2 = { level=name;            \n"
        "           format=\"quoted fmtstring\"\n"
        "           sink=sinkname\n"
        "}"
        "name3.name4.level=name;            \n"
        "name3.name4.format=fmtstring;      \n"
        "name3.name4.sink=appender_name(param1, \" param2\");         \n"
        "       }                          \n"
        "                                  \n"
        "       bla, yada, = {}            \n"
    ;

    configlexer_t *l;
    clog_str_st s;
    clog_str_init_c_str(&s, config);
    l = configlexer_init(&s);
    printf("slen=%d\n", s.len);

    do {
        configlexer_next(l);
        printf("t.lineno=%d, t.colno=%d, t.type=%s, ", configlexer_lineno(l), configlexer_colno(l), configlexer_typename(l));
        printf("t.data = \"");
        fwrite(configlexer_token(l).data, 1, configlexer_token(l).len, stdout);
        printf("\"\n");


    } while (!configlexer_isover(l));

    return 0;
}
