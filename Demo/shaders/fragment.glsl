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

layout(std140, binding=5) uniform Control
{
    int texture;
    int bump;
}control;

uniform sampler2D diffuseTexture;
uniform sampler2D bumpMap;

vec4 LightBasic(BaseLight source, vec4 direction, vec3 normal)
{
    vec4 ambient = vec4(0, 0, 0, 0); 
    vec4 diffuse = vec4(0, 0, 0, 0);
    vec4 specular = vec4(0, 0, 0, 0);

    if (control.texture == 0)
    {
        vec4 tex = texture2D(diffuseTexture, f_uv.xy);
        ambient = source.color * mix(colors.ambient, tex, 0.5) * source.ambientIntensity; 
        float diffuseFactor = dot(normal, -direction.xyz);
        if (diffuseFactor > 0) 
        {
            diffuse = source.color * mix(colors.diffuse, tex, colors.diffuseBlend/1.0) * source.diffuseIntensity * diffuseFactor;
            vec3 v_toEye = normalize(eye.position.xyz - f_position);
            vec3 l_reflect = normalize(reflect(direction.xyz, normal));
            float specularFactor = dot(v_toEye, l_reflect);
            specularFactor = pow(specularFactor, colors.shininess);
            if(specularFactor > 0)
            {
                specular = source.color * mix(colors.specular, tex, 0.5) * colors.intensity * specularFactor;
            }
        }
    }
    else
    {
        ambient = source.color * colors.ambient * source.ambientIntensity; 
        float diffuseFactor = dot(normal, -direction.xyz);
        if (diffuseFactor > 0) 
        {
            diffuse = source.color * colors.diffuse * source.diffuseIntensity * diffuseFactor;
            vec3 v_toEye = normalize(eye.position.xyz - f_position);
            vec3 l_reflect = normalize(reflect(direction.xyz, normal));
            float specularFactor = dot(v_toEye, l_reflect);
            specularFactor = pow(specularFactor, colors.shininess);
            if(specularFactor > 0)
            {
                specular = source.color * colors.specular * colors.intensity * specularFactor;
            }
        }
    }

    return (ambient + diffuse + specular + vec4(f_color, 0));
}

vec4 LightDir(vec3 normal)
{
    return LightBasic(light.basic.base, light.basic.direction, normal);
}

vec4 LightPt(PointLight pt, vec3 normal)
{
    vec3 dir = f_position - pt.position.xyz;
    float l = length(dir);
    dir = normalize(dir);

    vec4 color = LightBasic(pt.base, vec4(dir, 1.0), normal);

    return color;
}

vec4 LightSpt(SpotLight sp, vec3 normal)
{
    vec3 l_toPix = normalize(f_position - sp.point.position.xyz);
    float spotFactor = dot(l_toPix, sp.direction.xyz);
    if( spotFactor > 2)
    {
        vec4 color = LightPt(sp.point, normal) * spotFactor;
        return vec4(color.xyz, 1.0);
    }
    else
    {
        return vec4(0, 0, 0, 0);
    }
}

out vec4 f_out;

void main(void)
{
    vec3 normal = normalize(f_normal);
    vec4 totalLight = vec4(0, 0, 0, 0);
    vec4 ambient = vec4(0, 0, 0, 0); 

    if(control.texture == 1)
    {
        vec4 tex = texture2D(diffuseTexture, f_uv.xy);
        ambient = light.basic.base.color * mix(colors.ambient, tex, 0.5) * light.basic.base.ambientIntensity; 
    }
    else
    {
        float blend = colors.diffuseBlend;
        ambient = light.basic.base.color * colors.ambient * light.basic.base.ambientIntensity * ((blend + 1.0) / (blend + 1.00000000001));  
    }

    totalLight += ambient; 
    totalLight += LightDir(normal);
    totalLight += LightPt(light.point[0], normal);
    totalLight += LightSpt(light.spot[0], normal);
    totalLight += LightSpt(light.spot[1], normal);
    totalLight += LightSpt(light.spot[2], normal);
    totalLight += LightSpt(light.spot[3], normal);
    totalLight += LightSpt(light.spot[4], normal);
    totalLight += LightSpt(light.spot[5], normal);

    f_out = vec4(1,0,0,1) + totalLight;

}

 
