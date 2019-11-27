/*
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
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <rouse_config.h>
#include "common.h"
#include "resource.h"
#include "3rdparty/uthash_inc.h"


typedef struct R_Resource {
    /* The id or name for this resource. Strdup'd on construction. */
    char *id;
    /* Numerical resource type identifier, for ensuring you got the right
     * type at hand. Negative type ids are reserved for rouse itself. To
     * get a unique id, consider md5ing your type name or something. */
    int type;
    /* Your user data, which is the actual resource. */
    R_UserData user;
    /* Free callback, use this to free your user data (and only that!) */
    void (*on_free)(R_UserData);
    /* Don't you dare mess with this, it's for bookkeeping. */
    UT_hash_handle hh;
} R_Resource;


static R_Resource *resources = NULL;


void R_resource_add(const char *id, int type, R_UserData user,
                    void (*on_free)(R_UserData))
{
    if (R_resource_search(id)) {
        R_die("Duplicate resource id: '%s'", id);
    }
    R_Resource *res = R_NEW_INIT_STRUCT(res, R_Resource,
            R_strdup(id), type, user, on_free, R_NULL_UT_HH);
    HASH_ADD_KEYPTR(hh, resources, res->id, strlen(res->id), res);
}


static R_Resource *search_resource(const char *id)
{
    R_Resource *res;
    HASH_FIND_STR(resources, id, res);
    return res;
}

static R_Resource *find_resource(const char *id)
{
    R_Resource *res = search_resource(id);
    if (!res) {
        R_die("Resource id not found: '%s'", id);
    }
    return res;
}

R_UserData *R_resource_search(const char *id)
{
    R_Resource *res = search_resource(id);
    return res ? &res->user : NULL;
}

R_UserData R_resource_find(const char *id)
{
    return find_resource(id)->user;
}

R_UserData R_resource_find_with_type(const char *id, int type,
                                     const char *type_name)
{
    R_Resource *res = find_resource(id);
    if (res->type != type) {
        R_die("Resource '%s' has wrong type '%d', expected '%d' (%s)",
              res->id, res->type, type, type_name ? type_name : "");
    }
    return res->user;
}


static void free_resource(R_Resource *res)
{
    if (res) {
        /* clang-tidy reports a use-after-free, but that's a false positive. */
        /* See https://github.com/troydhanson/uthash/issues/128 */
        /* NOLINTNEXTLINE(clang-analyzer-unix.Malloc) */
        HASH_DEL(resources, res);
        if (res->on_free) {
            res->on_free(res->user);
        }
        free(res->id);
        free(res);
    }
}

bool R_resource_remove(const char *id)
{
    R_Resource *res = search_resource(id);
    if (res) {
        free_resource(res);
        return true;
    }
    else {
        return false;
    }
}

int R_resource_remove_where(
        bool (*predicate)(const char *, int, R_UserData, void *), void *user)
{
    R_Resource *res, *tmp;
    int        removed = 0;
    HASH_ITER(hh, resources, res, tmp) {
        if (!predicate || predicate(res->id, res->type, res->user, user)) {
            free_resource(res);
            ++removed;
        }
    }
    return removed;
}

int R_resource_clear(void)
{
    return R_resource_remove_where(NULL, NULL);
}
