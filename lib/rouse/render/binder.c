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
#include "../3rdparty/gles2_inc.h"
#include "../common.h"
#include "../parse.h"
#include "../model.h"
#include "gl.h"
#include "binder.h"


static int count_attribute_args(va_list ap)
{
    int count = 0;
    R_BinderBind fn;
    while ((fn = va_arg(ap, R_BinderBind))) {
        va_arg(ap, R_UserData);
        ++count;
    }
    return count;
}

static void fill_attribute_args(R_Binder *binder, va_list ap)
{
    for (int i = 0; i < binder->attribute.count; ++i) {
        binder->attribute.fns [i] = va_arg(ap, R_BinderBind);
        binder->attribute.args[i] = va_arg(ap, R_UserData);
    }
}

static int get_vertex_attrib_count(R_Binder *binder)
{
    int count = binder->attribute.count;
    return binder->features & R_BINDER_INDEXED ? count - 1 : count;
}

R_Binder *R_binder_new(unsigned int features, R_BinderDraw draw, R_UserData arg,
                       const char *vert, const char *frag, ...)
{
    R_Binder *binder = R_NEW_INIT_STRUCT(binder, R_Binder,
        R_MAGIC_INIT(R_Binder) NULL, R_user_int(0), 0, 0,
        R_BINDER_DEFAULT_DEPTH_TEST, R_BINDER_DEFAULT_CULL_FACE,
        R_BINDER_DEFAULT_SFACTOR, R_BINDER_DEFAULT_DFACTOR,
        {0, NULL, NULL, NULL});
    R_MAGIC_CHECK(R_Binder, binder);

    binder->features = features;
    binder->draw     = draw;
    binder->arg      = arg;

    if (vert || frag) {
        binder->program = R_gl_program_new(vert, frag);
    }

    va_list ap;
    va_start(ap, frag);
    int count = count_attribute_args(ap);
    va_end(ap);

    if (count > 0) {
        size_t size = R_int2size(count);

        binder->attribute.count   = count;
        binder->attribute.buffers = R_ANEW(binder->attribute.buffers, size);
        binder->attribute.fns     = R_ANEW(binder->attribute.fns,     size);
        binder->attribute.args    = R_ANEW(binder->attribute.args,    size);

        R_GL(glGenBuffers, count, binder->attribute.buffers);

        va_start(ap, frag);
        fill_attribute_args(binder, ap);
        va_end(ap);
    }

    R_debug("%s(features=%x, buffers=%d, vertex_attributes=%d)", __func__,
            binder->features, binder->attribute.count,
            get_vertex_attrib_count(binder));

    return binder;
}

void R_binder_free(R_Binder *binder)
{
    if (binder) {
        R_MAGIC_CHECK(R_Binder, binder);
        R_gl_program_free(binder->program);
        if (binder->attribute.count > 0 && binder->attribute.buffers) {
            R_GL(glDeleteBuffers, binder->attribute.count,
                                  binder->attribute.buffers);
        }
        free(binder->attribute.buffers);
        free(binder->attribute.fns);
        free(binder->attribute.args);
        R_MAGIC_POISON(R_Binder, binder);
        free(binder);
    }
}


void R_binder_begin(R_Binder *binder)
{
    R_MAGIC_CHECK(R_Binder, binder);
    R_GL_CLEAR_ERROR();
    unsigned int features = binder->features;

    if (features & R_BINDER_DEPTH_TEST) {
        R_GL(glEnable, GL_DEPTH_TEST);
        R_GL(glDepthFunc, binder->depth_test);
    }
    else {
        R_GL(glDisable, GL_DEPTH_TEST);
    }

    if (features & R_BINDER_CULLING) {
        R_GL(glEnable, GL_CULL_FACE);
        R_GL(glCullFace, binder->cull_face);
    }
    else {
        R_GL(glDisable, GL_CULL_FACE);
    }

    if (features & R_BINDER_BLENDING) {
        R_GL(glEnable, GL_BLEND);
        R_GL(glBlendFunc, binder->sfactor, binder->dfactor);
    }
    else {
        R_GL(glDisable, GL_BLEND);
    }

    R_GL(glUseProgram, binder->program);

    int count = get_vertex_attrib_count(binder);
    for (int i = 0; i < count; ++i) {
        R_GL(glEnableVertexAttribArray, R_int2uint(i));
    }

    int max = R_gl_max_vertex_attribs;
    for (int i = count; i < max; ++i) {
        R_GL(glDisableVertexAttribArray, R_int2uint(i));
    }
}

