#pragma once
#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>
#include <time.h>
#include <Windows.h>
using namespace std;
//日志级别枚举  
typedef enum LogLevel {
	信息 = 0,  //什么都不记录  
	错误,     //只记录严重错误  
	警告,     //记录严重错误，普通错误  
	致命,   //记录严重错误，普通错误，警告  
};
class CLog {

public:
	CLog();
	~CLog();
	//写日志方法
	void Log(string msg, LogLevel level);
protected:
	//初始化
	void GetExeName();
	//获取当前系统时间
	string GetCurrentTime();
	string m_Name;
	string m_Path;
};
#endif