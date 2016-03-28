#ifndef GLMAIN_H
#define GLMAIN_H

#include <GLViewport.hpp>
#include <GLCamera.hpp>
#include <GLProgram.hpp>
#include <GLFrame.hpp>
#include <GLModel.hpp>
#include <SoundManager.hpp>
#include <SoundPlayer.hpp>

#include <chrono>
#include <map>


class GLScene : public GLViewport
{
 public:
    GLScene();

    void AddModel(const char*, const char*);
    void InitShaders();
    void MoveCamera();
    void Event();
    void Render();
    void Create();

    // Positions and size are [0,1] for both x and y dimensions...    
    void DrawString(glm::vec2 ScreenPos, glm::vec2 Size, string text);

    template <typename T> 
        void Paint(shared_ptr<T>, GLenum);

 protected:
    void LoadGlobalUBOs(Matrices);

    std::vector<std::pair<int, glm::mat4>> strokes;
    int color;
    bool pallet;
    
    bool Break;
    bool record;
    bool PaintOff;

    // To call custom programs... or maybe to replace the one above
    template<typename T>
    void CustomHelper(string program, shared_ptr<T>, GLenum);

    SoundPlayer player;
};

template <typename T> 
void GLScene::Paint(shared_ptr<T> model, GLenum MODE)
{
    // Run Program
    shared_ptr<GLProgram> program = this->Get<GLProgram>("program");
    glUseProgram(program->getId());

    // Calculate MVP
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera");
    glm::mat4 vp = camera1->Projection() * camera1->View() ;
    Matrices matrices;
    matrices.mvpMatrix = vp * model->Matrix();
    matrices.mvMatrix = model->Matrix(); 
    matrices.normalMatrix = glm::transpose(glm::inverse(model->Matrix()));

    // Load Global UBOs
    LoadGlobalUBOs(matrices);

    // Bind Controls
    model->controlUBO = this->Get<GLUniform>("Shader")->getId();
    // Bind Colors
    model->materialUBO = this->Get<GLUniform>("GColors")->getId();
    // Bind Textures
    model->textureUBO = this->Get<GLUniform>("Texture")->getId();

    model->Draw(MODE);

    glUseProgram(0);
}

template <typename T> 
void GLScene::CustomHelper(string Program, shared_ptr<T> model, GLenum MODE)
{

    // Run Program
    shared_ptr<GLProgram> program = this->Get<GLProgram>(Program.c_str());
    glUseProgram(program->getId());

    // Calculate MVP
    shared_ptr<GLCamera> camera = this->Get<GLCamera>("camera");
    glm::mat4 vp = camera->Projection() * camera->View() ;

    Matrices matrices;
    matrices.mvpMatrix = /*vp **/ model->Matrix();
    matrices.mvMatrix = model->Matrix(); 
    matrices.normalMatrix = glm::transpose(glm::inverse(model->Matrix()));

    // Load Global UBOs
    LoadGlobalUBOs(matrices);

    // Bind Controls
    model->controlUBO = this->Get<GLUniform>("Shader")->getId();
    // Bind Colors
    model->materialUBO = this->Get<GLUniform>("GColors")->getId();
    // Bind Textures
    model->textureUBO = this->Get<GLUniform>("Texture")->getId();

    model->Draw(MODE);

    glUseProgram(0);

}
#endif 

