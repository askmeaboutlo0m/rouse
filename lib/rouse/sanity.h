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
    X(uint32_t,       Uint32    )


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


/*
 * These cglm types do some questionable things, conflating structs of floats
 * with arrays thereof. While that probably works, let's make extra sure it does.
 */
#define R_SANITY_OFFSET(TYPE_A, FIELD_A, TYPE_B, FIELD_B) \
    static_assert(offsetof(TYPE_A, FIELD_A) == offsetof(TYPE_B, FIELD_B), \
                  "offsetof(" #TYPE_A ", " #FIELD_A ") == " \
                  "offsetof(" #TYPE_B ", " #FIELD_B ")");

#define R_SANITY_OFFSET_2(TYPE, A, B) \
    R_SANITY_OFFSET(TYPE, A, TYPE, B)

#define R_SANITY_OFFSET_3(TYPE, A, B, C) \
    R_SANITY_OFFSET_2(TYPE, A, B) \
    R_SANITY_OFFSET_2(TYPE, A, C) \
    R_SANITY_OFFSET_2(TYPE, B, C)

#define R_SANITY_OFFSET_ARRAY(TYPE, FIELD, ARRAY) \
    static_assert(offsetof(TYPE, A) == offsetof(ARRAY, INDEX)

R_SANITY_OFFSET_2(R_V2, raw[0], x)
R_SANITY_OFFSET_2(R_V2, raw[1], y)

R_SANITY_OFFSET_2(R_V3, raw[0], x)
R_SANITY_OFFSET_2(R_V3, raw[1], y)
R_SANITY_OFFSET_2(R_V3, raw[2], z)

R_SANITY_OFFSET_2(R_V4, raw[0], x)
R_SANITY_OFFSET_2(R_V4, raw[1], y)
R_SANITY_OFFSET_2(R_V4, raw[2], z)
R_SANITY_OFFSET_2(R_V4, raw[3], w)

R_SANITY_OFFSET_3(R_Qn, raw[0], x, imag.x)
R_SANITY_OFFSET_3(R_Qn, raw[1], y, imag.y)
R_SANITY_OFFSET_3(R_Qn, raw[2], z, imag.z)
R_SANITY_OFFSET_3(R_Qn, raw[3], w, real)

R_SANITY_OFFSET(R_M3, raw[0][0], struct { float a[9]; }, a[0])
R_SANITY_OFFSET(R_M3, raw[0][1], struct { float a[9]; }, a[1])
R_SANITY_OFFSET(R_M3, raw[0][2], struct { float a[9]; }, a[2])
R_SANITY_OFFSET(R_M3, raw[1][0], struct { float a[9]; }, a[3])
R_SANITY_OFFSET(R_M3, raw[1][1], struct { float a[9]; }, a[4])
R_SANITY_OFFSET(R_M3, raw[1][2], struct { float a[9]; }, a[5])
R_SANITY_OFFSET(R_M3, raw[2][0], struct { float a[9]; }, a[6])
R_SANITY_OFFSET(R_M3, raw[2][1], struct { float a[9]; }, a[7])
R_SANITY_OFFSET(R_M3, raw[2][2], struct { float a[9]; }, a[8])

R_SANITY_OFFSET_3(R_M3, raw[0][0], m00, col[0].x)
R_SANITY_OFFSET_3(R_M3, raw[0][1], m01, col[0].y)
R_SANITY_OFFSET_3(R_M3, raw[0][2], m02, col[0].z)
R_SANITY_OFFSET_3(R_M3, raw[1][0], m10, col[1].x)
R_SANITY_OFFSET_3(R_M3, raw[1][1], m11, col[1].y)
R_SANITY_OFFSET_3(R_M3, raw[1][2], m12, col[1].z)
R_SANITY_OFFSET_3(R_M3, raw[2][0], m20, col[2].x)
R_SANITY_OFFSET_3(R_M3, raw[2][1], m21, col[2].y)
R_SANITY_OFFSET_3(R_M3, raw[2][2], m22, col[2].z)

R_SANITY_OFFSET(R_M4, raw[0][0], struct { float a[16]; }, a[ 0])
R_SANITY_OFFSET(R_M4, raw[0][1], struct { float a[16]; }, a[ 1])
R_SANITY_OFFSET(R_M4, raw[0][2], struct { float a[16]; }, a[ 2])
R_SANITY_OFFSET(R_M4, raw[0][3], struct { float a[16]; }, a[ 3])
R_SANITY_OFFSET(R_M4, raw[1][0], struct { float a[16]; }, a[ 4])
R_SANITY_OFFSET(R_M4, raw[1][1], struct { float a[16]; }, a[ 5])
R_SANITY_OFFSET(R_M4, raw[1][2], struct { float a[16]; }, a[ 6])
R_SANITY_OFFSET(R_M4, raw[1][3], struct { float a[16]; }, a[ 7])
R_SANITY_OFFSET(R_M4, raw[2][0], struct { float a[16]; }, a[ 8])
R_SANITY_OFFSET(R_M4, raw[2][1], struct { float a[16]; }, a[ 9])
R_SANITY_OFFSET(R_M4, raw[2][2], struct { float a[16]; }, a[10])
R_SANITY_OFFSET(R_M4, raw[2][3], struct { float a[16]; }, a[11])
R_SANITY_OFFSET(R_M4, raw[3][0], struct { float a[16]; }, a[12])
R_SANITY_OFFSET(R_M4, raw[3][1], struct { float a[16]; }, a[13])
R_SANITY_OFFSET(R_M4, raw[3][2], struct { float a[16]; }, a[14])
R_SANITY_OFFSET(R_M4, raw[3][3], struct { float a[16]; }, a[15])

R_SANITY_OFFSET_3(R_M4, raw[0][0], m00, col[0].x)
R_SANITY_OFFSET_3(R_M4, raw[0][1], m01, col[0].y)
R_SANITY_OFFSET_3(R_M4, raw[0][2], m02, col[0].z)
R_SANITY_OFFSET_3(R_M4, raw[0][3], m03, col[0].w)
R_SANITY_OFFSET_3(R_M4, raw[1][0], m10, col[1].x)
R_SANITY_OFFSET_3(R_M4, raw[1][1], m11, col[1].y)
R_SANITY_OFFSET_3(R_M4, raw[1][2], m12, col[1].z)
R_SANITY_OFFSET_3(R_M4, raw[1][3], m13, col[1].w)
R_SANITY_OFFSET_3(R_M4, raw[2][0], m20, col[2].x)
R_SANITY_OFFSET_3(R_M4, raw[2][1], m21, col[2].y)
R_SANITY_OFFSET_3(R_M4, raw[2][2], m22, col[2].z)
R_SANITY_OFFSET_3(R_M4, raw[2][3], m23, col[2].w)
R_SANITY_OFFSET_3(R_M4, raw[3][0], m30, col[3].x)
R_SANITY_OFFSET_3(R_M4, raw[3][1], m31, col[3].y)
R_SANITY_OFFSET_3(R_M4, raw[3][2], m32, col[3].z)
R_SANITY_OFFSET_3(R_M4, raw[3][3], m33, col[3].w)

#undef R_SANITY_OFFSET_3
#undef R_SANITY_OFFSET_2
#undef R_SANITY_OFFSET
