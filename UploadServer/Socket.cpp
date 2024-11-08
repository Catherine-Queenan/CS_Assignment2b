#include "Socket.h"
#include <sys/socket.h>
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

  // Initialize serverAddress structure to store IP address and port
  memset(&serverAddress, 0, sizeof(serverAddress));
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
  }
}

char* Socket::getRequest() {
  int rval;
  char *buf = new char[1024];

  if ((rval = read(sock, buf, 1024)) < 0){
    perror("reading socket");
  }else  {
    printf("%s\n",buf);
  }

	return buf;
}


void Socket::sendResponse(char *res) {
int rval;

  if ((rval = write(sock, res, strlen(res))) < 0){
    perror("writing socket");
  }else  {
    printf("%s\n",res);
  }

	return;
}

void Socket::bindSocket() {
  if (bind(sockFileDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    throw runtime_error("Socket Binding Failed");
  }
}

void Socket::listenForConnection(const int maxPending) const {
  if (listen(sockFileDescriptor, maxPending) < 0) {
    throw runtime_error("Socket Listening Failed");
  }
}

int Socket::acceptConnection() const {
  struct sockaddr clientAddress{};
  socklen_t clientAddressLength = sizeof(clientAddress);

  int clientFileDescriptor = accept(sockFileDescriptor, (struct sockaddr*)&clientAddress, &clientAddressLength);
  if (clientFileDescriptor == -1) {
    throw runtime_error("Client Socket Accept Failed");
  }

  return clientFileDescriptor;
}