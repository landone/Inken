#include "GBuffer.h"
#include <string>

const std::string GBuffer::SHADER_PATH = "./shaders/gBuffer";

GBuffer::GBuffer() : Shader(SHADER_PATH) {

	initializeBuffer();
	configureShader();

}

void GBuffer::updateDimensions(int w, int h) {

	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, gDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

}

void GBuffer::setTransMat(const glm::mat4& trans) {

	glUniformMatrix4fv(uniforms[UNIF::TRANS_MAT], 1, GL_FALSE, &trans[0][0]);

}

void GBuffer::setRotMat(const glm::mat4& rot) {

	glUniformMatrix4fv(uniforms[UNIF::ROT_MAT], 1, GL_FALSE, &rot[0][0]);

}

void GBuffer::setViewMat(const glm::mat4& view) {

	glUniformMatrix4fv(uniforms[UNIF::VIEW_MAT], 1, GL_FALSE, &view[0][0]);

}

void GBuffer::setTint(const glm::vec3& tint) {

	glUniform3fv(uniforms[UNIF::TINT], 1, &tint[0]);

}

void GBuffer::setWireFrame(bool toggle) {

	glPolygonMode(GL_FRONT_AND_BACK, toggle ? GL_LINE : GL_FILL);

}

void GBuffer::setGUI(bool toggle) {

	glUniform1i(uniforms[UNIF::GUI], toggle);

}

void GBuffer::bind() {

	Shader::bind();//Call super function

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GBuffer::configureShader() {

	bindAttrib("position");
	bindAttrib("texCoord");
	bindAttrib("normal");
	bindAttrib("texID");

	uniforms[UNIF::TRANS_MAT] = createUniform("transMatrix");
	uniforms[UNIF::ROT_MAT] = createUniform("rotMatrix");
	uniforms[UNIF::VIEW_MAT] = createUniform("viewMatrix");

	uniforms[UNIF::TEXTURES] = createUniform("textures");
	uniforms[UNIF::TINT] = createUniform("tint");
	uniforms[UNIF::GUI] = createUniform("gui");

	Shader::bind();//Bind to set default values below

	glm::vec3 defaultVec(1, 1, 1);
	glUniform3fv(uniforms[UNIF::TINT], 1, &defaultVec[0]);
	glUniform1i(uniforms[UNIF::GUI], false);
	/* Initialize sampler2D array uniform */
	GLint texIDs[MAX_TEXTURES];
	for (int i = 0; i < MAX_TEXTURES; i++) {
		texIDs[i] = i;
	}
	glUniform1iv(uniforms[UNIF::TEXTURES], MAX_TEXTURES, &texIDs[0]);
	

}

void GBuffer::initializeBuffer() {

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//Establish position texture
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 0, 0, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	//Establish normal texture
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 0, 0, 0, GL_RGB, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//Establish color texture
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	//Depth buffer
	glGenTextures(1, &gDepth);
	glBindTexture(GL_TEXTURE_2D, gDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 0, 0, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);
	
	static unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	
	//For safety, unbind this created buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
