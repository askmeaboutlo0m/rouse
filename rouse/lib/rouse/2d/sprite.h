/*
 * sprite.h - 2D sprite rendering and transformage
 *
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

typedef struct R_AffineTransform {
    R_MAGIC_FIELD
    R_V2  origin;
    R_V2  pos;
    R_V2  scale;
    R_V2  skew;  /* in radians */
    float angle; /* in radians */
    R_V2  base;
} R_AffineTransform;

typedef void (*R_SpriteDrawFn)(R_Nvg *, const float[static 6], R_UserData);
typedef void (*R_SpriteFreeFn)(R_UserData);

typedef struct R_Sprite R_Sprite;


/*
 * Get a default, "empty" affine transform: the origin and position at (0,0),
 * scale at (1,1) and an angle of 0 rad (and therefore 0 degrees).
 */
R_AffineTransform R_affine_transform(void);

static inline float R_affine_transform_rotation(const R_AffineTransform *tf)
{
    return R_to_deg(tf->angle);
}

static inline R_V2 R_affine_transform_rel(const R_AffineTransform *tf)
{
    return R_v2_sub(tf->pos, tf->base);
}

static inline float R_affine_transform_rel_x(const R_AffineTransform *tf)
{
    return tf->pos.x - tf->base.x;
}

static inline float R_affine_transform_rel_y(const R_AffineTransform *tf)
{
    return tf->pos.y - tf->base.y;
}

static inline void R_affine_transform_rotation_set(R_AffineTransform *tf,
                                                    float value)
{
    tf->angle = R_to_rad(value);
}

static inline void R_affine_transform_rel_set(R_AffineTransform *tf, R_V2 value)
{
    tf->pos = R_v2_add(tf->base, value);
}

static inline void R_affine_transform_rel_x_set(R_AffineTransform *tf,
                                                float value)
{
    tf->pos.x = tf->base.x + value;
}

static inline void R_affine_transform_rel_y_set(R_AffineTransform *tf,
                                                float value)
{
    tf->pos.y = tf->base.y + value;
}


/*
 * Create a new sprite with the given `name`, which may be `NULL` and will be
 * copied.
 *
 * Sprites are reference counted. A sprite always starts with a reference count
 * of 1. You must call `R_sprite_decref` when you're done with it. Operations
 * such as adding a sprite to a parent sprite will increment its reference
 * count and orphaning it will decrement it again.
 *
 * If you're gonna pick up a sprite and do any kind of operation on it that may
 * change its parent, make sure you call `R_sprite_incref` to increment its
 * reference count so that it doesn't suddenly free itself while you're fiddling
 * with it. And of course don't forget to decrement the reference count again
 * when you're done. If you're not doing anything involving setting the parent,
 * then you don't need to change the reference count.
 */
R_Sprite *R_sprite_new(const char *name);

/* Increment the `sprite`'s reference count. */
R_Sprite *R_sprite_decref(R_Sprite *sprite);
/* Decrement the `sprite`'s reference count, freeing it if it reaches 0. */
R_Sprite *R_sprite_incref(R_Sprite *sprite);
/* Returns the `sprite`'s current reference count. */
int R_sprite_refs(R_Sprite *sprite);


const char *R_sprite_name(R_Sprite *sprite);

void R_sprite_name_set(R_Sprite *sprite, const char *name);


R_UserData R_sprite_user(R_Sprite *sprite);

void R_sprite_user_set(R_Sprite *sprite, R_UserData user,
                       R_SpriteFreeFn on_free);


/*
 * Make the `sprite` call the given `on_draw` function when it's being rendered.
 * The given `draw_user` data will be passed to the function when that happens,
 * along with the nanovg context and the sprite's transformation matrix. When
 * the `sprite` is freed, the `on_free` function is called if given.
 *
 * If `on_draw` is `NULL`, nothing will be rendered for the sprite itself, but
 * its children will be. If `on_free` is `NULL` then no attempt will be made to
 * call it.
 */
void R_sprite_draw_fn(R_Sprite *sprite, R_SpriteDrawFn on_draw,
                      R_SpriteFreeFn on_free, R_UserData draw_user);

void R_sprite_draw_null(R_Sprite *sprite);

void R_sprite_draw_bitmap_image(R_Sprite *sprite, R_BitmapImage *bi);

/*
 * Make the `sprite` draw the given vector image when rendering it. Calls
 * `R_sprite_draw_fn`, so if you set anything with that it will be clobbered.
 * If `vi` is null, nothing will be rendered for the sprite itself.
 */
void R_sprite_draw_vector_image(R_Sprite *sprite, R_VectorImage *vi);

void R_sprite_draw_text_field(R_Sprite *sprite, R_TextField *field);


R_AffineTransform R_sprite_transform(R_Sprite *sprite);

