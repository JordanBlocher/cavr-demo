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

// Using IrrKlang for this example
#include <irrKlang.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")
// IrrKlang


// Context Data used for this example
struct ContextData {
  // Shader Programs
  cavr::gl::Program* simple_program;
  
  // Uniforms
  GLint color_uniform;
  GLint projection_uniform;
  GLint view_uniform;
  GLint model_uniform;
  GLint mvp_uniform;

  // VBOs and
  cavr::gl::VAO* sphere_vao;
  cavr::gl::VBO* sphere_vbo;
  size_t num_triangles_in_sphere;
  cavr::gl::VAO* cylinder_vao;
  cavr::gl::VBO* cylinder_vbo;
  size_t num_triangles_in_cylinder;

  cavr::math::mat4f sphere_matrix;
  bool found;

  // Lets add some sound library stuff here
   ISoundEngine* engine;
   ISound* music; 
};

// Initialize our program
void initContext() {
  ContextData* cd = new ContextData();

  // Initialize some irrklang music
  cd->engine = createIrrKlangDevice();
  cd->music = cd->engine->play3D("media/ghibli.wav",
    vec3df(0,1,0), // Music source position
    true, // play looped
    false, //  start paused
    true); //  enable sound

  cd->simple_program = cavr::gl::Program::createSimple();

  // custom shader initialization
  cavr::gl::VertexShader* vs = cavr::gl::VertexShader::fromFile("shaders/shader.vert");
  if (!vs) {
    LOG(ERROR) << "Failed to load simple vertex shader";
    cavr::System::shutdown();
    return;
  }

  cavr::gl::FragmentShader* fs = cavr::gl::FragmentShader::fromFile("shaders/shader.frag");
  if (!fs) {
    LOG(ERROR) << "Failed to load simple fragment shader";
    cavr::System::shutdown();
    return;
  }

  cd->color_uniform = cd->simple_program->getUniform("color");
  cd->projection_uniform = cd->simple_program->getUniform("projection");
  cd->view_uniform = cd->simple_program->getUniform("view");
  cd->model_uniform = cd->simple_program->getUniform("model");

  // Simple shader shader vbo and vao initialization
  std::vector<cavr::math::vec4f> sphere_vertices = 
    cavr::gfx::Shapes::solidSphere(30, 30);
  cd->num_triangles_in_sphere = sphere_vertices.size();
  cd->sphere_vbo = new cavr::gl::VBO(sphere_vertices);
  cd->sphere_vao = new cavr::gl::VAO();
  cd->sphere_vao->setAttribute(cd->simple_program->getAttribute("in_position"),
                               cd->sphere_vbo,
                               4,
                               GL_FLOAT,
                               0,
                               0, 
                               0);
  std::vector<cavr::math::vec4f> cylinder_vertices = 
    cavr::gfx::Shapes::solidCylinder(15, 10, 0.01);
  cd->num_triangles_in_cylinder = cylinder_vertices.size(); 
  cd->cylinder_vbo = new cavr::gl::VBO(cylinder_vertices);
  cd->cylinder_vao = new cavr::gl::VAO();
  cd->cylinder_vao->setAttribute(cd->simple_program->getAttribute("in_position"),
                               cd->cylinder_vbo,
                               4,
                               GL_FLOAT,
                               0,
                               0,
                               0);
  
  cd->sphere_matrix = cavr::math::mat4f::translate(0.6, 0.6, 0.6) * cavr::math::mat4f::scale(0.2) * cavr::math::mat4f::translate(0, 0, 10) ;
  cd->found = false;

  // set context data
  cavr::System::setContextData(cd);
}

void frame() {
  ContextData* cd = (ContextData*)cavr::System::getContextData();
}

void render() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // get the context data
  ContextData* cd = (ContextData*)cavr::System::getContextData();

  // use the simple program
  cd->simple_program->begin();
  cavr::math::mat4f ray_matrix = cavr::input::getSixDOF("wand")->getMatrix();

  // Check if a button has been pressed
  //if (cavr::input::getButton("pick")->delta() == cavr::input::Button::Held) 
  {
    // Ray and bounding sphere
    auto look = cavr::input::getSixDOF("wand")->getForward();
    auto pos = cavr::input::getSixDOF("wand")->getPosition();
    cavr::gfx::Ray ray = cavr::gfx::Ray(pos, look);
    cavr::gfx::Sphere bounding_sphere = cavr::gfx::Sphere(cd->sphere_matrix[3].xyz, 0.3);
    float dist;
    if (bounding_sphere.intersect(*(&ray), *(&dist)))
    {
        cd->found = true;
        cd->sphere_matrix = ray_matrix * cavr::math::mat4f::translate(0, 0, 10) * cavr::math::mat4f::scale(0.2);
    }

  } 

  // Cylinder
  //if (ray_matrix)
  {
      cd->cylinder_vao->bind();
      glUniformMatrix4fv(cd->projection_uniform, 1, GL_FALSE, cavr::gfx::getProjection().v);
      glUniformMatrix4fv(cd->view_uniform, 1, GL_FALSE, cavr::gfx::getView().v);
      glUniform3f(cd->color_uniform, 0, 1, 1);
      glUniformMatrix4fv(cd->model_uniform, 1, GL_FALSE, ray_matrix.v);
      glDrawArrays(GL_TRIANGLES, 0, cd->num_triangles_in_cylinder);
      glBindVertexArray(0); 
  }
  
  // Sphere
  cd->sphere_vao->bind();
  glUniformMatrix4fv(cd->projection_uniform, 1, GL_FALSE, cavr::gfx::getProjection().v);
  glUniformMatrix4fv(cd->view_uniform, 1, GL_FALSE, cavr::gfx::getView().v);
  if (cd->found)
    glUniform3f(cd->color_uniform, 0, 1, 1);
  else
    glUniform3f(cd->color_uniform, 0, 0, 1);
  glUniformMatrix4fv(cd->model_uniform, 1, GL_FALSE, cd->sphere_matrix.v);
  glDrawArrays(GL_TRIANGLES, 0, cd->num_triangles_in_sphere);
  glBindVertexArray(0);

  cd->simple_program->end();  

  // Set your current 
  auto position = cavr::input::getSixDOF("glass")->getPosition();
  position.x *= 10;
  position.z *= 10;

  cd->engine->setListenerPosition(vec3df(position.x,position.y,position.z), // Listener's position
  vec3df(0,0,1)); // What direction is the listener's facing directiion -- in this case we are always stareing forward..

}

void destructContext() {
  ContextData* cd = (ContextData*)cavr::System::getContextData();
  delete cd;
}

void update() {
  // shutdown cavr..
  if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
    cavr::System::shutdown();
    return;
  }

}

int main(int argc, char** argv) {
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
