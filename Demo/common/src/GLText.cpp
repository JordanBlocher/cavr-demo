#include "GLText.h"

// nothing to read here.


GLText::GLText(const char* name,string fontfile) : GLPrimitive(name, "GLText",4,6)
{
	ft = NULL;
	// We only need to have this done once. 
	if( ft == NULL && FT_Init_FreeType(&ft)) {
  		fprintf(stderr, "Could not init freetype library\n");
  		return;
	}

	LoadFont(fontfile.c_str());

	FT_Set_Pixel_Sizes(face, 0, 48);

	if(FT_Load_Char(face, 'a', FT_LOAD_RENDER)) {
	  fprintf(stderr, "Could not load character 'X'\n");
	  return;
	}
	texture = std::make_shared<GLTexture>(GLTexture()); 
	auto glyph = face->glyph;
	texture->Load(glyph->bitmap.width,glyph->bitmap.rows,glyph->bitmap.buffer);

	AddQuad(Vec3(0,1,0),Vec3(1,1,0),Vec3(0,0,0),Vec3(1,0,0));
	AddMesh();

	AddTexture(texture);
	AssignTexture(0,0);
}

GLText::~GLText() 
{

}

void GLText::LoadChar(char Character)
{
	FT_Load_Char(face, Character, FT_LOAD_RENDER);
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

void GLText::LoadString(string str)
{

}

void GLText::Draw(GLenum ENUM)
{
	
	GLPrimitive::Draw(ENUM);
}

