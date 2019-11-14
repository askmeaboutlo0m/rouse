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
#include <stddef.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <assert.h>
#include <cglm/struct.h>
#include <GL/glew.h>
#include "../3rdparty/nanovg_inc.h"
#include "../common.h"
#include "../string.h"
#include "../geom.h"
#include "../parse.h"
#include "../render/gl.h"
#include "../render/frame_buffer.h"
#include "nvg.h"
#include "bitmap.h"
#include "text.h"
#include "vector.h"
#include "sprite.h"
#include "canvas.h"

struct R_Canvas {
    R_MAGIC_FIELD
    int      width, height;
    R_Sprite *sprite;
};


R_FrameBufferOptions R_canvas_frame_buffer_options(int width, int height)
{
    R_FrameBufferOptions options = R_frame_buffer_options();
    options.width        = width;
    options.height       = height;
    options.color_type   = R_FRAME_BUFFER_ATTACHMENT_TEXTURE;
    options.stencil_type = R_FRAME_BUFFER_ATTACHMENT_BUFFER;
    return options;
}

R_Canvas *R_canvas_new(int width, int height)
{
    R_Canvas *canvas = R_NEW_INIT_STRUCT(canvas, R_Canvas,
            R_MAGIC_INIT(canvas) width, height, R_sprite_new_root());
    R_MAGIC_CHECK(canvas);
    return canvas;
}

void R_canvas_free(R_Canvas *canvas)
{
    if (canvas) {
        R_MAGIC_CHECK_NN(canvas);
        R_sprite_decref(canvas->sprite);
        R_MAGIC_POISON(canvas);
        free(canvas);
    }
}


R_Sprite *R_canvas_sprite(R_Canvas *canvas)
{
    R_MAGIC_CHECK(canvas);
    return canvas->sprite;
}


void R_canvas_render(R_Canvas *canvas, R_Nvg *nvg, int target_width,
                     int target_height)
{
    R_MAGIC_CHECK(canvas);

    float w = R_int2float(target_width);
    float h = R_int2float(target_height);

    float matrix[6];
    nvgTransformScale(matrix, w / R_int2float(canvas->width),
                              h / R_int2float(canvas->height));

    NVGcontext *ctx = R_nvg_context(nvg);
    nvgBeginFrame(ctx, w, h, 1.0f);
    R_sprite_draw(canvas->sprite, ctx, matrix);
    nvgEndFrame(ctx);
}
