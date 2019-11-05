/*
 * binder.h - binding all those buffers before rendering
 *
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

typedef struct R_Binder R_Binder;

typedef void (*R_BinderDraw)(void *, R_UserData);
typedef void (*R_BinderBind)(void *, R_UserData, unsigned int, int);

#define R_BINDER_INDEXED    0x1
#define R_BINDER_DEPTH_TEST 0x2
#define R_BINDER_CULLING    0x4
#define R_BINDER_BLENDING   0x8

#define R_BINDER_DEFAULT_DEPTH_TEST GL_LESS
#define R_BINDER_DEFAULT_CULL_FACE  GL_BACK
#define R_BINDER_DEFAULT_SFACTOR    GL_SRC_ALPHA
#define R_BINDER_DEFAULT_DFACTOR    GL_ONE_MINUS_SRC_ALPHA

struct R_Binder {
    R_BinderDraw draw;
    R_UserData   arg;
    unsigned int features;
    unsigned int program;
    unsigned int depth_test;
    unsigned int cull_face;
    unsigned int sfactor, dfactor;
    struct {
        int          count;
        unsigned int *buffers;
        R_BinderBind *fns;
        R_UserData   *args;
    } attribute;
};


R_Binder *R_binder_new(unsigned int features, R_BinderDraw draw, R_UserData arg,
                       const char *vert, const char *frag, ...);
void R_binder_free(R_Binder *);

void R_binder_begin(R_Binder *binder);
void R_binder_end(R_Binder *binder);


int R_binder_uniform_location(R_Binder *binder, const char *name);


void R_binder_draw(R_Binder *binder, void *subject);

void R_binder_draw_mesh_by_index(void *subject, R_UserData arg);

void R_binder_draw_mesh_by_name(void *subject, R_UserData arg);


void R_bind_mesh_buffer_by_index(void *subject, R_UserData user,
                                 unsigned int buffer, int index);

void R_bind_mesh_buffer_by_name(void *subject, R_UserData user,
                                unsigned int buffer, int index);
