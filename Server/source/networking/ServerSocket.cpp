#include "../../include/networking/ServerSocket.hpp"

WSKL::ServerSocket::ServerSocket()
{
    ServerSocket(DEFAULT_PORT);
}

WSKL::ServerSocket::ServerSocket(const char* port)
{
    WSADATA wsaData;

    m_ListenSocket = INVALID_SOCKET;
    m_ClientSocket = INVALID_SOCKET;

    m_result = NULL;

    int iSendm_result;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    m_iresult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (m_iresult != 0) 
        throw SocketException("WSAStartup failed with error: " + m_iresult);

    ZeroMemory(&m_hints, sizeof(m_hints));
    m_hints.ai_family = AF_INET;
    m_hints.ai_socktype = SOCK_STREAM;
    m_hints.ai_protocol = IPPROTO_TCP;
    m_hints.ai_flags = AI_PASSIVE;
    m_iresult = getaddrinfo(NULL, port, &m_hints, &m_result);
    if (m_iresult != 0) {
        WSACleanup();
        throw SocketException("getaddrinfo failed with error: " + m_iresult);
    }
    m_ListenSocket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
    if (m_ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(m_result);
        WSACleanup();
        throw SocketException("socket failed with error: " + WSAGetLastError());
    }
}

void WSKL::ServerSocket::setUpSocket()
{
    m_iresult = getaddrinfo(NULL, DEFAULT_PORT, &m_hints, &m_result);
    if (m_iresult != 0) {
        WSACleanup();
        throw SocketException("getaddrinfo failed with error: " + m_iresult);
    }

    // Initialize the ListenSocket for the server to listen for client connections.
    m_ListenSocket = socket(m_result->ai_family, m_result->ai_socktype, m_result->ai_protocol);
    if (m_ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(m_result);
        WSACleanup();
        throw SocketException("socket failed with error: " + WSAGetLastError());
    }

    // Setup the TCP listening socket
    m_iresult = bind(m_ListenSocket, m_result->ai_addr, (int)m_result->ai_addrlen);
    if (m_iresult == SOCKET_ERROR) {
        freeaddrinfo(m_result);
        closesocket(m_ListenSocket);
        WSACleanup();
        throw SocketException("bind failed with error: "+ WSAGetLastError());
    }

    freeaddrinfo(m_result);
}

void WSKL::ServerSocket::listenForClient()
{
    m_iresult = listen(m_ListenSocket, SOMAXCONN);
    if (m_iresult == SOCKET_ERROR) {
        closesocket(m_ListenSocket);
        WSACleanup();
        throw SocketException("listen failed with error: " + WSAGetLastError());
    }
    log("Waiting for client operation... " << std::endl);
    // Accept a client socket
    m_ClientSocket = accept(m_ListenSocket, NULL, NULL);
    if (m_ClientSocket == INVALID_SOCKET) {
        closesocket(m_ListenSocket);
        WSACleanup();
        throw SocketException("accept failed with error: " + WSAGetLastError());
    }
    closesocket(m_ListenSocket);
}
std::string WSKL::ServerSocket::getDataFromClient() {
    std::stringstream output;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    do {
        m_iresult = recv(m_ClientSocket, recvbuf, recvbuflen, 0);
        if (m_iresult > 0) {
            log("Bytes recieved: " << m_iresult << std::endl);
            if (m_iresult > 1) 
                for (int i = 0; i < m_iresult; i++) 
                    output << recvbuf[i];
        }
        else if (m_iresult == 0)
            log("No more client input...\n");
        else {
            closesocket(m_ClientSocket);
            WSACleanup();
            throw SocketException("recv failed with error " + WSAGetLastError());
        }

    } while (m_iresult > 0);
    return output.str();
}

void WSKL::ServerSocket::closeConnection()
{
    m_iresult = shutdown(m_ClientSocket, SD_SEND);
    if (m_iresult == SOCKET_ERROR) {
        closesocket(m_ClientSocket);
        WSACleanup();
        throw SocketException("shutdown failed with error " + WSAGetLastError());
    }
}

void WSKL::ServerSocket::cleanUp()
{
    closesocket(m_ClientSocket);
    WSACleanup();
}
void WSKL::ServerSocket::wait() {
    // for debugging purposes, so cmd doesn't immediately close
    int a;
    std::cin >> a;
}

void WSKL::ServerSocket::send_data(const std::string& str)
{
    int iSendResult = send(m_ClientSocket, str.c_str(), str.length(), 0);
    if (iSendResult == SOCKET_ERROR) {
        closesocket(m_ClientSocket);
        WSACleanup();
        throw SocketException("send failed with error:" + WSAGetLastError());
    }
    log("Bytes sent: " << iSendResult << std::endl);
}