#ifndef GLMAIN_H
#define GLMAIN_H

//#include <GLViewport.hpp>
//#include <GLCamera.hpp>

#include <chrono>

// Also does not belong here
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 


class GLScene : public GLViewport
{
 public:
    GLScene();

    void idleGL();
    void addModel(const char*, const char*);

    void initializeGL();
    void paintGL();
    float getDT();
    //void moveCamera(GLCamera::CamDirection);

 protected:
    std::string modelpath;
    std::string materialpath;
    void paintHelper(const char*);

    std::chrono::time_point<std::chrono::high_resolution_clock> time;

};

template <typename T> 
int sgn(T val) 
{
    return (T(0) < val) - (val < T(0));
}

#endif 

