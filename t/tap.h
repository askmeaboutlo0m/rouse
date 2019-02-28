#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>


#define TAP_CONTINUE_ON_FAIL 0
#define TAP_BAIL_ON_FAIL     1
#define TAP_TRAP_ON_FAIL     2

#ifndef TAP_ON_FAIL
#   define TAP_ON_FAIL TAP_CONTINUE_ON_FAIL
#endif


#ifndef TAP_FORK
#   ifdef _WIN32
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
    { \
        R_logger_debug = R_log_stdout;

#define TAP_END \
        return done_testing(); \
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

TAP_PRINT_FN(tap_say, TAP_OUT, "\n",   "\n")
TAP_PRINT_FN(note,    TAP_OUT, "\n# ", "\n")
TAP_PRINT_FN(diag,    TAP_ERR, "\n# ", "\n")


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
    fputs("\n", TAP_OUT);
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
    switch (tap_on_fail) {
        case TAP_BAIL_ON_FAIL:
            BAIL_OUT("TAP_BAIL_ON_FAIL is set");
        case TAP_TRAP_ON_FAIL:
            TAP_TRAP();
        default:
            break;
    }
}

TAP_FN int vok(int cond, const char *fmt, va_list ap)
{
    fputs("\n", TAP_OUT);
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
        tap_handle_fail();
    }
    return !!cond;
}

#define TAP_CALL_VOK(COND, FMT) \
    va_list ap; \
    va_start(ap, FMT); \
    int result = vok(COND, FMT, ap); \
    va_end(ap); \
    return result;

TAP_FORMAT(2, 3)
TAP_FN int ok(int cond, const char *fmt, ...)
{
    TAP_CALL_VOK(cond, fmt);
}

TAP_FORMAT(1, 2)
TAP_FN int fail(const char *fmt, ...)
{
    TAP_CALL_VOK(0, fmt);
}

TAP_FORMAT(1, 2)
TAP_FN int pass(const char *fmt, ...)
{
    TAP_CALL_VOK(1, fmt);
}


#if TAP_FORK != 0
TAP_FN int tap_report_process_status(pid_t pid, int wstatus)
{
    int retval;
    fprintf(TAP_OUT, "\n# child process %ld", (long) pid);
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
        return fail(message);
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
        return ok(want_success ? success == 0 : success != 0, message);
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
