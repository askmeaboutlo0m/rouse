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
#include <stddef.h>
#include <stdnoreturn.h>
#include <assert.h>
#include <cglm/struct.h>
#include "../3rdparty/parson.h"
#include "../common.h"
#include "../json.h"
#include "../geom.h"
#include "ease.h"
#include "seq.h"
#include "util.h"
#include "tween.h"

#define R_MAGIC_CHECK_CHILD(TWEEN) \
    R_MAGIC_CHECK_2((TWEEN), &(TWEEN)->base)

#define R_MAGIC_POISON_CHILD(TWEEN) do { \
        R_MAGIC_POISON_NN(TWEEN); \
        R_MAGIC_POISON_NN(&(TWEEN)->base); \
    } while (0)

#define R_MAGIC_CHECK_GRANDCHILD(TWEEN) \
    R_MAGIC_CHECK_3((TWEEN), &(TWEEN)->base, &(TWEEN)->base.base)


typedef struct R_Tween {
    R_MAGIC_FIELD
    int            lap;
    float          start, left;
    R_EaseFn       ease;
    R_TweenElement *elements;
} R_Tween;

typedef struct R_FixedTween {
    R_Tween base;
    R_MAGIC_FIELD
    float seconds;
} R_FixedTween;

struct R_TweenElement {
    R_MAGIC_FIELD
    R_TweenElementCalcFn on_calc;
    R_TweenElementTickFn on_tick;
    R_TweenElementFreeFn on_free;
    R_UserData           user;
    R_TweenElement       *next;
};

typedef struct R_FloatElement {
    R_TweenElement base;
    R_MAGIC_FIELD
    float             source;
    float             target;
    R_TweenFloatGetFn get;
    R_TweenFloatSetFn set;
} R_FloatElement;

typedef struct R_FixedFloatElement {
    R_FloatElement base;
    R_MAGIC_FIELD
    float value;
} R_FixedFloatElement;

typedef struct R_BetweenFloatElement {
    R_FloatElement base;
    R_MAGIC_FIELD
    float a, b;
} R_BetweenFloatElement;


static void calc_elements(R_TweenElement *elements)
{
    for (R_TweenElement *elem = elements; elem; elem = elem->next) {
        elem->on_calc((R_TweenCalcArgs){
            .elem = elem,
            .user = elem->user,
        });
    }
}

static void tick_elements(R_TweenElement *elements, float ratio)
{
    for (R_TweenElement *elem = elements; elem; elem = elem->next) {
        elem->on_tick((R_TweenTickArgs){
            .elem  = elem,
            .user  = elem->user,
            .ratio = ratio,
        });
    }
}

static void free_elements(R_TweenElement *elements)
{
    for (R_TweenElement *elem = elements, *next; elem; elem = next) {
        next = elem->next;
        if (elem->on_free) {
            elem->on_free((R_TweenFreeArgs){
                .user = elem->user,
            });
        }
        R_MAGIC_POISON_NN(elem);
        free(elem);
    }
}


static float apply_ease(R_EaseFn ease, float ratio)
{
    return ease ? ease(ratio) : ratio;
}

static R_StepStatus tick_tween(R_StepTickArgs args, float (*calc)(void *))
{
    R_Tween *tween = args.state;
    R_MAGIC_CHECK(tween);

    if (tween->lap != args.lap) {
        tween->start = calc(tween);
        tween->left  = tween->start;
        tween->lap   = args.lap;
        calc_elements(tween->elements);
    }

    float left = tween->left -= args.seconds;
    if (R_enough_seconds_left(left, args.seconds)) {
        float start = tween->start;
        float ratio = (start - left) / start;
        tick_elements(tween->elements, apply_ease(tween->ease, ratio));
        return R_STEP_STATUS_RUNNING;
    }
    else {
        tick_elements(tween->elements, apply_ease(tween->ease, 1.0f));
        return R_STEP_STATUS_COMPLETE;
    }
}


static float calc_fixed_tween(void *state)
{
    R_FixedTween *tween = state;
    R_MAGIC_CHECK(tween);
    return tween->seconds;
}

static R_StepStatus tick_fixed_tween(R_StepTickArgs args)
{
    return tick_tween(args, calc_fixed_tween);
}

static void free_fixed_tween(void *state, R_UNUSED R_UserData *seq_user)
{
    if (state) {
        R_FixedTween *tween = state;
        free_elements(tween->base.elements);
        R_MAGIC_POISON_CHILD(tween);
        free(tween);
    }
}

