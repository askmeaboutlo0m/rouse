/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2019, 2020 askmeaboutloom
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
#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"

static void check_matrix_index(lua_State *L, lua_Integer i, int arg)
{
    if (i < 1 || i > 6) {
        luaL_argerror(L, arg, "matrix index must be >= 1 and <= 6");
    }
}


static int r_luanvgtransform_intindex_xl(lua_State *L)
{
    R_LuaNvgTransform *self = R_CPPCAST(R_LuaNvgTransform *, XL_checkutype(L, 1, "R_LuaNvgTransform"));
    lua_Integer INDEX = luaL_checkinteger(L, 2);
    float RETVAL;
    check_matrix_index(L, INDEX, 2);
    RETVAL = self->matrix[INDEX - 1];
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_luanvgtransform_intnewindex_xl(lua_State *L)
{
    R_LuaNvgTransform *self = R_CPPCAST(R_LuaNvgTransform *, XL_checkutype(L, 1, "R_LuaNvgTransform"));
    lua_Integer INDEX = luaL_checkinteger(L, 2);
    float VALUE = XL_checkfloat(L, 3);
    check_matrix_index(L, INDEX, 2);
    self->matrix[INDEX - 1] = VALUE;
    return 0;
}

static int r_luanvgtransform_method_len_xl(lua_State *L)
{
    R_LuaNvgTransform *self = R_CPPCAST(R_LuaNvgTransform *, XL_checkutype(L, 1, "R_LuaNvgTransform"));
    lua_Integer RETVAL;
    XL_UNUSED(self);
    RETVAL = 6;
    lua_pushinteger(L, RETVAL);
    return 1;
}

static int r_luanvgtransform_method_tostring_xl(lua_State *L)
{
    R_LuaNvgTransform *self = R_CPPCAST(R_LuaNvgTransform *, XL_checkutype(L, 1, "R_LuaNvgTransform"));
    lua_pushfstring(L, "[a = %f, b = %f, c = %f, d = %f, e = %f, f = %f]",
                       self->matrix[0], self->matrix[1], self->matrix[2],
                       self->matrix[3], self->matrix[4], self->matrix[5]);
    return 1;
}


static void check_color_index(lua_State *L, lua_Integer i, int arg)
{
    if (i < 1 || i > 4) {
        luaL_argerror(L, arg, "color index must be >= 1 and <= 4");
    }
}


static int nvgcolor_r_index_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float RETVAL;
    RETVAL = self->r;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int nvgcolor_r_newindex_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float VALUE = XL_checkfloat(L, 2);
    self->r = VALUE;
    return 0;
}

static int nvgcolor_g_index_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float RETVAL;
    RETVAL = self->g;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int nvgcolor_g_newindex_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float VALUE = XL_checkfloat(L, 2);
    self->g = VALUE;
    return 0;
}

static int nvgcolor_b_index_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float RETVAL;
    RETVAL = self->b;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int nvgcolor_b_newindex_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float VALUE = XL_checkfloat(L, 2);
    self->b = VALUE;
    return 0;
}

static int nvgcolor_a_index_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float RETVAL;
    RETVAL = self->a;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int nvgcolor_a_newindex_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float VALUE = XL_checkfloat(L, 2);
    self->a = VALUE;
    return 0;
}

static int nvgcolor_intindex_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    lua_Integer INDEX = luaL_checkinteger(L, 2);
    float RETVAL;
    check_color_index(L, INDEX, 2);
    RETVAL = self->rgba[INDEX - 1];
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int nvgcolor_intnewindex_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    lua_Integer INDEX = luaL_checkinteger(L, 2);
    float VALUE = XL_checkfloat(L, 3);
    check_color_index(L, INDEX, 2);
    self->rgba[INDEX - 1] = VALUE;
    return 0;
}

static int nvgcolor_method_unpack_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    XL_pushfloat(L, self->r);
    XL_pushfloat(L, self->g);
    XL_pushfloat(L, self->b);
    XL_pushfloat(L, self->a);
    return 4;
}

static int nvgcolor_method_len_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    lua_Integer RETVAL;
    XL_UNUSED(self);
    RETVAL = 4;
    lua_pushinteger(L, RETVAL);
    return 1;
}

static int nvgcolor_method_tostring_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    lua_pushfstring(L, "[r = %f, g = %f, b = %f, a = %f]",
                       self->r, self->g, self->b, self->a);
    return 1;
}

