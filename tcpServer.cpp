#include <tcpServer.h>
#include <iostream>
#include <sstream>
#include <unistd.h>


const int BUFFER_SIZE = 30720;


namespace http
{
    
    TcpServer::TcpServer(std::string ip_address, int port) : ipv4_addr(ip_address), port(port), server_socket(), new_socket(),
                                                             msg(),
                                                             addr(),
                                                             response(buildResponse()){

        addr.sin_family = AF_INET; // IPv4
        addr.sin_addr.s_addr = inet_addr(ipv4_addr.c_str()); 
        addr.sin_port = htons(port);
        bindServer();
    }

    TcpServer::~TcpServer(){
        closeServer();
    }

    int TcpServer::bindServer(){
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1) {
            return 1;
        }


        if (bind(server_socket, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
            return 2;
        }
        return 0;
    }

    void TcpServer::closeServer(){
        close(server_socket);
        close(new_socket);
        exit(0);
    }

    int TcpServer::startListen(){
        if (listen(server_socket, 20) < 0){
            return 3;
        }

        int bytesReceived;
        

        while (true){
            acceptConnection(new_socket);

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(new_socket, buffer, BUFFER_SIZE);
            if (bytesReceived < 0){
                return 6;
            }

            sendResponse();

            close(new_socket);
        }
        return 0;
    }

    int TcpServer::acceptConnection(int &new_socket){
        u_int size = sizeof(addr);
        new_socket = accept(server_socket, (sockaddr *)&addr, &size);
        if (new_socket < 0){
            return 4;
        }
        return 0;
    }

    std::string TcpServer::buildResponse(){
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    int TcpServer::sendResponse(){
        long bytesSent = write(new_socket, response.c_str(), response.size());

        if (bytesSent == response.size()){
            return 7;
        }
        return 0;
    }

} // namespace http