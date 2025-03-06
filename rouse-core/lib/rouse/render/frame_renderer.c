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

#define R_FRAME_RENDERER_STR(X)  #X
#define R_FRAME_RENDERER_XSTR(X) R_FRAME_RENDERER_STR(X)

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

static const char *fr_frag =
    "#version 100\n"
    "precision mediump float;\n"
    "uniform sampler2D u_sampler;\n"
    "varying vec2 f_uv;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = texture2D(u_sampler, f_uv);\n"
    "}\n";

static const char *fc_frag =
    "#version 100\n"
    "#define R_BLEND_DIRECT     " R_FRAME_RENDERER_XSTR(R_BLEND_DIRECT    ) "\n"
    "#define R_BLEND_NORMAL     " R_FRAME_RENDERER_XSTR(R_BLEND_NORMAL    ) "\n"
    "#define R_BLEND_ERASE      " R_FRAME_RENDERER_XSTR(R_BLEND_ERASE     ) "\n"
    "#define R_BLEND_RECOLOR    " R_FRAME_RENDERER_XSTR(R_BLEND_RECOLOR   ) "\n"
    "#define R_BLEND_MULTIPLY   " R_FRAME_RENDERER_XSTR(R_BLEND_MULTIPLY  ) "\n"
    "#define R_BLEND_SCREEN     " R_FRAME_RENDERER_XSTR(R_BLEND_SCREEN    ) "\n"
    "#define R_BLEND_HARD_LIGHT " R_FRAME_RENDERER_XSTR(R_BLEND_HARD_LIGHT) "\n"
    "#define R_BLEND_OVERLAY    " R_FRAME_RENDERER_XSTR(R_BLEND_OVERLAY   ) "\n"
    "#define R_BLEND_ADD        " R_FRAME_RENDERER_XSTR(R_BLEND_ADD       ) "\n"
    "#define R_BLEND_SUBTRACT   " R_FRAME_RENDERER_XSTR(R_BLEND_SUBTRACT  ) "\n"
    "#define R_BLEND_SHINE      " R_FRAME_RENDERER_XSTR(R_BLEND_SHINE     ) "\n"
    "#define R_COMP_OP_SEPARABLE(OP) \\\n"
    "   cb /= ab; \\\n"
    "   cs /= s.a; \\\n"
    "   gl_FragColor = vec4((cb * (1.0 - as) + vec3(OP(cb.r, cs.r), \\\n"
    "                  OP(cb.g, cs.g), OP(cb.b, cs.b)) * as) * ab, ab)\n"
    "precision mediump float;\n"
    "uniform sampler2D u_bottom;\n"
    "uniform sampler2D u_top;\n"
    "uniform int u_blend_mode;\n"
    "uniform float u_opacity;\n"
    "varying vec2 f_uv;\n"
    "\n"
    "float multiply(float a, float b)\n"
    "{\n"
    "    return a * b;\n"
    "}\n"
    "\n"
    "float screen(float a, float b)\n"
    "{\n"
    "    return 1.0 - (1.0 - a) * (1.0 - b);\n"
    "}\n"
    "\n"
    "float hard_light(float a, float b)\n"
    "{\n"
    "    float b2 = b * 2.0;"
    "    return b2 <= 1.0 ? multiply(a, b2) : screen(a, b2 - 1.0);\n"
    "}\n"
    "\n"
    "float overlay(float a, float b)\n"
    "{\n"
    "    return hard_light(b, a);\n"
    "}\n"
    "float add(float a, float b)\n"
    "{\n"
    "    return min(1.0, a + b);\n"
    "}\n"
    "float subtract(float a, float b)\n"
    "{\n"
    "    return max(0.0, a - b);\n"
    "}\n"
    "\n"
    "float shine(float a, float b, float o)\n"
    "{\n"
    "    return add(a, b * o);\n"
    "}\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4  b  = texture2D(u_bottom, f_uv);\n"
    "    vec3  cb = b.rgb;\n"
    "    float ab = b.a;\n"
    "    float o  = u_opacity;\n"
    "    if (u_blend_mode == R_BLEND_DIRECT) {\n"
    "        gl_FragColor = vec4(cb, ab * o);\n"
    "    }\n"
    "    else {\n"
    "        vec4  s  = texture2D(u_top, f_uv);\n"
    "        vec3  cs = s.rgb;\n"
    "        float as = s.a * o;\n"
    "        if (as <= 0.0) {\n"
    "            gl_FragColor = b;\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_NORMAL) {\n"
    "            gl_FragColor = vec4(cs + cb * (1.0 - as), as + ab * (1.0 - as));\n"
    "        }\n"
    "        else if (ab <= 0.0) {\n"
    "            gl_FragColor = vec4(0.0);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_ERASE) {\n"
    "            gl_FragColor = b * (1.0 - as);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_RECOLOR) {\n"
    "            gl_FragColor = vec4(cb * (1.0 - as) + cs * ab * o, ab);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_MULTIPLY) {\n"
    "            R_COMP_OP_SEPARABLE(multiply);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_SCREEN) {\n"
    "            R_COMP_OP_SEPARABLE(screen);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_HARD_LIGHT) {\n"
    "            R_COMP_OP_SEPARABLE(hard_light);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_OVERLAY) {\n"
    "            R_COMP_OP_SEPARABLE(overlay);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_ADD) {\n"
    "            R_COMP_OP_SEPARABLE(add);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_SUBTRACT) {\n"
    "            R_COMP_OP_SEPARABLE(subtract);\n"
    "        }\n"
    "        else if (u_blend_mode == R_BLEND_SHINE) {\n"
    "            cb /= ab;\n"
    "            cs /= s.a;\n"
    "            gl_FragColor = vec4(\n"
    "                vec3(shine(cb.r, cs.r, as), shine(cb.g, cs.g, as),\n"
    "                     shine(cb.b, cs.b, as)) * ab, ab);\n"
    "        }\n"
    "        else {\n"
    "            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "        }\n"
    "    }\n"
    "}\n";

