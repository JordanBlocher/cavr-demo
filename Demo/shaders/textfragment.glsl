#version 420

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

in vec3 f_position;
in vec3 f_normal;
in vec2 f_uv;
in vec3 f_color;

layout(std140, binding=2) uniform GColors
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float intensity;
    float diffuseBlend;
}colors;


struct BaseLight
{
    vec4 color;
    float ambientIntensity;
    float diffuseIntensity;
};
    
struct DirectionalLight 
{
    vec4 direction;
    BaseLight base;
};

struct PointLight 
{
    vec4 position;
    BaseLight base;
};

struct SpotLight 
{
    vec4 direction;
    PointLight point;
};

layout(std140, binding=3) uniform GLights
{
    DirectionalLight basic;
    PointLight point[1];
    SpotLight spot[6];

}light;

layout(std140, binding=4) uniform Eye
{
    vec4 position;
}eye;

layout(std140, binding=5) uniform Shader
{
    bool texture;
    bool bump;
    bool material;
}shader;

uniform sampler2D diffuseTexture;
uniform sampler2D bumpMap;

out vec4 f_out;

void main(void)
{

    vec4 tex = texture2D(diffuseTexture, f_uv.xy);

    f_out = tex;
}

 
