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
}

GLTexture::~GLTexture()
{
}

bool GLTexture::Load()
{
    try {
        this->image = new Magick::Image(this->file);
        this->image->write(&this->blob, "RGBA");
    }
    catch (Magick::Error& error) {
        std::cout << "Error loading texture '" << this->file << "': " << error.what() << std::endl;
        return false;
    }

    glGenTextures(1, &(this->id));
    glBindTexture(this->type, this->id);
    glTexImage2D(this->type, 0, GL_RGB, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, this->blob.data());
    glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return true;
}

bool GLTexture::Load(int width,int height,unsigned char* Buffer)
{
    
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &(this->id));
    glBindTexture(this->type, this->id);
    glTexImage2D(this->type, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, Buffer);
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





