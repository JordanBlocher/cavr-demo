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

const GLuint NUM_ATTRIBUTES = 4;

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



    // Create sound manager
    shared_ptr<SoundManager> soundMan(new SoundManager("soundMan"));
    soundMan->PlayBgm(0, true, false);
    this->AddToContext(soundMan);

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
        this->AddToContext(GLRibbons);

    const char* colors[] = {"blue", "red", "purple", "white", "yellow", "green"};
    const char* colorpaths[] = {"models/blue.jpg", "models/red.jpg", "models/purple.jpg", "models/white.jpg", "models/yellow.jpg", "models/green.jpg"};
    for (int i=0; i<6; i++)
    {
        shared_ptr<GLTexture> tex(new GLTexture(colors[i], GL_TEXTURE_2D, colorpaths[i]));
        tex->Load();
        GLRibbons->AddTexture(tex);
    }

   this->color = 5; 
}

void GLScene::Paint()
{
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    auto wand = cavr::input:: getSixDOF("wand");
    glm::vec3 wandPos = camera1->RotatePoint(GLMath::vec3ftoGLM(wand->getPosition() )); 
    glm::vec3 paintPos = camera1->getCameraPosition() + wandPos + camera1->RotatePoint(GLMath::vec3ftoGLM(2.0*wand->getForward() )); ;

    //Clear the screen
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Choose Model
    shared_ptr<GLRibbon> GLRibbons = this->Get<GLRibbon>("GLRibbon");
    this->PaintHelper(GLRibbons, GL_TRIANGLES);
      
    shared_ptr<GLModel> brush = this->Get<GLModel>("brush");
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
    brush->setMatrix(glm::translate(glm::mat4(1.0), paintPos) * glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.4f, 1.4f)) * rot);

    this->PaintHelper(brush, GL_TRIANGLES);

    shared_ptr<SoundManager> soundMan = this->Get<SoundManager>("soundMan");
    soundMan->PlayFX(0, GLRibbons->Tail());

}

void GLScene::Event()
{
    shared_ptr<GLCamera> camera1 = this->Get<GLCamera>("camera1");
    auto wand = cavr::input:: getSixDOF("wand");
    
    glm::vec3 wandPos = camera1->RotatePoint(GLMath::vec3ftoGLM(wand->getPosition() )); 
    glm::vec3 paintPos = camera1->getCameraPosition() + wandPos + camera1->RotatePoint(GLMath::vec3ftoGLM(2.0*wand->getForward() )); ;

    shared_ptr<GLModel> brush = this->Get<GLModel>("brush");
    shared_ptr<GLModel> pallet = this->Get<GLModel>("pallet");

    // Paint here
    shared_ptr<GLRibbon> GLRibbons = this->Get<GLRibbon>("GLRibbon");

    if(cavr::input::getButton("clear")->delta() != cavr::input::Button::Open)
    {
       GLRibbons->ClearPoints();
    }

    if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Held)
    {
      //cout << glm::to_string(paintPos) << endl; 
      GLRibbons->AddPoints(paintPos,glm::vec3(1,1,1));
      if(GLRibbons->Size() > 0)
        GLRibbons->AssignTexture(GLRibbons->Size()-1, this->color);
    }
    if(cavr::input::getButton("pallet")->delta() == cavr::input::Button::Held)
    {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rot2 = glm::rotate(glm::mat4(1.0f), (float)(M_PI/3.7), glm::vec3(1.0f, 0.0f, 0.0f));

        shared_ptr<GLModel> pallet = this->Get<GLModel>("pallet");
        pallet->setMatrix(glm::translate(glm::mat4(1.0f), camera1->getCameraPosition()) 
                * glm::scale(glm::mat4(1.0f), glm::vec3(2.4f, 2.4f, 2.4f))* rot *rot2 
                * glm::translate(glm::mat4(1.0f), glm::vec3(-1.4, -3.2, -1.2)));

    	this->PaintHelper(pallet, GL_TRIANGLES);

        glm::vec3 translations[] = {glm::vec3(-0.4, 0.02, -0.9),
                               glm::vec3(0.19, 0.03, -0.8),
                               glm::vec3(-0.36, 0.02, -0.13),
                               glm::vec3(-0.9, 0.02, -0.3),
                               glm::vec3(-0.8, 0.02, 0.2),
                               glm::vec3(-0.5, 0.02, 0.67)};
        const char* colors[] = {"blue", "red", "purple", "white", "yellow", "green"};
        for (int i=0; i<6; i++)
        {
            shared_ptr<GLModel> color = this->Get<GLModel>(colors[i]);
            color->setMatrix(pallet->Matrix() * glm::translate(glm::mat4(1.0f), translations[i]));
            this->PaintHelper(color, GL_TRIANGLES);
        }

        // Ray and bounding sphere
        cavr::gfx::Ray ray = cavr::gfx::Ray(GLMath::GLMtovec3f(wandPos), GLMath::GLMtovec3f(paintPos));
        float dist;
        for (int i=0; i<6; i++)
        {
            shared_ptr<GLModel> color = this->Get<GLModel>(colors[i]);
            cavr::math::mat4f sphere_mat = GLMath::GLMtomat4f(color->Matrix());
            cavr::gfx::Sphere bounding_sphere = cavr::gfx::Sphere(sphere_mat[3].xyz, 0.5);
            if (bounding_sphere.intersect(*(&ray), *(&dist)))
            {
                this->color = i;
            }
        }
    }
    else if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Open && !Break)
    {
        GLRibbons->AddBreak();
    }

    Break = cavr::input::getButton("paint")->delta() == cavr::input::Button::Open;

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
    glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof(Vec4), glm::value_ptr(Vec4(camera1->getCameraPosition(), 1.0f)));
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



