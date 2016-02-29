#ifndef GL_MATH
#define GL_MATH

#define GLM_FORCE_RADIANS
#include <cmath>
#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include "glm/ext.hpp"
#include <math.h>

namespace GLMath
{

    glm::mat4 mat4ftoGLM(cavr::math::mat4f);
    cavr::math::mat4f GLMtomat4f(glm::mat4);
    glm::vec3 vec3ftoGLM(cavr::math::vec3f);
    cavr::math::vec3f GLMtovec3f(glm::vec4);
 
}

#endif
