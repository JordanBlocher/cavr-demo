#ifndef GLFRAME_H
#define GLFRAME_H

#include <memory>
#include <vector>

#include "GLNode.hpp"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))


class GLFrame : public GLNode
{

 public:
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };

    GLFrame(const char*);
    GLFrame(const char*, int, int);
    ~GLFrame();

    bool Status(GLenum, GLint);
    GLuint Buffer() const;

    void BindForWriting();
    void BindForReading();
 protected:
    GLuint buffer;
    GLuint textures[GBUFFER_NUM_TEXTURES];
    GLuint depthTexture;
};


#endif 

