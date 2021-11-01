#include "TextRenderer.h"



TextRenderer::TextRenderer(std::string inText, std::string inFont, int inSize,
	glm::vec3 inColor, GLuint inProgram)
{
	text	= inText;
	color	= inColor;
	scale	= 1.0f;
	program = inProgram;
	setPosition(position);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f,
		static_cast<GLfloat>(600));
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));

	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Counld not init FreeType Library" <<
			'\n';
	}

	// Load font
	FT_Face face;
	if (FT_New_Face(ft, inFont.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << '\n';
	}

	// Set size of glyphs
	FT_Set_Pixel_Sizes(face, 0, inSize);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte i = 0; i < 128; i++)
	{
		// Load character glyph
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load glyph" << '\n';
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);

		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Create a character
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		// Store character in characters map
		characters.insert(std::pair<GLchar, Character>(i, character));

	}

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL,
		GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::draw()
{
	glm::vec2 textPos = position;

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y,
		color.z);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);

	std::string::const_iterator c;
	
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = textPos.x + ch.Bearing.x * scale;
		GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		// Per character update VBO
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Use glBufferSubData and not glBufferData
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render Quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 
		// 1/64 pixels
		// Bitshift by 6 to get value in pixels (2^6 = 64 (divide amout of 
		// 1/64th pixels by 64 to get amount of pixels))
		textPos.x += (ch.Advance >> 6) * scale;

	}
	// Disable blending
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

void TextRenderer::setPosition(glm::vec2 inPosition)
{
	position = inPosition;
}

void TextRenderer::setText(std::string inText)
{
	text = inText;
}