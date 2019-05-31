#pragma once
#include "EmsNet.h"
#include <mysql.h>
#include "PublicDef.h"

#pragma comment (lib,"libmysql.lib")
#define NET_ERR -1
#define SQL_ERR -2
#define SUCCESS 0

#define SERVER_PORT 543210
class CEmsServerMain
{
public:
	CEmsServerMain();
	~CEmsServerMain();

public:
	int Init();
	void Shutdown();
	void String2RequestData(char * szData, REQUEST_DATA & reqData);
	void QueryHandle(DWORD dwId, const REQUEST_DATA & reqData);
	void AddHandle(DWORD dwId, const REQUEST_DATA & reqData);
	void UpdateHandle(DWORD dwId, const REQUEST_DATA & reqData);
	void ToClientSQMsg(DWORD dwId);
	void ToClientCCFProgramme(DWORD dwId);
private:
	MYSQL * m_msConnection;
	
private:
	bool InitMySql();
	void QueryDB(char * szSql, char * szRet,bool bAddSplit = false);
	void AddDB(char * szSql, char * szRet, int iLen);
	void AddExamMsg(DWORD dwId, const REQUEST_DATA & reqData);
	void Ret2String(RESULT_TYPE type, char * szRet);
	int UpdateDB(const char * szSql);

	void QuestionToStore(DWORD dwId, const REQUEST_DATA & reqData);
	void ImportCCFStore(DWORD dwId, const REQUEST_DATA & reqData);
	int GetExamDescription(DWORD dwId, const REQUEST_DATA & reqData);

	void GetApplyMsg(DWORD dwId, const REQUEST_DATA & reqData);

	void UpdateApplyMsg(DWORD dwId, const REQUEST_DATA & reqData);
	void StringAppId2Vector(std::string strAppId, std::vector<std::string> & vsAppId);
	void CheckMsgHandle(DWORD dwId, const REQUEST_DATA & reqData);
	void GetUserAnswer(DWORD dwId, std::string strSubject, std::string strType);
	void GetAnswer(DWORD dwId, std::string strQuestionId);
	void SetScoreAndGetNext(DWORD dwId, std::string applyId, std::string strQuestionId, std::string strScore, std::string strSubject, std::string strType);
	void GradeMsgHandle(DWORD dwId, const REQUEST_DATA & reqData);
	void GetGrade(DWORD dwId, std::string strSubject, std::string strNumber, std::string strStartTime);
	void UserMsgHandle(DWORD dwId, const REQUEST_DATA & reqData);
	void LoginValidate(DWORD dwId, std::string strUsername, std::string strPassword);
	void UpdatePassword(DWORD dwId, const REQUEST_DATA & reqData);
	
};
extern CEmsNet g_netServer;
void ServerNetCallBack(DWORD dwId, LPVOID lParam, int iLen);

