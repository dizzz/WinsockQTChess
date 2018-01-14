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
void SockData::getXY(SockData data,int &x,int &y) {
    std::string str = data.content;
    qDebug("content:%s",str.c_str());
    int p = str.find(",");
    x = atoi(str.substr(1, p - 1).c_str());
    y =  atoi(str.substr(p+1, str.length()-p-2).c_str());
    qDebug("here %d %d",x,y);
}
std::string SockData::packXY(int x,int y){
    char msg[SOCKMSG_MAX_LENGTH + 10];
    sprintf_s(msg, "{%d:(%d,%d)}", MOVE,x,y);
    return msg;
}
std::string SockData::packRPS(int rps){
    char msg[SOCKMSG_MAX_LENGTH + 10];
    sprintf_s(msg, "{%d:%d}", RPS,rps);
    return msg;
}
int SockData::getRPS(SockData data) { return atoi(data.content.c_str()); }
