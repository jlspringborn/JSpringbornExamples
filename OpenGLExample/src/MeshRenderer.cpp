#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(MeshType meshType, std::string inName,
	Camera* inCamera, btRigidBody* inRigidBody, LightRenderer* inLight,
	float inSpecularStrength, float inAmbientStrength)
{
	name				= inName;
	rigidBody			= inRigidBody;
	camera				= inCamera;
	light				= inLight;
	ambientStrength		= inAmbientStrength;
	specularStrength	= inSpecularStrength;
	scale				= glm::vec3(1.0f, 1.0f, 1.0f);
	position			= glm::vec3(0.0f, 0.0f, 0.0f);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
		&vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
		&indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::texCoords)));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::normal)));

	// unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::draw()
{
	// Calculate model position
	btTransform t;

	rigidBody->getMotionState()->getWorldTransform(t);

	btQuaternion rotation = t.getRotation();
	btVector3 translate = t.getOrigin();

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.getAngle(),
		glm::vec3(rotation.getAxis().getX(), rotation.getAxis().getY(),
			rotation.getAxis().getZ()));

	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),
		glm::vec3(translate.getX(), translate.getY(), translate.getZ()));
	
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	
	modelMatrix = glm::mat4(1.0f);

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	glm::mat4 viewProjection = camera->getProjectionMatrix() *
		camera->getViewMatrix();

	glUseProgram(program);
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Set texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set shader uniforms for lighting
	GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->getCameraPosition().x,
		camera->getCameraPosition().y, camera->getCameraPosition().z);

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, light->getPosition().x, light->getPosition().y,
		light->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, light->getColor().x, light->getColor().y,
		light->getColor().z);

	GLuint specularStrengthLoc = glGetUniformLocation(program,
		"specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);

	GLuint ambientStrengthLoc = glGetUniformLocation(program,
		"ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// unbind
	glBindVertexArray(0);
}

void MeshRenderer::setPosition(glm::vec3 inPosition)
{
	position = inPosition;
}

void MeshRenderer::setScale(glm::vec3 inScale)
{
	scale = inScale;
}

void MeshRenderer::setProgram(GLuint inProgram)
{
	program = inProgram;
}

void MeshRenderer::setTexture(GLuint inTexture)
{
	texture = inTexture;
}