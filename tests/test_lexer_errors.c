#include "clog_str.h"
#include "clog_lex.h"
#include "tests/support.h"
#include <strings.h>

int test_lex_err_unknown_token()
{
    const char config[] = "/cippo";

    clog_configlexer l;
    clog_str_st s;
    clog_str_init_c_str(&s, config);
    l = clog_configlexer_init(&s);

    clog_configlexer_next(l);
    check(clog_configlexer_type(l) == ERR_UNKNOWN_TOKEN);
    check(strcmp(clog_configlexer_typename(l), "ERR_UNKNOWN_TOKEN") == 0);
    check(strlen(clog_configlexer_geterror(l)) > 0);
    check(!clog_configlexer_isover(l));
    clog_configlexer_free(l);
    return 0;
}

int test_lex_err_unterminated_quote()
{
    const char config[] = "tok1\n\rblabla \"unterminated quote";

    clog_configlexer l;
    clog_str_st s;
    clog_str_init_c_str(&s, config);
    l = clog_configlexer_init(&s);

    clog_configlexer_next(l);
    check(clog_configlexer_type(l) == T_IDENT);
    check(!clog_configlexer_isover(l));
    check(strlen(clog_configlexer_geterror(l)) == 0);
    check(clog_configlexer_token(l).data == config);
    check(clog_configlexer_lineno(l) == 1);
    check(clog_configlexer_colno(l) == 1);

    clog_configlexer_next(l);
    check(clog_configlexer_type(l) == T_NEWLINE);
    check(!clog_configlexer_isover(l));

    clog_configlexer_next(l);
    check(clog_configlexer_type(l) == T_IDENT);
    check(!clog_configlexer_isover(l));

    clog_configlexer_next(l);
    check(clog_configlexer_type(l) == ERR_UNTERMINATED_QUOTE);
    check(strcmp(clog_configlexer_typename(l), "ERR_UNTERMINATED_QUOTE") == 0);
    check(clog_configlexer_isover(l));
    check(strlen(clog_configlexer_geterror(l)) > 0);
    check(clog_configlexer_at(l).data == config+13);
    clog_configlexer_free(l);
    return 0;
}

