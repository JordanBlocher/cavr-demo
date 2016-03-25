#include "GLText.h"

// nothing to read here.


GLText::GLText(const char* name,string fontfile)  : GLUIElement(name)
{
	cout << fontfile << endl;
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
	
	auto glyph = face->glyph;
	texture = std::make_shared<GLTexture>(GLTexture("text",GL_TEXTURE_2D,glyph->bitmap.width,glyph->bitmap.rows,glyph->bitmap.buffer,GL_RED,GL_RED)); 

	//AddQuad(Vec3(-1,1,0),Vec3(1,1,0),Vec3(-1,-1,0),Vec3(1,-1,0));
	//AddMesh();
	//return;
	texture->Load();
	
	AddQuad(Vec3(-1,1,0),Vec3(1,1,0),Vec3(-1,-1,0),Vec3(1,-1,0));
	AddMesh();

	AddTexture(texture);
	AssignTexture(0,0);

	screenPos = glm::vec2(0,-1);
	size = glm::vec2(.1,.1);
	//GLUIElement(name,texture);
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

void GLText::Draw(GLenum ENUM)
{
	GLUIElement::Draw(ENUM);
	//GLPrimitive::Draw(ENUM);
}


