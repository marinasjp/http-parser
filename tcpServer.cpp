#include <tcpServer.h>

#include <iostream>
#include <sstream>
#include <unistd.h>

namespace
{
    const int BUFFER_SIZE = 30720;

    void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    void exitWithError(const std::string &errorMessage)
    {
        log("ERROR: " + errorMessage);
        exit(1);
    }
}

namespace http
{
    
    TcpServer::TcpServer(std::string ip_address, int port) : ipv4_addr(ip_address), port(port), server_socket(), new_socket(),
                                                             msg(),
                                                             socketAddr(),
                                                             response(buildResponse())
    {
        socketAddr.sin_family = AF_INET;
        socketAddr.sin_port = htons(port);
        socketAddr.sin_addr.s_addr = inet_addr(ipv4_addr.c_str());

        

        if (startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(socketAddr.sin_port);
        }
    }

    TcpServer::~TcpServer()
    {
        closeServer();
    }

    int TcpServer::startServer()
    {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0)
        {
            exitWithError("Cannot create socket");
            return 1;
        }

        if (bind(server_socket, (sockaddr *)&socketAddr, sizeof(socketAddr)) < 0)
        {
            exitWithError("Cannot connect socket to address");
            return 1;
        }

        return 0;
    }

    void TcpServer::closeServer()
    {
        close(server_socket);
        close(new_socket);
        exit(0);
    }

    void TcpServer::startListen()
    {
        if (listen(server_socket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }

        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(socketAddr.sin_addr) << " PORT: " << ntohs(socketAddr.sin_port) << " ***\n\n";

        int bytesReceived;

        while (true)
        {
            acceptConnection(new_socket);

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(new_socket, buffer, BUFFER_SIZE);
            if (bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }

            std::ostringstream ss;
            ss << "------ Received Request from client ------\n\n";

            sendResponse();

            close(new_socket);
        }
    }

    void TcpServer::acceptConnection(int &new_socket)
    {
        u_int size = sizeof(socketAddr);
        new_socket = accept(server_socket, (sockaddr *)&socketAddr, &size);
        if (new_socket < 0)
        {
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(socketAddr.sin_addr) << "; PORT: " << ntohs(socketAddr.sin_port);
            exitWithError(ss.str());
        }
    }

    std::string TcpServer::buildResponse()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    void TcpServer::sendResponse()
    {
        long bytesSent;

        bytesSent = write(new_socket, response.c_str(), response.size());

        if (bytesSent == response.size())
        {
            log("------ Server Response sent to client ------\n\n");
        }
        else
        {
            log("Error sending response to client");
        }
    }

} // namespace http