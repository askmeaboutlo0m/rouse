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


static void tween_sprite_free(R_UserData user)
{
    R_sprite_decref(user.data);
}

#define DEF_SPRITE_TWEEN(NAME) \
    static float get_ ## NAME(R_UserData user) \
    { \
        R_Sprite* sprite = user.data; \
        return R_sprite_ ## NAME(sprite); \
    } \
    \
    static void set_ ## NAME(R_UserData user, float value) \
    { \
        R_Sprite *sprite = user.data; \
        R_sprite_ ## NAME ## _set(sprite, value); \
    } \
    \
    static void NAME ## _to_json(JSON_Object *obj, R_UserData user) \
    { \
        json_object_set_string(obj, "element_type", "sprite_" #NAME); \
        json_object_set_number(obj, "current_" #NAME, get_ ## NAME(user)); \
        const char *name = R_sprite_name(user.data); \
        json_object_set_string(obj, "sprite", name ? name : ""); \
    } \
    \
    void R_tween_sprite_ ## NAME(R_Step *step, R_Sprite *sprite, \
                                 R_TweenFloat value) \
    { \
        R_sprite_incref(sprite); \
        R_tween_add_float(step, value, R_user_data(sprite), get_ ## NAME, \
                          set_ ## NAME, tween_sprite_free, NAME ## _to_json); \
    }

DEF_SPRITE_TWEEN(origin_x)
DEF_SPRITE_TWEEN(origin_y)
DEF_SPRITE_TWEEN(pos_x)
DEF_SPRITE_TWEEN(pos_y)
DEF_SPRITE_TWEEN(scale_x)
DEF_SPRITE_TWEEN(scale_y)
DEF_SPRITE_TWEEN(skew_x)
DEF_SPRITE_TWEEN(skew_y)
DEF_SPRITE_TWEEN(angle)
DEF_SPRITE_TWEEN(alpha)
DEF_SPRITE_TWEEN(base_x)
DEF_SPRITE_TWEEN(base_y)
DEF_SPRITE_TWEEN(rotation)
DEF_SPRITE_TWEEN(rel_x)
DEF_SPRITE_TWEEN(rel_y)


static R_V2 get_scale(R_UserData user)
{
    R_Sprite* sprite = user.data;
    return R_sprite_scale(sprite);
}

static void set_scale(R_UserData user, R_V2 value)
{
    R_Sprite *sprite = user.data;
    R_sprite_scale_set(sprite, value);
}

static void scale_to_json(JSON_Object *obj, R_UserData user)
{
    json_object_set_string(obj, "element_type",  "sprite_scale");
    R_V2 scale = get_scale(user);
    json_object_set_number(obj, "current_scale.x", scale.x);
    json_object_set_number(obj, "current_scale.y", scale.y);
    const char *name = R_sprite_name(user.data);
    json_object_set_string(obj, "sprite", name ? name : "");
}

void R_tween_sprite_scale(R_Step *step, R_Sprite *sprite, R_TweenScale value)
{
    R_sprite_incref(sprite);
    R_tween_add_scale(step, value, R_user_data(sprite), get_scale,
                      set_scale, tween_sprite_free, scale_to_json);
}
