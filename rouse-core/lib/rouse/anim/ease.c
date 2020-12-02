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
 * ______________________________________________________________________________
 *
 * These easing functions are based on Robert Penner's easing functions, which
 * can be found at http://robertpenner.com/easing/. They're licensed under the
 * following terms:
 *
 * Copyright © 2001 Robert Penner
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
#include <string.h>
#include <assert.h>
#include <cglm/struct.h>
#include <rouse_config.h>
#include "../common.h"
#include "../geom.h"
#include "ease.h"


typedef struct R_EaseName R_EaseName;

struct R_EaseName {
    R_EaseFn   ease;
    R_EaseName *next;
    char       name[];
};

/*
 * Can't use a hash table for function pointers in ISO C, so a linked list it
 * is. Shouldn't be much of an issue, stringifying easings is a debug feature
 * that doesn't need to be fast or anything.
 */
static R_EaseName *ease_names;

const char *R_ease_name(R_EaseFn ease)
{
    for (R_EaseName *en = ease_names; en; en = en->next) {
        if (en->ease == ease) {
            return en->name;
        }
    }
    return NULL;
}

static R_EaseName *ease_name_new(R_EaseFn ease, const char *name,
                                 R_EaseName *next)
{
    size_t     size = strlen(name) + 1;
    R_EaseName *en  = R_malloc(sizeof(*en) + size);
    en->ease = ease;
    en->next = next;
    strncpy(en->name, name, size);
    return en;
}

static bool remove_name(R_EaseFn ease)
{
    R_EaseName **pp = &ease_names;
    for (R_EaseName *en = ease_names; en; pp = &en->next, en = en->next) {
        if (en->ease == ease) {
            *pp = en->next;
            free(en);
            return true;
        }
    }
    return false;
}

void R_ease_name_add(R_EaseFn ease, const char *name)
{
    R_assert_not_null(ease);
    R_assert_not_null(name);
    /* Since this is basically debug code, don't make this fatal. */
    if (ease && name) {
        remove_name(ease);
        ease_names = ease_name_new(ease, name, ease_names);
    }
}

bool R_ease_name_remove(R_EaseFn ease)
{
    return ease ? remove_name(ease) : false;
}


/* Called by `init` in main.c, intentionally not publicly in the header. */
void R_ease_init(void)
{
    R_EASE_NAME_ADD_FN(R_ease_linear);
    R_EASE_NAME_ADD_FN(R_ease_bounce_in);
    R_EASE_NAME_ADD_FN(R_ease_bounce_out);
    R_EASE_NAME_ADD_FN(R_ease_bounce_in_out);
    R_EASE_NAME_ADD_FN(R_ease_cubic_in);
    R_EASE_NAME_ADD_FN(R_ease_cubic_out);
    R_EASE_NAME_ADD_FN(R_ease_cubic_in_out);
    R_EASE_NAME_ADD_FN(R_ease_elastic_in);
    R_EASE_NAME_ADD_FN(R_ease_elastic_out);
    R_EASE_NAME_ADD_FN(R_ease_elastic_in_out);
    R_EASE_NAME_ADD_FN(R_ease_back_in);
    R_EASE_NAME_ADD_FN(R_ease_back_out);
    R_EASE_NAME_ADD_FN(R_ease_back_in_out);
    R_EASE_NAME_ADD_FN(R_ease_quint_in);
    R_EASE_NAME_ADD_FN(R_ease_quint_out);
    R_EASE_NAME_ADD_FN(R_ease_quint_in_out);
    R_EASE_NAME_ADD_FN(R_ease_quad_in);
    R_EASE_NAME_ADD_FN(R_ease_quad_out);
    R_EASE_NAME_ADD_FN(R_ease_quad_in_out);
    R_EASE_NAME_ADD_FN(R_ease_quart_in);
    R_EASE_NAME_ADD_FN(R_ease_quart_out);
    R_EASE_NAME_ADD_FN(R_ease_quart_in_out);
    R_EASE_NAME_ADD_FN(R_ease_sine_in);
    R_EASE_NAME_ADD_FN(R_ease_sine_out);
    R_EASE_NAME_ADD_FN(R_ease_sine_in_out);
    R_EASE_NAME_ADD_FN(R_ease_expo_in);
    R_EASE_NAME_ADD_FN(R_ease_expo_out);
    R_EASE_NAME_ADD_FN(R_ease_expo_in_out);
}


