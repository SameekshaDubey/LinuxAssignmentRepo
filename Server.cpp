// server code
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    int listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);      
    serverAddr.sin_addr.s_addr = INADDR_ANY; 

    if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        close(listenSock);
        return -1;
    }

    if (listen(listenSock, SOMAXCONN) == -1) {
        std::cerr << "Listen failed" << std::endl;
        close(listenSock);
        return -1;
    }

    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSock = accept(listenSock, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSock == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        close(listenSock);
        return -1;
    }

    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(clientSock, buffer, 1024, 0)) > 0) {
        send(clientSock, buffer, bytesReceived, 0);
    }

    close(clientSock);
    close(listenSock);

    return 0;
}