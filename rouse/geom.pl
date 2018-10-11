#!/usr/bin/env perl
#
# geom.pl - generates geom.h by wrapping cglm functions
#
# Copyright (c) 2019 askmeaboutloom
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
use strict;
use warnings;


my %name_types = (
    vec         => 'v3',
    vec3        => 'v3',
    vec4        => 'v4',
    mat3        => 'm3',
    mat4        => 'm4',
    perspective => 'm4_perspective',
    look        => 'm4_look',
    lookat      => 'm4_look_at',
    rotate      => 'm4_rotate',
    translate   => 'm4_translate',
);

my %arg_types = (
    float => 'float',
    int   => 'int',
    vec3  => 'R_V3',
    vec4  => 'R_V4',
    mat3  => 'R_M3',
    mat4  => 'R_M4',
);

my %arg_unwrap = (
    int   => '',
    float => '',
    R_V2  => '.a',
    R_V3  => '.a',
    R_V4  => '.a',
    R_M3  => '.a',
    R_M4  => '.a',
    R_Qn  => '.a',
);


sub translate_name {
    my ($name) = @_;
    $name =~ /\A[a-z0-9]+_([a-z0-9]+)(?:_(\w+))?\z/ or die "weird name: '$name'";
    my $prefix = $name_types{$1} or die "unknown type prefix: '$1'";
    my $suffix = $2 ? "_$2" : '';
    return "R_$prefix$suffix";
}


sub wrap_arg_type {
    my ($type) = @_;
    return $arg_types{$type} // die "unknown arg type '$type'";
}

sub wrap_arg_name {
    my ($name) = @_;
    $name =~ s/(\p{Lu})/_\l$1/g;
    return $name;
}

sub wrap_arg_pair {
    my ($pair) = @_;
    return {
        type => wrap_arg_type($_->[0]),
        name => wrap_arg_name($_->[1]),
    };
}

sub parse_args {
    my ($ret, $args) = @_;
    my @wrapped_args = map { wrap_arg_pair($_) }
                       map { [split ' ', $_, 2] }
                       split /\s*,\s*/, $args;

    my @dest = grep { $_->{name} eq 'dest' } @wrapped_args;
    die "got more than one dest in '$args'" if @dest > 1;

    if (@dest == 1) {
        die "got dest and non-void return '$ret'" if $ret ne 'void';
        $dest[0]{dest} = 1;
        $ret = $dest[0]{type};
    }

    return ($ret, @wrapped_args);
}

sub wrap_cglm_call {
    my ($line) = @_;
    $line =~ /\A\s*(\w+)\s+(\w+)\s*\((.+)\)\s*\z/ or die "don't understand '$line'";
    my ($ret, $name, $args) = ($1, $2, $3);
    my $wrapped_name = translate_name($name);
    my ($wrapped_ret, @wrapped_args) = parse_args($ret, $args);

    return {
        name    => $name,
        ret     => $ret,
        wrapped => {
            name => $wrapped_name,
            ret  => $wrapped_ret,
            args => \@wrapped_args,
        },
    };
}


sub unwrap_arg {
    my ($wrapped) = @_;
    my $unwrap = $arg_unwrap{$wrapped->{type}} // die "can't unwrap '$wrapped->{type}'";
    return "$wrapped->{name}$unwrap";
}

sub print_args {
    my ($fn) = @_;
    print join ', ', map { unwrap_arg($_) } @{$fn->{wrapped}{args}};
}

sub print_wrapper {
    my ($fn) = @_;
    print "static inline $fn->{wrapped}{ret} $fn->{wrapped}{name}(";

    print join ', ',
          map { "$_->{type} $_->{name}" }
          grep { !$_->{dest} } @{$fn->{wrapped}{args}};

    print ")\n";
    print "{\n";

    my ($dest) = grep { $_->{dest} } @{$fn->{wrapped}{args}};
    if ($dest) {
        print "    $dest->{type} dest;\n";
        print "    $fn->{name}(";
        print_args($fn);
        print ");\n";
        print "    return dest;\n";
    }
    else {
        print '    ';
        print 'return ' if $fn->{ret} ne 'void';
        print "$fn->{name}(";
        print_args($fn);
        print ");\n";
    }

    print "}\n";
    print "\n";
}


sub print_value {
    my ($ret, $name, $value) = @_;
    print "static inline $ret \u\L$ret\E_$name(void)\n";
    print "{\n";
    print "    return ($ret){$value};\n";
    print "}\n";
    print "\n";
}


while (<DATA>) {
    if (/\A\s*\@\s*value\s+(\w+)\s+(\w+)\s+(\w+)\s*\z/) {
        print_value($1, $2, $3);
    }
    elsif (/\A\s*\@(.+\(.*\).*)\s*\;?\s*\z/) {
        print_wrapper(wrap_cglm_call($1));
    }
    else {
        print;
    }
}

