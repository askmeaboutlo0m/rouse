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
#include "common.h"
#include "model.h"


static int read_from_file(uint8_t *out, int size, void *fp)
{
    return R_size2int(fread(out, sizeof(*out), R_int2size(size), fp));
}

R_Model *R_model_from_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        R_die("Can't open '%s': %s", path, strerror(errno));
    }

    uint8_t buf[1024];
    R_Model *model = R_model_new(path, read_from_file, fp, buf, sizeof(buf));

    if (fclose(fp) != 0) {
        R_warn("Error closing '%s': %s", path, strerror(errno));
    }

    return model;
}


static void mesh_buffer_free(R_MeshBuffer *mbuf)
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
}

static void mesh_free(R_Mesh *mesh)
{
    int bufcount = mesh->buffer.count;
    for (int i = 0; i < bufcount; ++i) {
        mesh_buffer_free(&mesh->buffer.values[i]);
    }
    free(mesh->buffer.values);
}

void R_model_free(R_Model *model)
{
    if (model) {
        int mcount = model->mesh.count;
        for (int i = 0; i < mcount; ++i) {
            mesh_free(&model->mesh.values[i]);
        }
        free(model);
    }
}


R_Mesh *R_model_mesh_by_index(R_Model *model, int index)
{
    if (index < 0 || index >= model->mesh.count) {
        R_die("Model mesh index %d out of bounds (0 ... %d)",
              index, model->mesh.count);
    }
    return &model->mesh.values[index];
}


int R_mesh_buffer_index_by_name(R_Mesh *mesh, const char *name)
{
    for (int i = 0; i < mesh->buffer.count; ++i) {
        R_MeshBuffer *buffer = &mesh->buffer.values[i];
        if (R_str_equal(name, buffer->name)) {
            return i;
        }
    }
    return -1;
}

R_MeshBuffer *R_mesh_buffer_by_index(R_Mesh *mesh, int index)
{
    if (index < 0 || index >= mesh->buffer.count) {
        R_die("Mesh buffer index %d out of bounds (0 ... %d)",
              index, mesh->buffer.count);
    }
    return &mesh->buffer.values[index];
}

R_MeshBuffer *R_mesh_buffer_by_name(R_Mesh *mesh, const char *name)
{
    return &mesh->buffer.values[R_mesh_buffer_index_by_name(mesh, name)];
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
