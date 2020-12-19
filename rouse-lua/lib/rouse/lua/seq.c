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

typedef R_Step R_LuaTween;



static int protected_tween_float_or_scale_calc(lua_State *L)
{
    lua_call(L, 1, 1);
    int        success;
    lua_Number value = lua_tonumberx(L, -1, &success);
    if (success) {
        lua_pushnumber(L, value);
        return 1;
    }
    else {
        return luaL_error(L, "Can't convert tween float target to number");
    }
}

static float tween_float_calc(R_UserData user, float source)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_tween_float_or_scale_calc);
    R_lua_getreg(L, lv->reg);
    XL_pushfloat(L, source);
    if (R_lua_pcall(L, 2, 1)) {
        R_LUA_ERROR_TO_WARNING(L);
        return 0.0f;
    }
    else {
        float result = R_lua_n2float(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return result;
    }
}

static void tween_float_free(R_UserData user)
{
    R_lua_value_free(user.data);
}

static int new_tween_float_fixed(lua_State *L)
{
    float value = XL_checkfloat(L, lua_upvalueindex(1));
    R_TweenFloat tf = R_tween_float_fixed(value);
    XL_pushnewutypeuv(L, &tf, sizeof(tf), "R_TweenFloat", 0);
    return 1;
}

static int new_tween_float_between(lua_State *L)
{
    float a = XL_checkfloat(L, lua_upvalueindex(1));
    float b = XL_checkfloat(L, lua_upvalueindex(2));
    R_TweenFloat tf = R_tween_float_between(a, b);
    XL_pushnewutypeuv(L, &tf, sizeof(tf), "R_TweenFloat", 0);
    return 1;
}

static int new_tween_float_custom(lua_State *L)
{
    R_LuaValue *lv = R_lua_value_new(L, lua_upvalueindex(1));
    R_TweenFloat tf = R_tween_float(tween_float_calc, tween_float_free,
                                    NULL, R_user_data(lv));
    XL_pushnewutypeuv(L, &tf, sizeof(tf), "R_TweenFloat", 0);
    return 1;
}

static R_TweenFloat tween_float_get(lua_State *L, int index)
{
    lua_pushvalue(L, index);
    lua_call(L, 0, 1);
    return *((R_TweenFloat *)XL_checkutype(L, -1, "R_TweenFloat"));
}


static int r_tweenfloat_fixed_xl(lua_State *L)
{
    float value = XL_checkfloat(L, 1);
    XL_pushfloat(L, value);
    lua_pushcclosure(L, new_tween_float_fixed, 1);
    return 1;
}

static int r_tweenfloat_between_xl(lua_State *L)
{
    float a = XL_checkfloat(L, 1);
    float b = XL_checkfloat(L, 2);
    XL_pushfloat(L, a);
    XL_pushfloat(L, b);
    lua_pushcclosure(L, new_tween_float_between, 2);
    return 1;
}

static int r_tweenfloat_custom_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int fn = 1;
    lua_pushvalue(L, fn);
    lua_pushcclosure(L, new_tween_float_custom, 1);
    return 1;
}


static float tween_scale_calc(R_UserData user, R_V2 source)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_tween_float_or_scale_calc);
    R_lua_getreg(L, lv->reg);
    XL_pushnewutypeuv(L, &source, sizeof(source), "R_V2", 0);
    if (R_lua_pcall(L, 2, 1)) {
        R_LUA_ERROR_TO_WARNING(L);
        return 0.0f;
    }
    else {
        float result = R_lua_n2float(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return result;
    }
}

static void tween_scale_free(R_UserData user)
{
    R_lua_value_free(user.data);
}

static int new_tween_scale_fixed(lua_State *L)
{
    float value = XL_checkfloat(L, lua_upvalueindex(1));
    R_TweenScale ts = R_tween_scale_fixed(value);
    XL_pushnewutypeuv(L, &ts, sizeof(ts), "R_TweenScale", 0);
    return 1;
}

