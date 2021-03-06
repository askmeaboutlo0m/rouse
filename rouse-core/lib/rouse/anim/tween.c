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
#include <assert.h>
#include <cglm/struct.h>
#include "../3rdparty/parson.h"
#include <rouse_config.h>
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
    R_TweenElement *elements;
    R_TweenCalcFn  on_calc;
    R_TweenFreeFn  on_free;
    R_TweenJsonFn  to_json;
    R_UserData     user;
    struct {
        R_EaseFn          fn;
        R_TweenEaseFreeFn on_free;
        R_UserData        user;
    } ease;
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
            float              source;
            float              target;
            R_TweenFloatGetFn  get;
            R_TweenFloatSetFn  set;
            R_TweenFloatCalcFn calc;
        } f;
        struct {
            R_V2               source;
            float              target;
            R_TweenScaleGetFn  get;
            R_TweenScaleSetFn  set;
            R_TweenScaleCalcFn calc;
        } s;
        struct {
            R_V4            source;
            R_V4            target;
            R_TweenV4GetFn  get;
            R_TweenV4SetFn  set;
            R_TweenV4CalcFn calc;
        } v4;
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

static void free_elements(R_TweenElement *elements)
{
    for (R_TweenElement *elem = elements, *next; elem; elem = next) {
        R_MAGIC_CHECK(R_TweenElement, elem);
        next = elem->next;

        if (elem->value_on_free) {
            elem->value_on_free(elem->value_user);
        }

        if (elem->on_free) {
            elem->on_free(elem->user);
        }

        R_MAGIC_POISON(R_TweenElement, elem);
        free(elem);
    }
}


static float apply_ease(R_EaseFn ease, R_UserData user, float ratio)
{
    return ease ? ease(ratio, user) : ratio;
}

static R_StepStatus tick_tween(R_StepTickArgs args)
{
    R_Tween *tween = args.state;
    R_MAGIC_CHECK(R_Tween, tween);
    R_UserData user = tween->user;

    if (tween->lap != args.lap) {
        tween->start = tween->on_calc(args, user);
        tween->left  = tween->start;
        tween->lap   = args.lap;
        calc_elements(tween->elements);
    }

    float      left      = tween->left -= args.seconds;
    R_EaseFn   ease      = tween->ease.fn;
    R_UserData ease_user = tween->ease.user;
    if (R_enough_seconds_left(left, args.seconds)) {
        float start = tween->start;
        float ratio = (start - left) / start;
        tick_elements(tween->elements, apply_ease(ease, ease_user, ratio));
        return R_STEP_STATUS_RUNNING;
    }
    else {
        tick_elements(tween->elements, apply_ease(ease, ease_user, 1.0f));
        return R_STEP_STATUS_COMPLETE;
    }
}

static void free_tween(void *state)
{
    if (state) {
        R_Tween *tween = state;
        R_MAGIC_CHECK(R_Tween, tween);

        free_elements(tween->elements);

        if (tween->on_free) {
            tween->on_free(tween->user);
        }

        if (tween->ease.on_free) {
            tween->ease.on_free(tween->ease.user);
        }

        R_MAGIC_POISON(R_Tween, tween);
        free(tween);
    }
}

static JSON_Value *tween_element_to_json(R_TweenElement *elem)
{
    JSON_Value  *val = json_value_init_object();
    JSON_Object *obj = json_value_get_object(val);

    if (elem->fields_to_json) {
        elem->fields_to_json(elem, obj);
    }

    if (elem->to_json) {
        elem->to_json(obj, elem->user);
    }

    if (elem->value_to_json) {
        elem->value_to_json(obj, elem->value_user);
    }

    return val;
}

