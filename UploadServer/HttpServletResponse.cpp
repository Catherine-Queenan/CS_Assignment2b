//
// Created by eugen on 2024-11-08.
//

#include "HttpServletResponse.h"

void HttpServletResponse::setContentType(const string &contentType) {
    this->contentType = contentType;
    outputStream += "HTTP/1.1 200 OK\r\n";
    outputStream += "Content-Type: " + this->contentType + "\r\n\r\n";
}

void HttpServletResponse::setCharacterEncoding(const string &characterEncoding) {
    this->contentType = contentType;
    outputStream += "Content-Encoding: " + this->contentType;
}

void HttpServletResponse::sendResponse(const string& responseBody) const {
    outputStream += responseBody;
}