static int new_tween_scale_between(lua_State *L)
{
    float a = XL_checkfloat(L, lua_upvalueindex(1));
    float b = XL_checkfloat(L, lua_upvalueindex(2));
    R_TweenScale ts = R_tween_scale_between(a, b);
    XL_pushnewutypeuv(L, &ts, sizeof(ts), "R_TweenScale", 0);
    return 1;
}

static int new_tween_scale_custom(lua_State *L)
{
    R_LuaValue *lv = R_lua_value_new(L, lua_upvalueindex(1));
    R_TweenScale ts = R_tween_scale(tween_scale_calc, tween_scale_free,
                                    NULL, R_user_data(lv));
    XL_pushnewutypeuv(L, &ts, sizeof(ts), "R_TweenScale", 0);
    return 1;
}

static R_TweenScale tween_scale_get(lua_State *L, int index)
{
    lua_pushvalue(L, index);
    lua_call(L, 0, 1);
    return *((R_TweenScale *)XL_checkutype(L, -1, "R_TweenScale"));
}


static int r_tweenscale_fixed_xl(lua_State *L)
{
    float value = XL_checkfloat(L, 1);
    XL_pushfloat(L, value);
    lua_pushcclosure(L, new_tween_scale_fixed, 1);
    return 1;
}

static int r_tweenscale_between_xl(lua_State *L)
{
    float a = XL_checkfloat(L, 1);
    float b = XL_checkfloat(L, 2);
    XL_pushfloat(L, a);
    XL_pushfloat(L, b);
    lua_pushcclosure(L, new_tween_scale_between, 2);
    return 1;
}

static int r_tweenscale_custom_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int fn = 1;
    lua_pushvalue(L, fn);
    lua_pushcclosure(L, new_tween_scale_custom, 1);
    return 1;
}


static int protected_tween_v4_calc(lua_State *L)
{
    lua_call(L, 1, 1);
    XL_checkutype(L, -1, "R_V4");
    return 1;
}

static R_V4 tween_v4_calc(R_UserData user, R_V4 source)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_tween_v4_calc);
    R_lua_getreg(L, lv->reg);
    XL_pushnewutypeuv(L, &source, sizeof(source), "R_V4", 0);
    if (R_lua_pcall(L, 2, 1)) {
        R_LUA_ERROR_TO_WARNING(L);
        return R_v4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else {
        R_V4 result = *(R_V4 *)lua_touserdata(L, -1);
        lua_pop(L, 1);
        return result;
    }
}

static void tween_v4_free(R_UserData user)
{
    R_lua_value_free(user.data);
}

static int new_tween_v4_fixed(lua_State *L)
{
    R_V4      value = *(R_V4 *)XL_checkutype(L, lua_upvalueindex(1), "R_V4");
    R_TweenV4 ts    = R_tween_v4_fixed(value);
    XL_pushnewutypeuv(L, &ts, sizeof(ts), "R_TweenV4", 0);
    return 1;
}

static int new_tween_v4_between(lua_State *L)
{
    R_V4      a  = *(R_V4 *)XL_checkutype(L, lua_upvalueindex(1), "R_V4");
    R_V4      b  = *(R_V4 *)XL_checkutype(L, lua_upvalueindex(2), "R_V4");
    R_TweenV4 ts = R_tween_v4_between(a, b);
    XL_pushnewutypeuv(L, &ts, sizeof(ts), "R_TweenV4", 0);
    return 1;
}

static int new_tween_v4_custom(lua_State *L)
{
    R_LuaValue *lv = R_lua_value_new(L, lua_upvalueindex(1));
    R_TweenV4   ts = R_tween_v4(tween_v4_calc, tween_v4_free,
                                NULL, R_user_data(lv));
    XL_pushnewutypeuv(L, &ts, sizeof(ts), "R_TweenV4", 0);
    return 1;
}

static R_TweenV4 tween_v4_get(lua_State *L, int index)
{
    lua_pushvalue(L, index);
    lua_call(L, 0, 1);
    return *((R_TweenV4 *)XL_checkutype(L, -1, "R_TweenV4"));
}


