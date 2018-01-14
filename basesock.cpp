#include "basesock.h"

bool BaseSock::isOpen = false;
int BaseSock::err=true;
SOCKET BaseSock::ConnectSocket = INVALID_SOCKET;
void BaseSock::setErr(int e) { err = e; }
int BaseSock::getErr() { return err; }
bool BaseSock::isAccept() {
    char s[1];
    return recv(ConnectSocket, s, 0, 0) >= 0;
}
int BaseSock::init(const char *port) {
    WSADATA wsaData;
    int iResult;
    // 初始化 Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        qDebug("WSAStartup failed with error: %d\n", iResult);
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
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult != 0) {
        ("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return err = 1;
    }

    // 为面向连接的服务器创建套接字
    SOCKET ListenSocket =
        socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        qDebug("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return err = 1;
    }

    // 为套接字绑定地址和端口号
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        qDebug("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return err = 1;
    }
    freeaddrinfo(result);
    // 监听连接请求
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        qDebug("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return err = 1;
    }
    qDebug("TCP server starting\n");
    qDebug("port: %s", port);
    // 接受客户端连接请求，返回连接套接字ClientSocket
    qDebug("blocking?");
    ConnectSocket = accept(ListenSocket, NULL, NULL);
    qDebug("nope");
     //ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ConnectSocket == INVALID_SOCKET) {
        qDebug("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return err = 1;
    }
    // 在必须要监听套接字的情况下释放该套接字
    closesocket(ListenSocket);
    isOpen = true;
    return err=0;
}
int BaseSock::init(const char *host, const char *port) {

    addrinfo *result = NULL, *ptr = NULL, hints;
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        qDebug("WSAStartup failed with error: %d\n", err);
        return err=1;
    }
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    err = getaddrinfo(host, port, &hints, &result);
    if (err != 0) {
        qDebug("getaddrinfo failed with error: %d\n", err);
        WSACleanup();
        return err=1;
    }
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        qDebug("Connecting...\n");

        ConnectSocket =
            socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            qDebug("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return err=1;
        }

        if (connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) ==
            INVALID_SOCKET) {
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
        return err=1;
    }
    qDebug("Successfully connected to %s\n", host);
    // if(BaseSock::isInvalid())        qDebug("hrer invade");
    isOpen = true;
    return err=0;
}
/*
int BaseSock::RollReceive() {
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
                //qDebug("recv failed with error: %d/n", WSAGetLastError());
                closesocket(ConnectSocket);
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
                qDebug("recv failed with error 0: %d/n", WSAGetLastError());
                closesocket(ConnectSocket);
                break;
            }
            iResult = recv(ConnectSocket, (char *)&length, sizeof(int), 0);
            if (iResult != sizeof(int)) {
                qDebug("recv failed with error 1: %d/n", WSAGetLastError());
                closesocket(ConnectSocket);
                break;
            }
            memset(content, 0, length + 1);
            iResult = recv(ConnectSocket, content, length, 0);
            if (iResult <= 0) {
                qDebug("recv failed with error 2: %d/n", WSAGetLastError());
                closesocket(ConnectSocket);
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
            BaseSock::SockData sockData;
            sockData.id = ID;
            sockData.length = length;
            sockData.content = content;
            qDebug("id: %d\ncontent:%s\n", sockData.id,sockData.content);
            if (sockData.id == 9) {
                return 1;
            } else
                return 2;
        }
    }
    return 0;
}
int BaseSock::send(BaseSock::SockData *data) {
    ::send(ConnectSocket, (char *)&data->id, sizeof(int), 0);
    ::send(ConnectSocket, (char *)&data->length, sizeof(int), 0);
    ::send(ConnectSocket, data->content, data->length, 0);
    return 0;
}
*/
bool BaseSock::ssend(std::string msg) {
    /*
    qDebug("ready to send");
    qDebug()<<msg.c_str();

    if(send(ConnectSocket, msg.c_str(), SOCKMSG_MAX_LENGTH, 0) == SOCKET_ERROR){
    //char str[10]="sdasd";

    //if(send(ConnectSocket,str,strlen(str),0)==SOCKET_ERROR){
        qDebug("send error");
        return 1;
    }

    qDebug("sent %s",msg.c_str());
    return 0;*/
    // if(isInvalid()) qDebug("fucking invaild");
    // return send(ConnectSocket, msg.c_str(),, 0) != SOCKET_ERROR;
    if (send(ConnectSocket, msg.c_str(), SOCKMSG_MAX_LENGTH, 0) ==
        SOCKET_ERROR) {
        qDebug("error sending %s", msg.c_str());
        return 1;
    }
    qDebug(" sended %s", msg.c_str());
    return 0;
}

bool BaseSock::receiveMsg(std::string &msg) {
    char buff[SOCKMSG_MAX_LENGTH + 10];
    // qDebug("wait for msg");
    int error = recv(ConnectSocket, buff, SOCKMSG_MAX_LENGTH, 0);
    // qDebug("get msg");
    if (error == SOCKET_ERROR || errno == EPIPE)
        return false;
    msg = buff;
    return true;
}
void BaseSock::close(){
    closesocket(ConnectSocket);
    WSACleanup();
}
