#pragma once

#include "LensceSocket.h"

#include <string>
#include <thread>
#include <mutex>

class LensceClient {
public:

	/* Maximum bytes to receive at a time */
	static const int MAX_READ = 1024;

	LensceClient();
	~LensceClient();

	/* Connect to an address */
	bool connect(std::string ip, int port);
	/*
	*	Disconnect from any current connection 
	*	Can be used precautionarily.
	*/
	void disconnect();
	/* If TCP connected to host */
	bool isConnected() { return connected; }

	/* Send TCP data to host */
	bool sendTCP(const char* buf, int len);
	/* Send UDP data to host */
	bool sendUDP(const char* buf, int len);

	/* Set callback to handle TCP data */
	void receiveTCPCallback(void(*f)(char*, int));
	/* Set callback to handle UDP data */
	void receiveUDPCallback(void(*f)(char*, int));
	/* Set callback to handle disconnections */
	void disconnectCallback(void(*f)());

private:

	friend void LensceClientTCPThread(LensceClient&);
	friend void LensceClientUDPThread(LensceClient&);

	LensceSocket soc;
	std::thread threads[2];
	std::mutex discMtx; /* Disconnect mutex */

	bool connected = false;

	void(*receiveTCPCbk)(char*, int) = nullptr;
	void(*receiveUDPCbk)(char*, int) = nullptr;
	void(*disconnectCbk)() = nullptr;

};