#define CHECKGLERROR if ( glGetError() != GL_NO_ERROR ) cout << __LINE__ << endl;

#include <iostream>
#include <chrono>
#include <csignal>

#include <GLShader.hpp>
#include <GLBufferObject.hpp>
#include <GLEmissive.hpp>
#include <GLRibbon.hpp>
#include <GLScene.hpp>

#include <GLPrimitive.hpp>
#include <GLText.h>

#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include <cavr/gfx/ray.h>
#include <cavr/gfx/sphere.h>

#define FOV 45.0f
#define SENSOR_DISTANCE 0.01f
#define FOCAL_DISTANCE 100.0f

const GLuint NUM_ATTRIBUTES = 5;

using namespace std;

GLScene::GLScene()
{

}

void GLScene::Create()
{
    cout<<"Creating Scene..\n\n";
    PaintOff = cavr::input::getButton("paint")->delta() == cavr::input::Button::Open;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // Create sound manager
    shared_ptr<SoundManager> soundMan(new SoundManager("soundMan"));
    //soundMan->PlayBgm("bgm0", true, false);
    //record = cavr::input::getButton("record")->delta() == cavr::input::Button::Open;
    /// C Major scale here 
    soundMan->AddFx("media/C5.mp3","C5");
    soundMan->AddFx("media/D5.mp3","D5");
    soundMan->AddFx("media/E5.mp3","E5");
    soundMan->AddFx("media/F5.mp3","F5");
    soundMan->AddFx("media/G5.mp3","G5");
    soundMan->AddFx("media/A5.mp3","A5");
    soundMan->AddFx("media/B5.mp3","B5");
    soundMan->AddFx("media/C5.mp3","C6");
    player = SoundPlayer(soundMan);
    player.SetSound(0,"C5");
    player.SetSound(1,"D5");
    player.SetSound(2,"E5");
    player.SetSound(3,"F5");
    player.SetSound(4,"G5");
    player.SetSound(5,"A5");
    player.SetSound(6,"B5");
    player.SetSound(7,"C6");
    this->AddToContext(soundMan);

    // Choose model
    this->AddModel("dragon", "models/dragon.obj");


    this->AddModel("brush", "models/brush.obj");
    this->AddModel("pallet", "models/pallet.obj");
    this->AddModel("blue", "models/blue.obj");
    this->AddModel("red", "models/red.obj");
    this->AddModel("purple", "models/purple.obj");
    this->AddModel("white", "models/white.obj");
    this->AddModel("yellow", "models/yellow.obj");
    this->AddModel("green", "models/green.obj");


    cavr::System::setContextData(this);

    this->InitShaders();

    shared_ptr<GLRibbon> ribbons (new GLRibbon("ribbons")); 
    ribbons->Create(GL_DYNAMIC_DRAW);
    this->AddToContext(ribbons);

    const char* colors[] = {"blue", "red", "purple", "white", "yellow", "green"};
    const char* colorpaths[] = {"models/blue.jpg", "models/red.jpg", "models/purple.jpg", "models/white.jpg", "models/yellow.jpg", "models/green.jpg"};
    for (int i=0; i<6; i++)
    {
        shared_ptr<GLTexture> tex(new GLTexture(colors[i], GL_TEXTURE_2D, colorpaths[i]));
        tex->Load();
        ribbons->AddTexture(tex);
    }

    shared_ptr<GLText> Text(new GLText("text"));
    Text->Create(GL_STATIC_DRAW); 
        this->AddToContext(Text);
    Material mat;
    mat.diffuse = glm::vec4(1,1,1,1);
    Text->AddMaterial(mat);
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
    program->SetAttributeIndex("v_color", 3);

    //Add Program
    if( this->AddProgram(program) )
        this->AddToContext( program );
    
    // Text Program
    //////////
    //Shaders
    shared_ptr<GLShader> vertex2(new GLShader("textvertex.glsl", GL_VERTEX_SHADER, "vshader"));
    shared_ptr<GLShader> fragment2(new GLShader("textfragment.glsl", GL_FRAGMENT_SHADER, "fshader"));

    //Programs
    shared_ptr<GLProgram> textprogram(new GLProgram("textprogram"));
    
    //Add Shaders
    textprogram->AddShader(vertex2); 
    textprogram->AddShader(fragment2); 
    textprogram->SetAttributeIndex("v_position", 0);
    textprogram->SetAttributeIndex("v_normal", 1);
    textprogram->SetAttributeIndex("v_uv", 2);

    //Add Program
    if( this->AddProgram(textprogram) )
        this->AddToContext( textprogram );
    ///////////////

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




   this->color = 5; 
   this->pallet = false;
}

