#include "GLTexture.hpp"


GLTexture::GLTexture()
{
    // Lets make the default type GL_TEXTURE_2D
    this->type = GL_TEXTURE_2D;
}

GLTexture::GLTexture(const char* name, GLenum type, const char* file) : GLNode(name, type)
{
	this->file = file;
    this->type = type;
    try {
        this->image = new Magick::Image(this->file);
        this->image->write(&this->blob, "RGBA");
        std::cout<<"Texture "<<name<<" loaded.\n";
    }
    catch (Magick::Error& error) {
        std::cout << "Error loading texture '" << this->file << "': " << error.what() << std::endl;
    }
    width = image->columns();
    height = image->rows();
    data = blob.data();
    internalFormat = GL_RGB;
    format = GL_RGBA;

}

GLTexture::GLTexture(const char* name, GLenum type, int width,int height,const void* Buffer,GLenum internalFormat,GLenum format) : GLNode(name, type)
{
    this->type = type;
    this->width = width;
    this->height = height;
    data = Buffer;
    this->internalFormat = internalFormat;
    this->format = format;
}


GLTexture::~GLTexture()
{
}


bool GLTexture::Load()
{
    glGenTextures(1, &(this->id));
    glBindTexture(this->type, this->id);
    glTexImage2D(this->type, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



void GLTexture::SetData(int width,int height, unsigned char* Buffer)
{
    glBindTexture(this->type, this->id);
    glTexImage2D(this->type, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, Buffer);
}

void GLTexture::Bind(GLenum index)
{
    glActiveTexture(index);
    glBindTexture(this->type, this->id);
}





