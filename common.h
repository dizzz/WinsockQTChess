#ifndef COMMON_H
#define COMMON_H
#include"sockthread.h"
class Common {
  public:
    static SockThread* sockthread;
    static void startThread();
};
#endif // COMMON_H
