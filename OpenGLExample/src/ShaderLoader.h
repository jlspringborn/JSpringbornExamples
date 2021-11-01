#pragma once

#include <gl/glew.h>

// std
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class ShaderLoader
{
public:

	GLuint createProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);

private:

	std::string readShader(const char* filename);
	GLuint createShader(GLenum shaderType, std::string source, const char* shaderName);
};