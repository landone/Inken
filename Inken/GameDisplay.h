#pragma once

#include "Display.h"
#include "GBuffer.h"
#include "LightShader.h"
#include "PostProcessor.h"
#include "Evt_Display.h"

class GameDisplay : public Display, DisplayListener {
public:

	GameDisplay();
	GameDisplay(const std::string& title);
	GameDisplay(int width, int height);
	GameDisplay(int width, int height, const std::string& title);

	void setAmbientColor(float r, float g, float b, float a = 1.0f);

	GBuffer gBuffer;
	LightShader lightShader;
	PostProcessor postProcessor;

private:

	void onResize(int, int) override;

};