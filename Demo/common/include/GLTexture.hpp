#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "GLNode.hpp"

#include <Magick++.h>

class GLTexture : GLNode
{
 
 public:
    GLTexture();
    GLTexture(const char*, GLenum, const char*);
    ~GLTexture();
    
    bool Load();
    bool Load(int width,int height,unsigned char* Buffer);


    void SetData(int width,int height, unsigned char* Buffer);

    void Bind(GLenum);

protected:
    Magick::Image* image;
    Magick::Blob blob;
    std::string file;
};

#endif 

