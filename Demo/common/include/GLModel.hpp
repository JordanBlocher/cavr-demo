#ifndef GLMODEL_H
#define GLMODEL_H

#include <vector>
#include <cstring>
#include <map>
#include <assimp/scene.h>
#include <climits>
#include <Magick++.h>

#include "GLNode.hpp"
#include "GLFrame.hpp"
#include "GLMesh.hpp"

class GLUniform;
class GLTexture;

class GLModel : public GLMesh
{
 
 public:
    enum class UBO {COLOR, TEXTURE, BUMP};
    GLModel(const char*, const char*, const GLuint);
    GLModel(const char*, const char*);
    ~GLModel();

    virtual bool Create();
    virtual bool LoadVertexData();

    virtual void Draw(GLenum);
    virtual void DrawToFBO(std::shared_ptr<GLFrame>);

    virtual size_t numVertices();
    virtual size_t numFaces();
    virtual size_t Size();
    virtual void SetColor(glm::vec4);
    virtual glm::vec4 GetColor();

    virtual void LoadUBO(std::shared_ptr<GLUniform>, UBO);
    const std::vector<glm::vec3>& Positions(size_t);
    const std::vector<GLuint>& Faces(size_t);
    glm::mat4 Matrix();
    glm::vec4 Position();
    void setMatrix(glm::mat4);
 

 protected:
    virtual void Allocate();
    virtual void CreateVBOs();
    void AddAttributeData(const aiMesh*, unsigned int);
    void AddVertexData(const aiMesh*, unsigned int);
    void AddMaterials(aiMaterial**, unsigned int);

    bool AddMaterials(const char*);

    std::shared_ptr<std::vector<std::pair<aiString, Material>>> materials;
    std::shared_ptr<std::vector<std::pair<bool,GLTexture>>> textures;
    std::shared_ptr<std::vector<std::pair<bool,GLTexture>>> bumpmaps;
    std::vector<GLuint> mtlIndices;

    glm::mat4 matrix;

    std::string path;
    std::string filename;
};

#endif 

