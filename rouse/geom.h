/*
 * geom.h - mostly a wrapper for cglm, generated from geom.pl
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
#define R_PI   CGLM_PI
#define R_PI_2 CGLM_PI_2
#define R_PI_4 CGLM_PI_4

typedef struct R_V2 { vec2   a; } R_V2;
typedef struct R_V3 { vec3   a; } R_V3;
typedef struct R_V4 { vec4   a; } R_V4;
typedef struct R_M3 { mat3   a; } R_M3;
typedef struct R_M4 { mat4   a; } R_M4;
typedef struct R_Qn { versor a; } R_Qn;

#define R_M3_GL(M) (*(M).a)
#define R_M4_GL(M) (*(M).a)

#define R_X(V) ((V).a[0])
#define R_Y(V) ((V).a[1])
#define R_Z(V) ((V).a[2])
#define R_W(V) ((V).a[3])

static inline float R_to_rad(float deg)
{
    return deg * (R_PI / 180.0f);
}

static inline float R_to_deg(float rad)
{
    return rad * (180.0f / R_PI);
}

static inline float R_power_of_two(float f)
{
    return powf(2.0f, ceilf(log2f(f)));
}

static inline R_V2 R_v2(float a, float b)
{
    return (R_V2){{a, b}};
}

static inline R_V3 R_v3(float a, float b, float c)
{
    return (R_V3){{a, b, c}};
}

static inline R_V4 R_v4(float a, float b, float c, float d)
{
    return (R_V4){{a, b, c, d}};
}

/* cglm/vec3.h */
static inline R_V3 R_v3_one(void)
{
    return (R_V3){GLM_VEC3_ONE_INIT};
}

static inline R_V3 R_v3_zero(void)
{
    return (R_V3){GLM_VEC3_ZERO_INIT};
}

static inline R_V3 R_v3_copy(R_V3 a)
{
    R_V3 dest;
    glm_vec_copy(a.a, dest.a);
    return dest;
}

static inline float R_v3_dot(R_V3 a, R_V3 b)
{
    return glm_vec_dot(a.a, b.a);
}

static inline R_V3 R_v3_cross(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_cross(a.a, b.a, dest.a);
    return dest;
}

static inline float R_v3_norm2(R_V3 v)
{
    return glm_vec_norm2(v.a);
}

static inline float R_v3_norm(R_V3 vec)
{
    return glm_vec_norm(vec.a);
}

