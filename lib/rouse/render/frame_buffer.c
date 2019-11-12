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
#include <assert.h>
#include <cglm/struct.h>
#include <GL/glew.h>
#include "../common.h"
#include "../geom.h"
#include "gl.h"
#include "viewport.h"
#include "frame_buffer.h"
#include "../stringify.h"


static int get_frame_buffer_binding(void)
{
    int handle;
    R_GL(glGetIntegerv, GL_FRAMEBUFFER_BINDING, &handle);
    return handle;
}


static void attach_buffer(unsigned int *buf, unsigned int component,
                          unsigned int attachment, int width, int height)
{
    R_GL(glGenRenderbuffers, 1, buf);
    R_GL(glBindRenderbuffer, GL_RENDERBUFFER, *buf);
    R_GL(glRenderbufferStorage, GL_RENDERBUFFER, component, width, height);
    R_GL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, attachment,
                                    GL_RENDERBUFFER, *buf);
}

static void attach_texture(unsigned int *tex, unsigned int attachment,
                           int internal_format, unsigned int format,
                           int width, int height)
{
    R_GL(glGenTextures, 1, tex);
    R_GL(glBindTexture, GL_TEXTURE_2D, *tex);
    R_GL(glTexImage2D, GL_TEXTURE_2D, 0, internal_format, width, height,
                       0, format, GL_UNSIGNED_BYTE, NULL);
    R_GL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    R_GL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    R_GL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    R_GL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    R_GL(glFramebufferTexture2D, GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, *tex, 0);
}

static void attach_color_buffer(R_FrameBuffer *fb,
                                R_FrameBufferAttachmentType type)
{
    switch (type) {
        case R_FRAME_BUFFER_ATTACHMENT_NONE:
            break;
        case R_FRAME_BUFFER_ATTACHMENT_BUFFER:
            attach_buffer(&fb->color, GL_RGBA8, GL_COLOR_ATTACHMENT0,
                          fb->real_width, fb->real_height);
            break;
        case R_FRAME_BUFFER_ATTACHMENT_TEXTURE:
            attach_texture(&fb->color, GL_COLOR_ATTACHMENT0, GL_RGBA, GL_RGBA,
                           fb->real_width, fb->real_height);
            break;
        default:
            R_die("Unknown frame buffer color type: %d", type);
    }
}

static void attach_depth_buffer(R_FrameBuffer *fb,
                                R_FrameBufferAttachmentType type)
{
    switch (type) {
        case R_FRAME_BUFFER_ATTACHMENT_NONE:
            break;
        case R_FRAME_BUFFER_ATTACHMENT_BUFFER:
            attach_buffer(&fb->depth, GL_DEPTH_COMPONENT16, GL_DEPTH_ATTACHMENT,
                          fb->real_width, fb->real_height);
            break;
        case R_FRAME_BUFFER_ATTACHMENT_TEXTURE:
            attach_texture(&fb->depth, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT16,
                           GL_DEPTH_COMPONENT, fb->real_width, fb->real_height);
            break;
        default:
            R_die("Unknown frame buffer depth type: %d", type);
    }
}

static void attach_stencil_buffer(R_FrameBuffer *fb,
                                  R_FrameBufferAttachmentType type)
{
    switch (type) {
        case R_FRAME_BUFFER_ATTACHMENT_NONE:
            break;
        case R_FRAME_BUFFER_ATTACHMENT_BUFFER:
            attach_buffer(&fb->stencil, GL_STENCIL_INDEX8, GL_STENCIL_ATTACHMENT,
                          fb->real_width, fb->real_height);
            break;
        case R_FRAME_BUFFER_ATTACHMENT_TEXTURE:
            R_die("Frame buffer stencil textures unsupported in OpenGL ES 2.0");
        default:
            R_die("Unknown frame buffer stencil type: %d", type);
    }
}


static void check_status(void)
{
    unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    R_GL_CHECK_ERROR("glCheckFramebufferStatus");

    const char *error = NULL;
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE:
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            error = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            error = "GL_FRAMEBUFFER_UNSUPPORTED";
            break;
        default:
            error = "unknown error";
            break;
    }

    if (error) {
        R_die("Constructing frame buffer failed: %s (%u)", error, status);
    }
}


