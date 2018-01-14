#include "sendthread.h"

std::queue<std::string> SendThread::send;
void SendThread::run()
{
    //qDebug("123123");
    /*&ia(BaseSock::isClose()){

    }*/
	while (!BaseSock::isClose())
	{

		if (!send.empty())
		{
            //qDebug("";)
            BaseSock::ssend(send.front());
            qDebug(send.front().c_str());
			send.pop();
		}
		Sleep(1);
	}
}
void SendThread::sendMsg(std::string msg){
    //std::string tmp = SockData::encode(SockData(0,msg));
    send.push(msg);
              //qDebug("FASD%s",tmp.c_str());
              //return 0;
}


