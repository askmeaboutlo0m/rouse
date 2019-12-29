#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"

lua_State *R_lua_state_new(void)
{
    lua_State *L = luaL_newstate();
    if (!L) {
        R_die("Disastrous failure: can't open Lua state");
    }
    luaL_openlibs(L);
    return L;
}

void R_lua_state_free(lua_State *L)
{
    lua_close(L);
}
