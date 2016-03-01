#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#ifndef GL_MATH
#define GL_MATH

#include <cmath>

#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/virtrev/xstream.hpp>

typedef glm::vec4 Vec4;
typedef glm::vec3 Vec3;
typedef glm::ivec2 Vec2;


namespace GLMath
{

    glm::mat4 mat4ftoGLM(cavr::math::mat4f);
    cavr::math::mat4f GLMtomat4f(glm::mat4);
    glm::vec3 vec3ftoGLM(cavr::math::vec3f);
    cavr::math::vec3f GLMtovec3f(glm::vec4);
 
}

#endif
