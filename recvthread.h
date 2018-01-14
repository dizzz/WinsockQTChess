#ifndef RECVTHREAD_H
#define RECVTHREAD_H
#include "QThread"
#include"string"
#include"basesock.h"
#include<queue>

#define  WINSOCK_PORT 5099
class RecvThread : public QThread {
    Q_OBJECT
  private:
    static std::queue<std::string>recvque;
  public:
    void init(const char*port);
    void run();
    void receiveMsg(std::string &msg);
};

#endif // RECVTHREAD_H
