#include "BasicShader.h"

const std::string BasicShader::SHADER_PATH = "./shaders/basicShader";
BasicShader* BasicShader::globalShader = nullptr;

BasicShader::BasicShader() : Shader(SHADER_PATH) {

	configureShader();

}

BasicShader* BasicShader::getGlobal() {

	if (!globalShader) {
		globalShader = new BasicShader();
	}

	return globalShader;

}

void BasicShader::updateDimensions(int w, int h) {

	glBindTexture(GL_TEXTURE_2D, gTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	width = w;
	height = h;

}

void BasicShader::setTransMat(const glm::mat4& trans) {

	glUniformMatrix4fv(uniforms[UNIF::TRANS_MAT], 1, GL_FALSE, &trans[0][0]);

}

void BasicShader::bind() {

	Shader::bind();//Call super function

	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

}

void BasicShader::setTint(glm::vec3 color) {

	glUniform3fv(uniforms[UNIF::TINT], 1, &color[0]);

}

void BasicShader::configureShader() {

	bindAttrib("position");
	bindAttrib("texCoord");

	uniforms[UNIF::TRANS_MAT] = createUniform("transMatrix");
	uniforms[UNIF::TEXTURE] = createUniform("texMap");
	uniforms[UNIF::TINT] = createUniform("tint");

	Shader::bind();//Bind to set default values below

	glm::vec3 def(1, 1, 1);
	glUniform1i(uniforms[UNIF::TEXTURE], 0);
	glUniform3fv(uniforms[UNIF::TINT], 1, &def[0]);

}

void BasicShader::setGLTex(GLuint tex) {

	bind();
	//glBindTexture(GL_TEXTURE_2D, tex);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	gTex = tex;

}

GLuint BasicShader::createTex(bool set) {

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 0, 0, 0, GL_RGBA, GL_FLOAT, NULL);

	if (set) {
		setGLTex(tex);
	}
	
	return tex;

}

GLuint BasicShader::createBuffer() {

	GLuint buffer = 0;
	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	static unsigned int attachment = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &attachment);

	setBuffer(buffer);

	return buffer;


}