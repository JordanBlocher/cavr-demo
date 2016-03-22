#include <GLScene.hpp>
#include <GLRibbon.hpp>
#include "cavr.hpp"


// Context Data 
static std::shared_ptr<GLScene> cd(new GLScene());

// Initialize our program
void initContext() 
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    GLenum status = glewInit();
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW not initialized: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return;
    }

    cavr::System::setContextData(cd.get());

    {
        GLScene *cd;
        cd = (GLScene*)(cavr::System::getContextData());

        cd->InitShaders();

        shared_ptr<GLModel> dragon = cd->Get<GLModel>("dragon");
        dragon->Create(GL_STATIC_DRAW);
        shared_ptr<GLRibbon> ribbons = cd->Get<GLRibbon>("ribbons");
        ribbons->Create(GL_DYNAMIC_DRAW);
    }
}

void frame() 
{
    //GLScene *cd;
    //cd = (GLScene*)(cavr::System::getContextData());
}

void render() 
{
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLScene *cd;
    cd = (GLScene*)(cavr::System::getContextData());

    shared_ptr<GLModel> dragon = cd->Get<GLModel>("dragon");
    dragon->setMatrix(glm::translate(glm::mat4(1.0f), Vec3(0.0f, -1.0f, -3.0f)) * glm::scale(glm::mat4(1.0f), Vec3(0.2f, 0.2f, 0.2f)));
    dragon->shader->texture = 1;
    cd->Paint<GLModel>(dragon, GL_TRIANGLES);
    
    shared_ptr<GLRibbon> ribbons = cd->Get<GLRibbon>("ribbons");
    ribbons->AssignTexture(0, 1);
    ribbons->AssignTexture(1, 0);
    ribbons->AssignTexture(2, 1);
    cd->Paint<GLRibbon>(ribbons, GL_TRIANGLES);
   
    shared_ptr<SoundManager> soundMan = cd->Get<SoundManager>("soundMan");
    soundMan->PlayFX(0, ribbons->Tail());

    //shared_ptr<GLPrimitive> primitive = this->Get<GLPrimitive>("primitive");
    //this->PaintHelper(primitive, GL_TRIANGLES);
}

void destructContext() 
{
    cd.reset();
}

void update() 
{
    if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
       cavr::System::shutdown();
      return;
    }
 
    cd->Event();

    cd->MoveCamera();
}

int main(int argc, char** argv) 
{
  // cavr is a system of callbacks
  cavr::System::setCallback("update", update);
  cavr::System::setCallback("init_gl_context", initContext);
  cavr::System::setCallback("update_gl_context", frame);
  cavr::System::setCallback("gl_render", render);
  cavr::System::setCallback("destruct_gl_context", destructContext);
  LOG(INFO) << "Callbacks set.";

  // Set inputs
  if (!setup_map(argc, argv))
  {
      cout << "FAILED TO LAUNCH CAVR!!!" << endl;
      return -1;
  }

  // Run cavr -- 
  // notice that this will be an infinite loop (cavr::system.cpp)
  LOG(INFO) << "Starting simulation.";
  cout << "Starting Simulation" << endl;
  cavr::System::run();

  LOG(INFO) << "Shutting down.";
  return 0;
}

