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
#include <stdbool.h>
#include <stdnoreturn.h>
#include <stdio.h>
#include <assert.h>
#include "3rdparty/parson.h"
#include "common.h"
#include "json.h"


static JSON_Status set_json_string_and_free(JSON_Object *obj, const char *name,
                                            char *buf)
{
    if (buf) {
        JSON_Status status = json_object_set_string(obj, name, buf);
        free(buf);
        return status;
    }
    else {
        return json_object_set_null(obj, name);
    }
}

JSON_Status R_json_object_set_hexdump(JSON_Object *obj, const char *name,
                                      void *ptr, size_t size)
{
    char *buf = R_hexdump(ptr, size);
    return set_json_string_and_free(obj, name, buf);
}

JSON_Status R_json_object_set_format(JSON_Object *obj, const char *name,
                                     const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *buf = R_vformat(fmt, ap);
    va_end(ap);
    return set_json_string_and_free(obj, name, buf);
}

JSON_Status R_json_object_set_address(JSON_Object *obj, const char *name,
                                      void *ptr)
{
    return ptr ? R_json_object_set_format(obj, name, "%p", ptr)
               : json_object_set_null(obj, name);
}


char *R_json_to_string(JSON_Value *value_will_be_freed, bool pretty)
{
    assert(value_will_be_freed && "JSON value to stringify can't be NULL");
    char *str = pretty
              ? json_serialize_to_string_pretty(value_will_be_freed)
              : json_serialize_to_string(value_will_be_freed);
    json_value_free(value_will_be_freed);
    return str;
}
