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
#include <assert.h>
#include <stdio.h>
#include <cglm/struct.h>
#include "../3rdparty/gles2_inc.h"
#include "../common.h"
#include "../geom.h"
#include "gl.h"
#include "binder.h"
#include "frame_buffer.h"
#include "frame_renderer.h"


struct R_FrameRenderer {
    R_MAGIC_FIELD
    R_Binder *binder;
    int      u_ratio, u_sampler;
};


static const char *vert =
    "#version 100\n"
    "uniform vec2 u_ratio;"
    "attribute mediump vec2 v_xy;\n"
    "varying vec2 f_uv;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(v_xy, 0.0, 1.0);\n"
    "    f_uv        = (v_xy + vec2(1.0, 1.0)) * 0.5 * u_ratio;\n"
    "}\n";

static const char *frag =
    "#version 100\n"
    "precision mediump float;\n"
    "uniform sampler2D u_sampler;\n"
    "varying vec2 f_uv;\n"
    "void main()\n"
    "{\n" \
    "    gl_FragColor = texture2D(u_sampler, f_uv);\n"
    "}\n";


static void bind(R_UNUSED void *subject, R_UNUSED R_UserData user,
                 unsigned int buffer, int index)
{
    static const float vertices[8] =
        {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f};
    R_GL(glBindBuffer, GL_ARRAY_BUFFER, buffer);
    R_GL(glBufferData, GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    R_GL(glVertexAttribPointer, R_int2uint(index), 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

static void draw(R_UNUSED void *subject, R_UNUSED R_UserData user)
{
    R_GL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
}


R_FrameRenderer *R_frame_renderer_new(bool alpha_blending)
{
    R_Binder *binder = R_binder_new(0, draw, R_user_null(), vert, frag,
                                    bind, R_user_null(), NULL);
    if (alpha_blending) {
        binder->features |= R_BINDER_BLENDING;
    }

    R_FrameRenderer *fr = R_NEW_INIT_STRUCT(fr, R_FrameRenderer,
            R_MAGIC_INIT(R_FrameRenderer) binder,
            R_binder_uniform_location(binder, "u_ratio"),
            R_binder_uniform_location(binder, "u_sampler"));
    R_MAGIC_CHECK(R_FrameRenderer, fr);
    return fr;
}

void R_frame_renderer_free(R_FrameRenderer *fr)
{
    if (fr) {
        R_MAGIC_CHECK(R_FrameRenderer, fr);
        R_binder_free(fr->binder);
        R_MAGIC_POISON(R_FrameRenderer, fr);
        free(fr);
    }
}


void R_frame_renderer_draw(R_FrameRenderer *fr, R_FrameBuffer *fb)
{
    R_MAGIC_CHECK(R_FrameRenderer, fr);
    R_MAGIC_CHECK(R_FrameBuffer, fb);
    R_binder_begin(fr->binder);

    R_GL(glActiveTexture, GL_TEXTURE0);
    R_GL(glBindTexture, GL_TEXTURE_2D, fb->color);
    R_GL(glUniform1i, fr->u_sampler, 0);

    R_V2 ratio = R_frame_buffer_ratio(fb);
    R_GL(glUniform2f, fr->u_ratio, ratio.x, ratio.y);

    R_binder_draw(fr->binder, NULL);
    R_binder_end(fr->binder);
}
