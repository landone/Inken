#include <time.h>
#include <iostream>

#include "Assets.h"
#include "GameDisplay.h"
#include "GameManager.h"
#include "Image.h"
#include "Text.h"

#define HIDE_CONSOLE	(false)

int main() {

	//ShowWindow(GetConsoleWindow(), (HIDE_CONSOLE ? SW_HIDE : SW_NORMAL));

	srand(clock()); //Enable random number gens
	
	/* Display initializes GLEW, so it must occur first */
	GameDisplay display(800, 450, "Inken");
	Assets::LoadAll();

	GameManager gameMan;

	Camera& cam = gameMan.getCamera();
	Camera& guiCam = gameMan.getGUICamera();

	display.setAmbientColor(0, 0.5, 1);
	display.setResizable(true);

	while (display.isOpen()) {

		/* Update frame listeners */
		Evt_Display::sendFrame();

		/* Draw Geometry */
		display.gBuffer.bind();
		display.gBuffer.setViewMat(cam.getViewMatrix());
		Evt_Display::sendDrawGeometry(display.gBuffer);
		/* Draw 3D GUI */
		display.gBuffer.setViewMat(guiCam.getViewMatrix());
		glDisable(GL_DEPTH_TEST);
		Evt_Display::sendDraw3DGUI(display.gBuffer);
		/* Draw 2D GUI */
		display.gBuffer.setGUI(true);
		display.gBuffer.setViewMat(glm::mat4());
		Evt_Display::sendDrawGUI(display.gBuffer);
		glEnable(GL_DEPTH_TEST);
		display.gBuffer.setGUI(false);
		/* Apply lighting */
		display.lightShader.bind();
		display.lightShader.setCameraPos(cam.getPos());
		display.lightShader.drawQuad();
		/* Apply post effects */
		display.postProcessor.bind();
		display.postProcessor.drawQuad();

		/* Swap buffers */
		display.update();

	}

	return 0;

}