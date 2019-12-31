#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"
#include "main.h"


typedef struct R_LuaMainArgs {
    lua_State *L;
    int       ref;
} R_LuaMainArgs;


#define R_LUA_INIT_REF(NAME) NAME,

static void init_modules(lua_State *L)
{
    static lua_CFunction init_funcs[] = {R_LUA_INIT_LIST(R_LUA_INIT_REF)};
    for (size_t i = 0; i < R_LENGTH(init_funcs); ++i) {
        lua_pushcfunction(L, init_funcs[i]);
        lua_call(L, 0, 0);
    }
}

static int init(lua_State *L)
{
    R_LUA_CHECK_ARGC(L, 0);
    lua_newtable(L);
    lua_pushglobaltable(L);
    lua_pushstring(L, "R");
    lua_pushvalue(L, -3);
    lua_rawset(L, -3);
    lua_pop(L, 1);
    init_modules(L);
    return 0;
}


int R_lua_msgh(lua_State *L)
{
    if (lua_isstring(L, 1)) {
        const char *msg = lua_tostring(L, 1);
        luaL_traceback(L, L, msg, 1);
    }
    return 1;
}


int R_lua_scene_new(lua_State *L);

static int protected_on_scene(lua_State *L)
{
    R_LUA_CHECK_ARGC(L, 1);
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_pushcfunction(L, R_lua_scene_new);

    if (!lua_getfield(L, 1, "on_scene")) {
        R_debug("no on_scene callback given");
        return 0;
    }

    lua_call(L, 1, 1);
    return 1;
}

static R_Scene *on_scene(void *user)
{
    R_LuaMainArgs *largs = user;
    lua_State     *L     = largs->L;

    lua_pushcfunction(L, init);
    if (R_lua_pcall(L, 0, 0)) {
        R_die("Can't initialize Lua bindings: %s", R_LUA_ERROR_GET(L));
    }

    lua_pushcfunction(L, protected_on_scene);
    R_lua_getreg(L, largs->ref);

    if (R_lua_pcall(L, 1, 1)) {
        R_LUA_ERROR_TO_WARNING(L);
        return NULL;
    }

    R_Scene *scene = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return scene;
}


static void set_int_arg(lua_State *L, int index, const char *name, int *target)
{
    if (lua_getfield(L, index, name)) {
        int         success;
        lua_Integer samples = lua_tointegerx(L, -1, &success);
        if (success) {
            *target = R_lua_i2int(samples);
        }
        else {
            R_warn("%s argument is not an integer", name);
        }
    }
    lua_pop(L, 1);
}

static void set_string_arg(lua_State *L, int index, const char *name,
                           const char **target)
{
    if (lua_getfield(L, index, name)) {
        if (lua_isstring(L, -1)) {
            /* Just store the string in the registry. It won't ever be */
            /* collected this way, but we can spare those few bytes.   */
            R_lua_reg_at(L, -1);
            *target = lua_tostring(L, -1);
        }
        else {
            R_warn("%s argument is not a string", name);
        }
    }
    lua_pop(L, 1);
}


static int unravel_sdl_gl_args(lua_State *L)
{
    R_SdlGlArgs *gl = lua_touserdata(L, 1);
#   define SET_GL_ARG(FIELD) set_int_arg(L, 2, #FIELD, &gl->FIELD)
    SET_GL_ARG(red_size);
    SET_GL_ARG(green_size);
    SET_GL_ARG(blue_size);
    SET_GL_ARG(alpha_size);
    SET_GL_ARG(buffer_size);
    SET_GL_ARG(doublebuffer);
    SET_GL_ARG(depth_size);
    SET_GL_ARG(stencil_size);
    SET_GL_ARG(accum_red_size);
    SET_GL_ARG(accum_green_size);
    SET_GL_ARG(accum_blue_size);
    SET_GL_ARG(accum_alpha_size);
    SET_GL_ARG(stereo);
    SET_GL_ARG(multisamplebuffers);
    SET_GL_ARG(multisamplesamples);
    SET_GL_ARG(accelerated_visual);
    SET_GL_ARG(context_major_version);
    SET_GL_ARG(context_minor_version);
    SET_GL_ARG(context_flags);
    SET_GL_ARG(context_profile_mask);
    SET_GL_ARG(share_with_current_context);
    SET_GL_ARG(framebuffer_srgb_capable);
    SET_GL_ARG(context_release_behavior);
#   undef SET_GL_ARG
    return 0;
}

static int unravel_window_args(lua_State *L)
{
    R_WindowArgs *window = lua_touserdata(L, 1);
    set_string_arg(L, 2, "title",  &window->title);
    set_int_arg   (L, 2, "x",      &window->x);
    set_int_arg   (L, 2, "y",      &window->y);
    set_int_arg   (L, 2, "width",  &window->width);
    set_int_arg   (L, 2, "height", &window->height);
    return 0;
}

static void unravel_subargs(lua_State *L, const char *key, void *subargs,
                            lua_CFunction unravel_fn)
{
    if (lua_getfield(L, 2, key)) {
        if (lua_istable(L, -1)) {
            lua_pushcfunction(L, unravel_fn);
            lua_pushlightuserdata(L, subargs);
            lua_pushvalue(L, -3);
            lua_call(L, 2, 0);
        }
        else {
            R_warn("%s argument is not a table", key);
        }
    }
    lua_pop(L, 1);
}

static int unravel_main_args(lua_State *L)
{
    R_MainArgs *args = lua_touserdata(L, 1);
    unravel_subargs(L, "gl",     &args->gl,     unravel_sdl_gl_args);
    unravel_subargs(L, "window", &args->window, unravel_window_args);
    return 0;
}

void R_lua_main(lua_State *L)
{
    luaL_checktype(L, -1, LUA_TTABLE);
    R_LuaMainArgs largs = {L, R_lua_reg_at(L, -1)};
    R_MainArgs    args  = R_main_args(on_scene, &largs);

    lua_pushcfunction(L, unravel_main_args);
    lua_pushlightuserdata(L, &args);
    lua_pushvalue(L, -3);
    if (R_lua_pcall(L, 2, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
    }

    R_main(&args);
    R_lua_unreg(L, largs.ref);
}
