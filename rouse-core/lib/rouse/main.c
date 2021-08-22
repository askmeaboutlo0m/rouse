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
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <cglm/struct.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "3rdparty/gles2_inc.h"
#include "3rdparty/parson.h"
#include <rouse_config.h>
#include "common.h"
#include "geom.h"
#include "anim/seq.h"
#include "anim/util.h"
#include "render/gl.h"
#include "render/viewport.h"
#include "stringify.h"
#include "main.h"

#ifdef ROUSE_AL_ENABLED
#   include "audio/sample.h"
#   include "audio/al.h"
#endif

#ifdef __EMSCRIPTEN__
#   include <emscripten.h>
#endif

/* Make sure our environment is sane at compile-time. */
#include <assert.h>
#include "sanity.h"

typedef struct R_QuitHandlerEntry R_QuitHandlerEntry;
struct R_QuitHandlerEntry {
    R_MAGIC_FIELD
    int                id;
    int                priority;
    R_QuitHandler      handler;
    void               *arg;
    R_QuitHandlerEntry *next;
};


SDL_Window    *R_window;
SDL_GLContext R_glcontext;

float R_tick_length  = R_TICK_LENGTH_DEFAULT;
float R_frame_length = R_FRAME_LENGTH_DEFAULT;
float R_max_delta_ms = R_MAX_DELTA_MS_DEFAULT;
bool  R_skip_frames  = true;
float R_width        = R_WIDTH_DEFAULT;
float R_height       = R_HEIGHT_DEFAULT;
bool  R_al_enabled   = false;

static R_Scene   *current_scene  = NULL;
static R_SceneFn next_scene_fn   = NULL;
static void      *next_scene_arg = NULL;

static int                quit_handler_id = 0;
static R_QuitHandlerEntry *quit_handlers  = NULL;


float R_tickrate_get(void)
{
    return 1000.0f / R_tick_length;
}

void R_tickrate_set(float ticks_per_second)
{
    R_tick_length = 1000.0f / ticks_per_second;
}

float R_framerate_get(void)
{
    return 1000.0f / R_frame_length;
}

void R_framerate_set(float frames_per_second)
{
    R_frame_length = 1000.0f / frames_per_second;
}


int R_quit_handler_add(R_QuitHandler handler, void *arg, int priority)
{
    R_QuitHandlerEntry **pp = &quit_handlers;
    while (*pp && (*pp)->priority > priority) {
        pp = &(*pp)->next;
    }

    int                id     = ++quit_handler_id;
    R_QuitHandlerEntry *next  = *pp ? (*pp)->next : NULL;
    R_QuitHandlerEntry *entry = R_NEW_INIT_STRUCT(entry, R_QuitHandlerEntry,
            R_MAGIC_INIT(R_QuitHandlerEntry) id, priority, handler, arg, next);
    *pp = entry;

    return id;
}

bool R_quit_handler_remove(int handler_id, void **out_arg)
{
    R_QuitHandlerEntry **pp = &quit_handlers;
    while ((*pp) && (*pp)->id != handler_id) {
        pp = &(*pp)->next;
    }

    if (*pp) {
        if (out_arg) {
            *out_arg = (*pp)->arg;
        }
        R_QuitHandlerEntry *next = (*pp)->next;
        free(*pp);
        *pp = next;
        return true;
    }
    else {
        return false;
    }
}

static void run_quit_handlers(void)
{
    R_QuitHandlerEntry *entry = quit_handlers;
    while (entry) {
        R_QuitHandlerEntry *next = entry->next;
        entry->handler(entry->arg);
        free(entry);
        entry = next;
    }
}


