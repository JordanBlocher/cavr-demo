/* Class: Spray
*  Author: Chase Carthen
*  Description: A class for spray painting.
*/
#pragma once
#include "GLUniform.hpp"
#include "GLNode.hpp"
#include <glm/glm.hpp>
#include <GLPrimitive.hpp>
#include <iostream>

using namespace std;
using namespace glm;

const long MAX_POINTS = 10000;


class Spray : public GLPrimitive
{
    public:

    struct PaintStruct
    {
        glm::vec3 position;
        glm::vec3 color;
        bool Break;
    };
	Spray(const char* name = "spray", long maxPoints = MAX_POINTS);
    ~Spray();

	bool AddPoints(vec3 worldPoint,vec3 Color);
	void ClearPoints();
	bool Init();
	void Update();
	void Draw(GLenum);
	void AddBreak();

private:
	int CurrentPoints;

	string fShader;
	string vShader;
	vector<PaintStruct> Points;
};
