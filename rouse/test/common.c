#include "tap.h"
#include <rouse.h>


static void test_user(void)
{
    R_UserData user = R_user_null();
    ptr_eq_ok(user.data, NULL,        "null user pointer is NULL");
    int_eq_ok(user.i, 0,              "null user int is 0");
    uint_eq_ok(user.u, 0u,            "null user uint is 0u");
    float_eq_ok(user.f, 0.0f,         "null user float is 0.0f");
    float_eq_ok(user.between.a, 0.0f, "null user between a is 0.0f");
    float_eq_ok(user.between.b, 0.0f, "null user between b is 0.0f");
}


static void test_format(void)
{
    char *text = R_format("about %.2f of %d is %s", 1.0 / 3.0, 3, "one");
    str_eq_ok(text, "about 0.33 of 3 is one", "formatting string");
    free(text);
}


static char *do_vformat(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *text = R_vformat(fmt, ap);
    va_end(ap);
    return text;
}

static void test_vformat(void)
{
    char *text = do_vformat("%s is exactly %d%c%ld", "π", 3, '.', 0L);
    str_eq_ok(text, "π is exactly 3.0", "formatting string with varargs");
    free(text);
}


#define HEXDUMP_FAILS_ON_BIG_ENDIAN_NOTE \
    "probably fails on big endian, but I don't have a platform to test that " \
    "on. If you got one, R_hexdump in lib/rouse/common.c is what needs to be " \
    "changed. I guess there should be some kind of #define from CMake to " \
    "tell us if we're in a wrong-endian world?"

static void hexdump_string(void)
{
    const char bytes[] = "\xab\xcd\xef";
    char       *text   = R_hexdump(bytes, sizeof(bytes));
    str_eq_ok(text, "0x00efcdab", "hex dump of a string of bytes, "
              HEXDUMP_FAILS_ON_BIG_ENDIAN_NOTE);
    free(text);
}

static void hexdump_uint32(void)
{
    uint32_t floofbab = 0xf100fbab;
    char     *text    = R_hexdump(&floofbab, sizeof(floofbab));
    size_eq_ok(sizeof(floofbab), 4, "uint32_t is 4 bytes long");
    str_eq_ok(text, "0xf100fbab", "hex dump of a uint32_t, "
              HEXDUMP_FAILS_ON_BIG_ENDIAN_NOTE);
    free(text);
}

static void test_hexdump(void)
{
    hexdump_string();
    hexdump_uint32();
}


static void test_strdup(void)
{
    char *text = R_strdup("you got duped");
    str_eq_ok(text, "you got duped", "strduping string");
    free(text);

    ptr_eq_ok(R_strdup(NULL), NULL, "strduping NULL gives NULL");
}


static void slurp_nonexistent(void *path)
{
    R_logger_die = NULL;
    R_slurp(path, NULL);
}

static void test_slurp(void)
{
    char *text = R_slurp("test/data/slurp.txt", NULL);
    str_eq_ok(text, "Some data to slurp.\nFor testing!\n",
              "slurping text file");
    free(text);

    long len;
    char *bin = R_slurp("test/data/slurp.bin", &len);
    long_eq_ok(len, 6, "slurped binary file length %ld == 6", len);
    ok(memcmp(bin, "\1\2\r\n\0\3", 6) == 0, "slurped binary file contents");
    free(bin);

    dies_ok(slurp_nonexistent, "nonexistent", "slurp nonexistent file dies");
}


TAP_BEGIN
    test_user();
    test_format();
    test_vformat();
    test_hexdump();
    test_strdup();
    test_slurp();
TAP_END
