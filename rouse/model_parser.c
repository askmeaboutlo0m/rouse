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
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "common.h"
#include "model.h"


#define PARSE_X_AT(X, PARSE, AT, FORMAT, ...) do { \
        X("%s: " FORMAT " (at position %d)", \
          (PARSE)->title ? (PARSE)->title : "R_model_new", \
          __VA_ARGS__, AT); \
    } while (0)

#define PARSE_DIE_AT(PARSE, AT, FORMAT, ...) \
    PARSE_X_AT(R_die, PARSE, AT, FORMAT, __VA_ARGS__)

#define PARSE_DIE(PARSE, FORMAT, ...) \
    PARSE_DIE_AT(PARSE, (PARSE)->pos, FORMAT, __VA_ARGS__);

#define PARSE_DIE_PREV(PARSE, FORMAT, ...) \
    PARSE_DIE_AT(PARSE, (PARSE)->pos - (1 + (int) strlen((char *)(PARSE)->buffer)), \
                 FORMAT, __VA_ARGS__);

#define PARSE_DEBUG(PARSE, FORMAT, ...) \
    PARSE_X_AT(R_debug, PARSE, (PARSE)->pos, FORMAT, __VA_ARGS__)


static const char FILE_MAGIC[] = "rmodel1";

struct Parse {
    const char  *title;
    R_ModelRead read;
    void        *user;
    uint8_t     *buffer;
    int         bufsize;
    int         pos;
};


static int read_into(struct Parse *parse, uint8_t *out, int count)
{
    int got = parse->read(out, count, parse->user);
    if (got != count) {
        PARSE_DIE(parse, "bad read, want %d bytes but got %d", count, got);
    }
    return got;
}

static void read_n(struct Parse *parse, int count, int extra)
{
    int needed = count + extra;
    if (parse->bufsize < needed) {
        PARSE_DIE(parse, "out of buffer space, want %d bytes but got %d",
                  needed, parse->bufsize);
    }
    parse->pos += read_into(parse, parse->buffer, count);
}

static char read_char(struct Parse *parse)
{
    read_n(parse, 1, 0);
    return (char) parse->buffer[0];
}

static unsigned char read_uchar(struct Parse *parse)
{
    return read_char(parse);
}

static float read_float(struct Parse *parse)
{
    unsigned char len = read_uchar(parse);
    read_n(parse, len, 1);
    parse->buffer[len] = '\0';

    char *end;
    errno = 0;
    float value = strtof((char *)parse->buffer, &end);
    /* We'll just pretend that infinity and NaN are invalid inputs. */
    if (end == (char *)parse->buffer + len && !isnan(value) && !isinf(value)) {
        return value;
    }
    else {
        const char *error = errno == 0 ? "invalid input" : strerror(errno);
        PARSE_DIE_PREV(parse, "can't parse '%s' into a float: %s",
                       parse->buffer, error);
    }
}

static unsigned short read_ushort(struct Parse *parse)
{
    read_n(parse, 2, 0);
    unsigned short lo = parse->buffer[0],
                   hi = parse->buffer[1];
    return lo + (hi << 8);
}

static int read_count(struct Parse *parse)
{
    return read_ushort(parse);
}

static char *read_string(struct Parse *parse)
{
    int len = read_count(parse);
    if (len == 0) {
        return NULL;
    }
    char *buf = R_malloc(len + 1);
    read_into(parse, (uint8_t *)buf, len);
    buf[len] = '\0';
    return buf;
}


static void check_file_magic(struct Parse *parse)
{
    read_n(parse, sizeof(FILE_MAGIC), 0);
    if (memcmp(parse->buffer, FILE_MAGIC, sizeof(FILE_MAGIC)) != 0) {
        PARSE_DIE(parse, "bad file magic, want \"%s\"", FILE_MAGIC);
    }
}


#define DEF_READ_VALUES(NAME, TYPE) \
    static void NAME ## s(struct Parse *parse, TYPE *arr, int count) \
    { \
        for (int i = 0; i < count; ++i) { \
            arr[i] = NAME(parse); \
        } \
    }

DEF_READ_VALUES(read_ushort, unsigned short)
DEF_READ_VALUES(read_float,  float)


