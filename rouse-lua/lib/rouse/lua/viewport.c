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


static int r_viewport_new_xl(lua_State *L)
{
    int x = XL_checkint(L, 1);
    int y = XL_checkint(L, 2);
    int w = XL_checkint(L, 3);
    int h = XL_checkint(L, 4);
    R_Viewport RETVAL;
    RETVAL = (R_Viewport){x, y, w, h};
    XL_pushnewutype(L, &RETVAL, sizeof(R_Viewport), "R_Viewport");
    return 1;
}

static int r_viewport_x_index_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int RETVAL;
    RETVAL = self->x;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_viewport_x_newindex_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int VALUE = XL_checkint(L, 2);
    self->x = VALUE;
    return 0;
}

static int r_viewport_y_index_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int RETVAL;
    RETVAL = self->y;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_viewport_y_newindex_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int VALUE = XL_checkint(L, 2);
    self->y = VALUE;
    return 0;
}

static int r_viewport_w_index_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int RETVAL;
    RETVAL = self->w;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_viewport_w_newindex_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int VALUE = XL_checkint(L, 2);
    self->w = VALUE;
    return 0;
}

static int r_viewport_h_index_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int RETVAL;
    RETVAL = self->h;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_viewport_h_newindex_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int VALUE = XL_checkint(L, 2);
    self->h = VALUE;
    return 0;
}

static int r_viewport_width_index_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int RETVAL;
    RETVAL = self->w;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_viewport_width_newindex_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int VALUE = XL_checkint(L, 2);
    self->w = VALUE;
    return 0;
}

static int r_viewport_height_index_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int RETVAL;
    RETVAL = self->h;
    XL_pushint(L, RETVAL);
    return 1;
}

static int r_viewport_height_newindex_xl(lua_State *L)
{
    R_Viewport *self = XL_checkutype(L, 1, "R_Viewport");
    int VALUE = XL_checkint(L, 2);
    self->h = VALUE;
    return 0;
}

static int r_viewport_method_set_xl(lua_State *L)
{
    return XL_setfromtable(L, "R_Viewport", 1, 2);
}

static int r_viewport_set_xl(lua_State *L)
{
    R_Viewport *vp = XL_checkutype(L, 1, "R_Viewport");
    XL_UNUSED(L);
    R_viewport_set(*vp);
    return 0;
}

static int r_viewport_reset_xl(lua_State *L)
{
    XL_UNUSED(L);
    R_viewport_reset();
    return 0;
}

static int r_viewport_window_xl(lua_State *L)
{
    R_Viewport RETVAL;
    XL_UNUSED(L);
    RETVAL = R_window_viewport();
    XL_pushnewutype(L, &RETVAL, sizeof(R_Viewport), "R_Viewport");
    return 1;
}

static int r_viewport_resize_window_xl(lua_State *L)
{
    XL_UNUSED(L);
    R_window_viewport_resize();
    return 0;
}

static int r_viewport_set_window_xl(lua_State *L)
{
    int width = XL_checkint(L, 1);
    int height = XL_checkint(L, 2);
    R_window_viewport_set(width, height);
    return 0;
}

static int r_viewport_index_dummy_xl;
static int r_viewport_index_xl(lua_State *L)
{
    return XL_index(L, "R_Viewport", &r_viewport_index_dummy_xl, 1, 2);
}

static int r_viewport_newindex_dummy_xl;
static int r_viewport_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_Viewport", &r_viewport_newindex_dummy_xl, 1, 2, 3);
}

static luaL_Reg r_viewport_function_registry_xl[] = {
    {"new", r_viewport_new_xl},
    {"reset", r_viewport_reset_xl},
    {"resize_window", r_viewport_resize_window_xl},
    {"set", r_viewport_set_xl},
    {"set_window", r_viewport_set_window_xl},
    {"window", r_viewport_window_xl},
    {NULL, NULL},
};

static luaL_Reg r_viewport_index_registry_xl[] = {
    {"h", r_viewport_h_index_xl},
    {"height", r_viewport_height_index_xl},
    {"w", r_viewport_w_index_xl},
    {"width", r_viewport_width_index_xl},
    {"x", r_viewport_x_index_xl},
    {"y", r_viewport_y_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_viewport_method_registry_xl[] = {
    {"__index", r_viewport_index_xl},
    {"__newindex", r_viewport_newindex_xl},
    {"set", r_viewport_method_set_xl},
    {NULL, NULL},
};

static luaL_Reg r_viewport_newindex_registry_xl[] = {
    {"h", r_viewport_h_newindex_xl},
    {"height", r_viewport_height_newindex_xl},
    {"w", r_viewport_w_newindex_xl},
    {"width", r_viewport_width_newindex_xl},
    {"x", r_viewport_x_newindex_xl},
    {"y", r_viewport_y_newindex_xl},
    {NULL, NULL},
};

int R_lua_viewport_init(lua_State *L)
{
    XL_initmetatable(L, "R_Viewport", r_viewport_method_registry_xl);
    XL_initindextable(L, &r_viewport_index_dummy_xl, r_viewport_index_registry_xl);
    XL_initnewindextable(L, &r_viewport_newindex_dummy_xl, r_viewport_newindex_registry_xl);
    XL_initfunctions(L, r_viewport_function_registry_xl, "R", "Viewport", (const char *)NULL);
    return 0;
}
