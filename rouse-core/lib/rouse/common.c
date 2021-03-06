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
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <rouse_config.h>
#ifdef _WIN32
#   include <windows.h>
#endif
#include "common.h"


#ifdef ROUSE_MAGIC
uint32_t R_magic_seed;

uint32_t R_magic_hash(const char *type)
{
    uint32_t h = R_magic_seed;
    for (const char *c = type; *c != '\0'; ++c) {
        h = 31 * h + R_char2uint32(*c);
    }
    return h == R_MAGIC_POISONED_NUMBER ? R_MAGIC_POISONED_NUMBER / 2 : h;
}
#endif


void R_log_stdout(const char *msg)
{
    fprintf(stdout, "%s\n", msg);
}

void R_log_stderr(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
}

void R_log_dialog(const char *msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "R_log_dialog", msg, NULL);
}


#ifdef __EMSCRIPTEN__
#   define R_LOGGER_DEFAULT R_log_stdout
#else
#   define R_LOGGER_DEFAULT R_log_stderr
#endif

R_LogFn R_logger_die   = R_LOGGER_DEFAULT;
R_LogFn R_logger_warn  = R_LOGGER_DEFAULT;
R_LogFn R_logger_info  = R_LOGGER_DEFAULT;
R_LogFn R_logger_debug = R_LOGGER_DEFAULT;

unsigned int R_logbits = R_LOGBIT_DIE | R_LOGBIT_WARN | R_LOGBIT_INFO;

unsigned int R_log_enable(unsigned int bits)
{
    return R_logbits |= bits;
}

unsigned int R_log_disable(unsigned int bits)
{
    return R_logbits &= ~bits;
}

/*
 * All these log functions basically do the same thing and just call a
 * different logging callback, but they have to faff around with varargs, so
 * it can't be split off into a separate function. So here's this beautiful
 * macro to implement it instead.
 */
#ifdef ROUSE_DEBUG
#   define MARK_ARGS_USED(FILE, LINE) /* nothing */
#   define FMT_PREFIX(LEVEL, FILE, LINE) "[%s|%s:%d] ", LEVEL, FILE, LINE
#else
#   define MARK_ARGS_USED(FILE, LINE) do { (void) FILE; (void) LINE; } while (0)
#   define FMT_PREFIX(LEVEL, FILE, LINE) "[%s] ", LEVEL
#endif
#define DO_LOG(LOGGER, LOGBIT, LEVEL, FILE, LINE, FMT, LAST) do { \
        MARK_ARGS_USED(FILE, LINE); \
        /* No logger or logbit not set, nothing to do. */ \
        if (!(LOGGER && R_logbits & LOGBIT)) { \
            break; \
        } \
        /* Logging NULL? We'll just let that slide I guess. */ \
        if (!FMT) { \
            FMT = ""; \
        } \
        /* How long will it be? */ \
        int prefix_len = snprintf(NULL, 0, FMT_PREFIX(LEVEL, FILE, LINE)); \
        va_list ap; \
        va_start(ap, LAST); \
        int message_len = vsnprintf(NULL, 0, FMT, ap); \
        va_end(ap); \
        int total_len = prefix_len + message_len; \
        /* Not R_malloc, because we don't want to R_die from logging. */ \
        char *buf = malloc(R_int2size(total_len) + 1); \
        if (buf) { \
            int offset = snprintf(buf, R_int2size(prefix_len) + 1, \
                                  FMT_PREFIX(LEVEL, FILE, LINE)); \
            int size = total_len - offset + 1; \
            va_start(ap, LAST); \
            vsnprintf(buf + offset, R_int2size(size), FMT, ap); \
            va_end(ap); \
            LOGGER(buf); \
            free(buf); \
        } \
        else { \
            /* Welp, we can't format that. Punt to the format string. */ \
            LOGGER(FMT); \
        } \
    } while (0)

/* We want to be able to flush gcov coverage info before we die horribly. */
#ifdef ROUSE_GCOV
void __gcov_flush(void);
#endif

void R_die_fn(const char *file, int line, const char *fmt, ...)
{
    DO_LOG(R_logger_die, R_LOGBIT_DIE, "DIE", file, line, fmt, fmt);
#ifdef ROUSE_GCOV
    __gcov_flush();
#endif
    R_TRAP();
}

