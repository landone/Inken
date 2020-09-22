#include "GameManager.h"
#include "Evt_Keyboard.h"

#include <iostream>

void onConnect(int client) {
	printf("Client%d connected\n", client);
}

void onDisconnect(int client) {
	printf("Client%d disconnected\n", client);
}

void GameManager::onPress(Button& btn, void* data) {

	GameManager& self = *((GameManager*)data);
	if (&self.connect[0] == &btn) {
		printf("SERVER\n");
		self.server = new LensceServer(5001, 10, 10);
		self.server->connectCallback(onConnect);
		self.server->disconnectCallback(onDisconnect);
		self.server->start();
	}
	else {
		printf("CLIENT\n");
		self.client = new LensceClient();
		self.client->connect("192.168.1.127", 5001);
	}

	self.connect[0].setVisibility(false);
	self.connect[1].setVisibility(false);

}

GameManager::GameManager() {

	display = Display::getGlobal();

	connect[0].setRelPos(-0.5, 0);
	connect[0].setAbsPos(-50, 0);
	connect[0].setRelSize(0, 0);
	connect[0].setAbsSize(100, 100);
	connect[0].setCallback(onPress, this);

	connect[1].setRelPos(0.5, 0);
	connect[1].setAbsPos(-50, 0);
	connect[1].setRelSize(0, 0);
	connect[1].setAbsSize(100, 100);
	connect[1].setCallback(onPress, this);

}