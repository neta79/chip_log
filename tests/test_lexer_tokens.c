#include "clog_str.h"
#include "clog_lex.h"
#include "tests/support.h"
#include <strings.h>

static const char config[] = "\n"
            "  this_is_an_id3ntifier;"
            "123450 # a comment after a number\n"
            "\" a quote \" 12345not_a_number"
            "\n\r" /* a two-byte newline */
            "(){}.,%$*\\="
            ;
static const char s1[] = "this_is_an_id3ntifier";
static const char s2[] = "123450";
static const char s3[] = "12345not_a_number";
static clog_str_st a;
static clog_str_st t;

static clog_configlexer l;
static clog_str_st s;

static int walk_tokens(int n)
{
    int i;
    clog_str_init_c_str(&s, config);
    l = clog_configlexer_init(&s);
    for (i=0; i<n && clog_configlexer_next(l) != T_END; ++i);
    return i == n;
}


int test_lex_T_NEWLINE1()
{
    walk_tokens(1);
    check(clog_configlexer_type(l) == T_NEWLINE);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 1);
    check(clog_configlexer_colno(l) == 1);
    check(strcmp(clog_configlexer_typename(l), "T_NEWLINE") == 0);
    return 0;
}

int test_lex_T_IDENT1()
{
    walk_tokens(2);
    check(clog_configlexer_type(l) == T_IDENT);
    t = clog_configlexer_token(l);
    clog_str_init_c_str(&a, s1);
    check(clog_str_eq(&a, &t))
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 2);
    check(clog_configlexer_colno(l) == 3);
    check(strcmp(clog_configlexer_typename(l), "T_IDENT") == 0);
    return 0;
}

int test_lex_T_SEMICOLON()
{
    walk_tokens(3);
    check(clog_configlexer_type(l) == T_SEMICOLON);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 2);
    check(clog_configlexer_colno(l) == 24);
    check(strcmp(clog_configlexer_typename(l), "T_SEMICOLON") == 0);
    return 0;
}

int test_lex_T_NUMBER()
{
    walk_tokens(4);
    check(clog_configlexer_type(l) == T_NUMBER);
    t = clog_configlexer_token(l);
    clog_str_init_c_str(&a, s2);
    check(clog_str_eq(&a, &t))
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 2);
    check(clog_configlexer_colno(l) == 25);
    check(strcmp(clog_configlexer_typename(l), "T_NUMBER") == 0);
    return 0;
}

int test_lex_T_REMARK()
{
    walk_tokens(5);
    check(clog_configlexer_type(l) == T_REMARK);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 2);
    check(clog_configlexer_colno(l) == 32);
    check(clog_configlexer_token(l).len == 25);
    check(strcmp(clog_configlexer_typename(l), "T_REMARK") == 0);
    return 0;
}

int test_lex_T_QUOTE()
{
    walk_tokens(7);
    check(clog_configlexer_type(l) == T_QUOTE);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 3);
    check(clog_configlexer_colno(l) == 1);
    check(clog_configlexer_token(l).len == 9);
    check(strcmp(clog_configlexer_typename(l), "T_QUOTE") == 0);
    return 0;
}

int test_lex_T_IDENT2()
{
    walk_tokens(8);
    check(clog_configlexer_type(l) == T_IDENT);
    t = clog_configlexer_token(l);
    clog_str_init_c_str(&a, s3);
    check(clog_str_eq(&a, &t))
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 3);
    check(clog_configlexer_colno(l) == 13);
    check(strcmp(clog_configlexer_typename(l), "T_IDENT") == 0);
    return 0;
}

int test_lex_T_NEWLINE2()
{
    walk_tokens(9);
    check(clog_configlexer_type(l) == T_NEWLINE);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 3);
    check(clog_configlexer_colno(l) == 30);
    check(strcmp(clog_configlexer_typename(l), "T_NEWLINE") == 0);
    return 0;
}

int test_lex_T_OPENPAR()
{
    walk_tokens(10);
    check(clog_configlexer_type(l) == T_OPENPAR);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 1);
    check(strcmp(clog_configlexer_typename(l), "T_OPENPAR") == 0);
    return 0;
}

int test_lex_T_CLOSEPAR()
{
    walk_tokens(11);
    check(clog_configlexer_type(l) == T_CLOSEPAR);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 2);
    check(strcmp(clog_configlexer_typename(l), "T_CLOSEPAR") == 0);
    return 0;
}

int test_lex_T_OPENBR()
{
    walk_tokens(12);
    check(clog_configlexer_type(l) == T_OPENBR);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 3);
    check(strcmp(clog_configlexer_typename(l), "T_OPENBR") == 0);
    return 0;
}

int test_lex_T_CLOSEBR()
{
    walk_tokens(13);
    check(clog_configlexer_type(l) == T_CLOSEBR);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 4);
    check(strcmp(clog_configlexer_typename(l), "T_CLOSEBR") == 0);
    return 0;
}

int test_lex_T_PERIOD()
{
    walk_tokens(14);
    check(clog_configlexer_type(l) == T_PERIOD);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 5);
    check(strcmp(clog_configlexer_typename(l), "T_PERIOD") == 0);
    return 0;
}

int test_lex_T_COMMA()
{
    walk_tokens(15);
    check(clog_configlexer_type(l) == T_COMMA);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 6);
    check(strcmp(clog_configlexer_typename(l), "T_COMMA") == 0);
    return 0;
}

int test_lex_T_PERCENT()
{
    walk_tokens(16);
    check(clog_configlexer_type(l) == T_PERCENT);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 7);
    check(strcmp(clog_configlexer_typename(l), "T_PERCENT") == 0);
    return 0;
}

int test_lex_T_DOLLAR()
{
    walk_tokens(17);
    check(clog_configlexer_type(l) == T_DOLLAR);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 8);
    check(strcmp(clog_configlexer_typename(l), "T_DOLLAR") == 0);
    return 0;
}

int test_lex_T_ASTERISK()
{
    walk_tokens(18);
    check(clog_configlexer_type(l) == T_ASTERISK);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 9);
    check(strcmp(clog_configlexer_typename(l), "T_ASTERISK") == 0);
    return 0;
}

int test_lex_T_BKSLASH()
{
    walk_tokens(19);
    check(clog_configlexer_type(l) == T_BKSLASH);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 10);
    check(strcmp(clog_configlexer_typename(l), "T_BKSLASH") == 0);
    return 0;
}

int test_lex_T_EQUAL()
{
    walk_tokens(20);
    check(clog_configlexer_type(l) == T_EQUAL);
    check(!clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 11);
    check(strcmp(clog_configlexer_typename(l), "T_EQUAL") == 0);
    return 0;
}

int test_lex_T_END()
{
    walk_tokens(20);
    check(clog_configlexer_next(l) == T_END);
    check(clog_configlexer_next(l) == T_END);

    check(clog_configlexer_type(l) == T_END);
    check(clog_configlexer_isover(l));
    check(clog_configlexer_lineno(l) == 4);
    check(clog_configlexer_colno(l) == 12);
    check(strcmp(clog_configlexer_typename(l), "T_END") == 0);

    clog_configlexer_free(l);
    return 0;
}
