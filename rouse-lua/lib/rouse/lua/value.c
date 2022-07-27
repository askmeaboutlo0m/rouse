#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"

static lua_State *get_main_state(lua_State *L)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
    lua_State *main_state = lua_tothread(L, -1);
    lua_pop(L, 1);
    return main_state;
}

R_LuaValue *R_lua_value_new(lua_State *L, int index)
{
    R_LuaValue *lv = R_NEW_INIT_STRUCT(lv, R_LuaValue,
            R_MAGIC_INIT(R_LuaValue) get_main_state(L), R_lua_reg_at(L, index));
    R_MAGIC_CHECK(R_LuaValue, lv);
    return lv;
}

void R_lua_value_free(R_LuaValue *lv)
{
    if (lv) {
        R_MAGIC_CHECK(R_LuaValue, lv);
        R_lua_unreg(lv->L, lv->reg);
        free(lv);
    }
}
