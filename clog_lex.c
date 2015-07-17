#include "clog_lex.h"
#include <stdio.h>


typedef struct {
    clog_str_st buf;
    token_typeid type;
    unsigned int lineno;
    unsigned int colno;
} token_t;

struct configlexer_t {
    const clog_ch *begin;
    const clog_ch *end;
    const clog_ch *p;
    unsigned int lineno;
    unsigned int colno;
    unsigned int ctr;
    token_t t;
};

static configlexer_t _lexer;



#define is_eof(l) ((l)->p == (l)->end)
#define is_sep(l) (clog_iseol(*(l)->p) || clog_isspace(*(l)->p) || is_eof(l))
#define is_ident_ch(l) (clog_isdigit(*(l)->p) || clog_isalpha(*(l)->p) || *(l)->p == '_')
static void eat_whitespaces(configlexer_t *l);
static void set_tok(token_t *t, configlexer_t *l, token_typeid _t);
static void set_tok_buf(token_t *t, configlexer_t *l, token_typeid _t, const clog_ch *p, clog_len len);
static int lex_ident(token_t *t, configlexer_t *l);
static int lex_number(token_t *t, configlexer_t *l);
static int lex_quote(token_t *t, configlexer_t *l);
static int lex_remark(token_t *t, configlexer_t *l);
static int lexer_next(token_t *t, configlexer_t *l);




configlexer_t *configlexer_init(clog_str config)
{
    configlexer_t *l = &_lexer;
    l->begin = config->data;
    l->end = config->data+config->len;
    configlexer_reset(l);
    return l;
}

void configlexer_free(configlexer_t *l)
{
    /* nop */
    (void)l;
}

void configlexer_reset(configlexer_t *l)
{
    l->p = l->begin;
    l->lineno = 1;
    l->colno = 1;
    l->ctr = 0;
    l->t.type = T_END;
    clog_str_init(&l->t.buf);
    l->t.lineno=0;
    l->t.colno=0;
}

int configlexer_isover(configlexer_t *l)
{
    return l->ctr > 0 &&
        (l->t.type == T_END || configlexer_iserror(l));
}

token_typeid configlexer_next(configlexer_t *l)
{
    l->ctr += (unsigned int)lexer_next(&l->t, l);
    return l->t.type;
}

token_typeid configlexer_type(configlexer_t *l)
{
    return l->t.type;
}

const char* configlexer_typename(configlexer_t *l)
{
    switch (l->t.type)
    {
    case T_IDENT               : return "T_IDENT";
    case T_NUMBER              : return "T_NUMBER";
    case T_QUOTE               : return "T_QUOTE";
    case T_OPENPAR             : return "T_OPENPAR";
    case T_CLOSEPAR            : return "T_CLOSEPAR";
    case T_OPENBR              : return "T_OPENBR";
    case T_CLOSEBR             : return "T_CLOSEBR";
    case T_COMMA               : return "T_COMMA";
    case T_PERIOD              : return "T_PERIOD";
    case T_EQUAL               : return "T_EQUAL";
    case T_SEMICOLON           : return "T_SEMICOLON";
    case T_PERCENT             : return "T_PERCENT";
    case T_DOLLAR              : return "T_DOLLAR";
    case T_BKSLASH             : return "T_BKSLASH";
    case T_NEWLINE             : return "T_NEWLINE";
    case T_REMARK              : return "T_REMARK";
    case T_END                 : return "T_END";
    case ERR_UNKNOWN_TOKEN     : return "ERR_UNKNOWN_TOKEN";
    case ERR_UNTERMINATED_QUOTE: return "ERR_UNTERMINATED_QUOTE";
    default:
        return "UNKNOWN"; /*LCOV_EXCL_LINE*/
    }
}

int configlexer_lineno(configlexer_t *l)
{
    return (int)l->t.lineno;
}
int configlexer_colno(configlexer_t *l)
{
    return (int)l->t.colno;
}
clog_str_st configlexer_token(configlexer_t *l)
{
    return l->t.buf;
}

clog_str_st configlexer_at(configlexer_t *l)
{
    clog_str_st res;
    clog_str_init_buf(&res, l->p, (clog_len)(l->end - l->p));
    return res;
}

int configlexer_iserror(configlexer_t *l)
{
    return l->t.type & 0x80;
}

const char* configlexer_geterror(configlexer_t *l)
{
    switch (l->t.type)
    {
    case ERR_UNKNOWN_TOKEN     : return "unrecognized token";
    case ERR_UNTERMINATED_QUOTE: return "unterminated quote";
    default:
        return "";
    }
}



static void eat_whitespaces(configlexer_t *l)
{
    while (l->p != l->end && (clog_isspace(*l->p) && !clog_iseol(*l->p)))
    {
        l->p++;
        l->colno++;
    }
}


