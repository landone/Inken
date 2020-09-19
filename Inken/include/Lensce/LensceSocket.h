#pragma once

#ifdef LENSCE_LINUX
#include <netinet/in.h>
#else
#include <WinSock2.h>
#endif

/*
Lensce_Socket:
Interface for basic socket functionality.
Sockets are NOT closed out of scope, MUST disconnect.
Closing socket on deconstruct may close sockets in use elsewhere.
*/
class LensceSocket {
public:

	friend class LensceServer;

	/* Create socket prepared to connect to host */
	LensceSocket(const char* ip, int port);
	/* Create socket prepared to host */
	LensceSocket(int port);
	/* Create socket without initialization */
	LensceSocket();
	/* Initialize socket to host or connect */
	bool init(const char* ip, int port);

	/* Connect to defined IP, must be initialized */
	bool Connect();
	/* Disconnect from current TCP socket */
	void Disconnect();
	/* Close UDP socket */
	void CloseUDP();
	/* Send data over TCP socket */
	bool SendTCP(const char* buf, int len);
	/* Receive data from TCP socket */
	bool RecvTCP(char* buf, int max_len, int& bytes_read);
	/* Send data over UDP socket */
	bool SendUDP(const char* buf, int len);
	/* Receive data from UDP socket */
	sockaddr RecvUDP(char* buf, int max_len, int& bytes_read);
	/* Bind sockets to local ports */
	bool Bind();
	/* Prepare TCP socket for listening */
	bool ListenTCP(const int backlog);
	/* Accept TCP connection */
	LensceSocket AcceptTCP();

	static void printErrors(bool);

	/* If TCP connection is stable */
	bool isConnected() { return connected; };
	/* Return TCP socket */
	int getTCPSocket() { return tcp; }
	/* Return UDP socket */
	int getUDPSocket() { return udp; }
	/* Return TCP socket address */
	struct sockaddr_in getAddr() { return tcpAddr; }

	/* Clean up WSA initialization */
	static void CleanUp();

protected:

	static bool PRINT_ERRORS;

	int tcp = { 0 };
	int udp = { 0 };
	struct sockaddr_in tcpAddr = { 0 };
	struct sockaddr_in udpAddr = { 0 };
	bool connected = false;

	static void printError(const char*);

};