void R_binder_end(R_Binder *binder)
{
    R_MAGIC_CHECK(R_Binder, binder);
    int count = get_vertex_attrib_count(binder);
    for (int i = 0; i < count; ++i) {
        R_GL(glDisableVertexAttribArray, R_int2uint(i));
    }
    R_GL(glUseProgram, 0);
}


int R_binder_uniform_location(R_Binder *binder, const char *name)
{
    R_MAGIC_CHECK(R_Binder, binder);
    return R_gl_uniform_location(binder->program, name);
}


void R_binder_draw(R_Binder *binder, void *subject)
{
    R_MAGIC_CHECK(R_Binder, binder);
    int count  = binder->attribute.count;
    int offset = binder->features & R_BINDER_INDEXED ? 1 : 0;

    for (int i = 0; i < count; ++i) {
        R_BinderBind fn     = binder->attribute.fns    [i];
        R_UserData   arg    = binder->attribute.args   [i];
        unsigned int buffer = binder->attribute.buffers[i];
        fn(subject, arg, buffer, i - offset);
    }

    binder->draw(subject, binder->arg);
}

static void draw_mesh(R_MeshBuffer *mb)
{
    R_MAGIC_CHECK(R_MeshBuffer, mb);
    R_debug("draw %d indexed triangles", mb->count);
    R_GL_CLEAR_ERROR();
    R_GL(glDrawElements, GL_TRIANGLES, mb->count, GL_UNSIGNED_SHORT, NULL);
}

void R_binder_draw_mesh_by_index(void *subject, R_UserData arg)
{
    draw_mesh(R_mesh_buffer_by_index(subject, arg.i));
}

void R_binder_draw_mesh_by_name(void *subject, R_UserData arg)
{
    draw_mesh(R_mesh_buffer_by_name(subject, arg.data));
}


static void bind_index_buffer(R_MeshBuffer *mb, unsigned int buffer)
{
    R_MAGIC_CHECK(R_MeshBuffer, mb);
    if (mb->type != R_BUFFER_TYPE_USHORT) {
        R_die("index mesh buffer '%s' type %d (ushort) != %d",
              mb->name ? mb->name : "", R_BUFFER_TYPE_USHORT, mb->type);
    }
    R_debug("bind index buffer '%s'", mb->name ? mb->name : "");
    R_GL_CLEAR_ERROR();
    R_GL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, buffer);
    size_t bufsize = sizeof(*mb->ushorts) * R_int2size(mb->count);
    R_GL(glBufferData, GL_ELEMENT_ARRAY_BUFFER, R_size2ptrdiff(bufsize),
                       mb->ushorts, GL_STATIC_DRAW);
}

static void bind_vertex_buffer(R_MeshBuffer *mb, unsigned int buffer, int index)
{
    R_MAGIC_CHECK(R_MeshBuffer, mb);
    unsigned int type;
    void         *values;
    size_t       size;

    switch (mb->type) {
        case R_BUFFER_TYPE_USHORT:
            R_debug("bind ushort buffer '%s' to vertex attribute %d",
                    mb->name ? mb->name : "", index);
            type   = GL_UNSIGNED_SHORT;
            values = mb->ushorts;
            size   = sizeof(*mb->ushorts);
            break;
        case R_BUFFER_TYPE_FLOAT:
            R_debug("bind float buffer '%s' to vertex attribute %d",
                    mb->name ? mb->name : "", index);
            type   = GL_FLOAT;
            values = mb->floats;
            size   = sizeof(*mb->floats);
            break;
        default:
            R_die("Unknown buffer type: %d", mb->type);
    }

    R_GL(glBindBuffer, GL_ARRAY_BUFFER, buffer);
    size_t bufsize = size * R_int2size(mb->count);
    R_GL(glBufferData, GL_ARRAY_BUFFER, R_size2ptrdiff(bufsize),
                       values, GL_STATIC_DRAW);
    R_GL(glVertexAttribPointer, R_int2uint(index), mb->divisor,
                                type, GL_FALSE, 0, NULL);
}

static void bind_mesh_buffer(R_MeshBuffer *mb, unsigned int buffer, int index)
{
    if (index < 0) {
        bind_index_buffer(mb, buffer);
    }
    else {
        bind_vertex_buffer(mb, buffer, index);
    }
}

void R_bind_mesh_buffer_by_index(void *subject, R_UserData user,
                                 unsigned int buffer, int index)
{
    bind_mesh_buffer(R_mesh_buffer_by_index(subject, user.i), buffer, index);
}

void R_bind_mesh_buffer_by_name(void *subject, R_UserData user,
                                unsigned int buffer, int index)
{
    bind_mesh_buffer(R_mesh_buffer_by_name(subject, user.data), buffer, index);
}
