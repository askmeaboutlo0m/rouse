/*
 * viewport.h - OpenGL viewport mangling
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

/*
 * A viewport, with a position, a width and a height.
 */
typedef struct R_Viewport {
    int x, y, w, h;
} R_Viewport;


/* Set the OpenGL viewport via `glViewport`. */
void R_viewport_set(R_Viewport viewport);

/* Reset the OpenGL view to the window viewport. */
void R_viewport_reset(void);

/* Gives you the application's window viewport. */
R_Viewport R_window_viewport(void);

/*
 * Resize the window viewport according to the application window's size.
 * This will center the viewport so that the aspect ratio of `R_width` and
 * `R_height` are maintained.
 */
void R_window_viewport_resize(void);

/*
 * Set `R_width` and `R_height` to the given parameters and resize the window
 * viewport accordingly.
 */
void R_window_viewport_set(int width, int height);
