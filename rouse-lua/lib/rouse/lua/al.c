/*************************************************/
/* This file was auto-generated by xlgen.        */
/* Don't edit it directly, it'll be overwritten! */
/* Modify the matching .xl file and rebuild.     */
/*************************************************/
/*
 * Copyright (c) 2020 askmeaboutloom
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


static int r_al_listener_pos_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_V3 RETVAL;
    RETVAL = R_al_listener_pos();
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V3), "R_V3", 0);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_pos_x_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_pos_x();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_pos_y_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_pos_y();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_pos_z_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_pos_z();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_V3 RETVAL;
    RETVAL = R_al_listener_velocity();
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V3), "R_V3", 0);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_x_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_velocity_x();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_y_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_velocity_y();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_z_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_velocity_z();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_gain_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_gain();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_pos_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_V3 VALUE = *((R_V3 *)luaL_checkudata(L, 1, "R_V3"));
    R_al_listener_pos_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_pos_x_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_pos_x_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_pos_y_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_pos_y_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_pos_z_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_pos_z_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_V3 VALUE = *((R_V3 *)luaL_checkudata(L, 1, "R_V3"));
    R_al_listener_velocity_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_x_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_velocity_x_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_y_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_velocity_y_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_velocity_z_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_velocity_z_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_gain_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_gain_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_x_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_pos_x();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_x_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_pos_x_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_y_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_pos_y();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_y_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_pos_y_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_z_staticindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float RETVAL;
    RETVAL = R_al_listener_pos_z();
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_listener_z_staticnewindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    float VALUE = XL_checkfloat(L, 1);
    R_al_listener_pos_z_set(VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_buffer_from_sample_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_Sample *sample = R_CPPCAST(R_Sample *, XL_checkpptype(L, 1, "R_Sample"));
    R_AlBuffer *RETVAL;
    RETVAL = R_al_buffer_from_sample(sample);
    XL_pushnewpptypeuv(L, RETVAL, "R_AlBuffer", 0);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_al_buffer_from_file_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    const char *path = luaL_checkstring(L, 1);
    R_AlBuffer *RETVAL;
    RETVAL = R_al_buffer_from_file(path);
    XL_pushnewpptypeuv(L, RETVAL, "R_AlBuffer", 0);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_albuffer_method_gc_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlBuffer *self = R_CPPCAST(R_AlBuffer *, XL_checkpptype_nullable(L, 1, "R_AlBuffer"));
    R_al_buffer_decref(self);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_albuffer_method_eq_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    luaL_checkany(L, 1);
    int a = 1;
    luaL_checkany(L, 2);
    int b = 2;
    bool RETVAL;
    R_AlBuffer **app = luaL_testudata(L, a, "R_AlBuffer");
    R_AlBuffer **bpp = luaL_testudata(L, b, "R_AlBuffer");
    RETVAL = app == bpp || (app && bpp && *app == *bpp);
    lua_pushboolean(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_albuffer_id_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlBuffer *self = R_CPPCAST(R_AlBuffer *, XL_checkpptype(L, 1, "R_AlBuffer"));
    unsigned int RETVAL;
    RETVAL = R_al_buffer_id(self);
    XL_pushuint(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}


static void maybe_set(lua_State *L, int argc, int index)
{
    if (argc >= index) {
        lua_getfield(L, -1, "set");
        lua_pushvalue(L, -2);
        lua_pushvalue(L, index);
        lua_call(L, 2, 0);
    }
}


#define XL_REQUIRED_ARGC 0
static int r_al_source_new_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *RETVAL;
    int argc = lua_gettop(L);
    RETVAL   = R_al_source_new();
    XL_pushnewpptypeuv(L, RETVAL, "R_AlSource", 1);
    maybe_set(L, argc, XL_REQUIRED_ARGC + 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}
#undef XL_REQUIRED_ARGC

#define XL_REQUIRED_ARGC 1
static int r_al_source_from_buffer_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlBuffer *buffer = R_CPPCAST(R_AlBuffer *, XL_checkpptype(L, 1, "R_AlBuffer"));
    R_AlSource *RETVAL;
    int argc = lua_gettop(L);
    RETVAL   = R_al_source_from_buffer(buffer);
    XL_pushnewpptypeuv(L, RETVAL, "R_AlSource", 1);
    maybe_set(L, argc, XL_REQUIRED_ARGC + 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}
#undef XL_REQUIRED_ARGC

#define XL_REQUIRED_ARGC 1
static int r_al_source_from_file_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    const char *path = luaL_checkstring(L, 1);
    R_AlSource *RETVAL;
    int argc = lua_gettop(L);
    RETVAL = R_al_source_from_file(path);
    XL_pushnewpptypeuv(L, RETVAL, "R_AlSource", 1);
    maybe_set(L, argc, XL_REQUIRED_ARGC + 1);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}
#undef XL_REQUIRED_ARGC

static int r_alsource_method_gc_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype_nullable(L, 1, "R_AlSource"));
    R_al_source_decref(self);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_id_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    unsigned int RETVAL;
    RETVAL = R_al_source_id(self);
    XL_pushuint(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_buffer_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_AlBuffer *RETVAL;
    RETVAL = R_al_source_buffer(self);
    if (RETVAL) {
        R_al_buffer_incref(RETVAL);
    }
    XL_pushnewpptypeuv_nullable(L, RETVAL, "R_AlBuffer", 0);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_buffer_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_AlBuffer *VALUE = R_CPPCAST(R_AlBuffer *, XL_checkpptype(L, 2, "R_AlBuffer"));
    R_al_source_buffer_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_V3 RETVAL;
    RETVAL = R_al_source_pos(self);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V3), "R_V3", 0);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_x_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_pos_x(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_y_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_pos_y(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_z_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_pos_z(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_V3 RETVAL;
    RETVAL = R_al_source_velocity(self);
    XL_pushnewutypeuv(L, &RETVAL, sizeof(R_V3), "R_V3", 0);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_x_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_velocity_x(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_y_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_velocity_y(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_z_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_velocity_z(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_gain_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_gain(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pitch_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_pitch(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_reference_distance_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_reference_distance(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_offset_in_seconds_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_offset_in_seconds(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_looping_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    bool RETVAL;
    RETVAL = R_al_source_looping(self);
    lua_pushboolean(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_V3 VALUE = *((R_V3 *)luaL_checkudata(L, 2, "R_V3"));
    R_al_source_pos_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_x_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_pos_x_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_y_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_pos_y_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pos_z_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_pos_z_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_V3 VALUE = *((R_V3 *)luaL_checkudata(L, 2, "R_V3"));
    R_al_source_velocity_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_x_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_velocity_x_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_y_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_velocity_y_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_velocity_z_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_velocity_z_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_gain_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_gain_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_pitch_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_pitch_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_reference_distance_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_reference_distance_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_offset_in_seconds_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_offset_in_seconds_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_looping_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    bool VALUE = XL_checkbool(L, 2);
    R_al_source_looping_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_x_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_pos_x(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_x_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_pos_x_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_y_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_pos_y(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_y_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_pos_y_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_z_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float RETVAL;
    RETVAL = R_al_source_pos_z(self);
    XL_pushfloat(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_z_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    float VALUE = XL_checkfloat(L, 2);
    R_al_source_pos_z_set(self, VALUE);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_cross_scene_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    bool RETVAL;
    XL_UNUSED(self);
    XL_getiuservalue(L, 1, 1);
    RETVAL = lua_toboolean(L, -1);
    lua_pushboolean(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_cross_scene_newindex_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    bool VALUE = XL_checkbool(L, 2);
    XL_UNUSED(self);
    lua_pushboolean(L, VALUE);
    XL_setiuservalue(L, 1, 1);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_method_set_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    return XL_setfromtable(L, "R_AlSource", 1, 2);
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_method_play_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_al_source_play(self);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_method_stop_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_al_source_stop(self);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_method_rewind_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_al_source_rewind(self);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_method_pause_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    R_al_source_pause(self);
    return 0;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_state_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    const char *RETVAL;
    switch (R_al_source_state(self)) {
        case AL_INITIAL: RETVAL = "initial"; break;
        case AL_PLAYING: RETVAL = "playing"; break;
        case AL_PAUSED:  RETVAL = "paused" ; break;
        case AL_STOPPED: RETVAL = "stopped"; break;
        default:         RETVAL = "unknown"; break;
    }
    lua_pushstring(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_alsource_playing_index_xl(lua_State *L)
{
#ifdef ROUSE_AL_ENABLED
    R_AlSource *self = R_CPPCAST(R_AlSource *, XL_checkpptype(L, 1, "R_AlSource"));
    bool RETVAL;
    RETVAL = R_al_source_playing(self);
    lua_pushboolean(L, RETVAL);
    return 1;
#else
    R_LUA_DIE(L, "OpenAL not compiled in")
#endif
}

static int r_albuffer_index_dummy_xl;
static int r_albuffer_index_xl(lua_State *L)
{
    return XL_index(L, "R_AlBuffer", &r_albuffer_index_dummy_xl, 1, 2);
}

static int r_alsource_index_dummy_xl;
static int r_alsource_index_xl(lua_State *L)
{
    return XL_index(L, "R_AlSource", &r_alsource_index_dummy_xl, 1, 2);
}

static int r_alsource_newindex_dummy_xl;
static int r_alsource_newindex_xl(lua_State *L)
{
    return XL_newindex(L, "R_AlSource", &r_alsource_newindex_dummy_xl, 1, 2, 3);
}

static int r_al_listener_staticindex_dummy_xl;
static int r_al_listener_staticindex_xl(lua_State *L)
{
    return XL_staticindex(L, &r_al_listener_staticindex_dummy_xl, 2);
}

static int r_al_listener_staticnewindex_dummy_xl;
static int r_al_listener_staticnewindex_xl(lua_State *L)
{
    return XL_staticnewindex(L, &r_al_listener_staticnewindex_dummy_xl, 1, 2, 3);
}

static luaL_Reg r_al_buffer_function_registry_xl[] = {
    {"from_file", r_al_buffer_from_file_xl},
    {"from_sample", r_al_buffer_from_sample_xl},
    {NULL, NULL},
};

static luaL_Reg r_al_source_function_registry_xl[] = {
    {"from_buffer", r_al_source_from_buffer_xl},
    {"from_file", r_al_source_from_file_xl},
    {"new", r_al_source_new_xl},
    {NULL, NULL},
};

static luaL_Reg r_al_listener_function_registry_xl[] = {
    {"__index", r_al_listener_staticindex_xl},
    {"__newindex", r_al_listener_staticnewindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_albuffer_index_registry_xl[] = {
    {"id", r_albuffer_id_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_alsource_index_registry_xl[] = {
    {"buffer", r_alsource_buffer_index_xl},
    {"cross_scene", r_alsource_cross_scene_index_xl},
    {"gain", r_alsource_gain_index_xl},
    {"id", r_alsource_id_index_xl},
    {"looping", r_alsource_looping_index_xl},
    {"offset_in_seconds", r_alsource_offset_in_seconds_index_xl},
    {"pitch", r_alsource_pitch_index_xl},
    {"playing", r_alsource_playing_index_xl},
    {"pos", r_alsource_pos_index_xl},
    {"pos_x", r_alsource_pos_x_index_xl},
    {"pos_y", r_alsource_pos_y_index_xl},
    {"pos_z", r_alsource_pos_z_index_xl},
    {"reference_distance", r_alsource_reference_distance_index_xl},
    {"state", r_alsource_state_index_xl},
    {"velocity", r_alsource_velocity_index_xl},
    {"velocity_x", r_alsource_velocity_x_index_xl},
    {"velocity_y", r_alsource_velocity_y_index_xl},
    {"velocity_z", r_alsource_velocity_z_index_xl},
    {"x", r_alsource_x_index_xl},
    {"y", r_alsource_y_index_xl},
    {"z", r_alsource_z_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_albuffer_method_registry_xl[] = {
    {"__eq", r_albuffer_method_eq_xl},
    {"__gc", r_albuffer_method_gc_xl},
    {"__index", r_albuffer_index_xl},
    {NULL, NULL},
};

static luaL_Reg r_alsource_method_registry_xl[] = {
    {"__gc", r_alsource_method_gc_xl},
    {"__index", r_alsource_index_xl},
    {"__newindex", r_alsource_newindex_xl},
    {"pause", r_alsource_method_pause_xl},
    {"play", r_alsource_method_play_xl},
    {"rewind", r_alsource_method_rewind_xl},
    {"set", r_alsource_method_set_xl},
    {"stop", r_alsource_method_stop_xl},
    {NULL, NULL},
};

static luaL_Reg r_alsource_newindex_registry_xl[] = {
    {"buffer", r_alsource_buffer_newindex_xl},
    {"cross_scene", r_alsource_cross_scene_newindex_xl},
    {"gain", r_alsource_gain_newindex_xl},
    {"looping", r_alsource_looping_newindex_xl},
    {"offset_in_seconds", r_alsource_offset_in_seconds_newindex_xl},
    {"pitch", r_alsource_pitch_newindex_xl},
    {"pos", r_alsource_pos_newindex_xl},
    {"pos_x", r_alsource_pos_x_newindex_xl},
    {"pos_y", r_alsource_pos_y_newindex_xl},
    {"pos_z", r_alsource_pos_z_newindex_xl},
    {"reference_distance", r_alsource_reference_distance_newindex_xl},
    {"velocity", r_alsource_velocity_newindex_xl},
    {"velocity_x", r_alsource_velocity_x_newindex_xl},
    {"velocity_y", r_alsource_velocity_y_newindex_xl},
    {"velocity_z", r_alsource_velocity_z_newindex_xl},
    {"x", r_alsource_x_newindex_xl},
    {"y", r_alsource_y_newindex_xl},
    {"z", r_alsource_z_newindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_al_listener_staticindex_registry_xl[] = {
    {"gain", r_al_listener_gain_staticindex_xl},
    {"pos", r_al_listener_pos_staticindex_xl},
    {"pos_x", r_al_listener_pos_x_staticindex_xl},
    {"pos_y", r_al_listener_pos_y_staticindex_xl},
    {"pos_z", r_al_listener_pos_z_staticindex_xl},
    {"velocity", r_al_listener_velocity_staticindex_xl},
    {"velocity_x", r_al_listener_velocity_x_staticindex_xl},
    {"velocity_y", r_al_listener_velocity_y_staticindex_xl},
    {"velocity_z", r_al_listener_velocity_z_staticindex_xl},
    {"x", r_al_listener_x_staticindex_xl},
    {"y", r_al_listener_y_staticindex_xl},
    {"z", r_al_listener_z_staticindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_al_listener_staticnewindex_registry_xl[] = {
    {"gain", r_al_listener_gain_staticnewindex_xl},
    {"pos", r_al_listener_pos_staticnewindex_xl},
    {"pos_x", r_al_listener_pos_x_staticnewindex_xl},
    {"pos_y", r_al_listener_pos_y_staticnewindex_xl},
    {"pos_z", r_al_listener_pos_z_staticnewindex_xl},
    {"velocity", r_al_listener_velocity_staticnewindex_xl},
    {"velocity_x", r_al_listener_velocity_x_staticnewindex_xl},
    {"velocity_y", r_al_listener_velocity_y_staticnewindex_xl},
    {"velocity_z", r_al_listener_velocity_z_staticnewindex_xl},
    {"x", r_al_listener_x_staticnewindex_xl},
    {"y", r_al_listener_y_staticnewindex_xl},
    {"z", r_al_listener_z_staticnewindex_xl},
    {NULL, NULL},
};

int R_lua_al_init(lua_State *L)
{
    XL_initmetatable(L, "R_AlBuffer", r_albuffer_method_registry_xl);
    XL_initmetatable(L, "R_AlSource", r_alsource_method_registry_xl);
    XL_initindextable(L, &r_albuffer_index_dummy_xl, r_albuffer_index_registry_xl);
    XL_initindextable(L, &r_alsource_index_dummy_xl, r_alsource_index_registry_xl);
    XL_initnewindextable(L, &r_alsource_newindex_dummy_xl, r_alsource_newindex_registry_xl);
    XL_initindextable(L, &r_al_listener_staticindex_dummy_xl, r_al_listener_staticindex_registry_xl);
    XL_initnewindextable(L, &r_al_listener_staticnewindex_dummy_xl, r_al_listener_staticnewindex_registry_xl);
    XL_initstaticmetatable(L, "R", "Al", "listener", (const char *)NULL);
    XL_initfunctions(L, r_al_buffer_function_registry_xl, "R", "Al", "Buffer", (const char *)NULL);
    XL_initfunctions(L, r_al_source_function_registry_xl, "R", "Al", "Source", (const char *)NULL);
    XL_initfunctions(L, r_al_listener_function_registry_xl, "R", "Al", "listener", (const char *)NULL);
    return 0;
}
