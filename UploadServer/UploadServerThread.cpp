//
// Created by eugen on 2024-11-08.
//

#include "UploadServerThread.h"

#include <iosfwd>
#include <sstream>
#include "HttpServletRequest.h"
#include "HttpServletResponse.h"

using namespace std;

void getValue(string s, string& boundary) {
    stringstream ss(s);
    getline(ss, boundary, '=');
    getline(ss, boundary, '=');
}

void * startThread(void *arg) {
    if(arg == nullptr) {
        return 0;
    }
    UploadServerThread *th = static_cast<UploadServerThread *>(arg);
    th -> run();
    return nullptr;
}

void UploadServerThread::start() {
    pthread_create(&thread, nullptr, startThread, static_cast<void *>(this));
}

void UploadServerThread::run() {
    try {
        constexpr string PREFIX = "Content-Type: multipart/form-data";
        constexpr string METHOD_GET = "GET";
        constexpr string METHOD_POST = "POST";

        stringstream in = socket->getRequest();
        string requestLine;
        getline(in, requestLine);
        if(requestLine.empty()) {
            return;
        }

        string method = requestLine;
        string boundary;
        while(getline(in, requestLine)) {
            if(requestLine.find(PREFIX)) {
                getValue(requestLine, boundary);
            }
        }

        HttpServletRequest req {in, boundary};
        stringstream baos;
        HttpServletResponse res {baos&};

        HttpServlet servlet = new UploadServlet();

        if(method.find(METHOD_GET)) {
            servlet.doGet(req, res);
        } else if (method.find(METHOD_POST)) {
            servlet.doPost(req, res);
        }

        
    } catch (...) {

    }

}