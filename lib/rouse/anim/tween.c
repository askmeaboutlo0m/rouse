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


typedef struct R_TweenElement R_TweenElement;

typedef struct R_Tween {
    R_MAGIC_FIELD
    int            lap;
    float          start, left;
    R_EaseFn       ease;
    R_TweenElement *elements;
    R_TweenCalcFn  on_calc;
    R_TweenFreeFn  on_free;
    R_UserData     user;
} R_Tween;


typedef void (*R_TweenElementCalcFn)(R_TweenElement *);
typedef void (*R_TweenElementTickFn)(R_TweenElement *, float);

struct R_TweenElement {
    R_MAGIC_FIELD
    R_UserData           user;
    R_TweenElementCalcFn on_calc;
    R_TweenElementTickFn on_tick;
    R_TweenElementFreeFn on_free;
    R_TweenElement       *next;
    R_UserData           user_value;
    R_TweenValueFreeFn   free_value;
    union {
        struct {
            float              source_float;
            float              target_float;
            R_TweenFloatGetFn  get_float;
            R_TweenFloatSetFn  set_float;
            R_TweenFloatCalcFn calc_float;
        };
    };
};


static void calc_elements(R_TweenElement *elements)
{
    for (R_TweenElement *elem = elements; elem; elem = elem->next) {
        elem->on_calc(elem);
    }
}

static void tick_elements(R_TweenElement *elements, float ratio)
{
    for (R_TweenElement *elem = elements; elem; elem = elem->next) {
        elem->on_tick(elem, ratio);
    }
}

static void free_elements(R_TweenElement *elements, R_UserData *seq_user)
{
    for (R_TweenElement *elem = elements, *next; elem; elem = next) {
        next = elem->next;

        if (elem->free_value) {
            elem->free_value(elem->user_value, seq_user);
        }

        if (elem->on_free) {
            elem->on_free(elem->user, seq_user);
        }

        R_MAGIC_POISON_NN(elem);
        free(elem);
    }
}


static float apply_ease(R_EaseFn ease, float ratio)
{
    return ease ? ease(ratio) : ratio;
}

static R_StepStatus tick_tween(R_StepTickArgs args)
{
    R_Tween *tween = args.state;
    R_MAGIC_CHECK(tween);

    if (tween->lap != args.lap) {
        tween->start = tween->on_calc(args, tween->user);
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

static void free_tween(void *state, R_UserData *seq_user)
{
    if (state) {
        R_Tween *tween = state;
        R_MAGIC_CHECK(tween);

        free_elements(tween->elements, seq_user);

        if (tween->on_free) {
            tween->on_free(tween->user);
        }

        R_MAGIC_POISON(tween);
        free(tween);
    }
}

R_Step *R_tween_new(R_TweenCalcFn on_calc, R_TweenFreeFn on_free,
                    R_UserData user, R_EaseFn ease)
{
    assert(on_calc && "tween calc function can't be NULL");
    R_Tween *tween = R_NEW_INIT_STRUCT(tween, R_Tween, R_MAGIC_INIT(tween)
            -1, 0.0f, 0.0f, ease, NULL, on_calc, on_free, user);
    R_MAGIC_CHECK(tween);
    return R_step_new(tween, tick_tween, free_tween, NULL);
}


static float calc_fixed_tween(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return user.f;
}

R_Step *R_tween_new_fixed(float seconds, R_EaseFn ease)
{
    return R_tween_new(calc_fixed_tween, NULL, R_user_float(seconds), ease);
}


static float calc_between_tween(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return R_rand_between(user.between.a, user.between.b);
}

R_Step *R_tween_new_between(float a, float b, R_EaseFn ease)
{
    return R_tween_new(calc_between_tween, NULL, R_user_between(a, b), ease);
}


R_TweenFloat R_tween_float(R_TweenFloatCalcFn calc, R_TweenValueFreeFn on_free,
                           R_UserData user)
{
    assert(calc && "tween float calc function can't be NULL");
    return (R_TweenFloat){calc, on_free, user};
}


static float calc_fixed_float(R_UserData user, R_UNUSED float source)
{
    return user.f;
}

R_TweenFloat R_tween_float_fixed(float value)
{
    return R_tween_float(calc_fixed_float, NULL, R_user_float(value));
}


static float calc_between_float(R_UserData user, R_UNUSED float source)
{
    return R_rand_between(user.between.a, user.between.b);
}

R_TweenFloat R_tween_float_between(float a, float b)
{
    return R_tween_float(calc_between_float, NULL, R_user_between(a, b));
}


static void calc_float_element(R_TweenElement *elem)
{
    float source       = elem->get_float(elem->user);
    elem->source_float = source;
    elem->target_float = elem->calc_float(elem->user_value, source);
}

static void tick_float_element(R_TweenElement *elem, float ratio)
{
    float current = R_lerp(elem->source_float, elem->target_float, ratio);
    elem->set_float(elem->user, current);
}

static R_TweenElement *new_float_element(
        R_UserData user, R_TweenElementFreeFn on_free, R_UserData user_value,
        R_TweenValueFreeFn free_value, R_TweenFloatGetFn get_float,
        R_TweenFloatSetFn set_float, R_TweenFloatCalcFn calc_float)
{
    assert(get_float  && "element get float function can't be NULL");
    assert(set_float  && "element set float function can't be NULL");
    assert(calc_float && "element calc float function can't be NULL");
    R_TweenElement *elem = R_NEW_INIT_STRUCT(elem, R_TweenElement,
            R_MAGIC_INIT(elem) user, calc_float_element, tick_float_element,
            on_free, NULL, user_value, free_value,
            {{0.0f, 0.0f, get_float, set_float, calc_float}});
    R_MAGIC_CHECK(elem);
    return elem;
}

static void tween_add_element(R_Step *step, R_TweenElement *elem)
{
    R_Tween *tween = R_step_state(step);
    R_MAGIC_CHECK(tween);
    elem->next      = tween->elements;
    tween->elements = elem;
}

void R_tween_add_float(R_Step *step, R_TweenFloat f, R_UserData user,
                       R_TweenFloatGetFn get_float, R_TweenFloatSetFn set_float,
                       R_TweenElementFreeFn on_free)
{
    R_TweenElement *elem = new_float_element(user, on_free, f.user, f.free,
                                             get_float, set_float, f.calc);
    tween_add_element(step, elem);
}
