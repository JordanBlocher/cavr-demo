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
#include <GLRibbon.hpp>

// Using IrrKlang for this example
#include <irrKlang.h>
using namespace irrklang;

static GLCamera::CamDirection CAM_DIRECTION;
static bool PAINT;

//#define DEBUG 

#pragma comment(lib, "irrKlang.lib")
// IrrKlang

// Initialize our program
void initContext() 
{
    // Context Data 
    static GLScene *cd;
    cd = new GLScene();
    cd->InitializeGL();

    // Choose model
    cd->AddModel("dragon", "models/dragon.obj");
    /*cd->addModel("pallet", "models/pallet.obj");
    cd->addModel("pallet", "models/pallet.obj");
    cd->addModel("brush", "models/brush.obj");
    cd->addModel("blob", "models/blob.obj");
    cd->addModel("paint", "models/paint.obj");
    cd->addModel("coords", "models/coords.obj");
    cd->addModel("blue", "models/blue.obj");
    cd->addModel("red", "models/red.obj");
    cd->addModel("purple", "models/purple.obj");
    cd->addModel("white", "models/white.obj");
    cd->addModel("yellow", "models/yellow.obj");
    cd->addModel("green", "models/green.obj");
    */


    // Camera
    CAM_DIRECTION = GLCamera::CamDirection::Nop;
    PAINT = false;

    cavr::System::setContextData(cd);
}

void frame() 
{
    static GLScene *cd;
    cd = static_cast<GLScene*>(cavr::System::getContextData());
    if (cavr::input::getButton("paint")->delta() == cavr::input::Button::Held) 
        PAINT = true;
    else PAINT = false;

    if (cavr::input::getButton("up")->delta() == cavr::input::Button::Held) 
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

    float xVec = 0; // rename these
    float yVec = 0; // rename
    glm::vec2 xyVec;
    float forwardForce = 0;
    xVec = cavr::input::getAnalog("x_vec")->getValue(); //* cavr::input::InputManager::dt();
    yVec = cavr::input::getAnalog("y_vec")->getValue(); //* cavr::input::InputManager::dt();
    if (abs(xVec) < .1)
      xVec = 0;
    if (abs(yVec) < .1)
      yVec = 0;

    if(cavr::input::getButton("forwardEnable")->delta() == cavr::input::Button::Open)
    {
      xyVec = glm::vec2(yVec,xVec);
    }
    else
    {
      forwardForce = -yVec;
    }
    
    cd->Event();

    //std::cout << cd << std::endl;
    shared_ptr<GLCamera> camera1 = cd->Get<GLCamera>("camera1");
    //std::cout << camera1 << std::endl;
    camera1->moveCamera(xyVec,forwardForce);
}

void render() 
{

    static GLScene *cd;
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    // get the context data
    //shared_ptr<GLScene> cd = (shared_ptr<GLScene>)cavr::System::getContextData();
    cd = static_cast<GLScene*>(cavr::System::getContextData());

    //Get view & projection matrices
    shared_ptr<GLCamera> camera1 = cd->Get<GLCamera>("camera1");
    camera1->updateCavrProjection();
    //camera1->updateCavrPosition();
    //camera1->updateCavrView();
    //camera1->updateView();
    cd->MoveCamera(CAM_DIRECTION);
    
    camera1->updateView();
    cd->Paint();

    auto position = cavr::input::getSixDOF("glass")->getPosition();
    position.x *= 10;
    position.z *= 10;
    //cd->engine->setListenerPosition(vec3df(position.x,position.y,position.z), // Listener's position
    //vec3df(0,0,1)); // What direction is the listener's facing directiion -- in this case we are always stareing forward..

}

void destructContext() 
{
    static GLScene *cd;
    cd = static_cast<GLScene*>(cavr::System::getContextData());
    delete cd;
}

void update() 
{
    // shutdown cavr..
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
  input_map.button_map["paint"] = "keyboard[p]";
  input_map.button_map["exit"] = "keyboard[Escape]";
  input_map.button_map["clear"] = "keyboard[b]";
#else
  input_map.button_map["up"] = "vrpn[WiiMote0[10]]";
  input_map.button_map["down"] = "vrpn[WWiiMote0[9]]";
  input_map.button_map["left"] = "vrpn[WWiiMote0[7]]";
  input_map.button_map["right"] = "vrpn[WWiiMote0[8]]";
  input_map.button_map["forward"] = "vrpn[WWiiMote0[4]]";
  //input_map.button_map["backward"] = "vrpn[WWiiMote0[3]]";
  input_map.button_map["exit"] = "vrpn[WiiMote0[0]]";
  input_map.button_map["paint"] = "vrpn[WiiMote0[4]]";
  input_map.button_map["clear"] = "vrpn[WiiMote0[16]]";
#endif


  

  input_map.button_map["forwardEnable"] = "vrpn[WiiMote0[17]]";// 16 and 17 are Z and c, respectively
  input_map.analog_map["x_vec"] = "vrpn[WiiMote0[21]]"; // analog sticks of the nunchaku
  input_map.analog_map["y_vec"] = "vrpn[WiiMote0[22]]"; // analog sticks of the nunchaku

  //input_map.button_map["c_button"] = "vrpn[WiiMote0[17]]"; 


  input_map.sixdof_map["wand"] = "vrpn[WiiMote0[0]]";
  //input_map.button_map["pick"] = "vrpn[WiiMote0[3]]";

  input_map.sixdof_map["glass"] = "vrpn[TallGlasses[0]]";
  input_map.sixdof_map["emulated"] = "emulated";

  
  if (!cavr::System::init(argc, argv, &input_map)) {
    LOG(ERROR) << "Failed to initialize cavr.";
    return -1;
  }
  auto emulated = cavr::input:: getSixDOF("glass");
  auto emulatedMatrix = emulated->getMatrix();

  // I really wish there was a set position
  emulatedMatrix[3].xyz = cavr::math::vec3f(0,1,0);
  emulated->setState(emulatedMatrix);
  LOG(INFO) << "Successfully initialized cavr.";
  LOG(INFO) << "Callbacks set.";
  LOG(INFO) << "Starting simulation.";

  // run cavr -- notice that this will be an infinite loop take a look at cavr::system.cpp
  cavr::System::run();

  LOG(INFO) << "Shutting down.";
  return 0;
}
