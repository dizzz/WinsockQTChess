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
	// ȡ����ִ���ļ�����
	char szExeName[MAX_PATH] = { 0 };
	//GetModuleFileNameA(NULL, szExeName, MAX_PATH);
	if (::GetModuleFileNameA(NULL, szExeName, MAX_PATH) > 0)//��ȡ��ǰ�����Ѽ���ģ����ļ�������·��
	{
		char* pBegin = szExeName;
		char* pTemp = szExeName;


		while (*pTemp != 0) {
			if (*pTemp == '\\') {
				pBegin = pTemp + 1;

			}
			pTemp++;

		}
		//(strrchr(szExeName, '\\'))[0] = 0; // ɾ���ļ�����ֻ���·���ִ�  
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
	case ��Ϣ:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->��Ϣ" << endl;
		break;
	case ����:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->����" << endl;
		break;
	case ����:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->����" << endl;
		break;
	case ����:
		if (outfile.is_open()) outfile << this->GetCurrentTime().c_str() << m_Path.c_str() << ": " << msg.c_str() << " -->����" << endl;
		break;
	}
	outfile.close();
}

string CLog::GetCurrentTime() {
	struct tm t;   //tm�ṹָ��
	time_t now;  //����time_t���ͱ���
	time(&now);      //��ȡϵͳ���ں�ʱ��
	localtime_s(&t, &now);   //��ȡ�������ں�ʱ��
	char temp[50] = { 0 };
	sprintf_s(temp, "[%d-%d-%d %d:%d:%d]", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	std::string  pTemp = temp;
	return pTemp;
}