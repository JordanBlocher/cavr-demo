/* Class: Spray
*  Author: Chase Carthen
*  Description: A class for spray painting.
*/
#pragma once
#include "GLUniform.hpp"
#include "GLNode.hpp"
#include <glm/glm.hpp>
#include "GLBufferObject.hpp"
using namespace std;
using namespace glm;
struct PaintStruct
{
	glm::vec3 position;
	glm::vec3 color;
	bool Break;
};

class Spray : public GLNode
{
	const GLuint V_INDEX = 0;
	const GLuint NORM_INDEX = 1;
	const GLuint UV_INDEX = 2;
	const GLuint COLOR_INDEX = 3;
public:
	Spray(int max=10000);
	bool AddPoints(vec3 worldPoint,vec3 Color);
	void ClearPoints();
	bool Init();
	void Update();
	void Draw(std::shared_ptr<GLUniform> fragment, GLuint program);
	void AddBreak();
private:
	GLBufferObject vbo_pos;
	GLBufferObject vbo_tex;
	GLBufferObject vbo_norm;
	GLBufferObject vbo_color;

	int CurrentPoints;
	int MaxPoints;

	string fShader;
	string vShader;
	vector<PaintStruct> Points;

	glm::mat4 matrix;
    GLuint vao;
    GLuint vbo[4];
    GLuint attributes;
};