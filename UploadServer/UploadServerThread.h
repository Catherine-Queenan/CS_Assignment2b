//
// Created by eugen on 2024-11-08.
//

#pragma once
#include <sys/_pthreadtypes.h>

#include "Socket.h"

class UploadServerThread {
private:
    Socket *socket = nullptr;
    pthread_t thread;

public:
    UploadServerThread(Socket *socket): socket(socket){}
    void run();
    void start();

};

