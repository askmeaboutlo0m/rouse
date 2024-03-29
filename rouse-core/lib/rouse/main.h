/*
 * main.h - main entrypoint, starting and quitting
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

#define R_SDL_INIT_FLAGS_DEFAULT (SDL_INIT_VIDEO | SDL_INIT_TIMER)
#define R_IMG_INIT_FLAGS_DEFAULT IMG_INIT_PNG

#define R_SDL_GL_CONTEXT_PROFILE_MASK_DEFAULT  SDL_GL_CONTEXT_PROFILE_ES
#define R_SDL_GL_CONTEXT_MAJOR_VERSION_DEFAULT 2
#define R_SDL_GL_CONTEXT_MINOR_VERSION_DEFAULT 0
#define R_SDL_GL_DOUBLEBUFFER_DEFAULT          1
#define R_SDL_GL_ARG_UNDEFINED                 (-9999)

#define R_WINDOW_TITLE_DEFAULT   "rouse"
#define R_WINDOW_X_DEFAULT       SDL_WINDOWPOS_UNDEFINED
#define R_WINDOW_Y_DEFAULT       SDL_WINDOWPOS_UNDEFINED
#define R_WINDOW_WIDTH_DEFAULT   1280
#define R_WINDOW_HEIGHT_DEFAULT  720
#define R_WINDOW_FLAGS_DEFAULT   (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)

#define R_TICK_LENGTH_DEFAULT  (1000.0f / 100.0f)
#define R_FRAME_LENGTH_DEFAULT (1000.0f /  60.0f)
#define R_MAX_DELTA_MS_DEFAULT 1000.0f

/* Default resolution is full HD. */
#define R_WIDTH_DEFAULT  1920
#define R_HEIGHT_DEFAULT 1080

/* Generic hook called during initialization. */
typedef void (*R_InitHook)(void *user);
/* Hook to modify flags passed to SDL_Init. */
typedef void (*R_InitSdlHook)(void *user, uint32_t *flags);
/* Hook to modify flags passed to IMG_Init. */
typedef void (*R_InitImgHook)(void *user, int *flags);
/* Hook to modify window arguments passed to SDL_CreateWindow. */
typedef void (*R_InitWindowHook)(void *user, const char **title, int *x, int *y,
                                 int *width, int *height, uint32_t *flags);

/* Callback that initializes a scene. */
typedef struct R_Scene R_Scene;
typedef R_Scene *(*R_SceneFn)(void *);

typedef struct R_SdlGlArgs {
    int red_size;                   /* SDL_GL_RED_SIZE */
    int green_size;                 /* SDL_GL_GREEN_SIZE */
    int blue_size;                  /* SDL_GL_BLUE_SIZE */
    int alpha_size;                 /* SDL_GL_ALPHA_SIZE */
    int buffer_size;                /* SDL_GL_BUFFER_SIZE */
    int doublebuffer;               /* SDL_GL_DOUBLEBUFFER */
    int depth_size;                 /* SDL_GL_DEPTH_SIZE */
    int stencil_size;               /* SDL_GL_STENCIL_SIZE */
    int accum_red_size;             /* SDL_GL_ACCUM_RED_SIZE */
    int accum_green_size;           /* SDL_GL_ACCUM_GREEN_SIZE */
    int accum_blue_size;            /* SDL_GL_ACCUM_BLUE_SIZE */
    int accum_alpha_size;           /* SDL_GL_ACCUM_ALPHA_SIZE */
    int stereo;                     /* SDL_GL_STEREO */
    int multisamplebuffers;         /* SDL_GL_MULTISAMPLEBUFFERS */
    int multisamplesamples;         /* SDL_GL_MULTISAMPLESAMPLES */
    int accelerated_visual;         /* SDL_GL_ACCELERATED_VISUAL */
    int context_major_version;      /* SDL_GL_CONTEXT_MAJOR_VERSION */
    int context_minor_version;      /* SDL_GL_CONTEXT_MINOR_VERSION */
    int context_flags;              /* SDL_GL_CONTEXT_FLAGS */
    int context_profile_mask;       /* SDL_GL_CONTEXT_PROFILE_MASK */
    int share_with_current_context; /* SDL_GL_SHARE_WITH_CURRENT_CONTEXT */
    int framebuffer_srgb_capable;   /* SDL_GL_FRAMEBUFFER_SRGB_CAPABLE */
    int context_release_behavior;   /* SDL_GL_CONTEXT_RELEASE_BEHAVIOR */
} R_SdlGlArgs;

typedef struct R_WindowArgs {
    const char *title;
    int        x, y, width, height;
    uint32_t   flags;
} R_WindowArgs;

typedef const char *(*R_AlGetDeviceNameHook       )(void *);
typedef const int  *(*R_AlGetContextAttributesHook)(void *);

typedef struct R_AlArgs {
    bool                         enabled;
    R_AlGetDeviceNameHook        get_device_name;
    R_AlGetContextAttributesHook get_context_attributes;
} R_AlArgs;

