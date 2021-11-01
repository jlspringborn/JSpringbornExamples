#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "bullet/btBulletDynamicsCommon.h"

#include "Camera.h"
#include "LightRenderer.h"

#include <vector>

class MeshRenderer {
public:
	MeshRenderer(MeshType meshType, std::string inName, Camera* inCamera,
		btRigidBody* inRigidBody, LightRenderer* inLight, 
		float inSpecularStrength, float inAmbientStrength);
	~MeshRenderer();

	void draw();

	void setPosition(glm::vec3 inPosition);
	void setScale(glm::vec3 inScale);
	void setProgram(GLuint inProgram);
	void setTexture(GLuint inTexture);

	std::string				name = "";
	btRigidBody*			rigidBody;

private:
	std::vector<Vertex>		vertices;
	std::vector<GLuint>		indices;
	glm::mat4				modelMatrix;
	Camera*					camera;
	glm::vec3				position;
	glm::vec3				scale;
	GLuint					vao;
	GLuint					vbo;
	GLuint					ebo;
	GLuint					texture;
	GLuint					program;
	LightRenderer*			light;
	float					ambientStrength;
	float					specularStrength;
};