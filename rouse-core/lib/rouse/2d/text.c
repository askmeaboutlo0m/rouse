/*
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
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <cglm/struct.h>
#include "../3rdparty/nanovg_inc.h"
#include <rouse_config.h>
#include "../common.h"
#include "../string.h"
#include "../geom.h"
#include "nvg.h"
#include "../refcount.h"
#include "text.h"


static inline void check_text_field(
    R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_TextField *field)
{
    R_MAGIC_CHECK(R_TextField, field);
    R_assert(field->refs > 0, "must always be positive");
}

R_TextField *R_text_field_new(void)
{
    R_TextField *field = R_NEW_INIT_STRUCT(field, R_TextField,
            R_MAGIC_INIT(R_TextField) 1, R_string_new(0), nvgRGB(0, 0, 0),
            -1, R_TEXT_FIELD_SIZE_DEFAULT, 0.0f, 1.0f, 0.0f, 1.0f,
            R_TEXT_FIELD_ALIGN_DEFAULT, 0.0f, 0.0f, 0.0f, nvgRGB(0, 0, 0),
            0.0f, 1.0f);
    check_text_field(field);
    return field;
}

static void free_text_field(R_TextField *field)
{
    R_string_free(field->string);
    R_MAGIC_POISON(R_TextField, field);
    free(field);
}

R_DEFINE_REFCOUNT_FUNCS(R_TextField, text_field, refs)


static void set_nvg_state(R_TextField *field, NVGcontext *ctx)
{
    nvgFontFaceId(ctx, field->font);
    nvgFontSize(ctx, field->size);
    nvgTextLetterSpacing(ctx, field->spacing);
    nvgTextLineHeight(ctx, field->line_height);
    nvgTextAlign(ctx, field->align);
}

R_V4 R_text_field_bounds(R_TextField *field, NVGcontext *ctx)
{
    check_text_field(field);
    int      font    = field->font;
    R_String *string = field->string;

    float  x = field->x;
    float  y = field->y;
    size_t len;

    if (font != -1 && string && (len = R_string_len(string)) > 0) {
        const char *start = R_string_body(string);
        const char *end   = start + len;
        set_nvg_state(field, ctx);
        nvgFontBlur(ctx, R_MAX(field->blur, field->outline_blur));
        R_V4 bounds;
        if (field->width > 0.0f) {
            nvgTextBoxBounds(ctx, x, y, field->width, start, end, bounds.raw);
        }
        else {
            nvgTextBounds(ctx, x, y, start, end, bounds.raw);
        }
        return bounds;
    }
    else {
        return R_v4(x, y, x, y);
    }
}


static void draw_text(NVGcontext *ctx, R_String *string,
                      float x, float y, float width, NVGcolor color, float blur,
                      float softness, NVGcolor outline_color,
                      float outline_blur, float outline_softness)
{
    const char *start = R_string_body(string);
    const char *end   = start + R_string_len(string);
    nvgBeginPath(ctx);
    if (width > 0.0f) {
        if (outline_blur > 0.0f) {
            nvgFillColor(ctx, outline_color);
            nvgFontBlur(ctx, outline_blur);
            nvgFontBlurSoftness(ctx, outline_softness);
            nvgTextBox(ctx, x, y, width, start, end);
        }
        nvgFillColor(ctx, color);
        nvgFontBlur(ctx, blur);
        nvgFontBlurSoftness(ctx, softness);
        nvgTextBox(ctx, x, y, width, start, end);
    }
    else {
        if (outline_blur > 0.0f) {
            nvgFillColor(ctx, outline_color);
            nvgFontBlur(ctx, outline_blur);
            nvgFontBlurSoftness(ctx, outline_softness);
            nvgText(ctx, x, y, start, end);
        }
        nvgFillColor(ctx, color);
        nvgFontBlur(ctx, blur);
        nvgFontBlurSoftness(ctx, softness);
        nvgText(ctx, x, y, start, end);
    }
    nvgFill(ctx);
}

void R_text_field_draw(R_TextField *field, NVGcontext *ctx,
                       const float matrix[static 6])
{
    check_text_field(field);
    R_String *string = field->string;
    if (field->font != -1 && string && R_string_len(string) > 0) {
        R_nvg_transform_set(ctx, matrix);
        set_nvg_state(field, ctx);
        draw_text(ctx, string, field->x, field->y, field->width, field->color,
                  field->blur, field->softness, field->outline_color,
                  field->outline_blur, field->outline_softness);
    }
}