typedef struct R_MainArgs {
    R_MAGIC_FIELD
    uint32_t         sdl_init_flags;
    int              img_init_flags;
    R_SdlGlArgs      gl;
    R_AlArgs         al;
    R_WindowArgs     window;
    R_InitSdlHook    on_sdl_init;
    R_InitImgHook    on_img_init;
    R_InitWindowHook on_window_init;
    R_InitHook       on_gl_init;
    R_InitHook       on_al_init;
    R_InitHook       on_post_init;
    R_SceneFn        on_scene;
    void             *user;
} R_MainArgs;

typedef void (*R_QuitHandler)(void *);

/*
 * A scene, the base unit of measurement. When you set a scene, it'll be
 * responsible for handling events, doing logic on every tick and rendering
 * stuff to the screen until you set a different scene. Setting a `NULL`
 * scene means the application is done and will quit. You can basically think
 * of a scene like a level or some other kind of game state, I guess.
 *
 * You set the scene by calling `R_scene_next` with a callback and and an
 * optional user data pointer. In that callback, you should create a new scene
 * (probably via `R_scene_new`), set up whatever user data you need an assign
 * your callback functions (or methods, if you will). From that point on, your
 * scene will go running until you call `R_scene_next` again with a different
 * callback - or your application exits by some other means, of course.
 */
struct R_Scene {
    R_MAGIC_FIELD
    /* Animation sequencing bookkeepery. Don't touch. */
    void *animator;
    /* This is where your scene-specific data goes. */
    R_UserData user;
    /* Event handling callback. Just uses SDL's event loop. */
    void (*on_event)(R_Scene *, SDL_Event *);
    /*
     * Callback for each logic tick. The boolean parameter tells you if this
     * tick will be rendered, so you can avoid doing logic that only has an
     * effect on the visuals based on that. Or you could do it in your render
     * callback instead, if you wanna put logic there too.
     */
    void (*on_tick)(R_Scene *, bool);
    /* Render callback. Do your OpenGL renderage here. */
    void (*on_render)(R_Scene *);
    /*
     * Callback for when the scene is over. Use this to free the scene object,
     * your user data, clean up your loaded resources and whatever else. If
     * your application `R_die`s or otherwise crashes, this won't get called,
     * but on scene changes, including changing to a `NULL` scene, it will.
     */
    void (*on_free)(R_Scene *);
};

/*
 * The One Window. Multiple windows aren't supported because how would that
 * work in a browser or on mobile. But you can create them manually via SDL
 * if you really, really need to.
 */
extern SDL_Window *R_window;
/*
 * The OpenGL context. Again, only one because there's no threads in the
 * browser, so I don't think multiple are ever needed. But again, you can
 * manually create new ones via SDL if you feel the urge.
 */
extern SDL_GLContext R_glcontext;

extern float R_tick_length;

extern float R_frame_length;

extern float R_max_delta_ms;

extern bool R_skip_frames;

/*
 * The "real" resolution of your application. This is used to set the window
 * viewport sensibly. The default is 1920 by 1080.
 */
extern float R_width;
extern float R_height;
/*
 * If you can play audio or not. OpenAL initialization may fail after requesting
 * it on startup, so check this to figure out if you can *actually* play audio.
 */
extern bool R_al_enabled;

float R_tickrate_get(void);
void  R_tickrate_set(float ticks_per_second);

float R_framerate_get(void);
void  R_framerate_set(float frames_per_second);


int  R_quit_handler_add(R_QuitHandler handler, void *arg, int priority);
bool R_quit_handler_remove(int handler_id, void **out_arg);


R_MainArgs R_main_args(R_SceneFn on_scene, void *user);

/*
 * Get this thing going. This is the entry point to start your application.
 * Use `R_main_args` to get the default set of arguments and populate them.
 * You shouldn't do anything after you call this! You can't rely on this
 * function ever returning, cause depending on the platform it won't.
 */
void R_main(const R_MainArgs *args);


/*
 * Allocating and freeing a scene. The `R_Animator` will be set up and torn
 * down together with the scene. Everything else is yours to handle yourself.
 */
R_Scene *R_scene_new(void);
void R_scene_free(R_Scene *scene);

/* Gets you the currently active scene, in case you lost track of it. */
R_Scene *R_scene_current(void);

/*
 * Set the next scene. The given `void *` argument is passed along to your
 * given callback function. If you call this multiple times within the same
 * scene tick, the latest arguments prevail. This returns the `void *` argument
 * from the *previous* unfinished call, which should give you a chance to free
 * it if necessary. Although, just making sure you only call this once is
 * probably a more sensible option.
 */
void *R_scene_next(R_SceneFn fn, void *arg);

/*
 * Stop the application sometime after the current frame is over. This is
 * actually the same as calling `R_scene_next(function-returning-NULL, NULL)`,
 * just in a less painful shape that doesn't look like voodoo.
 */
void *R_quit(void);
