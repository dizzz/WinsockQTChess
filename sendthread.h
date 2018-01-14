#ifndef SENDTHREAD_H
#define SENDTHREAD_H
#include"QThread"
#include"basesock.h"
#include"queue"
#include"sockdata.h"
class SendThread:public QThread
{
private:
    static std::queue<std::string> send;
public:
    
    void run();

    static void sendMsg(std::string msg);

};

#endif // SENDTHREAD_H
