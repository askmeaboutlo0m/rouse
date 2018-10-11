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
