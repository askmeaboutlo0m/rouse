/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2019, 2020, 2021 askmeaboutloom
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


static float v2_length(R_V2 v2)
{
    float x = v2.x;
    float y = v2.y;
    return sqrtf(x * x + y * y);
}


static int r_v2_length_index_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    float RETVAL;
    RETVAL = v2_length(*self);
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

static int r_v2_normal_index_xl(lua_State *L)
{
    R_V2 *self = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    R_V2 RETVAL;
    float length = v2_length(*self);
    RETVAL = length == 0.0 ? R_v2(0.0, 0.0)
           : R_v2(self->x / length, self->y / length);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V2), "R_V2", 0);
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

static int r_v2_method_unm_xl(lua_State *L)
{
    R_V2 *a = R_CPPCAST(R_V2 *, XL_checkutype(L, 1, "R_V2"));
    R_V2 RETVAL;
    RETVAL = R_v2(-a->x, -a->y);
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

static int r_v4_new_xl(lua_State *L)
{
    float x = XL_checkfloat(L, 1);
    float y = XL_checkfloat(L, 2);
    float z = XL_checkfloat(L, 3);
    float w = XL_checkfloat(L, 4);
    R_V4 RETVAL;
    RETVAL = R_v4(x, y, z, w);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V4), "R_V4", 0);
    return 1;
}

static int r_v4_x_index_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float RETVAL;
    RETVAL = self->x;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v4_x_newindex_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float VALUE = XL_checkfloat(L, 2);
    self->x = VALUE;
    return 0;
}

static int r_v4_y_index_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float RETVAL;
    RETVAL = self->y;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v4_y_newindex_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float VALUE = XL_checkfloat(L, 2);
    self->y = VALUE;
    return 0;
}

static int r_v4_z_index_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float RETVAL;
    RETVAL = self->z;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v4_z_newindex_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float VALUE = XL_checkfloat(L, 2);
    self->z = VALUE;
    return 0;
}

static int r_v4_w_index_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float RETVAL;
    RETVAL = self->w;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v4_w_newindex_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float VALUE = XL_checkfloat(L, 2);
    self->w = VALUE;
    return 0;
}

static int r_v4_length_index_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    float RETVAL;
    float x = self->x;
    float y = self->y;
    float z = self->z;
    float w = self->w;
    RETVAL = sqrtf(x * x + y * y + z * z + w * w);
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_v4_method_tostring_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    lua_pushfstring(L, "R_V4(%f, %f, %f, %f)", R_lua_float2n(self->x),
                                               R_lua_float2n(self->y),
                                               R_lua_float2n(self->z),
                                               R_lua_float2n(self->w));
    return 1;
}

static int r_v4_method_unpack_xl(lua_State *L)
{
    R_V4 *self = R_CPPCAST(R_V4 *, XL_checkutype(L, 1, "R_V4"));
    XL_pushfloat(L, self->x);
    XL_pushfloat(L, self->y);
    XL_pushfloat(L, self->z);
    XL_pushfloat(L, self->w);
    return 4;
}

static int r_v4_method_set_xl(lua_State *L)
{
    return XL_setfromtable(L, "R_V4", 1, 2);
}

static int r_m4_identity_xl(lua_State *L)
{
    R_M4 RETVAL;
    RETVAL = R_m4_identity();
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_m00_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m00;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m00_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m00 = VALUE;
    return 0;
}

static int r_m4_m01_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m01;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m01_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m01 = VALUE;
    return 0;
}

static int r_m4_m02_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m02;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m02_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m02 = VALUE;
    return 0;
}

static int r_m4_m03_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m03;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m03_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m03 = VALUE;
    return 0;
}

static int r_m4_m10_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m10;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m10_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m10 = VALUE;
    return 0;
}

static int r_m4_m11_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m11;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m11_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m11 = VALUE;
    return 0;
}

static int r_m4_m12_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m12;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m12_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m12 = VALUE;
    return 0;
}

static int r_m4_m13_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m13;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m13_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m13 = VALUE;
    return 0;
}

static int r_m4_m20_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m20;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m20_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m20 = VALUE;
    return 0;
}

static int r_m4_m21_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m21;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m21_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m21 = VALUE;
    return 0;
}

