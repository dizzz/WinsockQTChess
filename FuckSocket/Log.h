#pragma once
#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>
#include <time.h>
#include <Windows.h>
using namespace std;
//��־����ö��  
typedef enum LogLevel {
	��Ϣ = 0,  //ʲô������¼  
	����,     //ֻ��¼���ش���  
	����,     //��¼���ش�����ͨ����  
	����,   //��¼���ش�����ͨ���󣬾���  
};
class CLog {

public:
	CLog();
	~CLog();
	//д��־����
	void Log(string msg, LogLevel level);
protected:
	//��ʼ��
	void GetExeName();
	//��ȡ��ǰϵͳʱ��
	string GetCurrentTime();
	string m_Name;
	string m_Path;
};
#endif