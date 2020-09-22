#pragma once

#include "Shader.h"
#include "Texture.h"

class BasicShader : private Shader{
public:

	BasicShader();

	static BasicShader* getGlobal();

	void bind() override;
	void updateDimensions(int width, int height);
	void setTransMat(const glm::mat4& trans);

	//Binds buffer as well
	void setGLTex(GLuint tex);

	void setBuffer(GLuint buf) { buffer = buf; }
	void setTint(glm::vec3);

	Texture getTex() { return Texture(gTex, glm::vec2(width, height)); }
	GLuint createBuffer();
	GLuint createTex(bool set = true);
	GLuint getGLTex() { return gTex; }
	GLuint getBuffer() { return buffer; }

private:
	static BasicShader* globalShader;
	GLuint buffer = 0;
	GLuint gTex = 0;

	int width = 0;
	int height = 0;

	void configureShader();

	enum UNIF {
		TRANS_MAT,
		TEXTURE,
		TINT,
		TOTAL
	};

	GLuint uniforms[UNIF::TOTAL];

	static const std::string SHADER_PATH;

};