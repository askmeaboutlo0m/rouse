/*
 * json.h - utility routines on top of parson.
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

#define R_JSON_OBJECT_SET_FN(OBJ, NAME, FN) do { \
        if (FN) { \
            json_object_set_string(OBJ, NAME, "(function pointer)"); \
        } else { \
            json_object_set_null(OBJ, NAME); \
        } \
    } while (0)

JSON_Status R_json_object_set_hexdump(JSON_Object *obj, const char *name,
                                      void *ptr, size_t size);

JSON_Status R_json_object_set_format(JSON_Object *obj, const char *name,
                                     const char *fmt, ...);

JSON_Status R_json_object_set_address(JSON_Object *obj, const char *name,
                                      void *ptr);

/*
 * Serializes the given JSON value, either `pretty` or not. The value will be
 * freed, so don't use this if you need it again afterwards - for that, use
 * `json_serialize_to_string` or `json_serialize_to_string_pretty` from Parson
 * directly. The returned string must be `free`d by you though.
 */
char *R_json_to_string(JSON_Value *value_will_be_freed, bool pretty);
