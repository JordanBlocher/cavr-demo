/* Class: GLRibbon
*  Author: Chase Carthen
*  Description: A class for GLRibbon painting.
*/
#pragma once
#include "GLUniform.hpp"
#include "GLNode.hpp"
#include <glm/glm.hpp>
#include <GLPrimitive.hpp>
#include <iostream>

using namespace std;
using namespace glm;

const long MAX_POINTS = 100000;


class GLRibbon : public GLPrimitive
{
    public:

    struct PaintStruct
    {
        Vec3 position;
        Vec3 color;
        bool Break;
    };

	GLRibbon(const char* name = "GLRibbon", long maxPoints = MAX_POINTS);
    ~GLRibbon();

    virtual bool Create();
    virtual void CreateVBOs();
    virtual void DrawElements(size_t, GLint, GLint, GLenum);
	bool AddPoints(vec3 worldPoint,vec3 Color);
	void ClearPoints();
	bool Init();
	void AddBreak();
    Vec3 Tail();

private:
	vector<PaintStruct> Points;
};
