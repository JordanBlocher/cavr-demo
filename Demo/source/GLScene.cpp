#define CHECKGLERROR if ( glGetError() != GL_NO_ERROR ) cout << __LINE__ << endl;
#define GLM_FORCE_RADIANS

#include <GL/glew.h>

#include <iostream>
#include <chrono>
#include <csignal>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include <glm/virtrev/xstream.hpp>

#include <GLShader.hpp>
#include <GLProgram.hpp>
#include <GLBufferObject.hpp>
#include <GLFrame.hpp>
#include <GLModel.hpp>
#include <GLUniform.hpp>
#include <GLEmissive.hpp>
#include <GLMath.hpp>

#include <GLScene.hpp>
#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include <cavr/gfx/ray.h>
#include <cavr/gfx/sphere.h>

#define FOV 45.0f
#define SENSOR_DISTANCE 0.01f
#define FOCAL_DISTANCE 100.0f

const GLuint NUM_ATTRIBUTES = 3;

using namespace std;

GLScene::GLScene()
{
    // Start the timer
    this->time = std::chrono::high_resolution_clock::now();
    GLViewport::start_time = std::chrono::high_resolution_clock::now();  
}

void GLScene::initializeGL()
{
    GLViewport::initializeGL();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create camera
    std::shared_ptr<GLCamera> camera(new GLCamera("camera1"));
    this->AddToContext(camera);

    // Create sound manager
    //shared_ptr<SoundManager> soundMan(new SoundManager("soundMan"));
    //soundMan->playBgm(0);
    //this->AddToContext(soundMan);

       /****** Deep GPU Stuff ******/
    //Shaders
    shared_ptr<GLShader> vertex(new GLShader("tvertex.glsl", GL_VERTEX_SHADER, "vshader"));
    shared_ptr<GLShader> fragment(new GLShader("tfragment.glsl", GL_FRAGMENT_SHADER, "fshader"));

    //Programs
    shared_ptr<GLProgram> program(new GLProgram("program"));
    
    //Add Shaders
    program->AddShader(vertex); 
    program->AddShader(fragment); 

    //Add Program
    if( this->AddProgram(program) )
        this->AddToContext( program );
    
    //Create UBOs 
    cout<<"Matrix UBO"<<endl;
    shared_ptr<GLUniform> vertex_uniform(new GLUniform("GMatrices"));
    vertex_uniform->CreateUBO(program->getId(), 1, GL_STATIC_DRAW);
    const char* colors[] = {"blue", "red", "black", "white", "yellow", "green"};
    this->AddToContext(vertex_uniform);
    
    cout<<"Color UBO"<<endl;
    shared_ptr<GLUniform> frag_uniform(new GLUniform("GColors"));
    frag_uniform->CreateUBO(program->getId(), 2, GL_STREAM_DRAW);
    this->AddToContext(frag_uniform);
    cout<<"Colors index " <<glGetUniformBlockIndex(program->getId(), "GColors")<<endl;

    cout<<"Lights UBO"<<endl;
    shared_ptr<GLUniform> lights_uniform(new GLUniform("GLights"));
    lights_uniform->CreateUBO(program->getId(), 3, GL_STREAM_DRAW);
    this->AddToContext(lights_uniform);

    cout<<"Eye UBO"<<endl;
    shared_ptr<GLUniform> eye_uniform(new GLUniform("Eye"));
    eye_uniform->CreateUBO(program->getId(), 4, GL_STREAM_DRAW);
    this->AddToContext(eye_uniform);

    cout<<"Control UBO"<<endl;
    shared_ptr<GLUniform> control_uniform(new GLUniform("Control"));
    control_uniform->CreateUBO(program->getId(), 5, GL_STREAM_DRAW);
    this->AddToContext(control_uniform);

    //Add Samplers
    shared_ptr<GLUniform> texture_uniform(new GLUniform("Texture", program->getId(), 1, "i"));
    this->AddToContext(texture_uniform);


    //Set UBOs to Share
    program->SetUBO(vertex_uniform);
    program->SetUBO(lights_uniform);
    program->SetUBO(frag_uniform);
    program->SetUBO(eye_uniform);
    program->SetUBO(control_uniform);

    //Set Lighting
    shared_ptr<GLEmissive> emissive(new GLEmissive("lights"));
    this->AddToContext(emissive);

    // Add FBO
    shared_ptr<GLFrame> fbo(new GLFrame("fbo", 600, 600));
    this->AddToContext(fbo);

    this->color = glm::vec4(1,0,0,1);
}