static R_SdlGlArgs gl_args(void)
{
    return (R_SdlGlArgs) {
        .red_size                   = R_SDL_GL_ARG_UNDEFINED,
        .green_size                 = R_SDL_GL_ARG_UNDEFINED,
        .blue_size                  = R_SDL_GL_ARG_UNDEFINED,
        .alpha_size                 = R_SDL_GL_ARG_UNDEFINED,
        .buffer_size                = R_SDL_GL_ARG_UNDEFINED,
        .doublebuffer               = R_SDL_GL_DOUBLEBUFFER_DEFAULT,
        .depth_size                 = R_SDL_GL_ARG_UNDEFINED,
        .stencil_size               = R_SDL_GL_ARG_UNDEFINED,
        .accum_red_size             = R_SDL_GL_ARG_UNDEFINED,
        .accum_green_size           = R_SDL_GL_ARG_UNDEFINED,
        .accum_blue_size            = R_SDL_GL_ARG_UNDEFINED,
        .accum_alpha_size           = R_SDL_GL_ARG_UNDEFINED,
        .stereo                     = R_SDL_GL_ARG_UNDEFINED,
        .multisamplebuffers         = R_SDL_GL_ARG_UNDEFINED,
        .multisamplesamples         = R_SDL_GL_ARG_UNDEFINED,
        .accelerated_visual         = R_SDL_GL_ARG_UNDEFINED,
        .context_major_version      = R_SDL_GL_CONTEXT_MAJOR_VERSION_DEFAULT,
        .context_minor_version      = R_SDL_GL_CONTEXT_MINOR_VERSION_DEFAULT,
        .context_flags              = R_SDL_GL_ARG_UNDEFINED,
        .context_profile_mask       = R_SDL_GL_CONTEXT_PROFILE_MASK_DEFAULT,
        .share_with_current_context = R_SDL_GL_ARG_UNDEFINED,
        .framebuffer_srgb_capable   = R_SDL_GL_ARG_UNDEFINED,
        .context_release_behavior   = R_SDL_GL_ARG_UNDEFINED,
    };
}

static R_AlArgs al_args(void)
{
    return (R_AlArgs) {false, NULL, NULL};
}

static R_WindowArgs window_args(void)
{
    return (R_WindowArgs){R_WINDOW_TITLE_DEFAULT, R_WINDOW_X_DEFAULT,
                          R_WINDOW_Y_DEFAULT, R_WINDOW_WIDTH_DEFAULT,
                          R_WINDOW_HEIGHT_DEFAULT, R_WINDOW_FLAGS_DEFAULT};
}

R_MainArgs R_main_args(R_SceneFn on_scene, void *user)
{
    R_MainArgs args = {R_MAGIC_INIT(R_MainArgs) R_SDL_INIT_FLAGS_DEFAULT,
                       R_IMG_INIT_FLAGS_DEFAULT, gl_args(), al_args(),
                       window_args(), NULL, NULL, NULL, NULL, NULL, NULL,
                       on_scene, user};
    R_MAGIC_CHECK(R_MainArgs, &args);
    return args;
}


static bool initialized = false;

/* Reference to `ease.c`, not in the header because it's only needed here. */
void R_ease_init(void);

static void init_sdl(uint32_t flags, R_InitSdlHook on_sdl_init, void *user)
{
    if (on_sdl_init) {
        on_sdl_init(user, &flags);
    }
    R_debug("initializing SDL with flags 0x%x", (unsigned int) flags);
    if (SDL_Init(flags)) {
        R_die("SDL_Init: %s", SDL_GetError());
    }
}

static void init_img(int flags, R_InitImgHook on_img_init, void *user)
{
    if (on_img_init) {
        on_img_init(user, &flags);
    }
    R_debug("initializing SDL_image with flags 0x%x", (unsigned int) flags);
    if (!(IMG_Init(flags) & flags)) {
        R_die("IMG_Init: %s", IMG_GetError());
    }
}

static void set_sdl_gl_attribute(R_UNUSED_UNLESS_DEBUG const char *attr_name,
                                 SDL_GLattr attr, int value)
{
    if (value != R_SDL_GL_ARG_UNDEFINED) {
        R_debug("%s = %d", attr_name, value);
        SDL_GL_SetAttribute(attr, value);
    }
    else {
        R_debug("%s left on its default", attr_name);
    }
}

