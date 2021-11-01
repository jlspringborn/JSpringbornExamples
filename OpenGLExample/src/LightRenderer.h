#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "ShaderLoader.h"
#include "Camera.h"

class LightRenderer
{
public:
	LightRenderer(MeshType meshType, Camera* inCamera);
	~LightRenderer();

	void draw();

	void setPosition(glm::vec3 inPosition);
	void setColor(glm::vec3 inColor);
	void setProgram(GLuint inProgram);

	glm::vec3 getPosition();
	glm::vec3 getColor();

private:

	Camera*				camera;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	glm::vec3			position;
	glm::vec3			color;
	GLuint				vbo;
	GLuint				ebo;
	GLuint				vao;
	GLuint				program;
};