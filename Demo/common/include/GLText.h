#ifndef GLText_H
#define GLText_H

#include <ft2build.h>
#include <string>
#include <map>
#include "GLPrimitive.hpp"
#include "GLUIElement.hpp"
using std::string;
using std::map;

#include FT_FREETYPE_H


class GLText : public GLUIElement
{
	public:
		GLText(const char* name = "GLText",string fontfile="FreeSans/FreeSans.ttf");
	 	~GLText();

	 	virtual void Draw(GLenum);
	 	void Update();
	 	void Render();
	 	void LoadFont(string Str);
	 	void LoadChar(char Character);
	 	void SetChar(char Character);
	 	glm::vec2 GetAdvance();
	private:
		FT_Library ft;
		FT_Face face;
		int MaxFontSize;
		map<char,int> glyphs;
		map<char,shared_ptr<vector<char> > > buffers;
};

#endif