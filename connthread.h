#ifndef CONNTHREAD_H
#define CONNTHREAD_H
#include<string>
#include"headers.h"
#include<QThread>
#include"basesock.h"
#include<QDebug>
class ConnThread : public QThread {
    Q_OBJECT
  public:

    void run();
    int getType();
    void setType(int t);
    std::string getHost();
    void setHost(std::string t);
    std::string getPort();
    void setPort(std::string t);
    ~ConnThread();

  private:
    int type;
    std::string host;
    std::string port;
  signals:
    void sock_started();
    void sock_error();
};
#endif // CONNTHREAD_H
