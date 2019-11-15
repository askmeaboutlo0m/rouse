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
 * This implementation is based on Troy D. Hanson's utstring library. It's
 * licensed under the following terms:
 *
 * Copyright (c) 2008-2018, Troy D. Hanson   http://troydhanson.github.com/uthash/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include <string.h>
#include <stdio.h>
#include "common.h"
#include "string.h"

struct R_String {
    R_MAGIC_FIELD
    char  *d; /* pointer to allocated buffer */
    size_t n; /* allocated capacity */
    size_t i; /* index of first unused byte */
};

R_String *R_string_new(size_t initial_capacity)
{
    R_String *s = R_NEW_INIT_STRUCT(s, R_String, R_MAGIC_INIT(R_String)
                                    NULL, 0, 0);
    R_MAGIC_CHECK(R_String, s);
    R_string_reserve(s, R_MAX(initial_capacity, R_STRING_MINIMUM_CAPACITY));
    R_string_clear(s);
    return s;
}

R_String *R_string_from(const char *str)
{
    return R_string_from_bin(str, str ? strlen(str) : 0);
}

R_String *R_string_from_bin(const void *data, size_t len)
{
    R_String *s = R_string_new(len + 1);
    R_string_append_bin(s, data, len);
    return s;
}

void R_string_free(R_String *s)
{
    if (s) {
        R_MAGIC_CHECK(R_String, s);
        free(s->d);
        R_MAGIC_POISON(R_String, s);
        free(s);
    }
}

void R_string_reserve(R_String *s, size_t capacity)
{
    R_MAGIC_CHECK(R_String, s);
    if ((s->n - s->i) < capacity) {
        s->d  = R_realloc(s->d, s->n + capacity);
        s->n += capacity;
    }
}

void R_string_clear(R_String *s)
{
    R_MAGIC_CHECK(R_String, s);
    s->i = 0;
    s->d[0] = '\0';
}

void R_string_append(R_String *s, const char *append)
{
    if (append) {
        R_string_append_bin(s, append, strlen(append));
    }
}

void R_string_append_bin(R_String *s, const void *data, size_t len)
{
    if (len) {
        R_string_reserve(s, len + 1);
        memcpy(&s->d[s->i], data, len);
        size_t i = s->i += len;
        s->d[i] = '\0';
    }
}

void R_string_concat(R_String *dst, R_String *src)
{
    if (src) {
        R_MAGIC_CHECK(R_String, src);
        R_string_append_bin(dst, src->d, src->i);
    }
}

void R_string_vprintf(R_String *s, const char *fmt, va_list ap)
{
    va_list aq;
    va_copy(aq, ap);
    int len = vsnprintf(NULL, 0, fmt, aq);
    va_end(aq);

    if (len > 0) {
        size_t slen = R_int2size(len);
        R_string_reserve(s, slen + 1);
        vsnprintf(&s->d[s->i], s->n - s->i, fmt, ap);
        size_t i = s->i += slen;
        s->d[i]  = '\0';
    }
}

void R_string_printf(R_String *s, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    R_string_vprintf(s, fmt, ap);
    va_end(ap);
}

size_t R_string_len(R_String *s)
{
    R_MAGIC_CHECK(R_String, s);
    return s->i;
}

char *R_string_body(R_String *s)
{
    R_MAGIC_CHECK(R_String, s);
    return s->d;
}

/* There's also substring search functions in utstring, but they're very long
 * and complicated. Since I really don't need them, they've been elided here. */
