#ifndef COMMON_H
#define COMMON_H
#include"sockthread.h"
class Common {
  public:
    static SockThread* sockthread;
    static void createThread();
    //static int sr,sc,tr,tc;
    static bool playing;//游戏状态
    static int turn;
};
#endif // COMMON_H
