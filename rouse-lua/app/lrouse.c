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
#include <rouse/lua.h>

#ifdef __EMSCRIPTEN__
#   include <emscripten/fetch.h>
#endif


#define MAIN_PREFIX        "--main="
#define EMBOOTSTRAP_PREFIX "--embootstrap="


static void maybe_handle_arg(lua_State *L, const char *global_name,
                             const char *prefix, const char *arg)
{
    size_t prefix_len = strlen(prefix);
    if (strncmp(prefix, arg, prefix_len) == 0) {
        lua_pushstring(L, arg + prefix_len);
        lua_setglobal(L, global_name);
    }
}

static void parse_arg(lua_State *L, int i, const char *arg)
{
    maybe_handle_arg(L, "MAIN",        MAIN_PREFIX,        arg);
#ifdef __EMSCRIPTEN__
    maybe_handle_arg(L, "EMBOOTSTRAP", EMBOOTSTRAP_PREFIX, arg);
#endif
    lua_pushinteger(L, i);
    lua_pushstring(L, arg);
    lua_settable(L, -3);
}

static void parse_args(lua_State *L, int argc, char **argv)
{
    lua_createtable(L, 0, argc + 1);
    for (int i = 0; i < argc; ++i) {
        parse_arg(L, i, argv[i]);
    }
    lua_setglobal(L, "arg");
}


static const char *get_global_string(lua_State *L, const char *global_name)
{
    lua_getglobal(L, global_name);
    const char *s = luaL_checkstring(L, -1);
    lua_pop(L, 1);
    return s;
}

static void run_main(lua_State *L)
{
    const char *path = get_global_string(L, "MAIN");
    if (luaL_loadfile(L, path) || lua_pcall(L, 0, 1, 0)) {
        const char *error = lua_tostring(L, -1);
        R_die("Error running '%s': %s", path, error);
    }
    R_lua_main(L);
}


#ifdef __EMSCRIPTEN__

static int bootstrap_next(lua_State *L)
{
    lua_Integer i = luaL_checkinteger(L, lua_upvalueindex(2));
    if (lua_geti(L, lua_upvalueindex(1), i)) {
        const char *path = luaL_checkstring(L, -1);
        lua_pushvalue(L, lua_upvalueindex(1));
        lua_pushinteger(L, i + 1);
        lua_pushcclosure(L, bootstrap_next, 2);
        R_lua_fetch(L, path, path);
    }
    else {
        run_main(L);
    }
    return 0;
}


static int on_bootstrap_fetched(lua_State *L)
{
    emscripten_fetch_t *fetch = lua_touserdata(L, 2);
    if (luaL_loadbufferx(L, fetch->data, (size_t) fetch->numBytes,
                         fetch->url, "t")) {
        return lua_error(L);
    }
    lua_call(L, 0, 1);
    lua_pushinteger(L, 1);
    lua_pushcclosure(L, bootstrap_next, 2);
    lua_call(L, 0, 0);
    return 0;
}

static void fetch_bootstrap(lua_State *L)
{
    const char *url = get_global_string(L, "EMBOOTSTRAP");
    lua_pushcfunction(L, on_bootstrap_fetched);
    R_lua_fetch(L, url, NULL);
}

#endif


int main(int argc, char **argv)
{
    lua_State *L = R_lua_state_new();
    lua_pushliteral(L, "main.lua");
    lua_setglobal(L, "MAIN");

#ifdef __EMSCRIPTEN__
    lua_pushliteral(L, "embootstrap.lua");
    lua_setglobal(L, "EMBOOTSTRAP");
#endif

    parse_args(L, argc, argv);

#ifdef __EMSCRIPTEN__
    fetch_bootstrap(L);
#else
    run_main(L);
    R_lua_state_free(L);
#endif

    return 0;
}
