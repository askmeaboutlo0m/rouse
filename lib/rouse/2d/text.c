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
#include "nvg.h"
#include "refcount.h"
#include "text.h"


static int find_font(NVGcontext *ctx, const char *font_name)
{
    int font = nvgFindFont(ctx, font_name);
    if (font == -1) {
        R_die("Font '%s' not found", font_name);
    } else {
        return font;
    }
}

static inline void check_text_field(R_TextField *field)
{
    R_MAGIC_CHECK(field);
    R_assert(field->refs > 0, "must always be positive");
}

R_TextField *R_text_field_new(R_Nvg *nvg, R_String *string, NVGcolor color,
                              const char *font_name, float size)
{
    R_assert_not_null(font_name);
    int font = find_font(R_nvg_context(nvg), font_name);

    R_TextField *field = R_NEW_INIT_STRUCT(field, R_TextField,
            R_MAGIC_INIT(field) 1, string, color, font, size, 0.0f, 0.0f, 1.0f,
            NVG_ALIGN_TOP | NVG_ALIGN_LEFT, R_v2(0.0f, 0.0f), 0.0f);

    check_text_field(field);
    return field;
}

static void free_text_field(R_TextField *field)
{
    R_string_free(field->string);
    R_MAGIC_POISON_NN(field);
    free(field);
}

R_DEFINE_REFCOUNT_FUNCS(R_TextField, text_field, refs)


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

void R_text_field_draw(R_TextField *field, NVGcontext *ctx,
                       const float matrix[static 6])
{
    check_text_field(field);

    R_nvg_transform_set(ctx, matrix);
    nvgFillColor(ctx, field->color);
    nvgFontFaceId(ctx, field->font);
    nvgFontSize(ctx, field->size);
    nvgFontBlur(ctx, field->blur);
    nvgTextLetterSpacing(ctx, field->spacing);
    nvgTextLineHeight(ctx, field->line_height);
    nvgTextAlign(ctx, field->align);

    draw_text(ctx, field->string, field->pos, field->width);
}
