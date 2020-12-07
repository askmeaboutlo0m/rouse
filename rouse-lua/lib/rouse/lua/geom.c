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


static int r_v2_new_xl(lua_State *L)
{
    float x = XL_checkfloat(L, 1);
    float y = XL_checkfloat(L, 2);
    R_V2 RETVAL;
    RETVAL = R_v2(x, y);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int r_v2_polar_xl(lua_State *L)
{
    float len = XL_checkfloat(L, 1);
    float angle = XL_checkfloat(L, 2);
    R_V2 RETVAL;
    RETVAL = R_v2_polar(len, angle);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int r_v2_x_index_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    float RETVAL;
    RETVAL = self->x;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v2_x_newindex_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    float VALUE = XL_checkfloat(L, 2);
    self->x = VALUE;
    return 0;
}

static int r_v2_y_index_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    float RETVAL;
    RETVAL = self->y;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v2_y_newindex_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    float VALUE = XL_checkfloat(L, 2);
    self->y = VALUE;
    return 0;
}

static int r_v2_length_index_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    float RETVAL;
    float x = self->x;
    float y = self->y;
    RETVAL = sqrtf(x * x + y * y);
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v2_angle_index_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    int RETVAL;
    float x = self->x;
    float y = self->y;
    if (x == 0.0f && y == 0.0f) {
        lua_pushnil(L);
    }
    else {
        XL_pushfloat(L, atan2f(y, x));
    }
    return 1;
    lua_pushvalue(L, RETVAL);
    return 1;
}

static int r_v2_method_tostring_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    lua_pushfstring(L, "R_V2(%f, %f)", R_lua_float2n(self->x),
                                       R_lua_float2n(self->y));
    return 1;
}

static int r_v2_method_unpack_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    XL_pushfloat(L, self->x);
    XL_pushfloat(L, self->y);
    return 2;
}


#define TYPE_BAD   0
#define TYPE_V2    (1 << 0)
#define TYPE_FLOAT (1 << 1)

#define TYPES_V2_V2    (TYPE_V2    | (TYPE_V2    << 2))
#define TYPES_V2_FLOAT (TYPE_V2    | (TYPE_FLOAT << 2))
#define TYPES_FLOAT_V2 (TYPE_FLOAT | (TYPE_V2    << 2))

#define DECONSTRUCT_V2_TYPE(L, ARG, T, V, F) \
    V = luaL_testudata(L, ARG, "R_V2"); \
    if (V) { \
        T = TYPE_V2; \
    } \
    else { \
        int _success; \
        lua_Number _number = lua_tonumberx(L, ARG, &_success); \
        if (_success) { \
            T = TYPE_FLOAT; \
            F = R_lua_n2float(_number); \
        } \
        else { \
            T = TYPE_BAD; \
        } \
    }

#define DECONSTRUCT_V2_TYPES(L, A, B) \
    int    t1,  t2; \
    R_V2  *v1, *v2; \
    float  f1,  f2; \
    DECONSTRUCT_V2_TYPE(L, A, t1, v1, f1); \
    DECONSTRUCT_V2_TYPE(L, B, t2, v2, f2); \
    int types = t1 | (t2 << 2)

#define DIE_WITH_BAD_V2_TYPES(L, OP, A, B) \
    R_LUA_DIE(L, "R_V2 operation (%s " OP " %s) not applicable", \
              lua_typename(L, lua_type(L, A)), lua_typename(L, lua_type(L, B)))


