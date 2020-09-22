#include "Shader.h"
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <string>

static Mesh quadMesh;
static bool meshInit = false;

static GLuint CreateShader(const std::string& text, GLenum shaderType);
static std::string LoadShader(const std::string& fileName);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

void Shader::drawQuad() {

	quadMesh.draw();

}

GLuint Shader::bindAttrib(std::string name) {

	glBindAttribLocation(m_program, attribCount++, name.c_str());
	return attribCount - 1;

}

GLuint Shader::createUniform(std::string name) {

	return glGetUniformLocation(m_program, name.c_str());

}

void Shader::bind() {

	glUseProgram(m_program);

}

Shader::Shader(std::string pathway, bool geom) {

	if (!meshInit) {

		meshInit = true;
		quadMesh.createQuad();

	}

	if (m_program != NULL) {
		for (unsigned int i = 0; i < NUM_SHADERS; i++) {
			glDetachShader(m_program, m_shaders[i]);
			glDeleteShader(m_shaders[i]);
		}
		glDeleteProgram(m_program);
	}

	geometry = geom;

	m_program = glCreateProgram();
	m_shaders[0] = CreateShader(LoadShader(pathway + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(pathway + ".fs"), GL_FRAGMENT_SHADER);
	if (geometry) {
		m_shaders[2] = CreateShader(LoadShader(pathway + ".gs"), GL_GEOMETRY_SHADER);
	}else {
		NUM_SHADERS = 2;
	}

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glAttachShader(m_program, m_shaders[i]);
	}

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "ERROR: Program linking failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "ERROR: Program is invalid: ");
}

Shader::~Shader() {
	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(m_program, m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}
	glDeleteProgram(m_program);
}

static GLuint CreateShader(const std::string& text, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0) {
		std::cout << "Shader creation failed!" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = (GLint)text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "ERROR: Shader compilation failed: ");

	return shader;
}

static std::string LoadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());
	std::string output, line;
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}
	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage) {
	GLint success = 0;
	GLchar error[1024] = { 0 };
	if (isProgram) {
		glGetProgramiv(shader, flag, &success);
	}
	else {
		glGetShaderiv(shader, flag, &success);
	}
	if (success == GL_FALSE) {
		if (isProgram) {
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else {
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}
		std::cerr << errorMessage << ":  '" << error << "'" << std::endl;
	}
}