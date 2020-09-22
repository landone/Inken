#pragma once

#include "Shader.h"

class PostProcessor : private Shader{
public:

	PostProcessor(GLuint preProcessMap);

	void bind();
	void drawQuad() { Shader::drawQuad(); }//Publicize super method

	void updateDimensions(int width, int height);

private:

	GLuint preProcMap = 0;
	float timeOffset = 0.0;

	enum UNIF {
		PREPROCESS,
		INV_TEX_SIZE,
		TIME_OFFSET,
		TOTAL
	};
	GLuint uniforms[UNIF::TOTAL];

	void configureShader();

};