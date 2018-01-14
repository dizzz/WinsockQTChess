#include"common.h">
SockThread *Common::sockthread=nullptr;
void Common::startThread() { sockthread = new SockThread(); }
