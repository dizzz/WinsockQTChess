// FuckSocket.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "baseSock.h"

int main()
{
	int n;
	printf("0 for server and 1 for clinet\n");
	scanf_s("%d", &n);
	baseSock socket;
	if (n == 0) {
		char s[100];
		scanf("%s", s);
		socket.init(s);
	} else {
		char s1[100];
		char s2[100];
		scanf("%s %s", s1, s2);
		socket.init(s1, s2);
	}
    return 0;
}

