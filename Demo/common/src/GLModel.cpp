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


GLModel::GLModel(const char* filename, const char* name, const GLuint attributes) : GLPrimitive(name, "GLModel", attributes, 0)
{
    this->filename = filename;
    size_t found = (this->filename).find_last_of("/");
    if(found == std::string::npos)
        this->path = "./";
    else
        this->path = this->filename.substr(0,found);

    GLPrimitive::Allocate();
}

GLModel::~GLModel()
{
}

bool GLModel::LoadModel()
{
    cout<<"Loading "<<this->name<<" model."<<endl;

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
        this->shaders->resize(scene->mNumMaterials-1);
        this->AddMaterials(scene->mMaterials, scene->mNumMaterials);
        for(unsigned int i=0; i<scene->mNumMeshes; i++)
        {
            const aiMesh* mesh = scene->mMeshes[i];
            this->AddAttributeData(mesh, i);
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
    for ( unsigned int i = 0; i < numMaterials; ++i )
    {
        aiMaterial &material = *(materials[i]);

        aiColor3D ambient(0.0f, 0.0f, 0.0f);
        aiColor3D diffuse(0.0f, 0.0f, 0.0f);
        aiColor3D specular(0.0f, 0.0f, 0.0f);
        float shininess = 10.0f;
        float intensity = 1.0f;
        float diffuseBlend = 1.0f;
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

        aiString texPath;

        if ( material.Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE,0), texPath) == AI_SUCCESS )
        {
            std::string location;
            if( this->path == "./")
                location = this->path + texPath.data;
            else
                location = this->path + "/" + texPath.data;
            GLTexture texture(name.C_Str(), GL_TEXTURE_2D, location.c_str());

            //cout<<"Assigning texture "<<name.C_Str()<<" to shader "<<i-1<<endl;
            this->textures->push_back(texture);
            this->shaders->at(i-1).texture = i-1;
        }

        if ( material.Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT,0), texPath) == AI_SUCCESS )
        {
            std::string location;
            if( this->path == "./")
                location = this->path + texPath.data;
            else
                location = this->path + "/" + texPath.data;
            GLTexture texture(name.C_Str(), GL_TEXTURE_2D, location.c_str());

            this->bumpmaps->push_back(texture);
            this->shaders->at(i-1).bump = i-1;
        }
        
        if(numMaterials == 1)
        {
            //cout<<"Assigning material "<<i <<" to shader "<<i<<endl;
            this->materials->push_back(mat);
            this->shaders->at(i).material = i;
        }

    }
}

