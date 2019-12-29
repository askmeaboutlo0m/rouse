#ifdef ROUSE_MAGIC
#   define R_LUA_MAGIC_CHECK(L, TYPE, EXPR) do { \
            const TYPE *_magic_ptr = (EXPR); \
            if (_magic_ptr) { \
                uint32_t _expr_magic = _magic_ptr->MAGIC; \
                uint32_t _type_magic = R_MAGIC_OF(TYPE); \
                if (_expr_magic != _type_magic) { \
                    R_LUA_ERROR_FORMAT(L, "Bad %s magic for '%s': got 0x%x, " \
                                       "but should be 0x%x", #TYPE, #EXPR, \
                                       _expr_magic, _type_magic); \
                } \
            } \
            else { \
                R_LUA_ERROR_FORMAT(L, "Bad %s magic: '%s' is NULL", \
                                   #TYPE, #EXPR); \
            } \
        } while (0)
#else
#   define R_LUA_MAGIC_CHECK(L, TYPE, EXPR) R_NOOP()
#endif


#define R_LUA_ERROR_FORMAT(L, FORMAT, ...) do { \
        char *_errstr = R_format(FORMAT, __VA_ARGS__); \
        lua_pushstring(L, _errstr); \
        free(_errstr); \
        lua_error(L); \
    } while (0)

#define R_LUA_ERROR_GET(L) lua_tostring(L, -1)

#define R_LUA_ERROR_TO_WARNING(L) do { \
        R_warn("Lua error: %s", R_LUA_ERROR_GET(L)); \
        lua_pop(L, 1); \
    } while (0)


#define R_LUA_CHECK_ARGC_TOP(L, N, TOP) \
    luaL_argcheck(L, (TOP) == (N), (TOP), "need exactly " #N " argument(s)")

#define R_LUA_CHECK_ARGC_BETWEEN_TOP(L, MIN, MAX, TOP) \
        luaL_argcheck(L, (TOP) >= (MIN) && (TOP) <= (MAX), (TOP), \
                      "need between " #MIN " and " #MAX " argument(s)")

#define R_LUA_CHECK_ARGC(L, N) do { \
        int _top = lua_gettop(L); \
        R_LUA_CHECK_ARGC_TOP(L, N, _top); \
    } while (0)

#define R_LUA_CHECK_ARGC_BETWEEN(L, MIN, MAX) do { \
        int _top = lua_gettop(L); \
        R_LUA_CHECK_ARGC_BETWEEN_TOP(L, MIN, MAX, TOP); \
    } while (0)


static inline int R_lua_pcall(lua_State *L, int nargs, int nresults)
{
    int msgh = lua_gettop(L) - nargs - 1;
    lua_pushcfunction(L, R_lua_msgh);
    lua_insert(L, msgh);
    int error = lua_pcall(L, nargs, nresults, msgh);
    lua_remove(L, msgh);
    return error;
}


#define R_lua_reg(L)         luaL_ref   (L, LUA_REGISTRYINDEX)
#define R_lua_unreg(L, REF)  luaL_unref (L, LUA_REGISTRYINDEX, (REF))
#define R_lua_getreg(L, REF) lua_rawgeti(L, LUA_REGISTRYINDEX, (REF))

#define R_lua_reg_dup(L, INDEX) R_lua_reg_at(L, -1)

static inline int R_lua_reg_at(lua_State *L, int index)
{
    lua_pushvalue(L, index);
    return R_lua_reg(L);
}


static inline lua_Number R_lua_double2n(double x)
{
    return (lua_Number) x;
}

static inline lua_Number R_lua_float2n(float x)
{
    return (lua_Number) x;
}

static inline int R_lua_i2int(lua_Integer x)
{
    return (int) x;
}

static inline lua_Integer R_lua_int2i(int x)
{
    return (lua_Integer) x;
}

static inline float R_lua_n2float(lua_Number x)
{
    return (float) x;
}

static inline lua_Integer R_lua_size2i(size_t x)
{
    return (lua_Integer) x;
}

static inline lua_Integer R_lua_uint2i(unsigned int x)
{
    return (lua_Integer) x;
}

static inline lua_Integer R_lua_uint322i(uint32_t x)
{
    return (lua_Integer) x;
}

#define R_lua_check_int(L, INDEX) \
    R_lua_i2int(luaL_checkinteger(L, INDEX))

#define R_lua_check_float(L, INDEX) \
    R_lua_n2float(luaL_checknumber(L, INDEX))


#define R_lua_push_int(L, X)   lua_pushinteger(L, R_lua_int2i(X))
#define R_lua_push_uint(L, X)  lua_pushinteger(L, R_lua_uint2i(X))
#define R_lua_push_float(L, X) lua_pushnumber(L, R_lua_n2float(X))

