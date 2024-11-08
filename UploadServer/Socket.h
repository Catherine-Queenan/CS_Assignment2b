#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

#pragma once

class Socket {
public:
	// Constructor for Socket object that takes an integer for the socket port
	// PRE sock is an integer
	explicit Socket(int sock);

	// Destructor for socket object that closes the file descriptor
	~Socket();

	// Function to retrieve a request
	// RETURN: a character array for the request
	char* getRequest();

	// Function to send a response
	// PRE: res is a character array
	void sendResponse(char* res);

	// Function to bind the server socket to the address and port using the bind system call
	void bindSocket();

	// Function that causes the socket to enter a listening state through the listen system call
	// PRE: maxPending is an integer that represents the maximum pending connections allowed
	void listenForConnection(int maxPending) const;

	// Function to accept any incoming client connections
	// POST: clientAddress structure stores client address information
	// POST: clientFileDescriptor is the file descriptor for the new socket created for the client's connection
	int acceptConnection() const;

private:
	// Socket port to listen on
	int sock;

	// File descriptor of the socket
	int sockFileDescriptor;

	// Structure for the server address
	struct sockaddr serverAddress{};
};

