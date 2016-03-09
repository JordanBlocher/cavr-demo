#include "GLMath.hpp"

glm::mat4 GLMath::mat4ftoGLM(cavr::math::mat4f mat4f)
{
    glm::mat4 temp = glm::mat4(1.0);
    for(int i=0; i<4; i++)
    {
        temp[i].x = mat4f[i].x; temp[i].y = mat4f[i].y; 
        temp[i].z = mat4f[i].z; temp[i].w = mat4f[i].w; 
    }
    return temp;
}

cavr::math::mat4f GLMath::GLMtomat4f(glm::mat4 mat4)
{
    cavr::math::mat4f temp;
    for(int i=0; i<4; i++)
    {
        temp[i].x = mat4[i].x; temp[i].y = mat4[i].y; 
        temp[i].z = mat4[i].z; temp[i].w = mat4[i].w; 
    }
    return temp;
}

Vec3 GLMath::vec3ftoGLM(cavr::math::vec3f vec3f)
{
    Vec3 temp = Vec3(1.0);
    temp.x = vec3f.x; temp.y = vec3f.y; 
    temp.z = vec3f.z; 
    return temp;
}

cavr::math::vec3f GLMath::GLMtovec3f(Vec4 vec4)
{
    cavr::math::vec3f temp;
    temp.x = vec4.x; temp.y = vec4.y; 
    temp.z = vec4.z; 
    return temp;
}
cavr::math::vec3f GLMath::GLMtovec3f(Vec3 vec3)
{
    cavr::math::vec3f temp;
    temp.x = vec3.x; temp.y = vec3.y; 
    temp.z = vec3.z; 
    return temp;
}

