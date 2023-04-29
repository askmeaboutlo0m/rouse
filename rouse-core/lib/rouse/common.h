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

#ifndef ROUSE_CONFIG_H_INCLUDED
#   error "Missing #include of rouse_config.h, must come before common.h"
#endif

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

#define R_NOOP() do {} while (0)

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
    /* Mark an unreachable code path. */
#   define R_UNREACHABLE() __builtin_unreachable()
#else
#   define R_NORETURN /* nothing */
#   define R_UNUSED /* nothing */
#   define R_FORMAT(STRING_INDEX, FIRST_TO_CHECK) /* nothing */
#   define R_TRAP() abort()
#   define R_UNREACHABLE() /* nothing */
#endif

/*
 * C++ doesn't support `static` in array arguments
 * and requires casts to void *.
 */
#ifdef __cplusplus
#   define R_STATIC(X)           /* nothing */
#   define R_CPPCAST(TYPE, EXPR) static_cast<TYPE>(EXPR)
#else
#   define R_STATIC(X)           static X
#   define R_CPPCAST(TYPE, EXPR) (EXPR)
#endif

/* Sometimes parameters are only used in debug mode. */
#ifdef ROUSE_DEBUG
#    define R_UNUSED_UNLESS_DEBUG /* nothing */
#else
#    define R_UNUSED_UNLESS_DEBUG R_UNUSED
#endif

/* And sometiems parameters are only used if there's asserts or magic checks. */
#if defined(ROUSE_DEBUG) || defined(ROUSE_MAGIC)
#   define R_UNUSED_UNLESS_DEBUG_OR_MAGIC /* nothing */
#else
#   define R_UNUSED_UNLESS_DEBUG_OR_MAGIC R_UNUSED
#endif

/* Magic numbers to diagnose memory corruption etc. */
#ifdef ROUSE_MAGIC
extern uint32_t R_magic_seed;
uint32_t R_magic_hash(const char *type);

#   define R_MAGIC_FIELD uint32_t MAGIC;

#   define R_MAGIC_OF(TYPE) ((void) (TYPE *)NULL, R_magic_hash(#TYPE))

#   define R_MAGIC_INIT(TYPE) R_MAGIC_OF(TYPE),

#   define R_MAGIC_SET(TYPE, EXPR) do { \
        TYPE *_magic_ptr  = (EXPR); \
        _magic_ptr->MAGIC = R_MAGIC_OF(TYPE); \
    } while (0)

#    define R_MAGIC_CHECK(TYPE, EXPR) do { \
            const TYPE *_magic_ptr = (EXPR); \
            if (_magic_ptr) { \
                uint32_t _expr_magic = _magic_ptr->MAGIC; \
                uint32_t _type_magic = R_MAGIC_OF(TYPE); \
                if (_expr_magic != _type_magic) { \
                    R_die("Bad %s magic for '%s': got 0x%x, but should be 0x%x", \
                        #TYPE, #EXPR, _expr_magic, _type_magic); \
                } \
            } \
            else { \
                R_die("Bad %s magic: '%s' is NULL", #TYPE, #EXPR); \
            } \
        } while (0)

#   define R_MAGIC_POISONED_NUMBER 0xdeaddeadu

#   define R_MAGIC_INIT_POISON R_MAGIC_POISONED_NUMBER,

#   define R_MAGIC_POISON(TYPE, EXPR) do { \
        R_MAGIC_CHECK(TYPE, EXPR); \
        (EXPR)->MAGIC = R_MAGIC_POISONED_NUMBER; \
    } while (0)

#   define R_UNUSED_UNLESS_MAGIC /* nothing */
#else
#   define R_MAGIC_FIELD              /* nothing */
#   define R_MAGIC_OF(TYPE)           /* nothing */
#   define R_MAGIC_INIT(TYPE)         /* nothing */
#   define R_MAGIC_SET(TYPE, EXPR)    R_NOOP()
#   define R_MAGIC_CHECK(TYPE, EXPR)  R_NOOP()
#   define R_MAGIC_INIT_POISON        /* nothing */
#   define R_MAGIC_POISON(TYPE, EXPR) R_NOOP()
#   define R_UNUSED_UNLESS_MAGIC      R_UNUSED
#endif


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
/*
 * Allocate a variable `VAR` and then assign it a `TYPE` struct literal,
 * passing all arguments into its initializer. Example:
 *
 *     typedef struct { int i; const char *s; } SomeStruct;
 *     SomeStruct ss = R_NEW_INIT_STRUCT(ss, SomeStruct, 1, "two");
 */
#define R_NEW_INIT_STRUCT(VAR, TYPE, ...) R_NEW_INIT(VAR, ((TYPE){__VA_ARGS__}))
/*
 * Allocate an array of `COUNT` elements: `int *a = R_NEW(a, 10)`. A size of
 * zero will get you `NULL`, as per `R_malloc`.
 */
#define R_ANEW(VAR, COUNT) (R_malloc(sizeof(*VAR) * (COUNT)))


/*
 * Explicit conversion functions. We build with -Wconversion, so some casts are
 * simply necessary. Having these functions allows us to ensure both the inputs
 * and outputs are checked, a raw cast would just blindly cast *any* type away.
 * On top of that, we could add additional checks here in debug mode, like
 * checking overflow or trying to turn negative numbers into something unsigned.
 */

static inline unsigned char R_char2uchar(char x)
{
    return (unsigned char) x;
}

static inline uint32_t R_char2uint32(char x){
    return (uint32_t) x;
}

static inline float R_double2float(double x)
{
    return (float) x;
}

