#include "GLUIElement.hpp"

// nothing to read here.


GLUIElement::GLUIElement(const char* name,shared_ptr<GLTexture> texture) : GLPrimitive(name, "GLUIElement",5,6)
{

	this->texture = texture;
	AddQuad(Vec3(-1,1,0),Vec3(1,1,0),Vec3(-1,-1,0),Vec3(1,-1,0));
	AddMesh();

	AddTexture(texture);
	AssignTexture(0,0);

	screenPos = glm::vec2(0,-1);
	size = glm::vec2(.1,.1);


}

GLUIElement::GLUIElement(const char* name) : GLPrimitive(name,"GLUIElement",5,6)
{

}

GLUIElement::~GLUIElement() 
{

}

void GLUIElement::Update()
{

}

void GLUIElement::Render()
{


}


void GLUIElement::Draw(GLenum ENUM)
{
	cout << glm::to_string(size) << endl;
	setMatrix(glm::translate(glm::vec3(screenPos.x,screenPos.y,0)) *  glm::translate(glm::vec3(size.x-1,1-size.y,0)) * glm::scale(glm::vec3(size.x,size.y,1)));
	GLPrimitive::Draw(ENUM);
}


void GLUIElement::SetScreenPos(glm::vec2 screenPos)
{
	// scale the screenPos
	glm::vec2 temp = screenPos;
	temp.x = 2*temp.x;
	temp.y = -2*temp.y;
	this->screenPos = temp;
}
	 	
void GLUIElement::SetSize(glm::vec2 size)
{
	this->size =size;
}

