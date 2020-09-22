#include "Button.h"
#include "Display.h"

Button::Button(TEXTURES type) : Image(type) {



}

bool Button::onMouseRelease(int button, int x, int y) {

	if (button == 1 || !getVisibility()) {
		return false;
	}
	float point[2];
	glm::vec2 pxToScr = Display::getGlobal()->getPixelToScreen();
	point[0] = x * pxToScr[0] - 1.0f;
	point[1] = y * -pxToScr[1] + 1.0f;
	glm::vec2 pos = getTruePos();
	glm::vec2 size = getTrueSize();
	if ((point[0] > pos.x&& point[0] < pos.x + size.x) && (point[1] > pos.y&& point[1] < pos.y + size.y)) {
		if (callback) {
			callback((*this), data);
		}
		return true;
	}

	return false;

}

void Button::setCallback(void(*f)(Button&,void*), void* d) {

	callback = f;
	data = d;

}