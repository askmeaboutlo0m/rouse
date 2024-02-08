/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
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
#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"


static int r_bitmapimage_from_file_xl(lua_State *L)
{
    R_Nvg *nvg = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 1, "R_Nvg"));
    const char *path = luaL_checkstring(L, 2);
    int flags = XL_checkint(L, 3);
    R_BitmapImage *RETVAL;
    RETVAL = R_bitmap_image_from_file(nvg, path, flags);
    XL_pushnewpptypeuv(L, RETVAL, "R_BitmapImage", 0);
    return 1;
}

static int r_bitmapimage_method_gc_xl(lua_State *L)
{
    R_BitmapImage *self = R_CPPCAST(R_BitmapImage *, XL_checkpptype_nullable(L, 1, "R_BitmapImage"));
    R_bitmap_image_decref(self);
    return 0;
}

static int r_bitmapimage_refs_index_xl(lua_State *L)
{
    R_BitmapImage *self = R_CPPCAST(R_BitmapImage *, XL_checkpptype(L, 1, "R_BitmapImage"));
    int RETVAL;
    RETVAL = R_bitmap_image_refs(self);
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_bitmapimage_width_index_xl(lua_State *L)
{
    R_BitmapImage *self = R_CPPCAST(R_BitmapImage *, XL_checkpptype(L, 1, "R_BitmapImage"));
    int RETVAL;
    RETVAL = R_bitmap_image_width(self);
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_bitmapimage_height_index_xl(lua_State *L)
{
    R_BitmapImage *self = R_CPPCAST(R_BitmapImage *, XL_checkpptype(L, 1, "R_BitmapImage"));
    int RETVAL;
    RETVAL = R_bitmap_image_height(self);
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_bitmapimage_handle_index_xl(lua_State *L)
{
    R_BitmapImage *self = R_CPPCAST(R_BitmapImage *, XL_checkpptype(L, 1, "R_BitmapImage"));
    int RETVAL;
    RETVAL = R_bitmap_image_handle(self);
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_bitmapimage_method_draw_xl(lua_State *L)
{
    R_BitmapImage *self = R_CPPCAST(R_BitmapImage *, XL_checkpptype(L, 1, "R_BitmapImage"));
    R_Nvg *nvg = R_CPPCAST(R_Nvg *, XL_checkpptype(L, 2, "R_Nvg"));
    R_LuaNvgTransform *m = R_CPPCAST(R_LuaNvgTransform *, XL_checkutype(L, 3, "R_LuaNvgTransform"));
    R_bitmap_image_draw(self, R_nvg_context(nvg), m->matrix);
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

static int r_bitmapimage_index_anchor_xl;
static int r_bitmapimage_index_xl(lua_State *L)
{
    return XL_index(L, "R_BitmapImage", &r_bitmapimage_index_anchor_xl, 1, 2);
}

static luaL_Reg r_bitmapimage_function_registry_xl[] = {
    {"from_file", r_bitmapimage_from_file_xl},
    {NULL, NULL},
};

static luaL_Reg r_bitmapimage_index_registry_xl[] = {
    {"handle", r_bitmapimage_handle_index_xl},
    {"height", r_bitmapimage_height_index_xl},
    {"refs", r_bitmapimage_refs_index_xl},
    {"width", r_bitmapimage_width_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_bitmapimage_method_registry_xl[] = {
    {"__gc", r_bitmapimage_method_gc_xl},
    {"__index", r_bitmapimage_index_xl},
    {"draw", r_bitmapimage_method_draw_xl},
    {NULL, NULL},
};

int R_lua_bitmap_image_init(lua_State *L)
{
    XL_initmetatable(L, "R_BitmapImage", r_bitmapimage_method_registry_xl);
    XL_initindextable(L, &r_bitmapimage_index_anchor_xl, r_bitmapimage_index_registry_xl);
    XL_initfunctions(L, r_bitmapimage_function_registry_xl, "R", "BitmapImage", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
