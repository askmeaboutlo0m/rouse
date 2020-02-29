void R_lua_main(lua_State *L);

/*
 * Emscripten only! On any other platform this will just `lua_error`. You can
 * check if you're on Emscripten using `#ifdef __EMSCRIPTEN__`. In Lua, you can
 * use `R.get_platform() == "emscripten"`.
 *
 * Fetches the file from the given `url` and stores it at the given `path` if
 * the fetch succeeds. Parent directories will be created as needed. If `path`
 * is `NULL`, nothing is stored.
 *
 * You must pass a function at the top of the Lua stack which will be called
 * when the fetch is complete, successful or not. The first parameter of that
 * function will be the `path` given here and the second paramter will be the
 * `emscripten_fetch_t` that got done. You can figure out if there was an error
 * by inspecting the HTTP status code in there.
 *
 * The callback function is popped from the stack!
 */
void R_lua_fetch(lua_State *L, const char *url, const char *path);

void R_lua_fetch_pack(lua_State *L, const char *url);
