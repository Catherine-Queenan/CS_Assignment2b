//
// Created by eugen on 2024-11-08.
//

#pragma once
#include <thread>
#include "Socket.h"

using namespace std;

class UploadServerThread {
private:
	// Socket object for the client
	Socket* clientSocket;

	// Thread for the uploadServer
	pthread_t uploadServerThread;

	// Integer for the file descriptor of the client socket
	int clientSocketFileDescriptor;

	// Wrapper function that calls HandleClient with a thread
	// PRE: param is a void pointer
	// RETURN: a nullptr
	static void* startThread(void* param);

	// Method to handle the client upload by reading the data and processing it
	void run();

	//Finds the boundary used in the form data
	void getBoundary(const string &s, string &value);
public:
	// Constructor for the upload server thread that initializes the clientSocket and the associated file descriptor
	// PRE: clientSocketFileDescriptor is an integer
	// PRE: clientSocket is a pointer to a Socket
	UploadServerThread(int clientSocketFileDescriptor, Socket* clientSocket)
		: clientSocket(clientSocket), clientSocketFileDescriptor(clientSocketFileDescriptor) {};

	// Destructor
	~UploadServerThread(){}

	// Method that starts up the thread
	void start();
};

