#include <rouse.h>
#include "lua_inc.h"
#include "intern.h"
#include "util.h"

#ifdef __EMSCRIPTEN__

#include <emscripten/fetch.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


typedef struct R_LuaFetch {
    lua_State *L;
    int       path_reg;
    int       on_progress_reg;
    int       on_done_reg;
} R_LuaFetch;

static R_LuaFetch *fetch_new(lua_State *L, const char *path)
{
    int on_done_reg     = R_lua_reg(L);
    int on_progress_reg = R_lua_reg(L);

    lua_pushstring(L, path);
    int path_reg = R_lua_reg(L);

    R_LuaFetch *lf = R_NEW_INIT_STRUCT(lf, R_LuaFetch,
            L, path_reg, on_progress_reg, on_done_reg);

    return lf;
}

static void fetch_free(R_LuaFetch *lf)
{
    if (lf) {
        R_lua_unreg(lf->L, lf->path_reg);
        R_lua_unreg(lf->L, lf->on_progress_reg);
        R_lua_unreg(lf->L, lf->on_done_reg);
        free(lf);
    }
}


static void make_parent_dirs(const char *path)
{
    char *work  = R_strdup(path);
    char *start = work;
    while (*start == '/') {
        ++start;
    }

    for (char *end = start; *end; ++end) {
        if (*end == '/' && start != end) {
            *end = '\0';
            int error = mkdir(start, 0700);
            if (error && errno != EEXIST) {
                R_warn("mkdir '%s': %s", start, strerror(errno));
            }
            *end = '/';
        }
    }

    free(work);
}

static void spew(const char *path, const void *data, size_t nbytes)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        R_warn("Can't open '%s': %s", path, strerror(errno));
        return;
    }

    size_t written = fwrite(data, 1, nbytes, fp);
    if (written != nbytes) {
        R_warn("Wrote %zu bytes instead of the expected %zu bytes",
               written, nbytes);
    }

    if (fclose(fp)) {
        R_warn("Can't close '%s': %s", path, strerror(errno));
    }
}

static int write_fetched(lua_State *L)
{
    size_t     len;
    const char *path = lua_tolstring(L, 1, &len);
    if (path && len > 0) {
        emscripten_fetch_t *fetch = lua_touserdata(L, 2);
        make_parent_dirs(path);
        spew(path, fetch->data, (size_t) fetch->numBytes);
    }
    return 0;
}


static void fetch_done(emscripten_fetch_t *fetch)
{
    R_LuaFetch *lf = fetch->userData;
    lua_State  *L  = lf->L;
    R_lua_getreg(L, lf->on_done_reg);
    R_lua_getreg(L, lf->path_reg);
    lua_pushlightuserdata(L, fetch);
    fetch_free(lf);

    if (R_lua_pcall(L, 2, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
    }

    emscripten_fetch_close(fetch);
}


static void fetch_success(emscripten_fetch_t *fetch) {
    R_LuaFetch *lf = fetch->userData;
    lua_State  *L  = lf->L;
    lua_pushcfunction(L, write_fetched);
    R_lua_getreg(L, lf->path_reg);
    lua_pushlightuserdata(L, fetch);

    if (R_lua_pcall(L, 2, 0)) {
        R_LUA_ERROR_TO_WARNING(L);
    }

    fetch_done(fetch);
}

static void fetch_progress(emscripten_fetch_t *fetch)
{
    R_LuaFetch *lf = fetch->userData;
    if (lf->on_progress_reg != LUA_REFNIL) {
        lua_State *L = lf->L;
        R_lua_getreg(L, lf->on_progress_reg);

        uint64_t progress = fetch->dataOffset + fetch->numBytes;
        lua_pushinteger(L, (lua_Integer) progress);

        if (R_lua_pcall(L, 1, 0)) {
            R_LUA_ERROR_TO_WARNING(L);
        }
    }
}

static void fetch_error(emscripten_fetch_t *fetch) {
    fetch_done(fetch);
}


void R_lua_fetch(lua_State *L, const char *url, const char *path)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.userData   = fetch_new(L, path);
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess  = fetch_success;
    attr.onprogress = fetch_progress;
    attr.onerror    = fetch_error;
    emscripten_fetch(&attr, url);
}

#else

void R_lua_fetch(lua_State *L, R_UNUSED const char *url,
                 R_UNUSED const char *path)
{
    R_LUA_DIE(L, "Fetching files only implemented on Emscripten");
}

#endif
