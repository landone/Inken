#pragma once

#include "Entity.h"

class Light : public Entity {
public:

	static Light* create() { return new Light(); }

	void setColor(double r, double g, double b, double intensity = 1.0) { setColor(glm::vec4(r, g, b, intensity)); }
	void setColor(const glm::vec4& color) { this->color = color; }

	glm::vec4 getColor() { return color; }

	void setRange(double range) { this->range = range; }
	double getRange() { return range; }

private:

	Light() {}

	glm::vec4 color = glm::vec4(1,1,1,1);
	double range = 40;

};