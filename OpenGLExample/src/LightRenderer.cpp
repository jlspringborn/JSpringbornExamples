#include "LightRenderer.h"

LightRenderer::LightRenderer(MeshType meshType, Camera* inCamera)
{
	camera = inCamera;

	switch (meshType)
	{
	case kTriangle:
		Mesh::setTriData(vertices, indices);
		break;
	case kQuad:
		Mesh::setQuadData(vertices, indices);
		break;
	case kCube:
		Mesh::setCubeData(vertices, indices);
		break;
	case kSphere:
		Mesh::setSphereData(vertices, indices);
		break;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::color)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

LightRenderer::~LightRenderer()
{
}

void LightRenderer::draw()
{
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(glm::mat4(1.0), position);

	glUseProgram(program);

	GLint modelLoc = glGetUniformLocation(program, "model");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = camera->getViewMatrix();
	GLint vLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = camera->getProjectionMatrix();
	GLint pLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(proj));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// unbind
	glBindVertexArray(0);
	glUseProgram(0);
}

void LightRenderer::setPosition(glm::vec3 inPosition)
{
	position = inPosition;
}

void LightRenderer::setColor(glm::vec3 inColor)
{
	color = inColor;
}

void LightRenderer::setProgram(GLuint inProgram)
{
	program = inProgram;
}

glm::vec3 LightRenderer::getPosition()
{
	return position;
}

glm::vec3 LightRenderer::getColor()
{
	return color;
}