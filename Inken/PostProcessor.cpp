#include "PostProcessor.h"
#include "Display.h"
#include <math.h>
#include <string>

static const std::string SHADER_PATH = "./shaders/postShader";

PostProcessor::PostProcessor(GLuint map) : Shader(SHADER_PATH){

	preProcMap = map;
	configureShader();

}

void PostProcessor::configureShader() {

	uniforms[UNIF::PREPROCESS] = createUniform("preProcess");
	uniforms[UNIF::INV_TEX_SIZE] = createUniform("invTexSize");
	uniforms[UNIF::TIME_OFFSET] = createUniform("timeOffset");

	Shader::bind();//Bind to set default values below

	glUniform1i(uniforms[UNIF::PREPROCESS], 0);

}

void PostProcessor::updateDimensions(int w, int h) {

	glm::vec2 inverted(1.0 / w, 1.0 / h);
	glUniform2fv(uniforms[UNIF::INV_TEX_SIZE], 1, &inverted[0]);

}

void PostProcessor::bind() {

	Shader::bind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);//Bind default/display buffer

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, preProcMap);
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	timeOffset += (float)(M_PI / 144.0f);
	if (timeOffset >= 2 * M_PI) {
		timeOffset -= (float)(2 * M_PI);
	}
	glUniform1f(uniforms[UNIF::TIME_OFFSET], timeOffset);

}