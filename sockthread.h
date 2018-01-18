#ifndef SOCKTHREAD_H
#define SOCKTHREAD_H
#include "QThread"
#include "basesock.h"
#include "sockdata.h"
#include "string"
#include <queue>
#include"QDebug"
class SockThread : public QThread {
    Q_OBJECT
  public:
    // void init(const char *port);
    void run();
    void receiveMsg(std::string &msg);
    void sendMsg(std::string msg);
    void sendMsg(int type, std::string msg);
    bool isReceiving() { return receiving; }
    void setReceiving(bool st) { receiving = st;
                               qDebug("set recving:%d",receiving);}
    int getType() { return type; }
    void setType(int st) { type = st; }
    void closesock();
    bool isSockRunning();
    void setSockRunning(bool t);
    ~SockThread();

  private:
    std::queue<std::string> recvque;
    std::queue<std::string> sendque;
    int type;
    bool receiving;
    bool sock_running;
  signals:
    void server_started();
    void sock_error();
    void recvXY(int x,int y);
    void recvRPS(int rps);
    //void recvRPSReady();
    void recvNewGame(bool f);
};

#endif // SOCKTHREAD_H
