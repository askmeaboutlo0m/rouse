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
 * ______________________________________________________________________________
 *
 * The functions `temper` and `R_rand` are taken from musl libc, which can be
 * found at https://www.musl-libc.org/. It's licensed under the following terms:
 *
 * Copyright © 2005-2014 Rich Felker, et al.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * A copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * Without limitation the rights to use, copy, modify, merge, publish,
 * Distribute, sublicense, and/or sell copies of the Software, and to
 * Permit persons to whom the Software is furnished to do so, subject to
 * The following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * Included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../common.h"
#include "util.h"


unsigned int R_seed;

/* Taken from musl libc, see notice above. */
static unsigned int temper(unsigned int x)
{
    x ^= x >> 11;
    x ^= x << 7  & 0x9D2C5680;
    x ^= x << 15 & 0xEFC60000;
    x ^= x >> 18;
    return x;
}

/* Taken from musl libc, see notice above. */
int R_rand(void)
{
    return R_uint2int(temper(R_seed = R_seed * 1103515245 + 12345) / 2);
}

static const float RAND_MAX_FLOAT = (float) RAND_MAX;

float R_rand_between(float a, float b)
{
    float low = R_MIN(a, b);
    return low + (R_MAX(a, b) - low) * (R_int2float(rand()) / RAND_MAX_FLOAT);
}
