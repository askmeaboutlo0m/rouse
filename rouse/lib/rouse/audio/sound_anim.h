/*
 * sound_anim.h - audio tweening functions
 *
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
#ifndef ROUSE_CONFIG_H_INCLUDED
#   error "Missing #include of rouse_config.h, must come before al.h"
#endif
#ifndef ROUSE_AL_ENABLED
#   error "ROUSE_AL_ENABLED not defined, al.h is not available"
#endif


void R_tween_al_listener_gain(R_Step *step, R_TweenFloat value);

void R_tween_al_listener_pos_x(R_Step *step, R_TweenFloat value);

void R_tween_al_listener_pos_y(R_Step *step, R_TweenFloat value);

void R_tween_al_listener_pos_z(R_Step *step, R_TweenFloat value);

void R_tween_al_listener_velocity_x(R_Step *step, R_TweenFloat value);

void R_tween_al_listener_velocity_y(R_Step *step, R_TweenFloat value);

void R_tween_al_listener_velocity_z(R_Step *step, R_TweenFloat value);


void R_tween_al_source_gain(R_Step *step, R_AlSource *source,
                            R_TweenFloat value);

void R_tween_al_source_pitch(R_Step *step, R_AlSource *source,
                             R_TweenFloat value);

void R_tween_al_source_reference_distance(R_Step *step, R_AlSource *source,
                                          R_TweenFloat value);

void R_tween_al_source_pos_x(R_Step *step, R_AlSource *source,
                             R_TweenFloat value);

void R_tween_al_source_pos_y(R_Step *step, R_AlSource *source,
                             R_TweenFloat value);

void R_tween_al_source_pos_z(R_Step *step, R_AlSource *source,
                             R_TweenFloat value);

void R_tween_al_source_velocity_x(R_Step *step, R_AlSource *source,
                                  R_TweenFloat value);

void R_tween_al_source_velocity_y(R_Step *step, R_AlSource *source,
                                  R_TweenFloat value);

void R_tween_al_source_velocity_z(R_Step *step, R_AlSource *source,
                                  R_TweenFloat value);
