#include <ft2build.h>
#include <string>
#include "GLPrimitive.hpp"
using std::string;

#include FT_FREETYPE_H

class GLText : public GLPrimitive
{
	public:
		GLText(const char* name = "GLText",string fontfile="FreeSans/FreeSans.ttf");
	 	~GLText();

	 	virtual void Draw(GLenum);
	 	void Update();
	 	void Render();
	 	void LoadFont(string Str);
	 	void LoadString(string str);
	 	void LoadChar(char Character);

	private:
		FT_Library ft;
		FT_Face face;
		shared_ptr<GLTexture> texture;
};