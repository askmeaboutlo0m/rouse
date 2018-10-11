#include "tap.h"
#include <rouse/rouse.h>


static void find_resource(void *user)
{
    R_logger_die = R_log_stdout;
    R_resource_find(user);
}


static void test_empty_resources(void)
{
    ok(R_resource_search("asdf") == NULL, "search empty resources");
    dies_ok(find_resource, "asdf", "finding nonexistent resource dies");
    ok(R_resource_clear() == 0, "resources start empty");
}


static void test_single_resource(void)
{
    R_resource_add("test", 1, R_user_int(2), NULL);

    R_UserData *found = R_resource_search("test");
    ok(found && found->i == 2, "search resource by id");
    ok(R_resource_find("test").i == 2, "find resource by id");
    ok(R_resource_search("asdf") == NULL, "search nonexistent resource");
    dies_ok(find_resource, "asdf", "finding nonexistent resource dies");

    ok(R_resource_clear() == 1, "resource gets cleared");
    ok(R_resource_clear() == 0, "resources are empty after clearing");
}


static int freed;

static void free_resource(R_UserData user)
{
    free(user.data);
    ++freed;
}

static char *make_resource(const char *id, int type)
{
    char *data = R_strdup(id);
    R_resource_add(id, type, R_user_data(data), free_resource);
    return data;
}

static bool remove_containing(const char *id, int type,
                              R_UserData user, void *arg)
{
    return strstr(arg, id) != NULL;
}

static void test_multiple_resources(void)
{
    freed = 0;
    char *a = make_resource("a", 1);
    char *b = make_resource("b", 1);
    char *c = make_resource("c", 2);
    char *d = make_resource("d", 3);

    ok(a == R_resource_find  ("a") .data,   "find resource a by id");
    ok(a == R_resource_search("a")->data, "search resource a by id");
    ok(b == R_resource_find  ("b") .data,   "find resource b by id");
    ok(b == R_resource_search("b")->data, "search resource b by id");
    ok(c == R_resource_find  ("c") .data,   "find resource c by id");
    ok(c == R_resource_search("c")->data, "search resource c by id");
    ok(d == R_resource_find  ("d") .data,   "find resource d by id");
    ok(d == R_resource_search("d")->data, "search resource d by id");

    ok(NULL == R_resource_search("e"), "search nonexistent resource");
    dies_ok(find_resource, "e", "finding nonexistent resource dies");

    ok(freed == 0, "none freed");
    ok(R_resource_remove_where(remove_containing, "ab") == 2, "removing a and b");
    ok(freed == 2, "two freed");
    ok(NULL == R_resource_search("a"), "a is gone");
    ok(NULL == R_resource_search("b"), "b is gone too");

    ok(R_resource_remove_where(remove_containing, "ab") == 0,
       "not removing a and b again");
    ok(freed == 2, "still two freed");

    ok(R_resource_remove("c"), "removing c");
    ok(freed == 3, "three freed");
    ok(NULL == R_resource_search("c"), "c is gone");
    ok(!R_resource_remove("c"), "not removing c again");

    ok(R_resource_clear() == 1, "clearing all");
    ok(freed == 4, "four freed");
    ok(NULL == R_resource_search("d"), "d is gone");
    ok(NULL == R_resource_search("a"), "a is still gone");
    ok(NULL == R_resource_search("b"), "b is still gone too");
    ok(NULL == R_resource_search("c"), "c is also still gone");
}


static void make_duplicate_resource(R_UNUSED void *user)
{
    R_logger_die = R_log_stdout;
    R_resource_add("q", 9, R_user_null(), NULL);
}

static void assert_correct_type(void *user)
{
    R_resource_find_with_type(user, 8, "correct");
}

static void assert_wrong_type(void *user)
{
    R_logger_die = R_log_stdout;
    R_resource_find_with_type(user, -99, "wrong");
}

static void test_dying_resources(void)
{
    R_resource_add("q", 8, R_user_null(), NULL);
    dies_ok(make_duplicate_resource, NULL, "duplicate resource id dies");

    lives_ok(assert_correct_type, "q", "assert correct type lives");
    dies_ok (assert_wrong_type,   "q", "assert wrong type dies");

    ok(R_resource_clear() == 1, "clearing resources");
}


TAP_BEGIN
    test_empty_resources();
    test_single_resource();
    test_empty_resources();
    test_multiple_resources();
    test_empty_resources();
    test_dying_resources();
TAP_END
