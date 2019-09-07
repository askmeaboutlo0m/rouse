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
#include <stdnoreturn.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include "common.h"
#include "parse.h"
#include "model.h"


static const char FILE_MAGIC[] = "rmodel1";


static void check_file_magic(R_Parse *parse)
{
    R_parse_read_bytes(parse, sizeof(FILE_MAGIC));
    if (memcmp(parse->buffer, FILE_MAGIC, sizeof(FILE_MAGIC)) != 0) {
        R_PARSE_DIE(parse, "bad file magic, want \"%s\"", FILE_MAGIC);
    }
}


#define DEF_READ_VALUES(NAME, CALL, TYPE) \
    static void NAME(R_Parse *parse, TYPE *arr, int count) \
    { \
        for (int i = 0; i < count; ++i) { \
            arr[i] = CALL(parse); \
        } \
    }

DEF_READ_VALUES(read_ushorts, R_parse_read_ushort, unsigned short)
DEF_READ_VALUES(read_floats,  R_parse_read_float,  float)


static void read_buffer(R_Parse *parse, R_MeshBuffer *mbuf)
{
    R_MAGIC_SET(mbuf);
    mbuf->type = (R_BufferType) R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "   buffer type: %d", mbuf->type);
    if (mbuf->type != R_BUFFER_TYPE_USHORT && mbuf->type != R_BUFFER_TYPE_FLOAT) {
        R_PARSE_DIE(parse, "unknown mesh buffer type '%d'", mbuf->type);
    }

    mbuf->name = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "   buffer name \"%s\"", mbuf->name ? mbuf->name : "");

    mbuf->divisor = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "   buffer element divisor: %d", mbuf->divisor);

    mbuf->count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "   buffer element count: %d", mbuf->count);

    if (mbuf->divisor == 0 || mbuf->count % mbuf->divisor != 0) {
        R_PARSE_DIE(parse, "mesh buffer element count %d isn't divisible by divisor %d",
                  mbuf->count, mbuf->divisor);
    }

    switch (mbuf->type) {
        case R_BUFFER_TYPE_USHORT:
            mbuf->ushorts = R_ANEW(mbuf->ushorts, R_int2size(mbuf->count));
            read_ushorts(parse, mbuf->ushorts, mbuf->count);
            break;
        case R_BUFFER_TYPE_FLOAT:
            mbuf->floats = R_ANEW(mbuf->floats, R_int2size(mbuf->count));
            read_floats(parse, mbuf->floats, mbuf->count);
            break;
        default:
            R_PARSE_DIE(parse, "impossible mesh buffer type '%c'", mbuf->type);
    }
}

static void read_buffers(R_Parse *parse, R_Mesh *mesh)
{
    R_MAGIC_SET(mesh);
    int bufcount = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "  buffer count: %d", bufcount);
    mesh->buffer.count  = bufcount;
    mesh->buffer.values = R_ANEW(mesh->buffer.values, R_int2size(bufcount));
    for (int i = 0; i < bufcount; ++i) {
        R_PARSE_DEBUG(parse, "  buffer %d/%d", i + 1, bufcount);
        read_buffer(parse, &mesh->buffer.values[i]);
    }
}

static void read_meshes(R_Parse *parse, R_Model *model)
{
    int mcount = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, " mesh count: %d", mcount);
    model->mesh.count  = mcount;
    model->mesh.values = R_ANEW(model->mesh.values, R_int2size(mcount));
    for (int i = 0; i < mcount; ++i) {
        R_PARSE_DEBUG(parse, " mesh %d/%d", i + 1, mcount);
        read_buffers(parse, &model->mesh.values[i]);
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


R_Model *R_model_new(const char *title, R_ParseReadFn read, R_UserData user,
                     int bufsize, unsigned char buffer[static bufsize])
{
    R_Parse parse = R_parse(title, read, user, bufsize, buffer);
    check_file_magic(&parse);

    R_Model *model = R_NEW(model);
    R_MAGIC_SET(model);
    R_PARSE_DEBUG(&parse, "parsing model with bufsize %d", bufsize);
    read_meshes(&parse, model);

    R_parse_die_unless_eof(&parse);
#ifdef ROUSE_MAGIC
    check_magics(model);
#endif
    return model;
}
