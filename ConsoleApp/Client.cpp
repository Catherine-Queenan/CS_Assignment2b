#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <filesystem>
#include <map>
#include <iterator>
#include "Socket.h"

// Function to get the file content type based on file extension
std::string getContentType(const std::string &filePath) {
    // You can extend this based on the file types you need
    std::map<std::string, std::string> mimeTypes = {
        {".jpg", "image/jpeg"},
        {".jpeg", "image/jpeg"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".txt", "text/plain"}
    };

    std::filesystem::path file(filePath);
    std::string extension = file.extension().string();

    // Check if the extension exists in the map and return the corresponding MIME type
    if (mimeTypes.find(extension) != mimeTypes.end()) {
        return mimeTypes[extension];
    } else {
        return "application/octet-stream"; // Default MIME type
    }
}

int main() {
    Socket socket;

    if (!socket.createSocket()) {
        std::cerr << "Socket creation failed." << std::endl;
        return -1;
    }

    std::cout << "Socket created successfully." << std::endl;

    // Connect to server
    std::string ip = "127.0.0.1";
    int port = 8080;
    std::cout << "Connecting to server " << ip << " on port " << port << "..." << std::endl;

    if (!socket.connectToServer(ip, port)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return -1;
    }

    std::cout << "Connected to server." << std::endl;

    // Get user input for caption and file path
    std::string caption, filePath, date;
    std::cout << "Enter a caption for the image: ";
    std::getline(std::cin, caption);

    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);

    // Read the file into a string (binary data)
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return -1;
    }
    std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Generate the boundary for the multipart form-data
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";

std::time_t t = std::time(nullptr);
std::tm* now = std::localtime(&t);
std::ostringstream oss;
oss << (now->tm_year + 1900) << '-' 
    << (now->tm_mon + 1) << '-'
    << now->tm_mday;
date = oss.str();
    
    // Build the POST request data with multipart/form-data
    std::ostringstream body;
    body << "--" << boundary << "\r\n"
         << "Content-Disposition: form-data; name=\"caption\"\r\n\r\n"
         << caption << "\r\n"
         << "--" << boundary << "\r\n"
         << "Content-Disposition: form-data; name=\"date\"\r\n\r\n"
         << date << "\r\n"
         << "--" << boundary << "\r\n"
         << "Content-Disposition: form-data; name=\"File\"; filename=\""
         << std::filesystem::path(filePath).filename().string() << "\"\r\n"
         << "Content-Type: " << getContentType(filePath) << "\r\n\r\n"
         << fileData << "\r\n"
         << "--" << boundary << "--\r\n";

    // Send the data to the server
    std::string postData = body.str();
    std::cout << "Sending data to server..." << std::endl;
    socket.sendData(postData);

    std::cout << "Data sent successfully!" << std::endl;

    return 0;
}