static inline void R_lua_push_r_string(lua_State *L, R_String *string)
{
    if (string) {
        lua_pushlstring(L, R_string_body(string), R_string_len(string));
    }
    else {
        lua_pushnil(L);
    }
}


#define R_LUA_DEFINE_FIELD_FN(TYPE, NAME, FIELD, PUSH_TYPE) \
    static int NAME ## _ ## FIELD(lua_State *L) \
    { \
        TYPE *NAME = lua_touserdata(L, 1); \
        R_MAGIC_CHECK(TYPE, NAME); \
        R_lua_push_ ## PUSH_TYPE(L, NAME->FIELD); \
        return 1; \
    }

#define R_LUA_DEFINE_GETTER_FN(TYPE, NAME, FIELD, GETTER, PUSH_TYPE) \
    static int NAME ## _ ## FIELD(lua_State *L) \
    { \
        TYPE *NAME = lua_touserdata(L, 1); \
        R_lua_push_ ## PUSH_TYPE(L, GETTER(NAME)); \
        return 1; \
    }

#define R_LUA_DEFINE_FIELD_SET_FN(TYPE, NAME, FIELD, PUSH_TYPE) \
    static int NAME ## _ ## FIELD ## _set(lua_State *L) \
    { \
        TYPE *NAME  = lua_touserdata(L, 1); \
        NAME->FIELD = R_lua_check_ ## PUSH_TYPE(L, 2); \
        return 0; \
    }

static inline void R_lua_indextable_new(lua_State *L, const char *tname,
                                        luaL_Reg *fields)
{
    lua_pushstring(L, tname);
    lua_newtable(L);
    luaL_setfuncs(L, fields, 0);
    lua_settable(L, LUA_REGISTRYINDEX);
}

static inline bool R_lua_indextable_get(lua_State *L, void *subject,
                                        const char *tname, int kindex)
{
    lua_pushstring(L, tname);
    lua_rawget(L, LUA_REGISTRYINDEX);
    R_assert(lua_type(L, -1) == LUA_TTABLE, "indextable must be a table");
    lua_pushvalue(L, kindex);
    if (lua_rawget(L, -2)) {
        lua_pushlightuserdata(L, subject);
        lua_call(L, 1, 1);
        return true;
    }
    else {
        lua_pop(L, 2);
        return false;
    }
}

static inline void R_lua_indextable_set(lua_State *L, void *subject,
                                        const char *tname, int kindex,
                                        int vindex)
{
    lua_pushstring(L, tname);
    lua_rawget(L, LUA_REGISTRYINDEX);
    R_assert(lua_type(L, -1) == LUA_TTABLE, "indextable must be a table");
    lua_pushvalue(L, kindex);
    if (lua_rawget(L, -2)) {
        lua_pushlightuserdata(L, subject);
        lua_pushvalue(L, vindex);
        lua_call(L, 2, 0);
        lua_pop(L, 1);
    }
    else {
        luaL_error(L, "%s has no field '%s'", tname, lua_tostring(L, kindex));
    }
}

#define R_LUA_DEFINE_INDEX_FN(TYPE, NAME) \
    static int NAME ## _index(lua_State *L) \
    { \
        TYPE *NAME = R_lua_check_pp(L, 1, #TYPE); \
        if (!R_lua_indextable_get(L, NAME, #TYPE ".fields", 2)) { \
            lua_getmetatable(L, 1); \
            lua_pushvalue(L, 2); \
            lua_rawget(L, -2); \
        } \
        return 1; \
    }

#define R_LUA_DEFINE_NEWINDEX_FN(TYPE, NAME) \
    static int NAME ## _index(lua_State *L) \
    { \
        TYPE *NAME = R_lua_check_pp(L, 1, #TYPE); \
        R_lua_indextable_set(L, NAME, #TYPE ".newindex", 2); \
        return 0; \
    }


static inline void *R_lua_newuserdata(lua_State *L, size_t size,
                                      const char *tname)
{
    void *udata = lua_newuserdata(L, size);
    luaL_setmetatable(L, tname);
    return udata;
}

static inline void **R_lua_newuserdata_pp(lua_State *L, void *ptr,
                                          const char *tname)
{
    void **pp = R_lua_newuserdata(L, sizeof(*pp), tname);
    *pp = ptr;
    return pp;
}

static inline void *R_lua_check_pp(lua_State *L, int index, const char *tname)
{
    void **pp = luaL_checkudata(L, index, tname);
    return pp ? *pp : NULL;
}


static inline void R_lua_metatable_new(lua_State *L, const char *tname,
                                       bool indexed, luaL_Reg *funcs)
{
    luaL_newmetatable(L, tname);

    if (indexed) {
        lua_pushliteral(L, "__index");
        lua_pushvalue(L, -2);
        lua_settable(L, -3);
    }

    if (funcs) {
        luaL_setfuncs(L, funcs, 0);
    }

    lua_pop(L, 1);
}
