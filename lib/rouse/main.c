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
#include <stdnoreturn.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <cglm/struct.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "3rdparty/uthash.h"
#include "common.h"
#include "geom.h"
#include "resource.h"
#include "anim/seq.h"
#include "anim/util.h"
#include "render/gl.h"
#include "render/viewport.h"
#include "stringify.h"
#include "main.h"

#ifdef __EMSCRIPTEN__
#   include <emscripten.h>
#endif

/* Make sure our environment is sane at compile-time. */
#include <assert.h>
#include "sanity.h"


SDL_Window    *R_window;
SDL_GLContext R_glcontext;

float    R_tick_length             = R_TICK_LENGTH_DEFAULT;
uint32_t R_max_ticks_before_render = R_MAX_TICKS_BEFORE_RENDER_DEFAULT;
float    R_width                   = R_WIDTH_DEFAULT;
float    R_height                  = R_HEIGHT_DEFAULT;

#ifdef ROUSE_MAGIC
uint32_t R_magic_numbers[R_MAGIC_NUMBER_COUNT];
#endif

static R_Scene   *current_scene  = NULL;
static R_SceneFn next_scene_fn   = NULL;
static void      *next_scene_arg = NULL;



void R_framerate_set(float ticks_per_second)
{
    R_tick_length = 1000.0f / ticks_per_second;
}


static bool initialized = false;

static void init(const char *title, int width, int height)
{
    if (initialized) {
        R_die("Attempt to call R_main twice");
    }

    initialized = true;
    R_seed = (unsigned int) time(NULL);

#ifdef ROUSE_MAGIC
    for (unsigned int i = 0; i < R_LENGTH(R_magic_numbers); ++i) {
        uint32_t low  = R_int2uint32(R_rand()) % 0xffff;
        uint32_t high = R_int2uint32(R_rand()) % 2 == 0 ? 0xfefeffffu : 0xefefffff;
        R_magic_numbers[i] = high - low;
        R_debug("magic number %u = 0x%x", i, (unsigned int) R_magic_numbers[i]);
    }
#endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
        R_die("SDL_Init: %s", SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        R_die("IMG_Init: %s", IMG_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    R_window = SDL_CreateWindow(
            title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!R_window) {
        R_die("SDL_CreateWindow: %s", SDL_GetError());
    }

    R_glcontext = SDL_GL_CreateContext(R_window);
    if (!R_glcontext) {
        R_die("SDL_GL_CreateContext: %s", SDL_GetError());
    }

    R_gl_init();
    R_window_viewport_resize();
}

static void deinit(void)
{
    R_scene_free(current_scene);
    SDL_GL_DeleteContext(R_glcontext);
    SDL_DestroyWindow(R_window);
    IMG_Quit();
#ifndef R_HAVE_ASAN
    /* ASAN really hates this function and will report a bajillion leaks */
    /* when it's called. Just not calling is fine for it so... whatever. */
    SDL_Quit();
#endif
}


#define R_SPIN_EVENTS_CONTINUE 0
#define R_SPIN_EVENTS_QUIT     1

static int spin_events(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        R_debug("polled event %s", R_stringify_sdl_event_type(event.type));

        switch (event.type) {
            case SDL_QUIT:
                R_debug("SDL_QUIT: quitting");
                return R_SPIN_EVENTS_QUIT;
            case SDL_WINDOWEVENT:
                R_debug("polled window event %s",
                        R_stringify_sdl_window_event_type(event.window.event));
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    R_debug("window size changed, resizing viewport");
                    R_window_viewport_resize();
                }
                break;
            default:
                break;
        }

        if (current_scene && current_scene->on_event) {
            current_scene->on_event(current_scene, &event);
        }
    }
    R_debug("polled events");
    return R_SPIN_EVENTS_CONTINUE;
}


static R_Scene *swap_scene(void)
{
    if (next_scene_fn) {
        R_scene_free(current_scene);
        current_scene  = next_scene_fn(next_scene_arg);
        R_MAGIC_CHECK(current_scene);
        next_scene_fn  = NULL;
        next_scene_arg = NULL;
    }
    return current_scene;
}

struct MainLoop {
    uint32_t last_ms;
    bool     running;
};