static int nvgcolor_method_with_r_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    unsigned char r = XL_checkuchar(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(r / 255.0f, self->g, self->b, self->a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int nvgcolor_method_with_rf_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float r = XL_checkfloat(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(r, self->g, self->b, self->a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int nvgcolor_method_with_g_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    unsigned char g = XL_checkuchar(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(self->r, g / 255.0f, self->b, self->a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int nvgcolor_method_with_gf_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float g = XL_checkfloat(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(self->r, g, self->b, self->a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int nvgcolor_method_with_b_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    unsigned char b = XL_checkuchar(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(self->r, self->g, b / 255.0f, self->a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int nvgcolor_method_with_bf_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float b = XL_checkfloat(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(self->r, self->g, b, self->a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int nvgcolor_method_with_a_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    unsigned char a = XL_checkuchar(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(self->r, self->g, self->b, a / 255.0f);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int nvgcolor_method_with_af_xl(lua_State *L)
{
    NVGcolor *self = R_CPPCAST(NVGcolor *, XL_checkutype(L, 1, "NVGcolor"));
    float a = XL_checkfloat(L, 2);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(self->r, self->g, self->b, a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_new_xl(lua_State *L)
{
    int flags = XL_checkint(L, 1);
    R_Nvg *RETVAL;
    RETVAL = R_nvg_new(flags);
    XL_pushnewpptypeuv(L, RETVAL, "R_Nvg", 0);
    return 1;
}

static int r_nvg_method_gc_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype_nullable(L, 1, "R_Nvg"));
    R_nvg_decref(self);
    return 0;
}


static NVGcolor rgbxa(uint32_t rgb, float a)
{
    return nvgRGBAf((unsigned char)((rgb >> 16u) & 0xffu) / 255.0f,
                    (unsigned char)((rgb >>  8u) & 0xffu) / 255.0f,
                    (unsigned char)((rgb >>  0u) & 0xffu) / 255.0f,
                    a);
}

static NVGcolor rgbx(uint32_t rgb)
{
    return rgbxa(rgb, 1.0);
}


static int r_nvg_color_xl(lua_State *L)
{
    NVGcolor RETVAL;
    int argc = lua_gettop(L);
    if (argc == 1) { /* rgb: (0xabcdef) */
        RETVAL = rgbx(XL_checkuint32(L, 1));
    } else if (argc == 2) { /* rgb and float alpha: (0xabcedf, 1.0) */
        uint32_t rgb = XL_checkuint32(L, 1);
        float    a   = XL_checkfloat(L, 2);
        RETVAL       = rgbxa(rgb, a);
    } else if (argc == 3) { /* r, g and b: (171, 205, 239) */
        unsigned char r = XL_checkuchar(L, 1);
        unsigned char g = XL_checkuchar(L, 2);
        unsigned char b = XL_checkuchar(L, 3);
        RETVAL          = nvgRGB(r, g, b);
    } else if (argc == 4) { /* r, g, b and float alpha: (171, 205, 239, 1.0) */
        float r = XL_checkuchar(L, 1) / 255.0f;
        float g = XL_checkuchar(L, 2) / 255.0f;
        float b = XL_checkuchar(L, 3) / 255.0f;
        float a = XL_checkfloat(L, 4);
        RETVAL  = nvgRGBAf(r, g, b, a);
    } else {
        R_LUA_DIE(L, "Wrong number of arguments to R.Nvg.color: %d", argc);
    }
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_rgb_xl(lua_State *L)
{
    unsigned char r = XL_checkuchar(L, 1);
    unsigned char g = XL_checkuchar(L, 2);
    unsigned char b = XL_checkuchar(L, 3);
    NVGcolor RETVAL;
    RETVAL = nvgRGB(r, g, b);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_rgbf_xl(lua_State *L)
{
    float r = XL_checkfloat(L, 1);
    float g = XL_checkfloat(L, 2);
    float b = XL_checkfloat(L, 3);
    NVGcolor RETVAL;
    RETVAL = nvgRGBf(r, g, b);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_rgba_xl(lua_State *L)
{
    unsigned char r = XL_checkuchar(L, 1);
    unsigned char g = XL_checkuchar(L, 2);
    unsigned char b = XL_checkuchar(L, 3);
    unsigned char a = XL_checkuchar(L, 4);
    NVGcolor RETVAL;
    RETVAL = nvgRGBA(r, g, b, a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_rgbaf_xl(lua_State *L)
{
    float r = XL_checkfloat(L, 1);
    float g = XL_checkfloat(L, 2);
    float b = XL_checkfloat(L, 3);
    float a = XL_checkfloat(L, 4);
    NVGcolor RETVAL;
    RETVAL = nvgRGBAf(r, g, b, a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_rgbx_xl(lua_State *L)
{
    uint32_t rgb = XL_checkuint32(L, 1);
    NVGcolor RETVAL;
    RETVAL = rgbx(rgb);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_rgbxa_xl(lua_State *L)
{
    uint32_t rgb = XL_checkuint32(L, 1);
    float a = XL_checkfloat(L, 2);
    NVGcolor RETVAL;
    RETVAL = rgbxa(rgb, a);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_rgbax_xl(lua_State *L)
{
    uint32_t rgba = XL_checkuint32(L, 1);
    NVGcolor RETVAL;
    RETVAL = nvgRGBA((unsigned char)((rgba >> 24u) & 0xffu),
                     (unsigned char)((rgba >> 16u) & 0xffu),
                     (unsigned char)((rgba >>  8u) & 0xffu),
                     (unsigned char)((rgba >>  0u) & 0xffu));
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGcolor), "NVGcolor", 0);
    return 1;
}

static int r_nvg_method_image_pattern_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float ox = XL_checkfloat(L, 2);
    float oy = XL_checkfloat(L, 3);
    float ex = XL_checkfloat(L, 4);
    float ey = XL_checkfloat(L, 5);
    float angle = XL_checkfloat(L, 6);
    R_BitmapImage *image = R_CPPCAST(R_BitmapImage *, XL_checkpptype(L, 7, "R_BitmapImage"));
    float alpha = XL_checkfloat(L, 8);
    NVGpaint RETVAL;
    RETVAL = R_bitmap_image_pattern(image, self, ox, oy, ex, ey, angle, alpha);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGpaint), "NVGpaint", 0);
    return 1;
}

static int r_nvg_method_create_font_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    const char *name = luaL_checkstring(L, 2);
    const char *path = luaL_checkstring(L, 3);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    int RETVAL;
    RETVAL = nvgCreateFont(self, name, path);
    if (RETVAL == -1) {
        luaL_error(L, "Can't load font '%s' from '%s'", name, path);
    }
#   undef self
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_nvg_method_begin_frame_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float window_width = XL_checkfloat(L, 2);
    float window_height = XL_checkfloat(L, 3);
    float device_pixel_ratio = XL_checkfloat(L, 4);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgBeginFrame(self, window_width, window_height, device_pixel_ratio);
#   undef self
    return 0;
}

static int r_nvg_method_cancel_frame_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgCancelFrame(self);
#   undef self
    return 0;
}

static int r_nvg_method_end_frame_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgEndFrame(self);
#   undef self
    return 0;
}

static int r_nvg_method_save_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgSave(self);
#   undef self
    return 0;
}

static int r_nvg_method_restore_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgRestore(self);
#   undef self
    return 0;
}

static int r_nvg_method_reset_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgReset(self);
#   undef self
    return 0;
}

static int r_nvg_method_linear_gradient_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float sx = XL_checkfloat(L, 2);
    float sy = XL_checkfloat(L, 3);
    float ex = XL_checkfloat(L, 4);
    float ey = XL_checkfloat(L, 5);
    NVGcolor icol = *((NVGcolor *)luaL_checkudata(L, 6, "NVGcolor"));
    NVGcolor ocol = *((NVGcolor *)luaL_checkudata(L, 7, "NVGcolor"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    NVGpaint RETVAL;
    RETVAL = nvgLinearGradient(self, sx, sy, ex, ey, icol, ocol);
#   undef self
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGpaint), "NVGpaint", 0);
    return 1;
}

static int r_nvg_method_box_gradient_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    float w = XL_checkfloat(L, 4);
    float h = XL_checkfloat(L, 5);
    float r = XL_checkfloat(L, 6);
    float f = XL_checkfloat(L, 7);
    NVGcolor icol = *((NVGcolor *)luaL_checkudata(L, 8, "NVGcolor"));
    NVGcolor ocol = *((NVGcolor *)luaL_checkudata(L, 9, "NVGcolor"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    NVGpaint RETVAL;
    RETVAL = nvgBoxGradient(self, x, y, w, h, r, f, icol, ocol);
#   undef self
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGpaint), "NVGpaint", 0);
    return 1;
}

static int r_nvg_method_radial_gradient_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float cx = XL_checkfloat(L, 2);
    float cy = XL_checkfloat(L, 3);
    float inr = XL_checkfloat(L, 4);
    float outr = XL_checkfloat(L, 5);
    NVGcolor icol = *((NVGcolor *)luaL_checkudata(L, 6, "NVGcolor"));
    NVGcolor ocol = *((NVGcolor *)luaL_checkudata(L, 7, "NVGcolor"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    NVGpaint RETVAL;
    RETVAL = nvgRadialGradient(self, cx, cy, inr, outr, icol, ocol);
#   undef self
    XL_pushnewutypeuv(L, &RETVAL, sizeof(NVGpaint), "NVGpaint", 0);
    return 1;
}

static int r_nvg_method_shape_anti_alias_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    bool enabled = XL_checkbool(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgShapeAntiAlias(self, enabled);
#   undef self
    return 0;
}

static int r_nvg_method_stroke_color_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcolor color = *((NVGcolor *)luaL_checkudata(L, 2, "NVGcolor"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgStrokeColor(self, color);
#   undef self
    return 0;
}

static int r_nvg_method_stroke_paint_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGpaint paint = *((NVGpaint *)luaL_checkudata(L, 2, "NVGpaint"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgStrokePaint(self, paint);
#   undef self
    return 0;
}

static int r_nvg_method_fill_color_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcolor color = *((NVGcolor *)luaL_checkudata(L, 2, "NVGcolor"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgFillColor(self, color);
#   undef self
    return 0;
}

static int r_nvg_method_fill_paint_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGpaint paint = *((NVGpaint *)luaL_checkudata(L, 2, "NVGpaint"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgFillPaint(self, paint);
#   undef self
    return 0;
}

static int r_nvg_method_miter_limit_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float limit = XL_checkfloat(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgMiterLimit(self, limit);
#   undef self
    return 0;
}

static int r_nvg_method_stroke_width_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float size = XL_checkfloat(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgStrokeWidth(self, size);
#   undef self
    return 0;
}

static int r_nvg_method_line_cap_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    int cap = XL_checkint(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgLineCap(self, cap);
#   undef self
    return 0;
}

static int r_nvg_method_line_join_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    int join = XL_checkint(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgLineJoin(self, join);
#   undef self
    return 0;
}

static int r_nvg_method_global_alpha_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float alpha = XL_checkfloat(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgGlobalAlpha(self, alpha);
#   undef self
    return 0;
}

static int r_nvg_method_reset_transform_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgResetTransform(self);
#   undef self
    return 0;
}

static int r_nvg_method_transform_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float a = XL_checkfloat(L, 2);
    float b = XL_checkfloat(L, 3);
    float c = XL_checkfloat(L, 4);
    float d = XL_checkfloat(L, 5);
    float e = XL_checkfloat(L, 6);
    float f = XL_checkfloat(L, 7);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgTransform(self, a, b, c, d, e, f);
#   undef self
    return 0;
}

static int r_nvg_method_translate_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgTranslate(self, x, y);
#   undef self
    return 0;
}

static int r_nvg_method_rotate_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float angle = XL_checkfloat(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgRotate(self, angle);
#   undef self
    return 0;
}

static int r_nvg_method_skew_x_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float angle = XL_checkfloat(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgSkewX(self, angle);
#   undef self
    return 0;
}

static int r_nvg_method_skew_y_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float angle = XL_checkfloat(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgSkewY(self, angle);
#   undef self
    return 0;
}

static int r_nvg_method_scale_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgScale(self, x, y);
#   undef self
    return 0;
}

static int r_nvg_method_set_transform_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    R_LuaNvgTransform *transform = R_CPPCAST(R_LuaNvgTransform *, XL_checkutype(L, 2, "R_LuaNvgTransform"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    R_nvg_transform_set(self, transform->matrix);
#   undef self
    return 0;
}

static int r_nvg_method_transform_by_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    R_LuaNvgTransform *transform = R_CPPCAST(R_LuaNvgTransform *, XL_checkutype(L, 2, "R_LuaNvgTransform"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    R_nvg_transform(self, transform->matrix);
#   undef self
    return 0;
}

static int r_nvg_method_scissor_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    float w = XL_checkfloat(L, 4);
    float h = XL_checkfloat(L, 5);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgScissor(self, x, y, w, h);
#   undef self
    return 0;
}

static int r_nvg_method_intersect_scissor_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    float w = XL_checkfloat(L, 4);
    float h = XL_checkfloat(L, 5);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgIntersectScissor(self, x, y, w, h);
#   undef self
    return 0;
}

static int r_nvg_method_reset_scissor_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgResetScissor(self);
#   undef self
    return 0;
}

static int r_nvg_method_begin_path_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgBeginPath(self);
#   undef self
    return 0;
}

static int r_nvg_method_move_to_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgMoveTo(self, x, y);
#   undef self
    return 0;
}

static int r_nvg_method_line_to_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgLineTo(self, x, y);
#   undef self
    return 0;
}

static int r_nvg_method_bezier_to_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float c1x = XL_checkfloat(L, 2);
    float c1y = XL_checkfloat(L, 3);
    float c2x = XL_checkfloat(L, 4);
    float c2y = XL_checkfloat(L, 5);
    float x = XL_checkfloat(L, 6);
    float y = XL_checkfloat(L, 7);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgBezierTo(self, c1x, c1y, c2x, c2y, x, y);
#   undef self
    return 0;
}

static int r_nvg_method_quad_to_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float cx = XL_checkfloat(L, 2);
    float cy = XL_checkfloat(L, 3);
    float x = XL_checkfloat(L, 4);
    float y = XL_checkfloat(L, 5);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgQuadTo(self, cx, cy, x, y);
#   undef self
    return 0;
}

static int r_nvg_method_arc_to_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x1 = XL_checkfloat(L, 2);
    float y1 = XL_checkfloat(L, 3);
    float x2 = XL_checkfloat(L, 4);
    float y2 = XL_checkfloat(L, 5);
    float radius = XL_checkfloat(L, 6);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgArcTo(self, x1, y1, x2, y2, radius);
#   undef self
    return 0;
}

static int r_nvg_method_close_path_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgClosePath(self);
#   undef self
    return 0;
}

static int r_nvg_method_path_winding_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    int dir = XL_checkint(L, 2);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgPathWinding(self, dir);
#   undef self
    return 0;
}

static int r_nvg_method_arc_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float cx = XL_checkfloat(L, 2);
    float cy = XL_checkfloat(L, 3);
    float r = XL_checkfloat(L, 4);
    float a0 = XL_checkfloat(L, 5);
    float a1 = XL_checkfloat(L, 6);
    int dir = XL_checkint(L, 7);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgArc(self, cx, cy, r, a0, a1, dir);
#   undef self
    return 0;
}

