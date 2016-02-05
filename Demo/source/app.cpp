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
#include <GLScene.hpp>

// Using IrrKlang for this example
#include <irrKlang.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")
// IrrKlang


// Context Data used for this example
struct ContextData 
{

  GLScene *glView = new GLScene();

};

// Initialize our program
void initContext() 
{
  ContextData* cd = new ContextData();

  cd->glView->initializeGL();
  // Choose model
  cd->glView->setModel("dragon");
    
  cavr::System::setContextData(cd);
}

void frame() 
{
  ContextData* cd = (ContextData*)cavr::System::getContextData();
}

void render() 
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // get the context data
  ContextData* cd = (ContextData*)cavr::System::getContextData();

  cd->glView->paintGL();

  // Set your current 
  auto position = cavr::input::getSixDOF("glass")->getPosition();
  position.x *= 10;
  position.z *= 10;

  //cd->engine->setListenerPosition(vec3df(position.x,position.y,position.z), // Listener's position
  //vec3df(0,0,1)); // What direction is the listener's facing directiion -- in this case we are always stareing forward..

}

void destructContext() 
{
  ContextData* cd = (ContextData*)cavr::System::getContextData();
  delete cd;
}

void update() 
{
  // shutdown cavr..
  if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
    cavr::System::shutdown();
    return;
  }

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

  input_map.button_map["exit"] = "vrpn[WiiMote0[0]]";
  input_map.sixdof_map["wand"] = "vrpn[WiiMote[0]]";
  input_map.button_map["pick"] = "vrpn[WiiMote0[3]]";

  input_map.sixdof_map["glass"] = "vrpn[TallGlass[0]]";
  
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
