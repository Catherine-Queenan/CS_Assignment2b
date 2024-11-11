//
// Created by eugen on 2024-11-08.
//

#pragma once
#include "HttpServlet.h"

using namespace std;

// UploadServlet class to implement the HttpServlet interface
// Servlet that handles the uploading of files to an images folder from a client
class UploadServlet : public HttpServlet {
private:
	// Port number as an integer
	const int PORT = 8081;

	// Socket file path as a string
	const string PATH = "./images/";

	// Method to handle the post requests incoming from the client
	void handlePost(int clientSocket);

	// Method to handle the get requests from the client
	void handleGet(int clientSocket);

public:
	// Default constructor
	UploadServlet();

	// Destructor overridden from interface
	~UploadServlet() override;

	// Implementation of doGet method from HttpServlet interface
	// PRE: req as an HttpServletRequest reference
	// PRE: res as an HttpServletResponse reference
	void doGet(HttpServletRequest& req, HttpServletResponse& res);

	// Implementation of doPost method from HttpServlet interface
	// PRE: req as an HttpServletRequest reference
	// PRE: res as an HttpServletResponse reference
	void doPost(HttpServletRequest& req, HttpServletResponse& res);

	// Method to start the server
	void serverStart();
};
