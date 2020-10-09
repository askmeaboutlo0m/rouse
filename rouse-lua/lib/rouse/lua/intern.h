#include "xl.h"


#define R_LUA_VALUE_UNPACK(EXPR, OUT_LUA_VALUE, OUT_LUA_STATE) do { \
        (OUT_LUA_VALUE) = (EXPR); \
        R_MAGIC_CHECK(R_LuaValue, (OUT_LUA_VALUE)); \
        (OUT_LUA_STATE) = (OUT_LUA_VALUE)->L; \
    } while (0)

#define R_LUA_VALUE_DECLARE_UNPACK(EXPR, LUA_VALUE_VAR, LUA_STATE_VAR) \
    R_LuaValue *LUA_VALUE_VAR; \
    lua_State  *LUA_STATE_VAR; \
    R_LUA_VALUE_UNPACK(EXPR, LUA_VALUE_VAR, LUA_STATE_VAR)

typedef struct R_LuaValue {
    R_MAGIC_FIELD
    lua_State *L;
    int       reg;
} R_LuaValue;

R_LuaValue *R_lua_value_new(lua_State *L, int index);

void R_lua_value_free(R_LuaValue *lv);


typedef struct R_LuaNvgTransform {
    float matrix[6];
} R_LuaNvgTransform;


int R_lua_msgh(lua_State *L);

R_EaseFn R_lua_named_ease(lua_State *L, int name_index);


#define R_LUA_INIT_LIST(X) \
    X(R_lua_al_init) \
    X(R_lua_bitmap_image_init) \
    X(R_lua_double_init) \
    X(R_lua_ease_init) \
    X(R_lua_frame_buffer_init) \
    X(R_lua_frame_renderer_init) \
    X(R_lua_geom_init) \
    X(R_lua_gl_init) \
    X(R_lua_json_init) \
    X(R_lua_misc_init) \
    X(R_lua_nvg_init) \
    X(R_lua_scene_init) \
    X(R_lua_sdl_init) \
    X(R_lua_seq_init) \
    X(R_lua_sample_init) \
    X(R_lua_sprite_init) \
    X(R_lua_text_field_init) \
    X(R_lua_vector_image_init) \
    X(R_lua_viewport_init)

#define R_LUA_INIT_DECL(NAME) int NAME(lua_State *L);

R_LUA_INIT_LIST(R_LUA_INIT_DECL)