static bool step_main_loop(struct MainLoop *ml)
{
    if (spin_events() == R_SPIN_EVENTS_QUIT) {
        ml->running = false;
    }
    else {
        uint32_t ms    = SDL_GetTicks();
        uint32_t delta = ms - ml->last_ms;
        uint32_t ticks = R_float2uint32(R_uint322float(delta) / R_tick_length);
        uint32_t max   = R_MIN(R_max_ticks_before_render, ticks);
        ml->last_ms   += R_float2uint32(R_uint322float(ticks) * R_tick_length);

        R_Scene *scene;
        float   seconds = 1000.0f / R_tick_length;

        for (uint32_t i = 0; i < max; ++i) {
            scene = swap_scene();
            if (scene) {
                bool rendered = i == max - 1;
                R_animator_tick(scene->animator, rendered, seconds);
                if (scene->on_tick) {
                    scene->on_tick(scene, rendered);
                }
            }
        }

        if (max > 0 && (scene = swap_scene()) && scene->on_render) {
            R_GL_CLEAR_ERROR();
            scene->on_render(scene);
            SDL_GL_SwapWindow(R_window);
        }
    }

    return ml->running && swap_scene();
}

#ifdef __EMSCRIPTEN__
static void em_main_loop(void *arg)
{
    struct MainLoop *ml = arg;
    if (!step_main_loop(ml)) {
        free(ml);
        deinit();
        emscripten_cancel_main_loop();
    }
}
#endif

static void main_loop(void)
{
    struct MainLoop ml;
    {
        uint32_t ms  = SDL_GetTicks();
        ml.last_ms = R_uint322float(ms) > R_tick_length
                   ? R_float2uint32(R_uint322float(ms) - R_tick_length)
                   : 0;
        ml.running = true;
    }

#ifdef __EMSCRIPTEN__
    /* Can't use the stack, this function actually returns in Emscripten. */
    struct MainLoop *em_ml = R_NEW_INIT(em_ml, ml);
    emscripten_set_main_loop_arg(em_main_loop, em_ml, 0, false);
#else
    while (step_main_loop(&ml)) {
        /* TODO: Handle timer inaccuracy and oversleep. Windows for example
         * has such inaccurate timers that it sleeps too long and makes the
         * application chug. Instead of sleeping for the whole time between
         * frames, it'd be better to track oversleep, sleep conservatively and
         * spend the rest of the delay busy-waiting. */
        uint32_t ms      = SDL_GetTicks();
        uint32_t next_ms = R_float2uint32(
                R_uint322float(ml.last_ms) + R_tick_length);
        if (next_ms > ms) {
            SDL_Delay(next_ms - ms);
        }
    }
#endif
}


void R_main(const char *title, int window_width, int window_height,
            R_SceneFn fn, void *arg)
{
    init(title, window_width, window_height);

    if (spin_events() != R_SPIN_EVENTS_QUIT) {
        if (fn) {
            current_scene = fn(arg);
        }
        if (current_scene) {
            main_loop();
        }
    }

#ifndef __EMSCRIPTEN__
    deinit();
#endif
}


R_Scene *R_scene_new(void)
{
    R_Scene *scene = R_NEW_INIT_STRUCT(scene, R_Scene, R_MAGIC_INIT(scene)
            R_animator_new(), {.data = NULL}, NULL, NULL, NULL, NULL);
    R_MAGIC_CHECK(scene);
    return scene;
}

void R_scene_free(R_Scene *scene)
{
    if (scene) {
        R_MAGIC_CHECK(scene);
        if (scene->on_free) {
            scene->on_free(scene);
        }
        R_animator_free(scene->animator);
        R_MAGIC_POISON(scene);
        free(scene);
    }
}

R_Scene *R_scene_current(void)
{
    return current_scene;
}

void *R_scene_next(R_SceneFn fn, void *arg)
{
    void *prev_arg = next_scene_arg;
    next_scene_fn  = fn;
    next_scene_arg = arg;
    return prev_arg;
}


static R_Scene *null_scene_fn(R_UNUSED void *arg)
{
    return NULL;
}

void *R_quit(void)
{
    return R_scene_next(null_scene_fn, NULL);
}
