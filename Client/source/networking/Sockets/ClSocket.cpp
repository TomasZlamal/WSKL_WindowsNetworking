#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#include "../../../include/networking/Sockets/ClSocket.hpp"
constexpr const char* DEFAULT_PORT = "27015";

#pragma comment(lib, "ws2_32.lib")
WSKL::ClientSocket::ClientSocket(const char* serverName, const char* port){
    if (strlen(port) < 1)
        port = DEFAULT_PORT;
    m_ptr = NULL;
    m_result = NULL;
    m_iresult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
    if (m_iresult != 0)
        throw "ERROR WITH WSA-STARTUP";

    ZeroMemory(&m_hints, sizeof(m_hints));
    m_hints.ai_family = AF_UNSPEC;
    m_hints.ai_socktype = SOCK_STREAM;
    m_hints.ai_protocol = IPPROTO_TCP;
    m_hints.ai_flags = AI_PASSIVE;
    m_iresult = getaddrinfo(serverName, port, &m_hints, &m_result);
    if (m_iresult != 0) {
        WSACleanup();
        throw SocketException("getaddrinfo failed: " + m_iresult);
    }
    m_ConnectSock = INVALID_SOCKET;
    m_ptr = m_result;
    if (!m_ptr)
        throw SocketException("getaddrinfo failed, pointer m_ptr left unitialized");
    
}
WSKL::ClientSocket::~ClientSocket()
{
    
}
void WSKL::ClientSocket::connectToNetwork() {
    for (m_ptr = m_result; m_ptr != NULL; m_ptr = m_ptr->ai_next) {

        // Create a ConnectSocket for connecting to server
        m_ConnectSock = socket(m_ptr->ai_family, m_ptr->ai_socktype,
            m_ptr->ai_protocol);
        if (m_ConnectSock == INVALID_SOCKET) {
            WSACleanup();
            throw SocketException("socket failed with error: " + WSAGetLastError());
        }

        // Connect to server
        m_iresult = connect(m_ConnectSock, m_ptr->ai_addr, (int)m_ptr->ai_addrlen);
        if (m_iresult == SOCKET_ERROR) {
            closesocket(m_ConnectSock);
            m_ConnectSock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(m_result);

    if (m_ConnectSock == INVALID_SOCKET) {
        WSACleanup();
        throw SocketException("Unable to connect to server");
    }
}
constexpr int buflen = 512;
std::string WSKL::ClientSocket::recieveData()
{
    std::stringstream output;
    int recvbuflen = buflen;

    char recvbuf[buflen];

    // Receive data
    do {
        m_iresult = recv(m_ConnectSock, recvbuf, recvbuflen, 0);
        for (int i = 0; i < m_iresult; i++)
            output << recvbuf[i];
        }
        if (m_iresult > 0)
            log("Bytes recieved: " << m_iresult << std::endl);
        else if (m_iresult == 0)
            log("Connection closed" << std::endl);
        else {
            throw SocketException("recv failed: " + WSAGetLastError());
        }
    } while (m_iresult > 0);
    return output.str();
}

void WSKL::ClientSocket::cleanUp()
{
    closesocket(m_ConnectSock);
    WSACleanup();
}

void WSKL::ClientSocket::wait()
{
    int a;
    std::cin >> a;
}

void WSKL::ClientSocket::sendData(const char* msg)
{
    m_iresult = send(m_ConnectSock, msg, (int)strlen(msg), 0);
    if (m_iresult == SOCKET_ERROR) {
        closesocket(m_ConnectSock);
        WSACleanup();
        throw SocketException("send failed: " + WSAGetLastError());
    }

    log("Bytes sent: " << m_iresult << std::endl);
}

void WSKL::ClientSocket::shutDownSendingSocket()
{
    m_iresult = shutdown(m_ConnectSock, SD_SEND);
    if (m_iresult == SOCKET_ERROR) {
        closesocket(m_ConnectSock);
        WSACleanup();
        throw SocketException("shutdown failed: " + WSAGetLastError());
    }
}
