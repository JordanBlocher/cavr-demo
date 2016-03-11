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


int setup_map(cavr::input::InputMap &inputMap)
{
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


  return 1;
}


