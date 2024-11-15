#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

class Server {
public:
    Server(int port) : port(port) {
        serverSock = -1;
        clientSock = -1;
    }

    ~Server() {
        if (serverSock != -1) close(serverSock);
        if (clientSock != -1) close(clientSock);
    }

    bool createSocket() {
        serverSock = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSock < 0) {
            std::cerr << "Socket creation failed." << std::endl;
            return false;
        }
        return true;
    }

    bool bindSocket() {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
        serverAddr.sin_port = htons(port);

        if (bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Bind failed." << std::endl;
            return false;
        }
        return true;
    }

    bool listenForConnections() {
        if (listen(serverSock, 3) < 0) {
            std::cerr << "Listen failed." << std::endl;
            return false;
        }
        std::cout << "Server listening on port " << port << "..." << std::endl;
        return true;
    }

    bool acceptClientConnection() {
        sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSock < 0) {
            std::cerr << "Accept failed." << std::endl;
            return false;
        }
        std::cout << "Client connected." << std::endl;
        return true;
    }

    void handleClient() {
        char buffer[1024] = {0};
        int readVal = read(clientSock, buffer, sizeof(buffer));
        if (readVal > 0) {
            std::cout << "Received from client: " << buffer << std::endl;
            // Here you can parse the multipart data or handle the request accordingly
        }
        close(clientSock);
    }

    void run() {
        if (!createSocket()) return;
        if (!bindSocket()) return;
        if (!listenForConnections()) return;

        while (true) {
            if (acceptClientConnection()) {
                handleClient();
            }
        }
    }

private:
    int serverSock, clientSock;
    sockaddr_in serverAddr;
    int port;
};

int main() {
    Server server(8080);
    server.run();  // Start the server
    return 0;
}
