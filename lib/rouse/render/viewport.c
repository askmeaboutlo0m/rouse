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
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "../3rdparty/parson.h"
#include "../common.h"
#include "gl.h"
#include "viewport.h"
#include "../anim/seq.h"
#include "../main.h"


static R_Viewport window_viewport = {0, 0, 0, 0};


void R_viewport_set(R_Viewport viewport)
{
    R_GL_CLEAR_ERROR();
    R_GL(glViewport, viewport.x, viewport.y, viewport.w, viewport.h);
}

void R_viewport_reset(void)
{
    R_viewport_set(window_viewport);
}


R_Viewport R_window_viewport(void)
{
    return window_viewport;
}


static void get_window_size(float *out_w, float *out_h)
{
    int w, h;
    SDL_GL_GetDrawableSize(R_window, &w, &h);
    *out_w = R_int2float(w);
    *out_h = R_int2float(h);
}

void R_window_viewport_resize(void)
{
    float x, y, w, h;
    get_window_size(&w, &h);

    if (R_height * (w / R_width) <= h) {
        float ratio = w / R_width;
        x = 0.0f;
        y = h - R_height * ratio;
    }
    else {
        float ratio = h / R_height;
        x = w - R_width * ratio;
        y = 0.0f;
    }

    window_viewport.x = R_long2int(lroundf(x * 0.5f));
    window_viewport.y = R_long2int(lroundf(y * 0.5f));
    window_viewport.w = R_long2int(lroundf(w - x));
    window_viewport.h = R_long2int(lroundf(h - y));
}
