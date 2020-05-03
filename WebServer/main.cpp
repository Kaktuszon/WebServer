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

	server.setMessage("index.html");
	if (server.run) {
		server.runServer();
	}

	server.~Server();
	return 0;
}