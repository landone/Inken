#pragma once

#include "GL/glew.h"
#include "Transform.h"
#include <string>

class Shader {
public:

	Shader(std::string pathway, bool geometryShader = false);
	~Shader();

	virtual void bind();
	void drawQuad();

	GLuint bindAttrib(std::string name);
	GLuint createUniform(std::string name);

	GLuint getPrgm() { return m_program; }

private:
	GLuint m_program = 0;
	GLuint NUM_SHADERS = 3;
	GLuint m_shaders[3] = { 0 };
	int attribCount = 0;
	bool geometry;
};