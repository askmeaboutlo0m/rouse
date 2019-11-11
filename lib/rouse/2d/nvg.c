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
#include "refcount.h"
#include "nvg.h"

struct R_Nvg {
    R_MAGIC_FIELD
    int        refs;
    NVGcontext *ctx;
};

static inline void check_nvg(R_Nvg *nvg)
{
    R_MAGIC_CHECK(nvg);
    assert(nvg->refs > 0 && "nvg refcount must always be positive");
}


static NVGcontext *make_context(int flags)
{
    NVGcontext *ctx = nvgCreateGLES2(flags);
    if (ctx) {
        return ctx;
    }
    else {
        R_die("Can't create NanoVG GL ES 2.0 context with flags %d", flags);
    }
}

R_Nvg *R_nvg_new(int flags)
{
    R_Nvg *nvg = R_NEW_INIT_STRUCT(nvg, R_Nvg,
            R_MAGIC_INIT(nvg) 1, make_context(flags));
    check_nvg(nvg);
    return nvg;
}


static void free_nvg(R_Nvg *nvg)
{
    nvgDeleteGLES2(nvg->ctx);
    R_MAGIC_POISON_NN(nvg);
    free(nvg);
}

R_DEFINE_REFCOUNT_FUNCS(R_Nvg, nvg, refs)


NVGcontext *R_nvg_context(R_Nvg *nvg)
{
    check_nvg(nvg);
    return nvg->ctx;
}
