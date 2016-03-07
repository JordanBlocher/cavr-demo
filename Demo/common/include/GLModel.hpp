#ifndef GLMODEL_H
#define GLMODEL_H

#include <vector>
#include <cstring>
#include <map>
#include <assimp/scene.h>
#include <climits>
#include <Magick++.h>

#include "GLFrame.hpp"
#include "GLPrimitive.hpp"
#include "GLUniform.hpp"

class GLTexture;

class GLModel : public GLPrimitive
{
 
 public:
    GLModel(const char*, const char*, const GLuint);
    GLModel(const char*, const char*);
    ~GLModel();

    bool LoadVertexData();
    bool LoadModel();

    const std::vector<Vec3>& Positions(size_t);
    const std::vector<GLuint>& Faces(size_t);
    

 protected:
    void AddAttributeData(const aiMesh*, unsigned int);
    void AddVertexData(const aiMesh*, unsigned int);
    void AddMaterials(aiMaterial**, unsigned int);

    bool AddMaterials(const char*);

    std::string path;
    std::string filename;


};

#endif 

