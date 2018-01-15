#include "sockthread.h"

/*std::queue<std::string> SockThread::recvque;
std::queue<std::string> SockThread::sendque;
int SockThread::type;
bool SockThread::receiving = false;*/
void SockThread::run() {
    std::string msg;
    receiving = false;
    while (1) {
        // qDebug("working");
        if (!sendque.empty()) {
            qDebug("inque: %s", sendque.front().c_str());
            BaseSock::ssend(sendque.front());
            //qDebug("heher");
            receiving = true;
            sendque.pop();
            continue;
        }
        //Sleep(10);
        //qDebug("before receiving");
        if (receiving) {
            //qDebug("before receiving");
            if (BaseSock::receiveMsg(msg)) {
                receiveMsg(msg);
            }
        }
        Sleep(1);
    }
    this->quit();
    this->wait();
}
void SockThread::receiveMsg(std::string &str) {
    SockData data = SockData::decode(str);
    std::string tmp = "I receive:";
    tmp += str;
    qDebug(tmp.c_str());
    receiving = false;
    qDebug("i change here!!%d",receiving);
    switch (data.type) {
    case SockData::HELLO:
        // TODO
        break;
    case SockData::MOVE:
        int x, y;
        SockData::getXY(data, x, y);
        emit(recvXY(x, y));

        break;
    case SockData::RPS:
        emit(recvRPS(SockData::getRPS(data)));//首次

        break;
    }


}
void SockThread::sendMsg(std::string msg) {
    // sendque.push(SockData::encode(SockData(0, msg)));
    qDebug("pushed: %s",msg.c_str());
    sendque.push(msg);
}

void SockThread::sendMsg(int type, std::string msg) {
    sendque.push(SockData::encode(SockData(type, msg)));
}
void SockThread::closesock(){
    BaseSock::close();
}
SockThread::~SockThread() {
    this->quit();
    this->wait();
}
