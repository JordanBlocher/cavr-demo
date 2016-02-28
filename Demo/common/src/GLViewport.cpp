#include "GLViewport.hpp"
#include "GLProgram.hpp"

#include <fstream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

typedef std::pair<std::string, std::shared_ptr<GLNode>> GLPair;

GLViewport::GLViewport()
{
    if(context == NULL)
        this->context = dataPtr(new sceneData);
    else this->context = context;
}

GLViewport::~GLViewport()
{
}


void GLViewport::initializeGL()
{
     
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW not initialized: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return;
    }
}

void GLViewport::paintGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
     
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void GLViewport::idleGL()
{
}

void GLViewport::quit()
{
}

void GLViewport::Event()
{
}

bool GLViewport::AddProgram(std::shared_ptr<GLProgram> program)
{
    glLinkProgram(program->getId());
    this->start_time = std::chrono::high_resolution_clock::now();
    GLuint status = program->Status();

    return status;
}

bool GLViewport::AddToContext(const std::shared_ptr<GLNode> node)
{
    if( this->context->count(node->getName()) == 0 )
    {
        this->context->insert(GLPair(node->getName(), node));
        return true;
    }
    return false;
}

bool GLViewport::RemoveFromContext(const char* name)
{
    if( this->context->count(name) != 0 )
    {
        this->context->erase(name);
        return true;
    }
    return false;
}

bool GLViewport::Contains(const char* name)
{
    if( this->context->count(name) != 0 )
        return true;
    return false;
}

void GLViewport::SetScene(const std::shared_ptr<GLNode> node)
{
    this->glScene = node;
}

void GLViewport::ViewContext()
{
    std::cout<<"------------ Current Context -------------\n";
    std::map<std::string, std::shared_ptr<GLNode>>::const_iterator mi;
    for (mi = this->context->begin(); mi != this->context->end(); ++mi)
        std::cout<<mi->first<<std::endl;
}

dataPtr GLViewport::getContext()
{
    return this->context;
}

void GLViewport::pause()
{
}



