//
// Created by eugen on 2024-11-08.
//

#pragma once
#include <map>

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

	// Method for collecting form fields from request
	// PRE: boundary must be a string
	// PRE: input must be a string
	// RETURNS: a map containing the caption, date, filename and file bytes
	map<string,string> getFormData(const string& boundary, const string& input);

	// Method for generating list elements for the file listings
	string getFilesList();

public:
	// Default constructor
	UploadServlet(){}

	// Destructor overridden from interface
	~UploadServlet() override{}

	// Implementation of doGet method from HttpServlet interface
	// PRE: req as an HttpServletRequest reference
	// PRE: res as an HttpServletResponse reference
	void doGet(HttpServletRequest& req, HttpServletResponse& res) override;

	// Implementation of doPost method from HttpServlet interface
	// PRE: req as an HttpServletRequest reference
	// PRE: res as an HttpServletResponse reference
	void doPost(HttpServletRequest& req, HttpServletResponse& res) override;

	// Method to start the server
	void serverStart();
};
