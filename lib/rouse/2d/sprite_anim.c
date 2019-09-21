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
#include "../common.h"
#include "../json.h"
#include "../geom.h"
#include "../parse.h"
#include "../anim/seq.h"
#include "../anim/util.h"
#include "../anim/tween.h"
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
            R_MAGIC_INIT(data) sprite, transform_index);
    R_MAGIC_CHECK(data);
    return R_user_data(data);
}

static void free_tween_data(R_TweenFreeArgs args)
{
    R_SpriteTweenData *data = args.user.data;
    R_MAGIC_CHECK(data);
    R_sprite_decref(data->sprite);
    R_MAGIC_POISON_NN(data);
    free(data);
}

static R_AffineTransform *get_transform(void *user)
{
    R_SpriteTweenData *data = user;
    R_MAGIC_CHECK(data);
    return R_sprite_transform_at(data->sprite, data->transform_index);
}

#define ARGS_TRANSFORM(ARGS) get_transform((ARGS).user.data)


static float get_angle(R_TweenCalcArgs args)
{
    return ARGS_TRANSFORM(args)->angle;
}

static void set_angle(R_TweenTickArgs args, float value)
{
    ARGS_TRANSFORM(args)->angle = value;
}

void R_tween_sprite_angle(R_Step *step, R_Sprite *sprite, int transform_index,
                          R_TweenFloat value)
{
    R_tween_add_float(step, value, make_tween_data(sprite, transform_index),
                      get_angle, set_angle, free_tween_data);
}
