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
#include <stdbool.h>
#include <stdnoreturn.h>
#include <SDL2/SDL.h>
#include "../common.h"
#include "../3rdparty/uthash_inc.h"
#include "input.h"


typedef struct R_KeyBind {
    R_MAGIC_FIELD
    unsigned int   key;
    R_InputKeyFn   fn;
    R_UserData     user;
    R_InputFreeFn  on_free;
    UT_hash_handle hh;
} R_KeyBind;

struct R_Input {
    R_MAGIC_FIELD
    R_KeyBind *key_binds;
};


R_Input *R_input_new(void)
{
    R_Input *in = R_NEW_INIT_STRUCT(in, R_Input, R_MAGIC_INIT(R_Input) NULL);
    R_MAGIC_CHECK(R_Input, in);
    return in;
}

void R_input_clear(R_Input *in)
{
    R_MAGIC_CHECK(R_Input, in);
    R_input_key_clear(in);
}

void R_input_free(R_Input *in)
{
    if (in) {
        R_input_clear(in);
        R_MAGIC_POISON(R_Input, in);
        free(in);
    }
}


static R_KeyBind *search_key_bind(R_Input *in, unsigned int key)
{
    R_KeyBind *kb;
    HASH_FIND(hh, in->key_binds, &key, sizeof(key), kb);
    return kb;
}

static void free_key_bind(R_KeyBind *kb)
{
    if (kb) {
        R_MAGIC_CHECK(R_KeyBind, kb);
        if (kb->on_free) {
            kb->on_free(kb->user);
        }
        R_MAGIC_POISON(R_KeyBind, kb);
        free(kb);
    }
}

void R_input_key_bind(R_Input *in, unsigned int key, R_InputKeyFn fn,
                      R_UserData user, R_InputFreeFn on_free)
{
    R_MAGIC_CHECK(R_Input, in);
    R_KeyBind *kb = R_NEW_INIT_STRUCT(kb, R_KeyBind, R_MAGIC_INIT(R_KeyBind)
            key, fn, user, on_free, R_NULL_UT_HH);

    R_KeyBind *old;
    HASH_REPLACE(hh, in->key_binds, key, sizeof(kb->key), kb, old);
    free(old);
}

bool R_input_key_unbind(R_Input *in, unsigned int key)
{
    R_MAGIC_CHECK(R_Input, in);
    R_KeyBind *kb = search_key_bind(in, key);
    if (kb) {
        R_MAGIC_CHECK(R_KeyBind, kb);
        HASH_DELETE(hh, in->key_binds, kb);
        free_key_bind(kb);
        return true;
    }
    else {
        return false;
    }
}

void R_input_key_clear(R_Input *in)
{
    R_MAGIC_CHECK(R_Input, in);
    /* clang-tidy reports a use-after-free, but that's a false positive. */
    /* See https://github.com/troydhanson/uthash/issues/128 */
    /* NOLINTNEXTLINE(clang-analyzer-unix.Malloc) */
    R_HASH_CLEAR(R_KeyBind, in->key_binds, free_key_bind);
}


static R_InputHandled on_key(R_Input *in, SDL_KeyboardEvent *key,
                             R_UserData context)
{
    R_KeyBind *kb = search_key_bind(in, key->keysym.scancode);
    if (kb) {
        R_MAGIC_CHECK(R_KeyBind, kb);
        kb->fn(key, context, kb->user);
        return R_INPUT_BOUND_KEY;
    }
    else {
        return R_INPUT_UNBOUND_KEY;
    }
}

R_InputHandled R_input_handle_event(R_Input *in, SDL_Event *event,
                                    R_UserData context)
{
    R_MAGIC_CHECK(R_Input, in);
    switch (event->type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return on_key(in, &event->key, context);
        default:
            return R_INPUT_UNHANDLED;
    }
}
