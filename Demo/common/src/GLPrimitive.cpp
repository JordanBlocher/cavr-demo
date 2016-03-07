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
    this->materials = std::shared_ptr<std::vector<Material>>(new std::vector<Material>);
    this->textures = std::shared_ptr<std::vector<GLTexture>>(new std::vector<GLTexture>);
    this->shaders = std::shared_ptr<std::vector<Shader>>(new std::vector<Shader>);
    this->shader = std::shared_ptr<Shader>(new Shader());
    this->shader->material = -1;
    this->shader->texture = -1;
    this->shader->bump = -1;
    GLMesh::Allocate();
}

void GLPrimitive::AddMesh()
{
    GLMesh::AddMesh();
<<<<<<< HEAD
    Shader shader;
    shader.material = -1;
    shader.texture = -1;
    shader.bump = -1;
    this->shaders->push_back(shader);
    this->shader = this->shaders->at(index);
    //std::cout<<"SHADER SIZE "<<this->shaders->size()<<endl;
=======
    this->shaders->push_back(*this->shader);
    this->shader = std::shared_ptr<Shader>(new Shader());
    this->shader->material = -1;
    this->shader->texture = -1;
    this->shader->bump = -1;
    cout<<"Adding mesh... at idx= "<<index<<endl;
>>>>>>> 972bec9e18993cd287b95fe99831901f979005a0
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

void GLPrimitive::LoadUBO(std::shared_ptr<GLUniform> ubo, UBO rtype)
{
    bool texture, bump;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo->getId());
    
    if (rtype == UBO::CONTROL)
    {
        int textureIdx = this->shader->texture;
        int materialIdx = this->shader->material;
        Shader shader;
        shader.material = materialIdx == -1 ? 0 : 1;
        shader.texture = textureIdx == -1 ? 0 : 1;
        shader.bump = 0;
        glBufferSubData( GL_UNIFORM_BUFFER, 
                        0, 
                        sizeof(Shader), 
                         &shader);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLPrimitive::DrawElements(size_t i, GLint face_offset, GLint vertex_offset, GLenum MODE)
{
    glDrawElementsBaseVertex(MODE, 
        this->_faces->at(i).size(),
        GL_UNSIGNED_INT,
        (void*)(sizeof(GLuint) * face_offset),
        vertex_offset);
}

void GLPrimitive::Draw(GLenum MODE)
{
    GLint face_offset = 0;
    GLint vertex_offset = 0;

    glBindVertexArray(this->vao);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Draw Model 
        cout<<"DRAWING "<<_faces->size()<<" "<<_positions->size()<<" "<<shaders->size()<<endl;
    for(size_t i=0; i< this->_faces->size(); i++)
    {      
        cout<<"In face "<<i<<" : "<<_positions->at(i).size()<< " : "<<_faces->at(i).size()<<endl;
        if(_positions->at(i).size() == 0)
            continue;
        int textureIdx = this->shaders->at(i).texture;
        int materialIdx = this->shaders->at(i).material;
        std::cout<<"SHADER: "<<materialIdx<<" "<<textureIdx<<endl;
        if (textureIdx != -1)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, this->textureUBO);
            this->textures->at(textureIdx).Bind(GL_TEXTURE0);
            DrawElements(i, face_offset, vertex_offset, MODE);
        }
        if (materialIdx != -1)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, this->materialUBO);
            glBufferSubData(GL_UNIFORM_BUFFER,
                        0,
                        sizeof(this->materials->at(materialIdx)),
                        &(this->materials->at(materialIdx)) );
            DrawElements(i, face_offset, vertex_offset, MODE);
        }
        Shader tmp;
        tmp.material = materialIdx < 0 ? 0 : 1;
        tmp.texture = textureIdx < 0 ? 0 : 1;
        tmp.bump = 0;
        glBindBuffer(GL_UNIFORM_BUFFER, this->controlUBO);
        glBufferSubData( GL_UNIFORM_BUFFER, 
                        0, 
                        sizeof(Shader), 
                         &tmp);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        if(textureIdx == -1 && materialIdx == -1)
        {
            DrawElements(i, face_offset, vertex_offset, MODE);
        }

        face_offset += this->_faces->at(i).size();
        vertex_offset += this->_positions->at(i).size();
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}


void GLPrimitive::AddTexture(std::shared_ptr<GLTexture> tex)
{
   this->textures->push_back(*tex); 
   this->shaders->at(index-1).texture = this->textures->size()-1;
   std::cout<<"Adding texture "<<this->textures->size()-1<< " to idx "<<index-1<<endl;
}

void GLPrimitive::AddMaterial(Material mat)
{
    this->materials->push_back(mat);
    this->shaders->at(index-1).material = this->materials->size()-1;
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
