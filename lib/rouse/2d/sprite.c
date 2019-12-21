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
 * ______________________________________________________________________________
 *
 * The transform id system is based on PixiJS, which can be found at
 * https://www.pixijs.com/. It's licensed under the following terms:
 *
 * The MIT License
 *
 * Copyright (c) 2013-2017 Mathew Groves, Chad Engler
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdnoreturn.h>
#include <limits.h>
#include <assert.h>
#include <cglm/struct.h>
#include "../3rdparty/nanovg_inc.h"
#include <rouse_config.h>
#include "../common.h"
#include "../string.h"
#include "../geom.h"
#include "../parse.h"
#include "nvg.h"
#include "bitmap.h"
#include "text.h"
#include "vector.h"
#include "refcount.h"
#include "sprite.h"

struct R_Sprite {
    R_MAGIC_FIELD
    int               refs;
    char              *name;
    R_Sprite          *parent;
    R_Sprite          *children;
    R_Sprite          *next;
    int               transform_count;
    R_AffineTransform *transforms;
    /* These id things keep track of which transforms need to be recalculated.
     * This system is heavily inspired by the way PixiJS works. Each sprite
     * keeps track of its local matrix, which is calculated from its own affine
     * transforms, and its world matrix, which is calculated by multiplying the
     * local matrix with its parent's world matrix. A sprite that has no parent
     * has identical local and world matrices.
     *
     * The current_id is incremented whenever something about the sprite's
     * affine transforms changes. When current_id is different from local_id,
     * the local matrix is recalculated and local_id is updated to current_id.
     *
     * Whenever the local matrix changes or the parent_id is different from the
     * parent's world_id, the world matrix is recalculated. When that happens,
     * the world_id is incremented, invalidating all children's parent_id and
     * making them recalculate their world matrices as well. */
    int               current_id, local_id, world_id, parent_id;
    float             local[6], world[6];
    R_SpriteFreeFn    on_free;
    R_UserData        user;
    struct {
        R_SpriteDrawFn    on_draw;
        R_SpriteFreeFn    on_free;
        R_UserData        user;
    } content;
};


R_AffineTransform R_affine_transform(void)
{
    return (R_AffineTransform){
            R_MAGIC_INIT(R_AffineTransform) {{0.0f, 0.0f}},
            {{0.0f, 0.0f}}, {{1.0f, 1.0f}}, {{0.0f, 0.0f}}, 0.0f};
}


static void init_affine_transform(R_AffineTransform *transform)
{
    *transform = R_affine_transform();
    R_MAGIC_CHECK(R_AffineTransform, transform);
}

static inline void check_sprite(R_UNUSED_UNLESS_DEBUG R_Sprite *sprite)
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    R_assert(sprite->refs > 0, "refcount must always be positive");
}

static inline void check_parent_child(R_Sprite *parent, R_Sprite *child)
{
    check_sprite(parent);
    check_sprite(child);
    R_assert(parent != child, "parent can't be a child of itself");
}

R_Sprite *R_sprite_new(const char *name)
{
    R_Sprite *sprite = R_NEW_INIT_STRUCT(sprite, R_Sprite,
            R_MAGIC_INIT(R_Sprite) 1, R_strdup(name), NULL, NULL, NULL,
            0, NULL, 0, 0, 0, 0, {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f}, NULL, R_user_null(),
            {NULL, NULL, R_user_null()});
    check_sprite(sprite);
    return sprite;
}


R_Sprite *R_sprite_new_root(void)
{
    R_Sprite *sprite = R_sprite_new(NULL);
    /* A sprite with itself as parent is normally impossible. */
    /* We create that impossibility here as a marker that     */
    /* makes various parent-changing operations impossible.   */
    sprite->parent = sprite;
    return sprite;
}

bool R_sprite_is_root(R_Sprite *sprite)
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    return sprite->parent == sprite;
}


static void free_children(R_Sprite *children)
{
    for (R_Sprite *child = children, *next; child; child = next) {
        next = child->next;
        if (R_sprite_decref(child)) {
            /*
             * FIXME: hrm, orphaning children because their parent isn't
             * referenced anymore is pretty bad. I guess this sprite tree
             * needs some more sophisticated memory management, since it's
             * so self-referential and potentially circular.
             */
            child->parent = NULL;
        }
    }
}

static void free_user(R_SpriteFreeFn on_free, R_UserData user)
{
    if (on_free) {
        on_free(user);
    }
}

