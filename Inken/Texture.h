#pragma once

#include <string>
#include "GL/glew.h"
#include "glm/vec2.hpp"

class Texture {
public:

	Texture() {}
	Texture(std::string path) { Load(path); }
	Texture(GLuint tex, glm::vec2 d) { m_texture = tex;  dim = d; }

	Texture& Load(const std::string& fileName);
	void setIndex(GLuint tex) { m_texture = tex; }
	void bind(GLuint id = 0);

	GLuint getIndex() { return m_texture; }
	glm::vec2 getDimensions() { return dim; }

	static void unloadAll();
	/* CAREFUL: ensure others aren't referencing this */
	void unloadThis();

	std::string pathway;
private:

	GLuint m_texture = -1;
	glm::vec2 dim = glm::vec2();
	GLuint CreateTexture(const std::string &fileName);
	
};