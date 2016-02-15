#ifndef GLCAMERA_H
#define GLCAMERS_H
#define GLM_FORCE_RADIANS

#include "GLNode.hpp"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class GLCamera : public GLNode
{
 
public:
 enum class CamDirection {Up, Down, Left, Right, Forward, Backward};
    GLCamera(const char*);
    ~GLCamera();
    
    void updateCavrProjection();
    void updateCavrView();
    void updateCavrPosition();
    void updateView();
    void moveCamera(CamDirection);
    glm::vec3 getCameraPosition();
    void setAimTarget(glm::vec3 aimPos);
    void setCameraOffset(float zenith, float azimuth);

    glm::mat4 Projection();
    glm::mat4 View();


//protected:

    float radius, azimuth, polar;
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 aimTarget;
    glm::vec3 eye_pos;
    glm::vec3 near;
    glm::vec3 far;

    float zenOffset;
    float aziOffset;

};

#endif 

