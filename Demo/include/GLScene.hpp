#ifndef GLMAIN_H
#define GLMAIN_H

#include <GLViewport.hpp>
#include <GLCamera.hpp>
#include <GLProgram.hpp>
#include <GLFrame.hpp>
#include <GLModel.hpp>

#include <chrono>

class GLScene : public GLViewport
{
 public:
    GLScene();

    void AddModel(const char*, const char*);
    void InitializeGL();
    void Paint();
    void MoveCamera(GLCamera::CamDirection);

 protected:
    std::string modelpath;
    std::string materialpath;
    void LoadGlobalUBOs(Matrices);

    std::chrono::time_point<std::chrono::high_resolution_clock> time;
    std::vector<std::pair<Vec4, glm::mat4>> strokes;
    Vec4 color;
    bool painting;
    
    template <typename T> 
    void PaintHelper(shared_ptr<T>, GLenum);

};

template <typename T> 
int sgn(T val) 
{
    return (T(0) < val) - (val < T(0));
}

template <typename T> 
void GLScene::PaintHelper(shared_ptr<T> model, GLenum MODE)
{

    // Run Program
    shared_ptr<GLProgram> program = this->Get<GLProgram>("program");
    glUseProgram(program->getId());

    // Calculate MVP
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    glm::mat4 vp = camera1->Projection() * camera1->View() *GLMath::mat4ftoGLM(cavr::gfx::getView());
    Matrices matrices;
    matrices.mvpMatrix = vp * model->Matrix();
    matrices.mvMatrix = model->Matrix(); 
    matrices.normalMatrix = glm::transpose(glm::inverse(model->Matrix()));

    // Load Global UBOs
    LoadGlobalUBOs(matrices);

    // Bind Shader Controls
    shared_ptr<GLUniform> controluniform = this->Get<GLUniform>("Shader");
    model->LoadUBO(controluniform, UBO::CONTROL);
    //std::cout<<"UNIFORM "<< controluniform->getId() <<std::endl; 
    
    // Bind Colors
    if (model->shader.material)
        model->materialUBO = this->Get<GLUniform>("GColors");
    // Bind Textures
    if (model->shader.texture)
        model->textureUBO = this->Get<GLUniform>("Texture");

    model->Draw(MODE);

    glUseProgram(0);

}
#endif 

