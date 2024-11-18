#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <iomanip>

#define HOST "127.0.0.1"

//Function to get date
std::string getCurrentDate() {
    // Get the current time as a system clock time_point
    auto now = std::chrono::system_clock::now();

    // Convert to time_t (which is a more usable form for broken-down time)
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    // Convert time_t to tm (broken down time)
    std::tm now_tm = *std::localtime(&now_time_t);

    // Format the date as "yyyy-mm-dd"
    std::stringstream date_stream;
    date_stream << (now_tm.tm_year + 1900) << "-"       // Year
                << std::setw(2) << std::setfill('0') << (now_tm.tm_mon + 1) << "-"  // Month
                << std::setw(2) << std::setfill('0') << now_tm.tm_mday;          // Day

    return date_stream.str();
}

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
    serverAddress.sin_addr.s_addr = inet_addr(HOST);

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

    std::stringstream body;
    std::string boundary = "----Boundary";

    // Build request body
    body << "--" << boundary << "\r\n"
         << "Content-Disposition: form-data; name=\"caption\"\r\n\r\n"
         << caption << "\r\n"
         << "--" << boundary << "\r\n";

    body << "Content-Disposition: form-data; name=\"date\"\r\n\r\n"
         << getCurrentDate() << "\r\n"
         << "--" << boundary << "\r\n";

    std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);
    std::string mimeType = getFileMimeType(filePath);
    body << "Content-Disposition: form-data; name=\"File\"; filename=\"" << fileName << "\"\r\n"
         << "Content-Type: " << mimeType << "\r\n\r\n";

    // Read file content
    std::stringstream fileContent;
    fileContent << file.rdbuf();
    std::string endBoundary =  "\r\n--" + boundary + "--\r\n";
    body << fileContent.str() << "\r\n";
    body << "--" << boundary << "--\r\n";

    std::string bodyStr = body.str();
    std::stringstream request;

    // Build HTTP POST request
    request << "POST /upload/upload HTTP/1.1\r\n"
            << "Host: " << host << ":" << port << "\r\n"
            << "Content-Type: multipart/form-data; boundary=" << boundary << "\r\n"
            << "Content-Length: " << (bodyStr.size()) << "\r\n"
            << "Connection: close\r\n\r\n";
            // << bodyStr;

    std::string requestStr = request.str();
    // Send request to server
    size_t totalLength = requestStr.length();
    send(sock, requestStr.c_str(), totalLength, 0);
usleep(500);

send(sock, bodyStr.c_str(), bodyStr.size(), 0);

    shutdown(sock, SHUT_WR);

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

    std::cout << "Select server port (8081 for Tomcat upload servlet, 8082 for Java UploadServer, 8083 for C++ UploadServer): ";
    std::cin >> port;
    std::cin.ignore(); // Clear the newline from the input buffer

    std::cout << "Enter a caption for the file: ";
    std::getline(std::cin, caption);

    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);

    sendFileToServer(HOST, port, caption, filePath);

    return 0;
}