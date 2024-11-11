#pragma once
#include <iosfwd>

using namespace std;
class Socket
{
public:
	Socket(int sock);

	stringstream getRequest();
	void sendResponse(char* res);
	~Socket();
private:
	int sock;
};

