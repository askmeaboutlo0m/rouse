#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>
#include <assert.h>


/*
 * Emscripten uses .bc libraries in some places, for which
 * mocking doesn't work the way it does for regular libs.
 * You can't just define a function in your executable and
 * rely on the library version not being pulled in.
 */
#ifndef __EMSCRIPTEN__
#   define TAP_CAN_MOCK
#endif


#define TAP_CONTINUE_ON_FAIL 0
#define TAP_BAIL_ON_FAIL     1
#define TAP_TRAP_ON_FAIL     2

#ifndef TAP_ON_FAIL
#   define TAP_ON_FAIL TAP_CONTINUE_ON_FAIL
#endif


#ifndef TAP_FORK
#   if defined(_WIN32) || defined(__EMSCRIPTEN__)
#       define TAP_FORK 0
#   else
#       define TAP_FORK 1
#   endif
#endif

#if TAP_FORK != 0
#   include <sys/types.h>
#   include <sys/wait.h>
#   include <unistd.h>
#endif


#ifndef TAP_OUT
#   define TAP_OUT stdout
#endif

#ifndef TAP_ERR
#   define TAP_ERR stderr
#endif

#ifndef TAP_VAR
#   define TAP_VAR static
#endif

#ifndef TAP_FN
#   if __STDC_VERSION__ >= 199901L
#       define TAP_FN static inline
#   else
#       define TAP_FN static
#   endif
#endif


#ifdef __GNUC__
#   define TAP_NORETURN __attribute__ ((__noreturn__))
#   define TAP_FORMAT(STRING_INDEX, FIRST_TO_CHECK) \
        __attribute__((__format__(printf, STRING_INDEX, FIRST_TO_CHECK)))
#   define TAP_TRAP() __builtin_trap()
#else
#   define TAP_NORETURN                             /* nothing */
#   define TAP_FORMAT(STRING_INDEX, FIRST_TO_CHECK) /* nothing */
#   define TAP_TRAP()                               abort()
#endif


#ifdef ROUSE_GCOV
void __gcov_flush(void);
#endif


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"


#define TAP_BEGIN \
    int main(R_UNUSED int argc, R_UNUSED char **argv) \
    {

#define TAP_END \
        return done_testing(); \
    }

#define TAP_SKIP_ALL(MESSAGE) \
    int main(R_UNUSED int argc, R_UNUSED char **argv) \
    { \
        skip_all(MESSAGE); \
    }


TAP_VAR int tap_planned = -1;
TAP_VAR int tap_tests   = 0;
TAP_VAR int tap_failed  = 0;
TAP_VAR int tap_on_fail = TAP_ON_FAIL;


TAP_FN void tap_vsay(FILE *stream, const char *fmt, va_list ap,
                     const char *prefix, const char *suffix)
{
    if (prefix) {
        fputs(prefix, stream);
    }
    vfprintf(stream, fmt, ap);
    if (suffix) {
        fputs(suffix, stream);
    }
}

#define TAP_PRINT_FN(NAME, STREAM, PREFIX, SUFFIX) \
    TAP_FORMAT(1, 2) \
    TAP_FN void NAME(const char *fmt, ...) \
    { \
        va_list ap; \
        va_start(ap, fmt); \
        tap_vsay(STREAM, fmt, ap, PREFIX, SUFFIX); \
        va_end(ap); \
    }

TAP_PRINT_FN(tap_say, TAP_OUT, "",   "\n")
TAP_PRINT_FN(note,    TAP_OUT, "# ", "\n")
TAP_PRINT_FN(diag,    TAP_ERR, "# ", "\n")


TAP_FN void plan(int n)
{
    assert(tap_planned == -1);
    assert(n > 0);
    tap_planned = n;
    tap_say("1..%d", tap_planned);
}

TAP_NORETURN
TAP_FN void skip_all(const char *reason)
{
    tap_say("1..0 # SKIP %s", reason ? reason : "");
    exit(EXIT_SUCCESS);
}

TAP_NORETURN
TAP_FN void BAIL_OUT(const char *reason)
{
    tap_say("Bail out! %s", reason ? reason : "");
    exit(EXIT_FAILURE);
}

