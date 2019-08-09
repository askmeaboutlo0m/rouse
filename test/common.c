#include "tap.h"
#include <rouse.h>


static void test_format(void)
{
    char *text = R_format("about %.2f of %d is %s", 1.0 / 3.0, 3, "one");
    ok(R_str_equal(text, "about 0.33 of 3 is one"), "formatting string");
    free(text);
}


static void test_strdup(void)
{
    char *text = R_strdup("you got duped");
    ok(R_str_equal(text, "you got duped"), "strduping string");
    free(text);

    ok(R_strdup(NULL) == NULL, "strduping NULL gives NULL");
}


static void slurp_nonexistent(void *path)
{
    R_logger_die = NULL;
    R_slurp(path, NULL);
}

static void test_slurp(void)
{
    char *text = R_slurp("test/data/slurp.txt", NULL);
    ok(R_str_equal(text, "Some data to slurp.\nFor testing!\n"),
       "slurping text file");
    free(text);

    long len;
    char *bin = R_slurp("test/data/slurp.bin", &len);
    ok(len == 6, "slurped binary file length %ld == 6", len);
    ok(memcmp(bin, "\1\2\r\n\0\3", 6) == 0, "slurped binary file contents");
    free(bin);

    dies_ok(slurp_nonexistent, "nonexistent", "slurp nonexistent file dies");
}


TAP_BEGIN
    test_format();
    test_strdup();
    test_slurp();
TAP_END