static void free_sprite(R_Sprite *sprite)
{
    free_children(sprite->children);
    free_user(sprite->on_free, sprite->user);
    free_user(sprite->content.on_free, sprite->content.user);
    free(sprite->name);
    free(sprite->transforms);
    R_MAGIC_POISON(R_Sprite, sprite);
    free(sprite);
}

R_DEFINE_REFCOUNT_FUNCS(R_Sprite, sprite, refs)


const char *R_sprite_name(R_Sprite *sprite)
{
    check_sprite(sprite);
    return sprite->name;
}

R_UserData R_sprite_user(R_Sprite *sprite)
{
    check_sprite(sprite);
    return sprite->user;
}

void R_sprite_user_set(R_Sprite *sprite, R_UserData user,
                       R_SpriteFreeFn on_free)
{
    check_sprite(sprite);
    sprite->user    = user;
    sprite->on_free = on_free;
}


void R_sprite_draw_fn(R_Sprite *sprite, R_SpriteDrawFn on_draw,
                      R_SpriteFreeFn on_free, R_UserData draw_user)
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    if ((on_draw || on_free) && R_sprite_is_root(sprite)) {
        R_die("Can't assign a draw function to a root sprite");
    }
    sprite->content.on_draw = on_draw;
    sprite->content.on_free = on_free;
    sprite->content.user    = draw_user;
}

void R_sprite_draw_null(R_Sprite *sprite)
{
    R_sprite_draw_fn(sprite, NULL, NULL, R_user_null());
}


#define MAYBE_SET_DRAW(SPRITE, NAME, DATA) do { \
        if (DATA) { \
            R_sprite_draw_fn(SPRITE, draw_ ## NAME, free_ ## NAME, \
                             R_user_data(R_ ## NAME ## _incref(DATA))); \
        } \
        else { \
            R_sprite_draw_null(SPRITE); \
        } \
    } while (0)


static void draw_bitmap_image(R_Nvg *nvg, const float m[static 6],
                              R_UserData draw_user)
{
    R_bitmap_image_draw(draw_user.data, R_nvg_context(nvg), m);
}

static void free_bitmap_image(R_UserData draw_user)
{
    R_bitmap_image_decref(draw_user.data);
}

void R_sprite_draw_bitmap_image(R_Sprite *sprite, R_BitmapImage *bi)
{
    MAYBE_SET_DRAW(sprite, bitmap_image, bi);
}


static void draw_vector_image(R_Nvg *nvg, const float m[static 6],
                              R_UserData draw_user)
{
    R_vector_image_draw(draw_user.data, R_nvg_context(nvg), m);
}

static void free_vector_image(R_UserData draw_user)
{
    R_vector_image_decref(draw_user.data);
}

void R_sprite_draw_vector_image(R_Sprite *sprite, R_VectorImage *vi)
{
    MAYBE_SET_DRAW(sprite, vector_image, vi);
}


static void draw_text_field(R_Nvg *nvg, const float m[static 6],
                            R_UserData draw_user)
{
    R_text_field_draw(draw_user.data, R_nvg_context(nvg), m);
}

static void free_text_field(R_UserData draw_user)
{
    R_text_field_decref(draw_user.data);
}

void R_sprite_draw_text_field(R_Sprite *sprite, R_TextField *field)
{
    MAYBE_SET_DRAW(sprite, text_field, field);
}


static void resize_transforms(R_Sprite *sprite, int transform_count)
{
    int prev_count          = sprite->transform_count;
    sprite->transform_count = transform_count;
    sprite->transforms      = R_realloc(sprite->transforms,
        R_int2size(transform_count) * sizeof(*sprite->transforms));
    for (int i = prev_count; i < transform_count; ++i) {
        init_affine_transform(&sprite->transforms[i]);
    }
}

void R_sprite_transforms_resize(R_Sprite* sprite, int transform_count)
{
    R_assert(transform_count >= 0, "transform count must not be negative");
    R_assert(!R_sprite_is_root(sprite), "no transforms on root sprite");
    if (sprite->transform_count != transform_count) {
        resize_transforms(sprite, transform_count);
    }
}

void R_sprite_transforms_ensure(R_Sprite *sprite, int transform_count)
{
    check_sprite(sprite);
    R_assert(!R_sprite_is_root(sprite), "no transforms on root sprite");
    if (sprite->transform_count < transform_count) {
        resize_transforms(sprite, transform_count);
    }
}


static R_AffineTransform *check_transform(R_Sprite *sprite, int index)
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    R_assert(index >= 0, "transform index must not be negative");
    R_sprite_transforms_ensure(sprite, index + 1);
    return &sprite->transforms[index];
}

