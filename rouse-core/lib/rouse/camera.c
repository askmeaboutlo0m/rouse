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
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <cglm/struct.h>
#include "3rdparty/gles2_inc.h"
#include <rouse_config.h>
#include "common.h"
#include "geom.h"
#include "render/gl.h"
#include "camera.h"


R_Camera *R_camera_new(void)
{
    R_Camera *camera = R_NEW_INIT_STRUCT(camera, R_Camera,
            R_MAGIC_INIT(R_Camera) R_m4_identity(), R_m4_identity());
    R_MAGIC_CHECK(R_Camera, camera);
    return camera;
}

R_Camera *R_camera_new_perspective(float fov, float aspect_ratio,
                                   float near, float far)
{
    R_Camera *camera = R_camera_new();
    camera->proj = R_m4_perspective(fov, aspect_ratio, near, far);
    return camera;
}

void R_camera_free(R_Camera *camera)
{
    if (camera) {
        R_MAGIC_POISON(R_Camera, camera);
        free(camera);
    }
}

static void bind_matrix(R_M4 *matrix, int location)
{
    R_GL(glUniformMatrix4fv, location, 1, GL_FALSE, R_M4_GL(*matrix));
}

void R_camera_bind(R_Camera *camera, int proj_location, int view_location)
{
    R_MAGIC_CHECK(R_Camera, camera);
    if (proj_location != 0) {
        R_debug("bind projection matrix to uniform location %d", proj_location);
        bind_matrix(&camera->proj, proj_location);
    }
    if (view_location != 0) {
        R_debug("bind view matrix to uniform location %d", view_location);
        bind_matrix(&camera->view, view_location);
    }
}


R_FirstPerson *R_first_person_new(R_V3 pos, float h, float v)
{
    R_FirstPerson *fp = R_NEW_INIT_STRUCT(fp, R_FirstPerson,
            R_MAGIC_INIT(R_FirstPerson) pos, h, v);
    R_MAGIC_CHECK(R_FirstPerson, fp);
    return fp;
}

void R_first_person_free(R_FirstPerson *fp)
{
    if (fp) {
        R_MAGIC_POISON(R_FirstPerson, fp);
        free(fp);
    }
}


void R_first_person_directions(R_FirstPerson *fp, R_V3 *out_forward,
                               R_V3 *out_right, R_V3 *out_up)
{
    R_MAGIC_CHECK(R_FirstPerson, fp);

    R_V3 forward;
    if (out_forward || out_up) {
        forward = R_v3_dir(fp->h, fp->v);
        if (out_forward) {
            *out_forward = forward;
        }

    }

    R_V3 right;
    if (out_right || out_up) {
        right = R_v3_right(fp->h);
        if (out_right) {
            *out_right = right;
        }
    }

    if (out_up) {
        *out_up = R_v3_cross(right, forward);
    }
}


void R_first_person_look(R_FirstPerson *fp, float hd, float vd)
{
    R_MAGIC_CHECK(R_FirstPerson, fp);
    fp->h += hd;
    fp->v += vd;
}

void R_first_person_move(R_FirstPerson *fp, float forward, float right, float up)
{
    R_MAGIC_CHECK(R_FirstPerson, fp);
    R_V3 fv = R_v3_dir(fp->h, fp->v);
    R_V3 rv = R_v3_right(fp->h);
    R_V3 uv = R_v3_cross(rv, fv);
    fp->pos = R_v3_add(fp->pos, R_v3_scale(fv, forward));
    fp->pos = R_v3_add(fp->pos, R_v3_scale(rv, right  ));
    fp->pos = R_v3_add(fp->pos, R_v3_scale(uv, up     ));
}

void R_first_person_apply(R_FirstPerson *fp, R_Camera *camera)
{
    R_MAGIC_CHECK(R_FirstPerson, fp);
    R_MAGIC_CHECK(R_Camera, camera);
    R_V3 fv, rv, uv;
    R_first_person_directions(fp, &fv, &rv, &uv);
    camera->view = R_m4_look_at(fp->pos, R_v3_add(fp->pos, fv), uv);
}
