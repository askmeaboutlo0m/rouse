#version 100

precision mediump float;

const vec3  u_light    = vec3(10.0, 10.0, -10.0);
const vec3  u_ambient  = vec3( 1.0,  1.0,   1.0);
const vec3  u_diffuse  = vec3( 0.6,  0.6,   0.6);
const vec3  u_specular = vec3( 0.1,  0.1,   0.1);
const float u_shine    = 2.0;

varying vec3 f_eye;

void main()
{
    vec3 c = vec3(0.5, 0.1, 0.2);

    vec3 n = normalize(f_eye);
    vec3 l = normalize(u_light);
    vec3 e = vec3(0.0, 0.0, 1.0);
    vec3 h = normalize(l + e);

    float df = max(0.0, dot(n, l));
    float sf = pow(max(0.0, dot(n, h)), u_shine);

    vec3 color = u_ambient * c + df * u_diffuse * c + sf * u_specular;
    gl_FragColor = vec4(color, 1.0);
}
