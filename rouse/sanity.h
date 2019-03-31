/*
 * sanity.h - static assertions to ensure the expected environment.
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
 * For the model loader, it converts between char and byte buffers quite a bit.
 * On a system where this doesn't hold, you probably don't even have a uint8_t
 * in the first place. Not one I'd want to be in, cause it ain't supported.
 */
static_assert(sizeof(char) == sizeof(uint8_t), "char must be a byte");

/*
 * I don't like `#include`ing GL headers everywhere just to get these silly
 * types. So this ensures that they're identical to the expected normal C
 * types and those are used everywhere instead.
 */
#define R_SANITY_TYPES(X) \
    X(unsigned int,   GLenum    ) \
    X(unsigned int,   GLbitfield) \
    X(unsigned int,   GLuint    ) \
    X(int,            GLint     ) \
    X(int,            GLsizei   ) \
    X(unsigned char,  GLboolean ) \
    X(signed char,    GLbyte    ) \
    X(short,          GLshort   ) \
    X(unsigned char,  GLubyte   ) \
    X(unsigned short, GLushort  ) \
    X(float,          GLfloat   ) \
    X(float,          GLclampf  ) \
    X(double,         GLdouble  ) \
    X(double,         GLclampd  ) \
    X(ptrdiff_t,      GLsizeiptr)


#define R_SANITY_CHECK_SIZEOF(A, B) \
    static_assert(sizeof(A) == sizeof(B), "sizeof(" #A ") == sizeof(" #B ")");

R_SANITY_TYPES(R_SANITY_CHECK_SIZEOF)

#undef R_SANITY_CHECK_SIZEOF


#ifdef __GNUC__
#define R_SANITY_CHECK_COMPATIBLE(A, B) \
    static_assert(__builtin_types_compatible_p(A, B), #A " is " #B);

R_SANITY_TYPES(R_SANITY_CHECK_COMPATIBLE)

#undef R_SANITY_CHECK_COMPATIBLE
#endif


#undef R_SANITY_TYPES
