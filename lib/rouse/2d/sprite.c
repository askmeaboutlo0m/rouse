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
#include "../geom.h"
#include "../parse.h"
#include "vector.h"
#include "sprite.h"

struct R_Sprite {
    R_MAGIC_FIELD
    char              *name;
    R_DrawFn          draw;
    R_UserData        user;
    R_Sprite          *parent;
    R_Sprite          *children;
    R_Sprite          *next;
    int               refs;
    int               transform_count;
    R_AffineTransform *transforms;
};


R_AffineTransform R_affine_transform(void)
{
    return (R_AffineTransform){
            R_MAGIC_INIT_TYPE(R_AffineTransform) {{0.0f, 0.0f}},
            {{0.0f, 0.0f}}, {{1.0f, 1.0f}}, {{0.0f, 0.0f}}, 0.0f};
}


static void init_affine_transform(R_AffineTransform *transform)
{
    *transform = R_affine_transform();
    R_MAGIC_CHECK(transform);
}

static inline void check_sprite(R_Sprite *sprite)
{
    R_MAGIC_CHECK(sprite);
    assert(sprite->refs > 0 && "sprite refcount must always be positive");
}

static inline void check_parent_child(R_Sprite *parent, R_Sprite *child)
{
    check_sprite(parent);
    check_sprite(child);
    assert(parent != child && "parent can't be a child of itself");
}

R_Sprite *R_sprite_new(const char *name)
{
    R_Sprite *sprite = R_NEW_INIT_STRUCT(sprite, R_Sprite,
        R_MAGIC_INIT(sprite) R_strdup(name), NULL, R_user_null(),
        NULL, NULL, NULL, 1, 0, NULL);
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
    R_MAGIC_CHECK(sprite);
    return sprite->parent == sprite;
}


static void free_sprite(R_Sprite *sprite)
{
    for (R_Sprite *child = sprite->children, *next; child; child = next) {
        next = child->next;
        R_sprite_decref(child);
    }
    R_MAGIC_POISON(sprite);
    free(sprite->name);
    free(sprite->transforms);
    free(sprite);
}

R_Sprite *R_sprite_decref(R_Sprite *sprite)
{
    check_sprite(sprite);
    if (--sprite->refs == 0) {
        free_sprite(sprite);
        return NULL;
    }
    else {
        return sprite;
    }
}

R_Sprite *R_sprite_incref(R_Sprite *sprite)
{
    check_sprite(sprite);
    ++sprite->refs;
    return sprite;
}

int R_sprite_refs(R_Sprite *sprite)
{
    check_sprite(sprite);
    return sprite->refs;
}


void R_sprite_draw_fn(R_Sprite *sprite, R_DrawFn draw, R_UserData user)
{
    R_MAGIC_CHECK(sprite);
    if (draw && R_sprite_is_root(sprite)) {
        R_die("Can't assign a draw function to a root sprite");
    }
    sprite->draw = draw;
    sprite->user = user;
}

static void draw_vector_image(NVGcontext *vg, const float m[static 6],
                              R_UserData user)
{
    R_vector_image_draw(user.data, vg, m);
}

void R_sprite_draw_vector_image(R_Sprite *sprite, R_VectorImage *vi)
{
    R_sprite_draw_fn(sprite, vi ? draw_vector_image : NULL, R_user_data(vi));
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
    assert(transform_count >= 0 && "transform count must not be negative");
    assert(!R_sprite_is_root(sprite) && "no transforms on root sprite");
    if (sprite->transform_count != transform_count) {
        resize_transforms(sprite, transform_count);
    }
}

void R_sprite_transforms_ensure(R_Sprite *sprite, int transform_count)
{
    check_sprite(sprite);
    assert(!R_sprite_is_root(sprite) && "no transforms on root sprite");
    if (sprite->transform_count < transform_count) {
        resize_transforms(sprite, transform_count);
    }
}

R_AffineTransform *R_sprite_transform_at(R_Sprite *sprite, int index)
{
    R_MAGIC_CHECK(sprite);
    assert(index >= 0 && "transform index must not be negative");
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
    assert(index >= 0 && "child index must not be negative");
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

void R_sprite_child_remove(R_Sprite *sprite, R_Sprite *child)
{
    check_parent_child(sprite, child);
    assert(sprite == child->parent && "child can only be removed from parent");

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

static void draw_self(R_Sprite *sprite, NVGcontext *vg,
                      const float m[static 6])
{
    R_DrawFn draw = sprite->draw;
    if (draw) {
        draw(vg, m, sprite->user);
    }
}

static void draw_children(R_Sprite *sprite, NVGcontext *vg,
                          const float matrix[static 6])
{
    for (R_Sprite *child = sprite->children; child; child = child->next) {
        R_sprite_draw(child, vg, matrix);
    }
}

void R_sprite_draw(R_Sprite *sprite, NVGcontext *vg,
                   const float parent_matrix[static 6])
{
    R_MAGIC_CHECK(sprite);
    float matrix[6];
    calc_matrix(sprite, matrix, parent_matrix);
    draw_self(sprite, vg, matrix);
    draw_children(sprite, vg, matrix);
}
