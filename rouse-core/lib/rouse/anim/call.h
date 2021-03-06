/*
 * anim/call.h - function call sequence step.
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

typedef void (*R_CallStepFn)(R_StepTickArgs, R_UserData);
typedef void (*R_CallFreeFn)(R_UserData);
typedef void (*R_CallJsonFn)(JSON_Object *, R_UserData);

/*
 * Call the given `on_step` function when this step is hit. The optional
 * `on_free` function can be used to free the given `user` data. The second
 * parameter to this function is the parent sequence's user data (if any).
 * The optional `to_json` function lets you stringify your call in a nice way
 * when the call is dumped to JSON.
 */
R_Step *R_call_new(R_CallStepFn on_step, R_CallFreeFn on_free,
                   R_CallJsonFn to_json, R_UserData user);
