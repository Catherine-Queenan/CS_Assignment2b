#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

class Socket {
public:
    int sock;
    sockaddr_in serverAddr;

    void createSocket() {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "Socket creation failed" << std::endl;
            exit(1);
        }
        std::cout << "Socket created successfully." << std::endl;
    }

    void connectToServer(const std::string &ip, int port) {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address or Address not supported" << std::endl;
            exit(1);
        }
        std::cout << "Connecting to server " << ip << " on port " << port << "..." << std::endl;
        if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Connection failed" << std::endl;
            exit(1);
        }
        std::cout << "Connected to server." << std::endl;
    }

    void sendData(const char *data, size_t length) {
        std::cout << "Sending data to server..." << std::endl;
        send(sock, data, length, 0);
    }

    int getSock() { return sock; }

    void closeSocket() { close(sock); }
};

// Helper function to read file content into a string
std::string readFileContent(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        exit(1);
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

int main() {
    // Prompt for caption and file path
    std::string caption, filePath;
    std::cout << "Enter a caption for the image:\n";
    std::getline(std::cin, caption);
    std::cout << "Enter the file path:\n";
    std::getline(std::cin, filePath);

    // Socket setup
    Socket socket;
    socket.createSocket();
    socket.connectToServer("127.0.0.1", 8081);  // Connect to Tomcat on port 8081 (change to 8082 for custom server)

    // Prepare the multipart form data
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string date = "2024-11-10";  // Example date, can be dynamic if needed
    std::string fileContent = readFileContent(filePath);
    
    std::ostringstream postData;
    postData << "--" << boundary << "\r\n"
             << "Content-Disposition: form-data; name=\"caption\"\r\n\r\n"
             << caption << "\r\n"
             << "--" << boundary << "\r\n"
             << "Content-Disposition: form-data; name=\"date\"\r\n\r\n"
             << date << "\r\n"
             << "--" << boundary << "\r\n"
             << "Content-Disposition: form-data; name=\"file\"; filename=\"" << filePath << "\"\r\n"
             << "Content-Type: image/jpeg\r\n\r\n"
             << fileContent << "\r\n"
             << "--" << boundary << "--\r\n";

    std::string data = postData.str();

    // Display the breakdown of the multipart form data
    std::cout << "\n------Boundary\n"
              << "Content-Disposition: form-data; name=\"caption\"\n\n"
              << caption << "\n"
              << "------Boundary\n"
              << "Content-Disposition: form-data; name=\"date\"\n\n"
              << date << "\n"
              << "------Boundary\n"
              << "Content-Disposition: form-data; name=\"file\"; filename=\"" << filePath << "\"\n"
              << "Content-Type: image/jpeg\n\n"
              << "(binary file content here)\n"  // Don't print file content here, as it's binary
              << "------Boundary\n";

    // Construct the HTTP POST request
    std::ostringstream request;
    request << "POST /upload HTTP/1.1\r\n"
            << "Host: localhost\r\n"
            << "Content-Type: multipart/form-data; boundary=" << boundary << "\r\n"
            << "Content-Length: " << data.length() << "\r\n\r\n"
            << data;

    // Send the request
    socket.sendData(request.str().c_str(), request.str().length());

    // Receive the server response
    char buffer[4096] = {0};
    int bytesRead = read(socket.getSock(), buffer, sizeof(buffer) - 1);
    if (bytesRead < 0) {
        std::cerr << "Read error" << std::endl;
    } else {
        std::cout << "\nUpload completed!\n";
        std::cout << "Response from server:\n" << buffer << std::endl;
    }

    // Close the socket
    socket.closeSocket();
    return 0;
}
