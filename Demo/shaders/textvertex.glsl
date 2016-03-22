#version 420
layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 v_uv;
layout(location=3) in vec3 v_color;

layout(std140, binding=1) uniform GMatrices
{
    mat4 mvpMatrix;
    mat4 mvMatrix;
    mat4 normalMatrix;
};

layout(std140, binding=2) uniform GColors
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float intensity;
    float diffuseBlend;
}colors;

out vec3 f_position;
out vec3 f_normal;
out vec2 f_uv;
out vec3 f_color;

void main(void)
{
    f_uv = v_uv;
    f_color = v_color;
    vec4 p = (mvMatrix * vec4(v_position,1.0));
    f_position = p.xyz/p.w;
    vec4 n = normalMatrix * vec4(v_normal,1.0);
    f_normal = normalize(n.xyz/n.w);
    gl_Position = mvpMatrix * vec4(v_position, 1.0);
}
