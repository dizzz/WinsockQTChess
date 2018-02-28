#ifndef SOCKDATA_H
#define SOCKDATA_H
#include "basesock.h"
//#include<utility>
#include<string>
class SockData {
  public:
    enum { HELLO, MOVE,RPS,NEW_GAME,BYE,THROW};
    enum{
        SOCK_ERROR,
        SEND_ERROR,
        RECV_ERROR
    };
    int type;
    std::string content;
    SockData() {}
    SockData(int t, std::string c) {
        this->type = t;
        this->content = c;
    }
    static std::string encode(SockData data);
    static SockData decode(std::string str);
    static void getCod(SockData data,int &sr,int &sc,int &tr,int &tc);
    static std::string packCod(int sr,int sc,int tr,int tc);
    static std::string packRPS(int rps);
    static std::string packRPSReady(int rps);
    static int getRPS(SockData data);
};
#endif // SOCKDATA_H
