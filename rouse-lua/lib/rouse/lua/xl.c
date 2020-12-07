#include "lua_inc.h"
#include <stdint.h>
#include <string.h>
#include "xl.h"


int XL_checkbool(lua_State *L, int index)
{
    luaL_checkany(L, index);
    return lua_toboolean(L, index);
}

short XL_checkshort(lua_State *L, int index)
{
    return (short) luaL_checkinteger(L, index);
}

int XL_checkint(lua_State *L, int index)
{
    return (int) luaL_checkinteger(L, index);
}

float XL_checkfloat(lua_State *L, int index)
{
    return (float) luaL_checknumber(L, index);
}

unsigned int XL_checkuint(lua_State *L, int index)
{
    return (unsigned int) luaL_checkinteger(L, index);
}

unsigned char XL_checkuchar(lua_State *L, int index)
{
    return (unsigned char) luaL_checkinteger(L, index);
}

unsigned short XL_checkushort(lua_State *L, int index)
{
    return (unsigned short) luaL_checkinteger(L, index);
}

uint8_t XL_checkuint8(lua_State *L, int index)
{
    return (uint8_t) luaL_checkinteger(L, index);
}

int32_t XL_checkint32(lua_State *L, int index)
{
    return (int32_t) luaL_checkinteger(L, index);
}

uint32_t XL_checkuint32(lua_State *L, int index)
{
    return (uint32_t) luaL_checkinteger(L, index);
}

void *XL_checkpptype_nullable(lua_State *L, int index, const char *tname)
{
    void **pp = luaL_checkudata(L, index, tname);
    return pp ? *pp : NULL;
}

void *XL_checkpptype(lua_State *L, int index, const char *tname)
{
    void *ptr = XL_checkpptype_nullable(L, index, tname);
    if (!ptr) {
        luaL_error(L, "%s at index %d is NULL", tname, index);
    }
    return ptr;
}

void *XL_checkutype(lua_State *L, int index, const char *tname)
{
    void *ptr = luaL_checkudata(L, index, tname);
    if (!ptr) {
        luaL_error(L, "%s at index %d is NULL", tname, index);
    }
    return ptr;
}


