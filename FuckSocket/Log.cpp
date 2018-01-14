#include "stdafx.h"
#include "Log.h"
#include <fstream>  
CLog::CLog() {
	if (m_Name.size()>0) m_Name.clear();
	if (m_Path.size()>0) m_Path.clear();
	this->GetExeName();

}
CLog::~CLog() {

}
void CLog::GetExeName() {
	// 取进程执行文件名称
	char szExeName[MAX_PATH] = { 0 };
	//GetModuleFileNameA(NULL, szExeName, MAX_PATH);
	if (::GetModuleFileNameA(NULL, szExeName, MAX_PATH) > 0)//获取当前进程已加载模块的文件的完整路径
	{
		char* pBegin = szExeName;
		char* pTemp = szExeName;


		while (*pTemp != 0) {
			if (*pTemp == '\\') {
				pBegin = pTemp + 1;

			}
			pTemp++;

		}
		//(strrchr(szExeName, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
		m_Path.insert(0, szExeName);
		m_Name.insert(0, pBegin);
		int a = m_Name.find('.');
		m_Name.erase(a);
		ZeroMemory(szExeName, sizeof(szExeName));
		sprintf_s(szExeName, "%s%s", m_Name.c_str(), ".log");
		m_Name.clear();
		m_Name.insert(0, szExeName);

	}
}
void CLog::Log(string msg, LogLevel level) {
	ofstream outfile;
	outfile.open(m_Name.c_str(), ios::out | ios::app | ios::ate);
	switch (level) {
	case 信息:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->信息" << endl;
		break;
	case 错误:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->错误" << endl;
		break;
	case 警告:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->警告" << endl;
		break;
	case 致命:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->致命" << endl;
		break;
	}
	outfile.close();
}

string CLog::GetCurrentTime() {
	struct tm t;   //tm结构指针
	time_t now;  //声明time_t类型变量
	time(&now);      //获取系统日期和时间
	localtime_s(&t, &now);   //获取当地日期和时间
	char temp[50] = { 0 };
	sprintf_s(temp, "[%d-%d-%d %d:%d:%d]", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	std::string  pTemp = temp;
	return pTemp;
}