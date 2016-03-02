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

class GLPrimitive : public GLMesh
{
 public:
    GLPrimitive(const char*, GLuint, long);
    GLPrimitive(const char*, const char*, GLuint, long);
    ~GLPrimitive();

    virtual bool Create();
    //virtual void LoadUBO(std::shared_ptr<GLUniform>, UBO);
    bool LoadSphere(int, int);
    bool LoadUVSphere(int, int);
    bool LoadCircle(double, double, double, int, bool);
    void LoadPlane(double, double, double, int);
    bool LoadCylinder();
    //bool LoadCube();

    virtual void Draw(GLenum);
    virtual size_t numVertices();
    virtual size_t numFaces();
    virtual size_t Size();

 protected:
    void CreateVBOs();

    long maxPoints;
    
    Material mat;
};

#endif 

