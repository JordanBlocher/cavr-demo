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
	//texture = std::make_shared<GLTexture>(GLTexture("text",GL_TEXTURE_2D,glyph->bitmap.width,glyph->bitmap.rows,glyph->bitmap.buffer,GL_RED,GL_RED)); 

	//AddQuad(Vec3(-1,1,0),Vec3(1,1,0),Vec3(-1,-1,0),Vec3(1,-1,0));
	//AddMesh();
	//return;
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//texture->Load();

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
	AddQuad(Vec3(-1,1,0),Vec3(1,1,0),Vec3(-1,-1,0),Vec3(1,-1,0));
	AddMesh();

	//AddTexture(texture);
	//AssignTexture(0,0);

	//glyphs['x'] = 0;

	screenPos = glm::vec2(0,-1);
	size = glm::vec2(.1,.1);
	//GLUIElement(name,texture);
	//	LoadChar('l');
	//LoadChar('B');
	//LoadChar('B');
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
	if(glyphs.find(Character) == glyphs.end())
	{



		auto glyph = face->glyph;


		vector<char> buffer;
		for(int i = 0; i <glyph->bitmap.width * glyph->bitmap.rows ; i++ )
		{
			buffer.push_back(glyph->bitmap.buffer[i]);
		}
		buffers[Character] = make_shared<vector<char> > (buffer); 
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		auto temptext = std::make_shared<GLTexture>(GLTexture("a",GL_TEXTURE_2D,glyph->bitmap.width,glyph->bitmap.rows,&((*buffers[Character])[0]),GL_RED,GL_RED));

		temptext->Load();
		AddTexture(temptext);

		glyphs[Character] = textures->size()-1;  
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	}
	AssignTexture(0,glyphs[Character]);
	

}

void GLText::SetChar(char Character)
{
	if(FT_Load_Char(face, Character, FT_LOAD_RENDER))
	{
		cout << "OUCH SOMETHING WENT WRONG HERE!!!" << endl;
	}
}

glm::vec2 GLText::GetAdvance()
{
	auto glyph = face->glyph;
	return glm::vec2(glyph->advance.x/64,glyph->advance.y/64);
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
	size.y = size.y * (float)glyph->bitmap.rows / (float)MaxFontSize;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLUIElement::Draw(ENUM);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	size = tempscale;
	
	//GLPrimitive::Draw(ENUM);

}


