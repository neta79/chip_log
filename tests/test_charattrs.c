#include "clog_str.h"
#include "clog_lex.h"
#include "tests/support.h"
#include <strings.h>

int test_charattrs() {
    check(clog_isalpha('a'));
    check(!clog_isalpha('0'));
    check(clog_isdigit('0'));
    check(!clog_isdigit('a'));
    check(clog_iseol('\n'));
    check(clog_iseol('\r'));
    check(!clog_iseol('a'));
    check(clog_ishex('0'));
    check(clog_ishex('a'));
    check(clog_ishex('A'));
    check(clog_ishex('f'));
    check(clog_ishex('F'));
    check(!clog_ishex('h'));
    check(clog_islower('a'));
    check(!clog_islower('A'));
    check(!clog_islower('0'));
    check(clog_isupper('A'));
    check(!clog_isupper('a'));
    check(!clog_isupper('0'));
    check(clog_isprint('a'));
    check(clog_isprint('A'));
    check(clog_isprint(','));
    check(clog_isprint('_'));
    check(!clog_isprint((clog_ch)3));
    check(clog_isspace(' '));
    check(clog_isspace('\t'));
    check(clog_isspace('\r'));
    check(clog_isspace('\n'));
    check(!clog_isspace('_'));
    check(clog_chattr((clog_ch)200) == 0);
    return 0;
}