static int r_m4_m22_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m22;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m22_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m22 = VALUE;
    return 0;
}

static int r_m4_m23_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m23;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m23_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m23 = VALUE;
    return 0;
}

static int r_m4_m30_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m30;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m30_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m30 = VALUE;
    return 0;
}

static int r_m4_m31_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m31;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m31_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m31 = VALUE;
    return 0;
}

static int r_m4_m32_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m32;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m32_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m32 = VALUE;
    return 0;
}

static int r_m4_m33_index_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float RETVAL;
    RETVAL = self->m33;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_m4_m33_newindex_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float VALUE = XL_checkfloat(L, 2);
    self->m33 = VALUE;
    return 0;
}

static int r_m4_method_tostring_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    lua_pushfstring(L, "R_M4(%f, %f, %f, %f, %f, %f, %f, %f, "
                            "%f, %f, %f, %f, %f, %f, %f, %f)",
                    R_lua_float2n(self->m00), R_lua_float2n(self->m01),
                    R_lua_float2n(self->m02), R_lua_float2n(self->m03),
                    R_lua_float2n(self->m10), R_lua_float2n(self->m11),
                    R_lua_float2n(self->m12), R_lua_float2n(self->m13),
                    R_lua_float2n(self->m20), R_lua_float2n(self->m21),
                    R_lua_float2n(self->m22), R_lua_float2n(self->m23),
                    R_lua_float2n(self->m30), R_lua_float2n(self->m31),
                    R_lua_float2n(self->m32), R_lua_float2n(self->m33));
    return 1;
}

static int r_m4_method_unpack_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    XL_pushfloat(L, self->m00);
    XL_pushfloat(L, self->m01);
    XL_pushfloat(L, self->m02);
    XL_pushfloat(L, self->m03);
    XL_pushfloat(L, self->m10);
    XL_pushfloat(L, self->m11);
    XL_pushfloat(L, self->m12);
    XL_pushfloat(L, self->m13);
    XL_pushfloat(L, self->m20);
    XL_pushfloat(L, self->m21);
    XL_pushfloat(L, self->m22);
    XL_pushfloat(L, self->m23);
    XL_pushfloat(L, self->m30);
    XL_pushfloat(L, self->m31);
    XL_pushfloat(L, self->m32);
    XL_pushfloat(L, self->m33);
    return 16;
}

static int r_m4_method_mul_xl(lua_State *L)
{
    R_M4 a = *((R_M4 *)luaL_checkudata(L, 1, "R_M4"));
    R_M4 b = *((R_M4 *)luaL_checkudata(L, 2, "R_M4"));
    R_M4 RETVAL;
    RETVAL = R_m4_mul(a, b);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_rotate_x_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float angle = XL_checkfloat(L, 2);
    R_M4 RETVAL;
    RETVAL = R_m4_rotate_x(*self, angle);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_rotate_y_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float angle = XL_checkfloat(L, 2);
    R_M4 RETVAL;
    RETVAL = R_m4_rotate_y(*self, angle);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_rotate_z_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float angle = XL_checkfloat(L, 2);
    R_M4 RETVAL;
    RETVAL = R_m4_rotate_z(*self, angle);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_rotate_xyz_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    R_M4 RETVAL;
    int argc = lua_gettop(L);
    if (argc == 2) {
        R_V3 *pa = R_CPPCAST(R_V3 *, XL_checkutype(L, 2, "R_V3"));
        RETVAL   = R_m4_rotate_xyz(*self, *pa);
    }
    else if (argc == 4) {
        R_V3 a;
        a.x    = XL_checkfloat(L, 2);
        a.y    = XL_checkfloat(L, 3);
        a.z    = XL_checkfloat(L, 4);
        RETVAL = R_m4_rotate_xyz(*self, a);
    }
    else {
        R_LUA_DIE(L, "Expected 1 or 3 arguments, got %d", argc);
    }
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_scale_x_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float scale = XL_checkfloat(L, 2);
    R_M4 RETVAL;
    RETVAL = R_m4_scale_x(*self, scale);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_scale_y_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float scale = XL_checkfloat(L, 2);
    R_M4 RETVAL;
    RETVAL = R_m4_scale_y(*self, scale);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_scale_z_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    float scale = XL_checkfloat(L, 2);
    R_M4 RETVAL;
    RETVAL = R_m4_scale_z(*self, scale);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

static int r_m4_method_scale_xyz_xl(lua_State *L)
{
    R_M4 *self = R_CPPCAST(R_M4 *, XL_checkutype(L, 1, "R_M4"));
    R_M4 RETVAL;
    int argc = lua_gettop(L);
    if (argc == 2) {
        R_V3 *ps = R_CPPCAST(R_V3 *, XL_checkutype(L, 2, "R_V3"));
        RETVAL   = R_m4_scale_xyz(*self, *ps);
    }
    else if (argc == 4) {
        R_V3 s;
        s.x    = XL_checkfloat(L, 2);
        s.y    = XL_checkfloat(L, 3);
        s.z    = XL_checkfloat(L, 4);
        RETVAL = R_m4_scale_xyz(*self, s);
    }
    else {
        R_LUA_DIE(L, "Expected 1 or 3 arguments, got %d", argc);
    }
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_M4), "R_M4", 0);
    return 1;
}

