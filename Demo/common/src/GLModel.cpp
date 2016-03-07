#define CHECKGLERROR if ( glGetError() != GL_NO_ERROR ) std::cout << __FILE__ <<":"<< __LINE__ << std::endl;

#include "GLModel.hpp"
#include "GLBufferObject.hpp"
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


GLModel::GLModel(const char* name, const char* typeName) : GLMesh(name, typeName)
{
}

GLModel::GLModel(const char* filename, const char* name, const GLuint attributes) : GLMesh(name, "GLModel")
{
    this->filename = filename;
    size_t found = (this->filename).find_last_of("/");
    if(found == std::string::npos)
        this->path = "./";
    else
        this->path = this->filename.substr(0,found);
    this->attributes = attributes;

    GLMesh::Allocate();
    this->Allocate();
}

GLModel::~GLModel()
{
}

void GLModel::Allocate()
{
    this->materials = std::shared_ptr<std::vector<std::pair<aiString,Material>>>(new std::vector<std::pair<aiString,Material>>);
    this->textures = std::shared_ptr<std::vector<std::pair<bool,GLTexture>>>(new std::vector<std::pair<bool,GLTexture>>);
    this->bumpmaps = std::shared_ptr<std::vector<std::pair<bool,GLTexture>>>(new std::vector<std::pair<bool,GLTexture>>);
    this->shader = std::shared_ptr<Shader>(new Shader());

}

bool GLModel::Create()
{
    //Clear
    for(size_t i=0; i<this->_positions->size(); i++)
    {
        this->_positions->at(i).clear();
        this->_normals->at(i).clear();
        this->_uvs->at(i).clear();
        this->_colors->at(i).clear();
    }
    for(size_t i=0; i<this->_faces->size(); i++)
        this->_faces->at(i).clear();

    Assimp::Importer Importer;

    //Create the VAO
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);

    const aiScene* scene = Importer.ReadFile(filename,
            aiProcess_Triangulate | 
            aiProcess_GenSmoothNormals | 
            aiProcess_FlipUVs | 
            aiProcess_JoinIdenticalVertices);
    if (scene) 
    {
        this->_faces->resize(scene->mNumMeshes);
        this->_positions->resize(scene->mNumMeshes);
        this->_colors->resize(scene->mNumMeshes);
        this->_normals->resize(scene->mNumMeshes);
        this->_uvs->resize(scene->mNumMeshes);
        this->AddMaterials(scene->mMaterials, scene->mNumMaterials);
        for(unsigned int i=0; i<scene->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[i];
            this->AddAttributeData(mesh, i);
        }
        this->CreateVBOs();

        //Unbind the VAO
        glBindVertexArray(0);

        return true;
    }
    else 
    {
        std::cerr << "[E] Could not load " << filename << std::endl;
        std::cerr << Importer.GetErrorString() << std::endl;
    }

    return false;
}

// No VAO Created
bool GLModel::LoadVertexData()
{
    //Clear
    for(size_t i=0; i<this->_positions->size(); i++)
    {
        this->_positions->at(i).clear();
        this->_normals->at(i).clear();
    }
    for(size_t i=0; i<this->_faces->size(); i++)
        this->_faces->at(i).clear();

    Assimp::Importer Importer;

    const aiScene* scene = Importer.ReadFile(filename,
            aiProcess_Triangulate | 
            aiProcess_GenSmoothNormals | 
            aiProcess_FlipUVs | 
            aiProcess_JoinIdenticalVertices);
    if (scene) 
    {
        this->_faces->resize(scene->mNumMeshes);
        this->_positions->resize(scene->mNumMeshes);
        this->_colors->resize(scene->mNumMeshes);
        for(unsigned int i=0; i<scene->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[i];
            this->AddVertexData(mesh, i);
        }

        return true;
    }
    else 
    {
        std::cerr << "[E] Could not load " << filename << std::endl;
        std::cerr << Importer.GetErrorString() << std::endl;
    }

    return false;
}

