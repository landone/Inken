#pragma once

#include "Shader.h"
#include "Light.h"

class LightShader : private Shader {
public:

	LightShader(GLuint posMap, GLuint normalMap, GLuint colorMap);

	void bind();
	void drawQuad();

	Light* createLight();
	void removeLight(Light* light);
	Light* getLightArray(int& size);

	GLuint getTex() { return outputTex; }

	void setAmbientColor(glm::vec4);
	void setCameraPos(glm::vec3 pos);
	void updateDimensions(int width, int height);

	static const int MAX_LIGHTS = 256;//Correlates with value in shader

private:

	enum UNIF {
		POS_MAP, NORM_MAP, COLOR_MAP,
		LIGHT_COLOR, LIGHT_POS, LIGHT_SIZE,
		CAM_POS, AMB_COLOR,
		TOTAL
	};
	const char* UNIF_NAMES[UNIF::TOTAL] = {
		"gPosition", "gNormal", "gColor",
		"lightColor", "lightPos", "lightSize",
		"camPos", "ambientColor"
	};
	GLuint uniforms[UNIF::TOTAL];

	void configureShader();
	void initializeBuffer();

	GLuint myBuffer = 0;
	GLuint outputTex = 0;
	GLuint maps[3] = { 0 };

	std::vector<Light*> lights;
	std::vector<glm::vec4> lightColor;
	std::vector<glm::vec4> lightPos;//Also contains range value

};