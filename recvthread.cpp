#include "recvthread.h"
#include"QDebug"
std::queue<std::string>RecvThread::recvque;
void RecvThread::run() {
    if(BaseSock::isClose())
        BaseSock::init("27015");
    std::string msg;

    qDebug("started");
    //while (BaseSock::isAccept()) {
    while(1){
    qDebug("working");
        if (BaseSock::receiveMsg(msg)) {
            receiveMsg(msg);
        }
        Sleep(1);
    }
    //emit this->connectFailed();
    //emit w->close();
    this->quit();
    this->wait();
}
void RecvThread::receiveMsg(std::string &str){
    recvque.push(str);
    qDebug(str.c_str());
}
void RecvThread::init(const char*port){
    BaseSock::init(port);
}

