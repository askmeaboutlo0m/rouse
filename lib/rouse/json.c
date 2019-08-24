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
