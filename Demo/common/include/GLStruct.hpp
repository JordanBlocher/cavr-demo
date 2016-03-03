#ifndef GLSTRUCT_H
#define GLSTRUCT_H

#include "GLMath.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <vector>

struct Material
{
    Vec4 ambient;
    Vec4 diffuse;
    Vec4 specular;
    float shininess;
    float intensity;
    float diffuseBlend;
};

struct Uniform
{
    std::string name;
    GLint size;
    GLuint index;
    GLint offset;
};

struct Player
{
    std::string name;
    int score;
    std::string puck;
    bool winner;
};

struct BaseLight
{
    Vec4 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Vec4 direction;
    BaseLight base;
};

struct PointLight
{
    Vec4 position;
    BaseLight base;
};

struct SpotLight
{
    Vec4 direction;
    PointLight point;
};

struct Matrices
{
    glm::mat4 mvpMatrix;
    glm::mat4 mvMatrix;
    glm::mat4 normalMatrix;
};

struct Lights
{
    DirectionalLight basic;
    std::vector<PointLight> point;
    std::vector<SpotLight> spot;

};

struct Shader
{
    Shader():texture(0), bump(0), material(0) {}
    bool texture;
    bool bump;
    bool material;
};

#endif

