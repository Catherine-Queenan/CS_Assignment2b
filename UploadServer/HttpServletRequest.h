//
// Created by eugen on 2024-11-08.
//

#pragma once
#include <istream>

using namespace std;

// HttpServletRequest class to handle aspects of the request
class HttpServletRequest {
private:
	// Input stream used by the request
	istream& inputStream;

public:
	// Constructor for HttpServletRequest that initializes the input stream
	// PRE: inputStream is a reference to an istream object
	explicit HttpServletRequest(istream& inputStream) : inputStream(inputStream) {}

	// Getter for the input stream
	// RETURN: inputStream as a reference to an istream
	istream& getInputStream() { return inputStream; }
};
