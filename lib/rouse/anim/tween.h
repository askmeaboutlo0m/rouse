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

typedef struct R_TweenElement R_TweenElement;

typedef struct R_TweenCalcArgs {
    void       *elem;
    R_UserData user;
} R_TweenCalcArgs;

typedef struct R_TweenTickArgs {
    void       *elem;
    R_UserData user;
    float      ratio;
} R_TweenTickArgs;

typedef struct R_TweenFreeArgs {
    void       *elem;
    R_UserData user;
} R_TweenFreeArgs;

typedef void (*R_TweenElementCalcFn)(R_TweenCalcArgs);
typedef void (*R_TweenElementTickFn)(R_TweenTickArgs);
typedef void (*R_TweenElementFreeFn)(R_TweenFreeArgs);

typedef float (*R_TweenFloatGetFn)(R_TweenCalcArgs);
typedef void  (*R_TweenFloatSetFn)(R_TweenTickArgs, float);

typedef float (*R_TweenCustomFloatCalcFn)(R_TweenCalcArgs, R_UserData);
typedef void  (*R_TweenCustomFloatFreeFn)(R_UserData);

typedef enum R_TweenValueType {
    R_TWEEN_VALUE_FIXED,
    R_TWEEN_VALUE_BETWEEN,
    R_TWEEN_VALUE_CUSTOM,
} R_TweenValueType;

typedef struct R_TweenFloat {
    R_TweenValueType type;
    union {
        float fixed;
        struct {
            float a, b;
        } between;
        struct {
            R_TweenCustomFloatCalcFn calc;
            R_TweenCustomFloatFreeFn free;
            R_UserData               user;
        } custom;
    };
} R_TweenFloat;


R_Step *R_tween_new_fixed(float seconds, R_EaseFn ease);
R_Step *R_tween_new_between(float a, float b, R_EaseFn ease);


R_TweenFloat R_tween_float_fixed(float value);
R_TweenFloat R_tween_float_between(float a, float b);
R_TweenFloat R_tween_float_custom(R_TweenCustomFloatCalcFn custom_calc,
                                  R_TweenCustomFloatFreeFn custom_free,
                                  R_UserData               custom_user);

void R_tween_add_float(R_Step *step, R_TweenFloat value, R_UserData user,
                       R_TweenFloatGetFn get, R_TweenFloatSetFn set,
                       R_TweenElementFreeFn on_free);