static int r_v2_method_add_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int a = 1;
    luaL_checkany(L, 2);
    int b = 2;
    R_V2 RETVAL;
    DECONSTRUCT_V2_TYPES(L, a, b);
    switch (types) {
        case TYPES_V2_V2:
            RETVAL = R_v2_add(*v1, *v2);
            break;
        case TYPES_V2_FLOAT:
            RETVAL = R_v2_adds(*v1, f2);
            break;
        case TYPES_FLOAT_V2:
            RETVAL = R_v2_adds(*v2, f1);
            break;
        default:
            DIE_WITH_BAD_V2_TYPES(L, "+", a, b);
    }
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int r_v2_method_sub_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int a = 1;
    luaL_checkany(L, 2);
    int b = 2;
    R_V2 RETVAL;
    DECONSTRUCT_V2_TYPES(L, a, b);
    XL_UNUSED(f1);
    switch (types) {
        case TYPES_V2_V2:
            RETVAL = R_v2_sub(*v1, *v2);
            break;
        case TYPES_V2_FLOAT:
            RETVAL = R_v2_subs(*v1, f2);
            break;
        default:
            DIE_WITH_BAD_V2_TYPES(L, "-", a, b);
    }
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int r_v2_method_mul_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int a = 1;
    luaL_checkany(L, 2);
    int b = 2;
    R_V2 RETVAL;
    DECONSTRUCT_V2_TYPES(L, a, b);
    switch (types) {
        case TYPES_V2_FLOAT:
            RETVAL = R_v2_scale(*v1, f2);
            break;
        case TYPES_FLOAT_V2:
            RETVAL = R_v2_scale(*v2, f1);
            break;
        default:
            DIE_WITH_BAD_V2_TYPES(L, "*", a, b);
    }
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int r_v2_method_div_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int a = 1;
    luaL_checkany(L, 2);
    int b = 2;
    R_V2 RETVAL;
    DECONSTRUCT_V2_TYPES(L, a, b);
    (void) f1;
    switch (types) {
        case TYPES_V2_FLOAT:
            RETVAL = R_v2_scale(*v1, 1.0f / f2);
            break;
        default:
            DIE_WITH_BAD_V2_TYPES(L, "*", a, b);
    }
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
    return 1;
}

static int r_v2_method_distance2_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    R_V2 *b = R_CPPCAST(R_V2 *, XL_checkutype(L, 2, "R_V2"));
    float RETVAL;
    RETVAL = R_v2_distance2(*self, *b);
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v2_method_distance_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    R_V2 *b = R_CPPCAST(R_V2 *, XL_checkutype(L, 2, "R_V2"));
    float RETVAL;
    RETVAL = R_v2_distance(*self, *b);
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v2_method_set_xl(lua_State *L)
{
    return XL_setfromtable(L, "R_V2", 1, 2);
}

static int r_v3_new_xl(lua_State *L)
{
    float x = XL_checkfloat(L, 1);
    float y = XL_checkfloat(L, 2);
    float z = XL_checkfloat(L, 3);
    R_V3 RETVAL;
    RETVAL = R_v3(x, y, z);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V3), "R_V3", 0);
    return 1;
}

static int r_v3_x_index_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    float RETVAL;
    RETVAL = self->x;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v3_x_newindex_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    float VALUE = XL_checkfloat(L, 2);
    self->x = VALUE;
    return 0;
}

static int r_v3_y_index_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    float RETVAL;
    RETVAL = self->y;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v3_y_newindex_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    float VALUE = XL_checkfloat(L, 2);
    self->y = VALUE;
    return 0;
}

static int r_v3_z_index_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    float RETVAL;
    RETVAL = self->z;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v3_z_newindex_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    float VALUE = XL_checkfloat(L, 2);
    self->z = VALUE;
    return 0;
}

static int r_v3_length_index_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    float RETVAL;
    float x = self->x;
    float y = self->y;
    float z = self->z;
    RETVAL = sqrtf(x * x + y * y + z * z);
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v3_method_tostring_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    lua_pushfstring(L, "R_V3(%f, %f, %f)", R_lua_float2n(self->x),
                                           R_lua_float2n(self->y),
                                           R_lua_float2n(self->z));
    return 1;
}

static int r_v3_method_unpack_xl(lua_State *L)
{
    R_V3 *self = R_CPPCAST(R_V3 *, XL_checkutype(L, 1, "R_V3"));
    XL_pushfloat(L, self->x);
    XL_pushfloat(L, self->y);
    XL_pushfloat(L, self->z);
    return 3;
}

