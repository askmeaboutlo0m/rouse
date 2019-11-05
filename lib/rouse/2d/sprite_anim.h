/*
 * sprite_anim.h - sprite animation functions
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

void R_tween_sprite_pos_x(R_Step *step, R_Sprite *sprite, int transform_index,
                          R_TweenFloat value);

void R_tween_sprite_pos_y(R_Step *step, R_Sprite *sprite, int transform_index,
                          R_TweenFloat value);

void R_tween_sprite_shift_x(R_Step *step, R_Sprite *sprite, int transform_index,
                            R_TweenFloat value);

void R_tween_sprite_shift_y(R_Step *step, R_Sprite *sprite, int transform_index,
                            R_TweenFloat value);

void R_tween_sprite_scale_x(R_Step *step, R_Sprite *sprite, int transform_index,
                            R_TweenFloat value);

void R_tween_sprite_scale_y(R_Step *step, R_Sprite *sprite, int transform_index,
                            R_TweenFloat value);

void R_tween_sprite_skew_x(R_Step *step, R_Sprite *sprite, int transform_index,
                           R_TweenFloat value);

void R_tween_sprite_skew_y(R_Step *step, R_Sprite *sprite, int transform_index,
                           R_TweenFloat value);

void R_tween_sprite_angle(R_Step *step, R_Sprite *sprite, int transform_index,
                          R_TweenFloat value);
