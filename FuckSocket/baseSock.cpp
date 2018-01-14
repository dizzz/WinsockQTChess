#include "stdafx.h"
#include "baseSock.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib") 
baseSock::baseSock() {}
int baseSock::init(char* port) {
	WSADATA wsaData;
	int iResult;
	// 初始化 Winsock 
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	//声明IPv4地址族，流式套接字，TCP协议
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// 解析服务器地址和端口号
	struct addrinfo *result = NULL;
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// 为面向连接的服务器创建套接字
	SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// 为套接字绑定地址和端口号
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(result);
	// 监听连接请求
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	printf("TCP server starting\n");

	// 接受客户端连接请求，返回连接套接字ClientSocket 
	ConnectSocket = accept(ListenSocket, NULL, NULL);
	//ClientSocket1 = accept(ListenSocket, NULL, NULL);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	while (1) {
		if (RollReceive() == 1) break;
		
	}
	// 在必须要监听套接字的情况下释放该套接字
	closesocket(ListenSocket);
}
int baseSock::init(char* host, char* port) {
	addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (err != 0) {
		printf("WSAStartup failed with error: %d\n",err);
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	err = getaddrinfo(host, port, &hints, &result);
	if (err != 0) {
		printf("getaddrinfo failed with error: %d\n", err);
		WSACleanup();
		return 1;
	}
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		printf("Conning...\n");
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}
		err = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (err == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	printf("Successfully connected to %s\n", host);
	for (int i = 0; i < 10; i++) {
		SockData data;
		data.id = i;
		data.content = "fuck";
		data.length = 5;
		send(&data);
	}
	return 0;
}
int baseSock::RollReceive() {
	int iResult;
	int recvbuflen = 1024;
	bool isComplete = true;
	int ID;
	int length;
	int lenReaded;
	int lenLeaved;
	char content[100000];
	while (true) {
		if (!isComplete) {

			iResult = recv(ConnectSocket, content + lenReaded, lenLeaved, 0);
			if (iResult <= 0) {
				printf("recv failed with error: %d/n", WSAGetLastError());
				closesocket(this->ConnectSocket);
				break;
			}
			lenReaded += iResult;
			lenLeaved = length - lenReaded;
			if (lenReaded < length) {
				isComplete = false;
			}
		} else {
			iResult = recv(ConnectSocket, (char *)&ID, sizeof(int), 0);
			if (iResult <= 0) {
				printf("recv failed with error 0: %d/n", WSAGetLastError());
				closesocket(this->ConnectSocket);
				break;
			}
			iResult = recv(ConnectSocket, (char *)&length, sizeof(int), 0);
			if (iResult != sizeof(int)) {
				printf("recv failed with error 1: %d/n", WSAGetLastError());
				closesocket(this->ConnectSocket);
				break;
			}
			memset(content, 0, length + 1);
			iResult = recv(ConnectSocket, content, length, 0);
			if (iResult <= 0) {
				printf("recv failed with error 2: %d/n", WSAGetLastError());
				closesocket(this->ConnectSocket);
				break;
			}
			lenReaded = length;
			lenLeaved = length - lenReaded;
			if (iResult < length) {
				isComplete = false;
			}
		}
		if (lenLeaved <= 0) {
			isComplete = true;
			baseSock::SockData sockData;
			sockData.id = ID;
			sockData.length = length;
			sockData.content = content;
			printf("id: %d\ncontent:%s\n", sockData.id,sockData.content);
			if (sockData.id == 9) {
				return 1;
			} else
				return 2;
		}
	}
	return 0;
}
int baseSock::send(baseSock::SockData *data) {
	::send(ConnectSocket, (char *)&data->id, sizeof(int), 0);
	::send(ConnectSocket, (char *)&data->length, sizeof(int), 0);
	::send(ConnectSocket, data->content, data->length, 0);
	return 0;
}
baseSock::~baseSock() {}