void GLScene::Render()
{
    //shared_ptr<GLModel> dragon = cd->Get<GLModel>("dragon");
    //dragon->setMatrix(glm::translate(glm::mat4(1.0f), Vec3(0.0f, -1.0f, -3.0f)) * glm::scale(glm::mat4(1.0f), Vec3(0.2f, 0.2f, 0.2f)));
    //cd->Paint<GLModel>(dragon, GL_TRIANGLES);
    
    shared_ptr<GLCamera> camera = this->Get<GLCamera>("camera");
    auto wand = cavr::input:: getSixDOF("wand");
    glm::vec3 wandPos = camera->getCameraPosition() + camera->RotatePoint(GLMath::vec3ftoGLM(wand->getPosition() )); 
    glm::vec3 paintPos = wandPos + camera->RotatePoint(GLMath::vec3ftoGLM(2.0*wand->getForward() )); ;

    //Clear the screen
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Choose Model
    shared_ptr<GLRibbon> ribbons = this->Get<GLRibbon>("ribbons");
    this->Paint<GLRibbon>(ribbons, GL_TRIANGLES);
      
    shared_ptr<GLModel> brush = this->Get<GLModel>("brush");
    auto cv = glm::inverse(camera->GetCameraView());
    cv[3][0] = 0;
    cv[3][1] = 0;
    cv[3][2] = 0;
    auto wandMatrix = GLMath::mat4ftoGLM(wand->getMatrix());
    wandMatrix[3][0] = 0;
    wandMatrix[3][1] = 0;
    wandMatrix[3][2] = 0;
    brush->setMatrix( glm::translate(glm::mat4(1.0), paintPos) * glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.4f, 1.4f)) * cv * wandMatrix );//* GLMath::mat4ftoGLM(wand->getMatrix()) * glm::scale(glm::mat4(1.0f), glm::vec3(1.4f, 1.4f, 1.4f)) * rot );//glm::translate(glm::mat4(1.0), GLMath::vec3ftoGLM(wand->getPosition() )) *  );// *rot);
    this->Paint<GLModel>(brush, GL_TRIANGLES);

    shared_ptr<GLModel> pallet = this->Get<GLModel>("pallet");
    if(this->pallet)
    {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), (float)(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rot2 = glm::rotate(glm::mat4(1.0f), (float)(M_PI/3.7), glm::vec3(1.0f, 0.0f, 0.0f));


        pallet->setMatrix(glm::translate(glm::mat4(1.0f), camera->getCameraPosition()) 
                * glm::scale(glm::mat4(1.0f), glm::vec3(2.4f, 2.4f, 2.4f))* rot *rot2 
                * glm::translate(glm::mat4(1.0f), glm::vec3(-.4, -.2, -.2)));

    	this->Paint<GLModel>(pallet, GL_TRIANGLES);

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
            this->Paint<GLModel>(color, GL_TRIANGLES);
        }

        // Ray and bounding sphere
        cavr::gfx::Ray ray = cavr::gfx::Ray(GLMath::GLMtovec3f(wandPos), GLMath::GLMtovec3f(camera->RotatePoint(GLMath::vec3ftoGLM(2.0*wand->getForward()))));
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
    auto Test = this->Get<GLText>("text");
    // translate(desired x, -desired y) *translate(x-1,1-y)* Scale(x,y) -- x range is [0,2] and y range is [-2,0]
    Test->setMatrix(glm::translate(glm::vec3(1,-1,0)) *  glm::translate(glm::vec3(-.8,.8,0)) * glm::scale(glm::vec3(.2,.2,1)));
    this->CustomHelper("textprogram",Test,GL_TRIANGLES);
}

void GLScene::Event()
{
    // recording the player
    player.Update(cavr::input::InputManager::dt());

    // camera and wand code here
    shared_ptr<GLCamera> camera = this->Get<GLCamera>("camera");
    auto wand = cavr::input:: getSixDOF("wand");


    // Paint here
    shared_ptr<GLRibbon> ribbons = this->Get<GLRibbon>("ribbons");
    
    if(cavr::input::getButton("record")->delta() != cavr::input::Button::Open && record)
    {
        cout << "Record" << endl;
        player.ToggleRecord();
    }
    record = cavr::input::getButton("record")->delta() == cavr::input::Button::Open;

    glm::vec3 wandPos = camera->getCameraPosition() +  camera->RotatePoint(GLMath::vec3ftoGLM(wand->getPosition() )); 
    glm::vec3 paintPos = wandPos + camera->RotatePoint(GLMath::vec3ftoGLM(2.0*wand->getForward() )); ;

    if(cavr::input::getButton("pallet")->delta() == cavr::input::Button::Held)
    {
        this->pallet = true; 
    }
    else this->pallet = false;

    if(cavr::input::getButton("clear")->delta() != cavr::input::Button::Open)
    {
       ribbons->ClearPoints();
       player.ClearTracked();
    }

    if(cavr::input::getButton("play")->delta() != cavr::input::Button::Open)
    {
        player.PlaySound();
    }

    if(cavr::input::getButton("paint")->delta() != cavr::input::Button::Open && PaintOff)
    {
        //cout << "PRESSED" << endl;
        //player.AddPoint(paintPos,0);
    }
    else if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Open && !PaintOff)
    {
        //cout << "RELEASED" << endl;
        //player.SetOffPoint();
    }
 
    /*if(cavr::input::getButton("record")->delta() != cavr::input::Button::Open && record)
    {
        cout << "Record" << endl;
        auto soundMan = this->Get<SoundManager>("soundMan");
        soundMan->PlayFX(player.GetSound(6),camera1->getCameraPosition());
        player.ToggleRecord();
    }
    record = cavr::input::getButton("record")->delta() == cavr::input::Button::Open;*/


    PaintOff = cavr::input::getButton("paint")->delta() == cavr::input::Button::Open;
    if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Held)
    {


      //cout << glm::to_string(paintPos) << endl; 
      ribbons->AddPoints(paintPos,glm::vec3(1,1,1));
      if(ribbons->Size() > 0)
      {
        ribbons->AssignTexture(ribbons->Size()-1, this->color);
      }
        player.AddPoint(paintPos,color);
        player.SetOffPoint();
    }
    else if(cavr::input::getButton("paint")->delta() == cavr::input::Button::Open && !Break)
    {
        ribbons->AddBreak();
    }
    Break = cavr::input::getButton("paint")->delta() == cavr::input::Button::Open;


    shared_ptr<SoundManager> soundMan = this->Get<SoundManager>("soundMan");
    //soundMan->SetListener(GLMath::vec3ftoGLM(cavr::input::getSixDOF("glass")->getPosition()));

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
        camera->moveCamera(glm::vec2(0,0), -4* cavr::input::InputManager::dt());

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
    {
        model->Create(GL_STATIC_DRAW);
        this->AddToContext(model);
    }
}