void GLModel::AddVertexData(const aiMesh* mesh, unsigned int index)
{
    this->e_size += mesh->mNumFaces * 3;
    this->v_size += mesh->mNumVertices;

    this->_positions->at(index).resize(mesh->mNumVertices);
    this->_colors->at(index).resize(mesh->mNumVertices);
    this->_faces->at(index).resize(mesh->mNumFaces * 3);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < mesh->mNumVertices ; i++) 
    {
        const aiVector3D* pos = &(mesh->mVertices[i]);

        this->_positions->at(index).at(i) = Vec3(pos->x, pos->y, pos->z);
        this->_colors->at(index).at(i) = Vec3(1.0f, 1.0f, 1.0f);
    }

    // Populate the index buffer
    for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++) 
    {
        const aiFace* face = &(mesh->mFaces[i]);
        assert(face->mNumIndices == 3);
        this->_faces->at(index).at(3*i) = face->mIndices[0];
        this->_faces->at(index).at(3*i+1) = face->mIndices[1];
        this->_faces->at(index).at(3*i+2) = face->mIndices[2];
    }
}

void GLModel::AddAttributeData(const aiMesh* mesh, unsigned int index)
{
    const aiVector3D zero(0.0f, 0.0f, 0.0f);
    this->mtlIndices.push_back(mesh->mMaterialIndex);
    this->e_size += mesh->mNumFaces * 3;
    this->v_size += mesh->mNumVertices;

    this->_positions->at(index).resize(mesh->mNumVertices);
    this->_colors->at(index).resize(mesh->mNumVertices);
    this->_normals->at(index).resize(mesh->mNumVertices);
    this->_uvs->at(index).resize(mesh->mNumVertices);
    this->_faces->at(index).resize(mesh->mNumFaces * 3);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0 ; i < mesh->mNumVertices ; i++) 
    {
        const aiVector3D* pos = &(mesh->mVertices[i]);
        const aiVector3D* norm = &(mesh->mNormals[i]);
        const aiVector3D* uv = mesh->HasTextureCoords(0) ?
            &(mesh->mTextureCoords[0][i]) : &zero;

        this->_positions->at(index).at(i) = Vec3(pos->x, pos->y, pos->z);
        this->_normals->at(index).at(i) = Vec3(norm->x, norm->y, norm->z);
        this->_uvs->at(index).at(i) = Vec2(uv->x, uv->y);
        this->_colors->at(index).at(i) = Vec3(1.0f, 1.0f, 1.0f);
    }

    // Populate the index buffer
    for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++) 
    {
        const aiFace* face = &(mesh->mFaces[i]);
        //assert(face->mNumIndices == 3);
        this->_faces->at(index).at(3*i) = face->mIndices[0];
        this->_faces->at(index).at(3*i+1) = face->mIndices[1];
        this->_faces->at(index).at(3*i+2) = face->mIndices[2];
    }

}

void GLModel::AddMaterials(aiMaterial** materials, unsigned int numMaterials)
{
    this->materials->resize(numMaterials);

    for ( unsigned int i = 0; i < numMaterials; ++i )
    {
        aiMaterial &material = *(materials[i]);

        aiColor3D ambient(0.0f, 0.0f, 0.0f);
        aiColor3D diffuse(0.0f, 0.0f, 0.0f);
        aiColor3D specular(0.0f, 0.0f, 0.0f);
        float shininess = 10.0f;
        float intensity = 1.0f;
        float diffuseBlend = 1.0f;
        std::cout<<"diffuseBlend "<<diffuseBlend<<std::endl;
        aiString name;

        material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        material.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        material.Get(AI_MATKEY_COLOR_SPECULAR, specular); 
        material.Get(AI_MATKEY_SHININESS, shininess);
        material.Get(AI_MATKEY_TEXBLEND_DIFFUSE(0), diffuseBlend);
        material.Get(AI_MATKEY_SHININESS_STRENGTH, intensity);
        material.Get(AI_MATKEY_NAME, name);


        Material mat;
        mat.diffuse = Vec4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
        mat.ambient = Vec4(ambient.r, ambient.g, ambient.b, 1.0f);
        mat.specular = Vec4(specular.r, specular.g, specular.b, 1.0f);
        mat.shininess = shininess / 1.0f;
        mat.intensity = 1.0f + intensity;
        mat.diffuseBlend = diffuseBlend;

        //int numTextures = material.GetTextureCount(aiTextureType_DIFFUSE);

        aiString texPath;
        this->textures->resize(this->textures->size() + 1 + 1);
        this->bumpmaps->resize(this->bumpmaps->size() + 1 + 1);

        if ( material.Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE,0), texPath) == AI_SUCCESS )
        {
            std::string location;
            if( this->path == "./")
                location = this->path + texPath.data;
            else
                location = this->path + "/" + texPath.data;
            std::cout << "Texture at " << location << std::endl;
            GLTexture texture(name.C_Str(), GL_TEXTURE_2D, location.c_str());

            if (!texture.Load()) 
            {
                printf("Error loading texture '%s'\n", location.c_str());
            }
            this->textures->at(i) = std::pair<bool, GLTexture>(true, texture);
        }

        if ( material.Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT,0), texPath) == AI_SUCCESS )
        {
            std::string location;
            if( this->path == "./")
                location = this->path + texPath.data;
            else
                location = this->path + "/" + texPath.data;
            std::cout << "Bump Map at " << location << std::endl;
            GLTexture texture(name.C_Str(), GL_TEXTURE_2D, location.c_str());

            if (!texture.Load()) 
            {
                printf("Error loading texture '%s'\n", location.c_str());
            }
            this->bumpmaps->at(i) = std::pair<bool, GLTexture>(true, texture);
        }

        if(std::string(name.C_Str()) != std::string("DefaultMaterial") || numMaterials == 1)
            this->materials->at(i) = std::pair<aiString, Material>(name, mat);
    }
}

