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

/* Default tick length and frame skip, for 100 ticks per second. */
#define R_TICK_LENGTH_DEFAULT             10
#define R_MAX_TICKS_BEFORE_RENDER_DEFAULT 10

/* Default resolution is full HD. */
#define R_WIDTH_DEFAULT  1920
#define R_HEIGHT_DEFAULT 1080

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
/*
 * How long a tick (a logical frame) is in milliseconds. You can get the tick
 * rate per second via `float ticks_per_second = 1000.0f / R_tick_length`. A
 * smaller number gets you more fine-grained animation timing, at the expense
 * of eating more CPU.
 */
extern float R_tick_length;
/*
 * Maximum number of frames to skip rendering. The logical ticks will still be
 * run normally, but drawing them is skipped to catch up. A 1 here means every
 * frame is rendered, which is probably what you want for video recording, but
 * it's unsuitable for real-time usage since it'll probably cause annoying lag.
 */
extern Uint32 R_max_ticks_before_render;
/*
 * The "real" resolution of your application. This is used to set the window
 * viewport sensibly. The default is 1920 by 1080.
 */
extern float R_width;
extern float R_height;

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
typedef struct R_Scene R_Scene;
struct R_Scene {
    R_MAGIC_FIELD
    /* Animation sequencing bookkeepery. Don't touch. */
    R_Animator *animator;
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

/* Callback that initializes a scene. */
typedef R_Scene *(*R_SceneFn)(void *);

/*
 * Sets `R_tick_length` with the human-friendly unit of ticks per second.
 */
void R_framerate_set(float ticks_per_second);

/*
 * Get this thing going. This is the entry point to start your application.
 * The title will be used as the application name, e.g. in the window title bar
 * or the browser tab or whatever. The window width and height will be used for
 * the window being created, but not every platform will have a window at all.
 * Give it a function to set the first scene and an optional argument for that
 * function. You shouldn't do anything after you call this! You can't rely on
 * this function ever returning, cause depending on the platform it won't.
 */
void R_main(const char *title, int window_width, int window_height,
            R_SceneFn fn, void *arg);


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
