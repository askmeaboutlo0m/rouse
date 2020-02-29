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


#define OPEN_LUA_LIB(L, NAME, FN) do { \
        luaL_requiref(L, NAME, FN, 1); \
        lua_pop(L, 1); \
    } while (0)

static void open_lua_libs(lua_State *L)
{
    OPEN_LUA_LIB(L, "_G",            luaopen_base     );
    OPEN_LUA_LIB(L, LUA_LOADLIBNAME, luaopen_package  );
    OPEN_LUA_LIB(L, LUA_COLIBNAME,   luaopen_coroutine);
    OPEN_LUA_LIB(L, LUA_TABLIBNAME,  luaopen_table    );
    OPEN_LUA_LIB(L, LUA_IOLIBNAME,   luaopen_io       );
    OPEN_LUA_LIB(L, LUA_OSLIBNAME,   luaopen_os       );
    OPEN_LUA_LIB(L, LUA_STRLIBNAME,  luaopen_string   );
    OPEN_LUA_LIB(L, LUA_MATHLIBNAME, luaopen_math     );
    OPEN_LUA_LIB(L, LUA_UTF8LIBNAME, luaopen_utf8     );
#ifdef ROUSE_DEBUG
    OPEN_LUA_LIB(L, LUA_DBLIBNAME,   luaopen_debug    );
#endif
}

static int unsupported_function(lua_State *L)
{
    const char *lib = lua_tostring(L, lua_upvalueindex(1));
    const char *fn  = lua_tostring(L, lua_upvalueindex(2));
    return luaL_error(L, "Calling %s.%s is not allowed", lib, fn);
}

static void disallow_function_in(lua_State *L, const char *lib, const char *fn)
{
    lua_pushstring(L, lib);
    lua_pushstring(L, fn);
    lua_pushcclosure(L, unsupported_function, 2);
    lua_setfield(L, -2, fn);
}

static void disallow_functions(lua_State *L, const char *lib, ...)
{
    va_list ap;
    va_start(ap, lib);
    lua_getglobal(L, lib);

    const char *fn;
    while ((fn = va_arg(ap, const char *))) {
        disallow_function_in(L, lib, fn);
    }

    lua_pop(L, 1);
    va_end(ap);
}

static int call_wrapped(lua_State *L)
{
    int argc = lua_gettop(L);
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_insert(L, 1);
    lua_call(L, argc, LUA_MULTRET);
    return lua_gettop(L);
}

static int wrapped_io_open(lua_State *L)
{
    size_t     len;
    const char *mode = lua_tolstring(L, 2, &len);
    if (mode && ((len >= 1 && (mode[0] == 'w' || mode[0] == 'a'))
              || (len >= 2 && (mode[0] == 'r' && mode[1] == '+')))) {
        return luaL_error(L, "Calling io.open in write, append "
                             "or update mode is not allowed");
    }
    else {
        return call_wrapped(L);
    }
}

static int wrapped_io_output(lua_State *L)
{
    if (lua_isnoneornil(L, 1)) {
        return call_wrapped(L);
    }
    else {
        return luaL_error(L, "Calling io.output(file) is not allowed");
    }
}

static void wrap_function(lua_State *L, const char *lib, const char *fn,
                          lua_CFunction wrapper)
{
    lua_getglobal(L, lib);
    lua_getfield(L, -1, fn);
    lua_pushcclosure(L, wrapper, 1);
    lua_setfield(L, -2, fn);
    lua_pop(L, 1);
}

static void disallow_questionable_features(lua_State *L)
{
    disallow_functions(L, "io", "popen", "tmpfile", (const char *)NULL);
    disallow_functions(L, "os", "execute", "remove", "rename", "setlocale",
                                "tmpname", (const char *)NULL);
    wrap_function(L, "io", "open",   wrapped_io_open  );
    wrap_function(L, "io", "output", wrapped_io_output);
}

static void seed_random(lua_State *L)
{
    int top = lua_gettop(L);
    lua_getglobal(L, "os");
    lua_getfield(L, -1, "time");
    lua_call(L, 0, 1);
    lua_getglobal(L, "math");
    lua_getfield(L, -1, "randomseed");
    lua_pushvalue(L, -3);
    lua_call(L, 1, 0);
    lua_settop(L, top);
}

static lua_State *init_lua(void)
{
    lua_State *L = luaL_newstate();
    if (!L) {
        R_die("Can't open Lua state");
    }
    open_lua_libs(L);
    disallow_questionable_features(L);
    seed_random(L);
    return L;
}


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

static int on_bootstrap_pack_fetched(lua_State *L)
{
    run_main(L);
    return 0;
}

static int bootstrap_next(lua_State *L)
{
    lua_Integer i = luaL_checkinteger(L, lua_upvalueindex(2));
    if (lua_geti(L, lua_upvalueindex(1), i)) {
        const char *path = luaL_checkstring(L, -1);
        lua_pushnil(L);
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

#define PACK_PREFIX "pack:"

static void fetch_bootstrap(lua_State *L)
{
    const char *url = get_global_string(L, "EMBOOTSTRAP");
    lua_pushnil(L);

    size_t prefix_len = strlen(PACK_PREFIX);
    if (strncmp(PACK_PREFIX, url, prefix_len) == 0) {
        lua_pushcfunction(L, on_bootstrap_pack_fetched);
        R_lua_fetch_pack(L, url + prefix_len);
    }
    else {
        lua_pushcfunction(L, on_bootstrap_fetched);
        R_lua_fetch(L, url, NULL);
    }
}

#endif


int main(int argc, char **argv)
{
    lua_State *L = init_lua();
    lua_pushliteral(L, "main.lua");
    lua_setglobal(L, "MAIN");

#ifdef __EMSCRIPTEN__
    lua_pushliteral(L, PACK_PREFIX "embootstrap.pack");
    lua_setglobal(L, "EMBOOTSTRAP");
#endif

    parse_args(L, argc, argv);

#ifdef __EMSCRIPTEN__
    fetch_bootstrap(L);
#else
    run_main(L);
    lua_close(L);
#endif

    return 0;
}
