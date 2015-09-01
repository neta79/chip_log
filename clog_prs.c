#include "clog_prs.h"
#include <stdlib.h>
#include <stdio.h>

static clog_configparser_st _parser;

static clog_configparser_result ST_ROOT(clog_configparser p);
static clog_configparser_result ST_LABEL_TOK(clog_configparser p);
static clog_configparser_result ST_LABEL_TOK_signal_after_start(clog_configparser p);
static clog_configparser_result ST_CONF_PAYLOAD(clog_configparser p);
static clog_configparser_result ST_CONF_PAYLOAD2(clog_configparser p);
static clog_configparser_result ST_KEY(clog_configparser p);
static clog_configparser_result ST_VAL(clog_configparser p);
static clog_configparser_result ST_VAL2(clog_configparser p);
static clog_configparser_result ST_VAL_PARAMS(clog_configparser p);
static clog_configparser_result ST_VAL3(clog_configparser p);
static clog_configparser_result ST_ROOT_close_payload(clog_configparser p);


clog_configparser clog_configparser_init(clog_configlexer l)
{
    clog_configparser p = &_parser;
    p->l = l;
    p->param_level = 0;
    p->got_comma = 0;
    p->stage = ST_ROOT;
    return p;
}

void clog_configparser_free(clog_configparser p)
{
    (void)p;
}

static clog_token_typeid lex_next(clog_configparser p)
{
    return clog_configlexer_next(p->l);
}

/**
 * @brief clog_configparser_ST_ROOT
 * @param p
 *
 * Process lexer tokens at the config root level.
 * Basically, skips remarks and looks for labels.
 *
 * The config entry structure is something like this:
 * @code
 *      label, [label2] = {
 *          k=v
 *          k2=v2
 *          k3=v3
 *          ...
 *      }
 */
static clog_configparser_result ST_ROOT(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_END:
        return CLOG_CONFIG_END;
    case T_NEWLINE:
    case T_REMARK:
    case T_SEMICOLON:
        continue;
    case T_IDENT:
    case T_ASTERISK:
    case T_NUMBER:
        p->stage = ST_LABEL_TOK_signal_after_start;
        return CLOG_CONFIG_LABEL_START;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}


static clog_configparser_result ST_LABEL_TOK_signal_after_start(clog_configparser p)
{
    p->stage = ST_LABEL_TOK;
    return CLOG_CONFIG_LABEL_TOK;
}


/**
 * @brief clog_configparser_ST_LABEL_TOK
 * @param p
 *
 * triggered after detection of a label at root level. Will accept a period (multi-token label),
 * a comma (starts another label) or an equal (signals start of the payload).
 */
