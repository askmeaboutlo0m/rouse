/*
 * text.h - text field for 2D text rendering in sprites
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

#define R_TEXT_FIELD_SIZE_DEFAULT  12
#define R_TEXT_FIELD_ALIGN_DEFAULT (NVG_ALIGN_TOP | NVG_ALIGN_LEFT)

typedef struct R_TextField {
    R_MAGIC_FIELD
    int       refs;
    R_String  *string;
    NVGcolor  color;
    int       font;
    float     size;
    float     blur;
    float     spacing;
    float     line_height;
    int       align;
    float     x, y;
    float     width;
} R_TextField;

R_TextField *R_text_field_new(void);

R_TextField *R_text_field_incref(R_TextField *field);
R_TextField *R_text_field_decref(R_TextField *field);
int R_text_field_refs(R_TextField *field);

R_V4 R_text_field_bounds(R_TextField *field, NVGcontext *ctx);

void R_text_field_draw(R_TextField *field, NVGcontext *ctx,
                       const float matrix[static 6]);