static inline int R_float2int(float x)
{
    return (int) x;
}

static inline uint32_t R_float2uint32(float x)
{
    return (uint32_t) x;
}

static inline double R_int2double(int x)
{
    return (double) x;
}

static inline float R_int2float(int x)
{
    return (float) x;
}

static inline size_t R_int2size(int x)
{
    return (size_t) x;
}

static inline unsigned int R_int2uint(int x)
{
    return (unsigned int) x;
}

static inline uint32_t R_int2uint32(int x)
{
    return (uint32_t) x;
}

static inline uint64_t R_int2uint64(int x)
{
    return (uint64_t) x;
}

static inline unsigned short R_int2ushort(int x)
{
    return (unsigned short) x;
}

static inline int R_long2int(long x)
{
    return (int) x;
}

static inline size_t R_long2size(long x)
{
    return (size_t) x;
}

static inline int R_size2int(size_t x)
{
    return (int) x;
}

static inline long R_size2long(size_t x)
{
    return (long) x;
}

static inline ptrdiff_t R_size2ptrdiff(size_t x)
{
    return (ptrdiff_t) x;
}

static inline char R_uchar2char(unsigned char x)
{
    return (char) x;
}

static inline float R_uint2float(unsigned int x)
{
    return (float) x;
}

static inline int R_uint2int(unsigned int x)
{
    return (int) x;
}

static inline uint32_t R_uint2uint32(unsigned int x)
{
    return (uint32_t) x;
}

static inline float R_uint322float(uint32_t x)
{
    return (float) x;
}

static inline float R_uint642float(uint64_t x)
{
    return (float) x;
}

static inline int R_uint642int(uint64_t x)
{
    return (int) x;
}


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
    struct {
        float a, b;
    } between;
} R_UserData;

static inline R_UserData R_user_int(int i)
{
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
    R_UserData user;
    void *memset(void * ptr, int value, size_t num);
    memset(&user, 0, sizeof(user));
    return user;
}

static inline R_UserData R_user_between(float a, float b)
{
    return (R_UserData){.between = {a, b}};
}


/*
 * Function to log a string somewhere. The logging functions will take care
 * of pre-formatting the string, so no `printf`ery is needed here. If you need
 * to save the string, `R_strdup` it, it's not yours.
 *
 * The string you receive *usually* has the format `[LEVEL|FILE:LINE] MESSAGE`
 * if `ROUSE_DEBUG` is defined and `[LEVEL] MESSAGE` otherwise. `LEVEL` is one
 * of `DIE`, `WARN`, `INFO` or `DEBUG`, `FILE` is the file name, `LINE` is the
 * line number in that file and `MESSAGE` is the actual thing being logged. You
 * can use this format to split off the information you need out of the string.
 * Don't rely on this format though, you may get something different if an
 * allocation fails or someone else calls you directly.
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

/*
 * Enable or disable certain log levels with the `R_logbits` flags. By default
 * everything except debug is enabled, because that level is just extremely
 * noisy. If you really want that level of loggery, turn it on temporarily on
 * the sections that you really care about it.
 */
#define R_LOGBIT_DIE   0x1
#define R_LOGBIT_WARN  0x2
#define R_LOGBIT_INFO  0x4
#define R_LOGBIT_DEBUG 0x8

extern unsigned int R_logbits;

unsigned int R_log_enable (unsigned int bits);
unsigned int R_log_disable(unsigned int bits);

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
#ifdef ROUSE_DEBUG
#   define R_debug(...) R_debug_fn(__FILE__, __LINE__, __VA_ARGS__)
#else
#   define R_debug(...) ((void) 0)
#endif
void R_debug_fn(const char *file, int line, const char *fmt, ...) R_FORMAT(3, 4);


/*
 * Assertions, basically a conditional R_die if debug mode is enabled.
 */
#if defined(ROUSE_DEBUG) || defined(__clang_analyzer__)
#   define R_assert(EXPR, MESSAGE) do { \
            if (!(EXPR)) { \
                R_assert_fail(__FILE__, __LINE__, #EXPR, MESSAGE); \
            } \
        } while (0)
#else
#   define R_assert(EXPR, MESSAGE) /* nothing */
#endif

#define R_assert_not_null(EXPR) R_assert(EXPR, "can't be NULL")

void R_assert_fail(const char *file, int line, const char *expr,
                   const char *message) R_NORETURN;


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
 * Same thing, but with a `va_list`. You must call `va_start` before and
 * `va_end` after you call this.
 */
char *R_vformat(const char *fmt, va_list ap);
/*
 * Turn an arbitrary chunk of bytes pointed to by `ptr` of the given `size`
 * into a hexadecimal string. It'll always start with "0x" and all the bytes
 * in the string high to low, from 00 to ff. That is, a buffer of size 4 with
 * holding the integer 1 little endian will result in `0x00000001`. If `ptr` is
 * `NULL` or `size` is zero, then you'll get `NULL` back. You must `free` the
 * resulting string yourself. `R_die`s if the allocation fails.
 */
char *R_hexdump(const void *ptr, size_t size);
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

/*
 * Windows doesn't really provide a sensible stringification for its
 * `GetLastError` function, so this will have to stand in for it. It formats
 * the given error to a static string buffer that will be re-used, so the
 * returned string will only be valid until the next call to this function.
 * May return `NULL` if FormatMessage fails for whatever reason. May also
 * call `SetLastError`, so keep your error code around if you need it.
 */
#ifdef _WIN32
const char *R_win32_strerror(unsigned long error);
#endif
