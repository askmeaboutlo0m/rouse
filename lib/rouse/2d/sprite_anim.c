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
#include "../3rdparty/nanovg_inc.h"
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
#include "nvg.h"
#include "bitmap.h"
#include "text.h"
#include "vector.h"
#include "sprite.h"
#include "sprite_anim.h"


typedef struct R_SpriteTweenData {
    R_MAGIC_FIELD
    R_Sprite *sprite;
    int      transform_index;
} R_SpriteTweenData;

static R_UserData make_tween_data(R_Sprite *sprite, int transform_index)
{
    R_sprite_incref(sprite);
    R_SpriteTweenData *data = R_NEW_INIT_STRUCT(data, R_SpriteTweenData,
            R_MAGIC_INIT(R_SpriteTweenData) sprite, transform_index);
    R_MAGIC_CHECK(R_SpriteTweenData, data);
    return R_user_data(data);
}

static void free_tween_data(R_UserData user)
{
    R_SpriteTweenData *data = user.data;
    R_MAGIC_CHECK(R_SpriteTweenData, data);
    R_sprite_decref(data->sprite);
    R_MAGIC_POISON(R_SpriteTweenData, data);
    free(data);
}

static void sprite_tween_data_to_json(JSON_Object *obj, R_SpriteTweenData *data)
{
    R_MAGIC_CHECK(R_SpriteTweenData, data);
    const char *name = R_sprite_name(data->sprite);
    json_object_set_string(obj, "sprite", name ? name : "");
    json_object_set_number(obj, "transform_index",
                           R_int2double(data->transform_index));
}


#define DEF_SPRITE_TWEEN(NAME, FIELD) \
    static float get_ ## NAME(R_UserData user) \
    { \
        R_SpriteTweenData *data = user.data; \
        R_MAGIC_CHECK(R_SpriteTweenData, data); \
        return R_sprite_ ## NAME ## _at(data->sprite, data->transform_index); \
    } \
    \
    static void set_ ## NAME(R_UserData user, float value) \
    { \
        R_SpriteTweenData *data = user.data; \
        R_MAGIC_CHECK(R_SpriteTweenData, data); \
        R_sprite_ ## NAME ## _set(data->sprite, data->transform_index, value); \
    } \
    \
    static void NAME ## _to_json(JSON_Object *obj, R_UserData user) \
    { \
        json_object_set_string(obj, "element_type", "sprite_" #NAME); \
        json_object_set_number(obj, "current_" #NAME, get_ ## NAME(user)); \
        sprite_tween_data_to_json(obj, user.data); \
    } \
    \
    void R_tween_sprite_ ## NAME(R_Step *step, R_Sprite *sprite, \
                                 int transform_index, R_TweenFloat value) \
    { \
        R_tween_add_float(step, value, make_tween_data(sprite, transform_index), \
                          get_ ## NAME, set_ ## NAME, free_tween_data, \
                          NAME ## _to_json); \
    }

DEF_SPRITE_TWEEN(origin_x, origin.x)
DEF_SPRITE_TWEEN(origin_y, origin.y)
DEF_SPRITE_TWEEN(pos_x, pos.x)
DEF_SPRITE_TWEEN(pos_y, pos.y)
DEF_SPRITE_TWEEN(scale_x, scale.x)
DEF_SPRITE_TWEEN(scale_y, scale.y)
DEF_SPRITE_TWEEN(skew_x, skew.x)
DEF_SPRITE_TWEEN(skew_y, skew.y)
DEF_SPRITE_TWEEN(angle, angle)
DEF_SPRITE_TWEEN(rotation, rotation)
