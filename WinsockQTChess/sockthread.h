#ifndef SOCKTHREAD_H
#define SOCKTHREAD_H
#include "QThread"
#include "basesock.h"
#include "sockdata.h"
#include "string"
#include <queue>
#include"QMutex"
#include"QDebug"
class SockThread : public QThread {
    Q_OBJECT
  public:
    // void init(const char *port);
    enum{
        CLIENT,SERVER
    };
    void run();
    void receiveMsg(std::string &msg);
    void sendMsg(std::string msg);
    void sendMsg(int type, std::string msg);
    bool isReceiving() { return receiving; }
    void setReceiving(bool st) {
        QMutexLocker locker(&m_lock);
        receiving = st;
                               qDebug("set recving:%d",receiving);}
    int getType() { return type; }
    void setType(int st) { type = st; }
    void closesock();
    bool isSockRunning();
    void setSockRunning(bool t);
    ~SockThread();
public slots:
    void stopImmediately();



    //int getType();
    //void setType(int t);
    std::string getHost();
    void setHost(std::string t);
    std::string getPort();
    void setPort(std::string t);


  private:
    QMutex m_lock;
    std::queue<std::string> recvque;
    std::queue<std::string> sendque;
    int type;
    bool receiving;
    bool sock_running;



    //int type;
    std::string host;
    std::string port;
  signals:
     void sock_started();
    //void server_started();
    void sock_error(int err);
    void recv_cod(int sr,int sc,int tr,int tc);
    void recvRPS(int rps);
    void recvThrow();
    void recvNewGame(bool f);
};

#endif // SOCKTHREAD_H
