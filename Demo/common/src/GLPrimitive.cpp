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
    this->index = 0;
    
    this->Allocate();
}

GLPrimitive::GLPrimitive(const char* name, const char* typeName, GLuint attributes, long maxPoints) : GLMesh(name, typeName)
{
    this->maxPoints = maxPoints;
    this->attributes = attributes;
    this->index = 0;
    
    this->Allocate();
}

GLPrimitive::~GLPrimitive()
{
}

void GLPrimitive::Allocate()
{
    GLMesh::Allocate();

    this->materials = std::shared_ptr<std::vector<Material>>(new std::vector<Material>);
    this->textures = std::shared_ptr<std::vector<GLTexture>>(new std::vector<GLTexture>);
    this->shaders = std::shared_ptr<std::vector<Shader>>(new std::vector<Shader>);
    this->shader = std::shared_ptr<Shader>(new Shader());
}

void GLPrimitive::Create(GLenum GL_DRAW_TYPE)
{
    GLMesh::Create(GL_DRAW_TYPE);
    for(size_t i=0; i<this->textures->size(); i++)
        this->textures->at(i).Load();
    cout<<"Creating "<<name<<":: v_size: "<<v_size <<", e_size: "<<e_size<<endl;
}

void GLPrimitive::AddMesh()
{
    GLMesh::AddMesh();

    this->shaders->push_back(*this->shader);
    this->shader = std::shared_ptr<Shader>(new Shader());
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

    AddMesh();
}


bool GLPrimitive::AddCircle(double radius, double zlocation, double znormal, int npoints, bool fliporder)
{
    npoints = (npoints - 1);

    int offset = AddCircle(npoints, radius, zlocation, znormal, false);
    this->positions->push_back(Vec3(0, 0, zlocation));
    this->normals->push_back(Vec3(0, 0, znormal));

    AddTriangleFan(offset, offset + npoints, npoints, fliporder);

    AddMesh();
}

void GLPrimitive::AddPlane(double width, double height, double znormal, int textureRepeats)
{
    width *= 0.5;
    height *= 0.5;
    int currentVertex = this->positions->size();

    this->positions->push_back(Vec3(-width, -height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(0, textureRepeats));
 
    this->positions->push_back(Vec3(width, -height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(textureRepeats, textureRepeats));
    
    this->positions->push_back(Vec3(width, height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(textureRepeats, 0));
    
    this->positions->push_back(Vec3(-width, height, 0));
    this->normals->push_back(Vec3(0, 0, znormal));
    this->uvs->push_back(Vec2(0, 0));
    
    this->faces->push_back(currentVertex + 0);
    this->faces->push_back(currentVertex + 1);
    this->faces->push_back(currentVertex + 2);
    this->faces->push_back(currentVertex + 2);
    this->faces->push_back(currentVertex + 3);
    this->faces->push_back(currentVertex + 0);

    AddMesh();
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

            // uvs -- too be filled
            uvs->push_back(Vec2(0,0));
            uvs->push_back(Vec2(0,0));
            uvs->push_back(Vec2(0,0));
        }
    }
    
    AddMesh();
    return true;
}

bool GLPrimitive::AddCylinder()
{
    return true;
}

void GLPrimitive::LoadUBO(GLuint ubo, UBO rtype, Shader shader)
{
    int textureIdx = shader.texture;
    int materialIdx = shader.material;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    if (rtype == UBO::CONTROL)
    {

        Shader tmp;
        tmp.material = materialIdx == -1 ? 0 : 1;
        tmp.texture = textureIdx == -1 ? 0 : 1;
        tmp.bump = 0;
        glBufferSubData( GL_UNIFORM_BUFFER, 
                        0, 
                        sizeof(Shader), 
                         &tmp);
        return;
    }
    if (rtype == UBO::TEXTURE)
    {
        this->textures->at(textureIdx).Bind(GL_TEXTURE0);
    }
    if (rtype == UBO::COLOR)
    {
            glBufferSubData(GL_UNIFORM_BUFFER,
                        0,
                        sizeof(this->materials->at(materialIdx)),
                        &(this->materials->at(materialIdx)) );
    }

}

void GLPrimitive::DrawElements(size_t i, GLint face_offset, GLint vertex_offset, GLenum MODE)
{
    glDrawElementsBaseVertex(MODE, 
        this->_faces->at(i).size(),
        GL_UNSIGNED_INT,
        (void*)(sizeof(GLuint) * face_offset),
        vertex_offset);
}

void GLPrimitive::Clear()
{
    shaders->clear();
    GLMesh::Clear();
}

void GLPrimitive::Draw(GLenum MODE)
{
    GLint face_offset = 0;
    GLint vertex_offset = 0;

    glBindVertexArray(this->vao);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //cout<<this->name<<" has " <<this->textures->size()<<" textures\n";
    //Draw Model 
    for(size_t i=0; i< this->_faces->size(); i++)
    {      
        cout << this->_faces->size() << endl;
        int textureIdx = this->shaders->at(i).texture;
        int materialIdx = this->shaders->at(i).material;
        //cout<<"Checking Shader "<<i<<": "<<textureIdx<<" " <<materialIdx<<endl;

        this->LoadUBO(this->controlUBO, UBO::CONTROL, this->shaders->at(i));
        if (textureIdx != -1)
        {
            this->LoadUBO(this->textureUBO, UBO::TEXTURE, this->shaders->at(i));
            DrawElements(i, face_offset, vertex_offset, MODE);
        }
        else if (materialIdx != -1)
        {
            this->LoadUBO(this->materialUBO, UBO::COLOR, this->shaders->at(i));
            DrawElements(i, face_offset, vertex_offset, MODE);
        }
        else
        {
            DrawElements(i, face_offset, vertex_offset, MODE);
        }

        face_offset += this->_faces->at(i).size();
        vertex_offset += this->_positions->at(i).size();
    }

    glBindVertexArray(0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_2D, 0);
}


void GLPrimitive::AddTexture(std::shared_ptr<GLTexture> tex)
{
   cout << this->textures << endl;
   this->textures->push_back(*tex); 
}

void GLPrimitive::AssignTexture(int i, int j)
{
    if(j > this->textures->size() - 1)
    {
        cout<< "Texture not found\n";
        return;
    }
   this->shaders->at(i).texture = j;
}

void GLPrimitive::AddMaterial(Material mat)
{
    this->materials->push_back(mat);
}

void GLPrimitive::AssignMaterial(int i, int j)
{
    if(j > this->materials->size() - 1)
    {
        cout<< "Material not found\n";
        return;
    }
    this->shaders->at(i).material = j;
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
    return Vec4(this->matrix[3].x, this->matrix[3].y, this->matrix[3].z, 1.0f);
}

Vec4 GLPrimitive::Look()
{
    return Vec4(this->matrix[2].x, this->matrix[2].y, this->matrix[2].z, 1.0f);
}


void GLPrimitive::SetColor(Vec3 diffuse)
{
    this->colors->clear();
    for (int i=0; i<this->positions->size(); i++)
        this->colors->push_back(diffuse);
}

Vec3 GLPrimitive::GetColor()
{
    return this->colors->at(0);
}
