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

typedef struct R_DelayState {
    int   lap;
    float left;
} R_DelayState;

typedef struct R_FixedDelay {
    R_MAGIC_FIELD
    R_DelayState state;
    float        seconds;
} R_FixedDelay;

typedef struct R_DelayBetween {
    R_MAGIC_FIELD
    R_DelayState state;
    float        a, b;
} R_DelayBetween;

typedef struct R_CustomDelay {
    R_MAGIC_FIELD
    R_DelayState  state;
    R_DelayCalcFn on_calc;
    R_DelayFreeFn on_free;
    R_UserData    user;
} R_CustomDelay;


#define RETURN_NEW_DELAY(TYPE, ON_TICK, ON_FREE, TO_JSON, ...) do { \
        TYPE *_delay = R_NEW_INIT_STRUCT(_delay, TYPE, \
                R_MAGIC_INIT(_delay) {-1, 0.0f}, __VA_ARGS__); \
        R_MAGIC_CHECK(_delay); \
        return R_step_new(_delay, ON_TICK, ON_FREE, TO_JSON); \
    } while (0)

#define RETURN_TICK_DELAY(TYPE, NAME, RECALCULATE_EXPR) do { \
        TYPE *NAME = args.state; \
        R_MAGIC_CHECK(NAME); \
        R_DelayState *_ds = &NAME->state; \
        if (_ds->lap != args.lap) { \
            _ds->left = RECALCULATE_EXPR; \
            _ds->lap  = args.lap; \
        } \
        float left = _ds->left -= args.seconds; \
        return R_enough_seconds_left(left, args.seconds) \
             ? R_STEP_STATUS_RUNNING : R_STEP_STATUS_COMPLETE; \
    } while (0)

#define FREE_DELAY(TYPE, NAME, ...) do { \
        TYPE *NAME = state; \
        __VA_ARGS__ \
        R_MAGIC_POISON(NAME); \
        free(NAME); \
    } while(0)

#define DELAY_TO_JSON(TYPE, NAME, ...) do { \
        TYPE *NAME = state; \
        R_MAGIC_CHECK(NAME); \
        delay_to_json(obj, #TYPE, &NAME->state); \
        __VA_ARGS__ \
    } while(0)


static void delay_to_json(JSON_Object *obj, const char *type, R_DelayState *ds)
{
    json_object_set_string(obj, "type", type);
    json_object_set_number(obj, "lap",  R_int2double(ds->lap));
    json_object_set_number(obj, "left", ds->left);
}


static R_StepStatus tick_fixed_delay(R_StepTickArgs args)
{
    RETURN_TICK_DELAY(R_FixedDelay, fd, fd->seconds);
}

static void free_fixed_delay(void *state, R_UNUSED R_UserData *seq_user)
{
    FREE_DELAY(R_FixedDelay, fd, /* nothing */);
}

static void fixed_delay_to_json(JSON_Object *obj, void *state,
                                R_UNUSED R_UserData *seq_user)
{
    DELAY_TO_JSON(R_FixedDelay, fd, {
        json_object_set_number(obj, "seconds", fd->seconds);
    });
}

R_Step *R_delay_new_fixed(float seconds)
{
    RETURN_NEW_DELAY(R_FixedDelay, tick_fixed_delay, free_fixed_delay,
                     fixed_delay_to_json, seconds);
}


static R_StepStatus tick_delay_between(R_StepTickArgs args)
{
    RETURN_TICK_DELAY(R_DelayBetween, db, R_rand_between(db->a, db->b));
}

static void free_delay_between(void *state, R_UNUSED R_UserData *seq_user)
{
    FREE_DELAY(R_DelayBetween, db, /* nothing */);
}

static void delay_between_to_json(JSON_Object *obj, void *state,
                                  R_UNUSED R_UserData *seq_user)
{
    DELAY_TO_JSON(R_DelayBetween, db, {
        json_object_set_number(obj, "a", db->a);
        json_object_set_number(obj, "b", db->b);
    });
}

R_Step *R_delay_new_between(float a, float b)
{
    RETURN_NEW_DELAY(R_DelayBetween, tick_delay_between, free_delay_between,
                     delay_between_to_json, a, b);
}


static R_StepStatus tick_custom_delay(R_StepTickArgs args)
{
    RETURN_TICK_DELAY(R_CustomDelay, cd, cd->on_calc(cd->user, args));
}

static void free_custom_delay(void *state, R_UserData *seq_user)
{
    FREE_DELAY(R_CustomDelay, cd,
        R_MAGIC_CHECK(cd);
        if (cd->on_free) {
            cd->on_free(cd->user, seq_user);
        }
    );
}

static void custom_delay_to_json(JSON_Object *obj, void *state,
                                 R_UNUSED R_UserData *seq_user)
{
    DELAY_TO_JSON(R_CustomDelay, cd, {
        R_JSON_OBJECT_SET_FN(     obj, "on_calc", cd->on_calc);
        R_JSON_OBJECT_SET_FN(     obj, "on_free", cd->on_free);
        R_json_object_set_hexdump(obj, "user",    &cd->user, sizeof(cd->user));
    });
}

R_Step *R_delay_new_custom(R_DelayCalcFn on_calc, R_DelayFreeFn on_free,
                           R_UserData user)
{
    assert(on_calc && "on_calc must not be NULL");
    RETURN_NEW_DELAY(R_CustomDelay, tick_custom_delay, free_custom_delay,
                     custom_delay_to_json, on_calc, on_free, user);
}
