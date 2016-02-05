#ifndef GLCAMERA_H
#define GLCAMERS_H

#include "GLNode.hpp"

#include <glm/glm.hpp>

class GLCamera : public GLNode
{
 
public:
    GLCamera(const char*);
    ~GLCamera();
    
    void Update();
    glm::vec3 getCameraPosition();
    void setAimTarget(glm::vec3 far);

    glm::mat4 Projection();
    glm::mat4 View();


//protected:

    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 eye_pos;
    glm::vec3 near;
    glm::vec3 far;

};

#endif 

