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
#ifdef ROUSE_DEBUG
#   include "../3rdparty/uthash_inc.h"
#endif
#include "../3rdparty/nanovg_inc.h"
#include "../common.h"
#include "../render/gl.h"
#include "bitmap.h"

struct R_BitmapImage {
    R_MAGIC_FIELD
    NVGcontext *vg;
    int        handle;
    int        width;
    int        height;
};


/*
 * Bitmap images must be freed *before* the NVGcontext they're associated with,
 * otherwise you get a segfault when attempting to release the image handle.
 * Just segfaulting on shutdown is of course terrible error conveyance, so in
 * debug mode we instead keep track of all allocated NVGcontexts here. If an
 * image is freed after the context has been, you get a nice error message
 * that way. See `nanovg_inc.h` for more.
 */
#ifdef ROUSE_DEBUG

struct NvgContextEntry {
    NVGcontext    *vg;
    UT_hash_handle hh;
};

static struct NvgContextEntry *contexts;

static struct NvgContextEntry *search_context(NVGcontext *key)
{
    if (key) {
        struct NvgContextEntry *entry;
        HASH_FIND_PTR(contexts, &key, entry);
        return entry;
    }
    else {
        return NULL;
    }
}

void R_bitmap_image_debug_nvg_context_add(NVGcontext *vg)
{
    if (vg && !search_context(vg)) {
        R_debug("add NVGcontext %p", (void *)vg);
        struct NvgContextEntry *entry = R_NEW_INIT_STRUCT(
                entry, struct NvgContextEntry, vg, R_NULL_UT_HH);
        HASH_ADD_PTR(contexts, vg, entry);
    }
}

void R_bitmap_image_debug_nvg_context_remove(NVGcontext *vg)
{
    struct NvgContextEntry *entry = search_context(vg);
    if (entry) {
        R_debug("remove NVGcontext %p", (void *)vg);
        HASH_DEL(contexts, entry);
        free(entry);
    }
}

#endif


static int make_handle(NVGcontext *vg, const char *name, int width, int height,
                       const unsigned char *pixels, int flags)
{
    int handle = nvgCreateImageRGBA(vg, width, height, flags, pixels);
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

static R_BitmapImage *make_bitmap_image(NVGcontext *vg, int handle)
{
    int width, height;
    nvgImageSize(vg, handle, &width, &height);
    R_BitmapImage *bi = R_NEW_INIT_STRUCT(bi, R_BitmapImage,
            R_MAGIC_INIT(bi) vg, handle, width, height);
    R_MAGIC_CHECK(bi);
    return bi;
}

R_BitmapImage *R_bitmap_image_new(NVGcontext *vg, const char *name, int width,
                                  int height, const unsigned char *pixels,
                                  int flags)
{
    int handle = make_handle(vg, name, width, height, pixels, flags);
    return make_bitmap_image(vg, handle);
}

R_BitmapImage *R_bitmap_image_from_file(NVGcontext *vg, const char *path,
                                        int flags)
{
    SDL_Surface   *sfc = R_surface_load(path, SDL_PIXELFORMAT_RGBA32);
    R_BitmapImage *bi  = R_bitmap_image_new(vg, path, sfc->w, sfc->h,
                                            sfc->pixels, flags);
    SDL_FreeSurface(sfc);
    return bi;
}


static void free_handle(R_BitmapImage *bi)
{
    NVGcontext *vg = bi->vg;
    if (vg) {
#ifdef ROUSE_DEBUG
        if (!search_context(vg)) {
            R_die("BUG: looks like you freed your NVGcontext at %p before "
                  "freeing the R_BitmapImage at %p with image handle %d "
                  "associated with that context. Free your R_BitmapImage "
                  "first, otherwise it can't release its image handle. In "
                  "release mode, this would probably have segfaulted instead.",
                  (void *)vg, (void *)bi, bi->handle);
        }
#endif
        nvgDeleteImage(vg, bi->handle);
    }
}

void R_bitmap_image_free(R_BitmapImage *bi)
{
    if (bi) {
        R_MAGIC_CHECK_NN(bi);
        free_handle(bi);
        R_MAGIC_POISON_NN(bi);
        free(bi);
    }
}


int R_bitmap_image_width (R_BitmapImage *bi)
{
    R_MAGIC_CHECK(bi);
    return bi->width;
}

int R_bitmap_image_height(R_BitmapImage *bi)
{
    R_MAGIC_CHECK(bi);
    return bi->height;
}


static NVGpaint make_paint(NVGcontext *vg, float w, float h, int handle)
{
    return nvgImagePattern(vg, 0.0f, 0.0f, w, h, 0.0f, handle, 1.0f);
}

void R_bitmap_image_draw(R_BitmapImage *bi, NVGcontext *vg,
                         const float matrix[static 6])
{
    R_MAGIC_CHECK(bi);
    assert(vg == bi->vg && "nanovg context must match bitmap image's");
    R_nvg_transform_set(vg, matrix);
    float w = R_int2float(bi->width);
    float h = R_int2float(bi->height);
    nvgBeginPath(vg);
    nvgFillPaint(vg, make_paint(vg, w, h, bi->handle));
    nvgRect(vg, 0.0f, 0.0f, w, h);
    nvgFill(vg);
}
