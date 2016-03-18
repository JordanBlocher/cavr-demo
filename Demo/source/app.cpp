#include <GLScene.hpp>
#include <GLRibbon.hpp>
#include "cavr.hpp"


// Create camera
std::shared_ptr<GLCamera> camera(new GLCamera("camera"));

// Initialize our program
void initContext() 
{
    // Context Data
    static GLScene *cd;
    cd = new GLScene();
    cd->Create();
    cd->AddToContext(camera);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void frame() 
{
    static GLScene *cd;
    cd = static_cast<GLScene*>(cavr::System::getContextData());
    cd->Event();
    cd->Update();
    shared_ptr<SoundManager> soundMan = cd->Get<SoundManager>("soundMan");
    soundMan->SetListener(camera->getCameraPosition(),camera->GetForward());
}

void render() 
{
    static GLScene *cd;
    cd = static_cast<GLScene*>(cavr::System::getContextData());
    cd->MoveCamera();

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shared_ptr<GLModel> dragon = cd->Get<GLModel>("dragon");
    dragon->setMatrix(glm::translate(glm::mat4(1.0f), Vec3(0.0f, -1.0f, -3.0f)) * glm::scale(glm::mat4(1.0f), Vec3(0.2f, 0.2f, 0.2f)));
    cd->Paint<GLModel>(dragon, GL_TRIANGLES);
    
    camera->updateView();
}

void destructContext() 
{
    //cd.reset();
    static GLScene *cd;
    cd = static_cast<GLScene*>(cavr::System::getContextData());
    delete(cd);
}

void update() 
{
    if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
       cavr::System::shutdown();
      return;
    }

    // Get the emulated sixdof and update its position
    auto headPosition = cavr::input::getSixDOF("glass")->getPosition();
    auto emulated = cavr::input:: getSixDOF("emulated");
    auto emulatedMatrix = emulated->getMatrix();

    // I really wish there was a set position
    emulatedMatrix[3].xyz = headPosition;
    emulated->setState(emulatedMatrix);
    
    if (cavr::input::getButton("up")->delta() == cavr::input::Button::Held) 
        camera->moveCamera(glm::vec2(-4 * cavr::input::InputManager::dt(),0),0);
    else if (cavr::input::getButton("down")->delta() == cavr::input::Button::Held) 
        camera->moveCamera(glm::vec2(4* cavr::input::InputManager::dt(),0),0);
    else if (cavr::input::getButton("left")->delta() == cavr::input::Button::Held)
        camera->moveCamera(glm::vec2(0,-4* cavr::input::InputManager::dt()),0);
    else if (cavr::input::getButton("right")->delta() == cavr::input::Button::Held) 
        camera->moveCamera(glm::vec2(0,4* cavr::input::InputManager::dt()),0);
    else if (cavr::input::getButton("forward")->delta() == cavr::input::Button::Held) 
        camera->moveCamera(glm::vec2(0,0),4* cavr::input::InputManager::dt());
    else if (cavr::input::getButton("backward")->delta() == cavr::input::Button::Held)
        camera->moveCamera(glm::vec2(0,0),-4* cavr::input::InputManager::dt());

    float xVec = 0; // rename these
    float yVec = 0; // rename
    glm::vec2 xyVec;
    float forwardForce = 0;
    xVec = cavr::input::getAnalog("x_vec")->getValue()*10;
    yVec = cavr::input::getAnalog("y_vec")->getValue()*10;
    if (abs(xVec) < .1)
      xVec = 0;
    if (abs(yVec) < .1)
      yVec = 0;
    xVec *= cavr::input::InputManager::dt();
    yVec *= cavr::input::InputManager::dt();
    if(cavr::input::getButton("forwardEnable")->delta() == cavr::input::Button::Open)
    {
      xyVec = glm::vec2(yVec,xVec);
    }
    else
    {
      forwardForce = -yVec;
    }

    camera->moveCamera(xyVec,forwardForce);
    
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

  cavr::input::InputMap input_map;
  if (!setup_map(input_map))
      return -1;
  if (!cavr::System::init(argc, argv, &input_map)) {
    LOG(ERROR) << "Failed to initialize cavr.";
    return -1;
  }

  auto emulated = cavr::input::getSixDOF("glass");
  auto emulatedMatrix = emulated->getMatrix();

  // I really wish there was a set position
  emulatedMatrix[3].xyz = cavr::math::vec3f(0,1,0);
  emulated->setState(emulatedMatrix);

  LOG(INFO) << "Successfully initialized cavr.";
  // Run cavr -- 
  // notice that this will be an infinite loop (cavr::system.cpp)
  LOG(INFO) << "Starting simulation.";
  cavr::System::run();

  LOG(INFO) << "Shutting down.";
  return 0;
}

