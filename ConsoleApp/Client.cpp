#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define HOST "127.0.0.1"

// Function to determine MIME type
std::string getFileMimeType(const std::string &filePath) {
    std::string command = "file --mime-type -b " + filePath;
    char buffer[128];
    std::string mimeType;
    FILE *pipe = popen(command.c_str(), "r");
    if (pipe) {
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            mimeType += buffer;
        }
        pclose(pipe);
    }
    mimeType.erase(mimeType.find_last_not_of("\n\r") + 1);
    return mimeType;
}

// Function to send file
void sendFileToServer(const std::string &host, int port, const std::string &caption, const std::string &filePath) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    struct sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr);

    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection failed");
        close(sock);
        return;
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        close(sock);
        return;
    }

    std::ostringstream body;
    std::string boundary = "----Boundary";

    // Build request body
    body << "--" << boundary << "\r\n"
         << "Content-Disposition: form-data; name=\"caption\"\r\n\r\n"
         << caption << "\r\n"
         << "--" << boundary << "\r\n";

    std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);
    std::string mimeType = getFileMimeType(filePath);
    body << "Content-Disposition: form-data; name=\"File\"; filename=\"" << fileName << "\"\r\n"
         << "Content-Type: " << mimeType << "\r\n\r\n";

    // Read file content
    std::ostringstream fileContent;
    fileContent << file.rdbuf();
    body << fileContent.str() << "\r\n";
    body << "--" << boundary << "--\r\n";

    std::string bodyStr = body.str();
    std::ostringstream request;

    // Build HTTP POST request
    request << "POST /upload/upload HTTP/1.1\r\n"
            << "Host: " << host << ":" << port << "\r\n"
            << "Content-Type: multipart/form-data; boundary=" << boundary << "\r\n"
            << "Content-Length: " << bodyStr.size() << "\r\n\r\n"
            << bodyStr;

    std::string requestStr = request.str();

    // Send request to server
    send(sock, requestStr.c_str(), requestStr.size(), 0);

    // Handle server response
    char buffer[1024] = {0};
    ssize_t bytesRead = read(sock, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "Server response:\n" << std::string(buffer, bytesRead) << std::endl;
    } else {
        std::cerr << "No response or read error from server." << std::endl;
    }

    close(sock);
}

int main() {
    std::string caption, filePath;
    int port;

    std::cout << "Select server port (8081 for Tomcat, 8082 for custom Java server): ";
    std::cin >> port;
    std::cin.ignore(); // Clear the newline from the input buffer

    std::cout << "Enter a caption for the image: ";
    std::getline(std::cin, caption);

    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);

    sendFileToServer(HOST, port, caption, filePath);

    return 0;
}
