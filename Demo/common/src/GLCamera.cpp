#define GLM_FORCE_RADIANS
#include "GLCamera.hpp"

#include <cmath>
#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include "glm/ext.hpp"
#include <math.h>

GLCamera::GLCamera(const char* name)
{
    this->name = name;
    this->projection = glm::mat4(1.0f);
    this->view = glm::mat4(1.0f);
    this->eye_pos = glm::vec3(0.0f,0.0f, 0.0f);

    this->near = glm::vec3(0.0f,0.0f,0.1f);
    this->far = glm::vec3(0.0f,0.0f,10000.0f);

    this->radius  = 15.0f;
    this->azimuth = M_PI;
    this->polar = M_PI;
    this->aimTarget = glm::vec3(0.0f,0.0f,-10.0f);

}

GLCamera::~GLCamera()
{
}

void GLCamera::updateCavrProjection()
{
    cavr::math::mat4f proj = cavr::gfx::getProjection();
    for(int i=0; i<4; i++)
    {
        projection[i].x = proj[i].x; projection[i].y = proj[i].y; 
        projection[i].z = proj[i].z; projection[i].w = proj[i].w; 
    }
}

void GLCamera::updateCavrView()
{
    cavr::math::mat4f vw = cavr::gfx::getView();
    for(int i=0; i<4; i++)
    {
        view[i].x = vw[i].x; view[i].y = vw[i].y; 
        view[i].z = vw[i].z; view[i].w = vw[i].w; 
    }

}

void GLCamera::updateCavrPosition()
{
    cavr::math::vec3f eye = cavr::gfx::getEyePosition();
    eye_pos.x = eye.x; eye_pos.y = eye.y; 
    eye_pos.z = eye.z; 
}

void GLCamera::moveCamera(GLCamera::CamDirection direction)
{
    switch(direction)
    {
        case (CamDirection::Up):
            if(polar > ((2.0f*M_PI)))
                polar = ((M_PI));
            else
                polar += (M_PI/50.0f);
            break;
        case (CamDirection::Down):
            if(polar < ((2.0f*M_PI)))
                polar = ((M_PI));
            else
                polar -= (M_PI/50.0);
            break;
        case (CamDirection::Left):
                azimuth += (M_PI/200.0);
                azimuth -= ((int)(azimuth/(2.0f*M_PI))*(2.0f*M_PI));
            break;
        case (CamDirection::Right):
                azimuth -= M_PI/200.0;
                azimuth -= ((int)(azimuth/(2.0f*M_PI))*(2.0f*M_PI));
            break;
        case (CamDirection::Forward):
            if(radius < 0.2f)
                radius = 0.0f;
            else
                radius -= 0.2f;
            break;
        case (CamDirection::Backward):
            if(radius >= 180.0f)
                radius = 180.0f;
            else
                radius += 0.2f;
            break;
    }
    // Move the camera
    updateView();
}

void GLCamera::updateView()
{
    // Declare function variables
    float eyeX, eyeY, eyeZ;

    // Calculate the eye position
    eyeX = radius * cos (polar) * sin (azimuth);
    eyeY = radius * sin (polar) * sin (azimuth);
    eyeZ = radius * cos (polar);

    this->view = (glm::lookAt(
                    glm::vec3(eyeX, eyeY, -eyeZ),  //eye pos
                    this->aimTarget,    //focus point
                    glm::vec3(0, 1, 0)));  //up

    std::cout<<"polar "<<polar<<" azimuth "<<azimuth<<" radius "<<radius<<'\n';
    std::cout<< "eye pos "<<eyeX<<" "<< eyeY<< " " <<eyeZ <<'\n';
    
}

void GLCamera::setAimTarget(glm::vec3 aimPos)
{
    this->aimTarget = aimPos;
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



