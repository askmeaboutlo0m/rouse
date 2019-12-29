#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"

R_LuaValue *R_lua_value_new(lua_State *L, int index)
{
    R_LuaValue *lv = R_NEW_INIT_STRUCT(lv, R_LuaValue,
            R_MAGIC_INIT(R_LuaValue) L, R_lua_reg_at(L, index));
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