R_V2  R_sprite_origin  (R_Sprite *sprite);
float R_sprite_origin_x(R_Sprite *sprite);
float R_sprite_origin_y(R_Sprite *sprite);
R_V2  R_sprite_pos     (R_Sprite *sprite);
float R_sprite_pos_x   (R_Sprite *sprite);
float R_sprite_pos_y   (R_Sprite *sprite);
R_V2  R_sprite_scale   (R_Sprite *sprite);
float R_sprite_scale_x (R_Sprite *sprite);
float R_sprite_scale_y (R_Sprite *sprite);
R_V2  R_sprite_skew    (R_Sprite *sprite);
float R_sprite_skew_x  (R_Sprite *sprite);
float R_sprite_skew_y  (R_Sprite *sprite);
float R_sprite_angle   (R_Sprite *sprite); /* radians */
float R_sprite_rotation(R_Sprite *sprite); /* degrees */
R_V2  R_sprite_base    (R_Sprite *sprite);
float R_sprite_base_x  (R_Sprite *sprite);
float R_sprite_base_y  (R_Sprite *sprite);
R_V2  R_sprite_rel     (R_Sprite *sprite);
float R_sprite_rel_x   (R_Sprite *sprite);
float R_sprite_rel_y   (R_Sprite *sprite);

void R_sprite_transform_set(R_Sprite *sprite, R_AffineTransform tf);

void R_sprite_origin_set  (R_Sprite *sprite, R_V2  value);
void R_sprite_origin_x_set(R_Sprite *sprite, float value);
void R_sprite_origin_y_set(R_Sprite *sprite, float value);
void R_sprite_pos_set     (R_Sprite *sprite, R_V2  value);
void R_sprite_pos_x_set   (R_Sprite *sprite, float value);
void R_sprite_pos_y_set   (R_Sprite *sprite, float value);
void R_sprite_scale_set   (R_Sprite *sprite, R_V2  value);
void R_sprite_scale_x_set (R_Sprite *sprite, float value);
void R_sprite_scale_y_set (R_Sprite *sprite, float value);
void R_sprite_skew_set    (R_Sprite *sprite, R_V2  value);
void R_sprite_skew_x_set  (R_Sprite *sprite, float value);
void R_sprite_skew_y_set  (R_Sprite *sprite, float value);
void R_sprite_angle_set   (R_Sprite *sprite, float value); /* radians */
void R_sprite_rotation_set(R_Sprite *sprite, float value); /* degrees */
void R_sprite_base_set    (R_Sprite *sprite, R_V2  value);
void R_sprite_base_x_set  (R_Sprite *sprite, float value);
void R_sprite_base_y_set  (R_Sprite *sprite, float value);
void R_sprite_rel_set     (R_Sprite *sprite, R_V2  value);
void R_sprite_rel_x_set   (R_Sprite *sprite, float value);
void R_sprite_rel_y_set   (R_Sprite *sprite, float value);

R_V2  R_sprite_world_pos  (R_Sprite *sprite);
float R_sprite_world_pos_x(R_Sprite *sprite);
float R_sprite_world_pos_y(R_Sprite *sprite);

R_V2  R_sprite_world_origin  (R_Sprite *sprite);
float R_sprite_world_origin_x(R_Sprite *sprite);
float R_sprite_world_origin_y(R_Sprite *sprite);

/*
 * Returns the parent of the given `sprite`, or `NULL` if it doesn't have one.
 */
R_Sprite *R_sprite_parent(R_Sprite *sprite);

/*
 * Orphans the given `sprite` if it has a parent. After this, its parent will
 * be `NULL`. Returns if the sprite had a parent or not.
 */
bool R_sprite_orphan(R_Sprite *sprite);

/*
 * Adds the given `child` to the end of the parent `sprite`'s child list.
 * Increments the `child`'s refcount. If it had a parent before, it will be
 * removed from there. In that case its refcount will stay the same. Returns
 * the index that the child was added at.
 */
int R_sprite_child_add(R_Sprite *sprite, R_Sprite *child);

/*
 * Like `R_sprite_child_add`, but instead of at the end it tries to insert
 * the `child` at the given `index`. The returned index may be less than the
 * given `index` if the child list isn't that long. Negative indexes are not
 * sensible and will trigger an assertion if enabled.
 */
int R_sprite_child_add_at(R_Sprite *sprite, R_Sprite *child, int index);

/*
 * These functions are the same as above, but they decrement the reference count
 * after adding the sprite. This is particularly useful for adding the child for
 * the first time, since you usually don't want to stay responsible for it and
 * decrement the reference count right after anyway.
 */
int R_sprite_child_add_noinc   (R_Sprite *sprite, R_Sprite *child);
int R_sprite_child_add_at_noinc(R_Sprite *sprite, R_Sprite *child, int index);

/*
 * Removes the given `child` from the parent `sprite` and decrements its
 * refcount. Will `R_die` if the `sprite` doesn't actually have the `child`
 * in its list of children.
 */
void R_sprite_child_remove(R_Sprite *sprite, R_Sprite *child);

int R_sprite_child_index(R_Sprite *sprite, R_Sprite *child);


void R_sprite_draw(R_Sprite *sprite, R_Nvg *nvg,
                   int logical_width, int logical_height,
                   int target_width, int target_height);