TAP_FN int done_testing(void)
{
    if (tap_planned == -1) {
        tap_planned = tap_tests;
        tap_say("1..%d", tap_tests);
    }
    return tap_failed == 0 && tap_planned == tap_tests
         ? EXIT_SUCCESS : EXIT_FAILURE;
}

TAP_FORMAT(2, 3)
TAP_FN void skip(int count, const char *fmt, ...)
{
    int i;
    va_list ap;
    for (i = 0; i < count; ++i) {
        fprintf(TAP_OUT, "ok %d SKIP ", ++tap_tests);
        va_start(ap, fmt);
        vfprintf(TAP_OUT, fmt, ap);
        va_end(ap);
        fputs("\n", TAP_OUT);
    }
}


TAP_FN void tap_handle_fail(void)
{
    ++tap_failed;
    switch (tap_on_fail) {
        case TAP_BAIL_ON_FAIL:
            BAIL_OUT("TAP_BAIL_ON_FAIL is set");
        case TAP_TRAP_ON_FAIL:
            TAP_TRAP();
        default:
            break;
    }
}

TAP_FN int vok(int cond, const char *fmt, va_list ap,
               void (*detail_fn)(void *), void *detail_arg)
{
    if (!cond) {
        fputs("not ", TAP_OUT);
    }
    fprintf(TAP_OUT, "ok %d", ++tap_tests);
    if (fmt) {
        fputs(" - ", TAP_OUT);
        vfprintf(TAP_OUT, fmt, ap);
    }
    fputs("\n", TAP_OUT);
    if (!cond) {
        if (detail_fn) {
            detail_fn(detail_arg);
        }
        tap_handle_fail();
    }
    return !!cond;
}

#define TAP_CALL_VOK(COND, FMT, DETAIL_FN, DETAIL_ARG) do { \
        va_list ap; \
        va_start(ap, FMT); \
        int _result = vok(COND, FMT, ap, DETAIL_FN, DETAIL_ARG); \
        va_end(ap); \
        return _result; \
    } while (0)

TAP_FORMAT(2, 3)
TAP_FN int ok(int cond, const char *fmt, ...)
{
    TAP_CALL_VOK(cond, fmt, NULL, NULL);
}

TAP_FORMAT(1, 2)
TAP_FN int fail(const char *fmt, ...)
{
    TAP_CALL_VOK(0, fmt, NULL, NULL);
}

TAP_FORMAT(1, 2)
TAP_FN int pass(const char *fmt, ...)
{
    TAP_CALL_VOK(1, fmt, NULL, NULL);
}


#define TAP_CALL_VOK_DETAIL(COND, FMT, DETAIL_FN, TYPE, ...) do { \
        TYPE _detail = __VA_ARGS__; \
        TAP_CALL_VOK(COND, FMT, DETAIL_FN, &_detail); \
    } while (0)

