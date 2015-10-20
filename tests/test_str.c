#include "clog_str.h"
#include "clog_lex.h"
#include "tests/support.h"
#include <strings.h>

int test_str_cmp_eq()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = "abcd";
    static const clog_ch s2[] = "abcd";
    clog_str_init_c_str(&a, s1);
    clog_str_init_c_str(&b, s2);
    check(clog_str_cmq(&a, &b) == 0);
    check(clog_str_cmq(&b, &a) == 0);
    return 0;
}

int test_str_cmp_ne1()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = "abcd";
    static const clog_ch s2[] = "abdd";
    static const clog_ch s1a[] = "abcd";
    static const clog_ch s2a[] = "abcD";
    static const clog_ch s1b[] = "abcd";
    static const clog_ch s2b[] = "Abcd";
    clog_str_init_c_str(&a, s1);
    clog_str_init_c_str(&b, s2);
    check(clog_str_cmq(&a, &b) == -1);
    check(clog_str_cmq(&b, &a) == +1);
    clog_str_init_c_str(&a, s1a);
    clog_str_init_c_str(&b, s2a);
    check(clog_str_cmq(&a, &b) == +1);
    check(clog_str_cmq(&b, &a) == -1);
    clog_str_init_c_str(&a, s1b);
    clog_str_init_c_str(&b, s2b);
    check(clog_str_cmq(&a, &b) == +1);
    check(clog_str_cmq(&b, &a) == -1);
    return 0;
}

int test_str_cmp_ne2()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = "abcd";
    static const clog_ch s2[] = "abc";
    clog_str_init_c_str(&a, s1);
    clog_str_init_c_str(&b, s2);
    check(clog_str_cmq(&a, &b) == +1);
    check(clog_str_cmq(&b, &a) == -1);
    return 0;
}

int test_str_eq()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = "87yn8n7sey98 es7ys8e7s877y";
    static const clog_ch s2[] = "87yn8n7sey98 es7ys8e7s877y";
    clog_str_init_c_str(&a, s1);
    clog_str_init_c_str(&b, s2);
    check(clog_str_eq(&a, &a));
    check(clog_str_eq(&b, &b));
    check(clog_str_eq(&a, &b));
    check(clog_str_eq(&b, &a));
    return 0;
}

int test_str_find()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = "abcdef0123456";
    static const clog_ch s2a[] = "abc";
    static const clog_ch s2b[] = "456";
    static const clog_ch s2c[] = "a";
    static const clog_ch s2d[] = "6";
    static const clog_ch s2e[] = "ef01";
    static const clog_ch s2f[] = "hello";
    static const clog_ch nope[] = "";
    clog_str_init_c_str(&a, s1);

    clog_str_init_c_str(&b, s2a);
    check(clog_str_find(&a, &b) == 0);
    clog_str_init_c_str(&b, s2b);
    check(clog_str_find(&a, &b) == 10);
    clog_str_init_c_str(&b, s2c);
    check(clog_str_find(&a, &b) == 0);
    clog_str_init_c_str(&b, s2d);
    check(clog_str_find(&a, &b) == 12);
    clog_str_init_c_str(&b, s2e);
    check(clog_str_find(&a, &b) == 4);
    clog_str_init_c_str(&b, s2f);
    check(clog_str_find(&a, &b) == -1);

    clog_str_init_c_str(&b, s2e);
    check(clog_str_find_ranged(&a, &b, 5, a.len) == -1);
    check(clog_str_find_ranged(&a, &b, 4, a.len) == 4);

    clog_str_init_c_str(&b, nope);
    check(clog_str_find_ranged(&a, &b, 0, a.len) == 0);
    check(clog_str_find_ranged(&a, &b, 1, a.len) == 1);

    /* test range clamping */
    clog_str_init_c_str(&b, s2e);
    check(clog_str_find_ranged(&a, &b, 1000, a.len) == -1);
    check(clog_str_find_ranged(&a, &b, a.len, 10) == -1);
    check(clog_str_find_ranged(&a, &b, 0, 1000) == 4);
    return 0;
}

int test_str_findch()
{
    clog_str_st a;
    static const clog_ch s1[] = "abcdef0123456";
    clog_str_init_c_str(&a, s1);

    check(clog_str_find_ch_ranged(&a, 'a', 0, a.len) == 0);
    check(clog_str_find_ch_ranged(&a, '0', 0, a.len) == 6);
    check(clog_str_find_ch_ranged(&a, '6', 0, a.len) == 12);
    check(clog_str_find_ch_ranged(&a, 'b', 1, a.len) == 1);
    check(clog_str_find_ch_ranged(&a, 'b', 2, a.len) == -1);
    return 0;
}