static JSON_Value *tween_elements_to_json(R_TweenElement *elements)
{
    JSON_Value *val = json_value_init_array();
    JSON_Array *arr = json_value_get_array(val);

    for (R_TweenElement *elem = elements; elem; elem = elem->next) {
        json_array_append_value(arr, tween_element_to_json(elem));
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

static void tween_to_json(JSON_Object *obj, void *state)
{
    R_Tween *tween = state;
    R_MAGIC_CHECK(R_Tween, tween);

    json_object_set_string(obj, "type",  "R_Tween");
    json_object_set_number(obj, "lap",   R_int2double(tween->lap));
    json_object_set_number(obj, "start", tween->start);
    json_object_set_number(obj, "left",  tween->left);
    ease_to_json(obj, "ease", tween->ease.fn);

    if (tween->to_json) {
        tween->to_json(obj, tween->user);
    }
    else {
        json_object_set_string(obj, "tween_type", "custom");
        R_json_object_set_hexdump(obj, "user", &tween->user,
                                  sizeof(tween->user));
    }

    json_object_set_value(
            obj, "elements", tween_elements_to_json(tween->elements));
}

R_Step *R_tween_new(R_TweenCalcFn on_calc, R_TweenFreeFn on_free,
                    R_TweenJsonFn to_json, R_UserData user, R_EaseFn ease,
                    R_TweenEaseFreeFn ease_free, R_UserData ease_user)
{
    R_assert_not_null(on_calc);
    R_Tween *tween = R_NEW_INIT_STRUCT(tween, R_Tween, R_MAGIC_INIT(R_Tween)
            -1, 0.0f, 0.0f, NULL, on_calc, on_free, to_json, user,
            {ease, ease_free, ease_user});
    R_MAGIC_CHECK(R_Tween, tween);
    return R_step_new(tween, tick_tween, free_tween, tween_to_json);
}


static float calc_fixed_tween(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return user.f;
}

static void fixed_tween_to_json(JSON_Object *obj, R_UserData user)
{
    json_object_set_string(obj, "tween_type", "fixed");
    json_object_set_number(obj, "seconds", user.f);
}

R_Step *R_tween_new_fixed(float seconds, R_EaseFn ease,
                          R_TweenEaseFreeFn ease_free, R_UserData ease_user)
{
    return R_tween_new(calc_fixed_tween, NULL, fixed_tween_to_json,
                       R_user_float(seconds), ease, ease_free, ease_user);
}


static float calc_between_tween(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return R_rand_between(user.between.a, user.between.b);
}

static void between_tween_to_json(JSON_Object *obj, R_UserData user)
{
    json_object_set_string(obj, "tween_type", "between");
    json_object_set_number(obj, "a", user.between.a);
    json_object_set_number(obj, "b", user.between.b);
}

R_Step *R_tween_new_between(float a, float b, R_EaseFn ease,
                            R_TweenEaseFreeFn ease_free, R_UserData ease_user)
{
    return R_tween_new(calc_between_tween, NULL, between_tween_to_json,
                       R_user_between(a, b), ease, ease_free, ease_user);
}


R_TweenFloat R_tween_float(R_TweenFloatCalcFn on_calc, R_TweenValueFreeFn on_free,
                           R_TweenValueJsonFn to_json, R_UserData user)
{
    R_assert_not_null(on_calc);
    return (R_TweenFloat){on_calc, on_free, to_json, user};
}


static float calc_fixed_float(R_UserData user, R_UNUSED float source)
{
    return user.f;
}

static void fixed_float_to_json(JSON_Object *obj, R_UserData user)
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

static void between_float_to_json(JSON_Object *obj, R_UserData user)
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


static void tween_add_element(R_Step *step, R_TweenElement *elem)
{
    R_Tween *tween = R_step_state(step);
    R_MAGIC_CHECK(R_Tween, tween);
    elem->next      = tween->elements;
    tween->elements = elem;
}


static void calc_float_element(R_TweenElement *elem)
{
    float source   = elem->f.get(elem->user);
    elem->f.source = source;
    elem->f.target = elem->f.calc(elem->value_user, source);
}

static void tick_float_element(R_TweenElement *elem, float ratio)
{
    float current = R_lerp(elem->f.source, elem->f.target, ratio);
    elem->f.set(elem->user, current);
}

static void float_element_to_json(R_TweenElement *elem, JSON_Object *obj)
{
    json_object_set_string(obj, "element_type", "float_element");
    json_object_set_number(obj, "f.source",     elem->f.source);
    json_object_set_number(obj, "f.target",     elem->f.target);
}

static R_TweenElement *new_float_element(
        R_UserData user, R_TweenElementFreeFn on_free,
        R_TweenElementJsonFn to_json, R_UserData value_user,
        R_TweenValueFreeFn value_on_free, R_TweenValueJsonFn value_to_json,
        R_TweenFloatGetFn get_float, R_TweenFloatSetFn set_float,
        R_TweenFloatCalcFn calc_float)
{
    R_assert_not_null(get_float);
    R_assert_not_null(set_float);
    R_assert_not_null(calc_float);
    R_TweenElement *elem = R_NEW_INIT_STRUCT(elem, R_TweenElement,
            R_MAGIC_INIT(R_TweenElement) user, calc_float_element,
            tick_float_element, on_free, to_json, NULL, value_user,
            value_on_free, value_to_json, float_element_to_json,
            {.f = {0.0f, 0.0f, get_float, set_float, calc_float}});
    R_MAGIC_CHECK(R_TweenElement, elem);
    return elem;
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


R_TweenScale R_tween_scale(R_TweenScaleCalcFn on_calc, R_TweenValueFreeFn on_free,
                           R_TweenValueJsonFn to_json, R_UserData user)
{
    R_assert_not_null(on_calc);
    return (R_TweenScale){on_calc, on_free, to_json, user};
}


static float calc_fixed_scale(R_UserData user, R_UNUSED R_V2 source)
{
    return user.f;
}

static void fixed_scale_to_json(JSON_Object *obj, R_UserData user)
{
    json_object_set_string(obj, "value_type", "fixed_scale");
    json_object_set_number(obj, "value", user.f);
}

R_TweenScale R_tween_scale_fixed(float value)
{
    return R_tween_scale(calc_fixed_scale, NULL, fixed_scale_to_json,
                         R_user_float(value));
}


static float calc_between_scale(R_UserData user, R_UNUSED R_V2 source)
{
    return R_rand_between(user.between.a, user.between.b);
}

static void between_scale_to_json(JSON_Object *obj, R_UserData user)
{
    json_object_set_string(obj, "value_type", "between_scale");
    json_object_set_number(obj, "a", user.between.a);
    json_object_set_number(obj, "b", user.between.b);
}

R_TweenScale R_tween_scale_between(float a, float b)
{
    return R_tween_scale(calc_between_scale, NULL, between_scale_to_json,
                         R_user_between(a, b));
}


static void calc_scale_element(R_TweenElement *elem)
{
    R_V2 source    = elem->s.get(elem->user);
    elem->s.source = source;
    elem->s.target = elem->s.calc(elem->value_user, source);
}

static void tick_scale_element(R_TweenElement *elem, float ratio)
{
    R_V2  source  = elem->s.source;
    float target  = elem->s.target;
    R_V2  current = R_v2(R_lerp(source.x, target, ratio),
                         R_lerp(source.y, target, ratio));
    elem->s.set(elem->user, current);
}

static void scale_element_to_json(R_TweenElement *elem, JSON_Object *obj)
{
    json_object_set_string(obj, "element_type", "scale_element");
    json_object_set_number(obj, "s.source.x",   elem->s.source.x);
    json_object_set_number(obj, "s.source.y",   elem->s.source.y);
    json_object_set_number(obj, "s.target",     elem->s.target);
}

static R_TweenElement *new_scale_element(
        R_UserData user, R_TweenElementFreeFn on_free,
        R_TweenElementJsonFn to_json, R_UserData value_user,
        R_TweenValueFreeFn value_on_free, R_TweenValueJsonFn value_to_json,
        R_TweenScaleGetFn get_scale, R_TweenScaleSetFn set_scale,
        R_TweenScaleCalcFn calc_scale)
{
    R_assert_not_null(get_scale);
    R_assert_not_null(set_scale);
    R_assert_not_null(calc_scale);
    R_TweenElement *elem = R_NEW_INIT_STRUCT(elem, R_TweenElement,
            R_MAGIC_INIT(R_TweenElement) user, calc_scale_element,
            tick_scale_element, on_free, to_json, NULL, value_user,
            value_on_free, value_to_json, scale_element_to_json,
            {.s = {R_v2(0.0f, 0.0f), 0.0f, get_scale, set_scale, calc_scale}});
    R_MAGIC_CHECK(R_TweenElement, elem);
    return elem;
}

void R_tween_add_scale(R_Step *step, R_TweenScale s, R_UserData user,
                       R_TweenScaleGetFn get_scale, R_TweenScaleSetFn set_scale,
                       R_TweenElementFreeFn on_free, R_TweenElementJsonFn to_json)
{
    R_TweenElement *elem = new_scale_element(user, on_free, to_json, s.user,
                                             s.on_free, s.to_json, get_scale,
                                             set_scale, s.on_calc);
    tween_add_element(step, elem);
}


R_TweenV4 R_tween_v4(R_TweenV4CalcFn on_calc, R_TweenValueFreeFn on_free,
                           R_TweenValueJsonFn to_json, R_UserData user)
{
    R_assert_not_null(on_calc);
    return (R_TweenV4){on_calc, on_free, to_json, user};
}


static R_V4 calc_fixed_v4(R_UserData user, R_UNUSED R_V4 source)
{
    R_V4 *data = user.data;
    return *data;
}

static void free_fixed_v4(R_UserData user)
{
    R_V4 *data = user.data;
    free(data);
}

static void fixed_v4_to_json(JSON_Object *obj, R_UserData user)
{
    R_V4 *data = user.data;
    json_object_set_string(obj, "value_type", "fixed_v4");
    json_object_set_number(obj, "value.x", data->x);
    json_object_set_number(obj, "value.y", data->y);
    json_object_set_number(obj, "value.z", data->z);
    json_object_set_number(obj, "value.w", data->w);
}

R_TweenV4 R_tween_v4_fixed(R_V4 value)
{
    R_V4 *data = R_NEW_INIT(data, value);
    return R_tween_v4(calc_fixed_v4, free_fixed_v4, fixed_v4_to_json,
                      R_user_data(data));
}


typedef struct R_BetweenV4 {
    R_V4 a, b;
} R_BetweenV4;

static R_V4 calc_between_v4(R_UserData user, R_UNUSED R_V4 source)
{
    R_BetweenV4 *data = user.data;
    return R_v4(R_rand_between(data->a.x, data->b.x),
                R_rand_between(data->a.y, data->b.y),
                R_rand_between(data->a.z, data->b.z),
                R_rand_between(data->a.w, data->b.w));
}

static void free_between_v4(R_UserData user)
{
    R_BetweenV4 *data = user.data;
    free(data);
}

static void between_v4_to_json(JSON_Object *obj, R_UserData user)
{
    R_BetweenV4 *data = user.data;
    json_object_set_string(obj, "value_type", "between_v4");
    json_object_set_number(obj, "a.x", data->a.x);
    json_object_set_number(obj, "a.y", data->a.y);
    json_object_set_number(obj, "a.z", data->a.z);
    json_object_set_number(obj, "a.w", data->a.w);
    json_object_set_number(obj, "b.x", data->b.x);
    json_object_set_number(obj, "b.y", data->b.y);
    json_object_set_number(obj, "b.z", data->b.z);
    json_object_set_number(obj, "b.w", data->b.w);
}

R_TweenV4 R_tween_v4_between(R_V4 a, R_V4 b)
{
    R_BetweenV4 *data = R_NEW_INIT_STRUCT(data, R_BetweenV4, a, b);
    return R_tween_v4(calc_between_v4, free_between_v4, between_v4_to_json,
                      R_user_data(data));
}


static void calc_v4_element(R_TweenElement *elem)
{
    R_V4 source     = elem->v4.get(elem->user);
    elem->v4.source = source;
    elem->v4.target = elem->v4.calc(elem->value_user, source);
}

static void tick_v4_element(R_TweenElement *elem, float ratio)
{
    R_V4 source  = elem->v4.source;
    R_V4 target  = elem->v4.target;
    R_V4 current = R_v4(R_lerp(source.x, target.x, ratio),
                        R_lerp(source.y, target.y, ratio),
                        R_lerp(source.z, target.z, ratio),
                        R_lerp(source.w, target.w, ratio));
    elem->v4.set(elem->user, current);
}

static void v4_element_to_json(R_TweenElement *elem, JSON_Object *obj)
{
    json_object_set_string(obj, "element_type", "v4_element");
    json_object_set_number(obj, "v4.source.x",  elem->v4.source.x);
    json_object_set_number(obj, "v4.source.y",  elem->v4.source.y);
    json_object_set_number(obj, "v4.source.z",  elem->v4.source.z);
    json_object_set_number(obj, "v4.source.w",  elem->v4.source.w);
    json_object_set_number(obj, "v4.target.x",  elem->v4.target.x);
    json_object_set_number(obj, "v4.target.y",  elem->v4.target.y);
    json_object_set_number(obj, "v4.target.z",  elem->v4.target.z);
    json_object_set_number(obj, "v4.target.w",  elem->v4.target.w);
}

static R_TweenElement *new_v4_element(
        R_UserData user, R_TweenElementFreeFn on_free,
        R_TweenElementJsonFn to_json, R_UserData value_user,
        R_TweenValueFreeFn value_on_free, R_TweenValueJsonFn value_to_json,
        R_TweenV4GetFn get_v4, R_TweenV4SetFn set_v4,
        R_TweenV4CalcFn calc_v4)
{
    R_assert_not_null(get_v4);
    R_assert_not_null(set_v4);
    R_assert_not_null(calc_v4);
    R_TweenElement *elem = R_NEW_INIT_STRUCT(elem, R_TweenElement,
            R_MAGIC_INIT(R_TweenElement) user, calc_v4_element,
            tick_v4_element, on_free, to_json, NULL, value_user,
            value_on_free, value_to_json, v4_element_to_json,
            {.v4 = {R_v4(0.0f, 0.0f, 0.0f, 0.0f), R_v4(0.0f, 0.0f, 0.0f, 0.0f),
                    get_v4, set_v4, calc_v4}});
    R_MAGIC_CHECK(R_TweenElement, elem);
    return elem;
}

void R_tween_add_v4(R_Step *step, R_TweenV4 v4, R_UserData user,
                    R_TweenV4GetFn get_v4, R_TweenV4SetFn set_v4,
                    R_TweenElementFreeFn on_free, R_TweenElementJsonFn to_json)
{
    R_TweenElement *elem = new_v4_element(user, on_free, to_json, v4.user,
                                             v4.on_free, v4.to_json, get_v4,
                                             set_v4, v4.on_calc);
    tween_add_element(step, elem);
}
