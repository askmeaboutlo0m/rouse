/*
 * anim/delay.h - delay sequence steps.
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

typedef float (*R_DelayCalcFn)(R_UserData, R_StepTickArgs);
typedef void  (*R_DelayFreeFn)(R_UserData user, R_UserData *seq_user);

/*
 * Delay for the specified number of `seconds`. A negative value is treated
 * like a delay of zero.
 */
R_Step *R_delay_new_fixed(float seconds);

/*
 * Delay for a random value between `a` and `b`, recalculated every time this
 * step is hit. It doesn't matter which one is the smaller and which one is
 * the larger value. Negative results are a delay of zero. This uses
 * `R_rand_between`, so it depends on `R_seed`.
 */
R_Step *R_delay_new_between(float a, float b);

/*
 * A delay that calls a custom function to recalculate the delay amount every
 * time this step is hit. The `on_calc` function must return the delay amount
 * in seconds. The optional `on_free` function is called when this step is
 * freed, allowing you to free your `user` parameter. The first parameter of
 * `on_free` is the given `user` parameter, the second one is the user data
 * of the sequence this step belongs to (if any).
 */
R_Step *R_delay_new_custom(R_DelayCalcFn on_calc, R_DelayFreeFn on_free,
                           R_UserData user);