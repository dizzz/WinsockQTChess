#include "connthread.h"
int ConnThread::getType() { return type; }
void ConnThread::setType(int t) { type = t; }
std::string ConnThread::getHost() { return host; }
void ConnThread::setHost(std::string t) { host = t; }
std::string ConnThread::getPort() { return port; }
void ConnThread::setPort(std::string t) { port = t; }


void ConnThread::run() {
    bool ret=false;
    ret = (type==CLIENT?BaseSock::init(host.c_str(), port.c_str()): BaseSock::init(port.c_str()));
    if(ret)
        emit(sock_error());
    else{
        emit(sock_started());
        qDebug("started");
    }
}
ConnThread::~ConnThread(){
    BaseSock::close();
    quit();
    wait();
}
