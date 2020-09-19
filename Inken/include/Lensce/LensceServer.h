#pragma once

#include "LensceSocket.h"

#include <thread>
#include <mutex>

class LensceServer {
public:

	/* Maximum bytes to receive at a time */
	static const int MAX_READ = 1024;

	/*
	*	Prepare server information.
	*	Backlog is queue size for incoming connections.
	*/
	LensceServer(int port, int maxClients, int backlog);
	~LensceServer();

	/* Bind to ports and start accepting clients. */
	bool start();
	/* If server is still accepting connections */
	bool isRunning() { return soc.isConnected(); }
	/* Stop server and disconnect all clients */
	void stop();

	/* Send TCP data to a client */
	bool sendTCP(int client, const char* data, int size);
	/* Send TCP data to all clients */
	void sendTCPAll(const char* data, int size);
	/* Send UDP data to a client */
	bool sendUDP(int client, const char* data, int size);
	/* Send UDP data to all clients */
	void sendUDPAll(const char* data, int size);
	/*
	*	Flag client to be kicked.
	*	Performed next thread loop.
	*/
	void kick(int client);
	/* Maximum amount of clients to serve at once. */
	int getMaxClients() { return maxClients; }

	/* Set callback to handle TCP data */
	void receiveTCPCallback(void(*f)(int client, char[MAX_READ], int size));
	/* Set callback to handle UDP data */
	void receiveUDPCallback(void(*f)(int client, char[MAX_READ], int size));
	/* Set callback to handle connections */
	void connectCallback(void(*f)(int client));
	/* Set callback to handle disconnections */
	void disconnectCallback(void(*f)(int client));

private:

	friend void LensceClientLoop(LensceServer&, int);
	friend void LensceServerTCPLoop(LensceServer&);
	friend void LensceServerUDPLoop(LensceServer&);

	struct Client {
		std::thread thr;
		LensceSocket soc;
		bool kicked;
	};

	LensceSocket soc;
	int maxClients;
	int clientCount = 0;
	int backlog = 0;

	void (*receiveTCPCbk)(int, char[MAX_READ], int) = nullptr;
	void (*receiveUDPCbk)(int, char[MAX_READ], int) = nullptr;
	void (*connectCbk)(int) = nullptr;
	void (*disconnectCbk)(int) = nullptr;

	std::thread thrServer[2]; //2 threads for TCP & UDP respectively
	std::mutex clientMtx; //Mutex for changing client count value.
	std::mutex stopMtx; //Mutex for stopping server safely
	Client* thrClients;

	int addClient(LensceSocket);

	static void printError(const char*);

};