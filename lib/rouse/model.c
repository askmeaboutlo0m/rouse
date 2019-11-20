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
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "common.h"
#include "parse.h"
#include "model.h"


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


static void read_buffer(R_Parse *parse, R_MeshBuffer *mbuf)
{
    R_MAGIC_SET(R_MeshBuffer, mbuf);
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
    R_MAGIC_SET(R_Mesh, mesh);
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
    R_MAGIC_CHECK(R_Model, model);
    for (int i = 0; i < model->mesh.count; ++i) {
        R_Mesh *mesh = &model->mesh.values[i];
        R_MAGIC_CHECK(R_Mesh, mesh);
        for (int j = 0; j < mesh->buffer.count; ++j) {
            R_MeshBuffer *mbuf = &mesh->buffer.values[j];
            R_MAGIC_CHECK(R_MeshBuffer, mbuf);
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
    R_MAGIC_SET(R_Model, model);
    R_PARSE_DEBUG(&parse, "parsing model with bufsize %d", bufsize);
    read_meshes(&parse, model);

    R_parse_die_unless_eof(&parse);
#ifdef ROUSE_MAGIC
    check_magics(model);
#endif
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


static void mesh_buffer_free(R_MeshBuffer *mbuf)
{
    R_MAGIC_CHECK(R_MeshBuffer, mbuf);
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
}

static void mesh_free(R_Mesh *mesh)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    int bufcount = mesh->buffer.count;
    for (int i = 0; i < bufcount; ++i) {
        mesh_buffer_free(&mesh->buffer.values[i]);
    }
    free(mesh->buffer.values);
    R_MAGIC_POISON(R_Mesh, mesh);
}

void R_model_free(R_Model *model)
{
    if (model) {
        R_MAGIC_CHECK(R_Model, model);
        int mcount = model->mesh.count;
        for (int i = 0; i < mcount; ++i) {
            mesh_free(&model->mesh.values[i]);
        }
        free(model->mesh.values);
        R_MAGIC_POISON(R_Model, model);
        free(model);
    }
}


R_Mesh *R_model_mesh_by_index(R_Model *model, int index)
{
    R_MAGIC_CHECK(R_Model, model);
    if (index < 0 || index >= model->mesh.count) {
        R_die("Model mesh index %d out of bounds (0 ... %d)",
              index, model->mesh.count);
    }
    return &model->mesh.values[index];
}


int R_mesh_buffer_index_by_name(R_Mesh *mesh, const char *name)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    for (int i = 0; i < mesh->buffer.count; ++i) {
        R_MeshBuffer *buffer = &mesh->buffer.values[i];
        R_MAGIC_CHECK(R_MeshBuffer, buffer);
        if (R_str_equal(name, buffer->name)) {
            return i;
        }
    }
    return -1;
}

R_MeshBuffer *R_mesh_buffer_by_index(R_Mesh *mesh, int index)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    if (index < 0 || index >= mesh->buffer.count) {
        R_die("Mesh buffer index %d out of bounds (0 ... %d)",
              index, mesh->buffer.count);
    }
    R_MeshBuffer *buffer = &mesh->buffer.values[index];
    R_MAGIC_CHECK(R_MeshBuffer, buffer);
    return buffer;
}

R_MeshBuffer *R_mesh_buffer_by_name(R_Mesh *mesh, const char *name)
{
    R_MAGIC_CHECK(R_Mesh, mesh);
    int           index  = R_mesh_buffer_index_by_name(mesh, name);
    R_MeshBuffer *buffer = &mesh->buffer.values[index];
    R_MAGIC_CHECK(R_MeshBuffer, buffer);
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
