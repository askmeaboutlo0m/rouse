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


static int protected_scene_callback(lua_State *L)
{
    int argc = lua_gettop(L);
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TSTRING);
    lua_pushvalue(L, 2);

    if (lua_gettable(L, 1)) {
        lua_pushvalue(L, 1);
        for (int i = 3; i <= argc; ++i) {
            lua_pushvalue(L, i);
        }
        lua_call(L, argc - 1, 0);
    }

    return 0;
}

#define CALL_ON_SCENE(SCENE, NAME, NARGS, PUSH_MORE_ARGS) \
    R_LUA_VALUE_DECLARE_UNPACK(SCENE->user.data, lv, L); \
    lua_pushcfunction(L, protected_scene_callback); \
    R_lua_getreg(L, lv->reg); \
    lua_getuservalue(L, -1); \
    lua_replace(L, -2); \
    lua_pushliteral(L, NAME); \
    PUSH_MORE_ARGS; \
    if (lua_pcall(L, NARGS, 0, 0)) { \
        R_LUA_ERROR_TO_WARNING(L); \
    }

static void push_event(lua_State *L, SDL_Event *event)
{
    XL_pushnewutype(L, event, sizeof(*event), "SDL_Event");
}

static void scene_on_event(R_Scene *scene, SDL_Event *event)
{
    CALL_ON_SCENE(scene, "on_event", 3, push_event(L, event));
}

static void scene_on_tick(R_Scene *scene, bool rendered)
{
    CALL_ON_SCENE(scene, "on_tick", 3, lua_pushboolean(L, rendered));
}

static void scene_on_render(R_Scene *scene)
{
    CALL_ON_SCENE(scene, "on_render", 2, R_NOOP());
}

static void scene_on_free(R_Scene *scene)
{
    CALL_ON_SCENE(scene, "on_free", 2, R_NOOP());
    R_lua_getreg(L, lv->reg);
    void **pp = luaL_checkudata(L, -1, "R_Scene");
    *pp = NULL;
    lua_pop(L, 1);
    R_lua_value_free(lv);
    lua_gc(L, LUA_GCCOLLECT, 0);
}




int R_lua_scene_new(lua_State *L)
{
    R_Scene *scene   = R_scene_new();
    scene->on_event  = scene_on_event;
    scene->on_render = scene_on_render;
    scene->on_tick   = scene_on_tick;

    void **pp = XL_pushnewpptype(L, scene, "R_Scene");
    lua_pushvalue(L, -2);
    lua_pushvalue(L, -2);

    if (R_lua_pcall(L, 1, 1)) {
        *pp = NULL;
        R_scene_free(scene);
        return lua_error(L);
    }

    lua_setuservalue(L, -2);
    scene->user.data = R_lua_value_new(L, -1);
    scene->on_free   = scene_on_free;

    lua_pushlightuserdata(L, scene);
    lua_gc(L, LUA_GCCOLLECT, 0);
    return 1;
}

R_Scene *make_next_scene(void *user)
{
    R_LUA_VALUE_DECLARE_UNPACK(user, lv, L);
    lua_pushcfunction(L, R_lua_scene_new);
    R_lua_getreg(L, lv->reg);
    R_lua_value_free(lv);

    if (lua_pcall(L, 1, 1, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
        return NULL;
    }

    R_Scene *scene = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return scene;
}


static int r_scene_next_xl(lua_State *L)
{
    luaL_checkany(L, 1);
    int fn = 1;
    R_LuaValue *old_lv = R_scene_next(make_next_scene, R_lua_value_new(L, fn));
    R_lua_value_free(old_lv);
    return 0;
}

static luaL_Reg r_scene_function_registry_xl[] = {
    {"next", r_scene_next_xl},
    {NULL, NULL},
};

int R_lua_scene_init(lua_State *L)
{
    XL_initmetatable(L, "R_Scene", NULL);
    XL_initfunctions(L, r_scene_function_registry_xl, "R", "Scene", (const char *)NULL);
    return 0;
}
