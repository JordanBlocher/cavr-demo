#include "GLCamera.hpp"

#include <cmath>
#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>

GLCamera::GLCamera(const char* name)
{
    this->name = name;
    this->projection = glm::mat4(1.0f);
    this->view = glm::mat4(1.0f);
    this->eye_pos = glm::vec3(0.0f,0.0f, 0.0f);

    this->near = glm::vec3(0.0f,0.0f,0.0f);
    this->far = glm::vec3(0.0f,0.0f,10.0f);

}

GLCamera::~GLCamera()
{
}

void GLCamera::Update()
{
/*
    cavr::math::mat4f proj = cavr::gfx::getProjection();
    for(int i=0; i<4; i++)
    {
        this->projection[i].x = proj[i].x; this->projection[i].y = proj[i].y; 
        this->projection[i].z = proj[i].z; this->projection[i].w = proj[i].w; 
    }
    cavr::math::mat4f view = cavr::gfx::getView();
    for(int i=0; i<4; i++)
    {
        this->view[i].x = view[i].x; this->view[i].y = view[i].y; 
        this->view[i].z = view[i].z; this->view[i].w = view[i].w; 
    }
    cavr::math::vec3f eye = cavr::gfx::getEyePosition();
    this->eye_pos.x = eye.x; this->eye_pos.y = eye.y; 
    this->eye_pos.z = eye.z; 
 */
}

glm::mat4 GLCamera::Projection()
{
    return this->projection;
}

glm::mat4 GLCamera::View()
{
    return this->view;
}

glm::vec3 GLCamera::getCameraPosition()
{
    return this->eye_pos;
}

void GLCamera::setAimTarget(glm::vec3 far)
{
    this->far = far;
}


