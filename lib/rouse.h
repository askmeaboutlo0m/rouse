/*
 * rouse.h - another wrong animation library
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
#ifndef ROUSE_H_INCLUDED
#define ROUSE_H_INCLUDED

#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include <cglm/struct.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "rouse/3rdparty/gles2_inc.h"
#include "rouse/3rdparty/nanovg_inc.h"
#include "rouse/3rdparty/parson.h"

#include <rouse_config.h>
#include "rouse/common.h"
#include "rouse/string.h"
#include "rouse/stringify.h"
#include "rouse/geom.h"
#include "rouse/json.h"
#include "rouse/sanity.h"
#include "rouse/camera.h"
#include "rouse/parse.h"
#include "rouse/model.h"
#include "rouse/resource.h"
#include "rouse/render/gl.h"
#include "rouse/render/viewport.h"
#include "rouse/render/binder.h"
#include "rouse/render/frame_buffer.h"
#include "rouse/render/frame_renderer.h"
#include "rouse/anim/ease.h"
#include "rouse/anim/util.h"
#include "rouse/anim/seq.h"
#include "rouse/anim/call.h"
#include "rouse/anim/delay.h"
#include "rouse/anim/tween.h"
#include "rouse/2d/nvg.h"
#include "rouse/2d/bitmap.h"
#include "rouse/2d/text.h"
#include "rouse/2d/vector.h"
#include "rouse/2d/sprite.h"
#include "rouse/2d/canvas.h"
#include "rouse/2d/sprite_anim.h"
#include "rouse/interact/input.h"
#include "rouse/main.h"

#ifdef __cplusplus
}
#endif

#endif
