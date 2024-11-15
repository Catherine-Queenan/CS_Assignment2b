#ifndef CONSOLEAPP_H
#define CONSOLEAPP_H

#include "Socket.h"

class ConsoleApp {
public:
    ConsoleApp(const std::string& serverIp, int serverPort);
    void uploadFile(const std::string& filePath);

private:
    Socket socket;
};

#endif
