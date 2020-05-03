#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>

#include <Ws2tcpip.h>
#include <Windows.h>

#include <fstream>
#include <sstream>

#include "server.h"

#define PORT "8080"

Server::Server() {
	run = true;
	getAddress();

	//Create the winsock
	std::cout << "Initialising Winsock...\n";
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0) {
		std::cout << "Failed. Error code: " << WSAGetLastError() << std::endl;
		WSACleanup();
	}

	//Get address information
	std::cout << "Get address information...\n";
	if (getaddrinfo(NULL, PORT, &m_address, &m_result) != 0) {
		std::cout << "Could not get address information. Error code: " << WSAGetLastError() << std::endl;
		WSACleanup();
	}

	//Create socket
	std::cout << "Create listening socket...\n";
	if ((m_listen_socket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol)) == INVALID_SOCKET) {
		std::cout << "Could not create socket. Error code: " << WSAGetLastError() << std::endl;
		closesocket(m_listen_socket);
		WSACleanup();
	}

	//Bind listening socket
	std::cout << "Binding listening socket...\n";
	if (bind(m_listen_socket, m_result->ai_addr, (int)m_result->ai_addrlen) != 0) {
		std::cout << "Could not bind socket. Error code: " << WSAGetLastError() << std::endl;
		closesocket(m_listen_socket);
		WSACleanup();
	}

	freeaddrinfo(m_result);

	//Listen for connection to the listening socket
	std::cout << "Listen for connection...\n";
	if (listen(m_listen_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Could not listen. Error code: " << WSAGetLastError() << std::endl;
		closesocket(m_listen_socket);
		WSACleanup();
	}

	//Accept connection
	std::cout << "Accepting connection from client...\n";
	if ((m_client_socket = accept(m_listen_socket, NULL, NULL)) == INVALID_SOCKET) {
		std::cout << "Could not accept client connection. Error code: " << WSAGetLastError() << std::endl;
		closesocket(m_client_socket);
		WSACleanup();
	}

	closesocket(m_listen_socket);
}

Server::~Server() {
	closesocket(m_listen_socket);
	closesocket(m_client_socket);
	WSACleanup();
}

void Server::getAddress() {
	ZeroMemory(&m_address, sizeof(m_address));
	m_address.ai_family = AF_INET;
	m_address.ai_socktype = SOCK_STREAM;
	m_address.ai_protocol = IPPROTO_TCP;
	m_address.ai_flags = AI_PASSIVE;
}

void Server::runServer() {
	int iResult;
	char buffer[2000];
	int iSendResult;

	std::cout << "Started running server...\n\n\n";

	do {
		iResult = recv(m_client_socket, buffer, strlen(buffer), 0);

		if (iResult > 0) { //Server recieved information from client
			iSendResult = send(m_client_socket, m_messageToSend.c_str(), (int)strlen(m_messageToSend.c_str()), 0);
			if (iSendResult == SOCKET_ERROR) {
				std::cout << "Failed to send message to client. Error code: " << WSAGetLastError() << std::endl;
				closesocket(m_client_socket);
				WSACleanup();
			}
			std::cout << "Sent data to client.\n";
		}
		else if (iResult == 0) {
			std::cout << "Closing connection...\n";
		}
		else {
			std::cout << "Failed to recive PING from client. Error code: " << WSAGetLastError() << std::endl;
		}
	} while (iResult > 0);

	std::cout << "\n\n!-- SERVER IS CLOSING --!\n\n";

	WSACleanup();
	Server::~Server();
}

void Server::setMessage(std::string fileName) {
	std::ifstream file(fileName.c_str());
	std::string head = "HTTP/1.1 200 OK\nServer: kaktusskitserver\nContent-Type: text\nContent-Length: ";
	std::string body = "";
	std::string s = "";

	std::string message;

	if (file.is_open()) {
		while (std::getline(file, s)) {
			body = body + s;
		}
	}
	else {
		std::cout << "Could not find file... Program should exit!\n";
		run = false;
	}

	file.close();
	message.append(head);
	message.append(std::to_string(strlen(body.c_str())));
	message.append("\n\n");
	message.append(body);

	m_messageToSend = message;
}