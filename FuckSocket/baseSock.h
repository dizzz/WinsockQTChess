#pragma once
#include<WinSock2.h>
#define DEFAULT_PORT "27015"
class baseSock {
private:
	SOCKET ConnectSocket = INVALID_SOCKET;
	typedef struct SockData {
		int id;
		int length;
		const char *content;
	} SockData;
public:
	baseSock();
	int init(char* host, char* port);
	int init(char* port);
	int RollReceive();
	int send(SockData *data);
	~baseSock();
	
};