__DATA__
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
@ value R_V3 one  GLM_VEC3_ONE_INIT
@ value R_V3 zero GLM_VEC3_ZERO_INIT
@ void  glm_vec_copy(vec3 a, vec3 dest)
@ float glm_vec_dot(vec3 a, vec3 b)
@ void  glm_vec_cross(vec3 a, vec3 b, vec3 dest)
@ float glm_vec_norm2(vec3 v)
@ float glm_vec_norm(vec3 vec)
@ void  glm_vec_add(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_adds(vec3 a, float s, vec3 dest)
@ void  glm_vec_sub(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_subs(vec3 a, float s, vec3 dest)
@ void  glm_vec_mul(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_scale(vec3 v, float s, vec3 dest)
@ void  glm_vec_scale_as(vec3 v, float s, vec3 dest)
@ void  glm_vec_div(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_divs(vec3 a, float s, vec3 dest)
@ void  glm_vec_addadd(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_subadd(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_muladd(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_flipsign(vec3 v)
@ void  glm_vec_inv(vec3 v)
@ void  glm_vec_inv_to(vec3 v, vec3 dest)
@ void  glm_vec_normalize(vec3 v)
@ void  glm_vec_normalize_to(vec3 vec, vec3 dest)
@ float glm_vec_distance(vec3 v1, vec3 v2)
@ float glm_vec_angle(vec3 v1, vec3 v2)
@ void  glm_vec_rotate(vec3 v, float angle, vec3 axis)
@ void  glm_vec_rotate_m4(mat4 m, vec3 v, vec3 dest)
@ void  glm_vec_proj(vec3 a, vec3 b, vec3 dest)
@ void  glm_vec_center(vec3 v1, vec3 v2, vec3 dest)
@ void  glm_vec_maxv(vec3 v1, vec3 v2, vec3 dest)
@ void  glm_vec_minv(vec3 v1, vec3 v2, vec3 dest)
@ void  glm_vec_ortho(vec3 v, vec3 dest)
@ void  glm_vec_clamp(vec3 v, float minVal, float maxVal)

/* cglm/vec3-ext.h */
@ void  glm_vec_broadcast(float val, vec3 dest)
@ bool  glm_vec_eq(vec3 v, float val)
@ bool  glm_vec_eq_eps(vec3 v, float val)
@ bool  glm_vec_eq_all(vec3 v)
@ bool  glm_vec_eqv(vec3 v1, vec3 v2)
@ bool  glm_vec_eqv_eps(vec3 v1, vec3 v2)
@ float glm_vec_max(vec3 v)
@ float glm_vec_min(vec3 v)

static inline R_V3 R_v3_dir(float hrad, float vrad)
{
    return R_v3(cosf(vrad) * sinf(hrad), sinf(vrad), cosf(vrad) * cosf(hrad));
}

static inline R_V3 R_v3_right(float hrad)
{
    return R_v3(sinf(hrad - (R_PI / 2.0f)), 0.0f, cosf(hrad - (R_PI / 2.0f)));
}

/* cglm/vec4.h */
@ value R_V4 zero  GLM_VEC4_ZERO_INIT
@ value R_V4 one   GLM_VEC4_ONE_INIT
@ value R_V4 black GLM_VEC4_BLACK_INIT
@ void  glm_vec4_copy3(vec4 a, vec3 dest)
@ void  glm_vec4_copy(vec4 v, vec4 dest)
@ void  glm_vec4_ucopy(vec4 v, vec4 dest)
@ float glm_vec4_dot(vec4 a, vec4 b)
@ float glm_vec4_norm2(vec4 v)
@ float glm_vec4_norm(vec4 vec)
@ void  glm_vec4_add(vec4 a, vec4 b, vec4 dest)
@ void  glm_vec4_adds(vec4 v, float s, vec4 dest)
@ void  glm_vec4_sub(vec4 a, vec4 b, vec4 dest)
@ void  glm_vec4_subs(vec4 v, float s, vec4 dest)
@ void  glm_vec4_mul(vec4 a, vec4 b, vec4 dest)
@ void  glm_vec4_scale(vec4 v, float s, vec4 dest)
@ void  glm_vec4_scale_as(vec4 v, float s, vec4 dest)
@ void  glm_vec4_div(vec4 a, vec4 b, vec4 dest)
@ void  glm_vec4_divs(vec4 v, float s, vec4 dest)
@ void  glm_vec4_addadd(vec4 a, vec4 b, vec4 dest)
@ void  glm_vec4_subadd(vec4 a, vec4 b, vec4 dest)
@ void  glm_vec4_muladd(vec4 a, vec4 b, vec4 dest)
@ void  glm_vec4_flipsign(vec4 v)
@ void  glm_vec4_inv(vec4 v)
@ void  glm_vec4_inv_to(vec4 v, vec4 dest)
@ void  glm_vec4_normalize(vec4 v)
@ void  glm_vec4_normalize_to(vec4 vec, vec4 dest)
@ float glm_vec4_distance(vec4 v1, vec4 v2)
@ void  glm_vec4_maxv(vec4 v1, vec4 v2, vec4 dest)
@ void  glm_vec4_minv(vec4 v1, vec4 v2, vec4 dest)
@ void  glm_vec4_clamp(vec4 v, float minVal, float maxVal)
@ void  glm_vec4_lerp(vec4 from, vec4 to, float t, vec4 dest)

/* cglm/vec4-ext.h */
@ void  glm_vec4_broadcast(float val, vec4 dest)
@ bool  glm_vec4_eq(vec4 v, float val)
@ bool  glm_vec4_eq_eps(vec4 v, float val)
@ bool  glm_vec4_eq_all(vec4 v)
@ bool  glm_vec4_eqv(vec4 v1, vec4 v2)
@ bool  glm_vec4_eqv_eps(vec4 v1, vec4 v2)
@ float glm_vec4_max(vec4 v)
@ float glm_vec4_min(vec4 v)

/* cglm/mat3.h */
@ value R_M3 identity GLM_MAT3_IDENTITY_INIT
@ value R_M3 zero     GLM_MAT3_ZERO_INIT
@ void  glm_mat3_copy(mat3 mat, mat3 dest)
@ void  glm_mat3_mul(mat3 m1, mat3 m2, mat3 dest)
@ void  glm_mat3_transpose_to(mat3 m, mat3 dest)
@ void  glm_mat3_transpose(mat3 m)
@ void  glm_mat3_mulv(mat3 m, vec3 v, vec3 dest)
@ void  glm_mat3_scale(mat3 m, float s)
@ float glm_mat3_det(mat3 mat)
@ void  glm_mat3_inv(mat3 mat, mat3 dest)
@ void  glm_mat3_swap_col(mat3 mat, int col1, int col2)
@ void  glm_mat3_swap_row(mat3 mat, int row1, int row2)

/* cglm/mat4.h */
@ value R_M4 identity GLM_MAT4_IDENTITY_INIT
@ value R_M4 zero     GLM_MAT4_ZERO_INIT
@ void  glm_mat4_ucopy(mat4 mat, mat4 dest)
@ void  glm_mat4_copy(mat4 mat, mat4 dest)
@ void  glm_mat4_pick3(mat4 mat, mat3 dest)
@ void  glm_mat4_pick3t(mat4 mat, mat3 dest)
@ void  glm_mat4_ins3(mat3 mat, mat4 dest)
@ void  glm_mat4_mul(mat4 m1, mat4 m2, mat4 dest)
@ void  glm_mat4_mulv(mat4 m, vec4 v, vec4 dest)
@ void  glm_mat4_mulv3(mat4 m, vec3 v, float last, vec3 dest)
@ void  glm_mat4_transpose_to(mat4 m, mat4 dest)
@ void  glm_mat4_transpose(mat4 m)
@ void  glm_mat4_scale_p(mat4 m, float s)
@ void  glm_mat4_scale(mat4 m, float s)
@ float glm_mat4_det(mat4 mat)
@ void  glm_mat4_inv(mat4 mat, mat4 dest)
@ void  glm_mat4_inv_fast(mat4 mat, mat4 dest)
@ void  glm_mat4_swap_col(mat4 mat, int col1, int col2)
@ void  glm_mat4_swap_row(mat4 mat, int row1, int row2)

/* cglm/cam.h */
@ void glm_perspective(float fovy, float aspect, float nearVal, float farVal, mat4 dest)
@ void glm_perspective_default(float aspect, mat4 dest)
@ void glm_lookat(vec3 eye, vec3 center, vec3 up, mat4 dest)
@ void glm_look(vec3 eye, vec3 dir, vec3 up, mat4 dest)

/* cglm/affine.h */
@ void glm_translate_to(mat4 m, vec3 v, mat4 dest)
@ void glm_translate(mat4 m, vec3 v)
@ void glm_translate_x(mat4 m, float to)
@ void glm_translate_y(mat4 m, float to)
@ void glm_translate_z(mat4 m, float to)
@ void glm_translate_make(mat4 m, vec3 v)
@ void glm_rotate_x(mat4 m, float angle, mat4 dest)
@ void glm_rotate_y(mat4 m, float angle, mat4 dest)
@ void glm_rotate_z(mat4 m, float angle, mat4 dest)

static inline R_M4 R_m4_rotate_xyz(R_M4 m, R_V3 angles)
{
    return R_m4_rotate_z(R_m4_rotate_y(R_m4_rotate_x(m, R_X(angles)),
                                                        R_Y(angles)),
                                                        R_Z(angles));
}
