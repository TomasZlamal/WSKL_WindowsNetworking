# WSKL_WindowsNetworking

WSKL is a simple C++ library for easy client-server networking. I designed this simple library with simplicity in mind.
I personally recommend you to try out the base WSKL Socket classes and then extend one of the classes and implement your own class, suited for your needs.
If you don't want any logging, undefine DEBUG_MODE. This is a very bare-bones library, so use mainly it for learning.

If you want an example of code, here is code for the client side:
```c++
#include "Client/include/networking/Sockets/ClSocket.hpp"

int main() {
    std::string output;
    try {
         WSKL::ClientSocket Cl("PUT SERVER IP HERE", "PORT HERE, IF EMPTY DEFAULT PORT IS SELECTED");
         Cl.connectToNetwork();
         Cl.sendData("Test Client Data Send");
         Cl.shutDownSendingSocket();
         output = Cl.recieveData();
         log("\nContent: " << output << "\n");
         Cl.cleanUp();
    }
    catch (WSKL::SocketException error) {
        log("ERROR: " << error.c_str() << std::endl);
    }
#ifdef DEBUG_MODE
    WSKL::ClientSocket::wait();
#endif
    return 0;
}
```
And for the server side:
```c++
#include "Server/include/networking/ServerSocket.hpp"
void processClientData(const std::string& data, std::string& response) {
    // code for processing inputs here
    response = "Information accepted";
}
int main(){ 
    try {
        std::string input_buf;
        std::string response_buf;
        WSKL::ServerSocket server("");
        while (1) {
            server.setUpSocket();
            server.listenForClient();
            input_buf = server.getDataFromClient();
            processClientData(input_buf, response_buf);
            server.send_data(response_buf);
            server.closeConnection();
        }
        server.cleanUp();
    }
    catch (WSKL::SocketException exceptionObject) {
        log("SocketException, information provided: " << exceptionObject.c_str());
    }
#ifdef DEBUG_MODE
    WSKL::ServerSocket::wait();
#endif
}
```
