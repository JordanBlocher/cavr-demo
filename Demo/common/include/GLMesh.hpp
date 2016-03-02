#ifndef GLMESH_H
#define GLMESH_H

#include <vector>
#include <cstring>
#include <map>
#include <assimp/scene.h>
#include <climits>
#include <Magick++.h>

#include "GLNode.hpp"
#include "GLBufferObject.hpp"

const GLuint V_INDEX = 0;
const GLuint NORM_INDEX = 1;
const GLuint UV_INDEX = 2;
const GLuint COLOR_INDEX = 3;

typedef std::shared_ptr<std::vector<std::vector<Vec3>>> sp2dvec3;
typedef std::shared_ptr<std::vector<std::vector<glm::ivec2>>> sp2dvec2;
typedef std::shared_ptr<std::vector<std::vector<GLuint>>> sp2dvec;
typedef std::shared_ptr<std::vector<Vec3>> spvec3;
typedef std::shared_ptr<std::vector<glm::ivec2>> spvec2;
typedef std::shared_ptr<std::vector<GLuint>> spvec;

class GLMesh : public GLNode
{
 
 public:
    GLMesh(const char*);
    GLMesh(const char*, const char*);
    ~GLMesh();

    const std::vector<Vec3>& Positions(size_t);
    const std::vector<GLuint>& Faces(size_t);

    int AddCircle(int, double, double, double, Vec2 uv0=Vec2(0), Vec2 uv1=Vec2(1));
    void AddTriangleStrip(int, int, int, bool);
    void AddTriangleSurface(int, int, int, bool, bool);
    void AddTriangleFan(int, int, int, bool);

 protected:

    void AddTriangle(Vec3, Vec3, Vec3);
    void AddTriangle(Vec3, Vec3, Vec3, Vec3, int);
    Vec2 InterpolateU(Vec2, Vec2, int, int);
    Vec2 InterpolateV(Vec2, Vec2, int, int);
    Vec2 InterpolateUV(Vec2, Vec2, double, double, double, double);
    virtual void Allocate();

    sp2dvec3 _positions;
    sp2dvec3 _colors;
    sp2dvec3 _normals;
    sp2dvec2 _uvs;
    sp2dvec _faces;

	GLBufferObject vbo_pos;
	GLBufferObject vbo_tex;
	GLBufferObject vbo_norm;
	GLBufferObject vbo_color;
	GLBufferObject vbo_elements;

    spvec3 positions;
    spvec3 colors;
    spvec3 normals;
    spvec2 uvs;
    spvec faces;

    size_t e_size;
    size_t v_size;

    GLuint vao;
    GLuint vbo[5];
    GLuint attributes;

};

#endif 
