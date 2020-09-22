#include "Display.h"
#include <iostream>
#include <SDL2\SDL_syswm.h>
#include <dwmapi.h>
#include "GL/glew.h"
#include "Evt_Keyboard.h"
#include "Evt_Mouse.h"
#include "Evt_Display.h"
#include "Image.h"

static Display* globalDisplay = nullptr;

const static int DEFAULT_WIDTH = 1280;
const static int DEFAULT_HEIGHT = 720;
const static std::string DEFAULT_TITLE = "<|DEFAULT WINDOW|>";

Display::Display() : Display(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_TITLE) {}

Display::Display(const std::string& title) : Display(DEFAULT_WIDTH, DEFAULT_HEIGHT, title) {}

Display::Display(int width, int height) : Display(width, height, DEFAULT_TITLE) {}

Display::Display(int width, int height, const std::string& title) {

	if (!globalDisplay) {
		globalDisplay = this;
	}

	this->width = width;
	this->height = height;
	pxToScr[0] = 2.0f / width;
	pxToScr[1] = 2.0f / height;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	//glEnable(GL_MULTISAMPLE);

	SDL_DisplayMode dmode;
	SDL_GetCurrentDisplayMode(0, &dmode);
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(window);

	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cout << "Failed to initialize";
	}
	glViewport(0, 0, width, height);
	
	isClosed = false;

	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

}

Display::~Display() {
	if (globalDisplay == this) {
		globalDisplay = nullptr;
	}
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

Display* Display::getGlobal() {

	return globalDisplay;

}

void Display::setTitle(const std::string& title) {

	SDL_SetWindowTitle(window, title.c_str());

}

void Display::setPosition(int x, int y) {

	SDL_SetWindowPosition(window, x, y);

}

void Display::setFullscreen(bool toggle) {

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	setSize(DM.w, DM.h);
	if (toggle) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else {
		SDL_SetWindowFullscreen(window, 0);
		setPosition(0, 10);
	}

}

bool Display::isFullscreen() {

	return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);

}

void Display::setBorder(bool toggle) {

	SDL_SetWindowBordered(window, (SDL_bool)toggle);

}

void Display::setOpacity(float opacity) {

	SDL_SetWindowOpacity(window, opacity);

}

void Display::setResizable(bool toggle) {

	SDL_SetWindowResizable(window, (SDL_bool)toggle);

}

void Display::setSize(int width, int height) {

	this->width = abs(width);
	this->height = abs(height);
	SDL_SetWindowSize(window, width, height);

}

void Display::center() {

	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

}

void Display::hideCursor(bool toggle) {

	SDL_ShowCursor(toggle ? 0 : 1);

}

void Display::relativeCursor(bool toggle) {

	SDL_SetRelativeMouseMode(toggle ? SDL_TRUE : SDL_FALSE);

}

void Display::clear(double r, double g, double b, double a) {
	glClearColor((GLclampf)r, (GLclampf)g, (GLclampf)b, (GLclampf)a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::vec2 Display::getPixelToScreen() {

	return pxToScr;

}

void Display::update() {
	SDL_GL_SwapWindow(window);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			isClosed = true;
			break;
		}
		if (e.type == SDL_KEYDOWN) {
			SDL_Keycode key = e.key.keysym.sym;
			Evt_Keyboard::sendKeyPress(key);
		}
		else if (e.type == SDL_KEYUP) {
			SDL_Keycode key = e.key.keysym.sym;
			Evt_Keyboard::sendKeyRelease(key);
		}
		else if (e.type == SDL_MOUSEMOTION) {
			Evt_Mouse::sendMouseMotion(e.motion.xrel, e.motion.yrel);
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) {
			Evt_Mouse::sendMousePress(e.button.button - 1, e.button.x, e.button.y);
		}
		else if (e.type == SDL_MOUSEBUTTONUP) {
			Evt_Mouse::sendMouseRelease(e.button.button - 1, e.button.x, e.button.y);
		}
		else if (e.type == SDL_MOUSEWHEEL){
			Evt_Mouse::sendMouseWheel(e.wheel.y);
		}
		else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			width = e.window.data1;
			height = e.window.data2;
			glViewport(0, 0, width, height);
			pxToScr[0] = 2.0f / width;
			pxToScr[1] = 2.0f / height;
			Evt_Display::sendResize(width, height);
		}
	}
}