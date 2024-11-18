//
// Created by eugen on 2024-11-08.
//

#include "UploadServerThread.h"

#include <iosfwd>
#include <iostream>
#include <sstream>
#include <sys/unistd.h>

#include "HttpServletRequest.h"
#include "HttpServletResponse.h"
#include "UploadServlet.h"

using namespace std;

void UploadServerThread::getBoundary(const string& s, string& value) {
    value = s.substr(s.find('=') + 1);
}

void * UploadServerThread::startThread(void *param) {
    if(param == nullptr) {
        return nullptr;
    }
    auto *th = static_cast<UploadServerThread *>(param);
    th -> run();
    return nullptr;
}

void UploadServerThread::start() {
    pthread_create(&uploadServerThread, nullptr, startThread, (this));
}

void UploadServerThread::run() {
    try {
        //For string checking
        const string PREFIX = "Content-Type: multipart/form-data; boundary=";
        const string METHOD_GET = "GET";
        const string METHOD_POST = "POST";

        //For getting request headers and form data
        const string HEADER_DELIM = "\r\n\r\n";
        const string FORM_DELIM = "--\r\n";
        
        string request = clientSocket->getRequest(HEADER_DELIM);
        stringstream in{request};

        //Find method
        string requestLine;
        getline(in, requestLine, '\r');
        std::cout << requestLine << endl;
        if(requestLine.empty()) {
            return;
        }

        string method = requestLine;

        //Finds boundary if there is one
        string boundary;
        while(getline(in, requestLine, '\r')) {
            if(requestLine.find(PREFIX) != string::npos) {
                getBoundary(requestLine, boundary);
            }
        }

        //Sets up response with output string
        string out;
        HttpServletResponse res {out};

        //Create servlet
        UploadServlet servlet{};

        if(method.find(METHOD_GET) != string::npos) {
            HttpServletRequest req {in, boundary};
            servlet.doGet(req, res);
        } else if (method.find(METHOD_POST) != string::npos) {
            //Get form data part of request
            string formData = clientSocket -> getRequest(FORM_DELIM);
            stringstream data{formData};

            HttpServletRequest req {data, boundary};
            servlet.doPost(req, res);
        }

        //Send response from servlet and close connection
        clientSocket -> sendResponse(out);
        close(clientSocketFileDescriptor);
    } catch (...) {
        std::cout << "ERROR OCCURRED" << endl;
    }
}