#define uthash_malloc(sz) R_malloc(sz)
#define uthash_fatal(msg) R_die("uthash: %s", msg)
#include "uthash.h"

#define R_NULL_UT_HH {NULL, NULL, NULL, NULL, NULL, NULL, 0, 0}

#define R_HASH_CLEAR(TYPE, HEAD, FREE) do { \
        TYPE *_cur, *_tmp; \
        HASH_ITER(hh, (HEAD), _cur, _tmp) { \
            HASH_DEL((HEAD), _cur); \
            FREE(_cur); \
        } \
    } while (0)
