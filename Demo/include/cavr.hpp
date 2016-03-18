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


int setup_map(cavr::input::InputMap &input_map)
{
  input_map.button_map["up"] = "keyboard[w]";
  input_map.button_map["down"] = "keyboard[s]";
  input_map.button_map["left"] = "keyboard[a]";
  input_map.button_map["right"] = "keyboard[d]";
  input_map.button_map["forward"] = "keyboard[i]";
  input_map.button_map["backward"] = "keyboard[k]";
  input_map.button_map["paint"] = "keyboard[p]";
  input_map.button_map["exit"] = "keyboard[Escape]";
  input_map.button_map["clear"] = "keyboard[b]";
  
  input_map.button_map["exit"] = "vrpn[WiiMote0[0]]";
  input_map.button_map["pallet"] = "vrpn[WiiMote0[3]]";
  input_map.button_map["paint"] = "vrpn[WiiMote0[4]]";
  input_map.button_map["clear"] = "vrpn[WiiMote0[16]]";
  input_map.button_map["forwardEnable"] = "vrpn[WiiMote0[17]]";// 16 and 17 are Z and c, respectively
  input_map.analog_map["x_vec"] = "vrpn[WiiMote0[21]]"; // analog sticks of the nunchaku
  input_map.analog_map["y_vec"] = "vrpn[WiiMote0[22]]"; // analog sticks of the nunchaku

  input_map.button_map["play"] = "keyboard[m]";
  input_map.button_map["record"] = "keyboard[r]";
  //input_map.button_map["c_button"] = "vrpn[WiiMote0[17]]"; 


  input_map.sixdof_map["wand"] = "vrpn[WiiMote0[0]]";
  //input_map.button_map["pick"] = "vrpn[WiiMote0[3]]";

  input_map.sixdof_map["glass"] = "vrpn[TallGlasses[0]]";
  input_map.sixdof_map["emulated"] = "emulated";


  input_map.button_map["forwardEnable"] = "vrpn[WiiMote0[17]]";// 16 and 17 are Z and c, respectively
  input_map.analog_map["x_vec"] = "vrpn[WiiMote0[21]]"; // analog sticks of the nunchaku
  input_map.analog_map["y_vec"] = "vrpn[WiiMote0[22]]"; // analog sticks of the nunchaku
  //input_map.button_map["c_button"] = "vrpn[WiiMote0[17]]"; 

  input_map.sixdof_map["wand"] = "vrpn[WiiMote0[0]]";
  input_map.sixdof_map["glass"] = "vrpn[ShortGlasses[0]]";
  input_map.sixdof_map["emulated"] = "emulated";

  return 1;
}