void GLModel::DrawElements(size_t i, GLint face_offset, GLint vertex_offset, GLenum MODE)
{
    glDrawElementsBaseVertex(MODE, 
        this->_faces->at(i).size(),
        GL_UNSIGNED_INT,
        (void*)(sizeof(GLuint) * face_offset),
        vertex_offset);
}

void GLModel::Draw(GLenum MODE)
{
    GLint face_offset = 0;
    GLint vertex_offset = 0;
    bool texture, bump;

    glBindVertexArray(this->vao);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Draw Model 
    for(size_t i=0; i< this->_faces->size(); i++)
    {   
        texture = this->textures->at(this->mtlIndices.at(i)).first;
        bump = this->bumpmaps->at(this->mtlIndices.at(i)).first;
        if (this->shader->texture && texture)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, this->textureUBO);
            this->textures->at(this->mtlIndices.at(i)).second.Bind(GL_TEXTURE0);
            DrawElements(i, face_offset, vertex_offset, MODE);
        }
        if (this->shader->bump && bump)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, this->bumpUBO);
            this->bumpmaps->at(this->mtlIndices.at(i)).second.Bind(GL_TEXTURE1);
            DrawElements(i, face_offset, vertex_offset, MODE);
        }
        if (this->shader->material)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, this->materialUBO);
            glBufferSubData(GL_UNIFORM_BUFFER,
                        0,
                        sizeof(this->materials->at(this->mtlIndices.at(i)).second),
                        &(this->materials->at(this->mtlIndices.at(i)).second) );
            DrawElements(i, face_offset, vertex_offset, MODE);
        }
        else
            DrawElements(i, face_offset, vertex_offset, MODE);


        face_offset += this->_faces->at(i).size();
        vertex_offset += this->_positions->at(i).size();
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}


void GLModel::LoadUBO(std::shared_ptr<GLUniform> ubo, UBO rtype)
{
    glBindBuffer(GL_UNIFORM_BUFFER, ubo->getId());
    if (rtype == UBO::CONTROL)
    {
        glBufferSubData( GL_UNIFORM_BUFFER, 
                        0, 
                        sizeof(this->shader), 
                         &this->shader);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GLModel::setMatrix(glm::mat4 m)
{
    this->matrix = m;
}

glm::mat4 GLModel::Matrix()
{
    return this->matrix;
}

Vec4 GLModel::Position()
{
    return  Vec4(this->matrix[3].x, this->matrix[3].y, this->matrix[3].z, 1.0f);
}

void GLModel::SetColor(Vec4 diffuse)
{
    this->materials->at(this->mtlIndices.at(0)).second.diffuse = diffuse;
}

Vec4 GLModel::GetColor()
{
    return this->materials->at(this->mtlIndices.at(0)).second.diffuse;
}
