//
// Created by eugen on 2024-11-08.
//

#pragma once

#include <ostream>
#include <string>

using namespace std;

// HttpServletResponse class to handle aspects of the response
class HttpServletResponse {
private:
	string& outputStream;
	string contentType;
	string characterEncoding;

public:
	// Constructor for HttpServletResponse that initializes the output stream
	// PRE: os is a reference to a ostream
	explicit HttpServletResponse(string& os) : outputStream(os) {}

	// Getter for the output stream
	// RETURN: outputStream as a reference
	string& getOutputStream() const { return outputStream; }

	// Setter for content type
	// PRE: contentType is a string reference
	// POST: this HttpServletResponse's contentType is set to the passed value
	void setContentType(const string& contentType);

	// Setter for character encoding type
	// PRE: characterEncoding is a string reference
	// POST: this HttpServletResponse's characterEncoding is set to the passed value
	void setCharacterEncoding(const string& characterEncoding);

	//Send Response Body
	//PRE: responseBody is a string reference
	//POST: the body of the response is sent
	void sendResponse(const string& responseBody) const;

	// Getter for the content type
	// RETURN: contentType as a string
	string getContentType() const { return this->contentType; }

	// Getter for the character encoding type
	// RETURN: characterEncoding as a string
	string getCharacterEncoding() const { return this->characterEncoding; }
};

