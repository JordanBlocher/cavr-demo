#ifndef GLPrimitive_H
#define GLPrimitive_H

#include <vector>
#include <cstring>
#include <map>
#include <assimp/scene.h>
#include <climits>
#include <Magick++.h>

#include "GLBufferObject.hpp"
#include "GLMesh.hpp"
#include "GLUniform.hpp"
#include "GLTexture.hpp"


class GLPrimitive : public GLMesh
{
 public:
    GLPrimitive(const char*, GLuint, long);
    GLPrimitive(const char*, const char*, GLuint, long);
    ~GLPrimitive();
    
    void Create(GLenum);
    bool AddSphere(int, int);
    bool AddUVSphere(int, int);
    bool AddCircle(double, double, double, int, bool);
    void AddPlane(double, double, double, int);
    bool AddCylinder();
    void AddMesh();
    //bool AddCube();
    
    void AddTexture(std::shared_ptr<GLTexture>);
    void AddMaterial(Material mat);
    void AssignTexture(int, int);
    void AssignMaterial(int, int);
    virtual void Draw(GLenum);

    glm::mat4 Matrix();
    Vec4 Position();
    void setMatrix(glm::mat4);
    virtual void SetColor(Vec3);
    Vec3 GetColor();

    GLint materialUBO;
    GLint textureUBO;
    GLint bumpUBO;
    GLint controlUBO;
    std::shared_ptr<Shader> shader;

 protected:

    virtual void Allocate();
    virtual void LoadUBO(GLuint, UBO, Shader);
    virtual void DrawElements(size_t, GLint, GLint, GLenum);

    long maxPoints;
    glm::mat4 matrix;

    std::shared_ptr<std::vector<GLTexture>> textures;
    std::shared_ptr<std::vector<GLTexture>> bumpmaps;
    std::shared_ptr<std::vector<Material>> materials;
    std::shared_ptr<std::vector<Shader>> shaders;

};

#endif 

