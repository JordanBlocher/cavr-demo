#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include <cavr/gfx/shapes.h>
#include <cavr/gfx/ray.h>
#include <cavr/gfx/sphere.h>
#include <cavr/gl/shader.h>
#include <cavr/gl/vao.h>
#include <cavr/gl/vbo.h>
#include <glog/logging.h>
#include <math.h>
#define GLM_FORCE_RADIANS
#include <GLScene.hpp>

// Using IrrKlang for this example
#include <irrKlang.h>
using namespace irrklang;

static GLCamera::CamDirection CAM_DIRECTION;

#define DEBUG 

#pragma comment(lib, "irrKlang.lib")
// IrrKlang

// Initialize our program
void initContext() 
{
    // Context Data 
    static GLScene *cd;
    cd = new GLScene();
    cd->initializeGL();

    // Choose model
    cd->addModel("dragon", "models/dragon.obj");
    cd->addModel("coords", "models/coords.obj");

    // Camera
    CAM_DIRECTION = GLCamera::CamDirection::Nop;
    cavr::System::setContextData(cd);
}

void frame() 
{

}

void render() 
{
    static GLScene *cd;
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // get the context data
    //shared_ptr<GLScene> cd = (shared_ptr<GLScene>)cavr::System::getContextData();
    cd = static_cast<GLScene*>(cavr::System::getContextData());

    //Get view & projection matrices
    shared_ptr<GLCamera> camera1 = cd->Get<GLCamera>("camera1");
    camera1->updateCavrProjection();
    //camera1->updateCavrPosition();
    //camera1->updateCavrView();
    camera1->updateView();
    cd->moveCamera(CAM_DIRECTION);
    cd->paintGL();

    auto position = cavr::input::getSixDOF("glass")->getPosition();
    position.x *= 10;
    position.z *= 10;
    //cd->engine->setListenerPosition(vec3df(position.x,position.y,position.z), // Listener's position
    //vec3df(0,0,1)); // What direction is the listener's facing directiion -- in this case we are always stareing forward..

}

void destructContext() 
{
    //shared_ptr<GLScene> cd = (shared_ptr<GLScene>)cavr::System::getContextData();
    //cd.reset();
}

void update() 
{
    // shutdown cavr..
    if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
       cavr::System::shutdown();
      return;
    }
    else if (cavr::input::getButton("up")->delta() == cavr::input::Button::Held) 
        CAM_DIRECTION = GLCamera::CamDirection::Up;
    else if (cavr::input::getButton("down")->delta() == cavr::input::Button::Held) 
        CAM_DIRECTION = GLCamera::CamDirection::Down;
    else if (cavr::input::getButton("left")->delta() == cavr::input::Button::Held)
        CAM_DIRECTION = GLCamera::CamDirection::Left;
    else if (cavr::input::getButton("right")->delta() == cavr::input::Button::Held) 
        CAM_DIRECTION = GLCamera::CamDirection::Right;
    else if (cavr::input::getButton("forward")->delta() == cavr::input::Button::Held) 
        CAM_DIRECTION = GLCamera::CamDirection::Forward;
    else if (cavr::input::getButton("backward")->delta() == cavr::input::Button::Held)
        CAM_DIRECTION = GLCamera::CamDirection::Backward;
    else
        CAM_DIRECTION = GLCamera::CamDirection::Nop;
}

int main(int argc, char** argv) 
{
  LOG(INFO) << "Setting callbacks.";

  // cavr is a system of callbacks
  cavr::System::setCallback("update", update);
  cavr::System::setCallback("init_gl_context", initContext);
  cavr::System::setCallback("update_gl_context", frame);
  cavr::System::setCallback("gl_render", render);
  cavr::System::setCallback("destruct_gl_context", destructContext);
  cavr::input::InputMap input_map;

  // set input map for buttons,keyboard, and sixdofs 
#ifdef DEBUG
  input_map.button_map["up"] = "keyboard[w]";
  input_map.button_map["down"] = "keyboard[s]";
  input_map.button_map["left"] = "keyboard[a]";
  input_map.button_map["right"] = "keyboard[d]";
  input_map.button_map["forward"] = "keyboard[i]";
  input_map.button_map["backward"] = "keyboard[k]";
#else
  input_map.button_map["up"] = "WiiMote0[10]";
  input_map.button_map["down"] = "WiiMote0[9]";
  input_map.button_map["left"] = "WiiMote0[7]";
  input_map.button_map["right"] = "WiiMote0[8]";
  input_map.button_map["forward"] = "WiiMote0[4]";
  input_map.button_map["backward"] = "WiiMote0[3]";
#endif
  input_map.button_map["exit"] = "vrpn[WiiMote0[0]]";
  //input_map.sixdof_map["wand"] = "vrpn[WiiMote0[0]]";
  //input_map.button_map["pick"] = "vrpn[WiiMote0[3]]";

  input_map.sixdof_map["glass"] = "vrpn[TallGlasses[0]]";
  
  if (!cavr::System::init(argc, argv, &input_map)) {
    LOG(ERROR) << "Failed to initialize cavr.";
    return -1;
  }

  LOG(INFO) << "Successfully initialized cavr.";
  LOG(INFO) << "Callbacks set.";
  LOG(INFO) << "Starting simulation.";

  // run cavr -- notice that this will be an infinite loop take a look at cavr::system.cpp
  cavr::System::run();

  LOG(INFO) << "Shutting down.";
  return 0;
}
