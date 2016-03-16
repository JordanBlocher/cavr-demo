#include <ft2build.h>
#include <string>
#include "GLMesh.hpp"
using std::string;

#include FT_FREETYPE_H

class GLText : public GLMesh
{
	public:
		GLText(const char* name = "GLText",string fontfile="FreeSans/FreeSans.ttf");
	 	~GLText();

	 	void Update();
	 	void Render();
	 	void LoadFont(string Str);
	private:
		FT_Library ft;
		FT_Face face;

};