R_AffineTransform R_sprite_transform_at(R_Sprite *sprite, int index)
{
    return *check_transform(sprite, index);
}

void R_sprite_transform_set(R_Sprite *sprite, int index, R_AffineTransform tf)
{
    *check_transform(sprite, index) = tf;
    ++sprite->current_id;
}

#define GET_TRANSFORM(TYPE, NAME, ACCESS) \
    TYPE R_sprite_ ## NAME ## _at(R_Sprite *sprite, int index) \
    { \
        return check_transform(sprite, index)->ACCESS; \
    }

#define SET_TRANSFORM(TYPE, NAME, ACCESS) \
    void R_sprite_ ## NAME ## _set(R_Sprite *sprite, int index, TYPE value) \
    { \
        check_transform(sprite, index)->ACCESS = value; \
        ++sprite->current_id; \
    }

#define GET_SET_TRANSFORM(TYPE, NAME, ACCESS) \
    GET_TRANSFORM(TYPE, NAME, ACCESS) \
    SET_TRANSFORM(TYPE, NAME, ACCESS)

GET_SET_TRANSFORM(R_V2,  origin,   origin)
GET_SET_TRANSFORM(float, origin_x, origin.x)
GET_SET_TRANSFORM(float, origin_y, origin.y)
GET_SET_TRANSFORM(R_V2,  pos,      pos)
GET_SET_TRANSFORM(float, pos_x,    pos.x)
GET_SET_TRANSFORM(float, pos_y,    pos.y)
GET_SET_TRANSFORM(R_V2,  scale,    scale)
GET_SET_TRANSFORM(float, scale_x,  scale.x)
GET_SET_TRANSFORM(float, scale_y,  scale.y)
GET_SET_TRANSFORM(R_V2,  skew,     skew)
GET_SET_TRANSFORM(float, skew_x,   skew.x)
GET_SET_TRANSFORM(float, skew_y,   skew.y)
GET_SET_TRANSFORM(float, angle,    angle)

float R_sprite_rotation_at(R_Sprite *sprite, int index)
{
    return R_to_deg(check_transform(sprite, index)->angle);
}

void R_sprite_rotation_set(R_Sprite *sprite, int index, float value)
{
    check_transform(sprite, index)->angle = R_to_rad(value);
    ++sprite->current_id;
}


static void apply_transform(float matrix[static 6], R_AffineTransform *tf)
{
    float tmp[6];
    R_V2  origin = tf->origin;
    nvgTransformTranslate(tmp, -origin.x, -origin.y);
    nvgTransformMultiply(matrix, tmp);
    nvgTransformScale(tmp, tf->scale.x, tf->scale.y);
    nvgTransformMultiply(matrix, tmp);
    nvgTransformSkewX(tmp, tf->skew.x);
    nvgTransformMultiply(matrix, tmp);
    nvgTransformSkewY(tmp, tf->skew.y);
    nvgTransformMultiply(matrix, tmp);
    nvgTransformRotate(tmp, tf->angle);
    nvgTransformMultiply(matrix, tmp);
    nvgTransformTranslate(tmp, tf->pos.x + origin.x, tf->pos.y + origin.y);
    nvgTransformMultiply(matrix, tmp);
}

/* Watch out: this does NOT set the parent_id to a new value! You either need
 * to call `calc_world` right after or set the parent_id to -1 yourself. */
static void calc_local(R_Sprite *sprite)
{
    float *local = sprite->local;
    nvgTransformIdentity(local);
    int count = sprite->transform_count;
    for (int i = 0; i < count; ++i) {
        apply_transform(local, &sprite->transforms[i]);
    }
    sprite->local_id = sprite->current_id;
}

static void calc_world(R_Sprite *sprite, R_Sprite *parent)
{
    float *world = sprite->world;
    memcpy(world, sprite->local, sizeof(float[6]));
    nvgTransformMultiply(world, parent->world);
    sprite->parent_id = parent->world_id;
    ++sprite->world_id;
}