int test_str_init()
{
    clog_str_st s;
    clog_str_init(&s);
    check(s.data == NULL);
    check(s.len == 0);
    return 0;
}

int test_str_init_buf()
{
    clog_str_st s;
    static const clog_ch str[] = "cippalippa";
    clog_str_init_buf(&s, str, (clog_len)strlen(str));
    check(s.data == &str[0]);
    check(s.len == strlen(str));
    return 0;
}

int test_str_init_buf_null()
{
    clog_str_st s;
    clog_str_init_buf(&s, NULL, 1);
    check(s.data == NULL);
    check(s.len == 0);
    return 0;
}

int test_str_init_cstr()
{
    clog_str_st s;
    static const clog_ch str[] = "cippalippa";
    clog_str_init_c_str(&s, str);
    check(s.data == &str[0]);
    check(s.len == strlen(str));
    return 0;
}

int test_str_neq()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = "87yn8n7sey98 es7ys8e7s877y";
    static const clog_ch s2[] = "87yn8n7sey98 es7ys8e7s877y_";
    clog_str_init_c_str(&a, s1);
    clog_str_init_c_str(&b, s2);
    check(!clog_str_eq(&a, &b));
    check(!clog_str_eq(&b, &a));
    return 0;
}

int test_str_splitch()
{
    clog_str_st a;
    clog_strsplit_st sp;
    static const clog_ch s1[] = "..01.002..3...";
    clog_str_init_c_str(&a, s1);

    clog_str_split_init(&sp, &a, '.');
    check(clog_str_split_next(&sp));
    check(sp.tok.data == a.data+2);
    check(sp.tok.len == 2);
    check(clog_str_split_next(&sp));
    check(sp.tok.data == a.data+5);
    check(sp.tok.len == 3);
    check(clog_str_split_next(&sp));
    check(sp.tok.data == a.data+10);
    check(sp.tok.len == 1);
    check(!clog_str_split_next(&sp));
    return 0;
}

int test_str_substr()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = "abcdef";
    clog_str_init_c_str(&a, s1);
    clog_str_substr(&b, &a, 0, 1000);
    check(clog_str_eq(&a, &b));
    check(b.data == a.data);
    clog_str_substr(&b, &a, 1000, 1000);
    check(b.len == 0);
    check(b.data == a.data + a.len);
    return 0;
}

int test_str_trim()
{
    clog_str_st a;
    clog_str_st b;
    static const clog_ch s1[] = " \t\r\n  . abcdef .\t \t";
    static const clog_ch s2[] = ". abcdef .";
    clog_str_init_c_str(&a, s1);
    clog_str_init_c_str(&b, s2);
    clog_str_trim_whitespaces(&a);
    printf("[");
    fwrite(a.data, a.len, 1, stdout);
    printf("]\n");
    check(clog_str_eq(&a, &b));
    return 0;
}

int test_str_wildmat()
{
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "ciccio.pasticcio";
        static const clog_ch s2[] = "ciccio.pasticcio";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "ciccio.";
        static const clog_ch s2[] = "ciccio.pasticcio";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(!clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "ciccio.";
        static const clog_ch s2[] = "ciccio.*";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "ciccio.";
        static const clog_ch s2[] = "ciccio\\.";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "ciccio\\";
        static const clog_ch s2[] = "ciccio\\";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "ciccia";
        static const clog_ch s2[] = "ciccio";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(!clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "cicciaio";
        static const clog_ch s2[] = "cicci?io";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "cicciaio";
        static const clog_ch s2[] = "cicci*io";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "cicciaio";
        static const clog_ch s2[] = "cicci**io";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "cicciaio";
        static const clog_ch s2[] = "cicci*ia";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(!clog_str_wildmat(&a, &b));
    }
    {
        clog_str_st a;
        clog_str_st b;
        static const clog_ch s1[] = "cicciaio";
        static const clog_ch s2[] = "cicci*";
        clog_str_init_c_str(&a, s1);
        clog_str_init_c_str(&b, s2);
        check(clog_str_wildmat(&a, &b));
    }
    return 0;
}
