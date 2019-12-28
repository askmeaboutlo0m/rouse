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
#include "../json.h"
#include <rouse_config.h>
#include "../common.h"
#include "seq.h"
#include "call.h"

typedef struct R_Call {
    R_MAGIC_FIELD
    R_CallStepFn on_step;
    R_CallFreeFn on_free;
    R_CallJsonFn to_json;
    R_UserData   user;
} R_Call;


static R_StepStatus tick_call(R_StepTickArgs args)
{
    R_Call *call = args.state;
    R_MAGIC_CHECK(R_Call, call);
    call->on_step(args, call->user);
    return R_STEP_STATUS_COMPLETE;
}

static void free_call(void *state)
{
    R_Call *call = state;
    R_MAGIC_CHECK(R_Call, call);
    if (call->on_free) {
        call->on_free(call->user);
    }
    R_MAGIC_POISON(R_Call, call);
    free(call);
}

static void call_to_json(JSON_Object *obj, void *state)
{
    R_Call *call = state;
    R_MAGIC_CHECK(R_Call, call);
    json_object_set_string(obj, "type", "R_Call");
    if (call->to_json) {
        call->to_json(obj, call->user);
    }
    else {
        R_json_object_set_hexdump(obj, "user", &call->user, sizeof(call->user));
    }
}

R_Step *R_call_new(R_CallStepFn on_step, R_CallFreeFn on_free,
                   R_CallJsonFn to_json, R_UserData user)
{
    R_assert_not_null(on_step);
    R_Call *call = R_NEW_INIT_STRUCT(call, R_Call,
            R_MAGIC_INIT(R_Call) on_step, on_free, to_json, user);
    R_MAGIC_CHECK(R_Call, call);
    return R_step_new(call, tick_call, free_call, call_to_json);
}
