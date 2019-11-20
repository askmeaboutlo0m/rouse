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
#include <limits.h>
#include <assert.h>
#include <cglm/struct.h>
#include "../3rdparty/nanovg_inc.h"
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
    R_SpriteDrawFn    on_draw;
    R_SpriteFreeFn    on_free;
    R_UserData        user;
    R_Sprite          *parent;
    R_Sprite          *children;
    R_Sprite          *next;
    int               transform_count;
    R_AffineTransform *transforms;
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
            R_MAGIC_INIT(R_Sprite) 1, R_strdup(name), NULL, NULL,
            R_user_null(), NULL, NULL, NULL, 0, NULL);
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
        R_sprite_decref(child);
    }
}

static void free_content(R_SpriteFreeFn on_free, R_UserData user)
{
    if (on_free) {
        on_free(user);
    }
}

static void free_sprite(R_Sprite *sprite)
{
    free_children(sprite->children);
    free_content(sprite->on_free, sprite->user);
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


void R_sprite_draw_fn(R_Sprite *sprite, R_SpriteDrawFn on_draw,
                      R_SpriteFreeFn on_free, R_UserData user)
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    if ((on_draw || on_free) && R_sprite_is_root(sprite)) {
        R_die("Can't assign a draw function to a root sprite");
    }
    sprite->on_draw = on_draw;
    sprite->on_free = on_free;
    sprite->user    = user;
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


static void draw_bitmap_image(NVGcontext *ctx, const float m[static 6],
                              R_UserData user)
{
    R_bitmap_image_draw(user.data, ctx, m);
}

static void free_bitmap_image(R_UserData user)
{
    R_bitmap_image_decref(user.data);
}

void R_sprite_draw_bitmap_image(R_Sprite *sprite, R_BitmapImage *bi)
{
    MAYBE_SET_DRAW(sprite, bitmap_image, bi);
}


static void draw_vector_image(NVGcontext *ctx, const float m[static 6],
                              R_UserData user)
{
    R_vector_image_draw(user.data, ctx, m);
}

static void free_vector_image(R_UserData user)
{
    R_vector_image_decref(user.data);
}

void R_sprite_draw_vector_image(R_Sprite *sprite, R_VectorImage *vi)
{
    MAYBE_SET_DRAW(sprite, vector_image, vi);
}


static void draw_text_field(NVGcontext *ctx, const float m[static 6],
                            R_UserData user)
{
    R_text_field_draw(user.data, ctx, m);
}

static void free_text_field(R_UserData user)
{
    R_text_field_decref(user.data);
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

R_AffineTransform *R_sprite_transform_at(R_Sprite *sprite, int index)
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    R_assert(index >= 0, "transform index must not be negative");
    R_sprite_transforms_ensure(sprite, index + 1);
    return &sprite->transforms[index];
}


R_Sprite *R_sprite_parent(R_Sprite *sprite)
{
    check_sprite(sprite);
    return R_sprite_is_root(sprite) ? NULL : sprite->parent;
}

R_Sprite *R_sprite_orphan(R_Sprite *sprite)
{
    check_sprite(sprite);
    if (R_sprite_is_root(sprite)) {
        return NULL;
    }
    else {
        R_Sprite *parent = sprite->parent;
        if (parent) {
            R_sprite_child_remove(parent, sprite);
        }
        return parent;
    }
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

static void calc_matrix(R_Sprite *sprite, float matrix[static 6],
                        const float parent_matrix[static 6])
{
    nvgTransformIdentity(matrix);
    int count = sprite->transform_count;
    for (int i = 0; i < count; ++i) {
        apply_transform(matrix, &sprite->transforms[i]);
    }
    nvgTransformMultiply(matrix, parent_matrix);
}

static void draw_self(R_Sprite *sprite, NVGcontext *ctx,
                      const float m[static 6])
{
    R_SpriteDrawFn on_draw = sprite->on_draw;
    if (on_draw) {
        on_draw(ctx, m, sprite->user);
    }
}

static void draw_children(R_Sprite *sprite, NVGcontext *ctx,
                          const float matrix[static 6])
{
    for (R_Sprite *child = sprite->children; child; child = child->next) {
        R_sprite_draw(child, ctx, matrix);
    }
}

void R_sprite_draw(R_Sprite *sprite, NVGcontext *ctx,
                   const float parent_matrix[static 6])
{
    R_MAGIC_CHECK(R_Sprite, sprite);
    float matrix[6];
    calc_matrix(sprite, matrix, parent_matrix);
    draw_self(sprite, ctx, matrix);
    draw_children(sprite, ctx, matrix);
}
