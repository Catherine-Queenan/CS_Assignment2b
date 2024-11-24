//
// Created by eugen on 2024-11-08.
//

#include <iostream>

#include "Socket.h"
#include "UploadServerThread.h"

using namespace std;

int main() {

    //Set up socket
    Socket socket{8083};
    socket.bindSocket();
    socket.listenForConnection(1);

    cout << "Listening on port 8083..." << endl;
    while(true) {
        const int sockFileDesc = socket.acceptConnection();
        UploadServerThread serverThread{sockFileDesc, &socket};
        serverThread.start();
    }
}