static inline R_V3 R_v3_add(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_add(a.a, b.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_adds(R_V3 a, float s)
{
    R_V3 dest;
    glm_vec_adds(a.a, s, dest.a);
    return dest;
}

static inline R_V3 R_v3_sub(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_sub(a.a, b.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_subs(R_V3 a, float s)
{
    R_V3 dest;
    glm_vec_subs(a.a, s, dest.a);
    return dest;
}

static inline R_V3 R_v3_mul(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_mul(a.a, b.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_scale(R_V3 v, float s)
{
    R_V3 dest;
    glm_vec_scale(v.a, s, dest.a);
    return dest;
}

static inline R_V3 R_v3_scale_as(R_V3 v, float s)
{
    R_V3 dest;
    glm_vec_scale_as(v.a, s, dest.a);
    return dest;
}

static inline R_V3 R_v3_div(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_div(a.a, b.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_divs(R_V3 a, float s)
{
    R_V3 dest;
    glm_vec_divs(a.a, s, dest.a);
    return dest;
}

static inline R_V3 R_v3_addadd(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_addadd(a.a, b.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_subadd(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_subadd(a.a, b.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_muladd(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_muladd(a.a, b.a, dest.a);
    return dest;
}

static inline void R_v3_flipsign(R_V3 v)
{
    glm_vec_flipsign(v.a);
}

static inline void R_v3_inv(R_V3 v)
{
    glm_vec_inv(v.a);
}

static inline R_V3 R_v3_inv_to(R_V3 v)
{
    R_V3 dest;
    glm_vec_inv_to(v.a, dest.a);
    return dest;
}

static inline void R_v3_normalize(R_V3 v)
{
    glm_vec_normalize(v.a);
}

static inline R_V3 R_v3_normalize_to(R_V3 vec)
{
    R_V3 dest;
    glm_vec_normalize_to(vec.a, dest.a);
    return dest;
}

static inline float R_v3_distance(R_V3 v1, R_V3 v2)
{
    return glm_vec_distance(v1.a, v2.a);
}

static inline float R_v3_angle(R_V3 v1, R_V3 v2)
{
    return glm_vec_angle(v1.a, v2.a);
}

static inline void R_v3_rotate(R_V3 v, float angle, R_V3 axis)
{
    glm_vec_rotate(v.a, angle, axis.a);
}

static inline R_V3 R_v3_rotate_m4(R_M4 m, R_V3 v)
{
    R_V3 dest;
    glm_vec_rotate_m4(m.a, v.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_proj(R_V3 a, R_V3 b)
{
    R_V3 dest;
    glm_vec_proj(a.a, b.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_center(R_V3 v1, R_V3 v2)
{
    R_V3 dest;
    glm_vec_center(v1.a, v2.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_maxv(R_V3 v1, R_V3 v2)
{
    R_V3 dest;
    glm_vec_maxv(v1.a, v2.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_minv(R_V3 v1, R_V3 v2)
{
    R_V3 dest;
    glm_vec_minv(v1.a, v2.a, dest.a);
    return dest;
}

static inline R_V3 R_v3_ortho(R_V3 v)
{
    R_V3 dest;
    glm_vec_ortho(v.a, dest.a);
    return dest;
}

static inline void R_v3_clamp(R_V3 v, float min_val, float max_val)
{
    glm_vec_clamp(v.a, min_val, max_val);
}


/* cglm/vec3-ext.h */
static inline R_V3 R_v3_broadcast(float val)
{
    R_V3 dest;
    glm_vec_broadcast(val, dest.a);
    return dest;
}

static inline bool R_v3_eq(R_V3 v, float val)
{
    return glm_vec_eq(v.a, val);
}

static inline bool R_v3_eq_eps(R_V3 v, float val)
{
    return glm_vec_eq_eps(v.a, val);
}

static inline bool R_v3_eq_all(R_V3 v)
{
    return glm_vec_eq_all(v.a);
}

static inline bool R_v3_eqv(R_V3 v1, R_V3 v2)
{
    return glm_vec_eqv(v1.a, v2.a);
}

static inline bool R_v3_eqv_eps(R_V3 v1, R_V3 v2)
{
    return glm_vec_eqv_eps(v1.a, v2.a);
}

static inline float R_v3_max(R_V3 v)
{
    return glm_vec_max(v.a);
}

static inline float R_v3_min(R_V3 v)
{
    return glm_vec_min(v.a);
}


static inline R_V3 R_v3_dir(float hrad, float vrad)
{
    return R_v3(cosf(vrad) * sinf(hrad), sinf(vrad), cosf(vrad) * cosf(hrad));
}

static inline R_V3 R_v3_right(float hrad)
{
    return R_v3(sinf(hrad - (R_PI / 2.0f)), 0.0f, cosf(hrad - (R_PI / 2.0f)));
}

/* cglm/vec4.h */
static inline R_V4 R_v4_zero(void)
{
    return (R_V4){GLM_VEC4_ZERO_INIT};
}

static inline R_V4 R_v4_one(void)
{
    return (R_V4){GLM_VEC4_ONE_INIT};
}

static inline R_V4 R_v4_black(void)
{
    return (R_V4){GLM_VEC4_BLACK_INIT};
}

static inline R_V3 R_v4_copy3(R_V4 a)
{
    R_V3 dest;
    glm_vec4_copy3(a.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_copy(R_V4 v)
{
    R_V4 dest;
    glm_vec4_copy(v.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_ucopy(R_V4 v)
{
    R_V4 dest;
    glm_vec4_ucopy(v.a, dest.a);
    return dest;
}

static inline float R_v4_dot(R_V4 a, R_V4 b)
{
    return glm_vec4_dot(a.a, b.a);
}

static inline float R_v4_norm2(R_V4 v)
{
    return glm_vec4_norm2(v.a);
}

static inline float R_v4_norm(R_V4 vec)
{
    return glm_vec4_norm(vec.a);
}

static inline R_V4 R_v4_add(R_V4 a, R_V4 b)
{
    R_V4 dest;
    glm_vec4_add(a.a, b.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_adds(R_V4 v, float s)
{
    R_V4 dest;
    glm_vec4_adds(v.a, s, dest.a);
    return dest;
}

static inline R_V4 R_v4_sub(R_V4 a, R_V4 b)
{
    R_V4 dest;
    glm_vec4_sub(a.a, b.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_subs(R_V4 v, float s)
{
    R_V4 dest;
    glm_vec4_subs(v.a, s, dest.a);
    return dest;
}

static inline R_V4 R_v4_mul(R_V4 a, R_V4 b)
{
    R_V4 dest;
    glm_vec4_mul(a.a, b.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_scale(R_V4 v, float s)
{
    R_V4 dest;
    glm_vec4_scale(v.a, s, dest.a);
    return dest;
}

static inline R_V4 R_v4_scale_as(R_V4 v, float s)
{
    R_V4 dest;
    glm_vec4_scale_as(v.a, s, dest.a);
    return dest;
}

static inline R_V4 R_v4_div(R_V4 a, R_V4 b)
{
    R_V4 dest;
    glm_vec4_div(a.a, b.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_divs(R_V4 v, float s)
{
    R_V4 dest;
    glm_vec4_divs(v.a, s, dest.a);
    return dest;
}

static inline R_V4 R_v4_addadd(R_V4 a, R_V4 b)
{
    R_V4 dest;
    glm_vec4_addadd(a.a, b.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_subadd(R_V4 a, R_V4 b)
{
    R_V4 dest;
    glm_vec4_subadd(a.a, b.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_muladd(R_V4 a, R_V4 b)
{
    R_V4 dest;
    glm_vec4_muladd(a.a, b.a, dest.a);
    return dest;
}

static inline void R_v4_flipsign(R_V4 v)
{
    glm_vec4_flipsign(v.a);
}

static inline void R_v4_inv(R_V4 v)
{
    glm_vec4_inv(v.a);
}

static inline R_V4 R_v4_inv_to(R_V4 v)
{
    R_V4 dest;
    glm_vec4_inv_to(v.a, dest.a);
    return dest;
}

static inline void R_v4_normalize(R_V4 v)
{
    glm_vec4_normalize(v.a);
}

static inline R_V4 R_v4_normalize_to(R_V4 vec)
{
    R_V4 dest;
    glm_vec4_normalize_to(vec.a, dest.a);
    return dest;
}

static inline float R_v4_distance(R_V4 v1, R_V4 v2)
{
    return glm_vec4_distance(v1.a, v2.a);
}

static inline R_V4 R_v4_maxv(R_V4 v1, R_V4 v2)
{
    R_V4 dest;
    glm_vec4_maxv(v1.a, v2.a, dest.a);
    return dest;
}

static inline R_V4 R_v4_minv(R_V4 v1, R_V4 v2)
{
    R_V4 dest;
    glm_vec4_minv(v1.a, v2.a, dest.a);
    return dest;
}

static inline void R_v4_clamp(R_V4 v, float min_val, float max_val)
{
    glm_vec4_clamp(v.a, min_val, max_val);
}

static inline R_V4 R_v4_lerp(R_V4 from, R_V4 to, float t)
{
    R_V4 dest;
    glm_vec4_lerp(from.a, to.a, t, dest.a);
    return dest;
}


/* cglm/vec4-ext.h */
static inline R_V4 R_v4_broadcast(float val)
{
    R_V4 dest;
    glm_vec4_broadcast(val, dest.a);
    return dest;
}

static inline bool R_v4_eq(R_V4 v, float val)
{
    return glm_vec4_eq(v.a, val);
}

static inline bool R_v4_eq_eps(R_V4 v, float val)
{
    return glm_vec4_eq_eps(v.a, val);
}

static inline bool R_v4_eq_all(R_V4 v)
{
    return glm_vec4_eq_all(v.a);
}

static inline bool R_v4_eqv(R_V4 v1, R_V4 v2)
{
    return glm_vec4_eqv(v1.a, v2.a);
}

static inline bool R_v4_eqv_eps(R_V4 v1, R_V4 v2)
{
    return glm_vec4_eqv_eps(v1.a, v2.a);
}

static inline float R_v4_max(R_V4 v)
{
    return glm_vec4_max(v.a);
}

static inline float R_v4_min(R_V4 v)
{
    return glm_vec4_min(v.a);
}


/* cglm/mat3.h */
static inline R_M3 R_m3_identity(void)
{
    return (R_M3){GLM_MAT3_IDENTITY_INIT};
}

static inline R_M3 R_m3_zero(void)
{
    return (R_M3){GLM_MAT3_ZERO_INIT};
}

static inline R_M3 R_m3_copy(R_M3 mat)
{
    R_M3 dest;
    glm_mat3_copy(mat.a, dest.a);
    return dest;
}

static inline R_M3 R_m3_mul(R_M3 m1, R_M3 m2)
{
    R_M3 dest;
    glm_mat3_mul(m1.a, m2.a, dest.a);
    return dest;
}

static inline R_M3 R_m3_transpose_to(R_M3 m)
{
    R_M3 dest;
    glm_mat3_transpose_to(m.a, dest.a);
    return dest;
}

static inline void R_m3_transpose(R_M3 m)
{
    glm_mat3_transpose(m.a);
}

static inline R_V3 R_m3_mulv(R_M3 m, R_V3 v)
{
    R_V3 dest;
    glm_mat3_mulv(m.a, v.a, dest.a);
    return dest;
}

static inline void R_m3_scale(R_M3 m, float s)
{
    glm_mat3_scale(m.a, s);
}

static inline float R_m3_det(R_M3 mat)
{
    return glm_mat3_det(mat.a);
}

static inline R_M3 R_m3_inv(R_M3 mat)
{
    R_M3 dest;
    glm_mat3_inv(mat.a, dest.a);
    return dest;
}

static inline void R_m3_swap_col(R_M3 mat, int col1, int col2)
{
    glm_mat3_swap_col(mat.a, col1, col2);
}

static inline void R_m3_swap_row(R_M3 mat, int row1, int row2)
{
    glm_mat3_swap_row(mat.a, row1, row2);
}


/* cglm/mat4.h */
static inline R_M4 R_m4_identity(void)
{
    return (R_M4){GLM_MAT4_IDENTITY_INIT};
}

static inline R_M4 R_m4_zero(void)
{
    return (R_M4){GLM_MAT4_ZERO_INIT};
}

static inline R_M4 R_m4_ucopy(R_M4 mat)
{
    R_M4 dest;
    glm_mat4_ucopy(mat.a, dest.a);
    return dest;
}

static inline R_M4 R_m4_copy(R_M4 mat)
{
    R_M4 dest;
    glm_mat4_copy(mat.a, dest.a);
    return dest;
}

static inline R_M3 R_m4_pick3(R_M4 mat)
{
    R_M3 dest;
    glm_mat4_pick3(mat.a, dest.a);
    return dest;
}

static inline R_M3 R_m4_pick3t(R_M4 mat)
{
    R_M3 dest;
    glm_mat4_pick3t(mat.a, dest.a);
    return dest;
}

static inline R_M4 R_m4_ins3(R_M3 mat)
{
    R_M4 dest;
    glm_mat4_ins3(mat.a, dest.a);
    return dest;
}

static inline R_M4 R_m4_mul(R_M4 m1, R_M4 m2)
{
    R_M4 dest;
    glm_mat4_mul(m1.a, m2.a, dest.a);
    return dest;
}

static inline R_V4 R_m4_mulv(R_M4 m, R_V4 v)
{
    R_V4 dest;
    glm_mat4_mulv(m.a, v.a, dest.a);
    return dest;
}

static inline R_V3 R_m4_mulv3(R_M4 m, R_V3 v, float last)
{
    R_V3 dest;
    glm_mat4_mulv3(m.a, v.a, last, dest.a);
    return dest;
}

static inline R_M4 R_m4_transpose_to(R_M4 m)
{
    R_M4 dest;
    glm_mat4_transpose_to(m.a, dest.a);
    return dest;
}

static inline void R_m4_transpose(R_M4 m)
{
    glm_mat4_transpose(m.a);
}

static inline void R_m4_scale_p(R_M4 m, float s)
{
    glm_mat4_scale_p(m.a, s);
}

static inline void R_m4_scale(R_M4 m, float s)
{
    glm_mat4_scale(m.a, s);
}

static inline float R_m4_det(R_M4 mat)
{
    return glm_mat4_det(mat.a);
}

static inline R_M4 R_m4_inv(R_M4 mat)
{
    R_M4 dest;
    glm_mat4_inv(mat.a, dest.a);
    return dest;
}

static inline R_M4 R_m4_inv_fast(R_M4 mat)
{
    R_M4 dest;
    glm_mat4_inv_fast(mat.a, dest.a);
    return dest;
}

static inline void R_m4_swap_col(R_M4 mat, int col1, int col2)
{
    glm_mat4_swap_col(mat.a, col1, col2);
}

static inline void R_m4_swap_row(R_M4 mat, int row1, int row2)
{
    glm_mat4_swap_row(mat.a, row1, row2);
}


/* cglm/cam.h */
static inline R_M4 R_m4_perspective(float fovy, float aspect, float near_val, float far_val)
{
    R_M4 dest;
    glm_perspective(fovy, aspect, near_val, far_val, dest.a);
    return dest;
}

static inline R_M4 R_m4_perspective_default(float aspect)
{
    R_M4 dest;
    glm_perspective_default(aspect, dest.a);
    return dest;
}

static inline R_M4 R_m4_look_at(R_V3 eye, R_V3 center, R_V3 up)
{
    R_M4 dest;
    glm_lookat(eye.a, center.a, up.a, dest.a);
    return dest;
}

static inline R_M4 R_m4_look(R_V3 eye, R_V3 dir, R_V3 up)
{
    R_M4 dest;
    glm_look(eye.a, dir.a, up.a, dest.a);
    return dest;
}


/* cglm/affine.h */
static inline R_M4 R_m4_translate_to(R_M4 m, R_V3 v)
{
    R_M4 dest;
    glm_translate_to(m.a, v.a, dest.a);
    return dest;
}

static inline void R_m4_translate(R_M4 m, R_V3 v)
{
    glm_translate(m.a, v.a);
}

static inline void R_m4_translate_x(R_M4 m, float to)
{
    glm_translate_x(m.a, to);
}

static inline void R_m4_translate_y(R_M4 m, float to)
{
    glm_translate_y(m.a, to);
}

static inline void R_m4_translate_z(R_M4 m, float to)
{
    glm_translate_z(m.a, to);
}

static inline void R_m4_translate_make(R_M4 m, R_V3 v)
{
    glm_translate_make(m.a, v.a);
}

static inline R_M4 R_m4_rotate_x(R_M4 m, float angle)
{
    R_M4 dest;
    glm_rotate_x(m.a, angle, dest.a);
    return dest;
}

static inline R_M4 R_m4_rotate_y(R_M4 m, float angle)
{
    R_M4 dest;
    glm_rotate_y(m.a, angle, dest.a);
    return dest;
}

static inline R_M4 R_m4_rotate_z(R_M4 m, float angle)
{
    R_M4 dest;
    glm_rotate_z(m.a, angle, dest.a);
    return dest;
}


static inline R_M4 R_m4_rotate_xyz(R_M4 m, R_V3 angles)
{
    return R_m4_rotate_z(R_m4_rotate_y(R_m4_rotate_x(m, R_X(angles)),
                                                        R_Y(angles)),
                                                        R_Z(angles));
}