static clog_configparser_result ST_LABEL_TOK(clog_configparser p)
{
    int got_period = 0;

    for (;;) switch (lex_next(p))
    {
    case T_NEWLINE:
    case T_REMARK:
    case T_SEMICOLON:
        continue;
    case T_PERIOD:
        if (got_period)
        {
            return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
        }
        got_period = 1;
        continue;
    case T_IDENT:
    case T_ASTERISK:
    case T_NUMBER:
        got_period = 0;
        return CLOG_CONFIG_LABEL_TOK;
    case T_COMMA:
        if (got_period)
        {
            return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
        }
        p->stage = ST_ROOT;
        return CLOG_CONFIG_LABEL_END;
    case T_EQUAL:
        if (got_period)
        {
            return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
        }
        p->stage = ST_CONF_PAYLOAD;
        return CLOG_CONFIG_LABEL_END;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}


/**
 * @brief ST_CONF_PAYLOAD
 * @param p
 *
 * entered after the "=" sign after a label group (root level).
 * This transitional state only expects an open bracket to follow,
 * it is exploited to hard-reject everything non-compliant.
 */
static clog_configparser_result ST_CONF_PAYLOAD(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_NEWLINE:
    case T_REMARK:
    case T_SEMICOLON:
        continue;
    case T_OPENBR:
        p->stage = ST_CONF_PAYLOAD2;
        return CLOG_CONFIG_PAYLOAD_START;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}


/**
 * @brief ST_CONF_PAYLOAD2
 * @param p
 *
 * entered after the open bracket symbol, signifying the effective start of a configuration
 * payload for a previously parsed label group.
 * The configuration payload consists of a number of k=v assignments, each separated by newline or via ";".
 */
static clog_configparser_result ST_CONF_PAYLOAD2(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_NEWLINE:
    case T_REMARK:
    case T_SEMICOLON:
        continue;
    case T_IDENT:
        p->stage = ST_KEY;
        return CLOG_CONFIG_PAYLOAD_KEY;
    case T_CLOSEBR:
        p->stage = ST_ROOT;
        return CLOG_CONFIG_PAYLOAD_END;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}


/**
 * @brief ST_KEY
 * @param p
 *
 * entered after a configuration key (in a key=value specification).
 * This transitional state only expects a following "=" sign.
 * It is exploited to hard-reject anything else.
 */
static clog_configparser_result ST_KEY(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_NEWLINE:
    case T_REMARK:
    case T_SEMICOLON:
        continue;
    case T_EQUAL:
        p->stage = ST_VAL;
        return CLOG_CONFIG_CONTINUE;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}


/**
 * @brief ST_VAL
 * @param p
 *
 * entered after an "=" sign (in a key=value specification).
 * This transitional state expects an identifier to follow.
 * Optionally, T_OPENPAR may follow. In such a case, a comma-separated parameter
 * parsing state is entered.
 */
static clog_configparser_result ST_VAL(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_NEWLINE:
    case T_REMARK:
    case T_SEMICOLON:
        continue;
    case T_IDENT:
    case T_QUOTE:
        p->stage = ST_VAL2;
        return CLOG_CONFIG_PAYLOAD_VALUE_START;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}



/**
 * @brief ST_VAL2
 * @param p
 *
 * entered after k=v assignment (in a key=value specification).
 * This transitional state expects a newline/semicolon to consider the assignment complete.
 * In case T_CLOSEBR follows, the payload ends.
 * In case T_OPENPAR follows instead, a comma-separated parameter
 * parsing state is entered
 */
static clog_configparser_result ST_VAL2(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_REMARK:
        continue;
    case T_NEWLINE:
    case T_SEMICOLON:
        p->stage = ST_CONF_PAYLOAD2;
        return CLOG_CONFIG_PAYLOAD_VALUE_END;
    case T_OPENPAR:
        p->got_comma = 0;
        p->got_param = 0;
        p->param_level ++;
        p->stage = ST_VAL_PARAMS;
        return CLOG_CONFIG_PAYLOAD_PARAM_START;
    case T_CLOSEBR:
        p->stage = ST_ROOT_close_payload;
        return CLOG_CONFIG_PAYLOAD_VALUE_END;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}

/**
 * @brief ST_VAL_PARAMS
 * @param p
 *
 * entered after k=v assignment, if a T_OPENPAR follows.
 * This state allows for comma-separated parameter specification.
 * It is also recursive (parameters may have nested parameters).
 */
static clog_configparser_result ST_VAL_PARAMS(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_REMARK:
    case T_NEWLINE:
    case T_SEMICOLON:
        continue;
    case T_COMMA:
        if (!p->got_param || p->got_comma)
        {
            return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
        }
        p->got_comma = 1;
        continue;
    case T_OPENPAR:
        p->got_comma = 0;
        p->got_param = 0;
        p->param_level ++;
        p->stage = ST_VAL_PARAMS;
        return CLOG_CONFIG_PAYLOAD_PARAM_START;
    case T_CLOSEPAR:
        if (p->got_comma)
        {
            return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
        }
        p->param_level--;
        p->got_param = 1;
        if (p->param_level == 0)
        {
            p->stage = ST_VAL3;
        }
        return CLOG_CONFIG_PAYLOAD_PARAM_END;
    case T_IDENT:
    case T_QUOTE:
        p->got_comma = 0;
        p->got_param = 1;
        return CLOG_CONFIG_PAYLOAD_PARAM;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}



/**
 * @brief ST_VAL3
 * @param p
 *
 * entered after k=v(...) assignment (in a key=value specification).
 * This transitional state expects a newline/semicolon to consider the assignment complete.
 * It is exploited to hard-reject anything else
 */
static clog_configparser_result ST_VAL3(clog_configparser p)
{
    for (;;) switch (lex_next(p))
    {
    case T_REMARK:
        continue;
    case T_NEWLINE:
    case T_SEMICOLON:
        p->stage = ST_CONF_PAYLOAD2;
        return CLOG_CONFIG_PAYLOAD_VALUE_END;
    case T_CLOSEBR:
        p->stage = ST_ROOT_close_payload;
        return CLOG_CONFIG_PAYLOAD_VALUE_END;
    default:
        return CLOG_CONFIG_ERR_UNEXPECTED_TOKEN;
    }
}


/**
 * @brief ST_ROOT_close_payload
 * @param p
 *
 * entered after k=v assignment (in a key=value specification), after a T_CLOSEBR token is found.
 * This transitional state signals the end of payload. No parking steps are taken.
 */
static clog_configparser_result ST_ROOT_close_payload(clog_configparser p)
{
    p->stage = ST_ROOT;
    return CLOG_CONFIG_PAYLOAD_END;
}



clog_configparser_result clog_configparser_next(clog_configparser p)
{
    for (;;)
    {
        clog_configparser_result res;
        assert(p->stage != NULL);
        res = p->stage(p);
        switch (res)
        {
        case CLOG_CONFIG_CONTINUE:
            continue;
        default:
            return res;
        }
    }
}


