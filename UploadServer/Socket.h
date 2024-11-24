
#pragma once

#include <netinet/in.h>
#include <sstream>

using namespace std;

class Socket {
public:
	// Constructor for Socket object that takes an integer for the socket port
	// PRE sock is an integer
	explicit Socket(int sock);

	// Destructor for socket object that closes the file descriptor
	~Socket();

	// Function to retrieve a request
	// PRE: delimiter must be a string predicted to be contained in the request
	// RETURN: a string for the request
	string getRequest(const string& delimiter);

	// Function to send a response
	// PRE: res is a character array
	void sendResponse(string& res);

	// Function to bind the server socket to the address and port using the bind system call
	void bindSocket();

	// Function that causes the socket to enter a listening state through the listen system call
	// PRE: maxPending is an integer that represents the maximum pending connections allowed
	void listenForConnection(int maxPending) const;

	// Function to accept any incoming client connections
	// POST: clientAddress structure stores client address information
	// POST: clientFileDescriptor is the file descriptor for the new socket created for the client's connection
	int acceptConnection();


private:
	// Socket port to listen on
	int sock;

	// File descriptor of the socket
	int sockFileDescriptor;

	// File descriptor of the connection
	int connectionFileDescriptor;

	// Structure for the server address
	struct sockaddr_in serverAddress{};
};

