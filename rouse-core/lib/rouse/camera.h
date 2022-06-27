/*
 * camera.h - views, projections and things that bind them.
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

typedef struct R_Camera {
    R_MAGIC_FIELD
    R_M4 proj;
    R_M4 view;
} R_Camera;

typedef struct R_FirstPerson {
    R_MAGIC_FIELD
    R_V3  pos;
    float h;
    float v;
} R_FirstPerson;

typedef struct R_ThirdPerson {
    R_MAGIC_FIELD
    R_V3  pos;
    float yaw;
    float pitch;
    float roll;
} R_ThirdPerson;




R_Camera *R_camera_new(void);
R_Camera *R_camera_new_perspective(float fov, float aspect_ratio,
                                   float near, float far);

void R_camera_free(R_Camera *camera);

void R_camera_perspective_set(R_Camera *camera, float fov, float aspect_ratio,
                              float near, float far);

void R_camera_bind(R_Camera *camera, int proj_location, int view_location);


R_FirstPerson *R_first_person_new(R_V3 pos, float h, float v);

void R_first_person_free(R_FirstPerson *fp);

void R_first_person_directions(R_FirstPerson *fp, R_V3 *out_forward,
                               R_V3 *out_right, R_V3 *out_up);

void R_first_person_look(R_FirstPerson *fp, float hd, float vd);
void R_first_person_move(R_FirstPerson *fp, float forward, float right, float up);
void R_first_person_apply(R_FirstPerson *fp, R_Camera *camera);


R_ThirdPerson *R_third_person_new(R_V3 pos, float yaw, float pitch, float roll);

void R_third_person_free(R_ThirdPerson *tp);

void R_third_person_apply(R_ThirdPerson *tp, R_Camera *camera);
