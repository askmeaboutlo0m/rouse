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
    R_TweenJsonFn  to_json;
    R_UserData     user;
} R_Tween;


typedef void (*R_TweenElementCalcFn)(R_TweenElement *);
typedef void (*R_TweenElementTickFn)(R_TweenElement *, float);
typedef void (*R_TweenFieldsJsonFn )(R_TweenElement *, JSON_Object *);

struct R_TweenElement {
    R_MAGIC_FIELD
    R_UserData           user;
    R_TweenElementCalcFn on_calc;
    R_TweenElementTickFn on_tick;
    R_TweenElementFreeFn on_free;
    R_TweenElementJsonFn to_json;
    R_TweenElement       *next;
    R_UserData           value_user;
    R_TweenValueFreeFn   value_on_free;
    R_TweenValueJsonFn   value_to_json;
    R_TweenFieldsJsonFn  fields_to_json;
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

        if (elem->value_on_free) {
            elem->value_on_free(elem->value_user, seq_user);
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

static JSON_Value *tween_element_to_json(R_TweenElement *elem,
                                         R_UserData *seq_user)
{
    JSON_Value  *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    if (elem->fields_to_json) {
        elem->fields_to_json(elem, obj);
    }

    if (elem->to_json) {
        elem->to_json(obj, elem->user, seq_user);
    }

    if (elem->value_to_json) {
        elem->value_to_json(obj, elem->value_user, seq_user);
    }

    return val;
}

static JSON_Value *tween_elements_to_json(R_TweenElement *elements,
                                          R_UserData *seq_user)
{
    JSON_Value *val = json_value_init_array();
    JSON_Array *arr = json_value_get_array(val);

    for (R_TweenElement *elem = elements; elem; elem = elem->next) {
        json_array_append_value(arr, tween_element_to_json(elem, seq_user));
    }

    return val;
}

static void ease_to_json(JSON_Object *obj, const char *key, R_EaseFn ease)
{
    if (ease) {
        const char *name = R_ease_name(ease);
        json_object_set_string(obj, key, name ? name : "(unknown)");
    }
    else {
        json_object_set_null(obj, key);
    }
}

static void tween_to_json(JSON_Object *obj, void *state, R_UserData *seq_user)
{
    R_Tween *tween = state;
    R_MAGIC_CHECK(tween);

    json_object_set_string(obj, "type",  "R_Tween");
    json_object_set_number(obj, "lap",   R_int2double(tween->lap));
    json_object_set_number(obj, "start", tween->start);
    json_object_set_number(obj, "left",  tween->left);
    ease_to_json(obj, "ease", tween->ease);

    if (tween->to_json) {
        tween->to_json(obj, tween->user, seq_user);
    }
    else {
        json_object_set_string(obj, "tween_type", "custom");
        R_json_object_set_hexdump(obj, "user", &tween->user,
                                  sizeof(tween->user));
    }

    json_object_set_value(
            obj, "elements", tween_elements_to_json(tween->elements, seq_user));
}

R_Step *R_tween_new(R_TweenCalcFn on_calc, R_TweenFreeFn on_free,
                    R_TweenJsonFn to_json, R_UserData user, R_EaseFn ease)
{
    assert(on_calc && "tween calc function can't be NULL");
    R_Tween *tween = R_NEW_INIT_STRUCT(tween, R_Tween, R_MAGIC_INIT(tween)
            -1, 0.0f, 0.0f, ease, NULL, on_calc, on_free, to_json, user);
    R_MAGIC_CHECK(tween);
    return R_step_new(tween, tick_tween, free_tween, tween_to_json);
}


static float calc_fixed_tween(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return user.f;
}

static void fixed_tween_to_json(JSON_Object *obj, R_UserData user,
                                R_UNUSED R_UserData *seq_user)
{
    json_object_set_string(obj, "tween_type", "fixed");
    json_object_set_number(obj, "seconds", user.f);
}

R_Step *R_tween_new_fixed(float seconds, R_EaseFn ease)
{
    return R_tween_new(calc_fixed_tween, NULL, fixed_tween_to_json,
                       R_user_float(seconds), ease);
}


static float calc_between_tween(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return R_rand_between(user.between.a, user.between.b);
}

static void between_tween_to_json(JSON_Object *obj, R_UserData user,
                                  R_UNUSED R_UserData *seq_user)
{
    json_object_set_string(obj, "tween_type", "between");
    json_object_set_number(obj, "a", user.between.a);
    json_object_set_number(obj, "b", user.between.b);
}

R_Step *R_tween_new_between(float a, float b, R_EaseFn ease)
{
    return R_tween_new(calc_between_tween, NULL, between_tween_to_json,
                       R_user_between(a, b), ease);
}


R_TweenFloat R_tween_float(R_TweenFloatCalcFn on_calc, R_TweenValueFreeFn on_free,
                           R_TweenValueJsonFn to_json, R_UserData user)
{
    assert(on_calc && "tween float calc function can't be NULL");
    return (R_TweenFloat){on_calc, on_free, to_json, user};
}


static float calc_fixed_float(R_UserData user, R_UNUSED float source)
{
    return user.f;
}

static void fixed_float_to_json(JSON_Object *obj, R_UserData user,
                                R_UNUSED R_UserData *seq_user)
{
    json_object_set_string(obj, "value_type", "fixed_float");
    json_object_set_number(obj, "value", user.f);
}

R_TweenFloat R_tween_float_fixed(float value)
{
    return R_tween_float(calc_fixed_float, NULL, fixed_float_to_json,
                         R_user_float(value));
}


static float calc_between_float(R_UserData user, R_UNUSED float source)
{
    return R_rand_between(user.between.a, user.between.b);
}

static void between_float_to_json(JSON_Object *obj, R_UserData user,
                                  R_UNUSED R_UserData *seq_user)
{
    json_object_set_string(obj, "value_type", "between_float");
    json_object_set_number(obj, "a", user.between.a);
    json_object_set_number(obj, "b", user.between.b);
}

R_TweenFloat R_tween_float_between(float a, float b)
{
    return R_tween_float(calc_between_float, NULL, between_float_to_json,
                         R_user_between(a, b));
}


static void calc_float_element(R_TweenElement *elem)
{
    float source       = elem->get_float(elem->user);
    elem->source_float = source;
    elem->target_float = elem->calc_float(elem->value_user, source);
}

static void tick_float_element(R_TweenElement *elem, float ratio)
{
    float current = R_lerp(elem->source_float, elem->target_float, ratio);
    elem->set_float(elem->user, current);
}

static void float_element_to_json(R_TweenElement *elem, JSON_Object *obj)
{
    json_object_set_string(obj, "element_type", "float_element");
    json_object_set_number(obj, "source_float", elem->source_float);
    json_object_set_number(obj, "target_float", elem->target_float);
}

static R_TweenElement *new_float_element(
        R_UserData user, R_TweenElementFreeFn on_free,
        R_TweenElementJsonFn to_json, R_UserData value_user,
        R_TweenValueFreeFn value_on_free, R_TweenValueJsonFn value_to_json,
        R_TweenFloatGetFn get_float, R_TweenFloatSetFn set_float,
        R_TweenFloatCalcFn calc_float)
{
    assert(get_float  && "element get float function can't be NULL");
    assert(set_float  && "element set float function can't be NULL");
    assert(calc_float && "element calc float function can't be NULL");
    R_TweenElement *elem = R_NEW_INIT_STRUCT(elem, R_TweenElement,
            R_MAGIC_INIT(elem) user, calc_float_element, tick_float_element,
            on_free, to_json, NULL, value_user, value_on_free, value_to_json,
            float_element_to_json,
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
                       R_TweenElementFreeFn on_free, R_TweenElementJsonFn to_json)
{
    R_TweenElement *elem = new_float_element(user, on_free, to_json, f.user,
                                             f.on_free, f.to_json, get_float,
                                             set_float, f.on_calc);
    tween_add_element(step, elem);
}
