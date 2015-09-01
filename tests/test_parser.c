#include "clog_str.h"
#include "clog_lex.h"
#include "clog_prs.h"
#include "clog_cfg.h"
#include "tests/support.h"
#include <strings.h>

#define VERBOSE 1
#ifdef VERBOSE
static const char *event_name(clog_configparser_result i)
{
    static const char* names[] = {
        "CLOG_CONFIG_CONTINUE",
        "CLOG_CONFIG_END",
        "CLOG_CONFIG_LABEL_START",
        "CLOG_CONFIG_LABEL_TOK",
        "CLOG_CONFIG_LABEL_END",
        "CLOG_CONFIG_PAYLOAD_START",
        "CLOG_CONFIG_PAYLOAD_KEY",
        "CLOG_CONFIG_PAYLOAD_VALUE_START",
        "CLOG_CONFIG_PAYLOAD_PARAM_START",
        "CLOG_CONFIG_PAYLOAD_PARAM",
        "CLOG_CONFIG_PAYLOAD_PARAM_END",
        "CLOG_CONFIG_PAYLOAD_VALUE_END",
        "CLOG_CONFIG_PAYLOAD_END",
        "CLOG_CONFIG_ERR_UNEXPECTED_TOKEN"
    };
    static const unsigned int names_size = sizeof(names)/sizeof(names[0]);
    check(i>=0);
    check(i<names_size)
    return names[i];
}

static const char *type_name(clog_token_typeid i)
{
    clog_configlexer_st o;
    o.t.type = i;
    return clog_configlexer_typename(&o);
}
#endif


int test_parser_init()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "\n"
        "# comment2\n"
        "*.\n"
        "explosives, dev.*.fuses = \n"
        "{\n"
        "    sink\n"
        "=\n"
        "concat( #hello \n"
        "stdout, file(\"explosions.log\")) #hello \n"
        "format=blahblah; level=info\n"
        "}\n"
        "\n"
        "# comment3\n"
        "dev.driver.usb.fuses = {\n"
        "    level=debug\n"
        "    format=\"$(level) - in $(filepath), at line $(lineno): %(text)\" # hello\n"
        "}; a={b=c(d,\"e\")}; f={g=h} # inline compact"
        ;
    typedef struct {
        clog_configparser_result signal;
        clog_token_typeid toktype;
    } seq_entry;
    static const seq_entry sequence[] = {
        {CLOG_CONFIG_LABEL_START, T_IDENT},             /* root */
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},               /* root */
        {CLOG_CONFIG_LABEL_END, T_EQUAL},               /* = */
        {CLOG_CONFIG_PAYLOAD_START, T_OPENBR},          /* { */
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},             /* sink */
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},     /* stdout */
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_NEWLINE},     /* \n */
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},             /* level */
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},     /* warning */
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_NEWLINE},     /* \n */
        {CLOG_CONFIG_PAYLOAD_END, T_CLOSEBR},           /* } */

        {CLOG_CONFIG_LABEL_START, T_ASTERISK},
        {CLOG_CONFIG_LABEL_TOK, T_ASTERISK},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_END, T_COMMA},
        {CLOG_CONFIG_LABEL_START, T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_ASTERISK},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_END, T_EQUAL},
        {CLOG_CONFIG_PAYLOAD_START, T_OPENBR},
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_PARAM_START, T_OPENPAR},
        {CLOG_CONFIG_PAYLOAD_PARAM, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_PARAM, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_PARAM_START, T_OPENPAR},
        {CLOG_CONFIG_PAYLOAD_PARAM, T_QUOTE},
        {CLOG_CONFIG_PAYLOAD_PARAM_END, T_CLOSEPAR},
        {CLOG_CONFIG_PAYLOAD_PARAM_END, T_CLOSEPAR},
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_NEWLINE},
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_SEMICOLON},
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_NEWLINE},
        {CLOG_CONFIG_PAYLOAD_END, T_CLOSEBR},
        {CLOG_CONFIG_LABEL_START, T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_END, T_EQUAL},
        {CLOG_CONFIG_PAYLOAD_START, T_OPENBR},
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_NEWLINE},
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_QUOTE},
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_NEWLINE},
        {CLOG_CONFIG_PAYLOAD_END, T_CLOSEBR},
        {CLOG_CONFIG_LABEL_START, T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_END, T_EQUAL},
        {CLOG_CONFIG_PAYLOAD_START, T_OPENBR},
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_PARAM_START, T_OPENPAR},
        {CLOG_CONFIG_PAYLOAD_PARAM, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_PARAM, T_QUOTE},
        {CLOG_CONFIG_PAYLOAD_PARAM_END, T_CLOSEPAR},
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_CLOSEBR},
        {CLOG_CONFIG_PAYLOAD_END, T_CLOSEBR},
        {CLOG_CONFIG_LABEL_START,T_IDENT},
        {CLOG_CONFIG_LABEL_TOK, T_IDENT},
        {CLOG_CONFIG_LABEL_END, T_EQUAL},
        {CLOG_CONFIG_PAYLOAD_START, T_OPENBR},
        {CLOG_CONFIG_PAYLOAD_KEY, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_START, T_IDENT},
        {CLOG_CONFIG_PAYLOAD_VALUE_END, T_CLOSEBR},
        {CLOG_CONFIG_PAYLOAD_END, T_CLOSEBR},
        {CLOG_CONFIG_END, T_END},
    };
    static unsigned int sequence_size = sizeof(sequence) / sizeof(sequence[0]);

    clog_configlexer l;
    clog_configparser p;
    clog_str_st s;
    unsigned int i;

    clog_str_init_c_str(&s, config);
    l = clog_configlexer_init(&s);
    p = clog_configparser_init(l);
    check(p != NULL);

    for (i=0; ; ++i)
    {
        clog_configparser_result res;
        check(i < sequence_size);
        res = clog_configparser_next(p);
#ifdef VERBOSE
        printf("%d: exp=%s,%s got=%s,%s [", (int)i,
               event_name(sequence[i].signal), type_name(sequence[i].toktype),
               event_name(res), clog_configlexer_typename(l));
        fwrite(l->t.buf.data, l->t.buf.len, 1, stdout);
        printf("]\n");
#endif
        check(sequence[i].signal == res);
        check(sequence[i].toktype == clog_configlexer_type(l));
        if (res == CLOG_CONFIG_END)
        {
            break;
        }
    }

    check(i == sequence_size-1);

    clog_configparser_free(p);
    clog_configlexer_free(l);
    return 0;
}