void GLScene::paintGL(bool painting)
{
    //Clear the screen
    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    const char* colors[] = {"blue", "red", "purple", "white", "yellow", "green"};
    glm::vec3 translations[] = {glm::vec3(-0.4, 0.02, -0.9),
                               glm::vec3(0.19, 0.02, -0.8),
                               glm::vec3(-0.36, 0.02, -0.13),
                               glm::vec3(-0.9, 0.02, -0.3),
                               glm::vec3(-0.8, 0.02, 0.2),
                               glm::vec3(-0.5, 0.02, 0.67)};
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 wand = GLMath::mat4ftoGLM( cavr::input::getSixDOF("wand")->getMatrix());
    glm::vec3 wand_pos = GLMath::vec3ftoGLM( cavr::input::getSixDOF("wand")->getPosition());
    glm::mat4 glass = GLMath::mat4ftoGLM( cavr::input::getSixDOF("glass")->getMatrix());
    glm::vec3 glass_pos = GLMath::vec3ftoGLM( cavr::input::getSixDOF("glass")->getPosition());

    shared_ptr<GLModel> brush = this->Get<GLModel>("brush");
    shared_ptr<GLModel> blob = this->Get<GLModel>("blob");
    shared_ptr<GLModel> paint = this->Get<GLModel>("paint");
    shared_ptr<GLModel> pallet = this->Get<GLModel>("pallet");
    shared_ptr<GLModel> redsphere = this->Get<GLModel>("redsphere");

    // Pallet
    pallet->setMatrix(glm::translate(glm::mat4(1.0f), glass_pos) * glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.4f, 1.4f))* rot * glm::translate(glm::mat4(1.0f), glm::vec3(-0.89, -0.48, 0.7)));
    this->paintHelper("pallet", GL_TRIANGLES);
    for (int i=0; i<6; i++)
    {
        shared_ptr<GLModel> color = this->Get<GLModel>(colors[i]);
        color->setMatrix(pallet->Matrix() * glm::translate(glm::mat4(1.0f), translations[i]));
        this->paintHelper(colors[i], GL_TRIANGLES);
    }
    // Brush
    float weight = (glm::distance2(brush->Position(),
            glm::vec4(wand_pos.x, wand_pos.y, wand_pos.z, 0.0f)));  
    float distX = brush->Position().x - wand_pos.x;
    float distY = brush->Position().y - wand_pos.y;
    float distZ = brush->Position().z - wand_pos.z;  
    float shear[16] = {70*weight*distX, distX, 0, 0,
                       distY, 70*weight*distY, 0, 0,
                       0, 0, weight, 0,
                       0, 0, 0, 1};
    brush->setMatrix(wand * glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.4f, 1.4f)) * rot);
    blob->setMatrix(brush->Matrix());//* glm::translate(glm::mat4(1.0f), glm::vec3(-0.18, -0.25, -0.41)));
    paint->setMatrix(blob->Matrix());//* glm::translate(glm::mat4(1.0f), glm::vec3(0.04, -0.007, 0.035)));

    if(painting)
    {
        this->strokes.push_back(std::pair<glm::vec4, glm::mat4>(this->color, wand*glm::make_mat4(shear))); 
    }    
    else
    {
        // Ray and bounding sphere
        auto look = cavr::input::getSixDOF("wand")->getForward();
        auto pos = GLMath::GLMtovec3f(blob->Position());
        cavr::gfx::Ray ray = cavr::gfx::Ray(pos, look);
        float dist;
        for (int i=0; i<6; i++)
        {
            shared_ptr<GLModel> color = this->Get<GLModel>(colors[i]);
            cavr::math::mat4f sphere_mat = GLMath::GLMtomat4f(color->Matrix());
            cavr::gfx::Sphere bounding_sphere = cavr::gfx::Sphere(sphere_mat[3].xyz, 0.1);
            if (bounding_sphere.intersect(*(&ray), *(&dist)))
            {
                this->color = color->getColor();
                blob->setColor(this->color);
            }
        }
    }

    this->paintHelper("brush", GL_TRIANGLES);
    this->paintHelper("blob", GL_TRIANGLES);

    for (int i=0; i<this->strokes.size(); i++)
    {
        this->textures->resize(this->textures->size() + 1 + 1);
        paint->setColor(this->strokes.at(i).first);
        paint->setMatrix(this->strokes.at(i).second);
        this->paintHelper("paint", GL_TRIANGLES);
    }
    */

    shared_ptr<GLModel> dragon = this->Get<GLModel>("dragon");
    dragon->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -3.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
   
    this->paintHelper("dragon", GL_TRIANGLES);
}


