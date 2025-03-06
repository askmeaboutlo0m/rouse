/*
 * render/frame_renderer.h - translate a frame buffer to another or the screen.
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

#define R_BLEND_DIRECT     0
#define R_BLEND_NORMAL     1
#define R_BLEND_ERASE      2
#define R_BLEND_RECOLOR    3
#define R_BLEND_MULTIPLY   4
#define R_BLEND_SCREEN     5
#define R_BLEND_HARD_LIGHT 6
#define R_BLEND_OVERLAY    7
#define R_BLEND_ADD        8
#define R_BLEND_SUBTRACT   9
#define R_BLEND_SHINE      10


typedef struct R_FrameRenderer R_FrameRenderer;

R_FrameRenderer *R_frame_renderer_new(bool alpha_blending);

void R_frame_renderer_free(R_FrameRenderer *fr);

void R_frame_renderer_draw(R_FrameRenderer *fr, R_FrameBuffer *fb);


typedef struct R_FrameCompositor R_FrameCompositor;

R_FrameCompositor *R_frame_compositor_new(void);

void R_frame_compositor_free(R_FrameCompositor *fc);

void R_frame_compositor_draw(R_FrameCompositor *fc, R_FrameBuffer *fb_bottom,
                             R_FrameBuffer *fb_top, int blend_mode,
                             float opacity, bool premultiplied);
