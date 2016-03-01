#ifndef GLMODEL_H
#define GLMODEL_H
#define GLM_FORCE_RADIANS

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <map>
#include <assimp/scene.h>
#include <climits>
#include <Magick++.h>

#include "GLNode.hpp"
#include "GLFrame.hpp"

const GLuint V_INDEX = 0;
const GLuint NORM_INDEX = 1;
const GLuint UV_INDEX = 2;
const GLuint COLOR_INDEX = 3;

class GLUniform;
class GLTexture;

class GLModel : public GLNode
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

    std::string path;
    std::string filename;

    std::shared_ptr<std::vector<std::vector<glm::vec3>>> _positions;
    std::shared_ptr<std::vector<std::vector<glm::vec3>>> _colors;
    std::shared_ptr<std::vector<std::vector<glm::vec3>>> _normals;
    std::shared_ptr<std::vector<std::vector<glm::vec2>>> _uvs;
    std::shared_ptr<std::vector<std::vector<GLuint>>> _faces;
    std::shared_ptr<std::vector<std::pair<aiString, Material>>> materials;
    std::shared_ptr<std::vector<std::pair<bool,GLTexture>>> textures;
    std::shared_ptr<std::vector<std::pair<bool,GLTexture>>> bumpmaps;
    std::vector<GLuint> mtlIndices;

    size_t e_size;
    size_t v_size;

    glm::mat4 matrix;
    GLuint vao;
    GLuint vbo[5];
    GLuint attributes;

};

#endif 

