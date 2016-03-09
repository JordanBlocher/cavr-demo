#define CHECKGLERROR if ( glGetError() != GL_NO_ERROR ) cout << __LINE__ << endl;

#include <iostream>
#include <chrono>
#include <csignal>

#include <GLShader.hpp>
#include <GLBufferObject.hpp>
#include <GLEmissive.hpp>
#include <GLRibbon.hpp>
#include <GLScene.hpp>
#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include <cavr/gfx/ray.h>
#include <cavr/gfx/sphere.h>

#define FOV 45.0f
#define SENSOR_DISTANCE 0.01f
#define FOCAL_DISTANCE 100.0f

const GLuint NUM_ATTRIBUTES = 4;

using namespace std;

GLScene::GLScene()
{
}

void GLScene::Create()
{
    cout<<"Creating Scene..\n\n";

    // Create sound manager
    shared_ptr<SoundManager> soundMan(new SoundManager("soundMan"));
    soundMan->PlayBgm(0, true, false);
    this->AddToContext(soundMan);

    // Add models
    this->AddModel("dragon", "models/dragon.obj");

    // Init a GLRibbon
    shared_ptr<GLRibbon> ribbons (new GLRibbon("ribbons"));
    ribbons->AddPoints(glm::vec3(0,0,0),glm::vec3(1,1,1));
    ribbons->AddPoints(glm::vec3(0,0,1),glm::vec3(1,0,0));
    ribbons->AddPoints(glm::vec3(1,0,1),glm::vec3(0,0,1));
    ribbons->AddPoints(glm::vec3(0,0,-1),glm::vec3(0,1,0));
    ribbons->AddBreak();
    this->AddToContext(ribbons);

    // Add textures 
    shared_ptr<GLTexture> pebbles(new GLTexture("pebbles", GL_TEXTURE_2D, "models/pebbles.jpg"));
    ribbons->AddTexture(pebbles);
    shared_ptr<GLTexture> leaves(new GLTexture("leaves", GL_TEXTURE_2D, "models/leaves.jpg"));
    ribbons->AddTexture(leaves);

    /*
    shared_ptr<GLPrimitive> primitive(new GLPrimitive("primitive", 6, 10000));
    primitive->AddPlane(1000, 1000, 1, 1);
    this->AddToContext(primitive);
    */
}

void GLScene::InitShaders()
{
    /****** Deep GPU Stuff ******/
    //Shaders
    shared_ptr<GLShader> vertex(new GLShader("vertex.glsl", GL_VERTEX_SHADER, "vshader"));
    shared_ptr<GLShader> fragment(new GLShader("fragment.glsl", GL_FRAGMENT_SHADER, "fshader"));

    //Programs
    shared_ptr<GLProgram> program(new GLProgram("program"));
    
    //Add Shaders
    program->AddShader(vertex); 
    program->AddShader(fragment); 
    program->SetAttributeIndex("v_position", 0);
    program->SetAttributeIndex("v_normal", 1);
    program->SetAttributeIndex("v_uv", 2);

    //Add Program
    if( this->AddProgram(program) )
        this->AddToContext( program );
    
    //Create UBOs 
    cout<<"Matrix UBO"<<endl;
    shared_ptr<GLUniform> vertex_uniform(new GLUniform("GMatrices"));
    vertex_uniform->CreateUBO(program->getId(), 1, GL_STATIC_DRAW);
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
    shared_ptr<GLUniform> control_uniform(new GLUniform("Shader"));
    control_uniform->CreateUBO(program->getId(), 5, GL_STREAM_DRAW);
    this->AddToContext(control_uniform);

    //Set Lighting
    shared_ptr<GLEmissive> emissive(new GLEmissive("lights"));
    this->AddToContext(emissive);

    // Add FBO
    //shared_ptr<GLFrame> fbo(new GLFrame("fbo", 600, 600));
    //this->AddToContext(fbo);
    
    //Add Samplers
    shared_ptr<GLUniform> texture_uniform(new GLUniform("Texture", program->getId(), 1, "i"));
    this->AddToContext(texture_uniform);

    cout<<"Setting UBOS to share.\n";
    //Set UBOs to Share
    program->SetUBO(vertex_uniform);
    program->SetUBO(lights_uniform);
    program->SetUBO(frag_uniform);
    program->SetUBO(eye_uniform);
    program->SetUBO(control_uniform);
    
}

void GLScene::Update()
{
    shared_ptr<GLRibbon> ribbons = this->Get<GLRibbon>("ribbons");

    if(cavr::input::getButton("clear")->delta() != cavr::input::Button::Open)
    {
        ribbons->ClearPoints();
    }
    if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Held)
    {
        ribbons->LoadPoints();
    }
}

void GLScene::Event()
{
    // Paint here
    shared_ptr<GLRibbon> ribbons = this->Get<GLRibbon>("ribbons");
    shared_ptr<SoundManager> soundMan = this->Get<SoundManager>("soundMan");
  
    if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Held)
    {
        auto wand = cavr::input:: getSixDOF("wand");
        shared_ptr<GLCamera> camera = this->Get<GLCamera>("camera");

        auto paintPos = GLMath::vec3ftoGLM(-wand->getForward()) + camera->getCameraPosition() + GLMath::vec3ftoGLM(-wand->getPosition()) ;
        //cout << glm::to_string(paintPos) << endl; 
        ribbons->AddPoints(paintPos,glm::vec3(1,1,1));
        soundMan->PlayFX(0, ribbons->Tail());
    }
    else if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Open && !Break)
    {
        ribbons->AddBreak();
    }

    Break = cavr::input::getButton("paint")->delta() == cavr::input::Button::Open;

    soundMan->SetListener(GLMath::vec3ftoGLM(cavr::input::getSixDOF("glass")->getPosition()));
}

void GLScene::MoveCamera()
{
    std::shared_ptr<GLCamera> camera = this->Get<GLCamera>("camera");

    camera->updateCavrProjection();

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
    
    camera->updateView();
}

void GLScene::LoadGlobalUBOs(Matrices matrices)
{

    shared_ptr<GLCamera> camera = this->Get<GLCamera>("camera");

    // Bind MVP
    shared_ptr<GLUniform> vuniform = this->Get<GLUniform>("GMatrices");
    glBindBuffer(GL_UNIFORM_BUFFER, vuniform->getId());
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(matrices), &matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Bind Lights
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    shared_ptr<GLUniform> luniform = this->Get<GLUniform>("GLights");
    shared_ptr<GLEmissive> emissive = this->Get<GLEmissive>("lights");
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
    shared_ptr<GLUniform> eye = this->Get<GLUniform>("Eye");
    glBindBuffer(GL_UNIFORM_BUFFER, eye->getId());
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(Vec4), glm::value_ptr(Vec4(camera->getCameraPosition(), 1.0f)));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
}


void GLScene::AddModel(const char* name, const char* path)
{
    shared_ptr<GLModel> model(new GLModel(path, name, NUM_ATTRIBUTES));
    if (model->LoadModel())
        this->AddToContext(model);
}



