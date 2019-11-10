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
} R_AffineTransform;

typedef void (*R_DrawFn)(NVGcontext *, const float[static 6], R_UserData);

typedef struct R_Sprite R_Sprite;


/*
 * Get a default, "empty" affine transform: the origin and position at (0,0),
 * scale at (1,1) and an angle of 0 rad (and therefore 0 degrees).
 */
R_AffineTransform R_affine_transform(void);

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

/*
 * Create a new sprite with zero transforms that can't be added as a child to
 * any other sprite. Probably totally useless to you, but `R_Canvas` uses this
 * to make its root sprite.
 *
 * Root sprites can't have a parent or be drawn to. They have zero transforms.
 * Internally, they're implemented as a sprite that has itself as its parent,
 * since that's an impossible state by any other means.
 */
R_Sprite *R_sprite_new_root(void);

/* Tells you if the given `sprite` is a root sprite. */
bool R_sprite_is_root(R_Sprite *sprite);

/* Increment the `sprite`'s reference count. */
R_Sprite *R_sprite_decref(R_Sprite *sprite);
/* Decrement the `sprite`'s reference count, freeing it if it reaches 0. */
R_Sprite *R_sprite_incref(R_Sprite *sprite);
/* Returns the `sprite`'s current reference count. */
int R_sprite_refs(R_Sprite *sprite);


const char *R_sprite_name(R_Sprite *sprite);


/*
 * Make the `sprite` call the given `draw` function when it's being rendered.
 * The given `user` data will be passed to the function when that happens,
 * along with the nanovg context and the sprite's transformation matrix. If
 * `draw` is `NULL`, nothing will be rendered for the sprite itself, but its
 * children will be. Setting anything but `NULL` on a root sprite `R_die`s.
 */
void R_sprite_draw_fn(R_Sprite *sprite, R_DrawFn draw, R_UserData user);

void R_sprite_draw_null(R_Sprite *sprite);

/*
 * Make the `sprite` draw the given vector image when rendering it. Calls
 * `R_sprite_draw_fn`, so if you set anything with that it will be clobbered.
 * If `vi` is null, nothing will be rendered for the sprite itself.
 */
void R_sprite_draw_vector_image(R_Sprite *sprite, R_VectorImage *vi);

void R_sprite_draw_text_field(R_Sprite *sprite, R_TextField *field);


void R_sprite_transforms_resize(R_Sprite *sprite, int transform_count);

void R_sprite_transforms_ensure(R_Sprite *sprite, int transform_count);

R_AffineTransform *R_sprite_transform_at(R_Sprite *sprite, int index);


/*
 * Returns the parent of the given `sprite`, or `NULL` if it doesn't have one.
 */
R_Sprite *R_sprite_parent(R_Sprite *sprite);

/*
 * Orphans the given `sprite` if it has a parent. After this, its parent will
 * be `NULL`. Returns the parent it had or `NULL` if it didn't have one. The
 * `sprite`s refcount will be decremented if it had a parent.
 */
R_Sprite *R_sprite_orphan(R_Sprite *sprite);

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
 * Removes the given `child` from the parent `sprite` and decrements its
 * refcount. Will `R_die` if the `sprite` doesn't actually have the `child`
 * in its list of children.
 */
void R_sprite_child_remove(R_Sprite *sprite, R_Sprite *child);


void R_sprite_draw(R_Sprite *sprite, NVGcontext *vg,
                   const float parent_matrix[static 6]);