static int r_v3_method_set_xl(lua_State *L)
{
    return XL_setfromtable(L, "R_V3", 1, 2);
}

#ifdef __cplusplus
extern "C" {
#endif

static int r_v2_index_dummy_xl;
static int r_v2_index_xl(lua_State *L)
{
    return XL_index(L, "R_V2", &r_v2_index_dummy_xl, 1, 2);
}

static int r_v3_index_dummy_xl;
static int r_v3_index_xl(lua_State *L)
{
    return XL_index(L, "R_V3", &r_v3_index_dummy_xl, 1, 2);
}

static int r_v2_newindex_dummy_xl;
static int r_v2_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_V2", &r_v2_newindex_dummy_xl, 1, 2, 3);
}

static int r_v3_newindex_dummy_xl;
static int r_v3_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_V3", &r_v3_newindex_dummy_xl, 1, 2, 3);
}

static luaL_Reg r_v2_function_registry_xl[] = {
    {"new", r_v2_new_xl},
    {"polar", r_v2_polar_xl},
    {NULL, NULL},
};

static luaL_Reg r_v3_function_registry_xl[] = {
    {"new", r_v3_new_xl},
    {NULL, NULL},
};

static luaL_Reg r_v2_index_registry_xl[] = {
    {"angle", r_v2_angle_index_xl},
    {"length", r_v2_length_index_xl},
    {"x", r_v2_x_index_xl},
    {"y", r_v2_y_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_v3_index_registry_xl[] = {
    {"length", r_v3_length_index_xl},
    {"x", r_v3_x_index_xl},
    {"y", r_v3_y_index_xl},
    {"z", r_v3_z_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_v2_method_registry_xl[] = {
    {"__add", r_v2_method_add_xl},
    {"__div", r_v2_method_div_xl},
    {"__index", r_v2_index_xl},
    {"__mul", r_v2_method_mul_xl},
    {"__newindex", r_v2_newindex_xl},
    {"__sub", r_v2_method_sub_xl},
    {"__tostring", r_v2_method_tostring_xl},
    {"distance", r_v2_method_distance_xl},
    {"distance2", r_v2_method_distance2_xl},
    {"set", r_v2_method_set_xl},
    {"unpack", r_v2_method_unpack_xl},
    {NULL, NULL},
};

static luaL_Reg r_v3_method_registry_xl[] = {
    {"__index", r_v3_index_xl},
    {"__newindex", r_v3_newindex_xl},
    {"__tostring", r_v3_method_tostring_xl},
    {"set", r_v3_method_set_xl},
    {"unpack", r_v3_method_unpack_xl},
    {NULL, NULL},
};

static luaL_Reg r_v2_newindex_registry_xl[] = {
    {"x", r_v2_x_newindex_xl},
    {"y", r_v2_y_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_v3_newindex_registry_xl[] = {
    {"x", r_v3_x_newindex_xl},
    {"y", r_v3_y_newindex_xl},
    {"z", r_v3_z_newindex_xl},
    {NULL, NULL},
};

int R_lua_geom_init(lua_State *L)
{
    XL_initmetatable(L, "R_V2", r_v2_method_registry_xl);
    XL_initmetatable(L, "R_V3", r_v3_method_registry_xl);
    XL_initindextable(L, &r_v2_index_dummy_xl, r_v2_index_registry_xl);
    XL_initindextable(L, &r_v3_index_dummy_xl, r_v3_index_registry_xl);
    XL_initnewindextable(L, &r_v2_newindex_dummy_xl, r_v2_newindex_registry_xl);
    XL_initnewindextable(L, &r_v3_newindex_dummy_xl, r_v3_newindex_registry_xl);
    XL_initfunctions(L, r_v2_function_registry_xl, "R", "V2", (const char *)NULL);
    XL_initfunctions(L, r_v3_function_registry_xl, "R", "V3", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
