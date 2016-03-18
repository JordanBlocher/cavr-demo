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
    glm::mat4 GetCameraView() {return testview;};
    void moveCamera(glm::vec2 angle, float RadiusForce);
    void moveCamera(CamDirection);
    Vec3 getCameraPosition();
    void setAimTarget(Vec3 aimPos);
    void setCameraOffset(float zenith, float azimuth);
    Vec3 RotatePoint(Vec3 Ray); // for rotating the wand in the direction that the camera is looking
    Vec3 GetForward();

    glm::mat4 Projection();
    glm::mat4 View();



//protected:

    float radius, azimuth, polar;
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 testview;
    Vec3 aimTarget;
    Vec3 eye_pos;
    Vec3 near;
    Vec3 far;

    float zenOffset;
    float aziOffset;

};

#endif 

