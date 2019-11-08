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
#include "../3rdparty/parson.h"
#include "../common.h"
#include "../json.h"
#include "seq.h"
#include "util.h"
#include "delay.h"


typedef struct R_Delay {
    R_MAGIC_FIELD
    int           lap;
    float         left;
    R_DelayCalcFn on_calc;
    R_DelayFreeFn on_free;
    R_DelayJsonFn to_json;
    R_UserData    user;
} R_Delay;


static R_StepStatus tick_delay(R_StepTickArgs args)
{
    R_Delay *delay = args.state;
    R_MAGIC_CHECK(delay);

    if (delay->lap != args.lap) {
        delay->left = delay->on_calc(args, delay->user);
        delay->lap  = args.lap;
    }

    float left = delay->left -= args.seconds;
    return R_enough_seconds_left(left, args.seconds)
            ? R_STEP_STATUS_RUNNING : R_STEP_STATUS_COMPLETE;
}

static void free_delay(void *state, R_UserData *seq_user)
{
    R_Delay *delay = state;
    R_MAGIC_CHECK(delay);

    if (delay->on_free) {
        delay->on_free(delay->user, seq_user);
    }

    R_MAGIC_POISON(delay);
    free(delay);
}

static void delay_to_json(JSON_Object *obj, void *state, R_UserData *seq_user)
{
    R_Delay *delay = state;
    R_MAGIC_CHECK(delay);

    json_object_set_string(obj, "type", "R_Delay");
    json_object_set_number(obj, "lap",  R_int2double(delay->lap));
    json_object_set_number(obj, "left", delay->left);

    if (delay->to_json) {
        delay->to_json(obj, delay->user, seq_user);
    }
    else {
        json_object_set_string(obj, "delay_type", "custom");
        R_json_object_set_hexdump(obj, "user", &delay->user,
                                  sizeof(delay->user));
    }
}

R_Step *R_delay_new(R_DelayCalcFn on_calc, R_DelayFreeFn on_free,
                    R_DelayJsonFn to_json, R_UserData user)
{
    assert(on_calc && "delay calc function must not be NULL");
    R_Delay *delay = R_NEW_INIT_STRUCT(delay, R_Delay,
            R_MAGIC_INIT(delay) -1, 0.0f, on_calc, on_free, to_json, user);
    R_MAGIC_CHECK(delay);
    return R_step_new(delay, tick_delay, free_delay, delay_to_json);
}


static float calc_fixed_delay(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return user.f;
}

static void fixed_delay_to_json(JSON_Object *obj, R_UserData user,
                                R_UNUSED R_UserData *seq_user)
{
    json_object_set_string(obj, "delay_type", "fixed");
    json_object_set_number(obj, "seconds", user.f);
}

R_Step *R_delay_new_fixed(float seconds)
{
    return R_delay_new(calc_fixed_delay, NULL, fixed_delay_to_json,
                       R_user_float(seconds));
}


static float calc_between_delay(R_UNUSED R_StepTickArgs args, R_UserData user)
{
    return R_rand_between(user.between.a, user.between.b);
}

static void between_delay_to_json(JSON_Object *obj, R_UserData user,
                                  R_UNUSED R_UserData *seq_user)
{
    json_object_set_string(obj, "delay_type", "between");
    json_object_set_number(obj, "a", user.between.a);
    json_object_set_number(obj, "b", user.between.b);
}

R_Step *R_delay_new_between(float a, float b)
{
    return R_delay_new(calc_between_delay, NULL, between_delay_to_json,
                       R_user_between(a, b));
}
