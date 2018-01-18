#include "sockthread.h"

void SockThread::setSockRunning(bool t) { sock_running = t; }
bool SockThread::isSockRunning() { return sock_running; }
void SockThread::run() {
    std::string msg;
    receiving = false;
    sock_running = 1;
    while (1) {
        if (!sendque.empty()) {
            qDebug("inque: %s", sendque.front().c_str());
            if (BaseSock::ssend(sendque.front())) {
                emit(sock_error());
                closesock();
            }

            receiving = true;
            sendque.pop();
            continue;
        }
        if (receiving) {
            if (BaseSock::sreceive(msg)) {
                emit(sock_error());
                closesock();
                receiving = 0;
            } else {
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
    qDebug("i change here!!%d", receiving);
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
        emit(recvRPS(SockData::getRPS(data)));
        break;
    case SockData::NEW_GAME:
        emit(recvNewGame(data.content=="YES"?1:0));
        break;
    }
}
void SockThread::sendMsg(std::string msg) {
    if(!sock_running) return;
    // sendque.push(SockData::encode(SockData(0, msg)));
    qDebug("pushed: %s", msg.c_str());
    sendque.push(msg);
}

void SockThread::sendMsg(int type, std::string msg) {
    sendque.push(SockData::encode(SockData(type, msg)));
}
void SockThread::closesock() {
    if(sock_running) return;
    BaseSock::close();
    sock_running = 0;
    receiving = 0;
}
SockThread::~SockThread() {
    this->quit();
    this->wait();
}
