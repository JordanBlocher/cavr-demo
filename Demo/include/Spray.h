/* Class: Spray
*  Author: Chase Carthen
*  Description: A class for spray painting.
*/
#pragma once
#include <GLModel.h>

using namespace std;
using namespace glm;
struct PaintStruct
{
	glm::vec4 position;
	glm::vec4 color;
	bool Break;
};

class Spray : public Model
{
public:
	Spray(int max=10000);
	bool AddPoints(vec4 worldPoint,vec4 Color);
	void ClearPoints();
	virtual bool Init();
	virtual void Update();
	virtual void Render(mat4 projection, mat4 view);
	void AddBreak();
private:
	int CurrentPoints;
	int MaxPoints;
	renderer Renderer;
	string fShader;
	string vShader;
	buffer Buffer;
	vector<PaintStruct> Points;

	glm::mat4 matrix;
    GLuint vao;
    GLuint vbo[4];
    GLuint attributes;
};