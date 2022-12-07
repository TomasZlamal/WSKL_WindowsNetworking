#ifndef CL_SOCKET_HPP
#define CL_SOCKET_HPP
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <string>
#include <sstream>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include "../../../Errors.hpp"
#define DEBUG_MODE
#ifdef DEBUG_MODE
#define log(x) std::cout << x
#else
#define log(x)
#endif

#define WIN32_LEAN_AND_MEAN
namespace WSKL
{
class ClientSocket{
public:
    ClientSocket(const char* serverName, const char * port);
    ~ClientSocket();
    // communication with a server is generally done in this order
    void connectToNetwork();
    void sendData(const char* msg);
    void shutDownSendingSocket();
    std::string recieveData();
    void cleanUp();
    static void wait();
private:
    SOCKET m_ConnectSock;
    WSADATA m_wsaData;
    addrinfo* m_ptr;
    int m_connection;
    int m_iresult;
    addrinfo* m_result;
    addrinfo m_hints;

};
}
#endif