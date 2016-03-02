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

class GLPrimitive : public GLMesh
{
 public:
    GLPrimitive(const char*, GLuint, long);
    GLPrimitive(const char*, const char*, GLuint, long);
    ~GLPrimitive();

    virtual bool Create();
    virtual void LoadUBO(std::shared_ptr<GLUniform>, UBO);
    bool AddSphere(int, int);
    bool AddUVSphere(int, int);
    bool AddCircle(double, double, double, int, bool);
    void AddPlane(double, double, double, int);
    bool AddCylinder();
    //bool AddCube();

    virtual void Draw(GLenum);
    virtual size_t numVertices();
    virtual size_t numFaces();
    virtual size_t Size();

    glm::mat4 Matrix();
    Vec4 Position();
    void setMatrix(glm::mat4);
 
 protected:
    virtual void CreateVBOs();
    virtual void LoadVAO();

    long maxPoints;

    glm::mat4 matrix;

    Material mat;
};

#endif 