static void read_buffer(struct Parse *parse, R_MeshBuffer *mbuf)
{
    R_MAGIC_SET(mbuf);
    mbuf->type = read_char(parse);
    PARSE_DEBUG(parse, "   buffer type: %d", mbuf->type);
    if (mbuf->type != R_BUFFER_TYPE_USHORT && mbuf->type != R_BUFFER_TYPE_FLOAT) {
        PARSE_DIE(parse, "unknown mesh buffer type '%d'", mbuf->type);
    }

    mbuf->name = read_string(parse);
    PARSE_DEBUG(parse, "   buffer name \"%s\"", mbuf->name ? mbuf->name : "");

    mbuf->divisor = read_uchar(parse);
    PARSE_DEBUG(parse, "   buffer element divisor: %d", mbuf->divisor);

    mbuf->count = read_count(parse);
    PARSE_DEBUG(parse, "   buffer element count: %d", mbuf->count);

    if (mbuf->divisor == 0 || mbuf->count % mbuf->divisor != 0) {
        PARSE_DIE(parse, "mesh buffer element count %d isn't divisible by divisor %d",
                  mbuf->count, mbuf->divisor);
    }

    switch (mbuf->type) {
        case R_BUFFER_TYPE_USHORT:
            mbuf->ushorts = R_ANEW(mbuf->ushorts, mbuf->count);
            read_ushorts(parse, mbuf->ushorts, mbuf->count);
            break;
        case R_BUFFER_TYPE_FLOAT:
            mbuf->floats = R_ANEW(mbuf->floats, mbuf->count);
            read_floats(parse, mbuf->floats, mbuf->count);
            break;
        default:
            PARSE_DIE(parse, "impossible mesh buffer type '%c'", mbuf->type);
    }
}

static void read_buffers(struct Parse *parse, R_Mesh *mesh)
{
    R_MAGIC_SET(mesh);
    int bufcount = read_count(parse);
    PARSE_DEBUG(parse, "  buffer count: %d", bufcount);
    mesh->buffer.count  = bufcount;
    mesh->buffer.values = R_ANEW(mesh->buffer.values, bufcount);
    for (int i = 0; i < bufcount; ++i) {
        PARSE_DEBUG(parse, "  buffer %d/%d", i + 1, bufcount);
        read_buffer(parse, &mesh->buffer.values[i]);
    }
}

static void read_meshes(struct Parse *parse, R_Model *model)
{
    int mcount = read_count(parse);
    PARSE_DEBUG(parse, " mesh count: %d", mcount);
    model->mesh.count  = mcount;
    model->mesh.values = R_ANEW(model->mesh.values, mcount);
    for (int i = 0; i < mcount; ++i) {
        PARSE_DEBUG(parse, " mesh %d/%d", i + 1, mcount);
        read_buffers(parse, &model->mesh.values[i]);
    }
}


static void check_eof(struct Parse *parse)
{
    int at  = parse->pos;
    int got = parse->read(parse->buffer, parse->bufsize, parse->user);
    if (got != 0) {
        PARSE_DIE_AT(parse, at, "trailing garbage of %s%d byte(s)",
                     got == parse->bufsize ? "at least " : "", got);
    }
}

#ifdef ROUSE_MAGIC
static void check_magics(R_Model *model)
{
    R_MAGIC_CHECK(model);
    for (int i = 0; i < model->mesh.count; ++i) {
        R_Mesh *mesh = &model->mesh.values[i];
        R_MAGIC_CHECK(mesh);
        for (int j = 0; j < mesh->buffer.count; ++j) {
            R_MeshBuffer *mbuf = &mesh->buffer.values[j];
            R_MAGIC_CHECK(mbuf);
        }
    }
}
#endif


R_Model *R_model_new(const char *title, R_ModelRead read, void *user,
                     uint8_t *buffer, int bufsize)
{
    struct Parse parse = {title, read, user, buffer, bufsize, 0};
    check_file_magic(&parse);

    R_Model *model = R_NEW(model);
    R_MAGIC_SET(model);
    PARSE_DEBUG(&parse, "parsing model with bufsize %d", bufsize);
    read_meshes(&parse, model);

    check_eof(&parse);
#ifdef ROUSE_MAGIC
    check_magics(model);
#endif
    return model;
}