R_FrameBufferOptions R_frame_buffer_options(void)
{
    return (R_FrameBufferOptions){
        R_MAGIC_INIT_TYPE(R_FrameBufferOptions)
        .width        = 0,
        .height       = 0,
        .color_type   = R_FRAME_BUFFER_ATTACHMENT_NONE,
        .depth_type   = R_FRAME_BUFFER_ATTACHMENT_NONE,
        .stencil_type = R_FRAME_BUFFER_ATTACHMENT_NONE,
    };
}

static int next_power_of_two(int i)
{
    return R_float2int(R_power_of_two(R_int2float(i)));
}

R_FrameBuffer *R_frame_buffer_new(R_FrameBufferOptions *options)
{
    R_MAGIC_CHECK(options);
    R_assert(options->width  > 0, "frame buffer dimensions must be positive");
    R_assert(options->height > 0, "frame buffer dimensions must be positive");

    R_GL_CLEAR_ERROR();
    int previous_handle = get_frame_buffer_binding();
    int width           = options->width;
    int height          = options->height;

    R_FrameBuffer *fb = R_NEW_INIT_STRUCT(fb, R_FrameBuffer,
            width, height, next_power_of_two(width),
            next_power_of_two(height), 0, 0, 0, 0, options->color_type,
            options->depth_type, options->stencil_type, NULL);

    R_GL(glGenFramebuffers, 1, &fb->handle);
    R_GL(glBindFramebuffer, GL_FRAMEBUFFER, fb->handle);
    attach_color_buffer(  fb, options->color_type);
    attach_depth_buffer(  fb, options->depth_type);
    attach_stencil_buffer(fb, options->stencil_type);
    check_status();

    R_GL(glBindFramebuffer, GL_FRAMEBUFFER, R_int2uint(previous_handle));
    R_debug("R_frame_buffer_new(" R_FORMAT_STRING_FRAME_BUFFER ")",
            R_FORMAT_ARGS_FRAME_BUFFER(fb));
    return fb;
}


static void free_attachment(unsigned int *attachment,
                            R_FrameBufferAttachmentType type)
{
    switch (type) {
        case R_FRAME_BUFFER_ATTACHMENT_BUFFER:
            R_GL(glDeleteRenderbuffers, 1, attachment);
            break;
        case R_FRAME_BUFFER_ATTACHMENT_TEXTURE:
            R_GL(glDeleteTextures, 1, attachment);
            break;
        default:
            break;
    }
}

void R_frame_buffer_free(R_FrameBuffer *fb)
{
    if (fb) {
        R_GL_CLEAR_ERROR();
        R_GL(glDeleteFramebuffers, 1, &fb->handle);
        free_attachment(&fb->color,   fb->color_type);
        free_attachment(&fb->depth,   fb->depth_type);
        free_attachment(&fb->stencil, fb->stencil_type);
        free(fb->pixels);
        free(fb);
    }
}


void R_frame_buffer_bind(R_FrameBuffer *fb)
{
    R_GL_CLEAR_ERROR();
    if (fb) {
        R_GL(glBindFramebuffer, GL_FRAMEBUFFER, fb->handle);
        R_viewport_set((R_Viewport){0, 0, fb->width, fb->height});
    }
    else {
        R_GL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
        R_viewport_reset();
    }
}

void R_frame_buffer_unbind(void)
{
    R_frame_buffer_bind(NULL);
}


R_V2 R_frame_buffer_ratio(R_FrameBuffer *fb)
{
    return R_v2(R_int2float(fb->width)  / R_int2float(fb->real_width),
                R_int2float(fb->height) / R_int2float(fb->real_height));
}

unsigned char *R_frame_buffer_read(R_FrameBuffer *fb)
{
    if (!fb->pixels) {
        size_t size = R_int2size(fb->width) * R_int2size(fb->height) * 4;
        fb->pixels = R_ANEW(fb->pixels, size);
    }

    R_GL_CLEAR_ERROR();
    R_GL(glReadPixels, 0, 0, fb->width, fb->height, GL_RGBA,
                       GL_UNSIGNED_BYTE, fb->pixels);

    return fb->pixels;
}

void R_frame_buffer_write(R_FrameBuffer *fb, FILE *fp)
{
    unsigned char *pixels = R_frame_buffer_read(fb);
    int w = fb->width, h = fb->height;
    for (int y = 0; y < h; ++y) {
        int offset = (h - y - 1) * w * 4;
        fwrite(pixels + offset, 1, R_int2size(w) * 4, fp);
    }
}
