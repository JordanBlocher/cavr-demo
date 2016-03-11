#include <GLRibbon.hpp>

#include "GLTexture.hpp"

GLRibbon::GLRibbon(const char* name, long max) : GLPrimitive(name, "GLRibbon", 4, 3*max)
{

}

GLRibbon::~GLRibbon()
{
}

bool GLRibbon::Init()
{
    this->v_size = maxPoints;
    this->e_size = maxPoints*3;

    Create();
}

bool GLRibbon::AddPoints(vec3 worldPoint,vec3 Color)
{

	PaintStruct temp;
	temp.position = worldPoint;
    temp.color = Color;
	temp.Break = false;
	Points.push_back(temp);

	if(Points.size() > 1 && !Points[Points.size()-2].Break)
	{
		// Add the points at the current positionsition in the buffer
		Vec3 one;
		Vec3 two;
		Vec3 three;
		Vec3 four;

		Vec3 oneColor;
		Vec3 twoColor;
		Vec3 threeColor;
		Vec3 fourColor;

		one = Points[Points.size() - 2].position;
		two = Points[Points.size() - 1].position + vec3(0.0,1.0,0.0);
		three = Points[Points.size() - 1].position;
		four = Points[Points.size() - 2].position + vec3(0,1.0,0.0);
		oneColor = Points[Points.size() - 2].color;
		twoColor = Points[Points.size() - 1].color;
		threeColor = Points[Points.size() - 1].color;
		fourColor = Points[Points.size() - 2].color;

        // Set vertex colors
		colors->push_back(fourColor);
		colors->push_back(threeColor);
		colors->push_back(twoColor);
        colors->push_back(fourColor);
        colors->push_back(oneColor);
        colors->push_back(threeColor); 

        AddQuad(four, two, one, three);

		vbo_pos.LoadSubData((Points.size()-2)*6, 0, std::vector<Vec3>(positions->end() - 6, positions->end()));
		vbo_color.LoadSubData((Points.size()-2)*6, 3, std::vector<Vec3>(colors->end() - 6, colors->end()));
        vbo_tex.LoadSubData((Points.size()-2)*6, 2, std::vector<Vec2>(uvs->end() - 6, uvs->end()) );
        vbo_norm.LoadSubData((Points.size()-2)*6, 1, std::vector<Vec3>(normals->end() - 6, normals->end()) );
        vbo_elements.LoadSubData((Points.size()-2)*6, 1, std::vector<Vec3>(faces->end() - 6, faces->end()) );

        AddMesh();
		return true;
	}
	return false;
}

void GLRibbon::AddBreak()
{
	PaintStruct temp;
	temp.Break = true;
	Points.push_back(temp);
}

void GLRibbon::ClearPoints()
{
    Points.clear();
    
    vector<Vec3> clear3 = vector<Vec3> (maxPoints,Vec3(0,0,0.0));
    vector<Vec2> clear2 = vector<Vec2> (maxPoints,Vec2(0,0.0));

    vbo_pos.LoadSubData(0, 0, clear3);
    vbo_color.LoadSubData(0, 3, clear3);
    vbo_norm.LoadSubData(0,1, clear3);
    vbo_tex.LoadSubData(0,2,clear2);
    GLPrimitive::Clear();

}

void GLRibbon::DrawElements(size_t i, GLint face_offset, GLint vertex_offset, GLenum MODE)
{
    glDrawArrays(MODE, vertex_offset, _positions->at(i).size());
}

bool GLRibbon::Create()
{
	cout << "GLRibbon Create" << std::endl;
    //Create the VAO
    glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);

    this->CreateVBOs();

    //Unbind the VAO
    glBindVertexArray(0);

    return true; 
}

// GLRibbon Requires GL_DYNAMIC_DRAW
void GLRibbon::CreateVBOs()
{

    //Create VBOs 
    vbo_pos = GLBufferObject ("vbo_positions",
            sizeof(Vec3),
            this->v_size,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);

    if( this->attributes > V_INDEX)
    {
        glEnableVertexAttribArray(V_INDEX);
        glVertexAttribPointer( V_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    vbo_norm = GLBufferObject ("vbo_normals",
            sizeof(Vec3),
            this->v_size,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);

    if( this->attributes > NORM_INDEX)
    {
        glEnableVertexAttribArray(NORM_INDEX);
        glVertexAttribPointer( NORM_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    vbo_tex = GLBufferObject ("vbo_textures",
            sizeof(Vec2),
            this->v_size,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);


    if( this->attributes > UV_INDEX)
    {
        glEnableVertexAttribArray(UV_INDEX);
        glVertexAttribPointer( UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    vbo_color = GLBufferObject ("vbo_colors",
            sizeof(Vec3),
            this->v_size,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);

    if( this->attributes > COLOR_INDEX)
    {
        glEnableVertexAttribArray(COLOR_INDEX);
        glVertexAttribPointer( COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
}

Vec3 GLRibbon::Tail()
{
    return Points.back().position;
}
