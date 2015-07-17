#include "clog_str.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void clog_str_init(clog_str s)
{
    s->data = NULL;
    s->len = 0;
}

void clog_str_init_buf(clog_str s, const clog_ch *buf, clog_len size)
{
    if (!size || !buf)
    {
        clog_str_init(s);
        return;
    }
    s->data = (clog_ch*)buf;
    s->len = size / sizeof(clog_ch);
}

void clog_str_init_c_str(clog_str s, const char *o)
{
    clog_str_init_buf(s, o, (clog_len)strlen(o));
}

void clog_str_substr(clog_str out, const clog_str s, clog_len start, clog_len len)
{
    clog_len rlen = s->len;
    if (start > rlen)
    {
        start = rlen;
    }
    rlen -= start;
    if (len > rlen)
    {
        len = rlen;
    }

    out->data = s->data+start;
    out->len = len;
}

/*
 * __fast_strfind() - adapted from memmem() implementation proposed by Christian Thaeter <ct at pipapo dot org>
 * ref. http://www.sourceware.org/ml/libc-alpha/2007-12/msg00000.html
 */
static const clog_ch *__fast_strfind (
                const clog_ch *haystack, clog_len haystack_len,
                const clog_ch *needle, clog_len needle_len)
{
        const clog_ch *haystack_ = haystack;
        const clog_ch *needle_ = needle;
        unsigned int hash = 0;
        unsigned int hay_hash = 0;
        const clog_ch *last;
        clog_len i;

        if (haystack_len < needle_len)
            return NULL;

        if (needle_len == 0)
            return haystack_;

        /* initialize hashes */
        for (i = needle_len; i; --i)
        {
              hash += (unsigned int)*needle_++;
              hay_hash += (unsigned int)*haystack_++;
        }

        /* iterate over the haystack */
        haystack_ = haystack;
        needle_ = needle;
        last = haystack_+(haystack_len - needle_len + 1);
        for (; haystack_ < last; ++haystack_)
        {
            if (
                    hash == hay_hash &&
                    *haystack_ == *needle_ &&
                    memcmp(haystack_, needle_, needle_len*sizeof(clog_ch)) == 0
                    )
            {
                return haystack_;
            }

          /* roll the hash */
          hay_hash -= (unsigned int)*haystack_;
          hay_hash += (unsigned int)*(haystack_+needle_len);
        }

        return NULL;
}

int clog_str_find(clog_str s, clog_str sub)
{
    const clog_ch *res = __fast_strfind(s->data, s->len, sub->data, sub->len);
    if (res == NULL)
        return -1;
    return (int)(res - s->data);
}

static void __clamp_range(clog_str buf, clog_str s, clog_len *start, clog_len *stop)
{
    *buf = *s;
    if (*start > buf->len)
        *start = buf->len;
    if (*stop < *start)
        *stop = *start;
    if (*stop > buf->len)
        *stop = buf->len;
    buf->len = *stop - *start;
    buf->data += *start;
}

int clog_str_find_ranged(clog_str s, clog_str sub, clog_len start, clog_len stop)
{
    clog_str_st buf;
    int res;
    __clamp_range(&buf, s, &start, &stop);
    res = clog_str_find(&buf, sub);
    if (res < 0) return res;
    return (int)start + res;
}

int clog_str_find_ch(clog_str s, clog_ch ch)
{
    const clog_ch *i = s->data;
    const clog_ch *end = s->data + s->len;
    for (; i!=end; ++i)
    {
        if (*i == ch)
            return (int)(i - s->data);
    }
    return -1;
}

int clog_str_find_ch_ranged(clog_str s, clog_ch ch, clog_len start, clog_len stop)
{
    clog_str_st buf;
    int res;
    __clamp_range(&buf, s, &start, &stop);
    res = clog_str_find_ch(&buf, ch);
    if (res < 0) return res;
    return (int)start + res;
}

void clog_str_split_init(clog_strsplit ctx, clog_str s, clog_ch ch)
{
    ctx->buf = *s;
    ctx->ch = ch;
    ctx->p = s->data;
    clog_str_init(&ctx->tok);
}

int clog_str_split_next(clog_strsplit ctx)
{
    const clog_ch *end = ctx->buf.data + ctx->buf.len;
    while (ctx->p != end)
    {
        if (*ctx->p != ctx->ch)
            break;
        ctx->p++;
    }
    ctx->tok.data = ctx->p;
    while (ctx->p != end)
    {
        if (*ctx->p == ctx->ch)
            break;
        ctx->p++;
    }
    ctx->tok.len = (clog_len)(ctx->p - ctx->tok.data);
    return ctx->tok.len > 0;
}

int clog_str_cmq(clog_str a, clog_str b)
{
    const clog_ch *i = a->data;
    const clog_ch *aend = a->data + a->len;
    const clog_ch *j = b->data;
    const clog_ch *bend = b->data + b->len;
    for (; i!=aend && j!=bend; ++i, ++j)
    {
        if (*i == *j) continue;
        return *i > *j ? 1 : -1;
    }
    if (i != aend) return 1;
    /* i == aend */
    if (j != bend) return -1;
    /* a and b match */
    return 0;
}