static float *update_world(R_Sprite *sprite, bool update_parent)
{
    R_Sprite *parent = R_sprite_parent(sprite);
    if (parent) {
        /* If we're e.g. drawing the sprites, we're currently traversing and
         * updating them from the root up anyway. In those cases, we can skip
         * walking back up the tree and re-checking the parents. However, if
         * we're just retrieving the global position in the middle of a tick,
         * there's no walking going on already, so we have to do our own. */
        if (update_parent) {
            update_world(parent, true);
        }
        /* Recalculate matrices only as needed. */
        if (sprite->local_id != sprite->current_id) {
            calc_local(sprite);
            calc_world(sprite, parent);
        }
        else if (sprite->parent_id != parent->world_id) {
            calc_world(sprite, parent);
        }
    }
    else if (sprite->local_id != sprite->current_id) {
        calc_local(sprite);
        memcpy(sprite->world, sprite->local, sizeof(float[6]));
    }
    return sprite->world;
}

static float *get_world(R_Sprite *sprite)
{
    check_sprite(sprite);
    return update_world(sprite, true);
}

R_V2 R_sprite_world_pos(R_Sprite *sprite)
{
    float *world = get_world(sprite);
    return R_v2(world[4], world[5]);
}

float R_sprite_world_x(R_Sprite *sprite)
{
    return get_world(sprite)[4];
}

float R_sprite_world_y(R_Sprite *sprite)
{
    return get_world(sprite)[5];
}



R_Sprite *R_sprite_parent(R_Sprite *sprite)
{
    check_sprite(sprite);
    return R_sprite_is_root(sprite) ? NULL : sprite->parent;
}

bool R_sprite_orphan(R_Sprite *sprite)
{
    check_sprite(sprite);
    if (!R_sprite_is_root(sprite)) {
        R_Sprite *parent = sprite->parent;
        if (parent) {
            return true;
        }
    }
    return false;
}

int R_sprite_child_add(R_Sprite *sprite, R_Sprite *child)
{
    return R_sprite_child_add_at(sprite, child, INT_MAX);
}

int R_sprite_child_add_at(R_Sprite *sprite, R_Sprite *child, int index)
{
    R_assert(index >= 0, "child index must not be negative");
    check_parent_child(sprite, child);
    if (R_sprite_is_root(child)) {
        R_die("Can't treat a canvas root sprite as a child");
    }

    R_sprite_incref(child); /* Increment refcount first, orphaning */
    R_sprite_orphan(child); /* will potentially decrement it.      */

    int        at = 0;
    R_Sprite **pp = &sprite->children;
    while (*pp && at < index) {
        pp = &(*pp)->next;
        ++at;
    }

    child->next = *pp ? (*pp)->next : NULL;
    *pp = child;

    child->parent    = sprite;
    child->parent_id = -1;
    return at;
}

int R_sprite_child_add_noinc(R_Sprite *sprite, R_Sprite *child)
{
    int at = R_sprite_child_add(sprite, child);
    R_sprite_decref(child);
    return at;
}

int R_sprite_child_add_at_noinc(R_Sprite *sprite, R_Sprite *child, int index)
{
    int at = R_sprite_child_add_at(sprite, child, index);
    R_sprite_decref(child);
    return at;
}

void R_sprite_child_remove(R_Sprite *sprite, R_Sprite *child)
{
    check_parent_child(sprite, child);
    R_assert(sprite == child->parent, "child can only be removed from parent");

    for (R_Sprite **pp = &sprite->children; *pp; pp = &(*pp)->next) {
        if (*pp == child) {
            *pp         = child->next;
            child->next = NULL;
            R_sprite_decref(child);
            return;
        }
    }

    R_die("Parent sprite %p does not have child sprite %p with parent %p",
          (void *)sprite, (void *)child, (void *)child->parent);
}


static void draw_self(R_Sprite *sprite, R_Nvg *nvg,
                      const float canvas_matrix[static 6])
{
    R_SpriteDrawFn on_draw = sprite->content.on_draw;
    if (on_draw) {
        float matrix[6];
        memcpy(matrix, update_world(sprite, false), sizeof(float[6]));
        nvgTransformMultiply(matrix, canvas_matrix);
        on_draw(nvg, matrix, sprite->content.user);
    }
    else {
        update_world(sprite, false);
    }
}

static void draw_children(R_Sprite *sprite, R_Nvg *nvg,
                          const float canvas_matrix[static 6])
{
    for (R_Sprite *child = sprite->children; child; child = child->next) {
        R_sprite_draw(child, nvg, canvas_matrix);
    }
}

void R_sprite_draw(R_Sprite *sprite, R_Nvg *nvg,
                   const float canvas_matrix[static 6])
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    draw_self(sprite, nvg, canvas_matrix);
    draw_children(sprite, nvg, canvas_matrix);
}
