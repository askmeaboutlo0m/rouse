/*
 * tween.h - "tweens", i.e. smooth transitions of values over time
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

typedef float (*R_TweenCalcFn)(R_StepTickArgs, R_UserData);
typedef void  (*R_TweenFreeFn)(R_UserData);
typedef void  (*R_TweenJsonFn)(JSON_Object *, R_UserData);

typedef void (*R_TweenElementJsonFn)(JSON_Object *, R_UserData);
typedef void (*R_TweenElementFreeFn)(R_UserData);

typedef void (*R_TweenValueJsonFn)(JSON_Object *, R_UserData);
typedef void (*R_TweenValueFreeFn)(R_UserData);

typedef float (*R_TweenFloatGetFn )(R_UserData);
typedef void  (*R_TweenFloatSetFn )(R_UserData, float);
typedef float (*R_TweenFloatCalcFn)(R_UserData, float);

typedef struct R_TweenFloat {
    R_TweenFloatCalcFn on_calc;
    R_TweenValueFreeFn on_free;
    R_TweenValueJsonFn to_json;
    R_UserData         user;
} R_TweenFloat;


R_Step *R_tween_new(R_TweenCalcFn on_calc, R_TweenFreeFn on_free,
                    R_TweenJsonFn to_json, R_UserData user, R_EaseFn ease);

R_Step *R_tween_new_fixed(float seconds, R_EaseFn ease);
R_Step *R_tween_new_between(float a, float b, R_EaseFn ease);


R_TweenFloat R_tween_float(R_TweenFloatCalcFn on_calc, R_TweenValueFreeFn on_free,
                           R_TweenValueJsonFn to_json, R_UserData user);

R_TweenFloat R_tween_float_fixed(float value);
R_TweenFloat R_tween_float_between(float a, float b);

void R_tween_add_float(R_Step *step, R_TweenFloat f, R_UserData user,
                       R_TweenFloatGetFn get_float, R_TweenFloatSetFn set_float,
                       R_TweenElementFreeFn on_free, R_TweenElementJsonFn to_json);
