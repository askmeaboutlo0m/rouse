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
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../3rdparty/nanovg_inc.h"
#include "../common.h"
#include "../render/gl.h"
#include "nvg.h"
#include "refcount.h"
#include "bitmap.h"

struct R_BitmapImage {
    R_MAGIC_FIELD
    int   refs;
    R_Nvg *nvg;
    int   handle;
    int   width;
    int   height;
};


static inline void check_bitmap_image(R_BitmapImage *bi)
{
    R_MAGIC_CHECK(bi);
    R_assert(bi->refs > 0, "refcount must always be positive");
}

static int make_handle(NVGcontext *ctx, const char *name, int width, int height,
                       const unsigned char *pixels, int flags)
{
    int handle = nvgCreateImageRGBA(ctx, width, height, flags, pixels);
    if (handle == 0) {
        if (name) {
            R_die("Error loading bitmap image '%s' with size %dx%d, flags %d",
                  name, width, height, flags);
        }
        else {
            R_die("Error loading bitmap image with size %dx%d, flags %d",
                  width, height, flags);
        }
    }
    else {
        return handle;
    }
}

static R_BitmapImage *make_bitmap_image(R_Nvg *nvg, NVGcontext *ctx, int handle)
{
    int width, height;
    nvgImageSize(ctx, handle, &width, &height);
    R_BitmapImage *bi = R_NEW_INIT_STRUCT(bi, R_BitmapImage,
            R_MAGIC_INIT(bi) 1, R_nvg_incref(nvg), handle, width, height);
    check_bitmap_image(bi);
    return bi;
}

R_BitmapImage *R_bitmap_image_new(R_Nvg *nvg, const char *name, int width,
                                  int height, const unsigned char *pixels,
                                  int flags)
{
    NVGcontext *ctx   = R_nvg_context(nvg);
    int        handle = make_handle(ctx, name, width, height, pixels, flags);
    return make_bitmap_image(nvg, ctx, handle);
}

R_BitmapImage *R_bitmap_image_from_file(R_Nvg *nvg, const char *path,
                                        int flags)
{
    SDL_Surface   *sfc = R_surface_load(path, SDL_PIXELFORMAT_RGBA32);
    R_BitmapImage *bi  = R_bitmap_image_new(nvg, path, sfc->w, sfc->h,
                                            sfc->pixels, flags);
    SDL_FreeSurface(sfc);
    return bi;
}


static void free_context(R_Nvg *nvg, int handle)
{
    if (nvg) {
        NVGcontext *vg = R_nvg_context(nvg);
        nvgDeleteImage(vg, handle);
        R_nvg_decref(nvg);
    }
}

static void free_bitmap_image(R_BitmapImage *bi)
{
    free_context(bi->nvg, bi->handle);
    R_MAGIC_POISON_NN(bi);
    free(bi);
}

R_DEFINE_REFCOUNT_FUNCS(R_BitmapImage, bitmap_image, refs)


int R_bitmap_image_width (R_BitmapImage *bi)
{
    check_bitmap_image(bi);
    return bi->width;
}

int R_bitmap_image_height(R_BitmapImage *bi)
{
    check_bitmap_image(bi);
    return bi->height;
}


static NVGpaint make_paint(NVGcontext *ctx, float w, float h, int handle)
{
    return nvgImagePattern(ctx, 0.0f, 0.0f, w, h, 0.0f, handle, 1.0f);
}

void R_bitmap_image_draw(R_BitmapImage *bi, NVGcontext *ctx,
                         const float matrix[static 6])
{
    check_bitmap_image(bi);
    R_assert(ctx == R_nvg_context(bi->nvg), "nanovg context must match");
    R_nvg_transform_set(ctx, matrix);
    float w = R_int2float(bi->width);
    float h = R_int2float(bi->height);
    nvgBeginPath(ctx);
    nvgFillPaint(ctx, make_paint(ctx, w, h, bi->handle));
    nvgRect(ctx, 0.0f, 0.0f, w, h);
    nvgFill(ctx);
}