void GLScene::paintHelper(const char* model_name, GLenum MODE)
{
    shared_ptr<GLModel> model = this->Get<GLModel>(model_name);
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    glm::mat4 vp = camera1->Projection() * camera1->View() *GLMath::mat4ftoGLM(cavr::gfx::getView());
    //glm::mat4 vp = GLMath::mat4ftoGLM(cavr::gfx::getView());
    
     
    // Get UBOS
    shared_ptr<GLUniform> vuniform = this->Get<GLUniform>("GMatrices");
    shared_ptr<GLUniform> cuniform = this->Get<GLUniform>("GColors");
    shared_ptr<GLUniform> luniform = this->Get<GLUniform>("GLights");
    shared_ptr<GLUniform> eye = this->Get<GLUniform>("Eye");
    shared_ptr<GLUniform> control = this->Get<GLUniform>("Control");
   
    // Get ProgramsS
    shared_ptr<GLProgram> program = this->Get<GLProgram>("program");

    // Get Lights
    shared_ptr<GLEmissive> emissive = this->Get<GLEmissive>("lights");

    // Bind MVP
    Matrices matrices;
    
    //model->setMatrix(camera1->View()*model->Matrix());
    matrices.mvpMatrix = vp * model->Matrix();
    matrices.mvMatrix = model->Matrix(); 
    matrices.normalMatrix = glm::transpose(glm::inverse(model->Matrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, vuniform->getId());
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(matrices), &matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Bind Lights
    glBindBuffer(GL_UNIFORM_BUFFER, luniform->getId());
    size_t baseSize = sizeof(emissive->lights.basic);
    size_t ptSize = sizeof(emissive->lights.point[0]);
    size_t sptSize = sizeof(emissive->lights.spot[0]); 
    glBufferSubData( GL_UNIFORM_BUFFER, 0, baseSize, &(emissive->lights.basic));
    glBufferSubData( GL_UNIFORM_BUFFER, baseSize + 8, ptSize, &(emissive->lights.point[0]));
    for(int j=0; j<6; j++)
    {
        glBufferSubData( GL_UNIFORM_BUFFER, baseSize + ptSize + j*sptSize + 8*(j+2), sptSize, &(emissive->lights.spot[j]));
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Bind Eye Position
    glBindBuffer(GL_UNIFORM_BUFFER, eye->getId());
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(glm::vec4(camera1->getCameraPosition(), 1.0f)));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    int texture = true, bump = false;
    glBindBuffer(GL_UNIFORM_BUFFER, control->getId());
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(int), &texture);
    glBufferSubData( GL_UNIFORM_BUFFER, sizeof(int), sizeof(int), &bump);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Get Sampler
    shared_ptr<GLUniform> uniform = this->Get<GLUniform>("Texture");

    //Program
    glUseProgram(program->getId());
    model->Draw(cuniform, program->getId(), MODE);
    model->Draw(uniform, program->getId(), MODE);
    glUseProgram(0);

}

void GLScene::moveCamera(GLCamera::CamDirection direction)
{
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    camera1->moveCamera(direction);
}


void GLScene::idleGL()
{
    // Timer
    float dt;
    time = chrono::high_resolution_clock::now();
    dt = chrono::duration_cast< std::chrono::duration<float> >(time-this->start_time).count();
    this->start_time = chrono::high_resolution_clock::now();

}

float GLScene::getDT()
{
    float ret;
    time = chrono::high_resolution_clock::now();
    ret = chrono::duration_cast< std::chrono::duration<float> >(time-this->start_time).count();
    this->start_time = chrono::high_resolution_clock::now();
    return ret;
}


void GLScene::addModel(const char* name, const char* path)
{
    shared_ptr<GLModel> model(new GLModel(path, name, NUM_ATTRIBUTES));
    if( model->CreateVAO() )
        this->AddToContext(model);
}



