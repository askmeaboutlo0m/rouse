/*
 * Copyright (c) 2020 askmeaboutloom
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
#include <SDL2/SDL.h>
#include <cglm/struct.h>
#include "../3rdparty/parson.h"
#include <rouse_config.h>
#include "../common.h"
#include "../string.h"
#include "../json.h"
#include "../geom.h"
#include "../parse.h"
#include "../anim/ease.h"
#include "../anim/seq.h"
#include "../anim/util.h"
#include "../anim/tween.h"
#include "sample.h"
#include "../refcount.h"
#include "../main.h"
#include "al.h"
#include "sound_anim.h"


#define DEF_AL_LISTENER_TWEEN(NAME) \
    static float listener_get_ ## NAME(R_UNUSED R_UserData user) \
    { \
        return R_al_listener_ ## NAME(); \
    } \
    \
    static void listener_set_ ## NAME(R_UNUSED R_UserData user, float value) \
    { \
        R_al_listener_ ## NAME ## _set(value); \
    } \
    \
    static void listener_ ## NAME ## _to_json(JSON_Object *obj, \
                                              R_UserData user) \
    { \
        json_object_set_string(obj, "element_type", "al_listener_" #NAME); \
        json_object_set_number(obj, "current_" #NAME, \
                               listener_get_ ## NAME(user)); \
    } \
    \
    void R_tween_al_listener_ ## NAME(R_Step *step, R_TweenFloat value) \
    { \
        R_tween_add_float(step, value, R_user_null(), \
                          listener_get_ ## NAME, listener_set_ ## NAME, \
                          NULL, listener_ ## NAME ## _to_json); \
    }

DEF_AL_LISTENER_TWEEN(gain)
DEF_AL_LISTENER_TWEEN(pos_x)
DEF_AL_LISTENER_TWEEN(pos_y)
DEF_AL_LISTENER_TWEEN(pos_z)
DEF_AL_LISTENER_TWEEN(velocity_x)
DEF_AL_LISTENER_TWEEN(velocity_y)
DEF_AL_LISTENER_TWEEN(velocity_z)


static void tween_al_source_free(R_UserData user)
{
    R_al_source_decref(user.data);
}

#define DEF_AL_SOURCE_TWEEN(NAME) \
    static float source_get_ ## NAME(R_UserData user) \
    { \
        R_AlSource* source = user.data; \
        return R_al_source_ ## NAME(source); \
    } \
    \
    static void source_set_ ## NAME(R_UserData user, float value) \
    { \
        R_AlSource *source = user.data; \
        R_al_source_ ## NAME ## _set(source, value); \
    } \
    \
    static void source_ ## NAME ## _to_json(JSON_Object *obj, R_UserData user) \
    { \
        json_object_set_string(obj, "element_type", "al_source_" #NAME); \
        json_object_set_number(obj, "current_" #NAME, \
                               source_get_ ## NAME(user)); \
        json_object_set_number(obj, "source_id", R_al_source_id(user.data)); \
    } \
    \
    void R_tween_al_source_ ## NAME(R_Step *step, R_AlSource *source, \
                                    R_TweenFloat value) \
    { \
        R_al_source_incref(source); \
        R_tween_add_float(step, value, R_user_data(source), \
                          source_get_ ## NAME, source_set_ ## NAME, \
                          tween_al_source_free, source_ ## NAME ## _to_json); \
    }

DEF_AL_SOURCE_TWEEN(gain)
DEF_AL_SOURCE_TWEEN(pitch)
DEF_AL_SOURCE_TWEEN(reference_distance)
DEF_AL_SOURCE_TWEEN(pos_x)
DEF_AL_SOURCE_TWEEN(pos_y)
DEF_AL_SOURCE_TWEEN(pos_z)
DEF_AL_SOURCE_TWEEN(velocity_x)
DEF_AL_SOURCE_TWEEN(velocity_y)
DEF_AL_SOURCE_TWEEN(velocity_z)
