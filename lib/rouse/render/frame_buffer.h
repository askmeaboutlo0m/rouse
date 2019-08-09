/*
 * render/frame_buffer.h - OpenGL frame buffer handleage.
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

#define R_FRAME_BUFFER_NONE          0x0
#define R_FRAME_BUFFER_COLOR_BUFFER  0x1
#define R_FRAME_BUFFER_COLOR_TEXTURE 0x2
#define R_FRAME_BUFFER_DEPTH_BUFFER  0x4
#define R_FRAME_BUFFER_DEPTH_TEXTURE 0x8

typedef struct R_FrameBuffer {
    int           flags, width, height, real_width, real_height;
    unsigned int  handle, color, depth;
    unsigned char *pixels;
} R_FrameBuffer;


R_FrameBuffer *R_frame_buffer_new(int flags, int width, int height);

void R_frame_buffer_free(R_FrameBuffer *fb);

void R_frame_buffer_bind(R_FrameBuffer *fb);
void R_frame_buffer_unbind(void);

R_V2 R_frame_buffer_ratio(R_FrameBuffer *fb);

unsigned char *R_frame_buffer_read(R_FrameBuffer *fb);

void R_frame_buffer_write(R_FrameBuffer *fb, FILE *fp);
