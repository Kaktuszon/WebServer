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

		~Server();

		int runServer();
		void setupServer();
		void getAddressSettings();
		bool setMessage(std::string fileName);
};

