/*
 * resource.h - storing and retrieving of loadable thingies.
 *
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

/*
 * A resource, which is essentially just a thingy (like a `void *`) that you
 * can give an id that you can later recall it by. You can also give it a type
 * identifier, to make sure you're not recalling bogus. You'd probably want to
 * use this for preloadable things, like models, textures and even shaders.
 */


/*
 * Make a brand new resource and remember its id. That id can't be `NULL`.
 * The `on_free` function for clearing your user data is optional, but I'm
 * pretty sure if you got something to load, you also got something to free.
 */
void R_resource_add(const char *id, int type, R_UserData user,
                    void (*on_free)(R_UserData));


/* Finds a resource by its name, returns `NULL` if it doesn't exist. */
R_UserData *R_resource_search(const char *id);
/* Same thing, but calls `R_die` if it doesn't exist. */
R_UserData R_resource_find(const char *id);
/* Ensure a resource has the given type, or otherwise `R_die` over it. */
R_UserData R_resource_find_with_type(const char *id, int type,
                                     const char *type_name);

/* Remove and free single resource by its id. Returns if that id existed. */
bool R_resource_remove(const char *id);
/*
 * Remove and free all resources that match the given predicate. Returns the
 * number of ressources removed. If you choose your ids sensibly, you can use
 * this function to e.g. clear per-scene resources and such. A `NULL` predicate
 * just clears everything - which is what `R_resources_clear` actually does.
 */
int R_resource_remove_where(
        bool (*predicate)(const char *, int, R_UserData, void *), void *user);

int R_resource_clear(void);
