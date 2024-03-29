/*
 * Copyright (c) 2019 askmeaboutloom
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <rouse_config.h>
#include "common.h"
#include "parse.h"

/* Windows not supporting isnan and isinf properly. */
#ifdef _WIN32
#   define IS_NORMAL(X) (!isnan((float)(X)) && !isinf((float)(X)))
#else
#   define IS_NORMAL(X) (!isnan(X) && !isinf(X))
#endif


R_Parse R_parse(const char *title, R_ParseReadFn read, R_UserData user,
                int bufsize, unsigned char buffer[static bufsize])
{
    return (R_Parse){
        R_MAGIC_INIT(R_Parse) title, read, user, bufsize, buffer, 0};
}

int R_parse_from_file(int size, unsigned char *out, R_UserData user)
{
    FILE *fp = user.data;
    return R_size2int(fread(out, sizeof(*out), R_int2size(size), fp));
}


int R_parse_read_into(R_Parse *parse, int size, unsigned char buf[static size])
{
    R_MAGIC_CHECK(R_Parse, parse);
    int got = parse->read(size, buf, parse->user);
    if (got != size) {
        R_PARSE_DIE(parse, "bad read, want %d bytes but got %d", size, got);
    }
    return got;
}

void R_parse_read_bytes_with_extra_space(R_Parse *parse, int nbytes, int extra)
{
    R_MAGIC_CHECK(R_Parse, parse);
    int needed = nbytes + extra;
    if (parse->bufsize < needed) {
        R_PARSE_DIE(parse, "out of buffer space, want %d bytes but got %d",
                    needed, parse->bufsize);
    }
    parse->pos += R_parse_read_into(parse, nbytes, parse->buffer);
}

void R_parse_read_bytes(R_Parse *parse, int nbytes)
{
    R_parse_read_bytes_with_extra_space(parse, nbytes, 0);
}

unsigned char R_parse_read_uchar(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    R_parse_read_bytes(parse, 1);
    return parse->buffer[0];
}

char R_parse_read_char(R_Parse *parse)
{
    return R_uchar2char(R_parse_read_uchar(parse));
}

unsigned short R_parse_read_ushort(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    R_parse_read_bytes(parse, 2);
    unsigned short lo = parse->buffer[0];
    unsigned short hi = parse->buffer[1];
    return R_int2ushort(lo + (hi << 8));
}

unsigned int R_parse_read_uint(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    R_parse_read_bytes(parse, 4);
    unsigned int a = parse->buffer[0];
    unsigned int b = parse->buffer[1];
    unsigned int c = parse->buffer[2];
    unsigned int d = parse->buffer[3];
    return a + (b << 8u) + (c << 16u) + (d << 24u);
}


float R_parse_read_float(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    unsigned char len = R_parse_read_uchar(parse);
    int           at  = parse->pos;
    R_parse_read_bytes_with_extra_space(parse, len, 1);
    parse->buffer[len] = '\0';

    errno = 0;
    char  *end;
    char  *buffer = (char *)parse->buffer;
    float value   = strtof(buffer, &end);
    if (end == buffer + len && IS_NORMAL(value)) {
        return value;
    }
    else {
        const char *error = errno == 0 ? "invalid input" : strerror(errno);
        R_PARSE_DIE_AT(parse, at, "can't parse '%s' into a float: %s",
                       buffer, error);
    }
}

double R_parse_read_double(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    unsigned char len = R_parse_read_uchar(parse);
    int           at  = parse->pos;
    R_parse_read_bytes_with_extra_space(parse, len, 1);
    parse->buffer[len] = '\0';

    errno = 0;
    char  *end;
    char  *buffer = (char *)parse->buffer;
    double value  = strtod(buffer, &end);
    if (end == buffer + len && IS_NORMAL(value)) {
        return value;
    }
    else {
        const char *error = errno == 0 ? "invalid input" : strerror(errno);
        R_PARSE_DIE_AT(parse, at, "can't parse '%s' into a double: %s",
                       buffer, error);
    }
}


char *R_parse_read_string(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    int len = R_parse_read_ushort(parse);
    if (len == 0) {
        return NULL;
    }
    char *buf = R_malloc(R_int2size(len + 1));
    R_parse_read_into(parse, len, (unsigned char *)buf);
    buf[len] = '\0';
    return buf;
}

char *R_parse_read_string_to_buffer(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    int len = R_parse_read_ushort(parse);
    if (len == 0) {
        return NULL;
    }
    else {
        R_parse_read_bytes_with_extra_space(parse, len, 1);
        parse->buffer[len] = (unsigned char) '\0';
        return (char *)parse->buffer;
    }
}


void R_parse_die_unless_eof(R_Parse *parse)
{
    R_MAGIC_CHECK(R_Parse, parse);
    int at  = parse->pos;
    int got = parse->read(parse->bufsize, parse->buffer, parse->user);
    if (got != 0) {
        R_PARSE_DIE_AT(parse, at, "trailing garbage of %s%d byte(s)",
                       got == parse->bufsize ? "at least " : "", got);
    }
}
