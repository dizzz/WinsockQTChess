#ifndef SOCKDATA_H
#define SOCKDATA_H
#include "basesock.h"
//#include<utility>
#include<string>
class SockData {
  public:
    enum { HELLO, MOVE,RPS };
    int type;
    std::string content;
    SockData() {}
    SockData(int t, std::string c) {
        this->type = t;
        this->content = c;
    }
    static std::string encode(SockData data);
    static SockData decode(std::string str);
    static void getXY(SockData data,int &x,int &y);
    static std::string packXY(int x, int y);
    static std::string packRPS(int rps);
    static std::string packRPSReady(int rps);
    static int getRPS(SockData data);
};
#endif // SOCKDATA_H
