#version 100

const int NBONES   = 20;
const int NWEIGHTS = 3;

uniform mat4 u_pv;
uniform mat4 u_bones[NBONES];

attribute mediump vec3 v_pos;
attribute mediump vec3 v_normal;
attribute mediump vec2 v_uv;
attribute mediump vec3 v_bones;
attribute mediump vec3 v_weights;

varying vec2 f_uv;
varying vec3 f_eye;

void main()
{
    vec4 pos    = vec4(0.0);
    vec4 normal = vec4(0.0);
    for (int i = 0; i < NWEIGHTS; ++i) {
        mat4  bone   = u_bones[int(v_bones[i])];
        float weight = v_weights[i];
        pos         += bone * vec4(v_pos,    1.0) * weight;
        normal      += bone * vec4(v_normal, 0.0) * weight;
    }

    gl_Position = u_pv * pos;
    f_uv        = v_uv;
    f_eye       = normal.xyz;
}
