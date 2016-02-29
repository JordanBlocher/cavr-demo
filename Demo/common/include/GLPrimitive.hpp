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

#include "GLBufferObject.hpp"
#include "GLModel.hpp"

class GLPrimitive : public GLModel
{
 public:
    GLPrimitive(const char*, GLuint, long);
    GLPrimitive(const char*, const char*, GLuint, long);
    ~GLPrimitive();

    virtual bool Create();
    virtual void LoadUBO(std::shared_ptr<GLUniform>, UBO);
    bool LoadSphere(int num_lats, int num_lons);
    bool LoadCylinder();
    //bool LoadCube();

    virtual void Draw(GLenum);
    virtual size_t numVertices();
    virtual size_t numFaces();
    virtual size_t Size();

 protected:
    void Allocate();
    void CreateVBOs();

	GLBufferObject vbo_pos;
	GLBufferObject vbo_tex;
	GLBufferObject vbo_norm;
	GLBufferObject vbo_color;

    std::shared_ptr<std::vector<glm::vec3>> positions;
    std::shared_ptr<std::vector<glm::vec3>> normals;
    std::shared_ptr<std::vector<glm::vec2>> uvs;
    std::shared_ptr<std::vector<GLuint>> faces;

    long maxPoints;
    
    Material mat;
};

#endif 

