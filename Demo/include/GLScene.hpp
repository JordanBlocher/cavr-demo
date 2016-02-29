#ifndef GLMAIN_H
#define GLMAIN_H

#include <GLViewport.hpp>
#include <GLCamera.hpp>
#include <GLProgram.hpp>
#include <GLFrame.hpp>
#include <GLModel.hpp>
#include <GLMath.hpp>
#include <GLUniform.hpp>

#include <chrono>

// Also does not belong here
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 


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
    std::vector<std::pair<glm::vec4, glm::mat4>> strokes;
    glm::vec4 color;
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
    // Bind Colors
    shared_ptr<GLUniform> coloruniform = this->Get<GLUniform>("GColors");
    model->LoadUBO(coloruniform, GLModel::UBO::COLOR);

    // Bind Textures
    shared_ptr<GLUniform> texuniform = this->Get<GLUniform>("Texture");
    model->LoadUBO(texuniform, GLModel::UBO::TEXTURE);
    
    // Calculate MVP
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    glm::mat4 vp = camera1->Projection() * camera1->View() *GLMath::mat4ftoGLM(cavr::gfx::getView());
    Matrices matrices;
    matrices.mvpMatrix = vp * model->Matrix();
    matrices.mvMatrix = model->Matrix(); 
    matrices.normalMatrix = glm::transpose(glm::inverse(model->Matrix()));

    // Load the other UBOs
    LoadGlobalUBOs(matrices);

    // Run Program
    shared_ptr<GLProgram> program = this->Get<GLProgram>("program");
    glUseProgram(program->getId());
    model->Draw(MODE);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_TEXTURE0, 0);
    glBindBuffer(GL_TEXTURE1, 0);
    glUseProgram(0);

}
#endif 

