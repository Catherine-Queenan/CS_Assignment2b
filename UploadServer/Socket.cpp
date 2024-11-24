#include "Socket.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <resolv.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdexcept>

using namespace std;

// File Descriptor set to negative one as default, for easy check
Socket::Socket(const int sock) : sock(sock), sockFileDescriptor(-1) {
  // Creating new socket using socket system call
  // AF_INET: IPv4 addressing
  // SOCK_STREAM: TCP socket
  // 0: use default protocol for socket
  sockFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (sockFileDescriptor == -1) {
    throw runtime_error("Socket Failed to Open");
  }

  int opt = 1;
  if (setsockopt(sockFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
  {
    std::cout << "Failed to set SO_REUSEADDR option. " << strerror(errno) << "\n";
    return;
  }

  // Initialize serverAddress structure to store IP address and port
  // memset(&serverAddress, 0, sizeof(serverAddress));
  // AF_INET: IPv4
  serverAddress.sin_family = AF_INET;
  // INADDR_ANY: server address set to "any" to allow connections on any IP addresses
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  // Set the port number to the sock value
  // htons: converts port number to network byte order according to big-endian
  serverAddress.sin_port = htons(sock);
}

Socket::~Socket() {
  if (sockFileDescriptor != -1) {
    close(sockFileDescriptor);
    // Reset the file descriptor to the invalid state
    sockFileDescriptor = -1;

  }

  if(connectionFileDescriptor != -1) {
    close(connectionFileDescriptor);
    connectionFileDescriptor = -1;
  }

}

string Socket::getRequest(const string& delimiter) {
  int rval = 0;

  //Buffer for reading request
  int buffSize = 4096;
  char buf [buffSize];

  string requestData;
  // Read data in chunks until we get the full request (all headers or form data)
  while (true) {
    rval = read(connectionFileDescriptor, buf, buffSize);
    requestData.append(buf, rval);

    // Check for the end of the part of the request being read
    if (requestData.find(delimiter) != std::string::npos || rval <= 0) {
      break;
    }
  }

	return requestData;
}


void Socket::sendResponse(string& res) {
  int rval;

  if ((rval = send(connectionFileDescriptor, res.c_str(), res.length(), 0)) < 0){
    perror("writing socket");
  }else  {
    printf("%s\n",res.c_str());
  }

  cout << "Sent Response" << endl;
}

void Socket::bindSocket() {
  if (bind(sockFileDescriptor, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0) {
    cout << "Couldn't bind to socket, port is already busy" << endl;
    throw runtime_error("Socket Binding Failed");
  }
}

void Socket::listenForConnection(const int maxPending) const {
  if (listen(sockFileDescriptor, maxPending) < 0) {
    throw runtime_error("Socket Listening Failed");
  }
}

int Socket::acceptConnection() {
  struct sockaddr clientAddress{};
  socklen_t clientAddressLength = sizeof(clientAddress);

  connectionFileDescriptor = accept(sockFileDescriptor, (struct sockaddr*)&clientAddress, &clientAddressLength);
  if (connectionFileDescriptor == -1) {
    throw runtime_error("Client Socket Accept Failed");
  }
  return connectionFileDescriptor;
}