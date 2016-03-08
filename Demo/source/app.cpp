#include <GLScene.hpp>
#include <GLRibbon.hpp>
#include "cavr.hpp"


// Context Data 
static GLScene *cd;

// Initialize our program
void initContext() 
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    cavr::System::setContextData(cd);

    cd->InitShaders();

    shared_ptr<GLModel> dragon = cd->Get<GLModel>("dragon");
    dragon->Create(GL_STATIC_DRAW);
    
    shared_ptr<GLRibbon> ribbons = cd->Get<GLRibbon>("ribbons");
    ribbons->Create(GL_DYNAMIC_DRAW);
    
}

void frame() 
{
    cd->Update();
}

void render() 
{
    cd->MoveCamera();

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shared_ptr<GLModel> dragon = cd->Get<GLModel>("dragon");
    dragon->setMatrix(glm::translate(glm::mat4(1.0f), Vec3(0.0f, -1.0f, -3.0f)) * glm::scale(glm::mat4(1.0f), Vec3(0.2f, 0.2f, 0.2f)));
    dragon->shader->texture = 1;
    cd->Paint<GLModel>(dragon, GL_TRIANGLES);
    
    shared_ptr<GLRibbon> ribbons = cd->Get<GLRibbon>("ribbons");
    ribbons->AssignTexture(0, 1);
    ribbons->AssignTexture(1, 0);
    ribbons->AssignTexture(2, 1);
    cd->Paint<GLRibbon>(ribbons, GL_TRIANGLES);

    //shared_ptr<GLPrimitive> primitive = this->Get<GLPrimitive>("primitive");
    //cd->PaintHelper(primitive, GL_TRIANGLES);
}

void destructContext() 
{
    //cd.reset();
    delete(cd);
}

void update() 
{
    if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
       cavr::System::shutdown();
      return;
    }
    cd->Event();
}

int main(int argc, char** argv) 
{

  cd = new GLScene();
  cd->Create();

  // cavr is a system of callbacks
  cavr::System::setCallback("update", update);
  cavr::System::setCallback("init_gl_context", initContext);
  cavr::System::setCallback("update_gl_context", frame);
  cavr::System::setCallback("gl_render", render);
  cavr::System::setCallback("destruct_gl_context", destructContext);
  LOG(INFO) << "Callbacks set.";

  if (!setup_map(argc, argv))
      return -1;

  // Run cavr -- 
  // notice that this will be an infinite loop (cavr::system.cpp)
  LOG(INFO) << "Starting simulation.";
  cavr::System::run();

  LOG(INFO) << "Shutting down.";
  return 0;
}

