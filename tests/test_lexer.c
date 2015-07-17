#include "clog_str.h"
#include "clog_lex.h"
#include "tests/support.h"
#include <strings.h>

int test_lex_err_unknown_token()
{
    const char config[] = "/cippo";

    configlexer_t *l;
    clog_str_st s;
    clog_str_init_c_str(&s, config);
    l = configlexer_init(&s);

    configlexer_next(l);
    check(configlexer_type(l) == ERR_UNKNOWN_TOKEN);
    check(strcmp(configlexer_typename(l), "ERR_UNKNOWN_TOKEN") == 0);
    check(strlen(configlexer_geterror(l)) > 0);
    check(!configlexer_isover(l));
    configlexer_free(l);
    return 0;
}

int test_lex_err_unterminated_quote()
{
    const char config[] = "tok1\n\rblabla \"unterminated quote";

    configlexer_t *l;
    clog_str_st s;
    clog_str_init_c_str(&s, config);
    l = configlexer_init(&s);

    configlexer_next(l);
    check(configlexer_type(l) == T_IDENT);
    check(!configlexer_isover(l));
    check(strlen(configlexer_geterror(l)) == 0);
    check(configlexer_token(l).data == config);
    check(configlexer_lineno(l) == 1);
    check(configlexer_colno(l) == 1);

    configlexer_next(l);
    check(configlexer_type(l) == T_NEWLINE);
    check(!configlexer_isover(l));

    configlexer_next(l);
    check(configlexer_type(l) == T_IDENT);
    check(!configlexer_isover(l));

    configlexer_next(l);
    check(configlexer_type(l) == ERR_UNTERMINATED_QUOTE);
    check(strcmp(configlexer_typename(l), "ERR_UNTERMINATED_QUOTE") == 0);
    check(configlexer_isover(l));
    check(strlen(configlexer_geterror(l)) > 0);
    check(configlexer_at(l).data == config+13);
    configlexer_free(l);
    return 0;
}

int test_lex_tokens()
{
    const char config[] = "\n"
    "  this_is_an_id3ntifier;"
    "123450 # a comment after a number\n"
    "\" a quote \" 12345not_a_number"
    "\n\r" /* a two-byte newline */
    "(){}.,%$\\="
    ;
    const char s1[] = "this_is_an_id3ntifier";
    const char s2[] = "123450";
    const char s3[] = "12345not_a_number";
    clog_str_st a;
    clog_str_st t;

    configlexer_t *l;
    clog_str_st s;
    clog_str_init_c_str(&s, config);
    l = configlexer_init(&s);

    configlexer_next(l);
    check(configlexer_type(l) == T_NEWLINE);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 1);
    check(configlexer_colno(l) == 1);
    check(strcmp(configlexer_typename(l), "T_NEWLINE") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_IDENT);
    t = configlexer_token(l);
    clog_str_init_c_str(&a, s1);
    check(clog_str_eq(&a, &t))
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 2);
    check(configlexer_colno(l) == 3);
    check(strcmp(configlexer_typename(l), "T_IDENT") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_SEMICOLON);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 2);
    check(configlexer_colno(l) == 24);
    check(strcmp(configlexer_typename(l), "T_SEMICOLON") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_NUMBER);
    t = configlexer_token(l);
    clog_str_init_c_str(&a, s2);
    check(clog_str_eq(&a, &t))
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 2);
    check(configlexer_colno(l) == 25);
    check(strcmp(configlexer_typename(l), "T_NUMBER") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_REMARK);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 2);
    check(configlexer_colno(l) == 32);
    check(configlexer_token(l).len == 25);
    check(strcmp(configlexer_typename(l), "T_REMARK") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_NEWLINE);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 2);
    check(configlexer_colno(l) == 58);
    check(strcmp(configlexer_typename(l), "T_NEWLINE") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_QUOTE);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 3);
    check(configlexer_colno(l) == 1);
    check(configlexer_token(l).len == 9);
    check(strcmp(configlexer_typename(l), "T_QUOTE") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_IDENT);
    t = configlexer_token(l);
    clog_str_init_c_str(&a, s3);
    check(clog_str_eq(&a, &t))
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 3);
    check(configlexer_colno(l) == 13);
    check(strcmp(configlexer_typename(l), "T_IDENT") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_NEWLINE);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 3);
    check(configlexer_colno(l) == 30);
    check(strcmp(configlexer_typename(l), "T_NEWLINE") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_OPENPAR);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 1);
    check(strcmp(configlexer_typename(l), "T_OPENPAR") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_CLOSEPAR);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 2);
    check(strcmp(configlexer_typename(l), "T_CLOSEPAR") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_OPENBR);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 3);
    check(strcmp(configlexer_typename(l), "T_OPENBR") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_CLOSEBR);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 4);
    check(strcmp(configlexer_typename(l), "T_CLOSEBR") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_PERIOD);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 5);
    check(strcmp(configlexer_typename(l), "T_PERIOD") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_COMMA);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 6);
    check(strcmp(configlexer_typename(l), "T_COMMA") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_PERCENT);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 7);
    check(strcmp(configlexer_typename(l), "T_PERCENT") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_DOLLAR);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 8);
    check(strcmp(configlexer_typename(l), "T_DOLLAR") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_BKSLASH);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 9);
    check(strcmp(configlexer_typename(l), "T_BKSLASH") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_EQUAL);
    check(!configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 10);
    check(strcmp(configlexer_typename(l), "T_EQUAL") == 0);

    configlexer_next(l);
    check(configlexer_type(l) == T_END);
    check(configlexer_isover(l));
    check(configlexer_lineno(l) == 4);
    check(configlexer_colno(l) == 11);
    check(strcmp(configlexer_typename(l), "T_END") == 0);

    configlexer_free(l);
    return 0;
}