void XL_pushshort(lua_State *L, short value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void XL_pushint(lua_State *L, int value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void XL_pushfloat(lua_State *L, float value)
{
    lua_pushnumber(L, (lua_Number) value);
}

void XL_pushuint(lua_State *L, unsigned int value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void XL_pushuchar(lua_State *L, unsigned char value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void XL_pushushort(lua_State *L, unsigned short value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void XL_pushuint8(lua_State *L, uint8_t value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void XL_pushint32(lua_State *L, int32_t value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void XL_pushuint32(lua_State *L, uint32_t value)
{
    lua_pushinteger(L, (lua_Integer) value);
}

void **XL_pushnewpptypeuv(lua_State *L, void *value,
                          const char *tname, int nuvalue)
{
    void **pp = lua_newuserdatauv(L, sizeof(*pp), nuvalue);
    *pp = value;
    luaL_setmetatable(L, tname);
    return pp;
}

void **XL_pushnewpptypeuv_nullable(lua_State *L, void *value,
                                   const char *tname, int nuvalue)
{
    if (value) {
        return XL_pushnewpptypeuv(L, value, tname, nuvalue);
    }
    else {
        lua_pushnil(L);
        return NULL;
    }
}

void *XL_pushnewutypeuv(lua_State *L, const void *value,
                        size_t size, const char *tname,
                        int nuvalue)
{
    void *u = memcpy(lua_newuserdatauv(L, size, nuvalue), value, size);
    luaL_setmetatable(L, tname);
    return u;
}


void XL_clearpptypeuv(lua_State *L, int index, int nuvalue)
{
    void **pp = lua_touserdata(L, index);
    if (pp) {
        *pp = NULL;

        int absindex = lua_absindex(L, index);
        for (int i = 1; i <= nuvalue; ++i) {
            lua_pushnil(L);
            XL_setiuservalue(L, absindex, i);
        }
    }
}


void XL_getiuservalue(lua_State *L, int index, int n)
{
    if (lua_getiuservalue(L, index, n) == LUA_TNONE) {
        luaL_error(L, "Can't get nonexistent uservalue %d", n);
    }
}

void XL_setiuservalue(lua_State *L, int index, int n)
{
    if (lua_setiuservalue(L, index, n) == 0) {
        luaL_error(L, "Can't set nonexistent uservalue %d", n);
    }
}


static int index_metatable(lua_State *L, int selfindex, int keyindex)
{
    lua_getmetatable(L, selfindex);
    lua_pushvalue(L, keyindex);
    lua_rawget(L, -2);
    return 1;
}

int XL_index_fallback(lua_State *L, const char *tname,
                      int selfindex, int keyindex)
{
    luaL_checkudata(L, selfindex, tname);
    luaL_checkany(L, keyindex);
    return index_metatable(L, selfindex, keyindex);
}

int XL_index(lua_State *L, const char *tname, void *reg,
             int selfindex, int keyindex)
{
    luaL_checkudata(L, selfindex, tname);
    luaL_checktype(L, keyindex, LUA_TSTRING);
    lua_rawgetp(L, LUA_REGISTRYINDEX, reg);
    lua_pushvalue(L, keyindex);

    if (lua_rawget(L, -2)) {
        lua_pushvalue(L, selfindex);
        lua_call(L, 1, 1);
        return 1;
    }
    else {
        lua_pop(L, 2);
        return index_metatable(L, selfindex, keyindex);
    }
}


int XL_newindex(lua_State *L, const char *tname, void *reg,
                int selfindex, int keyindex, int valueindex)
{
    luaL_checkudata(L, selfindex, tname);
    luaL_checktype(L, keyindex, LUA_TSTRING);
    luaL_checkany(L, valueindex);
    lua_rawgetp(L, LUA_REGISTRYINDEX, reg);
    lua_pushvalue(L, keyindex);

    if (lua_rawget(L, -2)) {
        lua_pushvalue(L, selfindex);
        lua_pushvalue(L, valueindex);
        lua_call(L, 2, 0);
        return 0;
    }
    else {
        const char *key = lua_tostring(L, keyindex);
        return luaL_error(L, "%s has no field '%s'", tname, key);
    }
}


int XL_staticindex(lua_State *L, void *reg, int keyindex)
{
    luaL_checktype(L, keyindex, LUA_TSTRING);
    lua_rawgetp(L, LUA_REGISTRYINDEX, reg);
    lua_pushvalue(L, keyindex);

    if (lua_rawget(L, -2)) {
        lua_call(L, 0, 1);
        return 1;
    }
    else {
        return 0;
    }
}

int XL_staticnewindex(lua_State *L, void *reg, int selfindex,
                      int keyindex, int valueindex)
{
    luaL_checktype(L, keyindex, LUA_TSTRING);
    luaL_checkany(L, valueindex);
    lua_rawgetp(L, LUA_REGISTRYINDEX, reg);
    lua_pushvalue(L, keyindex);

    if (lua_rawget(L, -2)) {
        lua_pushvalue(L, valueindex);
        lua_call(L, 1, 0);
    }
    else {
        lua_pushvalue(L, keyindex);
        lua_pushvalue(L, valueindex);
        lua_rawset(L, selfindex);
    }

    return 0;
}


void XL_initmetatable(lua_State *L, const char *tname, luaL_Reg *methods)
{
    luaL_newmetatable(L, tname);
    if (methods) {
        luaL_setfuncs(L, methods, 0);
    }
    lua_pop(L, 1);
}

void XL_initindextable(lua_State *L, void *key, luaL_Reg *methods)
{
    lua_newtable(L);
    luaL_setfuncs(L, methods, 0);
    lua_rawsetp(L, LUA_REGISTRYINDEX, key);
}

void XL_initnewindextable(lua_State *L, void *key, luaL_Reg *methods)
{
    XL_initindextable(L, key, methods);
}


static void getnexttable(lua_State *L, int top, const char *key)
{
    lua_pushstring(L, key);
    int type = lua_rawget(L, top);
    if (type == LUA_TNIL) {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_pushstring(L, key);
        lua_pushvalue(L, -2);
        lua_rawset(L, top);
    }
    else if (type != LUA_TTABLE) {
        luaL_error(L, "Expected '%s' to be a table, but it's %s",
                   key, luaL_typename(L, -1));
    }
    lua_remove(L, top);
}

static void getpackage(lua_State *L, va_list ap)
{
    lua_pushglobaltable(L);
    int top = lua_gettop(L);
    const char *key;
    while ((key = va_arg(ap, const char *))) {
        getnexttable(L, top, key);
    }
}


void XL_initfunctions(lua_State *L, luaL_Reg *functions, ...)
{
    va_list ap;
    va_start(ap, functions);
    getpackage(L, ap);
    va_end(ap);
    luaL_setfuncs(L, functions, 0);
    lua_pop(L, 1);
}


static void setenum(lua_State *L, XL_EnumEntry *entries)
{
    for (XL_EnumEntry *entry = entries; entry->key; ++entry) {
        lua_pushinteger(L, entry->value);
        lua_setfield(L, -2, entry->key);
        lua_pushstring(L, entry->key);
        lua_seti(L, -2, entry->value);
    }
}

void XL_initenum(lua_State *L, XL_EnumEntry *entries, ...)
{
    va_list ap;
    va_start(ap, entries);
    getpackage(L, ap);
    va_end(ap);
    setenum(L, entries);
    lua_pop(L, 1);
}


void XL_initstaticmetatable(lua_State *L, ...)
{
    va_list ap;
    va_start(ap, L);
    getpackage(L, ap);
    va_end(ap);
    lua_pushvalue(L, -1);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
}


int XL_setfromtable(lua_State *L, const char *tname,
                    int selfindex, int tableindex)
{
    luaL_checkudata(L, selfindex, tname);
    luaL_checktype(L, tableindex, LUA_TTABLE);

    lua_pushnil(L);
    while (lua_next(L, tableindex)) {
        lua_pushvalue(L, -2);
        lua_insert(L, lua_absindex(L, -2));
        lua_settable(L, selfindex);
    }

    lua_pushvalue(L, selfindex);
    return 1;
}
