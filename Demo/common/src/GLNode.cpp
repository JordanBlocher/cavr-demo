#include "GLNode.hpp"
#include "GLProgram.hpp"


typedef std::pair<std::string, std::shared_ptr<GLNode>> QGLPair;

GLNode::GLNode()
{
}

GLNode::GLNode(const std::shared_ptr<GLNode> node)
{
    this->id = node->id;
    this->name = node->name;
    this->typeName = node->typeName;
}


GLNode::GLNode(const char* name)
{
    this->name = name;
    this->typeName = "";
}

GLNode::GLNode(const char* name, const char* typeName)
{
    this->name = name;
    this->typeName = typeName;
}

GLNode::GLNode(const char* name, GLenum type)
{
    this->name = name;
    this->typeName = "";
    this->type = type;
}

GLNode::~GLNode()
{
}

GLuint GLNode::getId() const
{
    return this->id;
}

std::string GLNode::getName() const
{
    return this->name;
}

std::string GLNode::getTypeName() const
{
    return this->typeName;
}

bool GLNode::Status()
{
    return false;
}


