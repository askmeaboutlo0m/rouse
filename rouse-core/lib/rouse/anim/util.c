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
 */
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <rouse_config.h>
#include "../common.h"
#include "util.h"

// The following random number generator is taken from Lua.

#define TRIM64(x)((x) & 0xffffffffffffffffu)

static uint64_t state[4];

/* rotate left 'x' by 'n' bits */
static uint64_t rotl(uint64_t x, int n) {
    return (x << n) | (TRIM64(x) >> (64 - n));
}

static uint64_t nextrand(void) {
    uint64_t state0 = state[0];
    uint64_t state1 = state[1];
    uint64_t state2 = state[2] ^ state0;
    uint64_t state3 = state[3] ^ state1;
    uint64_t res = rotl(state1 * 5, 7) * 9;
    state[0] = state0 ^ state3;
    state[1] = state1 ^ state2;
    state[2] = state2 ^ (state1 << 17);
    state[3] = rotl(state3, 45);
    return res;
}

void R_srand(uint64_t n1, uint64_t n2)
{
    int i;
    state[0] = n1;
    state[1] = 0xff;  /* avoid a zero state */
    state[2] = n2;
    state[3] = 0;
    for (i = 0; i < 16; ++i) {
        nextrand();  /* discard initial values to "spread" seed */
    }
}

/*
** Project the random integer 'ran' into the interval [0, n].
** Because 'ran' has 2^B possible values, the projection can only be
** uniform when the size of the interval is a power of 2 (exact
** division). Otherwise, to get a uniform projection into [0, n], we
** first compute 'lim', the smallest Mersenne number not smaller than
** 'n'. We then project 'ran' into the interval [0, lim].  If the result
** is inside [0, n], we are done. Otherwise, we try with another 'ran',
** until we have a result inside the interval.
*/
static uint64_t project(uint64_t ran, uint64_t n) {
    if ((n & (n + 1)) == 0) {  /* is 'n + 1' a power of 2? */
        return ran & n;  /* no bias */
    }
    else {
        uint64_t lim = n;
        /* compute the smallest (2^b - 1) not smaller than 'n' */
        lim |= (lim >> 1);
        lim |= (lim >> 2);
        lim |= (lim >> 4);
        lim |= (lim >> 8);
        lim |= (lim >> 16);
        lim |= (lim >> 32);  /* integer type has more than 32 bits */
        while ((ran &= lim) > n) {  /* project 'ran' into [0..lim] */
            ran = nextrand();  /* not inside [0..n]? try again */
        }
        return ran;
    }
}

int R_rand(int a, int b)
{
    if (a == b) {
        return a;
    }
    else {
        int lo  = R_MIN(a, b);
        int hi  = R_MAX(a, b);
        int res = lo + R_uint642int(project(nextrand(), R_int2uint64(hi - lo)));
        R_debug("R_rand(%d, %d) = %d", a, b, res);
        R_assert(lo <= res, "R_rand result must not be below lower bound");
        R_assert(hi >= res, "R_rand result must not be above upper bound");
        return res;
    }
}

float R_rand_between(float a, float b)
{
    float lo  = R_MIN(a, b);
    float hi  = R_MAX(a, b);
    float rv  = R_uint642float(nextrand());
    float res = lo + (hi - lo) * (rv / (float) UINT64_MAX);
    R_debug("R_rand_between(%f, %f) = %f", a, b, res);
    R_assert(lo <= res, "R_rand_between result must not be below lower bound");
    R_assert(hi >= res, "R_rand_between result must not be above upper bound");
    return res;
}
