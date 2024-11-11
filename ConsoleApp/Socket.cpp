#include "Socket.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::Socket() : sockfd(-1) {}

Socket::~Socket() {
    if (sockfd != -1) {
        close(sockfd);
    }
}

bool Socket::createSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    return sockfd >= 0;
}

int Socket::getSock() const {
    return sockfd;
}

bool Socket::connectToServer(const std::string &ip, int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (connect(sockfd, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed." << std::endl;
        return false;
    }
    return true;
}

void Socket::sendData(const std::string &data) {
    send(sockfd, data.c_str(), data.size(), 0);
}
