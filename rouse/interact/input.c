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
    R_Input *in = R_NEW_INIT_STRUCT(in, R_Input, R_MAGIC_INIT(in) NULL);
    R_MAGIC_CHECK(in);
    return in;
}

void R_input_clear(R_Input *in)
{
    R_MAGIC_CHECK(in);
    R_input_key_clear(in);
}

void R_input_free(R_Input *in)
{
    if (in) {
        R_MAGIC_CHECK(in);
        R_input_clear(in);
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
        if (kb->on_free) {
            kb->on_free(kb->user);
        }
        free(kb);
    }
}

void R_input_key_bind(R_Input *in, unsigned int key, R_InputKeyFn fn,
                      R_UserData user, R_InputFreeFn on_free)
{
    R_MAGIC_CHECK(in);
    R_KeyBind *kb = R_NEW_INIT_STRUCT(kb, R_KeyBind, R_MAGIC_INIT(kb)
            key, fn, user, on_free, R_NULL_UT_HH);

    R_KeyBind *old;
    HASH_REPLACE(hh, in->key_binds, key, sizeof(kb->key), kb, old);
    free(old);
}

bool R_input_key_unbind(R_Input *in, unsigned int key)
{
    R_MAGIC_CHECK(in);
    R_KeyBind *kb = search_key_bind(in, key);
    if (kb) {
        R_MAGIC_CHECK(kb);
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
    R_MAGIC_CHECK(in);
    R_HASH_CLEAR(R_KeyBind, in->key_binds, free_key_bind);
}


static R_InputHandled on_key(R_Input *in, SDL_KeyboardEvent *key,
                             R_UserData context)
{
    R_KeyBind *kb = search_key_bind(in, key->keysym.scancode);
    if (kb) {
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
    R_MAGIC_CHECK(in);
    switch (event->type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return on_key(in, &event->key, context);
        default:
            return R_INPUT_UNHANDLED;
    }
}
