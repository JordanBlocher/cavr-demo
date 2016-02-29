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

class GLUniform;
class GLTexture;

class GLModel : public GLNode
{
 enum MODEL {CUBE};
 
 public:
    GLModel(MODEL, const char*, const GLuint);
    GLModel(const char*, const char*, const GLuint);
    ~GLModel();

    bool CreateVAO();
    glm::mat4 Matrix();
    glm::vec4 Position();
    void setMatrix(glm::mat4);
    bool LoadVertexData();

    void Draw(std::shared_ptr<GLUniform>, GLuint, GLenum);
    void DrawToFBO(std::shared_ptr<GLFrame>, GLuint);
    const std::vector<glm::vec3>& Positions(size_t);
    const std::vector<GLuint>& Faces(size_t);
    size_t numVertices();
    size_t numFaces();
    size_t Size();
    void setColor(glm::vec4);
    glm::vec4 getColor();

 private:
    void Allocate();
    void AddAttributeData(const aiMesh*, unsigned int);
    void AddVertexData(const aiMesh*, unsigned int);
    void AddMaterials(aiMaterial**, unsigned int);
    void CreateVBOs();

    bool AddMaterials(const char*);
    std::string toString(MODEL);

    std::string path;
    std::string filename;
    MODEL type;

    std::shared_ptr<std::vector<std::vector<glm::vec3>>> positions;
    std::shared_ptr<std::vector<std::vector<glm::vec3>>> normals;
    std::shared_ptr<std::vector<std::vector<glm::vec2>>> uvs;
    std::shared_ptr<std::vector<std::vector<GLuint>>> faces;
    std::shared_ptr<std::vector<std::pair<aiString, Material>>> materials;
    std::shared_ptr<std::vector<std::pair<bool,GLTexture>>> textures;
    std::shared_ptr<std::vector<std::pair<bool,GLTexture>>> bumpmaps;
    std::vector<GLuint> mtlIndices;

    size_t e_size;
    size_t v_size;

    glm::mat4 matrix;
    GLuint vao;
    GLuint vbo[4];
    GLuint attributes;

};

#endif 

