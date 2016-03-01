#define CHECKGLERROR if ( glGetError() != GL_NO_ERROR ) std::cout << __FILE__ <<":"<< __LINE__ << std::endl;
#include "GLPrimitive.hpp"
#include "GLBufferObject.hpp"
#include "GLUniform.hpp"
#include "GLTexture.hpp"

#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/bind.hpp>
#include <csignal>
#include <sstream>
#include <assert.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


GLPrimitive::GLPrimitive(const char* name, GLuint attributes, long maxPoints) : GLModel(name, "GLPrimitive")
{
    this->maxPoints = maxPoints;
    this->attributes = attributes;
    this->Allocate();
}

GLPrimitive::GLPrimitive(const char* name, const char* typeName, GLuint attributes, long maxPoints) : GLModel(name, typeName)
{
    this->maxPoints = maxPoints;
    this->attributes = attributes;
    this->Allocate();
}

GLPrimitive::~GLPrimitive()
{
}

void GLPrimitive::Allocate()
{
    this->positions = std::shared_ptr<std::vector<glm::vec3>>(new std::vector<glm::vec3>);
    this->normals = std::shared_ptr<std::vector<glm::vec3>>(new std::vector<glm::vec3>);
    this->uvs = std::shared_ptr<std::vector<glm::vec2>>(new std::vector<glm::vec2>);
    this->faces = std::shared_ptr<std::vector<GLuint>>(new std::vector<GLuint>);
}

bool GLPrimitive::LoadSphere(int num_lats, int num_lons)
{
    // A Thanks to Roger Hoang
    int face_ind = 0;

    float rads_per_lat = M_PI / float(num_lats);
    float rads_per_lon = 2.0 * M_PI / float(num_lons);
    for (int lat = 0; lat < num_lats; ++lat) 
    {
        float top_rad = rads_per_lat * lat - M_PI / 2.0f;
        float bottom_rad = top_rad + rads_per_lat;
        float top_y = sin(top_rad);
        float top_xz = cos(top_rad);
        float bottom_y = sin(bottom_rad);
        float bottom_xz = cos(bottom_rad);
        for (int lon = 0; lon < num_lons; ++lon) {
            float left_rad = rads_per_lon * lon;
            float right_rad = left_rad + rads_per_lon;
            float sin_l = sin(left_rad);
            float cos_l = cos(left_rad);
            float sin_r = sin(right_rad);
            float cos_r = cos(right_rad);
            glm::vec3 ll(bottom_xz * cos_l, bottom_y, bottom_xz * sin_l);
            glm::vec3 lr(bottom_xz * cos_r, bottom_y, bottom_xz * sin_r);
            glm::vec3 ul(top_xz * cos_l, top_y, top_xz * sin_l);
            glm::vec3 ur(top_xz * cos_r, top_y, top_xz * sin_r);



            faces->push_back(face_ind);
            faces->push_back(face_ind++);

            // face one 
            positions->push_back(ll);
            positions->push_back(lr);
            positions->push_back(ul);

            // normals
            normals->push_back(glm::vec3(0,1,0));
            normals->push_back(glm::vec3(0,1,0));
            normals->push_back(glm::vec3(0,1,0));


            // uvs -- too be filled
            uvs->push_back(glm::vec2(0,0));
            uvs->push_back(glm::vec2(0,0));
            uvs->push_back(glm::vec2(0,0));

            // face two 
            positions->push_back(ul);
            positions->push_back(lr);
            positions->push_back(ur);

            // normals
            normals->push_back(glm::vec3(0,1,0));
            normals->push_back(glm::vec3(0,1,0));
            normals->push_back(glm::vec3(0,1,0));

            // uvs -- too be filled
            uvs->push_back(glm::vec2(0,0));
            uvs->push_back(glm::vec2(0,0));
            uvs->push_back(glm::vec2(0,0));
        }
    }
    v_size = uvs->size();
    return true;
}

bool GLPrimitive::LoadCylinder()
{
    return true;
}

bool GLPrimitive::Create()
{
    //Create the VAO
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);

    this->CreateVBOs();

    //Unbind the VAO
    glBindVertexArray(0);

    return true;
}

void GLPrimitive::CreateVBOs()
{
    this->vbo_pos = GLBufferObject("vbopositions",
            sizeof(glm::vec3),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
    if( this->attributes > V_INDEX)
    {
        glEnableVertexAttribArray(V_INDEX);
        glVertexAttribPointer( V_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

     this->vbo_norm = GLBufferObject("vbonormals",
            sizeof(glm::vec3),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
    if( this->attributes > NORM_INDEX)
    {
        glEnableVertexAttribArray(NORM_INDEX);
        glVertexAttribPointer( NORM_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    this->vbo_tex = GLBufferObject("vbotextures",
            sizeof(glm::vec2),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
    if( this->attributes > UV_INDEX)
    {
        glEnableVertexAttribArray(UV_INDEX);
        glVertexAttribPointer( UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    this->vbo_color = GLBufferObject("vbocolors",
            sizeof(glm::vec3),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);

    if( this->attributes > COLOR_INDEX)
    {
        glEnableVertexAttribArray(COLOR_INDEX);
        glVertexAttribPointer( COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
}

void GLPrimitive::LoadUBO(std::shared_ptr<GLUniform>, UBO)
{
}

void GLPrimitive::Draw(GLenum MODE)
{
    GLint face_offset = 0;
    GLint vertex_offset = 0;
    glBindVertexArray(this->vao);

    glDrawArrays(MODE,0,this->positions->size());

    glBindVertexArray(0);
}

size_t GLPrimitive::numFaces()
{
    return this->faces->size();
}

size_t GLPrimitive::numVertices()
{
    return this->positions->size();
}

size_t GLPrimitive::Size()
{
    return this->faces->size();
}