float R_ease_linear(float k, R_UNUSED R_UserData user)
{
    return k;
}


float R_ease_back_in(float k, R_UNUSED R_UserData user)
{
    return k * k * ((1.70158f + 1.0f) * k - 1.70158f);
}

float R_ease_back_out(float k, R_UNUSED R_UserData user)
{
    k -= 1.0f;
    return (k * k * ((1.70158f + 1.0f) * k + 1.70158f) + 1.0f);
}

float R_ease_back_in_out(float k, R_UNUSED R_UserData user)
{
    if ((k /= 0.5f) < 1.0f) {
        return 0.5f * (k * k * (((1.70158f *1.525f) + 1.0f)
             * k - (1.70158f * 1.525f)));
    }
    else {
        k -= 2.0f;
        return 0.5f * (k * k * (((1.70158f * 1.525f) + 1.0f)
             * k + (1.70158f * 1.525f)) + 2.0f);
    }
}


static float b_out(float t, float b, float c, float d)
{
    if ((t /= d) < (1.0f / 2.75f)) {
        return c * (7.5625f * t * t) + b;
    }
    else if (t < (2 / 2.75f)) {
        t -= 1.5f / 2.75f;
        return c * (7.5625f * t * t + 0.75f) + b;
    }
    else if (t < (2.5f / 2.75f)) {
        t -= 2.25f / 2.75f;
        return c * (7.5625f * t * t + 0.9375f) + b;
    }
    else {
        t -= 2.625f / 2.75f;
        return c * (7.5625f * t * t + 0.984375f) + b;
    }
}

static float b_in(float t, float b, float c, float d)
{
    return c - b_out(d - t, 0.0f, c, d) + b;
}

float R_ease_bounce_in(float k, R_UNUSED R_UserData user)
{
    return b_in(k, 0.0f, 1.0f, 1.0f);
}

float R_ease_bounce_out(float k, R_UNUSED R_UserData user)
{
    return b_out(k, 0.0f, 1.0f, 1.0f);
}

float R_ease_bounce_in_out(float k, R_UNUSED R_UserData user)
{
    if (k < 0.5f) {
        return b_in(k * 2.0f, 0.0f, 1.0f, 1.0f) * 0.5f;
    }
    else {
        return b_out(k * 2.0f - 1.0f, 0.0f, 1.0f, 1.0f) * 0.5f + 1.0f * 0.5f;
    }
}


float R_ease_cubic_in(float k, R_UNUSED R_UserData user)
{
    return k * k * k;
}

float R_ease_cubic_out(float k, R_UNUSED R_UserData user)
{
    k -= 1.0f;
    return k * k * k + 1.0f;
}

float R_ease_cubic_in_out(float k, R_UNUSED R_UserData user)
{
    if ((k /= 1.0f / 2.0f) < 1.0f) {
        return 0.5f * k * k * k;
    }
    else {
        k -= 2.0f;
        return 0.5f * (k * k * k + 2.0f);
    }
}


float R_ease_elastic_in(float k, R_UNUSED R_UserData user)
{
    if (k == 0.0f || k == 1.0f) {
        return k;
    }
    k -= 1.0f;
    return -(expf(6.931471805599453f * k)
         * sinf((k - (0.4f / (2.0f * R_PI) * asinf(1.0f)))
         * (2.0f * R_PI) / 0.4f));
}

