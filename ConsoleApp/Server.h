#ifndef SERVER_H
#define SERVER_H

#include "Socket.h"

class Server {
private:
    int port;
    Socket socket;

public:
    Server(int port);
    void runServer();
};

#endif
