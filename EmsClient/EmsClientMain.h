#pragma once
#include "EmsNet.h"
#include "PublicDef.h"
#include <map>
#include <vector>
#include <string>
#define NET_ERR -1
#define SUCCESS 0

#define SERVER_PORT 543210
class CEmsClientMain
{
public:
	CEmsClientMain();
	~CEmsClientMain();

public:

	int Init();
	int ImportQuestion(TCHAR * szFileName, TCHAR * szSheet,LPCTSTR szSubject, LPCTSTR szType);
	int GetSubjectNameAndQuestionName();
	int ScanQuestion(LPCTSTR szSubject, LPCTSTR szType);
	void String2VecMap(std::wstring strData,std::vector<map<std::wstring, std::wstring> > &vecMap);
	int AddExamMsg(LPCTSTR szSubject, LPCTSTR szStartTime, LPCTSTR szEndTime);
	int GetExamDescription(LPCTSTR szSubject);
	int GetApplyMsg(LPCTSTR szSubject, LPCTSTR szNumber);
	void HandleApplyExam(std::vector<std::wstring> & m_vsApplyId, int iOffset = -1, bool bPass = true);
	int GetQuestionAndUserAnswer(LPCTSTR szSubject, LPCTSTR szType);
	void GetAnswerById(std::wstring strQuestionId);
	int GetCheckNextAnswer(std::wstring strApplyId, std::wstring strQuestionId, std::wstring strScore, std::wstring strSubject, std::wstring strType);
	int GradeScan(LPCTSTR szSubject, LPCTSTR szNumber, LPCTSTR szStartTime);
	void Login(LPCTSTR szUsername, LPCTSTR szPassword);
	void UpdatePassword(LPCTSTR szUsername, LPCTSTR szPassword);
private:
	void RequestData2String(const REQUEST_DATA& resData, char * szBuf, const int& iLen);
	ADDITION_THIRD GetThird(LPCTSTR szThird);
	ADDITION_SECOND GetSecond(LPCTSTR szSecond);
	
private:
	CEmsNet * m_pClientNet;
};

void ClientNetCallBack(DWORD dwId, LPVOID lParam, int iLen);