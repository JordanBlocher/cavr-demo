#ifndef GLPrimitive_H
#define GLPrimitive_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <map>
#include <assimp/scene.h>
#include <climits>
#include <Magick++.h>

#include "GLNode.hpp"

const GLuint V_INDEX = 0;
const GLuint NORM_INDEX = 1;
const GLuint UV_INDEX = 2;

class GLUniform;
class GLTexture;

class GLPrimitive : public GLNode
{
 public:
    GLPrimitive(const char*, const GLuint);
    ~GLPrimitive();

    bool CreateVAO();
    glm::mat4 Matrix();
    glm::vec4 Position();
    void setMatrix(glm::mat4);
    bool LoadSphere(int num_lats, int num_lons);
    bool LoadCylinder();
    //bool LoadCube();

    void Draw(std::shared_ptr<GLUniform>, GLuint);
    size_t numVertices();
    size_t numFaces();
    size_t Size();

 private:
    void Allocate();
    void CreateVBOs();

    bool AddMaterials(const char*);
    std::string toString();

    std::string path;
    std::string filename;

    std::shared_ptr<std::vector<glm::vec3>> positions;
    std::shared_ptr<std::vector<glm::vec3>> normals;
    std::shared_ptr<std::vector<glm::vec2>> uvs;
    std::shared_ptr<std::vector<GLuint>> faces;
    //std::shared_ptr<std::vector<std::pair<aiString, Material>>> materials;
    std::shared_ptr<std::vector<std::pair<bool,GLTexture>>> textures;
    std::vector<GLuint> mtlIndices;

    size_t e_size;
    size_t v_size;

    glm::mat4 matrix;
    GLuint vao;
    GLuint vbo[4];
    GLuint attributes;

};

#endif 

