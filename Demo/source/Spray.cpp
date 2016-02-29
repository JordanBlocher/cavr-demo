#include <Spray.hpp>


#include "GLTexture.hpp"

Spray::Spray(int max,std::string name) :GLNode(name.c_str())
{
	MaxPoints = 6*max;
	CurrentPoints = 0;
	vShader = "shaders/defer.vert";
	fShader = "shaders/defer.frag";
	matrix = glm::mat4(1);
}

bool Spray::Init()
{
	std::cout << "SUCCESSFUL" << std::endl;

	//Create VBOs

	glGenVertexArrays(1, &(this->vao));
    glBindVertexArray(this->vao);
    
	// Allocate Positions 



    //Allocate uvs



    //Allocate Normals


    //Allocate Colors

    /*
	const GLuint V_INDEX = 0;
	const GLuint NORM_INDEX = 1;
	const GLuint UV_INDEX = 2;
	const GLuint COLOR_INDEX = 3;*/
	     vbo_pos = GLBufferObject("vbopositions",
            sizeof(glm::vec3),
            MaxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(V_INDEX);
    glVertexAttribPointer( V_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);


     vbo_norm = GLBufferObject("vbonormals",
            sizeof(glm::vec3),
            MaxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(NORM_INDEX);
    glVertexAttribPointer( NORM_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);


     vbo_tex = GLBufferObject("vbotextures",
            sizeof(glm::vec2),
            MaxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(UV_INDEX);
    glVertexAttribPointer( UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);

    vbo_color = GLBufferObject("vbocolors",
            sizeof(glm::vec3),
            MaxPoints,
            GL_ARRAY_BUFFER,
            GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(COLOR_INDEX);
    glVertexAttribPointer( COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	return true;
}

bool Spray::AddPoints(vec3 worldPoint,vec3 Color)
{
	if (CurrentPoints * 6 < MaxPoints)
	{
		PaintStruct temp;
		temp.position = worldPoint;
		temp.color = Color;
		temp.Break = false;
		Points.push_back(temp);
		CurrentPoints++;
	}
	if(Points.size() > 1 && !Points[Points.size()-2].Break)
	{
		

		// Add the points at the current position in the buffer
		vector<glm::vec3> pos = vector<glm::vec3>();
		std::vector<glm::vec3> colors = std::vector<glm::vec3>();
		glm::vec3 one;
		glm::vec3 two;
		glm::vec3 three;
		glm::vec3 four;

		glm::vec3 oneColor;
		glm::vec3 twoColor;
		glm::vec3 threeColor;
		glm::vec3 fourColor;

		one = Points[Points.size() - 2].position;
		two = Points[Points.size() - 1].position + vec3(0.0,1.0,0.0);
		three = Points[Points.size() - 1].position;
		four = Points[Points.size() - 2].position + vec3(0,1.0,0.0);
		oneColor = Points[Points.size() - 2].color;
		twoColor = Points[Points.size() - 1].color;
		threeColor = Points[Points.size() - 1].color;
		fourColor = Points[Points.size() - 2].color;





		pos.push_back(one);
		pos.push_back(three);
		pos.push_back(two);

		colors.push_back(oneColor);
		colors.push_back(threeColor);
		colors.push_back(twoColor);

		pos.push_back(one);
		pos.push_back(two);
		pos.push_back(four); 

		colors.push_back(oneColor);
		colors.push_back(twoColor);
		colors.push_back(fourColor); 

		vbo_pos.LoadSubData((CurrentPoints-2)*6, 0, (pos) );
		vbo_color.LoadSubData((CurrentPoints-2)*6, 3, (colors) );

		return true;
	}
	return false;
}

void Spray::AddBreak()
{
	
	CurrentPoints++;
	PaintStruct temp;
	temp.Break = true;
	Points.push_back(temp);
}

void Spray::ClearPoints()
{
	CurrentPoints = 0;
}

void Spray::Update()
{

}

void Spray::Draw(std::shared_ptr<GLUniform> fragment, GLuint program, GLenum drawmode,int)
{
    GLint face_offset = 0;
    GLint vertex_offset = 0;
    glBindVertexArray(vao);
    glBindBuffer(GL_UNIFORM_BUFFER, fragment->getId());

    glDrawArrays(drawmode,0,48);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
