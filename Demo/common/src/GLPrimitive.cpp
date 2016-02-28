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


GLPrimitive::GLPrimitive(const char* name, const GLuint attributes) : GLNode(name)
{
    this->filename = this->toString(type);
    this->type = type;
    this->path = "models/";
    this->attributes = attributes;
    this->Allocate();
}

GLPrimitive::~GLPrimitive()
{
}

void GLPrimitive::Allocate()
{
    this->e_size = 0;
    this->v_size = 0;

    // Allocate based on primitive that is loaded in. 
    this->positions = std::shared_ptr<std::vector<std::vector<glm::vec3>>>(new std::vector<std::vector<glm::vec3>>);
    this->normals = std::shared_ptr<std::vector<std::vector<glm::vec3>>>(new std::vector<std::vector<glm::vec3>>);
    this->uvs = std::shared_ptr<std::vector<std::vector<glm::vec2>>>(new std::vector<std::vector<glm::vec2>>);
    this->faces = std::shared_ptr<std::vector<std::vector<GLuint>>>(new std::vector<std::vector<GLuint>>);
    this->materials = std::shared_ptr<std::vector<std::pair<aiString,Material>>>(new std::vector<std::pair<aiString,Material>>);
    this->textures = std::shared_ptr<std::vector<std::pair<bool,GLTexture>>>(new std::vector<std::pair<bool,GLTexture>>);

}

bool GLPrimitive::LoadSphere()
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
            vec3 ll(bottom_xz * cos_l, bottom_y, bottom_xz * sin_l);
            vec3 lr(bottom_xz * cos_r, bottom_y, bottom_xz * sin_r);
            vec3 ul(top_xz * cos_l, top_y, top_xz * sin_l);
            vec3 ur(top_xz * cos_r, top_y, top_xz * sin_r);



            faces.push_back(face_ind);
            faces.push_back(face_ind++);

            // face one 
            positions.push_back(ll);
            positions.push_back(lr);
            positions.push_back(ul);

            // normals
            normals.push_back(glm::vec3(0,1,0));
            normals.push_back(glm::vec3(0,1,0));
            normals.push_back(glm::vec3(0,1,0));


            // uvs -- too be filled
            uvs.push_back(glm::vec2(0,0));
            uvs.push_back(glm::vec2(0,0));
            uvs.push_back(glm::vec2(0,0));

            // face two 
            positions.push_back(ul);
            positions.push_back(lr);
            positions.push_back(ur);

            // normals
            normals.push_back(glm::vec3(0,1,0));
            normals.push_back(glm::vec3(0,1,0));
            normals.push_back(glm::vec3(0,1,0));

            // uvs -- too be filled
            uvs.push_back(glm::vec2(0,0));
            uvs.push_back(glm::vec2(0,0));
            uvs.push_back(glm::vec2(0,0));
        }
    }
    return false;
}

bool GLPrimitive::LoadCylinder()
{
    return true;
}

bool GLPrimitive::CreateVAO()
{
    //Create the VAO
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);

    this->CreateVBOs();

    //Unbind the VAO
    glBindVertexArray(0);

    return true;
}

// No VAO Created
bool GLPrimitive::LoadVertexData()
{
    // Load some primitive here.

    return false;
}

void GLPrimitive::CreateVBOs()
{
    //Create VBOs 
    GLBufferObject vbo_pos("vbopositions",
            sizeof(glm::vec3),
            this->v_size,
            GL_ARRAY_BUFFER,
            GL_STATIC_DRAW);
    size_t offset = 0;
    for(size_t i=0; i<this->positions->size(); i++)
    {
        vbo_pos.LoadSubData(offset, 0, this->positions->at(i));
        offset += this->positions->at(i).size();
    }
    if( this->attributes > V_INDEX)
    {
        glEnableVertexAttribArray(V_INDEX);
        glVertexAttribPointer( V_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    GLBufferObject vbo_norms("vbonormals",
            sizeof(glm::vec3),
            this->v_size,
            GL_ARRAY_BUFFER,
            GL_STATIC_DRAW);
    offset = 0;
    for(size_t i=0; i<this->positions->size(); i++)
    {
        vbo_norms.LoadSubData(offset, 1, this->normals->at(i));
        offset += this->positions->at(i).size();
    }
    if( this->attributes > NORM_INDEX)
    {
        glEnableVertexAttribArray(NORM_INDEX);
        glVertexAttribPointer( NORM_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    GLBufferObject vbo_uvs("vbotextures",
            sizeof(glm::vec2),
            this->v_size,
            GL_ARRAY_BUFFER,
            GL_STATIC_DRAW);
    offset = 0;
    for(size_t i=0; i<this->positions->size(); i++)
    {
        vbo_uvs.LoadSubData(offset, 2, this->uvs->at(i));
        offset += this->positions->at(i).size();
    }
    if( this->attributes > UV_INDEX)
    {
        glEnableVertexAttribArray(UV_INDEX);
        glVertexAttribPointer( UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    GLBufferObject vbo_elements("vboelements",
            sizeof(GLuint),
            this->e_size,
            GL_ELEMENT_ARRAY_BUFFER,
            GL_STATIC_DRAW);
    offset = 0;
    for(size_t i=0; i<this->faces->size(); i++)
    {
        vbo_elements.LoadSubData(offset, 0, this->faces->at(i));
        offset += this->faces->at(i).size();
    }
}


void GLPrimitive::Draw(std::shared_ptr<GLUniform> fragment, GLuint program)
{
    GLint face_offset = 0;
    GLint vertex_offset = 0;
    glBindVertexArray(this->vao);

    bool texture, color;

    color = (fragment->getId() != UINT_MAX);

    if(color)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, fragment->getId());
    }

    //Draw Model 
    for(size_t i=0; i< this->faces->size(); i++)
    {   

        texture = this->textures->at(this->mtlIndices.at(i)).first;
        if(texture)
            this->textures->at(this->mtlIndices.at(i)).second.Bind(GL_TEXTURE0);
        if(color)
        {
            glBufferSubData(GL_UNIFORM_BUFFER,
                        0,
                        sizeof(this->materials->at(this->mtlIndices.at(i)).second),
                        &(this->materials->at(this->mtlIndices.at(i)).second) );
        }

        if( (color && !texture) || (!color && texture) )
        {
            glDrawElementsBaseVertex(GL_TRIANGLES, 
                    this->faces->at(i).size(),
                    GL_UNSIGNED_INT,
                    (void*)(sizeof(GLuint) * face_offset),
                    vertex_offset);
        }

        face_offset += this->faces->at(i).size();
        vertex_offset += this->positions->at(i).size();
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

size_t GLPrimitive::numFaces()
{
    return this->e_size;
}

size_t GLPrimitive::numVertices()
{
    return this->v_size;
}

size_t GLPrimitive::Size()
{
    return this->faces->size();
}

const std::vector<glm::vec3>& GLPrimitive::Positions(size_t index)
{
    return this->positions->at(index);
}

const std::vector<GLuint>& GLPrimitive::Faces(size_t index)
{
    return this->faces->at(index);
}

void GLPrimitive::setMatrix(glm::mat4 m)
{
    this->matrix = m;
}

glm::mat4 GLPrimitive::Matrix()
{
    return this->matrix;
}

glm::vec4 GLPrimitive::Position()
{
    return glm::vec4(10.0f, 10.0f, 10.0f, 1.0f) * this->matrix;
}

std::string GLPrimitive::toString(MODEL type)
{
    switch(type)
    {
        case (CUBE):
            return "cube.obj";
            break;
        default:
            return "";
            break;
    }
}


