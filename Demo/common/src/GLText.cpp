#include "GLText.h"

// nothing to read here.


GLText::GLText(const char* name,string fontfile) : GLMesh(name, "GLText")
{
	ft = NULL;
	// We only need to have this done once. 
	if( ft == NULL && FT_Init_FreeType(&ft)) {
  		fprintf(stderr, "Could not init freetype library\n");
  		return;
	}

	LoadFont(fontfile.c_str());

	FT_Set_Pixel_Sizes(face, 0, 48);

	if(FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
	  fprintf(stderr, "Could not load character 'X'\n");
	  return;
	}
}

GLText::~GLText() 
{

}

void GLText::Update()
{

}

void GLText::Render()
{


}

void GLText::LoadFont(string Str)
{
	if(FT_New_Face(ft, Str.c_str(), 0, &face)) {
  		fprintf(stderr, "Could not open font\n");
  		return;
	}
}

