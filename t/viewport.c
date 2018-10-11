#include "tap.h"
#include <rouse/rouse.h>


static int mock_width  = 0;
static int mock_height = 0;

void SDL_GL_GetDrawableSize(R_UNUSED SDL_Window *window, int *w, int *h)
{
    *w = mock_width;
    *h = mock_height;
}


static R_Viewport mock_gl_viewport = {0, 0, 0, 0};

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    mock_gl_viewport = (R_Viewport){x, y, width, height};
}


static void viewport_ok(R_Viewport viewport, int x, int y, int w, int h)
{
    ok(viewport.x == x, "viewport x %d == %d", viewport.x, x);
    ok(viewport.y == y, "viewport y %d == %d", viewport.y, y);
    ok(viewport.w == w, "viewport w %d == %d", viewport.w, w);
    ok(viewport.h == h, "viewport h %d == %d", viewport.h, h);
}

static void window_viewport_ok(int x, int y, int w, int h)
{
    viewport_ok(R_window_viewport(), x, y, w, h);
}


static void resize(float rw, float rh, int ww, int wh)
{
    R_width     = rw;
    R_height    = rh;
    mock_width  = ww;
    mock_height = wh;
    R_window_viewport_resize();
}

static void test_window_viewport_resize(void)
{
    note("window viewport starts out zeroed");
    window_viewport_ok(0, 0, 0, 0);

    note("window size same as real size");
    resize(1920.0f, 1080.0f, 1920, 1080);
    window_viewport_ok(0, 0, 1920, 1080);

    note("window size smaller, but with same aspect ratio");
    resize(1920.0f, 1080.0f, 1280, 720);
    window_viewport_ok(0, 0, 1280, 720);

    note("window size larger, but with same aspect ratio");
    resize(1280.0f, 720.0f, 1920, 1080);
    window_viewport_ok(0, 0, 1920, 1080);

    note("window too thin, bars on top and bottom");
    resize(1920.0f, 1080.0f, 1280, 1024);
    window_viewport_ok(0, 152, 1280, 720);

    note("window too wide, bars on left and right");
    resize(320.0f, 240.0f, 1920, 1080);
    window_viewport_ok(240, 0, 1440, 1080);

    note("window size of zero works sanely");
    resize(500.0f, 500.0f, 0, 0);
    window_viewport_ok(0, 0, 0, 0);
}


static void test_viewport_set(void)
{
    resize(1920.0f, 1080.0f, 1920, 1080);
    window_viewport_ok(0, 0, 1920, 1080);
    R_viewport_reset();
    viewport_ok(mock_gl_viewport, 0, 0, 1920, 1080);

    R_viewport_set((R_Viewport){1, 2, 3, 4});
    viewport_ok(mock_gl_viewport, 1, 2, 3, 4);
}


TAP_BEGIN
    test_window_viewport_resize();
    test_viewport_set();
TAP_END