static void init_sdl_hints(const R_SdlGlArgs *gl_args)
{
    /* By default, SDL will use the native OpenGL ES 2.0 support in Windows if
     * the driver supports it instead of using DirectX via ANGLE. This seems to
     * be the case for Intel and NVidia drivers, basically. However, the actual
     * implementation isn't great and e.g. refuses to create frame buffers with
     * a stencil attachment. This hint causes SDL to prefer ANGLE, which is the
     * more compatible option and therefore the default. If you want to change
     * it, use the `on_window_init` hook and set this hint to "0" again. */
#ifdef _WIN32
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
#endif

#   define SET_GL_ATTR(FIELD, ATTRIBUTE) \
        set_sdl_gl_attribute(#ATTRIBUTE, ATTRIBUTE, gl_args->FIELD)
    SET_GL_ATTR(red_size,                   SDL_GL_RED_SIZE);
    SET_GL_ATTR(green_size,                 SDL_GL_GREEN_SIZE);
    SET_GL_ATTR(blue_size,                  SDL_GL_BLUE_SIZE);
    SET_GL_ATTR(alpha_size,                 SDL_GL_ALPHA_SIZE);
    SET_GL_ATTR(buffer_size,                SDL_GL_BUFFER_SIZE);
    SET_GL_ATTR(doublebuffer,               SDL_GL_DOUBLEBUFFER);
    SET_GL_ATTR(depth_size,                 SDL_GL_DEPTH_SIZE);
    SET_GL_ATTR(stencil_size,               SDL_GL_STENCIL_SIZE);
    SET_GL_ATTR(accum_red_size,             SDL_GL_ACCUM_RED_SIZE);
    SET_GL_ATTR(accum_green_size,           SDL_GL_ACCUM_GREEN_SIZE);
    SET_GL_ATTR(accum_blue_size,            SDL_GL_ACCUM_BLUE_SIZE);
    SET_GL_ATTR(accum_alpha_size,           SDL_GL_ACCUM_ALPHA_SIZE);
    SET_GL_ATTR(stereo,                     SDL_GL_STEREO);
    SET_GL_ATTR(multisamplebuffers,         SDL_GL_MULTISAMPLEBUFFERS);
    SET_GL_ATTR(multisamplesamples,         SDL_GL_MULTISAMPLESAMPLES);
    SET_GL_ATTR(accelerated_visual,         SDL_GL_ACCELERATED_VISUAL);
    SET_GL_ATTR(context_major_version,      SDL_GL_CONTEXT_MAJOR_VERSION);
    SET_GL_ATTR(context_minor_version,      SDL_GL_CONTEXT_MINOR_VERSION);
    SET_GL_ATTR(context_flags,              SDL_GL_CONTEXT_FLAGS);
    SET_GL_ATTR(context_profile_mask,       SDL_GL_CONTEXT_PROFILE_MASK);
    SET_GL_ATTR(share_with_current_context, SDL_GL_SHARE_WITH_CURRENT_CONTEXT);
    SET_GL_ATTR(framebuffer_srgb_capable,   SDL_GL_FRAMEBUFFER_SRGB_CAPABLE);
    SET_GL_ATTR(context_release_behavior,   SDL_GL_CONTEXT_RELEASE_BEHAVIOR);
#   undef SET_GL_ATTR
}

static void init_window(const char *title, int x, int y, int width, int height,
                        uint32_t flags, R_InitWindowHook on_window_init,
                        void *user)
{
    if (on_window_init) {
        on_window_init(user, &title, &x, &y, &width, &height, &flags);
    }
    if (!title) {
        title = R_WINDOW_TITLE_DEFAULT;
    }
    R_debug("creating window titled '%s' at %d, %d with dimensions %dx%d and "
            " flags 0x%x", title, x, y, width, height, (unsigned int) flags);
    R_window = SDL_CreateWindow(title, x, y, width, height, flags);
    if (!R_window) {
        R_die("SDL_CreateWindow: %s", SDL_GetError());
    }
}

static void init_gl(R_InitHook on_gl_init, void *user)
{
    if (on_gl_init) {
        on_gl_init(user);
    }
    R_debug("creating GL context");
    R_glcontext = SDL_GL_CreateContext(R_window);
    if (!R_glcontext) {
        R_die("SDL_GL_CreateContext: %s", SDL_GetError());
    }
    R_gl_init();
}

static void init_al(const R_AlArgs *al, R_InitHook on_al_init, void *user)
{
    if (on_al_init) {
        on_al_init(user);
    }
#ifdef ROUSE_AL_ENABLED
    if (al->enabled) {
        R_debug("initializing OpenAL audio");
        R_al_enabled = R_al_init(
            al->get_device_name, al->get_context_attributes, user);
    }
    else {
        R_debug("not initializing OpenAL audio (as requested)");
    }
#else
    if (al->enabled) {
        R_warn("OpenAL audio initialization requested, but "
               "OpenAL support is not compiled in");
    }
    else {
        R_debug("not initializing OpenAL audio (as requested, "
                "it's not compiled in anyway)");
    }
#endif
}

static void post_init(R_InitHook on_post_init, void *user)
{
    R_window_viewport_resize();
    if (on_post_init) {
        on_post_init(user);
    }
}

static void init(const R_MainArgs *args)
{
    R_MAGIC_CHECK(R_MainArgs, args);
    if (initialized) {
        R_die("Attempt to call R_main twice");
    }

    R_debug("initializing rouse");
    initialized = true;

    unsigned int seed = (unsigned int) time(NULL);
    srand(seed);
    R_debug("srand seed = %u", seed);

    json_set_allocation_functions(R_malloc, free);
    R_ease_init();

#ifdef ROUSE_MAGIC
    R_magic_seed = R_int2uint32(rand());
    R_debug("magic number seed = %u", (unsigned int) R_magic_seed);
#endif

    void *user = args->user;
    init_sdl(args->sdl_init_flags, args->on_sdl_init, user);
    init_img(args->img_init_flags, args->on_img_init, user);
    init_sdl_hints(&args->gl);
    const R_WindowArgs *window = &args->window;
    init_window(window->title, window->x, window->y, window->width,
                window->height, window->flags, args->on_window_init, user);
    init_gl(args->on_gl_init, user);
    init_al(&args->al, args->on_al_init, args->user);
    post_init(args->on_post_init, user);
}

static void deinit(void)
{
    R_scene_free(current_scene);
    run_quit_handlers();
#ifdef ROUSE_AL_ENABLED
    R_al_deinit();
#endif
    SDL_GL_DeleteContext(R_glcontext);
    SDL_DestroyWindow(R_window);
    IMG_Quit();
#ifndef ROUSE_ASAN
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


static void tick_scene(R_Scene *scene, float seconds, bool rendered)
{
    R_animator_tick(scene->animator, rendered, seconds);
    if (scene->on_tick) {
        scene->on_tick(scene, rendered);
    }
}

static R_Scene *swap_scene(float seconds)
{
    /*
     * While it's a strange thing to do, a scene *could* potentially set a new
     * scene during its own initialization. Some kind of trampoline I guess. So
     * we'll keep looping until there's no more next scene to set.
     */
    while (next_scene_fn) {
        /*
         * Of course an even stranger thing can occur: the free callback can
         * change the next scene function. So we have to do that first.
         */
        R_scene_free(current_scene);

        /* Clear the global variables now so that they can be set again. */
        R_SceneFn fn   = next_scene_fn;
        void      *arg = next_scene_arg;
        next_scene_fn  = NULL;
        next_scene_arg = NULL;

        /*
         * The free callback could have set the next scene function to NULL.
         * That's weird, but we'll just treat it as if `R_quit()` was called.
         */
        current_scene = fn ? fn(arg) : NULL;

        if (current_scene) {
            R_MAGIC_CHECK(R_Scene, current_scene);
            /*
             * The `seconds` parameter only has a non-zero value if we're about
             * to render the scene. In that case, force a tick on it so that it
             * gets a chance to run its logic at least once. Otherwise we might
             * catch it in a weird state for one frame.
             */
            if (seconds != 0.0f) {
                tick_scene(current_scene, seconds, true);
            }
        }
    }
    return current_scene;
}


static void clamp_max_delta_ms(float *next_tick, float *next_frame,
                               float delta_ms, float max_delta_ms)
{
    if (delta_ms > max_delta_ms) {
        float correction = delta_ms - max_delta_ms;
        *next_tick      += correction;
        *next_frame     += correction;
    }
}

static void skip_frames(float *next_frame, bool should_skip, float ms_f,
                        float frame_length)
{
    if (should_skip) {
        float frames_in_interval = (ms_f - *next_frame) / frame_length;
        if (frames_in_interval >= 2.0f) {
            float skipped = floorf(frames_in_interval);
            *next_frame  += skipped * frame_length;
        }
    }
}


#define STEP_NONE  0x0
#define STEP_TICK  0x1
#define STEP_FRAME 0x2
#define STEP_BOTH  (STEP_TICK | STEP_FRAME)

static int check_step(float next_tick, float next_frame, float ms_f)
{
    if (next_frame < next_tick) {
        return ms_f >= next_frame ? STEP_FRAME : STEP_NONE;
    }
    else if (ms_f >= next_tick) {
        return next_frame == next_tick ? STEP_BOTH : STEP_TICK;
    }
    else {
        return STEP_NONE;
    }
}

static float step_tick(float next_tick, float tick_length, float seconds,
                       float next_frame)
{
    float   subsequent_tick = next_tick + tick_length;
    R_Scene *scene          = swap_scene(0.0f);
    if (scene) {
        tick_scene(scene, seconds, subsequent_tick > next_frame);
    }
    return subsequent_tick;
}

static bool step_ticks(float *last_ms, float *next_tick, float next_frame,
                       float ms_f, float tick_length, float seconds)
{
    int step;
    while ((step = check_step(*next_tick, next_frame, ms_f)) & STEP_TICK) {
        *last_ms = *next_tick = step_tick(*next_tick, tick_length,
                                          seconds, next_frame);
    }
    return step & STEP_FRAME;
}


static void render_frame(float *last_ms, float *next_frame,
                         float frame_length, float seconds)
{
    R_Scene *scene = swap_scene(seconds);
    if (scene && scene->on_render) {
        R_GL_CLEAR_ERROR();
        scene->on_render(scene);
        SDL_GL_SwapWindow(R_window);
    }
    *last_ms = *next_frame += frame_length;
}


struct MainLoop {
    float last_ms;
    float next_tick;
    float next_frame;
    bool  running;
};

static bool step_main_loop(struct MainLoop *ml)
{
    if (spin_events() == R_SPIN_EVENTS_QUIT) {
        ml->running = false;
    }
    else {
        float    tick_length  = R_tick_length;
        float    frame_length = R_frame_length;
        float    seconds      = tick_length / 1000.0f;
        uint32_t ms           = SDL_GetTicks();
        float    ms_f         = R_uint322float(ms);
        float    delta_ms     = ms_f - ml->last_ms;

        /*
         * If more than `max_delta_ms` has passed since the last step, skip time
         * forward to reduce the delta to that amount. This avoids craziness in
         * case the application is suspended for a long time or something.
         */
        clamp_max_delta_ms(&ml->next_tick, &ml->next_frame,
                           delta_ms, R_max_delta_ms);

        /*
         * It's usually pointless to render anything more than the latest frame.
         * If more than one frame would be rendered in this step, skip all but
         * the last one. The exception is when recording: for that you want
         * every frame to actually get rendered so that the video plays with a
         * perfect frame rate. For that, set `R_skip_frames` to `false`.
         */
        skip_frames(&ml->next_frame, R_skip_frames, ms_f, frame_length);

        /*
         * Tick the logic forward until the next frame to be rendered or until
         * we're out of time for this step, whichever comes first.
         */
        bool want_frame = step_ticks(&ml->last_ms, &ml->next_tick,
                                     ml->next_frame, ms_f, tick_length,
                                     seconds);

        /*
         * Render a frame if need be. If the framerate is different from the
         * tickrate, then there may be cases where only logic is run.
         */
        if (want_frame) {
            render_frame(&ml->last_ms, &ml->next_frame, frame_length, seconds);
        }
    }

    return ml->running && swap_scene(0.0f);
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
        uint32_t ms   = SDL_GetTicks();
        ml.next_frame = ml.next_tick = ml.last_ms = R_uint322float(ms);
        ml.running    = true;
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
        uint32_t next_ms = R_float2uint32(R_MIN(ml.next_tick, ml.next_frame));
        if (next_ms > ms) {
            SDL_Delay(next_ms - ms);
        }
    }
#endif
}


void R_main(const R_MainArgs *args)
{
    init(args);

    if (spin_events() != R_SPIN_EVENTS_QUIT) {
        R_SceneFn on_scene = args->on_scene;
        if (on_scene) {
            current_scene = on_scene(args->user);
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
    R_Scene *scene = R_NEW_INIT_STRUCT(scene, R_Scene, R_MAGIC_INIT(R_Scene)
            R_animator_new(), {.data = NULL}, NULL, NULL, NULL, NULL);
    R_MAGIC_CHECK(R_Scene, scene);
    return scene;
}

void R_scene_free(R_Scene *scene)
{
    if (scene) {
        R_MAGIC_CHECK(R_Scene, scene);
        if (scene->on_free) {
            scene->on_free(scene);
        }
        R_animator_free(scene->animator);
        R_MAGIC_POISON(R_Scene, scene);
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