#ifdef __cplusplus
extern "C" {
#endif

static int r_m4_index_anchor_xl;
static int r_m4_index_xl(lua_State *L)
{
    return XL_index(L, "R_M4", &r_m4_index_anchor_xl, 1, 2);
}

static int r_v2_index_anchor_xl;
static int r_v2_index_xl(lua_State *L)
{
    return XL_index(L, "R_V2", &r_v2_index_anchor_xl, 1, 2);
}

static int r_v3_index_anchor_xl;
static int r_v3_index_xl(lua_State *L)
{
    return XL_index(L, "R_V3", &r_v3_index_anchor_xl, 1, 2);
}

static int r_v4_index_anchor_xl;
static int r_v4_index_xl(lua_State *L)
{
    return XL_index(L, "R_V4", &r_v4_index_anchor_xl, 1, 2);
}

int r_m4_newindex_anchor_xl;
static int r_m4_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_M4", &r_m4_newindex_anchor_xl, 1, 2, 3);
}

int r_v2_newindex_anchor_xl;
static int r_v2_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_V2", &r_v2_newindex_anchor_xl, 1, 2, 3);
}

int r_v3_newindex_anchor_xl;
static int r_v3_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_V3", &r_v3_newindex_anchor_xl, 1, 2, 3);
}

int r_v4_newindex_anchor_xl;
static int r_v4_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_V4", &r_v4_newindex_anchor_xl, 1, 2, 3);
}

static luaL_Reg r_m4_function_registry_xl[] = {
    {"identity", r_m4_identity_xl},
    {NULL, NULL},
};

static luaL_Reg r_v2_function_registry_xl[] = {
    {"new", r_v2_new_xl},
    {"polar", r_v2_polar_xl},
    {NULL, NULL},
};

static luaL_Reg r_v3_function_registry_xl[] = {
    {"new", r_v3_new_xl},
    {NULL, NULL},
};

static luaL_Reg r_v4_function_registry_xl[] = {
    {"new", r_v4_new_xl},
    {NULL, NULL},
};

