#include "GameDisplay.h"
#include <iostream>

void GameDisplay::setAmbientColor(float r, float g, float b, float a) {

	lightShader.setAmbientColor(glm::vec4(r, g, b, a));

}

void GameDisplay::onResize(int w, int h) {

	gBuffer.updateDimensions(w, h);
	lightShader.updateDimensions(w, h);
	postProcessor.updateDimensions(w, h);

}

GameDisplay::GameDisplay(int width, int height, const std::string& title) : Display(width, height, title), 
gBuffer(), lightShader(gBuffer.getPositionTex(), gBuffer.getNormalTex(), gBuffer.getColorTex()), postProcessor(lightShader.getTex()) {

	onResize(width, height);

}

GameDisplay::GameDisplay(int width, int height) : Display(width, height),
gBuffer(), lightShader(gBuffer.getPositionTex(), gBuffer.getNormalTex(), gBuffer.getColorTex()), postProcessor(lightShader.getTex()) {

	onResize(width, height);

}

GameDisplay::GameDisplay(const std::string& title) : Display(title),
gBuffer(), lightShader(gBuffer.getPositionTex(), gBuffer.getNormalTex(), gBuffer.getColorTex()), postProcessor(lightShader.getTex()) {

	int width = getWidth();
	int height = getHeight();

	onResize(width, height);

}

GameDisplay::GameDisplay() : Display(),
gBuffer(), lightShader(gBuffer.getPositionTex(), gBuffer.getNormalTex(), gBuffer.getColorTex()), postProcessor(lightShader.getTex()) {

	int width = getWidth();
	int height = getHeight();

	onResize(width, height);

}