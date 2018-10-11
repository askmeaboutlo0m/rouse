/*
 * common.h - stuff that's basically needed everywhere
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

/*
 * Minimum, maximum and clamping between two values. Arguments are evaluated
 * multiple times.
 */
#define R_MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define R_MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define R_CLAMP(VAL, MIN, MAX) \
    ((VAL) < (MIN) ? (MIN) : (VAL) > (MAX) ? (MAX) : (VAL))

/* Classic static array length check. */
#define R_LENGTH(ARR) (sizeof(ARR) / sizeof((ARR)[0]))

/*
 * GCC- and Clang-specific options. MSVC can probably do some of this stuff
 * as well, but its C support beyond C89 is so bad that I don't care.
 */
#ifdef __GNUC__
    /* Mark a function as never returning. */
#   define R_NORETURN __attribute__((__noreturn__))
    /* Mark a parameter as unused, preventing a warning. */
#   define R_UNUSED   __attribute__((__unused__))
    /* Check a function for `printf`-like formatting. */
#   define R_FORMAT(STRING_INDEX, FIRST_TO_CHECK) \
        __attribute__((__format__(printf, STRING_INDEX, FIRST_TO_CHECK)))
    /* Trap to terminate the program or trigger the debugger. */
#   define R_TRAP() __builtin_trap()
#   if defined(__SANITIZE_ADDRESS__)
#       define R_HAVE_ASAN
#   elif defined(__has_feature)
#       if __has_feature(address_sanitizer)
#           define R_HAVE_ASAN
#       endif
#   endif
#else
#   define R_NORETURN /* nothing */
#   define R_UNUSED /* nothing */
#   define R_FORMAT(STRING_INDEX, FIRST_TO_CHECK) /* nothing */
#   define R_TRAP() abort()
#endif

/* Sometimes parameters are only used in debug mode. */
#ifdef ROUSE_DEBUG
#    define R_UNUSED_UNLESS_DEBUG /* nothing */
#else
#    define R_UNUSED_UNLESS_DEBUG R_UNUSED
#endif

/* Magic numbers to diagnose memory corruption etc. */
#ifdef ROUSE_MAGIC
#   define R_MAGIC_NUMBER_COUNT 8
extern uint32_t R_magic_numbers[R_MAGIC_NUMBER_COUNT];

#   define R_MAGIC_OF(EXPR) _Generic((EXPR), \
        struct R_Scene       *: (R_magic_numbers[0]), \
        struct R_Model       *: (R_magic_numbers[1]), \
        struct R_Mesh        *: (R_magic_numbers[2]), \
        struct R_MeshBuffer  *: (R_magic_numbers[3]), \
        struct R_Camera      *: (R_magic_numbers[4]), \
        struct R_FirstPerson *: (R_magic_numbers[5]), \
        struct R_Input       *: (R_magic_numbers[6]), \
        struct R_KeyBind     *: (R_magic_numbers[7]))

#   define R_MAGIC_FIELD uint32_t MAGIC;
#   define R_MAGIC_INIT(EXPR) R_MAGIC_OF(EXPR),
#   define R_MAGIC_SET(EXPR)  (EXPR)->MAGIC = R_MAGIC_OF(EXPR)

