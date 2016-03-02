#define CHECKGLERROR if ( glGetError() != GL_NO_ERROR ) cout << __LINE__ << endl;

#include <GL/glew.h>

#include <iostream>
#include <chrono>
#include <csignal>

#include <GLShader.hpp>
#include <GLBufferObject.hpp>
#include <GLEmissive.hpp>
#include <GLRibbon.hpp>
#include <GLScene.hpp>
#include <GLPrimitive.hpp>
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

void GLScene::InitializeGL()
{
    GLViewport::InitializeGL();

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
    shared_ptr<GLShader> vertex(new GLShader("vertex.glsl", GL_VERTEX_SHADER, "vshader"));
    shared_ptr<GLShader> fragment(new GLShader("fragment.glsl", GL_FRAGMENT_SHADER, "fshader"));

    //Programs
    shared_ptr<GLProgram> program(new GLProgram("program"));
    
    //Add Shaders
    program->AddShader(vertex); 
    program->AddShader(fragment); 
    program->SetAttributeIndex("/v_position", 0);
    program->SetAttributeIndex("v_normal", 1);
    program->SetAttributeIndex("v_uv", 2);

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

    //Set Lighting
    shared_ptr<GLEmissive> emissive(new GLEmissive("lights"));
    this->AddToContext(emissive);

    // Add FBO
    //shared_ptr<GLFrame> fbo(new GLFrame("fbo", 600, 600));
    //this->AddToContext(fbo);
    //Add Samplers
    shared_ptr<GLUniform> texture_uniform(new GLUniform("Texture", program->getId(), 1, "i"));
    this->AddToContext(texture_uniform);


    //Set UBOs to Share
    program->SetUBO(vertex_uniform);
    program->SetUBO(lights_uniform);
    program->SetUBO(frag_uniform);
    program->SetUBO(eye_uniform);
    program->SetUBO(control_uniform);

    // Init a GLRibbon
    shared_ptr<GLRibbon> GLRibbons (new GLRibbon());
    cout << "NEW GLRibbon" << endl;
    if(GLRibbons->Init())
    {
        cout << "NEW GLRibbon INSIDE" << endl;
        GLRibbons->AddPoints(glm::vec3(0,0,0),glm::vec3(1,1,1));
        GLRibbons->AddPoints(glm::vec3(0,0,1),glm::vec3(1,1,1));
        GLRibbons->AddPoints(glm::vec3(1,0,1),glm::vec3(1,1,1));
        GLRibbons->AddPoints(glm::vec3(0,0,-1),glm::vec3(1,1,1));
        this->AddToContext(GLRibbons);
    }

    shared_ptr<GLTexture> tex(new GLTexture("gold", GL_TEXTURE_2D, "models/gold.jpg"));
    tex->Load();
    shared_ptr<GLPrimitive> primitive(new GLPrimitive("primitive", 6, 10000));
    primitive->AddPlane(1000, 1000, 1, 1);
    primitive->SetColor(Vec3(0,0,1));
    primitive->AddUVSphere(20, 20);
    primitive->AddTexture(tex);
    primitive->SetColor(Vec3(1,0,0));
    primitive->Create();
    this->AddToContext(primitive);

    
}

void GLScene::Paint()
{
    //Clear the screen
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Choose Model
    shared_ptr<GLRibbon> GLRibbons = this->Get<GLRibbon>("GLRibbon");
    this->PaintHelper(GLRibbons, GL_TRIANGLES);
    
    shared_ptr<GLModel> dragon = this->Get<GLModel>("dragon");
    dragon->setMatrix(glm::translate(glm::mat4(1.0f), Vec3(0.0f, -1.0f, -3.0f)) * glm::scale(glm::mat4(1.0f), Vec3(0.2f, 0.2f, 0.2f)));
    //this->PaintHelper(dragon, GL_TRIANGLES);

    shared_ptr<GLPrimitive> primitive = this->Get<GLPrimitive>("primitive");
    //this->PaintHelper(primitive, GL_TRIANGLES);
}

void GLScene::LoadGlobalUBOs(Matrices matrices)
{

    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");

    // Bind MVP
    shared_ptr<GLUniform> vuniform = this->Get<GLUniform>("GMatrices");
    glBindBuffer(GL_UNIFORM_BUFFER, vuniform->getId());
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(matrices), &matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Bind Lights
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
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(Vec4), glm::value_ptr(Vec4(camera1->getCameraPosition(), 1.0f)));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    // Bind Controls
    shared_ptr<GLUniform> control = this->Get<GLUniform>("Control");
    int texture = true, bump = false;
    glBindBuffer(GL_UNIFORM_BUFFER, control->getId());
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(int), &texture);
    glBufferSubData( GL_UNIFORM_BUFFER, sizeof(int), sizeof(int), &bump);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void GLScene::MoveCamera(GLCamera::CamDirection direction)
{
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    camera1->moveCamera(direction);
}


void GLScene::AddModel(const char* name, const char* path)
{
    shared_ptr<GLModel> model(new GLModel(path, name, NUM_ATTRIBUTES));
    if( model->Create() )
        this->AddToContext(model);
}



