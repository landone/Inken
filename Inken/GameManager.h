#pragma once

#include "Camera.h"
#include "Button.h"
#include "Evt_Mouse.h"
#include "Evt_Display.h"
#include "Display.h"
#include "Model.h"

#include <Lensce/LensceServer.h>
#include <Lensce/LensceClient.h>

class GameManager : public MouseListener, public DisplayListener {
public:

	GameManager();

	Camera& getCamera() { return cam; }
	Camera& getGUICamera() { return guiCam; }

private:

	static void onPress(Button&, void*);

	Display* display = nullptr;
	Camera cam;
	Camera guiCam;

	Button connect[2];
	LensceServer* server = nullptr;
	LensceClient* client = nullptr;

};