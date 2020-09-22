#pragma once

#include "Image.h"

class Button : public Image {
public:

	Button(TEXTURES = TEXTURE_NONE);

	/* Callback passes button and data back */
	void setCallback(void(*f)(Button&,void*), void* data);

private:

	bool onMouseRelease(int, int, int) override;

	void(*callback)(Button&,void*) = nullptr;
	void* data = nullptr;

};