#include "GLMesh.hpp"


GLMesh::GLMesh(const char* name) : GLNode(name)
{
    this->Allocate();
}

GLMesh::GLMesh(const char* name, const char* typeName) : GLNode(name, typeName)
{
    this->Allocate();
}

GLMesh::~GLMesh()
{
}

void GLMesh::Allocate()
{
    this->e_size = 0;
    this->v_size = 0;
    this->_positions = sp2dvec3(new std::vector<std::vector<Vec3>>);
    this->_colors = sp2dvec3(new std::vector<std::vector<Vec3>>);
    this->_normals = sp2dvec3(new std::vector<std::vector<Vec3>>);
    this->_uvs = sp2dvec2(new std::vector<std::vector<glm::ivec2>>);
    this->_faces = sp2dvec(new std::vector<std::vector<GLuint>>);
    this->_positions->resize(1);
    this->_normals->resize(1);
    this->_colors->resize(1);
    this->_uvs->resize(1);
    this->_faces->resize(1);
    this->positions = std::make_shared<std::vector<Vec3>>(this->_positions->at(0));
    this->normals = std::make_shared<std::vector<Vec3>>(this->_normals->at(0));
    this->colors = std::make_shared<std::vector<Vec3>>(this->_colors->at(0));
    this->uvs = std::make_shared<std::vector<glm::ivec2>>(this->_uvs->at(0));
    this->faces = std::make_shared<std::vector<GLuint>>(this->_faces->at(0));
}

void GLMesh::AddTriangle(Vec3 v0, Vec3 v1, Vec3 v2)
{
    Vec3 d0 = glm::normalize(v1 - v0);
    Vec3 d1 = glm::normalize(v2 - v0);
    Vec3 normal = glm::cross(d0, d1);
    int cnt = this->positions->size();
    AddTriangle(v0, v1, v2, normal, cnt);
}

void GLMesh::AddTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Vec3 normal, int cnt)
{
    this->positions->push_back(v0);
    this->positions->push_back(v1);
    this->positions->push_back(v2);
    this->normals->push_back(normal);
    this->normals->push_back(normal);
    this->normals->push_back(normal);
    this->faces->push_back(cnt);
    this->faces->push_back(cnt+1);
    this->faces->push_back(cnt+2);

}

int GLMesh::AddCircle(int nvertices, double radius, double zcoord, double znormal = 0.0, Vec2 uvStart = Vec2(0.0f), Vec2 uvEnd = Vec2(1.0f))
{
    int ret = this->positions->size();
    for (int a = 0; a < nvertices; a++)
    {
        double alpha = 2 * M_PI * a / nvertices;
        double x = cos(alpha);
        double y = sin(alpha);
        Vec3 normal = Vec3(x, y, 0);
        if (znormal != 0.0)
        {
            normal *= 1 - abs(znormal*znormal*znormal);       // Decent approximation..
            normal.z = znormal;    
        }
        this->positions->push_back(Vec3(radius * x, radius * y, zcoord));
        this->normals->push_back(normal);
        this->uvs->push_back(InterpolateU(uvStart, uvEnd, a, nvertices));
    }
    return ret;
}


void GLMesh::AddTriangleStrip(int vertexOffset1, int vertexOffset2, int nvertices, bool wrap = false)
{
    int a;
    int loopcnt = wrap ? nvertices : (nvertices - 1);
    for (a = 0; a < loopcnt; a++)
    {
        int a1 = (a + 1) % nvertices;

        this->faces->push_back(vertexOffset1 + a);
        this->faces->push_back(vertexOffset2 + a1);
        this->faces->push_back(vertexOffset1 + a1);

        this->faces->push_back(vertexOffset2 + a1);
        this->faces->push_back(vertexOffset1 + a);
        this->faces->push_back(vertexOffset2 + a);
    }
}


void GLMesh::AddTriangleSurface(int offset, int nverticesX, int nverticesY, bool wrapX = false, bool wrapY = false)
{
    for (int y = 0; y < nverticesY - 1; y++)
    {
        AddTriangleStrip(offset + y * nverticesX, offset + (y + 1) * nverticesX, nverticesX, wrapX);
    }
    if (wrapY)
    {
        AddTriangleStrip(offset + (nverticesY - 1) * nverticesX, offset, nverticesX, wrapX);
    }
}

void GLMesh::AddTriangleFan(int offsetVertices, int offsetCenter, int nvertices, bool fliporder = false)
{
    for (int a = 0; a < nvertices; a++)
    {
        if (fliporder)
        {
            this->faces->push_back(offsetVertices + a);
            this->faces->push_back(offsetVertices + ((a + 1) % nvertices));
            this->faces->push_back(offsetCenter);
        }
        else
        {
            this->faces->push_back(offsetVertices + ((a + 1) % nvertices));
            this->faces->push_back(offsetVertices + a);
            this->faces->push_back(offsetCenter);
        }
    }
}

Vec2 GLMesh::InterpolateUV(Vec2 start, Vec2 stop, double ucounter, double utotal, double vcounter, double vtotal)
{
    return Vec2((stop.x - start.x) * ucounter / utotal + start.x, (stop.y - start.x) * vcounter / vtotal + start.y);
}

Vec2 GLMesh::InterpolateU(Vec2 start, Vec2 stop, int counter, int total)
{
    return Vec2((stop.x - start.x) * counter / total + start.x, start.y);
}

Vec2 GLMesh::InterpolateV(Vec2 start, Vec2 stop, int counter, int total)
{
    return Vec2(start.x, (stop.y - start.y) * counter / total + start.y);
}

