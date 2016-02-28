#include <Spray.h>

Spray::Spray(int max)
{
	MaxPoints = 6*max;
	CurrentPoints = 0;
	vShader = "shaders/defer.vert";
	fShader = "shaders/defer.frag";
}

bool Spray::Init()
{
	Renderer.init();
	Renderer.addShader(GL_VERTEX_SHADER, vShader);
	Renderer.addShader(GL_FRAGMENT_SHADER, fShader);

	if(!Renderer.compile())
	{
		std::cout << "FAILED TO COMPILE" << std::endl;
		// Let the user know that everything is not okay
		return false;
	}

	if(!Renderer.link())
	{
		std::cout << "FAILED TO LINK" << std::endl;
		// Let the user know that everything is not okay
		return false;
	}
	std::cout << "SUCCESSFUL" << std::endl;
	// Load our vertexs -- do we want a loader class

	// Set up the buffers
	Renderer.useProgram();
	Buffer.generateBuffer(GL_ARRAY_BUFFER);
  	Buffer.bindBuffer();
  	Buffer.allocateBufferData(MaxPoints*sizeof(Vertex),NULL,GL_DYNAMIC_DRAW);
	Buffer.bindBuffer();

	//	layout (location = 0) in vec4 Position;
	//	layout (location = 1) in vec2 TexCoord;
	//	layout (location = 2) in vec3 Normal;
	//	layout (location = 3) in vec3 color;

	//{
	//glm::vec4 position;
	//	glm::vec3 normal;
	//	glm::vec4 color;
	//	glm::vec2 uv;
	// This only needs to be done once
	Renderer.enableVertexAttribPointer("Position");
	Renderer.setGLVertexAttribPointer("Position",4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	Renderer.enableVertexAttribPointer("TexCoord");
	Renderer.setGLVertexAttribPointer("TexCoord",2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, uv));

	Renderer.enableVertexAttribPointer("Normal");
	Renderer.setGLVertexAttribPointer("Normal",3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, normal));

	Renderer.enableVertexAttribPointer("color");
	Renderer.setGLVertexAttribPointer("color",4, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex, color));


	model = mat4(1);
	Renderer.endProgram();
	return true;
}

bool Spray::AddPoints(vec4 worldPoint,vec4 Color)
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
		vector<Vertex> vs = vector<Vertex>();
		Vertex one;
		Vertex two;
		Vertex three;
		Vertex four;

		one.position = Points[Points.size() - 2].position;
		two.position = Points[Points.size() - 1].position + vec4(0.0,1.0,0.0,0);
		three.position = Points[Points.size() - 1].position;
		four.position = Points[Points.size() - 2].position + vec4(0,1.0,0.0,0);
		one.color = Points[Points.size() - 2].color;
		two.color = Points[Points.size() - 1].color;
		three.color = Points[Points.size() - 1].color;
		four.color = Points[Points.size() - 2].color;

		vs.push_back(one);
		vs.push_back(two);
		vs.push_back(three);

		vs.push_back(one);
		vs.push_back(two);
		vs.push_back(four); 
		Buffer.updateBuffer(6*CurrentPoints*sizeof(Vertex), sizeof(Vertex)*6, &vs[0]);
		return true;
	}
	return false;
}

void Spray::AddBreak()
{
	//cout << "ADD BREAK" << std::endl;
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

void Spray::Render(mat4 projection, mat4 view)
{
	Renderer.useProgram();
	Buffer.bindBuffer();
	mat4 mvp = projection * view * model;
	Renderer.setUniformMatrix4x4("mvp", mvp);
	Renderer.setUniformMatrix4x4("model",model);
	Renderer.renderRaw(GL_TRIANGLES,6*CurrentPoints);
	Renderer.endProgram();
	//cout << CurrentPoints << endl;
}