static int run_til_unexpected_token(const char *config)
{
    clog_configlexer l;
    clog_configparser p;
    clog_str_st s;
    int unexpected_token_found = 0;
    int i;

    clog_str_init_c_str(&s, config);
    l = clog_configlexer_init(&s);
    p = clog_configparser_init(l);
    for (i=0; i<1000; ++i)
    {
        switch (clog_configparser_next(p))
        {
        case CLOG_CONFIG_ERR_UNEXPECTED_TOKEN:
            unexpected_token_found = 1;
        case CLOG_CONFIG_END:
            goto _out;
        default:
            continue;
        }
    }

    _out:
    check(unexpected_token_found);
    return 0;
}


int test_parser_bad_token_root()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        ".  # bad token\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_double_period_label()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla..bla = {}  # double token label\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unfinished_label1()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla., ciao = {}  # unfinished label\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unfinished_label2()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla. = {}  # unfinished label\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unexpected_token_in_label()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla.{ = {}  # bad token in label\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unexpected_token_in_payload1()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla.bla = hello  # bad payload\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unexpected_token_in_payload2()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla.bla = { . }  # bad payload\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unexpected_token_after_key()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla.bla = { key.value }  # bad k=v assignment\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unexpected_token_after_key2()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla.bla = { key=.value }  # bad k=v assignment\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_unexpected_token_after_kv()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=stdout\n"
        "    level=warning\n"
        "}\n"
        "bla.bla = { key=value. }  # bad k=v assignment\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_params_double_comma()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=something(param, ,) # double comma\n"
        "}\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_params_unfinished_list()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=something(param,) # unfinished list\n"
        "}\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_params_bad_token()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=something(param, .) # anomalous token\n"
        "}\n"
        ;
    return run_til_unexpected_token(config);
}


int test_parser_params_bad_token_after_closepar()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=something(param). # anomalous token\n"
        "}\n"
        ;
    return run_til_unexpected_token(config);
}

int test_parser_params_comma_start()
{
    static char config[] = ""
        "# comment1\n"
        "root = {\n"
        "    sink=something(, param) # comma start\n"
        "}\n"
        ;
    return run_til_unexpected_token(config);
}

