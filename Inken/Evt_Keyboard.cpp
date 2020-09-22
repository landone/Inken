#include "Evt_Keyboard.h"

#include <vector>
#include <algorithm>
#include <iostream>

bool Evt_Keyboard::keys[KEY_TOTAL] = { false };

std::vector<KeyboardListener*> Evt_Keyboard::listeners = {};

KeyboardListener::KeyboardListener() : listeners(Evt_Keyboard::getListeners()){
	listeners.push_back(this);
}

KeyboardListener::~KeyboardListener() {
	listeners.erase(std::remove(listeners.begin(), listeners.end(), this), listeners.end());
}

void Evt_Keyboard::sendKeyPress(SDL_Keycode code) {

	KEY key = keyCodeToEnum(code);
	keys[key] = true;

	for (unsigned int i = 0; i < listeners.size(); i++) {
		listeners[i]->onKeyPress(key);
	}

}

void Evt_Keyboard::sendKeyRelease(SDL_Keycode code) {

	KEY key = keyCodeToEnum(code);
	keys[key] = false;

	for (unsigned int i = 0; i < listeners.size(); i++) {
		listeners[i]->onKeyRelease(key);
	}

}

bool Evt_Keyboard::isKeyDown(KEY key) {

	return keys[key];

}

KEY Evt_Keyboard::keyCodeToEnum(SDL_Keycode code) {

	switch (code) {
	case SDLK_w:
		return KEY_W;
	case SDLK_a:
		return KEY_A;
	case SDLK_s:
		return KEY_S;
	case SDLK_d:
		return KEY_D;
	case SDLK_UP:
		return KEY_UP;
	case SDLK_LEFT:
		return KEY_LEFT;
	case SDLK_DOWN:
		return KEY_DOWN;
	case SDLK_RIGHT:
		return KEY_RIGHT;
	case SDLK_EQUALS:
		return KEY_EQUAL;
	case SDLK_MINUS:
		return KEY_MINUS;
	case SDLK_SPACE:
		return KEY_SPACE;
	case SDLK_LSHIFT:
		return KEY_LSHIFT;
	}

	return UNDEFINED;

}