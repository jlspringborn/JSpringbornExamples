#include "ShaderLoader.h"


GLuint ShaderLoader::createProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	std::string vertexShaderCode	= readShader(vertexShaderFilename);
	std::string fragmentShaderCoder = readShader(fragmentShaderFilename);

	GLuint vertexShader		= createShader(GL_VERTEX_SHADER, vertexShaderCode, "vertex shader");
	GLuint fragmentShader	= createShader(GL_FRAGMENT_SHADER, fragmentShaderCoder, "fragment shader");

	int linkResult = 0;

	// Create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	// Check for errors
	if (linkResult == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> programLog(infoLogLength);

		glGetShaderInfoLog(program, infoLogLength, NULL, &programLog[0]);
		std::cout << "Shader Loader : LINK ERROR" << '\n' << &programLog[0] << '\n';
		return 0;
	}

	return program;
}

std::string ShaderLoader::readShader(const char* filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good())
	{
		std::cout << "Can't read file " << filename << '\n';
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
}

GLuint ShaderLoader::createShader(GLenum shaderType, std::string source, const char* shaderName)
{
	int compileResult = 0;
	GLuint shader = glCreateShader(shaderType);
	const char* shaderCodePtr = source.c_str();
	const int shaderCodeSize = source.size();

	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	// Check for errors
	if (compileResult == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::vector<char> shaderLog(infoLogLength);
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);
		std::cout << "Error compiling shader: " << shaderName << '\n' << &shaderLog[0] << '\n';
		return 0;
	}

	return shader;
}