#   define R_MAGIC_CHECK(PTR) do { \
        if (!(PTR)) { \
            R_die("Bad magic: '%s' is NULL", #PTR); \
        } \
        if ((PTR)->MAGIC != R_MAGIC_OF(PTR)) { \
            R_die("Bad magic for '%s': 0x%x != 0x%x", \
                  #PTR, (PTR)->MAGIC, R_MAGIC_OF(PTR)); \
        } \
    } while (0)
#else
#   define R_MAGIC_FIELD       /* nothing */
#   define R_MAGIC_OF(EXPR)    /* nothing */
#   define R_MAGIC_INIT(EXPR)  /* nothing */
#   define R_MAGIC_SET(EXPR)   /* nothing */
#   define R_MAGIC_CHECK(EXPR) /* nothing */
#endif

#define R_MAGIC_CHECK_2(A, B) \
    do { R_MAGIC_CHECK(A); R_MAGIC_CHECK(B); } while (0)


/*
 * Allocate a variable: `int *i = R_NEW(i)`.
 */
#define R_NEW(VAR) (R_malloc(sizeof(*VAR)))
/*
 * Allocate a variable and then assign it a value: `int *i = R_NEW_INIT(i, 9)`.
 * This is a slightly dirty macro in that it expands to one and a half
 * statements, but it ties up some extremely common code so I'm okay with it.
 */
#define R_NEW_INIT(VAR, INIT) R_NEW(VAR); do { *VAR = INIT; } while (0)

#define R_NEW_INIT_STRUCT(VAR, TYPE, ...) \
    R_NEW(VAR); do { *VAR = (TYPE){__VA_ARGS__}; } while (0)
/*
 * Allocate an array of `COUNT` elements: `int *a = R_NEW(a, 10)`. A size of
 * zero will get you `NULL`, as per `R_malloc`.
 */
#define R_ANEW(VAR, COUNT) (R_malloc(sizeof(*VAR) * (COUNT)))


/*
 * User data that can hold a variety of different number types, or a `void *`.
 * This avoids having to stuff integers into a pointer and then unwrapping them
 * again later. Take care that you only take out the things that you put in.
 */
typedef union R_UserData {
    int          i;
    unsigned int u;
    float        f;
    void         *data;
} R_UserData;

static inline R_UserData R_user_int(int i) {
    return (R_UserData){.i = i};
}

static inline R_UserData R_user_uint(unsigned int u)
{
    return (R_UserData){.u = u};
}

static inline R_UserData R_user_float(float f)
{
    return (R_UserData){.f = f};
}

static inline R_UserData R_user_data(void *data)
{
    return (R_UserData){.data = data};
}

static inline R_UserData R_user_null(void)
{
    return (R_UserData){.data = NULL};
}


/*
 * Function to log a string somewhere. The logging functions will take care
 * of pre-formatting the string, so no `printf`ery is needed here. If you need
 * to save the string, `R_strdup` it, it's not yours.
 *
 * The string you receive *usually* has the format `[LEVEL|FILE:LINE] MESSAGE`,
 * where `LEVEL` is one of `DIE`, `WARN`, `INFO` or `DEBUG`, `FILE` is the file
 * name, `LINE` is the line number in that file and `MESSAGE` is the actual
 * thing being logged. You can use this format to split off the information
 * you need out of the string. Don't rely on this format though, you may get
 * something different if an allocation fails or someone else calls you.
 */
typedef void (*R_LogFn)(const char *);

/*
 * Log to stdout and stderr. The default for Emscripten is to log to stdout,
 * because it goes into the visible text output like you'd expect. Anywhere
 * else, stderr is the default.
 */
void R_log_stdout(const char *msg);
void R_log_stderr(const char *msg);

/* Log to an SDL message box, blocks until the box is dismissed. */
void R_log_dialog(const char *msg);

/*
 * Logger callbacks for the `R_(die|warn|info|debug)` functions below.
 * Just assign to these to change the respective logger.
 */
extern R_LogFn R_logger_die;
extern R_LogFn R_logger_warn;
extern R_LogFn R_logger_info;
extern R_LogFn R_logger_debug;

/* Log to `R_logger_die` and then `R_TRAP` to terminate program. */
#define R_die(...) R_die_fn(__FILE__, __LINE__, __VA_ARGS__)
void R_die_fn(const char *file, int line, const char *fmt, ...) R_FORMAT(3, 4)
                                                                R_NORETURN;

/* Log to `R_logger_warn`, when there's something fishy going on. */
#define R_warn(...) R_warn_fn(__FILE__, __LINE__, __VA_ARGS__)
void R_warn_fn(const char *file, int line, const char *fmt, ...) R_FORMAT(3, 4);

/* Log to `R_logger_info`, when you're feeling talkative. */
#define R_info(...) R_info_fn(__FILE__, __LINE__, __VA_ARGS__)
void R_info_fn(const char *file, int line, const char *fmt, ...) R_FORMAT(3, 4);

/* Log to `R_logger_debug`, if `ROUSE_DEBUG` is defined anyway. */
#if ROUSE_DEBUG
#   define R_debug(...) R_debug_fn(__FILE__, __LINE__, __VA_ARGS__)
#else
#   define R_debug(...) ((void) 0)
#endif
void R_debug_fn(const char *file, int line, const char *fmt, ...) R_FORMAT(3, 4);


/*
 * Try to `malloc` some memory, but `R_die` if that fails. If `size` is 0, this
 * will return `NULL`, otherwise a pointer to the allocated buffer. So ideally,
 * you should never have to check the return value for success.
 */
void *R_malloc(size_t size);
/*
 * Like `R_malloc`, but for `realloc`. Calls `R_die` on failure. If `size` is
 * 0, the buffer is `free`d and `NULL` is returned.
 */
void *R_realloc(void *ptr, size_t size);

/*
 * Does a `sprintf` with a dynamically allocated buffer and returns that. You
 * have to `free` the returned value yourself. `R_die`s if the allocation fails.
 */
char *R_format(const char *fmt, ...) R_FORMAT(1, 2);
/*
 * Copy a string to a newly allocated buffer, which you're responsible for
 * `free`ing. `R_die`s if the allocation fails. When you try to dup `NULL`,
 * you'll get `NULL` back.
 */
char *R_strdup(const char *str);
/*
 * Do a `strcmp` and return if the strings were equal.
 */
bool R_str_equal(const char *a, const char *b);
/*
 * Read a file into a newly allocated buffer. You gotta `free` the returned
 * value yourself. This will `R_die` if it can't read the file, the allocation
 * fails or something weird happens and it gets less bytes out of the file than
 * it was told by `fseek`ing to the end and asking `ftell`. If `out_len` is not
 * `NULL`, it'll be filled with the number of bytes read.
 */
char *R_slurp(const char *path, long *out_len);
