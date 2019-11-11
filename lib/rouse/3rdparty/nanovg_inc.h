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

#define NANOVG_GLES2
#include "nanovg/nanovg.h"

/*
 * nanovg_gl.h uses GLuint, which in the end is just a silly typedef for an
 * unsigned int. Including massive GL headers just for that one type would be
 * pretty excessive, so let's just #define it away. In the unlikely case it's
 * already been #defined, just include the header without doing anything.
 */
#ifndef GLuint
#   define GLuint unsigned int
#   include "nanovg/nanovg_gl.h"
#   undef GLuint
#else
#   include "nanovg/nanovg_gl.h"
#endif


/*
 * NVGcontexts must be freed *after* every `R_BitmapImage` associated with them,
 * otherwise you get segfaults. This trickery allows giving a nice error message
 * if you accidentally do it the wrong way round. See `bitmap.c` for more.
 */
#ifdef ROUSE_DEBUG

void R_bitmap_image_debug_nvg_context_add(NVGcontext *vg);
void R_bitmap_image_debug_nvg_context_remove(NVGcontext *vg);

static inline NVGcontext *R_nvg_debug_context_new(int flags)
{
    NVGcontext *vg = nvgCreateGLES2(flags);
    R_bitmap_image_debug_nvg_context_add(vg);
    return vg;
}

static inline void R_nvg_debug_context_free(NVGcontext *vg)
{
    R_bitmap_image_debug_nvg_context_remove(vg);
    nvgDeleteGLES2(vg);
}

#define nvgCreateGLES2 R_nvg_debug_context_new
#define nvgDeleteGLES2 R_nvg_debug_context_free

#endif


static inline void R_nvg_transform(NVGcontext *vg, const float m[static 6])
{
    nvgTransform(vg, m[0], m[1], m[2], m[3], m[4], m[5]);
}

static inline void R_nvg_transform_set(NVGcontext *vg, const float m[static 6])
{
    nvgResetTransform(vg);
    R_nvg_transform(vg, m);
}

static inline void R_nvg_transform_set_2(NVGcontext *vg, const float m[static 6],
                                         const float n[static 6])
{
    nvgResetTransform(vg);
    R_nvg_transform(vg, m);
    R_nvg_transform(vg, n);
}
