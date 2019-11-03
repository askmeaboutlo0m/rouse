#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <assert.h>
#include <cglm/struct.h>
#include <GL/glew.h>
#include "../3rdparty/nanovg_inc.h"
#include "../common.h"
#include "../geom.h"
#include "../parse.h"
#include "../render/gl.h"
#include "../render/frame_buffer.h"
#include "vector.h"
#include "sprite.h"
#include "canvas.h"

struct R_Canvas {
    R_MAGIC_FIELD
    R_FrameBuffer *fb;
    R_Sprite      *sprite;
};


static R_FrameBuffer *make_fb(int width, int height)
{
    R_FrameBufferOptions options = R_frame_buffer_options();
    options.width        = width;
    options.height       = height;
    options.color_type   = R_FRAME_BUFFER_ATTACHMENT_TEXTURE;
    options.stencil_type = R_FRAME_BUFFER_ATTACHMENT_BUFFER;
    return R_frame_buffer_new(&options);
}

R_Canvas *R_canvas_new(int width, int height)
{
    R_Canvas *canvas = R_NEW_INIT_STRUCT(canvas, R_Canvas,
            R_MAGIC_INIT(canvas) make_fb(width, height), R_sprite_new_root());
    R_MAGIC_CHECK(canvas);
    return canvas;
}


void R_canvas_free(R_Canvas *canvas)
{
    if (canvas) {
        R_MAGIC_CHECK_NN(canvas);
        R_sprite_decref(canvas->sprite);
        R_frame_buffer_free(canvas->fb);
        R_MAGIC_POISON(canvas);
        free(canvas);
    }
}


R_Sprite *R_canvas_sprite(R_Canvas *canvas)
{
    R_MAGIC_CHECK(canvas);
    return canvas->sprite;
}


R_FrameBuffer *R_canvas_render(R_Canvas *canvas, NVGcontext *vg)
{
    R_MAGIC_CHECK(canvas);
    assert(vg && "NVGcontext can't be null");
    R_FrameBuffer *fb = canvas->fb;

    R_frame_buffer_bind(fb);
    R_gl_clear(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0);

    nvgBeginFrame(vg, R_int2float(fb->width), R_int2float(fb->height), 1.0f);
    R_sprite_draw(canvas->sprite, vg,
                  (float[6]){1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f});
    nvgEndFrame(vg);

    return fb;
}