void R_warn_fn(const char *file, int line, const char *fmt, ...)
{
    DO_LOG(R_logger_warn, R_LOGBIT_WARN, "WARN", file, line, fmt, fmt);
}

void R_info_fn(const char *file, int line, const char *fmt, ...)
{
    DO_LOG(R_logger_info, R_LOGBIT_INFO, "INFO", file, line, fmt, fmt);
}

void R_debug_fn(const char *file R_UNUSED_UNLESS_DEBUG,
                int        line  R_UNUSED_UNLESS_DEBUG,
                const char *fmt  R_UNUSED_UNLESS_DEBUG,
                ...)
{
#ifdef ROUSE_DEBUG
    DO_LOG(R_logger_debug, R_LOGBIT_DEBUG, "DEBUG", file, line, fmt, fmt);
#endif
}

void R_assert_fail(const char *file, int line, const char *expr,
                   const char *message)
{
    R_die_fn(file, line, "Assertion botched: '%s' %s", expr, message);
}


void *R_malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }
    void *buf = malloc(size);
    if (!buf) {
        R_die("R_malloc: out of memory!");
    }
    return buf;
}

void *R_realloc(void *ptr, size_t size)
{
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    void *buf = realloc(ptr, size);
    if (!buf) {
        R_die("R_realloc: out of memory!");
    }
    return buf;
}


char *R_vformat(const char *fmt, va_list ap)
{
    va_list aq;

    va_copy(aq, ap);
    int len = vsnprintf(NULL, 0, fmt, aq);
    va_end(aq);

    size_t size = R_int2size(len) + 1;
    char   *buf = R_malloc(size);
    vsnprintf(buf, size, fmt, ap);

    return buf;
}

char *R_format(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *buf = R_vformat(fmt, ap);
    va_end(ap);
    return buf;
}

char *R_hexdump(const void *ptr, size_t size)
{
    if (ptr && size > 0) {
        char *buf   = R_malloc(size * 2 + 3);
        char *start = buf + 2;
        buf[0]      = '0';
        buf[1]      = 'x';

        const unsigned char *uchars = ptr;
        for (size_t i = 0; i < size; ++i) {
            snprintf(start + i * 2, 3, "%02x", uchars[size - i - 1]);
        }

        start[size * 2] = '\0';
        return buf;
    }
    else {
        return NULL;
    }
}

char *R_strdup(const char *str)
{
    if (!str) {
        return NULL;
    }
    size_t size = strlen(str) + 1;
    char   *buf = R_malloc(size);
    memcpy(buf, str, size);
    return buf;
}

bool R_str_equal(const char *a, const char *b)
{
    return a == b || (a && b && strcmp(a, b) == 0);
}

char *R_slurp(const char *path, long *out_len)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        R_die("Can't open '%s': %s", path, strerror(errno));
    }

    if (fseek(fp, 0L, SEEK_END) == -1) {
        R_die("Can't seek end of '%s': %s", path, strerror(errno));
    }

    long len = ftell(fp);
    if (len == -1) {
        R_die("Can't tell length of '%s': %s", path, strerror(errno));
    }

    if (fseek(fp, 0L, SEEK_SET) == -1L) {
        R_die("Can't seek start of '%s': %s", path, strerror(errno));
    }

    size_t size = R_long2size(len) + 1;
    char   *buf = R_malloc(size);
    memset(buf, 0, size);
    long got = R_size2long(fread(buf, 1, R_long2size(len), fp));
    if (got != len) {
        R_die("Read '%s': expected %ld bytes, but got %ld", path, len, got);
    }

    if (fclose(fp) != 0) {
        R_warn("Error closing '%s': %s", path, strerror(errno));
    }

    if (out_len) {
        *out_len = len;
    }
    return buf;
}


#ifdef _WIN32
const char *R_win32_strerror(unsigned long error)
{
    static char buf[1024];

    unsigned long flags  = FORMAT_MESSAGE_FROM_SYSTEM
                         | FORMAT_MESSAGE_IGNORE_INSERTS;
    unsigned long langid = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);

    if (FormatMessage(flags, NULL, error, langid, buf, sizeof(buf), NULL) > 0) {
        return buf;
    }
    else {
        return NULL;
    }
}
#endif
