#include <Spray.hpp>


#include "GLTexture.hpp"

Spray::Spray(const char* name, long max) : GLPrimitive(name, "spray", 4, 6*max)
{
	CurrentPoints = 0;
	vShader = "shaders/defer.vert";
	fShader = "shaders/defer.frag";
}

Spray::~Spray()
{
}

bool Spray::Init()
{
    GLPrimitive::Create();
    this->positions->resize(this->maxPoints);
    this->normals->resize(this->maxPoints);
    this->uvs->resize(this->maxPoints);
}

bool Spray::AddPoints(vec3 worldPoint,vec3 Color)
{
	if (CurrentPoints * 6 < this->maxPoints)
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

void Spray::Draw(GLenum drawmode)
{
    GLint face_offset = 0;
    GLint vertex_offset = 0;
    glBindVertexArray(vao);

    glDrawArrays(drawmode,0,CurrentPoints*6);

    glBindVertexArray(0);
}
