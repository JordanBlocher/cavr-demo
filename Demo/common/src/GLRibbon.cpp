#include <GLRibbon.hpp>

#include "GLTexture.hpp"


GLRibbon::GLRibbon(const char* name, long maxPoints) : GLPrimitive(name, "GLRibbon", 4, maxPoints)
{
    this->v_size = maxPoints;
    this->e_size = maxPoints*3;
}

GLRibbon::~GLRibbon()
{
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
        if(glm::distance(Points[Points.size()-2].position, worldPoint) > 0.8)
            return false;

        PaintStruct temp;
        temp.position = worldPoint;
        temp.color = Color;
        temp.Break = false;
        Points.push_back(temp);

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

void GLRibbon::LoadPoints()
{
    cout << positions->size() << endl;
    cout << Points.size() << endl;
    vbo_pos.LoadSubData((Points.size()-2)*6, 0, std::vector<Vec3>(positions->end() - 6, positions->end()));
    vbo_color.LoadSubData((Points.size()-2)*6, 3, std::vector<Vec3>(colors->end() - 6, colors->end()));
    vbo_tex.LoadSubData((Points.size()-2)*6, 2, std::vector<Vec2>(uvs->end() - 6, uvs->end()) );
    vbo_norm.LoadSubData((Points.size()-2)*6, 1, std::vector<Vec3>(normals->end() - 6, normals->end()) );
    vbo_elements.LoadSubData((Points.size()-2)*6, 1, std::vector<Vec3>(faces->end() - 6, faces->end()) );
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

Vec3 GLRibbon::Tail()
{
    return Points.back().position;
}
