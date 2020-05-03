#pragma once
class Server
{
	public:
		WSADATA m_wsa;
		SOCKET m_listen_socket;
		SOCKET m_client_socket;

		struct addrinfo m_address;
		struct addrinfo* m_result = NULL;

		std::string m_messageToSend;

		Server();
		~Server();

		void getAddress();
		int runServer();
		void setMessage(std::string fileName);
};

