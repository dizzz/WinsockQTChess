#include"sockdata.h"
std::string SockData::encode(SockData data) {
    char msg[SOCKMSG_MAX_LENGTH + 10];
    sprintf_s(msg, "{%d:%s}", data.type, data.content.c_str());
    return msg;
    // std::string str = "{"+std::to_string(data.type) + ":"+data.content+"}";
}
SockData SockData::decode(std::string str) {
    int p = str.find(":");
    SockData data;
    data.type = atoi(str.substr(1, p - 1).c_str());
    data.content = str.substr(p + 1, str.length() - p - 2);
    return data;
}
void SockData::getCod(SockData data,int &sr,int &sc,int &tr,int &tc) {
    std::string str = data.content;
    qDebug("content:%s",str.c_str());
    int p1 = str.find(",");
    int p2 = str.find(",",p1+1);
    sr = atoi(str.substr(1, p1 - 1).c_str());
    sc =  atoi(str.substr(p1+1, p2-p1-1).c_str());
    p1 = str.find(",", p2+1);
    tr = atoi(str.substr(p2 + 1, p1-p2-1).c_str());
    tc = atoi(str.substr(p1 + 1, str.length()-p1-2).c_str());
    qDebug("decode: %d,%d,%d,%d",sr,sc,tr,tc);
}
std::string SockData::packCod(int sr,int sc,int tr,int tc){
    char msg[SOCKMSG_MAX_LENGTH + 10];
    sprintf_s(msg, "{%d:(%d,%d,%d,%d)}", MOVE,sr,sc,tr,tc);
    return msg;
}
std::string SockData::packRPS(int rps){
    char msg[SOCKMSG_MAX_LENGTH + 10];
    sprintf_s(msg, "{%d:%d}", RPS,rps);
    return msg;
}
int SockData::getRPS(SockData data) { return atoi(data.content.c_str()); }
