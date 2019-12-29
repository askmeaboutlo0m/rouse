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


#define MAIN_PREFIX "--main="

static void parse_arg(lua_State *L, int i, const char *arg)
{
    size_t prefix_len = strlen(MAIN_PREFIX);
    if (strncmp(MAIN_PREFIX, arg, prefix_len) == 0) {
        lua_pushstring(L, arg + prefix_len);
        lua_setglobal(L, "MAIN");
    }
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


int lrouse_main(int argc, char **argv)
{
    lua_State *L = R_lua_state_new();
    lua_pushliteral(L, "main.lua");
    lua_setglobal(L, "MAIN");

    parse_args(L, argc, argv);

    lua_getglobal(L, "MAIN");
    const char *path = lua_tostring(L, -1);
    lua_pop(L, 1);

    if (luaL_loadfile(L, path) || lua_pcall(L, 0, 1, 0)) {
        const char *error = lua_tostring(L, -1);
        R_die("Error running '%s': %s", path, error);
    }

    R_lua_main(L);

#ifndef __EMSCRIPTEN__
    R_lua_state_free(L);
#endif
    return 0;
}
