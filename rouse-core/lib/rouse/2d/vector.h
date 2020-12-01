/*
 * vector.h - 2D vector images loading and rendering
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

typedef struct R_VectorImage R_VectorImage;


R_VectorImage *R_vector_image_new(const char *title, R_ParseReadFn read,
                                  R_UserData user, int bufsize,
                                  unsigned char buffer[R_STATIC(bufsize)]);

R_VectorImage *R_vector_image_from_file(const char *path);

R_VectorImage *R_vector_image_incref(R_VectorImage *vi);
R_VectorImage *R_vector_image_decref(R_VectorImage *vi);
int R_vector_image_refs(R_VectorImage *vi);

int R_vector_image_width (R_VectorImage *vi);
int R_vector_image_height(R_VectorImage *vi);

void R_vector_image_draw(R_VectorImage *vi, NVGcontext *ctx,
                         const float parent_matrix[R_STATIC(6)]);

void R_vector_image_dump(R_VectorImage *vi, FILE *fp);