R_Step *R_tween_new_fixed(float seconds, R_EaseFn ease)
{
    R_FixedTween *tween = R_NEW_INIT_STRUCT(tween, R_FixedTween,
            {R_MAGIC_INIT_TYPE(R_Tween) -1, 0.0f, 0.0f, ease, NULL},
            R_MAGIC_INIT(tween) seconds);
    R_MAGIC_CHECK_CHILD(tween);
    return R_step_new(tween, tick_fixed_tween, free_fixed_tween, NULL);
}


static void init_element(R_Step *step, R_TweenElement *elem, R_UserData user,
                         R_TweenElementCalcFn on_calc,
                         R_TweenElementTickFn on_tick,
                         R_TweenElementFreeFn on_free)
{
    assert(on_calc && "tween element calc function can't be NULL");
    assert(on_tick && "tween element tick function can't be NULL");
    R_Tween *tween = R_step_state(step);
    R_MAGIC_SET(elem);
    elem->on_calc   = on_calc;
    elem->on_tick   = on_tick;
    elem->on_free   = on_free;
    elem->user      = user;
    elem->next      = tween->elements;
    tween->elements = elem;
}


R_TweenFloat R_tween_float_fixed(float value)
{
    return (R_TweenFloat){R_TWEEN_VALUE_FIXED, {.fixed = value}};
}

R_TweenFloat R_tween_float_between(float a, float b)
{
    return (R_TweenFloat){R_TWEEN_VALUE_BETWEEN, {.between = {a, b}}};
}


static void tick_float_element(R_TweenTickArgs args)
{
    R_FloatElement *elem = args.elem;
    R_MAGIC_CHECK_CHILD(elem);
    elem->set(args, R_lerp(elem->source, elem->target, args.ratio));
}

static void init_float_element(R_Step *step, R_FloatElement *elem,
                               R_TweenFloatGetFn get, R_TweenFloatSetFn set,
                               R_UserData user, R_TweenElementCalcFn on_calc,
                               R_TweenElementFreeFn on_free)
{
    init_element(step, &elem->base, user, on_calc, tick_float_element, on_free);
    R_MAGIC_SET(elem);
    elem->get    = get;
    elem->set    = set;
    elem->source = 0.0f;
    elem->target = 0.0f;
}


static void calc_fixed_float_element(R_TweenCalcArgs args)
{
    R_FixedFloatElement *elem = args.elem;
    R_MAGIC_CHECK_GRANDCHILD(elem);
    elem->base.source = elem->base.get(args);
    elem->base.target = elem->value;
}

static void add_fixed_float_element(R_Step *step, float value,
                                    R_TweenFloatGetFn get,
                                    R_TweenFloatSetFn set, R_UserData user,
                                    R_TweenElementFreeFn on_free)
{
    R_FixedFloatElement *elem = R_NEW(elem);
    init_float_element(step, &elem->base, get, set, user,
                       calc_fixed_float_element, on_free);
    R_MAGIC_SET(elem);
    elem->value = value;
    R_MAGIC_CHECK_GRANDCHILD(elem);
}


static void calc_between_float_element(R_TweenCalcArgs args)
{
    R_BetweenFloatElement *elem = args.elem;
    R_MAGIC_CHECK_GRANDCHILD(elem);
    elem->base.source = elem->base.get(args);
    elem->base.target = R_rand_between(elem->a, elem->b);
}

static void add_between_float_element(R_Step *step, float a, float b,
                                      R_TweenFloatGetFn get,
                                      R_TweenFloatSetFn set, R_UserData user,
                                      R_TweenElementFreeFn on_free)
{
    R_BetweenFloatElement *elem = R_NEW(elem);
    init_float_element(step, &elem->base, get, set, user,
                       calc_between_float_element, on_free);
    R_MAGIC_SET(elem);
    elem->a = a;
    elem->b = b;
    R_MAGIC_CHECK_GRANDCHILD(elem);
}

void R_tween_add_float(R_Step *step, R_TweenFloat value, R_UserData user,
                       R_TweenFloatGetFn get, R_TweenFloatSetFn set,
                       R_TweenElementFreeFn on_free)
{
    switch (value.type) {
        case R_TWEEN_VALUE_FIXED:
            add_fixed_float_element(step, value.fixed, get, set, user, on_free);
            break;
        case R_TWEEN_VALUE_BETWEEN:
            add_between_float_element(step, value.between.a, value.between.b,
                                      get, set, user, on_free);
            break;
        default:
            R_die("Unknown tween value type: %d", (int) value.type);
    }
}
