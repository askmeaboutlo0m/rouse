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
#include <assert.h>
#include <stdio.h>
#include <cglm/struct.h>
#include "../3rdparty/gles2_inc.h"
#include <rouse_config.h>
#include "../common.h"
#include "../geom.h"
#include "gl.h"
#include "frame_buffer.h"
#include "frame_renderer.h"


struct R_FrameRenderer {
    R_MAGIC_FIELD
    bool         alpha_blending;
    unsigned int program;
    unsigned int buffer;
    int          u_ratio, u_sampler;
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

static const float verts[8] =
    {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f};


R_FrameRenderer *R_frame_renderer_new(bool alpha_blending)
{
    unsigned int program = R_gl_program_new(vert, frag);
    unsigned int buffer;
    R_GL(glGenBuffers, 1, &buffer);

    R_FrameRenderer *fr = R_NEW_INIT_STRUCT(fr, R_FrameRenderer,
            R_MAGIC_INIT(R_FrameRenderer) alpha_blending, program, buffer,
            R_gl_uniform_location(program, "u_ratio"),
            R_gl_uniform_location(program, "u_sampler"));
    R_MAGIC_CHECK(R_FrameRenderer, fr);
    return fr;
}

void R_frame_renderer_free(R_FrameRenderer *fr)
{
    if (fr) {
        R_MAGIC_CHECK(R_FrameRenderer, fr);
        R_GL(glDeleteBuffers, 1, &fr->buffer);
        R_gl_program_free(fr->program);
        R_MAGIC_POISON(R_FrameRenderer, fr);
        free(fr);
    }
}


void R_frame_renderer_draw(R_FrameRenderer *fr, R_FrameBuffer *fb)
{
    R_MAGIC_CHECK(R_FrameRenderer, fr);
    R_MAGIC_CHECK(R_FrameBuffer, fb);
    R_GL(glDisable, GL_DEPTH_TEST);
    R_GL(glDisable, GL_CULL_FACE);
    if (fr->alpha_blending) {
        R_GL(glEnable, GL_BLEND);
        R_GL(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        R_GL(glDisable, GL_BLEND);
    }

    R_GL(glUseProgram, fr->program);
    R_GL(glActiveTexture, GL_TEXTURE0);
    R_GL(glBindTexture, GL_TEXTURE_2D, fb->color);
    R_GL(glUniform1i, fr->u_sampler, 0);

    R_V2 ratio = R_frame_buffer_ratio(fb);
    R_GL(glUniform2f, fr->u_ratio, ratio.x, ratio.y);

    R_GL(glEnableVertexAttribArray, 0);
    R_GL(glBindBuffer, GL_ARRAY_BUFFER, fr->buffer);
    R_GL(glBufferData, GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    R_GL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    int max = R_gl_max_vertex_attribs;
    for (int i = 1; i < max; ++i) {
        R_GL(glDisableVertexAttribArray, R_int2uint(i));
    }

    R_GL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
}