static int r_nvg_method_rect_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    float w = XL_checkfloat(L, 4);
    float h = XL_checkfloat(L, 5);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgRect(self, x, y, w, h);
#   undef self
    return 0;
}

static int r_nvg_method_rounded_rect_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    float w = XL_checkfloat(L, 4);
    float h = XL_checkfloat(L, 5);
    float r = XL_checkfloat(L, 6);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgRoundedRect(self, x, y, w, h, r);
#   undef self
    return 0;
}

static int r_nvg_method_rounded_rect_varying_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float x = XL_checkfloat(L, 2);
    float y = XL_checkfloat(L, 3);
    float w = XL_checkfloat(L, 4);
    float h = XL_checkfloat(L, 5);
    float rad_top_left = XL_checkfloat(L, 6);
    float rad_top_right = XL_checkfloat(L, 7);
    float rad_bottom_right = XL_checkfloat(L, 8);
    float rad_bottom_left = XL_checkfloat(L, 9);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgRoundedRectVarying(self, x, y, w, h, rad_top_left, rad_top_right, rad_bottom_right, rad_bottom_left);
#   undef self
    return 0;
}

static int r_nvg_method_ellipse_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float cx = XL_checkfloat(L, 2);
    float cy = XL_checkfloat(L, 3);
    float rx = XL_checkfloat(L, 4);
    float ry = XL_checkfloat(L, 5);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgEllipse(self, cx, cy, rx, ry);
