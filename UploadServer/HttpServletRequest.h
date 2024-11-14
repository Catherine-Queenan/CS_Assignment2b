//
// Created by eugen on 2024-11-08.
//

#pragma once
#include <istream>
#include <sstream>

using namespace std;

// HttpServletRequest class to handle aspects of the request
class HttpServletRequest {
private:
	// Input stream used by the request
	stringstream& inputStream;

	//Boundary used in request
	string boundary;

public:
	// Constructor for HttpServletRequest that initializes the input stream
	// PRE: inputStream is a reference to an istream object
	explicit HttpServletRequest(stringstream& inputStream, const string& boundary) : inputStream(inputStream), boundary(boundary){}

	// Getter for the input stream
	// RETURN: inputStream as a reference to an istream
	string getInputStreamString() { return inputStream.str(); }

	// Getter for the category
	// RETURN: the boundary used in the request
	string getBoundary() {return boundary;}
};
