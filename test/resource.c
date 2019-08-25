#include "tap.h"
#include <rouse.h>


static void find_resource(void *user)
{
    R_logger_die = R_log_stdout;
    R_resource_find(user);
}


static void test_empty_resources(void)
{
    ptr_eq_ok(R_resource_search("asdf"), NULL, "search empty resources");
    dies_ok(find_resource, "asdf", "finding nonexistent resource dies");
    int_eq_ok(R_resource_clear(), 0, "resources start empty");
}


static void test_single_resource(void)
{
    R_resource_add("test", 1, R_user_int(2), NULL);

    R_UserData *found = R_resource_search("test");
    int_eq_ok(found->i, 2, "search resource by id");
    int_eq_ok(R_resource_find("test").i, 2, "find resource by id");
    ptr_eq_ok(R_resource_search("asdf"), NULL, "search nonexistent resource");
    dies_ok(find_resource, "asdf", "finding nonexistent resource dies");

    int_eq_ok(R_resource_clear(), 1, "resource gets cleared");
    int_eq_ok(R_resource_clear(), 0, "resources are empty after clearing");
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

static bool remove_containing(const char *id, R_UNUSED int type,
                              R_UNUSED R_UserData user, void *arg)
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

    ptr_eq_ok(R_resource_find  ("a") .data, a,   "find resource a by id");
    ptr_eq_ok(R_resource_search("a")->data, a, "search resource a by id");
    ptr_eq_ok(R_resource_find  ("b") .data, b,   "find resource b by id");
    ptr_eq_ok(R_resource_search("b")->data, b, "search resource b by id");
    ptr_eq_ok(R_resource_find  ("c") .data, c,   "find resource c by id");
    ptr_eq_ok(R_resource_search("c")->data, c, "search resource c by id");
    ptr_eq_ok(R_resource_find  ("d") .data, d,   "find resource d by id");
    ptr_eq_ok(R_resource_search("d")->data, d, "search resource d by id");

    ptr_eq_ok(R_resource_search("e"), NULL, "search nonexistent resource");
    dies_ok(find_resource, "e", "finding nonexistent resource dies");

    int_eq_ok(freed, 0, "none freed");
    int_eq_ok(R_resource_remove_where(remove_containing, "ab"), 2, "removing a and b");
    int_eq_ok(freed, 2, "two freed");
    ptr_eq_ok(R_resource_search("a"), NULL, "a is gone");
    ptr_eq_ok(R_resource_search("b"), NULL, "b is gone too");

    int_eq_ok(R_resource_remove_where(remove_containing, "ab"), 0,
              "not removing a and b again");
    int_eq_ok(freed, 2, "still two freed");

    ok(R_resource_remove("c"), "removing c");
    int_eq_ok(freed, 3, "three freed");
    ptr_eq_ok(R_resource_search("c"), NULL, "c is gone");
    ok(!R_resource_remove("c"), "not removing c again");

    int_eq_ok(R_resource_clear(), 1, "clearing all");
    int_eq_ok(freed, 4, "four freed");
    ptr_eq_ok(R_resource_search("d"), NULL, "d is gone");
    ptr_eq_ok(R_resource_search("a"), NULL, "a is still gone");
    ptr_eq_ok(R_resource_search("b"), NULL, "b is still gone too");
    ptr_eq_ok(R_resource_search("c"), NULL, "c is also still gone");
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

    int_eq_ok(R_resource_clear(), 1, "clearing resources");
}


TAP_BEGIN
    test_empty_resources();
    test_single_resource();
    test_empty_resources();
    test_multiple_resources();
    test_empty_resources();
    test_dying_resources();
TAP_END
