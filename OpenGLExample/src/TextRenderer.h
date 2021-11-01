#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <iostream>

struct Character 
{
	GLuint		TextureID;	// Texture ID of each glyph texture
	glm::ivec2	Size;		// glyph size
	glm::ivec2	Bearing;	// baseline to left/top of glyph
	GLuint		Advance;	// id to next glyph
};

class TextRenderer
{
public:
	TextRenderer(std::string inText, std::string inFont, int inSize, 
		glm::vec3 inColor, GLuint inProgram);
	~TextRenderer();

	void draw();
	void setPosition(glm::vec2 inPosition);
	void setText(std::string inText);

private:

	std::string text;
	GLfloat		scale;
	glm::vec3	color;
	glm::vec2	position;
	GLuint		VAO;
	GLuint		VBO;
	GLuint		program;

	std::map<GLchar, Character> characters;
};