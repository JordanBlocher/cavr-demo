#ifndef GLPrimitive_H
#define GLPrimitive_H

#include <vector>
#include <cstring>
#include <map>
#include <assimp/scene.h>
#include <climits>
#include <Magick++.h>

#include "GLBufferObject.hpp"
#include "GLModel.hpp"
#include "GLUniform.hpp"
#include "GLTexture.hpp"


class GLPrimitive : public GLMesh
{
 public:
    GLPrimitive(const char*, GLuint, long);
    GLPrimitive(const char*, const char*, GLuint, long);
    ~GLPrimitive();
    
    virtual void LoadUBO(std::shared_ptr<GLUniform>, UBO);
    virtual void DrawElements(size_t, GLint, GLint, GLenum);
    bool AddSphere(int, int);
    bool AddUVSphere(int, int);
    bool AddCircle(double, double, double, int, bool);
    void AddPlane(double, double, double, int);
    bool AddCylinder();
    void AddMesh();
    //bool AddCube();
    //
    void AddTexture(std::shared_ptr<GLTexture>);
    void AddMaterial(Material mat);
    void SetColor(Vec3);
    Vec3 GetColor();

    virtual void Draw(GLenum);

    glm::mat4 Matrix();
    Vec4 Position();
    void setMatrix(glm::mat4);

    std::shared_ptr<GLUniform> materialUBO;
    std::shared_ptr<GLUniform> textureUBO;
    std::shared_ptr<GLUniform> bumpUBO;
    Shader shader;

 protected:
    virtual void Allocate();

    long maxPoints;
    glm::mat4 matrix;

    std::shared_ptr<std::vector<GLTexture>> textures;
    std::shared_ptr<std::vector<Material>> materials;
    std::shared_ptr<std::vector<Shader>> shaders;

};

#endif 

