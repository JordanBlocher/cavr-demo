#include "GLPrimitive.hpp"
#include "GLBufferObject.hpp"
#include "GLUniform.hpp"
#include "GLTexture.hpp"

#include <fstream>
#include <csignal>
#include <sstream>
#include <assert.h>



GLPrimitive::GLPrimitive(const char* name, GLuint attributes, long maxPoints) : GLMesh(name, "GLPrimitive")
{
    this->maxPoints = maxPoints;
    this->attributes = attributes;
    this->Allocate();
}

GLPrimitive::GLPrimitive(const char* name, const char* typeName, GLuint attributes, long maxPoints) : GLMesh(name, typeName)
{
    this->maxPoints = maxPoints;
    this->attributes = attributes;
    this->Allocate();
}

GLPrimitive::~GLPrimitive()
{
}

bool GLPrimitive::AddUVSphere(int nlats, int nlongs)
{
    nlongs = (nlongs - 1);
    nlats = (nlats - 1);

    double z, x, y, radius;
    double longstep = 1.0 / nlongs;
    double latstep = 1.0 / nlats;
    int offset, previousOffset = 0;
    int uvOffset = -1, uvPreviousOffset = -1;
    Vec3 d;

    for (int u = 0; u < nlats + 1; u++)
    {
        z = cos(M_PI * u * latstep);
        radius = sqrt(1 - (z * z));
        offset = positions->size();
        uvOffset = uvs->size();

        for (int v = 0; v < nlongs; v++)
        {
            x = radius * cos(v * longstep * M_PI * 2.0);
            y = radius * sin(v * longstep * M_PI * 2.0);
            d = glm::normalize(Vec3(x, y, z));
            this->positions->push_back(Vec3(x, y, z));
            this->normals->push_back(d);
            this->uvs->push_back(Vec2(0.5 + (atan2(d.z, d.x) / (2 * M_PI)), 0.5 - 2 * (asin(d.y) / (2 * M_PI))));
        }
        if (u > 0)
        {
            this->AddTriangleStrip(previousOffset, offset, nlongs, true);
        }
        previousOffset = offset;
        uvPreviousOffset = uvOffset;
    }
}


bool GLPrimitive::AddCircle(double radius, double zlocation, double znormal, int npoints, bool fliporder)
{
    npoints = (npoints - 1);

    int offset = AddCircle(npoints, radius, zlocation, znormal, false);
    this->positions->push_back(Vec3(0, 0, zlocation));
    this->normals->push_back(Vec3(0, 0, znormal));

    AddTriangleFan(offset, offset + npoints, npoints, fliporder);
}

void GLPrimitive::AddPlane(double width, double height, double znormal, int textureRepeats)
{
    width *= 0.5;
    height *= 0.5;
    int currentVertex = this->positions->size();

    this->positions->push_back(Vec3(-width, height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(0, textureRepeats));
 
    this->positions->push_back(Vec3(width, height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(textureRepeats, textureRepeats));
    
    this->positions->push_back(Vec3(width, -height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(textureRepeats, 0));
    
    this->positions->push_back(Vec3(-width, -height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(0, 0));
    
    this->faces->push_back(currentVertex + 2);
    this->faces->push_back(currentVertex + 0);
    this->faces->push_back(currentVertex + 3);
    this->faces->push_back(currentVertex + 0);
    this->faces->push_back(currentVertex + 2);
    this->faces->push_back(currentVertex + 1);
}

bool GLPrimitive::AddSphere(int num_lats, int num_lons)
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
            Vec3 ll(bottom_xz * cos_l, bottom_y, bottom_xz * sin_l);
            Vec3 lr(bottom_xz * cos_r, bottom_y, bottom_xz * sin_r);
            Vec3 ul(top_xz * cos_l, top_y, top_xz * sin_l);
            Vec3 ur(top_xz * cos_r, top_y, top_xz * sin_r);



            faces->push_back(face_ind);
            faces->push_back(face_ind++);

            // face one 
            positions->push_back(ll);
            positions->push_back(lr);
            positions->push_back(ul);

            // normals
            normals->push_back(Vec3(0,1,0));
            normals->push_back(Vec3(0,1,0));
            normals->push_back(Vec3(0,1,0));


            // uvs -- too be filled
            uvs->push_back(Vec2(0,0));
            uvs->push_back(Vec2(0,0));
            uvs->push_back(Vec2(0,0));

            // face two 
            positions->push_back(ul);
            positions->push_back(lr);
            positions->push_back(ur);

            // normals
            normals->push_back(Vec3(0,1,0));
            normals->push_back(Vec3(0,1,0));
            normals->push_back(Vec3(0,1,0));

            // uvs -- too be filled
            uvs->push_back(Vec2(0,0));
            uvs->push_back(Vec2(0,0));
            uvs->push_back(Vec2(0,0));
        }
    }
    v_size = uvs->size();
    return true;
}

bool GLPrimitive::AddCylinder()
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
            sizeof(Vec3),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
    if( this->attributes > V_INDEX)
    {
        glEnableVertexAttribArray(V_INDEX);
        glVertexAttribPointer( V_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

     this->vbo_norm = GLBufferObject("vbonormals",
            sizeof(Vec3),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
    if( this->attributes > NORM_INDEX)
    {
        glEnableVertexAttribArray(NORM_INDEX);
        glVertexAttribPointer( NORM_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    this->vbo_tex = GLBufferObject("vbotextures",
            sizeof(Vec2),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
    if( this->attributes > UV_INDEX)
    {
        glEnableVertexAttribArray(UV_INDEX);
        glVertexAttribPointer( UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    this->vbo_color = GLBufferObject("vbocolors",
            sizeof(Vec3),
            maxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);

    if( this->attributes > COLOR_INDEX)
    {
        glEnableVertexAttribArray(COLOR_INDEX);
        glVertexAttribPointer( COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }


    this->vbo_elements = GLBufferObject("vboelements",
            sizeof(GLuint),
            maxPoints,
            GL_ELEMENT_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);

}

void GLPrimitive::LoadUBO(std::shared_ptr<GLUniform>, UBO)
{
}

void GLPrimitive::Draw(GLenum MODE)
{
    LoadVAO();
    GLint face_offset = 0;
    GLint vertex_offset = 0;
    glBindVertexArray(this->vao);
        
    /*
    glDrawElements(MODE, 
            this->faces->size(),
            GL_UNSIGNED_INT,
            (void*)(sizeof(GLuint) * face_offset));
            */
    glDrawArrays(MODE,0,this->positions->size());

    glBindVertexArray(0);
}

void GLPrimitive::LoadVAO()
{
    vbo_pos.LoadSubData(0, 0, *(this->positions.get()));
    vbo_norm.LoadSubData(0, 1, *(this->normals.get()));
    vbo_color.LoadSubData(0, 2, *(this->colors.get()));
    vbo_tex.LoadSubData(0, 3, *(this->uvs.get()));
    vbo_elements.LoadSubData(0, 0, *(this->faces.get()));
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

void GLPrimitive::setMatrix(glm::mat4 m)
{
    this->matrix = m;
}

glm::mat4 GLPrimitive::Matrix()
{
    return this->matrix;
}

Vec4 GLPrimitive::Position()
{
    return  Vec4(this->matrix[3].x, this->matrix[3].y, this->matrix[3].z, 1.0f);
}


