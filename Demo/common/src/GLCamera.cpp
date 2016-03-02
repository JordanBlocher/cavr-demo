#include "GLCamera.hpp"

#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>

GLCamera::GLCamera(const char* name)
{
    this->name = name;
    this->projection = glm::mat4(1.0f);
    this->view = glm::mat4(1.0f);
    this->eye_pos = Vec3(0.0f,0.0f, 0.0f);

    this->near = Vec3(0.0f,0.0f,0.1f);
    this->far = Vec3(0.0f,0.0f,10000.0f);

    this->radius  = 9.0f;
    this->azimuth = M_PI/2.0;
    this->polar = M_PI/2.0;
    this->aimTarget = Vec3(0.0f,0.0f,-10.0f);
}

GLCamera::~GLCamera()
{
}

void GLCamera::updateCavrProjection()
{
    projection = GLMath::mat4ftoGLM(cavr::gfx::getProjection());
}

void GLCamera::updateCavrView()
{
    view = GLMath::mat4ftoGLM(cavr::gfx::getView());
}

void GLCamera::updateCavrPosition()
{
    eye_pos = GLMath::vec3ftoGLM(cavr::gfx::getEyePosition());
}

void GLCamera::moveCamera(GLCamera::CamDirection direction)
{
    switch(direction)
    {
        case (CamDirection::Up):
            if(polar >= M_PI)
                polar = -1.0*M_PI;
            else
                polar += M_PI/150.0f;
            break;
        case (CamDirection::Down):
            if(polar <= -1.0*M_PI)
                polar = M_PI;
            else
                polar -= M_PI/150.0f;
            break;
        case (CamDirection::Left):
            if(azimuth <= -1.0*M_PI)
                azimuth = M_PI;
            else
                azimuth += M_PI/150.0f;
            break;
        case (CamDirection::Right):
            if(azimuth <= -1.0*M_PI)
                azimuth = M_PI;
            else
                azimuth -= M_PI/150.0f;
            break;
        case (CamDirection::Forward):
            if(radius <= -30.0f)
                radius = -30.0f;
            else
                radius -= 0.2f;
            break;
        case (CamDirection::Backward):
            if(radius >= 30.0f)
                radius = 30.0f;
            else
                radius += 0.2f;
            break;
    }
    // Move the camera
    updateView();
}

void GLCamera::updateView()
{
    float eyeX, eyeY, eyeZ;

    eyeX = radius * sin (polar) * cos (azimuth);
    eyeZ = -radius * sin (polar) * sin (azimuth);
    eyeY = radius * cos (polar);
    
    glm::mat4 view = (GLMath::mat4ftoGLM(cavr::gfx::getView()));
    Vec3 forward = -Vec3(view[2].x,view[2].y,view[2].z);
    Vec3 up = Vec3(view[1].x,view[1].y,view[1].z);
    //Vec3 forward = GLMath::vec3ftoGLM(cavr::input::getSixDOF("glass")->getForward());
    //Vec3 up = GLMath::vec3ftoGLM(cavr::input::getSixDOF("glass")->getUp());
    setAimTarget(forward + Vec3(0,0,1000));

    this->view = (glm::lookAt(
                    Vec3(eyeX, eyeY, eyeZ),  //eye pos
                    this->aimTarget,    //focus point
                    up));  //up

    //std::cout<<"polar "<<polar<<" azimuth "<<azimuth<<" radius "<<radius<<'\n';
    //std::cout<< "eye pos "<<eyeX<<" "<< eyeY<< " " <<eyeZ <<'\n';
    this->view = view*this->view; 
}

void GLCamera::setCameraOffset(float zenith, float azimuth)
{
    this->zenOffset = zenith;
    this->aziOffset = azimuth;
    this->updateView();
}

void GLCamera::setAimTarget(Vec3 aimPos)
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

Vec3 GLCamera::getCameraPosition()
{
    return this->eye_pos;
}



