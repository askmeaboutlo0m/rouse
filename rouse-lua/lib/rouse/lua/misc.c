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
#include "main.h"

#ifdef _WIN32
#   include <windows.h>
#else
#   include <time.h>
#endif


static void log_message(lua_State *L, void (*log_fn)(const char *, int,
                                                     const char *, ...))
{
    int argc = lua_gettop(L);
    lua_getglobal(L, "string");
    lua_getfield(L, -1, "format");
    lua_insert(L, 1);
    lua_pop(L, 1);
    lua_call(L, argc, 1);
    const char *message = lua_tostring(L, -1);

    lua_Debug ar;
    lua_getstack(L, 1, &ar);
    lua_getinfo(L, "Sl", &ar);
    const char *file = ar.currentline > 0 ? ar.short_src : "(unknown)";

    log_fn(file, ar.currentline, "%s", message);
}


static int r_log_enable_xl(lua_State *L)
{
    unsigned int logbits = XL_checkuint(L, 1);
    R_log_enable(logbits);
    return 0;
}

static int r_log_disable_xl(lua_State *L)
{
    unsigned int logbits = XL_checkuint(L, 1);
    R_log_disable(logbits);
    return 0;
}

static int r_debug_xl(lua_State *L)
{
    log_message(L, R_debug_fn);
    return 0;
}

static int r_info_xl(lua_State *L)
{
    log_message(L, R_info_fn);
    return 0;
}

static int r_warn_xl(lua_State *L)
{
    log_message(L, R_warn_fn);
    return 0;
}

static int r_die_xl(lua_State *L)
{
    log_message(L, R_die_fn);
    return 0;
}

static int r_platform_staticindex_xl(lua_State *L)
{
    const char *RETVAL;
    RETVAL = ROUSE_PLATFORM;
    lua_pushstring(L, RETVAL);
    return 1;
}

static int r_get_platform_xl(lua_State *L)
{
    const char *RETVAL;
    R_LUA_WARN_ON_DEPRECATED_GETTER("R.platform");
    RETVAL = ROUSE_PLATFORM;
    lua_pushstring(L, RETVAL);
    return 1;
}