static luaL_Reg r_m4_index_registry_xl[] = {
    {"m00", r_m4_m00_index_xl},
    {"m01", r_m4_m01_index_xl},
    {"m02", r_m4_m02_index_xl},
    {"m03", r_m4_m03_index_xl},
    {"m10", r_m4_m10_index_xl},
    {"m11", r_m4_m11_index_xl},
    {"m12", r_m4_m12_index_xl},
    {"m13", r_m4_m13_index_xl},
    {"m20", r_m4_m20_index_xl},
    {"m21", r_m4_m21_index_xl},
    {"m22", r_m4_m22_index_xl},
    {"m23", r_m4_m23_index_xl},
    {"m30", r_m4_m30_index_xl},
    {"m31", r_m4_m31_index_xl},
    {"m32", r_m4_m32_index_xl},
    {"m33", r_m4_m33_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_v2_index_registry_xl[] = {
    {"angle", r_v2_angle_index_xl},
    {"length", r_v2_length_index_xl},
    {"normal", r_v2_normal_index_xl},
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

static luaL_Reg r_v4_index_registry_xl[] = {
    {"length", r_v4_length_index_xl},
    {"w", r_v4_w_index_xl},
    {"x", r_v4_x_index_xl},
    {"y", r_v4_y_index_xl},
    {"z", r_v4_z_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_m4_method_registry_xl[] = {
    {"__index", r_m4_index_xl},
    {"__mul", r_m4_method_mul_xl},
    {"__newindex", r_m4_newindex_xl},
    {"__tostring", r_m4_method_tostring_xl},
    {"rotate_x", r_m4_method_rotate_x_xl},
    {"rotate_xyz", r_m4_method_rotate_xyz_xl},
    {"rotate_y", r_m4_method_rotate_y_xl},
    {"rotate_z", r_m4_method_rotate_z_xl},
    {"scale_x", r_m4_method_scale_x_xl},
    {"scale_xyz", r_m4_method_scale_xyz_xl},
    {"scale_y", r_m4_method_scale_y_xl},
    {"scale_z", r_m4_method_scale_z_xl},
    {"unpack", r_m4_method_unpack_xl},
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
    {"__unm", r_v2_method_unm_xl},
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

static luaL_Reg r_v4_method_registry_xl[] = {
    {"__index", r_v4_index_xl},
    {"__newindex", r_v4_newindex_xl},
    {"__tostring", r_v4_method_tostring_xl},
    {"set", r_v4_method_set_xl},
    {"unpack", r_v4_method_unpack_xl},
    {NULL, NULL},
};

static luaL_Reg r_m4_newindex_registry_xl[] = {
    {"m00", r_m4_m00_newindex_xl},
    {"m01", r_m4_m01_newindex_xl},
    {"m02", r_m4_m02_newindex_xl},
    {"m03", r_m4_m03_newindex_xl},
    {"m10", r_m4_m10_newindex_xl},
    {"m11", r_m4_m11_newindex_xl},
    {"m12", r_m4_m12_newindex_xl},
    {"m13", r_m4_m13_newindex_xl},
    {"m20", r_m4_m20_newindex_xl},
    {"m21", r_m4_m21_newindex_xl},
    {"m22", r_m4_m22_newindex_xl},
    {"m23", r_m4_m23_newindex_xl},
    {"m30", r_m4_m30_newindex_xl},
    {"m31", r_m4_m31_newindex_xl},
    {"m32", r_m4_m32_newindex_xl},
    {"m33", r_m4_m33_newindex_xl},
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

static luaL_Reg r_v4_newindex_registry_xl[] = {
    {"w", r_v4_w_newindex_xl},
    {"x", r_v4_x_newindex_xl},
    {"y", r_v4_y_newindex_xl},
    {"z", r_v4_z_newindex_xl},
    {NULL, NULL},
};

int R_lua_geom_init(lua_State *L)
{
    XL_initmetatable(L, "R_M4", r_m4_method_registry_xl);
    XL_initmetatable(L, "R_V2", r_v2_method_registry_xl);
    XL_initmetatable(L, "R_V3", r_v3_method_registry_xl);
    XL_initmetatable(L, "R_V4", r_v4_method_registry_xl);
    XL_initindextable(L, &r_m4_index_anchor_xl, r_m4_index_registry_xl);
    XL_initindextable(L, &r_v2_index_anchor_xl, r_v2_index_registry_xl);
    XL_initindextable(L, &r_v3_index_anchor_xl, r_v3_index_registry_xl);
    XL_initindextable(L, &r_v4_index_anchor_xl, r_v4_index_registry_xl);
    XL_initnewindextable(L, &r_m4_newindex_anchor_xl, r_m4_newindex_registry_xl);
    XL_initnewindextable(L, &r_v2_newindex_anchor_xl, r_v2_newindex_registry_xl);
    XL_initnewindextable(L, &r_v3_newindex_anchor_xl, r_v3_newindex_registry_xl);
    XL_initnewindextable(L, &r_v4_newindex_anchor_xl, r_v4_newindex_registry_xl);
    XL_initfunctions(L, r_m4_function_registry_xl, "R", "M4", (const char *)NULL);
    XL_initfunctions(L, r_v2_function_registry_xl, "R", "V2", (const char *)NULL);
    XL_initfunctions(L, r_v3_function_registry_xl, "R", "V3", (const char *)NULL);
    XL_initfunctions(L, r_v4_function_registry_xl, "R", "V4", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
