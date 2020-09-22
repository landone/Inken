#include "Image.h"
#include "Display.h"
#include "Assets.h"

#define DEFAULT_TEXTURE ("textures/missing.png")

/* Default mesh */
static const std::vector<Vertex> vertices{
	Vertex{ glm::vec3(0,0,0),glm::vec3(0, 0, 1),glm::vec2(0,0) },
	Vertex{ glm::vec3(1,0,0),glm::vec3(0, 0, 1),glm::vec2(1,0) },
	Vertex{ glm::vec3(1,1,0),glm::vec3(0, 0, 1),glm::vec2(1,1) },
	Vertex{ glm::vec3(0,1,0),glm::vec3(0, 0, 1),glm::vec2(0,1) }
};
static const std::vector<GLuint> indices{
	0, 1, 2,
	2, 3, 0
};

Image::Image(TEXTURES type) {

	setTexture(type);
	setGUILayer(GUILayer_Middle);
	setMouseLayer(MouseLayer_Middle);
	mesh.Init(vertices, indices);

}

bool Image::onMousePress(int button, int x, int y) {

	if (button == 1 || !visible) {
		return false;
	}
	float point[2];
	glm::vec2 pxToScr = Display::getGlobal()->getPixelToScreen();
	point[0] = x * pxToScr[0] - 1.0f;
	point[1] = y * -pxToScr[1] + 1.0f;
	glm::vec2 pos = getTruePos();
	glm::vec2 size = getTrueSize();
	if ((point[0] > pos.x&& point[0] < pos.x + size.x) && (point[1] > pos.y&& point[1] < pos.y + size.y)) {
		return true;
	}

	return false;

}

bool Image::onMouseRelease(int button, int x, int y) {

	if (button == 1 || !visible) {
		return false;
	}
	float point[2];
	glm::vec2 pxToScr = Display::getGlobal()->getPixelToScreen();
	point[0] = x * pxToScr[0] - 1.0f;
	point[1] = y * -pxToScr[1] + 1.0f;
	glm::vec2 pos = getTruePos();
	glm::vec2 size = getTrueSize();
	if ((point[0] > pos.x && point[0] < pos.x + size.x) && (point[1] > pos.y && point[1] < pos.y + size.y)) {
		return true;
	}

	return false;

}

void Image::onDrawGUI(GBuffer& buf) {

	if (!visible) {
		return;
	}
	checkChanged();
	tex.bind();
	if (tinted) {
		buf.setTint(tint);
	}
	buf.setRotMat(trans.GetRotMatrix());
	buf.setTransMat(trans.GetMatrix());
	mesh.draw();
	if (tinted) {
		buf.setTint(glm::vec3(1, 1, 1));
	}

}

void Image::setTiled(bool toggle) {

	if (tiled != toggle) {
		tiled = toggle;
		texChanged = true;
	}

}

void Image::setTexture(TEXTURES type) {

	tex = Assets::texture(type);
}

void Image::onResize(int x, int y) {

	transChanged = true;
	if (tiled) {
		texChanged = true;
	}

}

void Image::checkChanged() {

	if (transChanged) {
		transChanged = false;
		calcTrans();
	}
	if (texChanged) {
		texChanged = false;
		calcTex();
	}

}

glm::vec2 Image::getTruePos() {

	checkChanged();
	return trans.GetPos();

}

glm::vec2 Image::getTrueSize() {

	checkChanged();
	return trans.GetScale();

}

void Image::setRelPos(float x, float y) {

	setRelPos(glm::vec2(x, y));

}

void Image::setRelPos(glm::vec2 pos) {

	relPos = pos;
	transChanged = true;

}

void Image::setAbsPos(float x, float y) {

	setAbsPos(glm::vec2(x, y));

}

void Image::setAbsPos(glm::vec2 pos) {

	absPos = pos;
	transChanged = true;

}

void Image::setRelSize(float x, float y) {

	setRelSize(glm::vec2(x, y));

}

void Image::setRelSize(glm::vec2 amt) {

	relSz = amt;
	transChanged = true;

}

void Image::setAbsSize(float x, float y) {

	setAbsSize(glm::vec2(x, y));

}

void Image::setAbsSize(glm::vec2 amt) {

	absSz = amt;
	transChanged = true;

}

void Image::setRelOffset(float x, float y) {

	setRelOffset(glm::vec2(x, y));

}

void Image::setRelOffset(glm::vec2 offset) {

	relOffs = offset;
	texChanged = true;

}

void Image::setTint(float r, float g, float b) {

	setTint(glm::vec3(r, g, b));

}

void Image::setTint(glm::vec3 color) {

	tint = color;
	tinted = (tint != glm::vec3(1, 1, 1));

}

void Image::setVisibility(bool toggle) {

	visible = toggle;

}

void Image::calcTrans() {
	
	glm::vec2 pxToScr = Display::getGlobal()->getPixelToScreen();
	trans.SetPos(glm::vec3(relPos[0] + absPos[0] * pxToScr[0], relPos[1] + absPos[1] * pxToScr[1], 0));
	trans.SetScale(glm::vec3(relSz[0] + absSz[0] * pxToScr[0], relSz[1] + absSz[1] * pxToScr[1], 0));

}

void Image::calcTex() {

	glm::vec2 texScale = glm::vec2(1, 1);
	glm::vec2 pxToScr = Display::getGlobal()->getPixelToScreen();
	if (tiled) {
		texScale = glm::vec2(trans.GetScale()) / (pxToScr * tex.getDimensions());
	}
	mesh.setTextureScale(texScale);
	mesh.setTextureOffset(relOffs);

}