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
    void Bind(GLenum);

    Material material;
    Vec3 color;

protected:
    Magick::Image* image;
    Magick::Blob blob;
    std::string file;

    Magick::Image* bumpImage;
    Magick::Blob bumpBlob;
    std::string bumpFile;
};

#endif 