static int r_tweenv4_fixed_xl(lua_State *L)
{
    R_V4 value = *((R_V4 *)luaL_checkudata(L, 1, "R_V4"));
    XL_pushnewutypeuv(L, &value, sizeof(value), "R_V4", 0);
    lua_pushcclosure(L, new_tween_v4_fixed, 1);
    return 1;
}

static int r_tweenv4_between_xl(lua_State *L)
{
    R_V4 a = *((R_V4 *)luaL_checkudata(L, 1, "R_V4"));
    R_V4 b = *((R_V4 *)luaL_checkudata(L, 2, "R_V4"));
    XL_pushnewutypeuv(L, &a, sizeof(a), "R_V4", 0);
    XL_pushnewutypeuv(L, &b, sizeof(b), "R_V4", 0);
    lua_pushcclosure(L, new_tween_v4_between, 2);
    return 1;
}

static int r_tweenv4_custom_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int fn = 1;
    lua_pushvalue(L, fn);
    lua_pushcclosure(L, new_tween_v4_custom, 1);
    return 1;
}

static int r_luatween_method_sprite_origin_x_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_origin_x(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_origin_y_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_origin_y(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_pos_x_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_pos_x(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_pos_y_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_pos_y(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_scale_x_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_scale_x(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_scale_y_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_scale_y(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_skew_x_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_skew_x(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_skew_y_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_skew_y(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_angle_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_angle(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_alpha_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_alpha(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_base_x_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_base_x(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_base_y_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_base_y(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_rotation_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_rotation(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_rel_x_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_rel_x(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_rel_y_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_rel_y(self, sprite, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_tint_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_tint(self, sprite, tween_v4_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_sprite_scale_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_Sprite *sprite = R_CPPCAST(R_Sprite *, XL_checkpptype(L, 2, "R_Sprite"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_sprite_scale(self, sprite, tween_scale_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_al_listener_pos_x_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int topic = 2;
    luaL_checkany(L, 3);
    int value_fn = 3;
    XL_UNUSED(topic);
    R_tween_al_listener_pos_x(self, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_listener_pos_y_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int topic = 2;
    luaL_checkany(L, 3);
    int value_fn = 3;
    XL_UNUSED(topic);
    R_tween_al_listener_pos_y(self, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_listener_pos_z_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int topic = 2;
    luaL_checkany(L, 3);
    int value_fn = 3;
    XL_UNUSED(topic);
    R_tween_al_listener_pos_z(self, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_listener_velocity_x_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int topic = 2;
    luaL_checkany(L, 3);
    int value_fn = 3;
    XL_UNUSED(topic);
    R_tween_al_listener_velocity_x(self, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_listener_velocity_y_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int topic = 2;
    luaL_checkany(L, 3);
    int value_fn = 3;
    XL_UNUSED(topic);
    R_tween_al_listener_velocity_y(self, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_listener_velocity_z_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int topic = 2;
    luaL_checkany(L, 3);
    int value_fn = 3;
    XL_UNUSED(topic);
    R_tween_al_listener_velocity_z(self, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_listener_gain_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int topic = 2;
    luaL_checkany(L, 3);
    int value_fn = 3;
    XL_UNUSED(topic);
    R_tween_al_listener_gain(self, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_pos_x_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_pos_x(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_pos_y_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_pos_y(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_pos_z_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_pos_z(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_velocity_x_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_velocity_x(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_velocity_y_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_velocity_y(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_velocity_z_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_velocity_z(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_gain_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_gain(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_pitch_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_pitch(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_luatween_method_al_source_reference_distance_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    R_AlSource *source = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 2, "R_AlSource"));
    luaL_checkany(L, 3);
    int value_fn = 3;
    R_tween_al_source_reference_distance(self, source, tween_float_get(L, value_fn));
    lua_settop(L, 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}


static int protected_get_float_field(lua_State *L)
{
    lua_rawgeti(L, 1, 1);
    lua_rawgeti(L, 1, 2);
    lua_gettable(L, -2);

    int        success;
    lua_Number value = lua_tonumberx(L, -1, &success);
    if (success) {
        lua_pushnumber(L, value);
        return 1;
    }
    else {
        return luaL_error(L, "Can't convert field value to number");
    }
}

static float get_float_field(R_UserData user)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_get_float_field);
    R_lua_getreg(L, lv->reg);

    if (R_lua_pcall(L, 1, 1)) {
        R_LUA_ERROR_TO_WARNING(L);
        return 0.0f;
    }

    float f = XL_checkfloat(L, -1);
    lua_pop(L, 1);
    return f;
}

static int protected_set_float_field(lua_State *L)
{
    lua_rawgeti(L, 1, 1);
    lua_rawgeti(L, 1, 2);
    lua_pushvalue(L, 2);
    lua_settable(L, -3);
    return 0;
}

static void set_float_field(R_UserData user, float value)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_set_float_field);
    R_lua_getreg(L, lv->reg);
    XL_pushfloat(L, value);
    if (R_lua_pcall(L, 2, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
    }
}

static void free_float_field(R_UserData user)
{
    R_lua_value_free(user.data);
}


static int r_luatween_method_field_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    luaL_checkany(L, 2);
    int subject = 2;
    luaL_checkany(L, 3);
    int key = 3;
    luaL_checkany(L, 4);
    int value_fn = 4;
    lua_createtable(L, 2, 0);
    int table = lua_gettop(L);
    lua_pushvalue(L, subject);
    lua_rawseti(L, table, 1);
    lua_pushvalue(L, key);
    lua_rawseti(L, table, 2);

    R_TweenFloat value = tween_float_get(L, value_fn);
    R_LuaValue *lv = R_lua_value_new(L, table);
    R_tween_add_float(self, value, R_user_data(lv), get_float_field,
                      set_float_field, free_float_field, NULL);
    lua_settop(L, 1);
    return 1;
}

static int r_luatween_method_build_xl(lua_State *L)
{
    R_LuaTween *self = R_CPPCAST(R_LuaTween *, XL_checkpptype(L, 1, "R_LuaTween"));
    XL_UNUSED(self);
    XL_getiuservalue(L, 1, 1);
    return 1;
}

static int r_sequence_new_xl(lua_State *L)
{
    R_Sequence *RETVAL;
    RETVAL = R_sequence_new();
    XL_pushnewpptypeuv(L, RETVAL, "R_Sequence", 0);
    return 1;
}

static int r_sequence_method_gc_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype_nullable(L, 1, "R_Sequence"));
    R_sequence_free(self);
    return 0;
}


static int add_step(lua_State *L, R_Sequence *self, R_Step *step)
{
    R_sequence_add(self, step);
    lua_settop(L, 1);
    return 1;
}


static int protected_delay_step(lua_State *L)
{
    lua_call(L, 0, 1);
    int        success;
    lua_Number seconds = lua_tonumberx(L, -1, &success);
    if (success) {
        lua_pushnumber(L, seconds);
        return 1;
    }
    else {
        return luaL_error(L, "Can't convert delay amount to number");
    }
}

static float delay_step(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_delay_step);
    R_lua_getreg(L, lv->reg);
    if (lua_pcall(L, 1, 1, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
        return 0.0f;
    }
    else {
        float delay = R_lua_n2float(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return delay;
    }
}

static void delay_free(R_UserData user)
{
    R_lua_value_free(user.data);
}


static int r_sequence_method_delay_fixed_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype(L, 1, "R_Sequence"));
    float seconds = XL_checkfloat(L, 2);
    return add_step(L, self, R_delay_new_fixed(seconds));
}

static int r_sequence_method_delay_between_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype(L, 1, "R_Sequence"));
    float a = XL_checkfloat(L, 2);
    float b = XL_checkfloat(L, 3);
    return add_step(L, self, R_delay_new_between(a, b));
}

static int r_sequence_method_delay_custom_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype(L, 1, "R_Sequence"));
    luaL_checkany(L, 2);
    int fn = 2;
    return add_step(L, self, R_delay_new(delay_step, delay_free, NULL,
                                         R_user_data(R_lua_value_new(L, fn))));
}


static void call_step(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    R_lua_getreg(L, lv->reg);
    if (lua_pcall(L, 0, 0, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
    }
}

static void call_free(R_UserData user)
{
    R_lua_value_free(user.data);
}


static int r_sequence_method_call_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype(L, 1, "R_Sequence"));
    luaL_checkany(L, 2);
    int fn = 2;
    return add_step(L, self, R_call_new(call_step, call_free, NULL,
                                        R_user_data(R_lua_value_new(L, fn))));
}


typedef struct R_LuaEase {
    R_EaseFn          fn;
    R_TweenEaseFreeFn on_free;
    R_UserData        user;
} R_LuaEase;

static R_LuaEase to_named_ease(lua_State *L, int arg)
{
    R_EaseFn ease = R_lua_named_ease(L, arg);
    if (ease) {
        return (R_LuaEase){ease, NULL, R_user_null()};
    }
    else {
        luaL_error(L, "Don't know easing named '%s'", lua_tostring(L, arg));
        R_UNREACHABLE();
    }
}

static int protected_call_ease(lua_State *L)
{
    lua_call(L, 1, 1);
    int        success;
    lua_Number result = lua_tonumberx(L, -1, &success);
    if (success) {
        lua_pushnumber(L, result);
        return 1;
    }
    else {
        return luaL_error(L, "Can't convert ease result to number");
    }
}

static float call_ease(float k, R_UserData user)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_call_ease);
    R_lua_getreg(L, lv->reg);
    XL_pushfloat(L, k);
    if (lua_pcall(L, 2, 1, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
        return k;
    }
    else {
        float result = R_lua_n2float(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return result;
    }
}

static void free_ease(R_UserData user)
{
    R_lua_value_free(user.data);
}

static R_LuaEase to_custom_ease(lua_State *L, int arg)
{
    R_LuaValue *lv = R_lua_value_new(L, arg);
    return (R_LuaEase){call_ease, free_ease, R_user_data(lv)};
}

static R_LuaEase to_ease(lua_State *L, int arg)
{
    if (lua_isnoneornil(L, arg)) {
        return (R_LuaEase){NULL, NULL, R_user_null()};
    }
    else if (lua_isstring(L, arg)) {
        return to_named_ease(L, arg);
    }
    else {
        return to_custom_ease(L, arg);
    }
}

static int protected_tween_step(lua_State *L)
{
    lua_call(L, 0, 1);
    int        success;
    lua_Number seconds = lua_tonumberx(L, -1, &success);
    if (success) {
        lua_pushnumber(L, seconds);
        return 1;
    }
    else {
        return luaL_error(L, "Can't convert tween time to number");
    }
}

static float tween_step(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    lua_pushcfunction(L, protected_tween_step);
    R_lua_getreg(L, lv->reg);
    if (lua_pcall(L, 1, 1, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
        return 0.0f;
    }
    else {
        float result = R_lua_n2float(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return result;
    }
}

static void tween_free(R_UserData user)
{
    R_lua_value_free(user.data);
}


static int r_sequence_method_tween_fixed_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype(L, 1, "R_Sequence"));
    float seconds = XL_checkfloat(L, 2);
    R_LuaTween *RETVAL;
    R_LuaEase ease = to_ease(L, 3);
    RETVAL = R_tween_new_fixed(seconds, ease.fn, ease.on_free, ease.user);
    R_sequence_add(self, RETVAL);
    XL_pushnewpptypeuv(L, RETVAL, "R_LuaTween", 1);
    lua_pushvalue(L, 1);
    XL_setiuservalue(L, -2, 1);
    return 1;
}

static int r_sequence_method_tween_between_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype(L, 1, "R_Sequence"));
    float a = XL_checkfloat(L, 2);
    float b = XL_checkfloat(L, 3);
    R_LuaTween *RETVAL;
    R_LuaEase ease = to_ease(L, 4);
    RETVAL = R_tween_new_between(a, b, ease.fn, ease.on_free, ease.user);
    R_sequence_add(self, RETVAL);
    XL_pushnewpptypeuv(L, RETVAL, "R_LuaTween", 1);
    lua_pushvalue(L, 1);
    XL_setiuservalue(L, -2, 1);
    return 1;
}

static int r_sequence_method_tween_custom_xl(lua_State *L)
{
    R_Sequence *self = R_CPPCAST(R_Sequence *, XL_checkpptype(L, 1, "R_Sequence"));
    luaL_checkany(L, 2);
    int fn = 2;
    R_LuaTween *RETVAL;
    R_LuaEase ease = to_ease(L, 3);
    RETVAL = R_tween_new(tween_step, tween_free, NULL,
                         R_user_data(R_lua_value_new(L, fn)),
                         ease.fn, ease.on_free, ease.user);
    R_sequence_add(self, RETVAL);
    XL_pushnewpptypeuv(L, RETVAL, "R_LuaTween", 1);
    lua_pushvalue(L, 1);
    XL_setiuservalue(L, -2, 1);
    return 1;
}


static void seq_on_done(R_UserData user)
{
    R_LUA_VALUE_DECLARE_UNPACK(user.data, lv, L);
    R_lua_getreg(L, lv->reg);
    if (lua_pcall(L, 0, 0, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
    }
}

static void seq_on_free(R_UserData user)
{
    R_lua_value_free(user.data);
}


static int r_sequence_method_run_xl(lua_State *L)
{
    int RETVAL;
    R_Sequence **pp   = luaL_checkudata(L, 1, "R_Sequence");
    R_Scene    *scene = XL_checkpptype(L, 2, "R_Scene");
    int        laps   = (int) luaL_optinteger(L, 3, R_SEQUENCE_RUN_FOREVER);

    R_SequenceDoneFn on_done;
    R_SequenceFreeFn on_free;
    R_UserData       user;

    if (lua_isnoneornil(L, 4)) {
        on_done = NULL;
        on_free = NULL;
        user    = R_user_null();
    }
    else if (laps < 0) {
        return luaL_error(L, "Can't have done callback on infinite sequence");
    }
    else {
        on_done = seq_on_done;
        on_free = seq_on_free;
        user    = R_user_data(R_lua_value_new(L, 4));
    }

    RETVAL = R_animator_add(scene->animator, *pp, laps, on_done, on_free, user);
    *pp = NULL;
    XL_pushint(L, RETVAL);
    return 1;
}

#ifdef __cplusplus
extern "C" {
#endif

static int r_luatween_index_xl(lua_State *L)
{
    return XL_index_fallback(L, "R_LuaTween", 1, 2);
}

static int r_sequence_index_xl(lua_State *L)
{
    return XL_index_fallback(L, "R_Sequence", 1, 2);
}

static luaL_Reg r_sequence_function_registry_xl[] = {
    {"new", r_sequence_new_xl},
    {NULL, NULL},
};

static luaL_Reg r_tweenfloat_function_registry_xl[] = {
    {"between", r_tweenfloat_between_xl},
    {"custom", r_tweenfloat_custom_xl},
    {"fixed", r_tweenfloat_fixed_xl},
    {NULL, NULL},
};

static luaL_Reg r_tweenscale_function_registry_xl[] = {
    {"between", r_tweenscale_between_xl},
    {"custom", r_tweenscale_custom_xl},
    {"fixed", r_tweenscale_fixed_xl},
    {NULL, NULL},
};

static luaL_Reg r_tweenv4_function_registry_xl[] = {
    {"between", r_tweenv4_between_xl},
    {"custom", r_tweenv4_custom_xl},
    {"fixed", r_tweenv4_fixed_xl},
    {NULL, NULL},
};

static luaL_Reg r_luatween_method_registry_xl[] = {
    {"__index", r_luatween_index_xl},
    {"al_listener_gain", r_luatween_method_al_listener_gain_xl},
    {"al_listener_pos_x", r_luatween_method_al_listener_pos_x_xl},
    {"al_listener_pos_y", r_luatween_method_al_listener_pos_y_xl},
    {"al_listener_pos_z", r_luatween_method_al_listener_pos_z_xl},
    {"al_listener_velocity_x", r_luatween_method_al_listener_velocity_x_xl},
    {"al_listener_velocity_y", r_luatween_method_al_listener_velocity_y_xl},
    {"al_listener_velocity_z", r_luatween_method_al_listener_velocity_z_xl},
    {"al_source_gain", r_luatween_method_al_source_gain_xl},
    {"al_source_pitch", r_luatween_method_al_source_pitch_xl},
    {"al_source_pos_x", r_luatween_method_al_source_pos_x_xl},
    {"al_source_pos_y", r_luatween_method_al_source_pos_y_xl},
    {"al_source_pos_z", r_luatween_method_al_source_pos_z_xl},
    {"al_source_reference_distance", r_luatween_method_al_source_reference_distance_xl},
    {"al_source_velocity_x", r_luatween_method_al_source_velocity_x_xl},
    {"al_source_velocity_y", r_luatween_method_al_source_velocity_y_xl},
    {"al_source_velocity_z", r_luatween_method_al_source_velocity_z_xl},
    {"build", r_luatween_method_build_xl},
    {"field", r_luatween_method_field_xl},
    {"sprite_alpha", r_luatween_method_sprite_alpha_xl},
    {"sprite_angle", r_luatween_method_sprite_angle_xl},
    {"sprite_base_x", r_luatween_method_sprite_base_x_xl},
    {"sprite_base_y", r_luatween_method_sprite_base_y_xl},
    {"sprite_origin_x", r_luatween_method_sprite_origin_x_xl},
    {"sprite_origin_y", r_luatween_method_sprite_origin_y_xl},
    {"sprite_pos_x", r_luatween_method_sprite_pos_x_xl},
    {"sprite_pos_y", r_luatween_method_sprite_pos_y_xl},
    {"sprite_rel_x", r_luatween_method_sprite_rel_x_xl},
    {"sprite_rel_y", r_luatween_method_sprite_rel_y_xl},
    {"sprite_rotation", r_luatween_method_sprite_rotation_xl},
    {"sprite_scale", r_luatween_method_sprite_scale_xl},
    {"sprite_scale_x", r_luatween_method_sprite_scale_x_xl},
    {"sprite_scale_y", r_luatween_method_sprite_scale_y_xl},
    {"sprite_skew_x", r_luatween_method_sprite_skew_x_xl},
    {"sprite_skew_y", r_luatween_method_sprite_skew_y_xl},
    {"sprite_tint", r_luatween_method_sprite_tint_xl},
    {NULL, NULL},
};

static luaL_Reg r_sequence_method_registry_xl[] = {
    {"__gc", r_sequence_method_gc_xl},
    {"__index", r_sequence_index_xl},
    {"call", r_sequence_method_call_xl},
    {"delay_between", r_sequence_method_delay_between_xl},
    {"delay_custom", r_sequence_method_delay_custom_xl},
    {"delay_fixed", r_sequence_method_delay_fixed_xl},
    {"run", r_sequence_method_run_xl},
    {"tween_between", r_sequence_method_tween_between_xl},
    {"tween_custom", r_sequence_method_tween_custom_xl},
    {"tween_fixed", r_sequence_method_tween_fixed_xl},
    {NULL, NULL},
};

int R_lua_seq_init(lua_State *L)
{
    XL_initmetatable(L, "R_LuaTween", r_luatween_method_registry_xl);
    XL_initmetatable(L, "R_Sequence", r_sequence_method_registry_xl);
    XL_initmetatable(L, "R_TweenFloat", NULL);
    XL_initmetatable(L, "R_TweenScale", NULL);
    XL_initmetatable(L, "R_TweenV4", NULL);
    XL_initfunctions(L, r_sequence_function_registry_xl, "R", "Sequence", (const char *)NULL);
    XL_initfunctions(L, r_tweenfloat_function_registry_xl, "R", "TweenFloat", (const char *)NULL);
    XL_initfunctions(L, r_tweenscale_function_registry_xl, "R", "TweenScale", (const char *)NULL);
    XL_initfunctions(L, r_tweenv4_function_registry_xl, "R", "TweenV4", (const char *)NULL);
    return 0;
}

#ifdef __cplusplus
}
#endif
