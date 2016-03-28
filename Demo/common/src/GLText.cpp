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
	MaxFontSize = 48;

	if(FT_Load_Char(face, 'x', FT_LOAD_RENDER)) {
	  fprintf(stderr, "Could not load character 'X'\n");
	  return;
	}
	
	auto glyph = face->glyph;
	texture = std::make_shared<GLTexture>(GLTexture("text",GL_TEXTURE_2D,glyph->bitmap.width,glyph->bitmap.rows,glyph->bitmap.buffer,GL_RED,GL_RED)); 

	//AddQuad(Vec3(-1,1,0),Vec3(1,1,0),Vec3(-1,-1,0),Vec3(1,-1,0));
	//AddMesh();
	//return;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	texture->Load();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
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
	if(FT_Load_Char(face, Character, FT_LOAD_RENDER))
	{
		cout << "OUCH SOMETHING WENT WRONG HERE!!!" << endl;
	}
	auto glyph = face->glyph;
	//cout << "WIDTH: " << glyph->bitmap.width << endl;
	//cout << &glyph->bitmap.buffer << endl;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	(*textures)[0].SetData(glyph->bitmap.width,glyph->bitmap.rows,glyph->bitmap.buffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
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
	// Need for freetype
	glm::vec2 tempscale = size;
	auto glyph = face->glyph;
	size.x = size.x * (float)glyph->bitmap.width / (float)MaxFontSize;
	size.y = size.y * (float)glyph->bitmap.width / (float)MaxFontSize;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLUIElement::Draw(ENUM);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	size = tempscale;
	
	//GLPrimitive::Draw(ENUM);
}


