#ifndef BASESOCK_H
#define BASESOCK_H

#include <QDebug>
#include <WinSock2.h>
#include <ws2tcpip.h>
#define DEFAULT_PORT "27015"
#define SOCKMSG_MAX_LENGTH 512
#pragma comment(lib, "Ws2_32.lib")
class BaseSock {
  private:
    static bool isOpen;
    static SOCKET ConnectSocket;
    static int err;

  public:
    static int getErr();
    static void setErr(int e);
    static bool isAccept();

    static bool isClose() { return !isOpen; }
    static bool isInvalid() { return ConnectSocket == INVALID_SOCKET; }
    static int init(const char *ip, const char *port);
    // init(const char* ip, unsigned short port);
    static int init(const char *port);
    // static int RollReceive();
    // static int send(SockData *data);
    static bool ssend(std::string msg);
    static bool receiveMsg(std::string &msg);
    static void close();
};
#endif // BASESOCK_H
