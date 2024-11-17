// #include "Socket.h"
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <resolv.h>
// #include <unistd.h>
// #include <string.h>
// #include <stdio.h>

// Socket::Socket(int sock)
// {
// 	this->sock = sock;
// }
// char* Socket::getRequest()
// {
//   int rval;
//   char *buf = new char[1024];

//   if ((rval = read(sock, buf, 1024)) < 0){
//     perror("reading socket");
//   }else  {
//     printf("%s\n",buf);
//   }

// 	return buf;
// }
// void Socket::sendResponse(char *res){
// int rval;

//   if ((rval = write(sock, res, strlen(res))) < 0){
//     perror("writing socket");
//   }else  {
//     printf("%s\n",res);
//   }

// 	return;
// }
// Socket::~Socket()
// {
// }


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <ctime>
#include <regex>
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

    // Fallback for empty or unrecognized MIME types
    if (mimeType == "inode/x-empty" || mimeType.empty()) {
        std::regex txtFileRegex(".*\\.txt$", std::regex_constants::icase);
        if (std::regex_match(filePath, txtFileRegex)) {
            return "text/plain";
        }
        // Add more fallbacks as needed for other extensions
    }

    return mimeType;
}

// Function to get the current date in YYYY-MM-DD format
std::string getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
    return std::string(buffer);
}

// Function to send file
void sendFileToServer(const std::string &host, int port, const std::string &caption, const std::string &filePath) {
    std::cout << "Waiting for Callback\n";

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
    std::string boundary = "----Boundary" + std::to_string(std::rand());

    // Build request body
    body << "--" << boundary << "\r\n"
         << "Content-Disposition: form-data; name=\"caption\"\r\n\r\n"
         << caption << "\r\n"
         << "--" << boundary << "\r\n"
         << "Content-Disposition: form-data; name=\"date\"\r\n\r\n"
         << getCurrentDate() << "\r\n"
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

    // Print request for debugging
  std::cout << "--" << boundary << "\n"
              << "Content-Disposition: form-data; name=\"caption\"\n\n"
              << caption << "\n"
              << "--" << boundary << "\n"
              << "Content-Disposition: form-data; name=\"date\"\n\n"
              << getCurrentDate() << "\n"
              << "--" << boundary << "\n"
              << "Content-Disposition: form-data; name=\"File\"; filename=\"" << fileName << "\"\n"
              << "Content-Type: " << mimeType << "\n\n";

    // Send request to server
    send(sock, requestStr.c_str(), requestStr.size(), 0);

    // Handle server response
    char buffer[1024] = {0};
    ssize_t bytesRead = read(sock, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        std::cout << "\nUpload completed!\n" << std::string(buffer, bytesRead) << std::endl;
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

    std::cout << "Enter a caption: ";
    std::getline(std::cin, caption);

    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);

    sendFileToServer(HOST, port, caption, filePath);

    return 0;
}
