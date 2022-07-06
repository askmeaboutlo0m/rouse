/*
 * render/frame_buffer.h - OpenGL frame buffer handleage.
 *
 * Copyright (c) 2019 - 2022 askmeaboutloom
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

typedef enum R_FrameBufferAttachmentType {
    R_FRAME_BUFFER_ATTACHMENT_NONE,
    R_FRAME_BUFFER_ATTACHMENT_BUFFER,
    R_FRAME_BUFFER_ATTACHMENT_TEXTURE,
    R_FRAME_BUFFER_ATTACHMENT_CUBEMAP,
} R_FrameBufferAttachmentType;

typedef struct R_FrameBufferOptions {
    R_MAGIC_FIELD
    int                         width, height, samples, min_filter, mag_filter;
    R_FrameBufferAttachmentType color_type, depth_type, stencil_type;
} R_FrameBufferOptions;

typedef struct R_FrameBuffer {
    R_MAGIC_FIELD
    int                         width, height, real_width, real_height, samples;
    unsigned int                handle, color, depth, stencil;
    R_FrameBufferAttachmentType color_type, depth_type, stencil_type;
    unsigned char               *pixels;
} R_FrameBuffer;


R_FrameBufferOptions R_frame_buffer_options(void);

R_FrameBuffer *R_frame_buffer_new(const R_FrameBufferOptions *options);

R_FrameBufferOptions R_frame_buffer_2d_options(int width, int height);
R_FrameBuffer       *R_frame_buffer_2d_new    (int width, int height);

void R_frame_buffer_free(R_FrameBuffer *fb);

void R_frame_buffer_bind(R_FrameBuffer *fb);
void R_frame_buffer_unbind(void);

void R_frame_buffer_attach_cubemap_side(R_FrameBuffer *fb, unsigned int side);

R_V2 R_frame_buffer_ratio(R_FrameBuffer *fb);

unsigned char *R_frame_buffer_read(R_FrameBuffer *fb);

void R_frame_buffer_write(R_FrameBuffer *fb, FILE *fp);
