#ifndef GLCAMERA_H
#define GLCAMERS_H

#include "GLNode.hpp"

class GLCamera : public GLNode
{
 
public:
 enum class CamDirection {Up, Down, Left, Right, Forward, Backward, Nop};
    GLCamera(const char*);
    ~GLCamera();
    
    void updateCavrProjection();
    void updateCavrView();
    void updateCavrPosition();
    void updateView();
    void moveCamera(glm::vec2 angle, float RadiusForce);
    void moveCamera(CamDirection);
    Vec3 getCameraPosition();
    void setAimTarget(Vec3 aimPos);
    void setCameraOffset(float zenith, float azimuth);

    glm::mat4 Projection();
    glm::mat4 View();



//protected:

    float radius, azimuth, polar;
    glm::mat4 projection;
    glm::mat4 view;
    Vec3 aimTarget;
    Vec3 eye_pos;
    Vec3 near;
    Vec3 far;

    float zenOffset;
    float aziOffset;

};

#endif 

