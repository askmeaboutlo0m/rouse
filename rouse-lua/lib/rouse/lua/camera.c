/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2021 askmeaboutloom
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


static int r_camera_proj_index_xl(lua_State *L)
{
    R_Camera *self = R_CPPCAST(R_Camera *, XL_checkpptype(L, 1, "R_Camera"));
    R_M4 RETVAL;
    RETVAL = self->proj;
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_camera_proj_newindex_xl(lua_State *L)
{
    R_Camera *self = R_CPPCAST(R_Camera *, XL_checkpptype(L, 1, "R_Camera"));
    R_M4 VALUE = *((R_M4 *)luaL_checkudata(L, 2, "R_M4"));
    self->proj = VALUE;
    return 0;
}

static int r_camera_view_index_xl(lua_State *L)
{
    R_Camera *self = R_CPPCAST(R_Camera *, XL_checkpptype(L, 1, "R_Camera"));
    R_M4 RETVAL;
    RETVAL = self->view;
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_camera_view_newindex_xl(lua_State *L)
{
    R_Camera *self = R_CPPCAST(R_Camera *, XL_checkpptype(L, 1, "R_Camera"));
    R_M4 VALUE = *((R_M4 *)luaL_checkudata(L, 2, "R_M4"));
    self->view = VALUE;
    return 0;
}

static int r_camera_new_xl(lua_State *L)
{
    R_Camera *RETVAL;
    RETVAL = R_camera_new();
    XL_pushnewpptypeuv(L, RETVAL, "R_Camera", 0);
    return 1;
}

static int r_camera_new_perspective_xl(lua_State *L)
{
    float fov = XL_checkfloat(L, 1);
    float aspect_ratio = XL_checkfloat(L, 2);
    float near = XL_checkfloat(L, 3);
    float far = XL_checkfloat(L, 4);
    R_Camera *RETVAL;
    RETVAL = R_camera_new_perspective(fov, aspect_ratio, near, far);
    XL_pushnewpptypeuv(L, RETVAL, "R_Camera", 0);
    return 1;
}

static int r_camera_method_gc_xl(lua_State *L)
{
    R_Camera *self = R_CPPCAST(R_Camera *, XL_checkpptype_nullable(L, 1, "R_Camera"));
    R_camera_free(self);
    return 0;
}

static int r_camera_method_set_perspective_xl(lua_State *L)
{
    R_Camera *self = R_CPPCAST(R_Camera *, XL_checkpptype(L, 1, "R_Camera"));
    float fov = XL_checkfloat(L, 2);
    float aspect_ratio = XL_checkfloat(L, 3);
    float near = XL_checkfloat(L, 4);
    float far = XL_checkfloat(L, 5);
    R_camera_perspective_set(self, fov, aspect_ratio, near, far);
    return 0;
}

static int r_camera_method_bind_xl(lua_State *L)
{
    R_Camera *self = R_CPPCAST(R_Camera *, XL_checkpptype(L, 1, "R_Camera"));
    int proj_location = XL_checkint(L, 2);
    int view_location = XL_checkint(L, 3);
    R_camera_bind(self, proj_location, view_location);
    return 0;
}

static int r_firstperson_pos_index_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    R_V3 RETVAL;
    RETVAL = self->pos;
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V3), "R_V3", 0);
    return 1;
}

static int r_firstperson_pos_newindex_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    R_V3 VALUE = *((R_V3 *)luaL_checkudata(L, 2, "R_V3"));
    self->pos = VALUE;
    return 0;
}

static int r_firstperson_h_index_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    float RETVAL;
    RETVAL = self->h;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_firstperson_h_newindex_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    float VALUE = XL_checkfloat(L, 2);
    self->h = VALUE;
    return 0;
}

static int r_firstperson_v_index_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    float RETVAL;
    RETVAL = self->v;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_firstperson_v_newindex_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    float VALUE = XL_checkfloat(L, 2);
    self->v = VALUE;
    return 0;
}

static int r_firstperson_new_xl(lua_State *L)
{
    R_V3 pos = *((R_V3 *)luaL_checkudata(L, 1, "R_V3"));
    float h = XL_checkfloat(L, 2);
    float v = XL_checkfloat(L, 3);
    R_FirstPerson *RETVAL;
    RETVAL = R_first_person_new(pos, h, v);
    XL_pushnewpptypeuv(L, RETVAL, "R_FirstPerson", 0);
    return 1;
}

static int r_firstperson_method_gc_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype_nullable(L, 1, "R_FirstPerson"));
    R_first_person_free(self);
    return 0;
}

