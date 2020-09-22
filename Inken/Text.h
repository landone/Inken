#pragma once

#include "Image.h"

class Text : public Image {
public:

	Text();
	Text(std::string);
	~Text();

	void setText(std::string);
	std::string getText() { return text; }
	void setFont(std::string);
	/* Set font size in pixels */
	void setFontSize(float);

private:

	void createTexture();

	Mesh mesh;
	std::string text;
	std::string font;
	float fontSz;
	/* Personal buffer prevents blocking */
	GLuint buf = 0;

};