#   undef self
    return 0;
}

static int r_nvg_method_circle_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    float cx = XL_checkfloat(L, 2);
    float cy = XL_checkfloat(L, 3);
    float r = XL_checkfloat(L, 4);
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgCircle(self, cx, cy, r);
#   undef self
    return 0;
}

static int r_nvg_method_fill_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgFill(self);
#   undef self
    return 0;
}

static int r_nvg_method_stroke_xl(lua_State *L)
{
    R_Nvg *self = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    NVGcontext *ctx = R_nvg_context(self);
#   define self ctx
    nvgStroke(self);
#   undef self
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

static int nvgcolor_index_dummy_xl;
static int nvgcolor_index_xl(lua_State *L)
{
    if (lua_isinteger(L, 2)) {
        return nvgcolor_intindex_xl(L);
    }
    return XL_index(L, "NVGcolor", &nvgcolor_index_dummy_xl, 1, 2);
}

static int r_luanvgtransform_index_xl(lua_State *L)
{
    if (lua_isinteger(L, 2)) {
        return r_luanvgtransform_intindex_xl(L);
    }
    return XL_index_fallback(L, "R_LuaNvgTransform", 1, 2);
}

static int r_nvg_index_xl(lua_State *L)
{
    return XL_index_fallback(L, "R_Nvg", 1, 2);
}

static int nvgcolor_newindex_dummy_xl;
static int nvgcolor_newindex_xl(lua_State *L)
{
    if (lua_isinteger(L, 2)) {
        return nvgcolor_intnewindex_xl(L);
    }
    return XL_newindex(L, "NVGcolor", &nvgcolor_newindex_dummy_xl, 1, 2, 3);
}

#define r_luanvgtransform_newindex_xl r_luanvgtransform_intnewindex_xl

static luaL_Reg r_nvg_function_registry_xl[] = {
    {"color", r_nvg_color_xl},
    {"new", r_nvg_new_xl},
    {"rgb", r_nvg_rgb_xl},
    {"rgba", r_nvg_rgba_xl},
    {"rgbaf", r_nvg_rgbaf_xl},
    {"rgbax", r_nvg_rgbax_xl},
    {"rgbf", r_nvg_rgbf_xl},
    {"rgbx", r_nvg_rgbx_xl},
    {"rgbxa", r_nvg_rgbxa_xl},
    {NULL, NULL},
};

static luaL_Reg nvgcolor_index_registry_xl[] = {
    {"a", nvgcolor_a_index_xl},
    {"b", nvgcolor_b_index_xl},
    {"g", nvgcolor_g_index_xl},
    {"r", nvgcolor_r_index_xl},
    {NULL, NULL},
};

static luaL_Reg nvgcolor_method_registry_xl[] = {
    {"__index", nvgcolor_index_xl},
    {"__len", nvgcolor_method_len_xl},
    {"__newindex", nvgcolor_newindex_xl},
    {"__tostring", nvgcolor_method_tostring_xl},
    {"unpack", nvgcolor_method_unpack_xl},
    {"with_a", nvgcolor_method_with_a_xl},
    {"with_af", nvgcolor_method_with_af_xl},
    {"with_b", nvgcolor_method_with_b_xl},
    {"with_bf", nvgcolor_method_with_bf_xl},
    {"with_g", nvgcolor_method_with_g_xl},
    {"with_gf", nvgcolor_method_with_gf_xl},
    {"with_r", nvgcolor_method_with_r_xl},
    {"with_rf", nvgcolor_method_with_rf_xl},
    {NULL, NULL},
};

static luaL_Reg r_luanvgtransform_method_registry_xl[] = {
    {"__index", r_luanvgtransform_index_xl},
    {"__len", r_luanvgtransform_method_len_xl},
    {"__newindex", r_luanvgtransform_newindex_xl},
    {"__tostring", r_luanvgtransform_method_tostring_xl},
    {NULL, NULL},
};

static luaL_Reg r_nvg_method_registry_xl[] = {
    {"__gc", r_nvg_method_gc_xl},
    {"__index", r_nvg_index_xl},
    {"arc", r_nvg_method_arc_xl},
    {"arc_to", r_nvg_method_arc_to_xl},
    {"begin_frame", r_nvg_method_begin_frame_xl},
    {"begin_path", r_nvg_method_begin_path_xl},
    {"bezier_to", r_nvg_method_bezier_to_xl},
    {"box_gradient", r_nvg_method_box_gradient_xl},
    {"cancel_frame", r_nvg_method_cancel_frame_xl},
    {"circle", r_nvg_method_circle_xl},
    {"close_path", r_nvg_method_close_path_xl},
    {"create_font", r_nvg_method_create_font_xl},
    {"ellipse", r_nvg_method_ellipse_xl},
    {"end_frame", r_nvg_method_end_frame_xl},
    {"fill", r_nvg_method_fill_xl},
    {"fill_color", r_nvg_method_fill_color_xl},
    {"fill_paint", r_nvg_method_fill_paint_xl},
    {"global_alpha", r_nvg_method_global_alpha_xl},
    {"image_pattern", r_nvg_method_image_pattern_xl},
    {"intersect_scissor", r_nvg_method_intersect_scissor_xl},
    {"line_cap", r_nvg_method_line_cap_xl},
    {"line_join", r_nvg_method_line_join_xl},
    {"line_to", r_nvg_method_line_to_xl},
    {"linear_gradient", r_nvg_method_linear_gradient_xl},
    {"miter_limit", r_nvg_method_miter_limit_xl},
    {"move_to", r_nvg_method_move_to_xl},
    {"path_winding", r_nvg_method_path_winding_xl},
    {"quad_to", r_nvg_method_quad_to_xl},
    {"radial_gradient", r_nvg_method_radial_gradient_xl},
    {"rect", r_nvg_method_rect_xl},
    {"reset", r_nvg_method_reset_xl},
    {"reset_scissor", r_nvg_method_reset_scissor_xl},
    {"reset_transform", r_nvg_method_reset_transform_xl},
    {"restore", r_nvg_method_restore_xl},
    {"rotate", r_nvg_method_rotate_xl},
    {"rounded_rect", r_nvg_method_rounded_rect_xl},
    {"rounded_rect_varying", r_nvg_method_rounded_rect_varying_xl},
    {"save", r_nvg_method_save_xl},
    {"scale", r_nvg_method_scale_xl},
    {"scissor", r_nvg_method_scissor_xl},
    {"set_transform", r_nvg_method_set_transform_xl},
    {"shape_anti_alias", r_nvg_method_shape_anti_alias_xl},
    {"skew_x", r_nvg_method_skew_x_xl},
    {"skew_y", r_nvg_method_skew_y_xl},
    {"stroke", r_nvg_method_stroke_xl},
    {"stroke_color", r_nvg_method_stroke_color_xl},
    {"stroke_paint", r_nvg_method_stroke_paint_xl},
    {"stroke_width", r_nvg_method_stroke_width_xl},
    {"transform", r_nvg_method_transform_xl},
    {"transform_by", r_nvg_method_transform_by_xl},
    {"translate", r_nvg_method_translate_xl},
    {NULL, NULL},
};

static luaL_Reg nvgcolor_newindex_registry_xl[] = {
    {"a", nvgcolor_a_newindex_xl},
    {"b", nvgcolor_b_newindex_xl},
    {"g", nvgcolor_g_newindex_xl},
    {"r", nvgcolor_r_newindex_xl},
    {NULL, NULL},
};

static XL_EnumEntry r_nvg_winding_enum_xl[] = {
    {"CCW", (lua_Integer) NVG_CCW},
    {"CW", (lua_Integer) NVG_CW},
    {NULL, (lua_Integer) 0},
};

static XL_EnumEntry r_nvg_solidity_enum_xl[] = {
    {"SOLID", (lua_Integer) NVG_SOLID},
    {"HOLE", (lua_Integer) NVG_HOLE},
    {NULL, (lua_Integer) 0},
};

static XL_EnumEntry r_nvg_linecap_enum_xl[] = {
    {"BUTT", (lua_Integer) NVG_BUTT},
    {"ROUND", (lua_Integer) NVG_ROUND},
    {"SQUARE", (lua_Integer) NVG_SQUARE},
    {"BEVEL", (lua_Integer) NVG_BEVEL},
    {"MITER", (lua_Integer) NVG_MITER},
    {NULL, (lua_Integer) 0},
};

static XL_EnumEntry r_nvg_align_enum_xl[] = {
    {"LEFT", (lua_Integer) NVG_ALIGN_LEFT},
    {"CENTER", (lua_Integer) NVG_ALIGN_CENTER},
    {"RIGHT", (lua_Integer) NVG_ALIGN_RIGHT},
    {"TOP", (lua_Integer) NVG_ALIGN_TOP},
    {"MIDDLE", (lua_Integer) NVG_ALIGN_MIDDLE},
    {"BOTTOM", (lua_Integer) NVG_ALIGN_BOTTOM},
    {"BASELINE", (lua_Integer) NVG_ALIGN_BASELINE},
    {NULL, (lua_Integer) 0},
};

static XL_EnumEntry r_nvg_blendfactor_enum_xl[] = {
    {"ZERO", (lua_Integer) NVG_ZERO},
    {"ONE", (lua_Integer) NVG_ONE},
    {"SRC_COLOR", (lua_Integer) NVG_SRC_COLOR},
    {"ONE_MINUS_SRC_COLOR", (lua_Integer) NVG_ONE_MINUS_SRC_COLOR},
    {"DST_COLOR", (lua_Integer) NVG_DST_COLOR},
    {"ONE_MINUS_DST_COLOR", (lua_Integer) NVG_ONE_MINUS_DST_COLOR},
    {"SRC_ALPHA", (lua_Integer) NVG_SRC_ALPHA},
    {"ONE_MINUS_SRC_ALPHA", (lua_Integer) NVG_ONE_MINUS_SRC_ALPHA},
    {"DST_ALPHA", (lua_Integer) NVG_DST_ALPHA},
    {"ONE_MINUS_DST_ALPHA", (lua_Integer) NVG_ONE_MINUS_DST_ALPHA},
    {"SRC_ALPHA_SATURATE", (lua_Integer) NVG_SRC_ALPHA_SATURATE},
    {NULL, (lua_Integer) 0},
};

static XL_EnumEntry r_nvg_compositeoperation_enum_xl[] = {
    {"SOURCE_OVER", (lua_Integer) NVG_SOURCE_OVER},
    {"SOURCE_IN", (lua_Integer) NVG_SOURCE_IN},
    {"SOURCE_OUT", (lua_Integer) NVG_SOURCE_OUT},
    {"ATOP", (lua_Integer) NVG_ATOP},
    {"DESTINATION_OVER", (lua_Integer) NVG_DESTINATION_OVER},
    {"DESTINATION_IN", (lua_Integer) NVG_DESTINATION_IN},
    {"DESTINATION_OUT", (lua_Integer) NVG_DESTINATION_OUT},
    {"DESTINATION_ATOP", (lua_Integer) NVG_DESTINATION_ATOP},
    {"LIGHTER", (lua_Integer) NVG_LIGHTER},
    {"COPY", (lua_Integer) NVG_COPY},
    {"XOR", (lua_Integer) NVG_XOR},
    {NULL, (lua_Integer) 0},
};

int R_lua_nvg_init(lua_State *L)
{
    XL_initmetatable(L, "NVGcolor", nvgcolor_method_registry_xl);
    XL_initmetatable(L, "NVGpaint", NULL);
    XL_initmetatable(L, "R_LuaNvgTransform", r_luanvgtransform_method_registry_xl);
    XL_initmetatable(L, "R_Nvg", r_nvg_method_registry_xl);
    XL_initindextable(L, &nvgcolor_index_dummy_xl, nvgcolor_index_registry_xl);
    XL_initnewindextable(L, &nvgcolor_newindex_dummy_xl, nvgcolor_newindex_registry_xl);
    XL_initfunctions(L, r_nvg_function_registry_xl, "R", "Nvg", (const char *)NULL);
    XL_initenum(L, r_nvg_winding_enum_xl, "R", "Nvg", "Winding", (const char *)NULL);
    XL_initenum(L, r_nvg_solidity_enum_xl, "R", "Nvg", "Solidity", (const char *)NULL);
    XL_initenum(L, r_nvg_linecap_enum_xl, "R", "Nvg", "LineCap", (const char *)NULL);
    XL_initenum(L, r_nvg_align_enum_xl, "R", "Nvg", "Align", (const char *)NULL);
    XL_initenum(L, r_nvg_blendfactor_enum_xl, "R", "Nvg", "BlendFactor", (const char *)NULL);
    XL_initenum(L, r_nvg_compositeoperation_enum_xl, "R", "Nvg", "CompositeOperation", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