static int r_firstperson_directions_index_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    R_V3 forward, right, up;
    R_first_person_directions(self, &forward, &right, &up);
    lua_createtable(L, 0, 3);
    XL_pushnewutypeuv(L, &forward, sizeof(R_V3), "R_V3", 0);
    lua_setfield(L, -1, "forward");
    XL_pushnewutypeuv(L, &right, sizeof(R_V3), "R_V3", 0);
    lua_setfield(L, -1, "right");
    XL_pushnewutypeuv(L, &up, sizeof(R_V3), "R_V3", 0);
    lua_setfield(L, -1, "up");
    return 1;
}

static int r_firstperson_method_look_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    float hd = XL_checkfloat(L, 2);
    float vd = XL_checkfloat(L, 3);
    R_first_person_look(self, hd, vd);
    return 0;
}

static int r_firstperson_method_move_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    float forward = XL_checkfloat(L, 2);
    float right = XL_checkfloat(L, 3);
    float up = XL_checkfloat(L, 4);
    R_first_person_move(self, forward, right, up);
    return 0;
}

static int r_firstperson_method_apply_xl(lua_State *L)
{
    R_FirstPerson *self = R_CPPCAST(R_FirstPerson *, XL_checkpptype(L, 1, "R_FirstPerson"));
    R_Camera *camera = R_CPPCAST(R_Camera *, XL_checkpptype(L, 2, "R_Camera"));
    R_first_person_apply(self, camera);
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

static int r_camera_index_anchor_xl;
static int r_camera_index_xl(lua_State *L)
{
    return XL_index(L, "R_Camera", &r_camera_index_anchor_xl, 1, 2);
}

static int r_firstperson_index_anchor_xl;
static int r_firstperson_index_xl(lua_State *L)
{
    return XL_index(L, "R_FirstPerson", &r_firstperson_index_anchor_xl, 1, 2);
}

int r_camera_newindex_anchor_xl;
static int r_camera_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_Camera", &r_camera_newindex_anchor_xl, 1, 2, 3);
}

int r_firstperson_newindex_anchor_xl;
static int r_firstperson_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_FirstPerson", &r_firstperson_newindex_anchor_xl, 1, 2, 3);
}

static luaL_Reg r_camera_function_registry_xl[] = {
    {"new", r_camera_new_xl},
    {"new_perspective", r_camera_new_perspective_xl},
    {NULL, NULL},
};

static luaL_Reg r_firstperson_function_registry_xl[] = {
    {"new", r_firstperson_new_xl},
    {NULL, NULL},
};

static luaL_Reg r_camera_index_registry_xl[] = {
    {"proj", r_camera_proj_index_xl},
    {"view", r_camera_view_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_firstperson_index_registry_xl[] = {
    {"directions", r_firstperson_directions_index_xl},
    {"h", r_firstperson_h_index_xl},
    {"pos", r_firstperson_pos_index_xl},
    {"v", r_firstperson_v_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_camera_method_registry_xl[] = {
    {"__gc", r_camera_method_gc_xl},
    {"__index", r_camera_index_xl},
    {"__newindex", r_camera_newindex_xl},
    {"bind", r_camera_method_bind_xl},
    {"set_perspective", r_camera_method_set_perspective_xl},
    {NULL, NULL},
};

static luaL_Reg r_firstperson_method_registry_xl[] = {
    {"__gc", r_firstperson_method_gc_xl},
    {"__index", r_firstperson_index_xl},
    {"__newindex", r_firstperson_newindex_xl},
    {"apply", r_firstperson_method_apply_xl},
    {"look", r_firstperson_method_look_xl},
    {"move", r_firstperson_method_move_xl},
    {NULL, NULL},
};

static luaL_Reg r_camera_newindex_registry_xl[] = {
    {"proj", r_camera_proj_newindex_xl},
    {"view", r_camera_view_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_firstperson_newindex_registry_xl[] = {
    {"h", r_firstperson_h_newindex_xl},
    {"pos", r_firstperson_pos_newindex_xl},
    {"v", r_firstperson_v_newindex_xl},
    {NULL, NULL},
};

int R_lua_camera_init(lua_State *L)
{
    XL_initmetatable(L, "R_Camera", r_camera_method_registry_xl);
    XL_initmetatable(L, "R_FirstPerson", r_firstperson_method_registry_xl);
    XL_initindextable(L, &r_camera_index_anchor_xl, r_camera_index_registry_xl);
    XL_initindextable(L, &r_firstperson_index_anchor_xl, r_firstperson_index_registry_xl);
    XL_initnewindextable(L, &r_camera_newindex_anchor_xl, r_camera_newindex_registry_xl);
    XL_initnewindextable(L, &r_firstperson_newindex_anchor_xl, r_firstperson_newindex_registry_xl);
    XL_initfunctions(L, r_camera_function_registry_xl, "R", "Camera", (const char *)NULL);
    XL_initfunctions(L, r_firstperson_function_registry_xl, "R", "FirstPerson", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
