#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "GLNode.hpp"

#include <Magick++.h>

class GLTexture : GLNode
{
 
 public:
    GLTexture(const char*, GLenum,int width,int height,const void* Buffer,GLenum internalFormat=GL_RGB,GLenum format=GL_RGB);
    GLTexture();
    GLTexture(const char*, GLenum, const char*);
    ~GLTexture();
    
    bool Load();

    void SetData(int width,int height, const unsigned char* Buffer);

    void Bind(GLenum);

protected:
    Magick::Image* image;
    Magick::Blob blob;
    int width;
    int height;
    const void* data;
    std::string file;
    GLenum format;
    GLenum internalFormat;
};

#endif 

