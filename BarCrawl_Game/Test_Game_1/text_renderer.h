/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>


#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;     // Horizontal offset to advance to next glyph
};


// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer
{
public:
	GLfloat text_width;
	GLfloat text_height;

	// Holds a list of pre-compiled Characters
	std::map<GLchar, Character> Characters;
	// Shader used for text rendering
	Shader TextShader;
	// Constructor
	TextRenderer(glm::mat4 camera_projection);
	// Pre-compiles a list of characters from the given font
	void Load(std::string font, GLuint fontSize);
	
	// get string size
	glm::vec2 GetStringSize(std::string text, GLfloat scale = 1.0f);

	// Renders a string of text using the precompiled list of characters
	void RenderText(std::string text, glm::mat4 camera_projection, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	GLuint VAO, VBO;
};

#endif 