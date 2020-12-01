/*
 * bitmap.h - 2D bitmap images loading and rendering
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

typedef struct R_BitmapImage R_BitmapImage;

R_BitmapImage *R_bitmap_image_new(R_Nvg *nvg, const char *name, int width,
                                  int height, const unsigned char *pixels,
                                  int flags);

R_BitmapImage *R_bitmap_image_from_file(R_Nvg *nvg, const char *path,
                                        int flags);

R_BitmapImage *R_bitmap_image_incref(R_BitmapImage *bi);
R_BitmapImage *R_bitmap_image_decref(R_BitmapImage *bi);
int R_bitmap_image_refs(R_BitmapImage *bi);

int R_bitmap_image_width (R_BitmapImage *bi);
int R_bitmap_image_height(R_BitmapImage *bi);

void R_bitmap_image_draw(R_BitmapImage *bi, NVGcontext *ctx,
                         const float matrix[R_STATIC(6)]);