static const float verts[8] =
    {-1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f};


struct R_FrameRenderer {
    R_MAGIC_FIELD
    bool         alpha_blending;
    unsigned int program;
    unsigned int buffer;
    int          u_ratio, u_sampler;
};

R_FrameRenderer *R_frame_renderer_new(bool alpha_blending)
{
    unsigned int program = R_gl_program_new(vert, fr_frag);
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


static void draw_frame(bool alpha_blending, bool premultiplied,
                       unsigned int program, unsigned int buffer, int u_ratio,
                       int u_sampler1, int u_sampler2, int u_blend_mode,
                       int u_opacity, int blend_mode, float opacity,
                       R_FrameBuffer *fb1, R_FrameBuffer *fb2)
{
    R_GL(glDisable, GL_DEPTH_TEST);
    R_GL(glDisable, GL_CULL_FACE);
    if (alpha_blending) {
        R_GL(glEnable, GL_BLEND);
        if (premultiplied) {
            R_GL(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else {
            R_GL(glBlendFuncSeparate, GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
                                      GL_ONE_MINUS_SRC_ALPHA);
        }
    }
    else {
        R_GL(glDisable, GL_BLEND);
    }

    R_GL(glUseProgram, program);
    R_GL(glActiveTexture, GL_TEXTURE0);
    R_GL(glBindTexture, GL_TEXTURE_2D, fb1->color);
    R_GL(glUniform1i, u_sampler1, 0);

    R_V2 ratio = R_frame_buffer_ratio(fb1);
    R_GL(glUniform2f, u_ratio, ratio.x, ratio.y);

    if (fb2) {
        R_assert(fb1->width == fb2->width, "Frame width");
        R_assert(fb1->height == fb2->height, "Frame height");
        R_assert(fb1->real_width == fb2->real_width, "Frame real width");
        R_assert(fb1->real_height == fb2->real_height, "Frame real height");
        R_GL(glActiveTexture, GL_TEXTURE1);
        R_GL(glBindTexture, GL_TEXTURE_2D, fb2->color);
        R_GL(glUniform1i, u_sampler2, 1);
    }

    if(u_opacity != -1) {
        R_GL(glUniform1i, u_blend_mode, blend_mode);
        R_GL(glUniform1f, u_opacity, opacity);
    }

    R_GL(glEnableVertexAttribArray, 0);
    R_GL(glBindBuffer, GL_ARRAY_BUFFER, buffer);
    R_GL(glBufferData, GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    R_GL(glVertexAttribPointer, 0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    int max = R_gl_max_vertex_attribs;
    for (int i = 1; i < max; ++i) {
        R_GL(glDisableVertexAttribArray, R_int2uint(i));
    }

    R_GL(glDrawArrays, GL_TRIANGLE_STRIP, 0, 4);
}

void R_frame_renderer_draw(R_FrameRenderer *fr, R_FrameBuffer *fb)
{
    R_MAGIC_CHECK(R_FrameRenderer, fr);
    R_MAGIC_CHECK(R_FrameBuffer, fb);
    draw_frame(fr->alpha_blending, true, fr->program, fr->buffer, fr->u_ratio,
               fr->u_sampler, -1, -1, -1, 0, 0.0f, fb, NULL);
}


struct R_FrameCompositor {
    R_MAGIC_FIELD
    unsigned int program;
    unsigned int buffer;
    int          u_ratio, u_blend_mode, u_opacity, u_bottom, u_top;
};

R_FrameCompositor *R_frame_compositor_new(void)
{
    unsigned int program = R_gl_program_new(vert, fc_frag);
    unsigned int buffer;
    R_GL(glGenBuffers, 1, &buffer);

    R_FrameCompositor *fc = R_malloc(sizeof(*fc));
    *fc = (R_FrameCompositor){R_MAGIC_INIT(R_FrameCompositor) program, buffer,
                              R_gl_uniform_location(program, "u_ratio"),
                              R_gl_uniform_location(program, "u_blend_mode"),
                              R_gl_uniform_location(program, "u_opacity"),
                              R_gl_uniform_location(program, "u_bottom"),
                              R_gl_uniform_location(program, "u_top")};
    R_MAGIC_CHECK(R_FrameCompositor, fc);
    return fc;
}

void R_frame_compositor_free(R_FrameCompositor *fc)
{
    if (fc) {
        R_MAGIC_CHECK(R_FrameCompositor, fc);
        R_GL(glDeleteBuffers, 1, &fc->buffer);
        R_gl_program_free(fc->program);
        R_MAGIC_POISON(R_FrameCompositor, fc);
        free(fc);
    }
}

void R_frame_compositor_draw(R_FrameCompositor *fc, R_FrameBuffer *fb_bottom,
                             R_FrameBuffer *fb_top, int blend_mode,
                             float opacity, bool premultiplied)
{
    R_MAGIC_CHECK(R_FrameCompositor, fc);
    R_MAGIC_CHECK(R_FrameBuffer, fb_bottom);
    if(fb_top) {
        R_MAGIC_CHECK(R_FrameBuffer, fb_top);
    } else {
        R_assert(blend_mode == R_BLEND_DIRECT,
                 "Blend mode without top must be direct");
    }
    draw_frame(blend_mode == R_BLEND_DIRECT, premultiplied, fc->program,
               fc->buffer, fc->u_ratio, fc->u_bottom, fc->u_top,
               fc->u_blend_mode, fc->u_opacity, blend_mode, opacity, fb_bottom,
               fb_top);
}
