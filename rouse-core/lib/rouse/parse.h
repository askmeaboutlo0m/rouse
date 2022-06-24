/*
 * parse.h - parsing of binary streams.
 *
 * Copyright (c) 2019 - 2022 askmeaboutloom
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

#define R_PARSE_X_AT(X, PARSE, AT, FORMAT, ...) do { \
        X("%s: " FORMAT " (at position %d)", \
          (PARSE)->title ? (PARSE)->title : "R_Parse", \
          __VA_ARGS__, AT); \
    } while (0)

#define R_PARSE_DIE_AT(PARSE, AT, FORMAT, ...) \
    R_PARSE_X_AT(R_die, (PARSE), AT, FORMAT, __VA_ARGS__)

#define R_PARSE_DIE(PARSE, FORMAT, ...) \
    R_PARSE_DIE_AT((PARSE), (PARSE)->pos, FORMAT, __VA_ARGS__);

#define R_PARSE_DEBUG(PARSE, FORMAT, ...) \
    R_PARSE_X_AT(R_debug, (PARSE), (PARSE)->pos, FORMAT, __VA_ARGS__)


typedef int (*R_ParseReadFn)(int, unsigned char *, R_UserData);

typedef struct R_Parse {
    R_MAGIC_FIELD
    const char    *title;
    R_ParseReadFn read;
    R_UserData    user;
    int           bufsize;
    unsigned char *buffer;
    int           pos;
} R_Parse;


R_Parse R_parse(const char *title, R_ParseReadFn read, R_UserData user,
                int bufsize, unsigned char buffer[R_STATIC(bufsize)]);

int R_parse_from_file(int size, unsigned char *out, R_UserData user);

/*
 * Read `size` bytes using `parse->read` into the given `buf`fer. `R_die`s if
 * the number of bytes read was not equal to what we wanted. Returns how many
 * bytes were read (which is always equal to `size`).
 */
int R_parse_read_into(R_Parse *parse, int size,
                      unsigned char buf[R_STATIC(size)]);

/*
 * Read `nbytes` bytes using `parse->read` into `parse->buffer`. `R_die`s if the
 * buffer has less than `nbytes + extra` bytes of space, or if the number of
 * bytes read wasn't equal to `nbytes`.
 */
void R_parse_read_bytes_with_extra_space(R_Parse *parse, int nbytes, int extra);

/* Shortcut for `R_parse_read_bytes_with_extra_space(parse, nbytes, 0)`. */
void R_parse_read_bytes(R_Parse *parse, int nbytes);

/* Read a byte from `parse->read` and return it as an unsigned char. */
unsigned char R_parse_read_uchar(R_Parse *parse);

/* Read a byte from `parse->read` and return it as a char. */
char R_parse_read_char(R_Parse *parse);

/*
 * Read two bytes from `parse->read` and return them as an unsigned short. The
 * byte order read is always little endian, but the returned value will have the
 * appropriate endianness of your platform.
 */
unsigned short R_parse_read_ushort(R_Parse *parse);

/*
 * Same as `R_parse_read_ushort`, except for 4 bytes and an unsigned int.
 */
unsigned int R_parse_read_uint(R_Parse *parse);

/*
 * Read a floating point value from `parse_read`. The value must be represented
 * as a stringified, non-zero-terminated value prefixed by a single unsigned
 * byte telling the length of the string. Failure to `strtof` the string will
 * `R_die`, as will any infinite or NaN value.
 */
float R_parse_read_float(R_Parse *parse);

/*
 * Same as `R_parse_float`, except for double-precision floats.
 */
double R_parse_read_double(R_Parse *parse);


/*
 * Reads a string from `parse->read` and returns it in a newly allocated buffer.
 * The string must be formatted as a 16 bit unsigned little endian length
 * followed by the actual content of the string _without zero termination_.
 * The returned string will be zero-terminated though. If the length is zero,
 * `NULL` will be returned, *not* a buffer with just a zero byte in it.
 */
char *R_parse_read_string(R_Parse *parse);

/*
 * Like `R_parse_read_string`, except it just sticks it into `parse->buffer`.
 * Returns that, so don't free it or ovewrite it until you're done.
 */
char *R_parse_read_string_to_buffer(R_Parse *parse);


/*
 * Tries to read `parse->bufsize` bytes from `parse->read`, then `R_die`s if
 * any bytes were read. Use this to ensure you're at the end of file and there's
 * no trailing garbage left afterwards.
 */
void R_parse_die_unless_eof(R_Parse *parse);
