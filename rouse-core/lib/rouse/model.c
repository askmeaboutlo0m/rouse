/*
 * Copyright (c) 2019, 2021 askmeaboutloom
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
#include <rouse_config.h>
#include "common.h"
#include "parse.h"
#include "model.h"
#include "refcount.h"
#include "string.h"


static void check_file_magic(R_Parse *parse)
{
    static const char FILE_MAGIC[] = "rmodel1";
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


static R_MeshBuffer *read_buffer(R_Parse *parse)
{
    int type = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "   buffer type: %d", type);
    if (type != R_BUFFER_TYPE_USHORT && type != R_BUFFER_TYPE_FLOAT) {
        R_PARSE_DIE(parse, "unknown mesh buffer type '%d'", type);
    }

    char *name = R_parse_read_string(parse);
    R_PARSE_DEBUG(parse, "   buffer name \"%s\"", name ? name : "");

    int divisor = R_parse_read_uchar(parse);
    R_PARSE_DEBUG(parse, "   buffer element divisor: %d", divisor);

    int count = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "   buffer element count: %d", count);

    if (divisor == 0 || count % divisor != 0) {
        R_PARSE_DIE(parse, "mesh buffer element count %d isn't divisible by divisor %d",
                  count, divisor);
    }

    R_MeshBufferValues values;
    switch (type) {
        case R_BUFFER_TYPE_USHORT:
            values.ushorts = R_ANEW(values.ushorts, R_int2size(count));
            read_ushorts(parse, values.ushorts, count);
            break;
        case R_BUFFER_TYPE_FLOAT:
            values.floats = R_ANEW(values.floats, R_int2size(count));
            read_floats(parse, values.floats, count);
            break;
        default:
            R_PARSE_DIE(parse, "impossible mesh buffer type '%c'", type);
    }

    R_MeshBuffer *mbuf = R_NEW_INIT_STRUCT(mbuf, R_MeshBuffer,
        R_MAGIC_INIT(R_MeshBuffer) 1, (R_BufferType) type, name, count, divisor,
        {values});
    return mbuf;
}

static R_Mesh *read_buffers(R_Parse *parse)
{
    int bufcount = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, "  buffer count: %d", bufcount);
    R_MeshBuffer **values = R_ANEW(values, R_int2size(bufcount));
    for (int i = 0; i < bufcount; ++i) {
        R_PARSE_DEBUG(parse, "  buffer %d/%d", i + 1, bufcount);
        values[i] = read_buffer(parse);
    }

    R_Mesh *mesh = R_NEW_INIT_STRUCT(mesh, R_Mesh,
        R_MAGIC_INIT(R_Mesh) 1, {bufcount, values});
    return mesh;
}

static void read_meshes(R_Parse *parse, R_Model *model)
{
    int mcount = R_parse_read_ushort(parse);
    R_PARSE_DEBUG(parse, " mesh count: %d", mcount);
    model->mesh.count  = mcount;
    model->mesh.values = R_ANEW(model->mesh.values, R_int2size(mcount));
    for (int i = 0; i < mcount; ++i) {
        R_PARSE_DEBUG(parse, " mesh %d/%d", i + 1, mcount);
        model->mesh.values[i] = read_buffers(parse);
    }
}



static inline void check_mesh_buffer(R_UNUSED_UNLESS_DEBUG R_MeshBuffer *mbuf)
{
    R_MAGIC_CHECK(R_MeshBuffer, mbuf);
    R_assert(mbuf->refs > 0, "refcount must always be positive");
}

static inline void check_mesh(R_UNUSED_UNLESS_DEBUG R_Mesh *mesh)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    R_assert(mesh->refs > 0, "refcount must always be positive");
}

static inline void check_model(R_UNUSED_UNLESS_DEBUG R_Model *model)
{
    R_MAGIC_CHECK(R_Model, model);
    R_assert(model->refs > 0, "refcount must always be positive");
}


static void check_tree(R_Model *model)
{
    check_model(model);
    for (int i = 0; i < model->mesh.count; ++i) {
        R_Mesh *mesh = model->mesh.values[i];
        check_mesh(mesh);
        for (int j = 0; j < mesh->buffer.count; ++j) {
            R_MeshBuffer *mbuf = mesh->buffer.values[j];
            check_mesh_buffer(mbuf);
        }
    }
}


R_Model *R_model_new(const char *title, R_ParseReadFn read, R_UserData user,
                     int bufsize, unsigned char buffer[static bufsize])
{
    R_Parse parse = R_parse(title, read, user, bufsize, buffer);
    check_file_magic(&parse);

    R_Model *model = R_NEW(model);
    R_MAGIC_SET(R_Model, model);
    model->refs = 1;
    R_PARSE_DEBUG(&parse, "parsing model with bufsize %d", bufsize);
    read_meshes(&parse, model);

    R_parse_die_unless_eof(&parse);
    check_tree(model);
    return model;
}


R_Model *R_model_from_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        R_die("Can't open '%s': %s", path, strerror(errno));
    }

    unsigned char buffer[1024];
    R_Model *model = R_model_new(path, R_parse_from_file, R_user_data(fp),
                                 sizeof(buffer), buffer);

    if (fclose(fp) != 0) {
        R_warn("Error closing '%s': %s", path, strerror(errno));
    }

    return model;
}


static void free_model(R_Model *model)
{
    int mcount = model->mesh.count;
    for (int i = 0; i < mcount; ++i) {
        R_mesh_decref(model->mesh.values[i]);
    }
    free(model->mesh.values);
    R_MAGIC_POISON(R_Model, model);
    free(model);
}

R_DEFINE_REFCOUNT_FUNCS(R_Model, model, refs)


static void write_ushort(R_MeshBuffer *mbuf, int i, R_String *s)
{
    R_string_printf(s, "%u", (unsigned int) mbuf->ushorts[i]);
}

static void write_float(R_MeshBuffer *mbuf, int i, R_String *s)
{
    R_string_printf(s, "%f", (double) mbuf->floats[i]);
}

static void dump_mesh_buffer_values_to(
    R_MeshBuffer *mbuf, R_String *s,
    void (*write)(R_MeshBuffer *, int, R_String *))
{
    int count   = mbuf->count;
    int divisor = mbuf->divisor;
    for (int i = 0; i < count; ++i) {
        int mod = i % divisor;
        if (mod == 0) {
            R_string_append(s, "        (");
        }
        write(mbuf, i, s);
        if (mod == divisor - 1) {
            R_string_append(s, ")\n");
        }
        else {
            R_string_append(s, ", ");
        }
    }
}

static void dump_mesh_buffer_to(R_MeshBuffer *mbuf, R_String *s)
{
    R_string_printf(s, "buffer with %d elements and divisor %d\n",
                    mbuf->count, mbuf->divisor);
    if (mbuf->name) {
        R_string_printf(s, "      name: '%s'\n", mbuf->name);
    }

    if (mbuf->type == R_BUFFER_TYPE_USHORT) {
        R_string_append(s, "      ushort values:\n");
        dump_mesh_buffer_values_to(mbuf, s, write_ushort);
    }
    else if (mbuf->type == R_BUFFER_TYPE_FLOAT) {
        R_string_append(s, "      float values:\n");
        dump_mesh_buffer_values_to(mbuf, s, write_float);
    }
    else {
        R_string_append(s, "      unknown type, can't decode values\n");
    }
}

static void dump_mesh_to(R_Mesh *mesh, R_String *s)
{
    int buffer_count = mesh->buffer.count;
    R_string_printf(s, "mesh with %d buffer(s)\n", buffer_count);
    for (int i = 0; i < buffer_count; ++i) {
        R_string_printf(s, "    [%d] ", i);
        dump_mesh_buffer_to(mesh->buffer.values[i], s);
    }
}

static void dump_model_to(R_Model *model, R_String *s)
{
    int mesh_count = model->mesh.count;
    R_string_printf(s, "model with %d mesh(es)\n", mesh_count);
    for (int i = 0; i < mesh_count; ++i) {
        R_string_printf(s, "  [%d] ", i);
        dump_mesh_to(model->mesh.values[i], s);
    }
}

char *R_model_dump(R_Model *model)
{
    check_model(model);
    R_String *s = R_string_new(0);
    dump_model_to(model, s);
    return R_string_free_keep_buffer(s);
}


R_Mesh *R_model_mesh_by_index(R_Model *model, int index)
{
    check_model(model);
    if (index < 0 || index >= model->mesh.count) {
        R_die("Model mesh index %d out of bounds (0 ... %d)",
              index, model->mesh.count);
    }
    return model->mesh.values[index];
}


static void free_mesh(R_Mesh *mesh)
{
    int bufcount = mesh->buffer.count;
    for (int i = 0; i < bufcount; ++i) {
        R_mesh_buffer_decref(mesh->buffer.values[i]);
    }
    free(mesh->buffer.values);
    R_MAGIC_POISON(R_Mesh, mesh);
    free(mesh);
}

R_DEFINE_REFCOUNT_FUNCS(R_Mesh, mesh, refs)

int R_mesh_buffer_index_by_name(R_Mesh *mesh, const char *name)
{
    check_mesh(mesh);
    for (int i = 0; i < mesh->buffer.count; ++i) {
        R_MeshBuffer *buffer = mesh->buffer.values[i];
        check_mesh_buffer(buffer);
        if (R_str_equal(name, buffer->name)) {
            return i;
        }
    }
    return -1;
}

R_MeshBuffer *R_mesh_buffer_by_index(R_Mesh *mesh, int index)
{
    check_mesh(mesh);
    if (index < 0 || index >= mesh->buffer.count) {
        R_die("Mesh buffer index %d out of bounds (0 ... %d)",
              index, mesh->buffer.count);
    }
    R_MeshBuffer *buffer = mesh->buffer.values[index];
    check_mesh_buffer(buffer);
    return buffer;
}

R_MeshBuffer *R_mesh_buffer_by_name(R_Mesh *mesh, const char *name)
{
    check_mesh(mesh);
    int           index  = R_mesh_buffer_index_by_name(mesh, name);
    R_MeshBuffer *buffer = mesh->buffer.values[index];
    check_mesh_buffer(buffer);
    return buffer;
}


static const char *buffer_type_name(R_BufferType type)
{
    switch (type) {
        case R_BUFFER_TYPE_USHORT:
            return "USHORT";
        case R_BUFFER_TYPE_FLOAT:
            return "FLOAT";
        default:
            return "unknown";
    }
}

#define R_DEF_MESH_BUFFER_BY_FNS(RETURN_TYPE, NAME, BUFFER_TYPE) \
    RETURN_TYPE *R_mesh_ ## NAME ## _by_index(R_Mesh *mesh, int index, \
                                              int *out_count) \
    { \
        R_MeshBuffer *buffer = R_mesh_buffer_by_index(mesh, index); \
        if (buffer->type != BUFFER_TYPE) { \
            R_die("Bad %s buffer type for '%s', wanted %d (%s) but got %d (%s)", \
                  #NAME, buffer->name ? buffer->name : "", \
                  BUFFER_TYPE,  buffer_type_name(BUFFER_TYPE), \
                  buffer->type, buffer_type_name(buffer->type)); \
        } \
        if (out_count) { \
            *out_count = buffer->count; \
        } \
        return buffer->NAME; \
    } \
    \
    RETURN_TYPE *R_mesh_ ## NAME ## _by_name(R_Mesh *mesh, const char *name, \
                                             int *out_count) \
    { \
        int index = R_mesh_buffer_index_by_name(mesh, name); \
        if (index < 0) { \
            R_die("Can't find %s buffer '%s'", #NAME, name); \
        } \
        return R_mesh_ ## NAME ## _by_index(mesh, index, out_count); \
    }

R_DEF_MESH_BUFFER_BY_FNS(unsigned short, ushorts, R_BUFFER_TYPE_USHORT)
R_DEF_MESH_BUFFER_BY_FNS(float,          floats,  R_BUFFER_TYPE_FLOAT)



static void free_mesh_buffer(R_MeshBuffer *mbuf)
{
    free(mbuf->name);
    switch (mbuf->type) {
        case R_BUFFER_TYPE_USHORT:
            free(mbuf->ushorts);
            break;
        case R_BUFFER_TYPE_FLOAT:
            free(mbuf->floats);
            break;
        default: /* Uhh... that's not a valid model? */
            R_warn("Can't free mesh buffer with unknown type '%d'", mbuf->type);
            break;
    }
    R_MAGIC_POISON(R_MeshBuffer, mbuf);
    free(mbuf);
}

R_DEFINE_REFCOUNT_FUNCS(R_MeshBuffer, mesh_buffer, refs)