static void set_tok(token_t *t, configlexer_t *l, token_typeid _t)
{
    t->colno = l->colno;
    t->lineno = l->lineno;
    t->type = _t;
}
static void set_tok_buf(token_t *t, configlexer_t *l, token_typeid _t, const clog_ch *p, clog_len len)
{
    t->buf.data = p;
    t->buf.len = len;
    set_tok(t, l, _t);
}


static int lex_ident(token_t *t, configlexer_t *l)
{
    set_tok_buf(t, l, T_IDENT, l->p, 0);
    while (!is_eof(l) && is_ident_ch(l))
    {
        l->p++;
        l->colno++;
        t->buf.len++;
    }
    return t->buf.len > 0;
}

static int lex_number(token_t *t, configlexer_t *l)
{
    configlexer_t save = *l;
    set_tok_buf(t, l, T_NUMBER, l->p, 0);
    while (!is_eof(l) && clog_isdigit(*l->p))
    {
        l->p++;
        l->colno++;
        t->buf.len++;
    }
    if (is_sep(l) || !is_ident_ch(l))
    {
        return t->buf.len > 0;
    }
    *l = save;
    return lex_ident(t, l);
}

static int lex_quote(token_t *t, configlexer_t *l)
{
    configlexer_t save = *l;
    assert(!is_eof(l));
    assert(*l->p == '"');

    l->p++;
    /* token colno will have to point to the quote start character;
     * l->colno is increased after seting up the token */
    set_tok_buf(t, l, T_QUOTE, l->p, 0);
    l->colno++;
    while (!is_eof(l))
    {
        if (*l->p == '"' || clog_iseol(*l->p))
        {
            break;
        }
        l->p++;
        l->colno++;
        t->buf.len++;
    }
    if (is_eof(l) || *l->p != '"')
    {
        *l = save;
        t->type = ERR_UNTERMINATED_QUOTE;
        return 0;
    }
    /* step over trailing quote */
    l->p++;
    l->colno++;
    return 1;
}

static int lex_remark(token_t *t, configlexer_t *l)
{
    assert(!is_eof(l));
    assert(*l->p == '#');

    l->p++;
    /* token colno will have to point to the remark marker sign;
     * l->colno is increased after seting up the token */
    set_tok_buf(t, l, T_REMARK, l->p, 0);
    l->colno++;
    while (!is_eof(l))
    {
        if (clog_iseol(*l->p))
        {
            break;
        }
        l->p++;
        l->colno++;
        t->buf.len++;
    }
    return 1;
}

static int lexer_next(token_t *t, configlexer_t *l)
{
    configlexer_t save;
    eat_whitespaces(l);

    if (is_eof(l))
    {
        set_tok(t, l, T_END);
        return 1;
    }

    save = *l;

    if (clog_isdigit(*l->p))
    {
        lex_number(t, l);
        return !configlexer_iserror(l);
    }

    if (is_ident_ch(l))
    {
        lex_ident(t, l);
        return !configlexer_iserror(l);
    }

    if (clog_iseol(*l->p))
    {
        clog_ch prev = *l->p;
        set_tok_buf(t, l, T_NEWLINE, l->p, 1);
        l->p++;
        l->colno = 1;
        l->lineno++;
        if (!is_eof(l) && clog_iseol(*l->p) && *l->p != prev)
        {
            /* handle two-byte eol cases*/
            l->p++;
        }
        return 1;
    }

    switch (*l->p)
    {
    case '"':
        if (lex_quote(t, l))
        {
            return 1;
        }
        goto _parse_error;
        break;
    case '(':
        set_tok_buf(t, l, T_OPENPAR, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case ')':
        set_tok_buf(t, l, T_CLOSEPAR, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '{':
        set_tok_buf(t, l, T_OPENBR, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '}':
        set_tok_buf(t, l, T_CLOSEBR, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '.':
        set_tok_buf(t, l, T_PERIOD, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case ',':
        set_tok_buf(t, l, T_COMMA, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '=':
        set_tok_buf(t, l, T_EQUAL, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case ';':
        set_tok_buf(t, l, T_SEMICOLON, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '%':
        set_tok_buf(t, l, T_PERCENT, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '\\':
        set_tok_buf(t, l, T_BKSLASH, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '$':
        set_tok_buf(t, l, T_DOLLAR, l->p, 1);
        l->p++;
        l->colno++;
        return 1;
    case '#':
        lex_remark(t, l);
        return 1;
    default:
        break;
    }

_parse_error:
    if (!configlexer_iserror(l))
    {
        /* can't figure out next token */
        set_tok(t, l, ERR_UNKNOWN_TOKEN);
    }

    return 0;
}
