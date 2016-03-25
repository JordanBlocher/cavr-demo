#ifndef GLUIElement_H
#define GLUIElement_H

#include <string>
#include "GLPrimitive.hpp"
using std::string;

class GLUIElement : public GLPrimitive
{
	public:
		GLUIElement(const char* name,shared_ptr<GLTexture> );
		GLUIElement(const char* name);
	 	~GLUIElement();

	 	virtual void Draw(GLenum);
	 	void Update();
	 	void Render();
	 	void SetScreenPos(glm::vec2);
	 	void SetSize(glm::vec2);

	protected:
		shared_ptr<GLTexture> texture;
		glm::vec2 screenPos;
		glm::vec2 size;
};

#endif