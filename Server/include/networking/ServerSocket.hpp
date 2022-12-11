#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "../../Errors.hpp"
#include <string_view>
#pragma comment (lib, "Ws2_32.lib")
constexpr bool debug_mode = false;
inline void log(std::string str) {
	if(debug_mode)
		std::cout << str;
}

#define WIN32_LEAN_AND_MEAN
constexpr int DEFAULT_BUFLEN = 512;
constexpr const char* DEFAULT_PORT = "27015";
namespace WSKL {
	class ServerSocket
	{
	public:
		ServerSocket();
		ServerSocket(const char* port);
		~ServerSocket();
		void setUpSocket();
		void listenForClient();
		std::string getDataFromClient();
		void closeConnection();
		void cleanUp();
		void send_data(const std::string& str);
		static void wait();
	private:
		void initSocket(const char* port);
		void bindSocket(const char* port);
		int m_iresult;
		SOCKET m_ListenSocket;
		SOCKET m_ClientSocket;
		bool m_isClosed;
		addrinfo* m_result;
		addrinfo m_hints;
	};

}
#endif