#define TAP_OK_DETAIL_FN(ARG_TYPE, NAME, COND, DETAIL_TITLE, DETAIL_FORMAT) \
    struct TapDetail_ ## NAME { \
        ARG_TYPE got; \
        ARG_TYPE want; \
    }; \
    \
    static void tap_detail_ ## NAME(void *arg) \
    { \
        struct TapDetail_ ## NAME *detail = arg; \
        diag(DETAIL_TITLE); \
        diag("  wanted  «" DETAIL_FORMAT "»", detail->want); \
        diag("  but got «" DETAIL_FORMAT "»", detail->got); \
    } \
    \
    TAP_FORMAT(3, 4) \
    TAP_FN int NAME(ARG_TYPE got, ARG_TYPE want, const char *fmt, ...) \
    { \
        TAP_CALL_VOK_DETAIL(COND, fmt, tap_detail_ ## NAME, \
                            struct TapDetail_ ## NAME, {got, want}); \
    }

TAP_FN int tap_str_eq(const char *a, const char *b)
{
    return a == b || (a && strcmp(a, b) == 0);
}

TAP_OK_DETAIL_FN(int, int_eq_ok, got == want,
                 "ints should be equal, but they're not", "%d")

TAP_OK_DETAIL_FN(long, long_eq_ok, got == want,
                 "longs should be equal, but they're not", "%ld")

TAP_OK_DETAIL_FN(unsigned short, ushort_eq_ok, got == want,
                 "unsigned shorts should be equal, but they're not", "%u")

TAP_OK_DETAIL_FN(const void *, ptr_eq_ok, got == want,
                 "pointers should be equal, but they're not", "%p")

TAP_OK_DETAIL_FN(size_t, size_eq_ok, got == want,
                 "size_t should be equal, but they're not", "%zd")

TAP_OK_DETAIL_FN(const char *, str_eq_ok, tap_str_eq(got, want),
                 "strings should be equal, but they're not", "%s")

#define TAP_OK_EPS_DETAIL_FN(ARG_TYPE, NAME, COND, DETAIL_TITLE, DETAIL_FORMAT) \
    struct TapDetail_ ## NAME { \
        ARG_TYPE got; \
        ARG_TYPE want; \
        ARG_TYPE eps; \
    }; \
    \
    static void tap_detail_ ## NAME(void *arg) \
    { \
        struct TapDetail_ ## NAME *detail = arg; \
        diag(DETAIL_TITLE); \
        diag("  wanted  «" DETAIL_FORMAT "»", detail->want); \
        diag("  but got «" DETAIL_FORMAT "»", detail->got); \
        diag("  epsilon «" DETAIL_FORMAT "»", detail->eps); \
    } \
    \
    TAP_FORMAT(4, 5) \
    TAP_FN int NAME(ARG_TYPE got, ARG_TYPE want, ARG_TYPE eps, \
                    const char *fmt, ...) \
    { \
        TAP_CALL_VOK_DETAIL(COND, fmt, tap_detail_ ## NAME, \
                            struct TapDetail_ ## NAME, {got, want, eps}); \
    }

TAP_OK_EPS_DETAIL_FN(double, double_eq_eps_ok, fabs(got - want) < eps,
                     "doubles should be (mostly) equal, but they're not", "%f")

TAP_OK_EPS_DETAIL_FN(float, float_eq_eps_ok, fabsf(got - want) < eps,
                    "floats should be (mostly) equal, but they're not", "%f")


#if TAP_FORK != 0
TAP_FN int tap_report_process_status(pid_t pid, int wstatus)
{
    int retval;
    fprintf(TAP_OUT, "# child process %ld", (long) pid);
    if (WIFEXITED(wstatus)) {
        retval = WEXITSTATUS(wstatus);
        fprintf(TAP_OUT, " exited with %d", retval);
    }
    else if (WIFSIGNALED(wstatus)) {
        int signal = WTERMSIG(wstatus);
        fprintf(TAP_OUT, " caught signal %d", signal);
        retval = -1;
    }
    else {
        fprintf(TAP_OUT, " neither exited nor caught a signal?!");
        retval = -2;
    }
    fprintf(TAP_OUT, "\n");
    return retval;
}
#endif

TAP_FN int tap_check_child(const char *context, int want_success,
                           void (*fn)(void *), void *user, const char *message)
{
#if TAP_FORK != 0
    pid_t pid = fork();
    if (pid == -1) {
        diag("fork: %s", strerror(errno));
        return fail("%s", message);
    }
    else if (pid == 0) {
        fn(user);
#   ifdef ROUSE_GCOV
        __gcov_flush();
#   endif
        _Exit(EXIT_SUCCESS);
    }
    else {
        int wstatus, success;
        do {
            waitpid(pid, &wstatus, 0);
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
        success = tap_report_process_status(pid, wstatus);
        return ok(want_success ? success == 0 : success != 0, "%s", message);
    }
#else
    skip(1, "%s: avoiding fork(), `#define TAP_FORK 1` to override", context);
    return -1;
#endif
}

TAP_FN int lives_ok(void (*fn)(void *), void *user, const char *message)
{
    return tap_check_child("lives_ok", 1, fn, user, message);
}

TAP_FN int dies_ok(void (*fn)(void *), void *user, const char *message)
{
    return tap_check_child("dies_ok", 0, fn, user, message);
}


#pragma GCC diagnostic pop
