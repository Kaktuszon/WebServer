/*
	Created by Kaktuszon for fun
*/

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Ws2tcpip.h>
#include <Windows.h>
#include <fstream>
#include <sstream>

#include "server.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT "8080"

int main(int argc, char* argv[]) {
	Server server;

	if (server.setMessage("index.html")) {
		server.getAddressSettings();
		server.setupServer();
		server.listenToSocket();
		server.runServer();
	}

	server.~Server();
	return 0;
}