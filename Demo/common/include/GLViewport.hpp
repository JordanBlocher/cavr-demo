#ifndef GLVIEW_H
#define GLVIEW_H
#define GLM_FORCE_RADIANS

#include "GLNode.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <chrono>

// for std::cerr
#include <iostream>
#include <map>
#include <string>

class GLProgram;
//class GLCamera;
struct Dynamics;
struct Player;

typedef std::map<std::string, std::shared_ptr<GLNode>> sceneData;
typedef std::shared_ptr<sceneData> dataPtr;

class GLViewport 
{

 public:
    GLViewport();
    ~GLViewport();

    dataPtr getContext();

    template<class T>
    std::shared_ptr<T> Get(const char*) const;

    void quit();
    virtual void pause();
    virtual void idleGL();
    void ViewContext();

 protected: 
    virtual void initializeGL();
    virtual void paintGL();
    virtual void Event();

    bool AddProgram(std::shared_ptr<GLProgram>);
    bool AddToContext(const std::shared_ptr<GLNode>);
    bool RemoveFromContext(const char*);
    void SetScene(const std::shared_ptr<GLNode>);
    bool Contains(const char*);

    
    std::shared_ptr<GLNode> glScene;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    glm::mat4 mvp;

    // Data Structure for Nodes
    dataPtr context;
    // Physics Engine
    std::shared_ptr<Dynamics> dynamics;
    

};

template<class T>
std::shared_ptr<T> GLViewport::Get(const char* name) const 
{
    if( this->context->count(name) == 1 )
        return std::static_pointer_cast<T>(this->context->find(name)->second);
    else 
        std::cerr << "Node " <<name <<" was not found"<< std::endl;
    return NULL;
}


#endif 

