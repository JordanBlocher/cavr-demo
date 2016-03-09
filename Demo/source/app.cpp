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

    cavr::System::setContextData(cd);

    cd->InitShaders();

    shared_ptr<GLModel> dragon = cd->Get<GLModel>("dragon");
    dragon->Create(GL_STATIC_DRAW);
    
    shared_ptr<GLRibbon> ribbons = cd->Get<GLRibbon>("ribbons");
    ribbons->Create(GL_DYNAMIC_DRAW);
}

void frame() 
{
    static GLScene *cd;
    cd = static_cast<GLScene*>(cavr::System::getContextData());
    cd->Event();
    cd->Update();
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

  //if (!setup_map(argc, argv))
    //  return -1;
  cavr::input::InputMap input_map;
  // set input map for buttons,keyboard, and sixdofs 
  input_map.button_map["up"] = "keyboard[w]";
  input_map.button_map["down"] = "keyboard[s]";
  input_map.button_map["left"] = "keyboard[a]";
  input_map.button_map["right"] = "keyboard[d]";
  input_map.button_map["forward"] = "keyboard[i]";
  input_map.button_map["backward"] = "keyboard[k]";
  input_map.button_map["paint"] = "keyboard[p]";
  input_map.button_map["exit"] = "keyboard[Escape]";
  input_map.button_map["clear"] = "keyboard[b]";
  input_map.button_map["forwardEnable"] = "vrpn[WiiMote0[17]]";// 16 and 17 are Z and c, respectively
  input_map.analog_map["x_vec"] = "vrpn[WiiMote0[21]]"; // analog sticks of the nunchaku
  input_map.analog_map["y_vec"] = "vrpn[WiiMote0[22]]"; // analog sticks of the nunchaku
  //input_map.button_map["c_button"] = "vrpn[WiiMote0[17]]"; 

  input_map.sixdof_map["wand"] = "vrpn[WiiMote0[0]]";
  input_map.sixdof_map["glass"] = "vrpn[TallGlasses[0]]";
  input_map.sixdof_map["emulated"] = "emulated";

  auto emulated = cavr::input::getSixDOF("glass");
  auto emulatedMatrix = emulated->getMatrix();

  // I really wish there was a set position
  emulatedMatrix[3].xyz = cavr::math::vec3f(0,1,0);
  emulated->setState(emulatedMatrix);

  if (!cavr::System::init(argc, argv, &input_map)) {
    LOG(ERROR) << "Failed to initialize cavr.";
    return 0;
  }
  LOG(INFO) << "Successfully initialized cavr.";
  // Run cavr -- 
  // notice that this will be an infinite loop (cavr::system.cpp)
  LOG(INFO) << "Starting simulation.";
  cavr::System::run();

  LOG(INFO) << "Shutting down.";
  return 0;
}

