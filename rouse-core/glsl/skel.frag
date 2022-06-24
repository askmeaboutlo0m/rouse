#version 100

precision mediump float;

const vec3  LIGHT    = vec3(10.0, 10.0, 10.0);
const vec3  AMBIENT  = vec3( 1.0,  1.0,  1.0);
const vec3  DIFFUSE  = vec3( 0.6,  0.6,  0.6);
const vec3  SPECULAR = vec3( 0.1,  0.1,  0.1);
const float SHINE    = 2.0;

uniform sampler2D u_sampler;

varying vec2 f_uv;
varying vec3 f_eye;

void main()
{
    vec3 c = texture2D(u_sampler, f_uv).rgb;

    vec3 n = normalize(f_eye);
    vec3 l = normalize(LIGHT);
    vec3 e = vec3(0.0, 0.0, 1.0);
    vec3 h = normalize(l + e);

    float df = max(0.0, dot(n, l));
    float sf = pow(max(0.0, dot(n, h)), SHINE);

    vec3 color = AMBIENT * c + df * DIFFUSE * c + sf * SPECULAR;
    gl_FragColor = vec4(color, 1.0);
}
