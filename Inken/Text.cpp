#include "Text.h"
#include "BasicShader.h"

#define DEFAULT_FONT			("textures/fonts/ascii.png")
static const float DIMENSION_RATIO = 2.0f / 3.0f;

Text::Text() {

	setMouseLayer(MouseLayer_None);
	font = DEFAULT_FONT;
	BasicShader& shader = (*BasicShader::getGlobal());
	buf = shader.createBuffer();
	tex.setIndex(shader.createTex());
	mesh.createQuad(false);
	
}

Text::Text(std::string str) : Text() {

	setText(str);

}

void Text::setFont(std::string str) {

	font = str;

}

Text::~Text() {

	glDeleteBuffers(1, &buf);
	tex.unloadThis();

}

void Text::setFontSize(float h) {

	setRelSize(0, 0);
	setAbsSize(text.length() * h * DIMENSION_RATIO, h);
	fontSz = h;

}

void Text::setText(std::string str) {

	text = str;
	createTexture();
	/* Update length */
	setFontSize(fontSz);

}

void Text::createTexture() {

	GLint prevBound = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevBound);

	/* Set up drawing tools */
	Texture fontTex(font);
	BasicShader& shader = (*BasicShader::getGlobal());
	shader.setBuffer(buf);
	shader.setGLTex(tex.getIndex());

	/* Prepare texture dimensions */
	/* Assume ascii 16*16 character map */
	int len = (int)text.length();
	float width = fontTex.getDimensions().x / 16.0f * len;
	float height = fontTex.getDimensions().y / 16.0f;
	shader.updateDimensions((int)width, (int)height);
	fontTex.bind();

	Transform trans;
	GLint dims[4] = { 0 };
	glGetIntegerv(GL_VIEWPORT, dims);
	/* Draw characters to reusable texture */
	if (len > 0) {
		/* Scale for texture from viewport size */
		glm::vec2 convert = glm::vec2(2 * width / (len * dims[2]), 2 * height / dims[3]);
		trans.SetScale(glm::vec3(convert.x, convert.y, 1.0f));
		for (int i = 0; i < len; i++) {
			/* Position mesh starting from bottom left corner */
			trans.SetPos(glm::vec3(i * convert.x - 1, -1, 0));
			shader.setTransMat(trans.GetMatrix());
			/* Position texture coords to point at character */
			/* Font coords start at bottom left */
			char c = text.at(i);
			glm::vec2 bottomLeft((c % 16) / 16.0f, (16 - c / 16) / 16.0f);
			mesh.setQuadTextureCoord(bottomLeft, bottomLeft + glm::vec2(1 / 16.0f, -1 / 16.0f));
			mesh.draw();

		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, prevBound);

}