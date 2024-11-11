#include "Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sstream>

using namespace std;

Socket::Socket(int sock)
{
  this->sock = sock;
}
stringstream Socket::getRequest()
{
  int rval;
  int buffSize = 4096;
  char *buf = new char[buffSize];
  stringstream in;
  while((rval = read(sock, buf, buffSize)) > 0) {
    in << buf;
  }

	return in;
}
void Socket::sendResponse(char *res){
int rval;

  if ((rval = write(sock, res, strlen(res))) < 0){
    perror("writing socket");
  }else  {
    printf("%s\n",res);
  }

	return;
}
Socket::~Socket()
{
}
