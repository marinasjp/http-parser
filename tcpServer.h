#ifndef INCLUDED_HTTP_TCPSERVER_LINUX
#define INCLUDED_HTTP_TCPSERVER_LINUX

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>

namespace http{

    class TcpServer{
    public:
        TcpServer(std::string ip_address, int port);
        ~TcpServer();
        int startListen();

    private:
        std::string ipv4_addr;
        int port;
        int server_socket;
        int new_socket;
        int size;
        long msg;
        struct sockaddr_in addr;
        std::string response;
        int bindServer();
        void closeServer();
        int acceptConnection(int &new_socket);
        std::string buildResponse();
        int sendResponse();
    };

} // namespace http

#endif