static int r_framerate_staticindex_xl(lua_State *L)
{
    float RETVAL;
    RETVAL = R_framerate_get();
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_get_framerate_xl(lua_State *L)
{
    float RETVAL;
    R_LUA_WARN_ON_DEPRECATED_GETTER("R.framerate");
    RETVAL = R_framerate_get();
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_framerate_staticnewindex_xl(lua_State *L)
{
    float VALUE = XL_checkfloat(L, 1);
    R_framerate_set(VALUE);
    return 0;
}

static int r_set_framerate_xl(lua_State *L)
{
    float ticks_per_second = XL_checkfloat(L, 1);
    R_LUA_WARN_ON_DEPRECATED_SETTER("R.framerate");
    R_framerate_set(ticks_per_second);
    return 0;
}

static int r_tick_length_staticindex_xl(lua_State *L)
{
    float RETVAL;
    RETVAL = R_tick_length;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_get_tick_length_xl(lua_State *L)
{
    float RETVAL;
    R_LUA_WARN_ON_DEPRECATED_GETTER("R.tick_length");
    RETVAL = R_tick_length;
    XL_pushfloat(L, RETVAL);
    return 1;
}

static int r_max_ticks_before_render_staticnewindex_xl(lua_State *L)
{
    uint32_t VALUE = XL_checkuint32(L, 1);
    R_max_ticks_before_render = VALUE;
    return 0;
}

static int r_set_max_ticks_before_render_xl(lua_State *L)
{
    uint32_t max_ticks_before_render = XL_checkuint32(L, 1);
    R_LUA_WARN_ON_DEPRECATED_SETTER("R.max_ticks_before_render");
    R_max_ticks_before_render = max_ticks_before_render;
    return 0;
}

static int r_al_enabled_staticindex_xl(lua_State *L)
{
    bool RETVAL;
    RETVAL = R_al_enabled;
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int r_get_al_enabled_xl(lua_State *L)
{
    bool RETVAL;
    R_LUA_WARN_ON_DEPRECATED_GETTER("R.al_enabled");
    RETVAL = R_al_enabled;
    lua_pushboolean(L, RETVAL);
    return 1;
}

static int r_rand_between_xl(lua_State *L)
{
    float a = XL_checkfloat(L, 1);
    float b = XL_checkfloat(L, 2);
    float RETVAL;
    RETVAL = R_rand_between(a, b);
    XL_pushfloat(L, RETVAL);
    return 1;
}


static int on_fetch_done(lua_State *L)
{
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_call(L, 0, 0);
    return 0;
}


static int r_fetch_xl(lua_State *L)
{
    const char *url = luaL_checkstring(L, 1);
    const char *path = luaL_checkstring(L, 2);
    luaL_checkany(L, 3);
    int on_progress = 3;
    luaL_checkany(L, 4);
    int on_done = 4;
    lua_pushvalue(L, on_progress);
    lua_pushvalue(L, on_done);
    lua_pushcclosure(L, on_fetch_done, 1);
    R_lua_fetch(L, url, path);
    return 0;
}

static int r_fetch_pack_xl(lua_State *L)
{
    const char *url = luaL_checkstring(L, 1);
    luaL_checkany(L, 2);
    int on_progress = 2;
    luaL_checkany(L, 3);
    int on_done = 3;
    lua_pushvalue(L, on_progress);
    lua_pushvalue(L, on_done);
    lua_pushcclosure(L, on_fetch_done, 1);
    R_lua_fetch_pack(L, url);
    return 0;
}

static int r_timestamp_as_double_xl(lua_State *L)
{
    double RETVAL;
#ifdef _WIN32
    /* Gives the number of 100 nanosecond intervals since 1601-01-01. */
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    /* FILETIME is not aligned properly, so we gotta move it over. */
    LARGE_INTEGER timestamp;
    timestamp.LowPart  = ft.dwLowDateTime;
    timestamp.HighPart = (LONG) ft.dwHighDateTime;
    /* Subtract the difference between 1970 and 1601 to get the right epoch. */
    timestamp.QuadPart -= 116444736000000000LL;
    /* And finally turn it into a double of seconds and a fraction. */
    RETVAL = (double) timestamp.QuadPart / 10000000.0;
#else
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    RETVAL = (double) ts.tv_sec + (double) ts.tv_nsec / 1000000000.0;
#endif
    XL_pushnewutypeuv(L, &RETVAL, sizeof(double), "double", 0);
    return 1;
}

static int r_staticindex_dummy_xl;
static int r_staticindex_xl(lua_State *L)
{
    return XL_staticindex(L, &r_staticindex_dummy_xl, 2);
}

static int r_staticnewindex_dummy_xl;
static int r_staticnewindex_xl(lua_State *L)
{
    return XL_staticnewindex(L, &r_staticnewindex_dummy_xl, 1, 2, 3);
}

static luaL_Reg r_function_registry_xl[] = {
    {"__index", r_staticindex_xl},
    {"__newindex", r_staticnewindex_xl},
    {"debug", r_debug_xl},
    {"die", r_die_xl},
    {"fetch", r_fetch_xl},
    {"fetch_pack", r_fetch_pack_xl},
    {"get_al_enabled", r_get_al_enabled_xl},
    {"get_framerate", r_get_framerate_xl},
    {"get_platform", r_get_platform_xl},
    {"get_tick_length", r_get_tick_length_xl},
    {"info", r_info_xl},
    {"rand_between", r_rand_between_xl},
    {"set_framerate", r_set_framerate_xl},
    {"set_max_ticks_before_render", r_set_max_ticks_before_render_xl},
    {"timestamp_as_double", r_timestamp_as_double_xl},
    {"warn", r_warn_xl},
    {NULL, NULL},
};

static luaL_Reg r_log_function_registry_xl[] = {
    {"disable", r_log_disable_xl},
    {"enable", r_log_enable_xl},
    {NULL, NULL},
};

static luaL_Reg r_staticindex_registry_xl[] = {
    {"al_enabled", r_al_enabled_staticindex_xl},
    {"framerate", r_framerate_staticindex_xl},
    {"platform", r_platform_staticindex_xl},
    {"tick_length", r_tick_length_staticindex_xl},
    {NULL, NULL},
};

static luaL_Reg r_staticnewindex_registry_xl[] = {
    {"framerate", r_framerate_staticnewindex_xl},
    {"max_ticks_before_render", r_max_ticks_before_render_staticnewindex_xl},
    {NULL, NULL},
};

static XL_EnumEntry r_logbit_enum_xl[] = {
    {"DEBUG", (lua_Integer) R_LOGBIT_DEBUG},
    {"INFO", (lua_Integer) R_LOGBIT_INFO},
    {"WARN", (lua_Integer) R_LOGBIT_WARN},
    {"DIE", (lua_Integer) R_LOGBIT_DIE},
    {NULL, (lua_Integer) 0},
};

int R_lua_misc_init(lua_State *L)
{
    XL_initindextable(L, &r_staticindex_dummy_xl, r_staticindex_registry_xl);
    XL_initnewindextable(L, &r_staticnewindex_dummy_xl, r_staticnewindex_registry_xl);
    XL_initstaticmetatable(L, "R", (const char *)NULL);
    XL_initfunctions(L, r_function_registry_xl, "R", (const char *)NULL);
    XL_initfunctions(L, r_log_function_registry_xl, "R", "log", (const char *)NULL);
    XL_initenum(L, r_logbit_enum_xl, "R", "Logbit", (const char *)NULL);
    return 0;
}