float R_ease_elastic_out(float k, R_UNUSED R_UserData user)
{
    if (k == 0.0f || k == 1.0f) {
        return k;
    }
    return (expf(-6.931471805599453f * k)
         * sinf((k - (0.4f / (2.0f * R_PI) * asinf(1.0f)))
         * (2.0f * R_PI) / 0.4f) + 1.0f);
}

float R_ease_elastic_in_out(float k, R_UNUSED R_UserData user)
{
    if (k == 0.0f) {
        return 0.0f;
    }

    if ((k /= 1.0f / 2.0f) == 2.0f) {
        return 1.0f;
    }

    if (k < 1.0f) {
        k -= 1.0f;
        return -0.5f * (expf(6.931471805599453f * k)
             * sinf((k - 0.1125f) * (2.0f * R_PI) / 0.45f));
    }
    else {
        k -= 1.0f;
        return expf(-6.931471805599453f * k)
             * sinf((k - 0.1125f) * (2.0f * R_PI) / 0.45f) * 0.5f + 1.0f;
    }
}


float R_ease_expo_in(float k, R_UNUSED R_UserData user)
{
    return k == 0.0f ? 0.0f : expf(6.931471805599453f * (k - 1.0f));
}

float R_ease_expo_out(float k, R_UNUSED R_UserData user)
{
    return k == 1.0f ? 1.0f : (1.0f - expf(-6.931471805599453f * k));
}

float R_ease_expo_in_out(float k, R_UNUSED R_UserData user)
{
    if (k == 0.0f || k == 1.0f) {
        return k;
    }

    if ((k /= 1.0f / 2.0f) < 1.0f) {
        return 0.5f * expf(6.931471805599453f * (k - 1.0f));
    }
    else {
        return 0.5f * (2.0f - expf(-6.931471805599453f * (k - 1.0f)));
    }
}


float R_ease_quad_in(float k, R_UNUSED R_UserData user)
{
    return k * k;
}

float R_ease_quad_out(float k, R_UNUSED R_UserData user)
{
    return -k * (k - 2.0f);
}

float R_ease_quad_in_out(float k, R_UNUSED R_UserData user)
{
    if ((k *= 2.0f) < 1.0f) {
        return 1.0f / 2.0f * k * k;
    }
    else {
        return -1.0f / 2.0f * ((k - 1.0f) * (k - 3.0f) - 1.0f);
    }
}


float R_ease_quart_in(float k, R_UNUSED R_UserData user)
{
    return k * k * k * k;
}

float R_ease_quart_out(float k, R_UNUSED R_UserData user)
{
    k -= 1.0f;
    return -(k * k * k * k - 1.0f);
}

float R_ease_quart_in_out(float k, R_UNUSED R_UserData user)
{
    if ((k *= 2.0f) < 1.0f) {
        return 0.5f * k * k * k * k;
    }
    else {
        k -= 2.0f;
        return -0.5f * (k * k * k * k - 2.0f);
    }
}


float R_ease_quint_in(float k, R_UNUSED R_UserData user)
{
    return k * k * k * k * k;
}

float R_ease_quint_out(float k, R_UNUSED R_UserData user)
{
    k -= 1.0f;
    return k * k * k * k * k + 1.0f;
}

float R_ease_quint_in_out(float k, R_UNUSED R_UserData user)
{
    if ((k *= 2.0f) < 1.0f) {
        return 0.5f * k * k * k * k * k;
    }
    else {
        k -= 2.0f;
        return 0.5f * (k * k * k * k * k + 2.0f);
    }
}


float R_ease_sine_in(float k, R_UNUSED R_UserData user)
{
    return 1.0f - cosf(k * R_PI_2);
}

float R_ease_sine_out(float k, R_UNUSED R_UserData user)
{
    return sinf(k * R_PI_2);
}

float R_ease_sine_in_out(float k, R_UNUSED R_UserData user)
{
    return -(cosf(R_PI * k) - 1.0f) / 2.0f;
}
