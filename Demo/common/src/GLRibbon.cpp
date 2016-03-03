#include <GLRibbon.hpp>

#include "GLTexture.hpp"

GLRibbon::GLRibbon(const char* name, long max) : GLPrimitive(name, "GLRibbon", 4, 6*max)
{

}

GLRibbon::~GLRibbon()
{
}

bool GLRibbon::Init()
{

    this->positions->resize(this->maxPoints);
    this->normals->resize(this->maxPoints);
    this->colors->resize(this->maxPoints);
    this->uvs->resize(this->maxPoints);
/*
    this->_positions->push_back(*this->positions);
    this->_normals->push_back(*this->normals);
    this->_colors->push_back(*this->colors);
    this->_uvs->push_back(*this->uvs);
*/

	this->v_size += this->positions->size();    
	//GLMesh::AddMesh();
    Create();
}

bool GLRibbon::AddPoints(vec3 worldPoint,vec3 Color)
{

	PaintStruct temp;
	temp.position = worldPoint;
	temp.Break = false;
	Points.push_back(temp);

	if(Points.size() > 1 && !Points[Points.size()-2].Break)
	{
		cout << "ADD TWO" << endl;

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

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> colors;
		positions.push_back(one);
		positions.push_back(three);
		positions.push_back(two);

		colors.push_back(oneColor);
		colors.push_back(threeColor);
		colors.push_back(twoColor);

		positions.push_back(one);
		positions.push_back(two);
		positions.push_back(four); 

		colors.push_back(oneColor);
		colors.push_back(twoColor);
		colors.push_back(fourColor); 

		vbo_pos.LoadSubData((Points.size()-2)*6, 0, (positions) );
		vbo_color.LoadSubData((Points.size()-2)*6, 3, (colors) );

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

}

void GLRibbon::Draw(GLenum drawmode)
{
	//cout << "DRAW" << endl;
    GLint face_offset = 0;
    GLint vertex_offset = 0;
    glBindVertexArray(vao);

    glDrawArrays(drawmode, 0, Points.size() * 6);

    glBindVertexArray(0);
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
