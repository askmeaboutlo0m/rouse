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
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <assert.h>
#include <cglm/struct.h>
#include "../3rdparty/nanovg_inc.h"
#include "../common.h"
#include "../string.h"
#include "../geom.h"
#include "text.h"


static int find_font(NVGcontext *vg, const char *font_name)
{
    int font = nvgFindFont(vg, font_name);
    if (font == -1) {
        R_die("Font '%s' not found", font_name);
    } else {
        return font;
    }
}

R_TextField *R_text_field_new(NVGcontext *vg, R_String *string, NVGcolor color,
                              const char *font_name, float size)
{
    assert(vg && "nanovg context can't be NULL");
    assert(vg && "font name can't be NULL");
    int font = find_font(vg, font_name);

    R_TextField *field = R_NEW_INIT_STRUCT(field, R_TextField,
            R_MAGIC_INIT(field) string, color, font, size, 0.0f, 0.0f, 1.0f,
            NVG_ALIGN_TOP | NVG_ALIGN_LEFT, R_v2(0.0f, 0.0f), 0.0f);

    R_MAGIC_CHECK(field);
    return field;
}

void R_text_field_free(R_TextField *field)
{
    if (field) {
        R_MAGIC_CHECK_NN(field);
        R_string_free(field->string);
        R_MAGIC_POISON(field);
        free(field);
    }
}


static void draw_text(NVGcontext *vg, R_String *string, R_V2 pos, float width)
{
    const char *start = R_string_body(string);
    const char *end   = start + R_string_len(string);
    nvgBeginPath(vg);
    if (width > 0.0f) {
        nvgTextBox(vg, pos.x, pos.y, width, start, end);
    }
    else {
        nvgText(vg, pos.x, pos.y, start, end);
    }
    nvgFill(vg);
}

void R_text_field_draw(R_TextField *field, NVGcontext *vg,
                       const float matrix[static 6])
{
    R_MAGIC_CHECK(field);

    R_nvg_transform_set(vg, matrix);
    nvgFillColor(vg, field->color);
    nvgFontFaceId(vg, field->font);
    nvgFontSize(vg, field->size);
    nvgFontBlur(vg, field->blur);
    nvgTextLetterSpacing(vg, field->spacing);
    nvgTextLineHeight(vg, field->line_height);
    nvgTextAlign(vg, field->align);

    draw_text(vg, field->string, field->pos, field->width);
}
