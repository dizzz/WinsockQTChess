#include "sockthread.h"
#include "game.h"
void SockThread::setSockRunning(bool t) { sock_running = t; }
bool SockThread::isSockRunning() { return sock_running; }
void SockThread::stopImmediately() {
    QMutexLocker locker(&m_lock);
    sock_running = 0;
}
void SockThread::run() {
    bool ret = (type == CLIENT ? BaseSock::init(host.c_str(), port.c_str())
                               : BaseSock::init(port.c_str()));

    //根据输入进行连接
    if (ret) {
        sock_running = 0;
        emit(sock_error(SockData::SOCK_ERROR));
        return;
    } else {
        emit(sock_started());
        qDebug("started");
    }
    std::string msg;
    receiving = false;
    sock_running = 1;
    while (sock_running) {
        if (!sendque.empty()) {
            //发送队列中消息
            qDebug("inque: %s", sendque.front().c_str());
            if (BaseSock::ssend(sendque.front())) {
                emit(sock_error(SockData::SEND_ERROR));
                sock_running = 0;
                closesock();
            }
            receiving = true;
            sendque.pop();
            continue;
        }
        QMutexLocker locker(&m_lock);
        //判断是否进入recv
        if (receiving) {
            if (BaseSock::sreceive(msg)) {
                sock_running = 0;
                emit(sock_error(SockData::RECV_ERROR));
                closesock();
                receiving = 0;
            } else {
                receiveMsg(msg);
            }
        }
        Sleep(1);
    }
}
void SockThread::receiveMsg(std::string &str) {
    //解码并进行相关操作
    SockData data = SockData::decode(str);
    std::string tmp = "I receive:";
    tmp += str;
    qDebug(tmp.c_str());

    qDebug("i change here!!%d", receiving);
    switch (data.type) {
    case SockData::HELLO:
        if (!Game::gamestarted)
            receiving = false;
        break;
    case SockData::MOVE:
        receiving = false;
        int sr, sc, tr, tc;
        SockData::getCod(data, sr, sc, tr, tc);
        emit(recv_cod(sr, sc, tr, tc));
        break;
    case SockData::RPS:
        receiving = false;
        emit(recvRPS(SockData::getRPS(data)));
        break;
    case SockData::NEW_GAME:
        receiving = false;
        emit(recvNewGame(data.content == "YES" ? 1 : 0));
        break;
    case SockData::THROW:
        receiving = false;
        emit(recvThrow());
        break;
    }
}
void SockThread::sendMsg(std::string msg) {
    if (!sock_running)
        return;
    //发送信息
    qDebug("pushed: %s", msg.c_str());
    sendque.push(msg);
}

void SockThread::sendMsg(int type, std::string msg) {
    sendque.push(SockData::encode(SockData(type, msg)));
}
void SockThread::closesock() {
    if (sock_running)
        return;
    BaseSock::close();
    sock_running = 0;
    receiving = 0;
}
SockThread::~SockThread() { closesock(); }
std::string SockThread::getHost() { return host; }
void SockThread::setHost(std::string t) { host = t; }
std::string SockThread::getPort() { return port; }
void SockThread::setPort(std::string t) { port = t; }
