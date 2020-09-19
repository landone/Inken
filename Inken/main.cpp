#include <Lensce/LensceServer.h>
#include <Lensce/LensceClient.h>
#include <iostream>
#include <string>

void onConnect(int client) {

	printf("Client%d connected\n", client);

}

void onDisconnect(int client) {

	printf("Client%d disconnected\n", client);

}

void onServerTCP(int client, char* buf, int size) {

	printf("Client%d: ", client);
	for (int i = 0; i < size; i++) {
		printf("%c", buf[i]);
	}
	printf("\n");

}

void onClientTCP(char* buf, int size) {

	printf("Server: ");
	for (int i = 0; i < size; i++) {
		printf("%c", buf[i]);
	}
	printf("\n");

}

int main() {

	LensceSocket::printErrors(true);
	std::string input;
	int port = 4000;
	int maxClients = 10;
	bool server = false;
	while(true) {

		std::getline(std::cin, input);
		if (input.compare("s") == 0) {
			
			printf("Server Mode\n");
			server = true;
			break;

		}
		else if (input.compare("c") == 0) {

			printf("Client Mode\n");
			server = false;
			break;

		}

	}

	if (server) {

		LensceServer server(port, maxClients, maxClients);
		server.connectCallback(onConnect);
		server.disconnectCallback(onDisconnect);
		server.receiveTCPCallback(onServerTCP);
		server.start();
		while (server.isRunning()) {

			std::getline(std::cin, input);
			if (input.compare("exit") == 0) {
				break;
			}
			else {
				server.sendTCPAll(input.c_str(), input.size());
			}

		}
		server.stop();

	}
	else {

		LensceClient client;
		client.receiveTCPCallback(onClientTCP);
		if (!client.connect("192.168.1.127", port)) {
			printf("Unable to connect\n");
			return 0;
		}

		while (client.isConnected()) {

			std::getline(std::cin, input);
			if (input.compare("exit") == 0) {
				break;
			}
			else {
				client.sendTCP(input.c_str(), input.size());
			}

		}
		client.disconnect();

	}

	return 0;

}