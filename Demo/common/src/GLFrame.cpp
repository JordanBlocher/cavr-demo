#include "GLFrame.hpp"

#include <algorithm>
#include <climits>
//#include <glm/virtrev/xstream.hpp>

GLFrame::GLFrame(const char* name) : GLNode(name)
{
}

GLFrame::GLFrame(const char* name, int width, int height) : GLNode(name)
{
    glGenFramebuffers(1, &this->buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->buffer);
  
    // Create the gbuffer textures
    glGenTextures(ARRAY_SIZE_IN_ELEMENTS(this->textures), this->textures);
    glGenTextures(1, &this->depthTexture);

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(this->textures); i++) 
    {
        glBindTexture(GL_TEXTURE_2D, this->textures[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->textures[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, this->depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture, 0);

    GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(ARRAY_SIZE_IN_ELEMENTS(drawbuffers), drawbuffers);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

GLFrame::~GLFrame()
{
}

bool GLFrame::Status(GLenum type, GLint size)
{
    glGetFramebufferParameteriv(type, GL_BUFFER_SIZE, &status);
    
    return (this->status == size);
}

GLuint GLFrame::Buffer() const
{
    return this->buffer;
}


void GLFrame::BindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer);
}

void GLFrame::BindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer);
}


