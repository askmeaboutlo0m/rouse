/*
 * ease.h - easing functions
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
 * ______________________________________________________________________________
 *
 * These easing functions are based on Robert Penner's easing functions, which
 * can be found at http://robertpenner.com/easing/. They're licensed under the
 * following terms:
 *
 * Copyright © 2001 Robert Penner
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

typedef float (*R_EaseFn)(float);

float R_ease_linear(float k);

float R_ease_bounce_in(float k);
float R_ease_bounce_out(float k);
float R_ease_bounce_in_out(float k);

float R_ease_cubic_in(float k);
float R_ease_cubic_out(float k);
float R_ease_cubic_in_out(float k);

float R_ease_elastic_in(float k);
float R_ease_elastic_out(float k);
float R_ease_elastic_in_out(float k);

float R_ease_back_in(float k);
float R_ease_back_out(float k);
float R_ease_back_in_out(float k);

float R_ease_quint_in(float k);
float R_ease_quint_out(float k);
float R_ease_quint_in_out(float k);

float R_ease_quad_in(float k);
float R_ease_quad_out(float k);
float R_ease_quad_in_out(float k);

float R_ease_quart_in(float k);
float R_ease_quart_out(float k);
float R_ease_quart_in_out(float k);

float R_ease_sine_in(float k);
float R_ease_sine_out(float k);
float R_ease_sine_in_out(float k);

float R_ease_expo_in(float k);
float R_ease_expo_out(float k);
float R_ease_expo_in_out(float k);
