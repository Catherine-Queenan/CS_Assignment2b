#ifndef SOCKET_H
#define SOCKET_H

#include <string>

class Socket {
public:
    Socket();
    ~Socket();

    bool createSocket(); // Declaration
    int getSock() const;

    bool connectToServer(const std::string &ip, int port); // Declaration
    void sendData(const std::string &data);

private:
    int sockfd;
};

#endif // SOCKET_H