int clog_str_eq(clog_str a, clog_str b)
{
    if (a->len != b->len)
        return 0;
    return clog_str_cmq(a, b) == 0;
}




#define CHAR_PRINTABLE  0x01
#define CHAR_EOL        0x02
#define CHAR_ALPHA      0x04
#define CHAR_DIGIT      0x08
#define CHAR_SPACE      0x10
#define CHAR_HEX        0x20
#define CHAR_UPPER      0x40
#define CHAR_LOWER      0x80


static unsigned char _ascii_charmap[] = {
    0x0, /* 0x00 */
    0x0, /* 0x01 */
    0x0, /* 0x02 */
    0x0, /* 0x03 */
    0x0, /* 0x04 */
    0x0, /* 0x05 */
    0x0, /* 0x06 */
    0x0, /* 0x07 */
    0x0, /* 0x08 */
    CHAR_SPACE, /* 0x09 */
    CHAR_EOL | CHAR_SPACE, /* 0x0a */
    CHAR_SPACE, /* 0x0b */
    CHAR_SPACE, /* 0x0c */
    CHAR_EOL | CHAR_SPACE, /* 0x0d */
    0x0, /* 0x0e */
    0x0, /* 0x0f */
    0x0, /* 0x10 */
    0x0, /* 0x11 */
    0x0, /* 0x12 */
    0x0, /* 0x13 */
    0x0, /* 0x14 */
    0x0, /* 0x15 */
    0x0, /* 0x16 */
    0x0, /* 0x17 */
    0x0, /* 0x18 */
    0x0, /* 0x19 */
    0x0, /* 0x1a */
    0x0, /* 0x1b */
    0x0, /* 0x1c */
    0x0, /* 0x1d */
    0x0, /* 0x1e */
    0x0, /* 0x1f */
    CHAR_PRINTABLE | CHAR_SPACE, /* 0x20 */
    CHAR_PRINTABLE, /* 0x21 */
    CHAR_PRINTABLE, /* 0x22 */
    CHAR_PRINTABLE, /* 0x23 */
    CHAR_PRINTABLE, /* 0x24 */
    CHAR_PRINTABLE, /* 0x25 */
    CHAR_PRINTABLE, /* 0x26 */
    CHAR_PRINTABLE, /* 0x27 */
    CHAR_PRINTABLE, /* 0x28 */
    CHAR_PRINTABLE, /* 0x29 */
    CHAR_PRINTABLE, /* 0x2a */
    CHAR_PRINTABLE, /* 0x2b */
    CHAR_PRINTABLE, /* 0x2c */
    CHAR_PRINTABLE, /* 0x2d */
    CHAR_PRINTABLE, /* 0x2e */
    CHAR_PRINTABLE, /* 0x2f */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x30 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x31 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x32 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x33 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x34 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x35 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x36 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x37 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x38 */
    CHAR_PRINTABLE | CHAR_DIGIT | CHAR_HEX, /* 0x39 */
    CHAR_PRINTABLE, /* 0x3a */
    CHAR_PRINTABLE, /* 0x3b */
    CHAR_PRINTABLE, /* 0x3c */
    CHAR_PRINTABLE, /* 0x3d */
    CHAR_PRINTABLE, /* 0x3e */
    CHAR_PRINTABLE, /* 0x3f */
    CHAR_PRINTABLE, /* 0x40 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_UPPER, /* 0x41 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_UPPER, /* 0x42 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_UPPER, /* 0x43 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_UPPER, /* 0x44 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_UPPER, /* 0x45 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_UPPER, /* 0x46 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x47 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x48 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x49 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x4a */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x4b */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x4c */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x4d */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x4e */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x4f */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x50 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x51 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x52 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x53 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x54 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x55 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x56 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x57 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x58 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x59 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_UPPER, /* 0x5a */
    CHAR_PRINTABLE, /* 0x5b */
    CHAR_PRINTABLE, /* 0x5c */
    CHAR_PRINTABLE, /* 0x5d */
    CHAR_PRINTABLE, /* 0x5e */
    CHAR_PRINTABLE, /* 0x5f */
    CHAR_PRINTABLE, /* 0x60 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_LOWER, /* 0x61 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_LOWER, /* 0x62 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_LOWER, /* 0x63 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_LOWER, /* 0x64 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_LOWER, /* 0x65 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_HEX | CHAR_LOWER, /* 0x66 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x67 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x68 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x69 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x6a */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x6b */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x6c */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x6d */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x6e */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x6f */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x70 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x71 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x72 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x73 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x74 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x75 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x76 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x77 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x78 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x79 */
    CHAR_PRINTABLE | CHAR_ALPHA | CHAR_LOWER, /* 0x7a */
    CHAR_PRINTABLE, /* 0x7b */
    CHAR_PRINTABLE, /* 0x7c */
    CHAR_PRINTABLE, /* 0x7d */
    CHAR_PRINTABLE, /* 0x7e */
    0x0 /* 0x7f */
};

DLL_LOCAL unsigned int clog_chattr(clog_ch ch)
{
    static const unsigned int map_size = sizeof(_ascii_charmap) / sizeof(_ascii_charmap[0]);
    if ((unsigned int)ch >= map_size)
        return 0x00;
    return _ascii_charmap[(unsigned int)ch];
}


