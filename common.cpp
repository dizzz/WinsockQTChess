#include"common.h">
SockThread *Common::sockthread=nullptr;
bool Common::playing=0;
int Common::turn=0;
void Common::createThread() { sockthread = new SockThread(); }
