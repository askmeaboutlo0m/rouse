#version 100

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

attribute mediump vec3 v_pos;
attribute mediump vec3 v_normal;
attribute mediump vec2 v_uv;

varying vec2 f_uv;
varying vec3 f_eye;

void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(v_pos, 1.0);
    f_uv        = v_uv;
    f_eye       = (u_model * vec4(v_normal, 0.0)).xyz;
}
