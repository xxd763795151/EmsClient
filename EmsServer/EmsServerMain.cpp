#include "stdafx.h"
#include "EmsServerMain.h"

CEmsNet g_netServer;
MYSQL * g_msConnection;
CEmsServerMain::CEmsServerMain()
{
}


CEmsServerMain::~CEmsServerMain()
{
}
/************************************************************************/
/* �������������ó�ʼ��                                                 */
/************************************************************************/
int CEmsServerMain::Init()
{
	int iRet = SUCCESS;
	do 
	{
		//::GetPrivateProfileStringA("net","net.port")
		int iPort = ::GetPrivateProfileIntA("net", "net.port", SERVER_PORT, "..\\conf\\server.ini");
		bool bInitServer = g_netServer.InitServer(iPort, ServerNetCallBack);
		if (!bInitServer)
		{
			iRet = NET_ERR;
			break;
		}
		printf("���������绷���ɹ���ʼ��...\n");
		if (!InitMySql())
		{
			iRet = SQL_ERR;
			break;
		}
		printf("���ݿ⻷�����������ѳɹ�����...\n");


		printf("��������ʼ�����...\n");
	} while (false);
	
	return iRet;
}

bool CEmsServerMain::InitMySql()
{
	char szUser[0xff] = { 0 };
	::GetPrivateProfileStringA("database", "db.username", "root", szUser, 0xff, "..\\conf\\server.ini");
	char szPass[0xff] = { 0 };
	::GetPrivateProfileStringA("database", "db.password", "123456", szPass, 0xff, "..\\conf\\server.ini");
	char szHost[0xff] = { 0 };
	::GetPrivateProfileStringA("database", "db.host", "localhost", szHost, 0xff, "..\\conf\\server.ini");
	char szDatabase[0xff] = { 0 };
	::GetPrivateProfileStringA("database", "db.source", "emsdb", szDatabase, 0xff, "..\\conf\\server.ini");
	unsigned int iPort = ::GetPrivateProfileIntA("net", "db.port", 3306, "..\\conf\\server.ini");;
	g_msConnection = mysql_init(0);
	if (mysql_real_connect(g_msConnection, szHost, szUser, szPass, szDatabase, iPort, NULL, 0) == 0)
	{
		printf("���ݿ�����ʧ��...\n");
		return false;
	}
	mysql_query(g_msConnection, "SET NAMES GBK");//�������ݿ�Ĭ�ϱ���ΪGBK
	return true;
}
/************************************************************************/
/* �˳��ر�����                                                         */
/************************************************************************/
void CEmsServerMain::Shutdown()
{
	mysql_close(g_msConnection);
	g_netServer.CloseServerSocket();
}
/************************************************************************/
/* ��char��������ת��ΪREQUEST_DATA�ṹ������                           */
/************************************************************************/
void CEmsServerMain::String2RequestData(char * szData, REQUEST_DATA & reqData)
{
	std::string strData = szData;
	reqData.enAction = (ACTION)std::stoi(strData.substr(0, 2));
	reqData.enFirst = (ADDITION_FIRST)std::stoi(strData.substr(2, 2));
	reqData.enSecond = (ADDITION_SECOND)std::stoi(strData.substr(4, 2));
	reqData.enThird = (ADDITION_THIRD)std::stoi(strData.substr(6, 2));
	std::string strRequestData = strData.substr(9);
	if (strRequestData == "NULL")  return;

	size_t kPos = 0, vPos = 0;
	int iRet = 0;
	std::string strKeyFind = "Request_Data_Key:";
	std::string strValueFind = "Request_Data_Value:";
	iRet = strRequestData.find(strKeyFind, kPos);
	while (true)
	{
		kPos = iRet + strKeyFind.size();
		vPos = strRequestData.find(strValueFind, iRet);
		std::string strKey = strRequestData.substr(kPos, vPos - kPos - 1);
		iRet = strRequestData.find(strKeyFind, vPos);
		if (iRet == -1)
		{
			int iValueBegin = vPos + strValueFind.size();
			std::string strValue = strRequestData.substr(iValueBegin, strRequestData.size()-iValueBegin);
			reqData.mapData.insert(make_pair(strKey, strValue));
			break;
		}
		int iValueBegin = vPos + strValueFind.size();
		std::string strValue = strRequestData.substr(iValueBegin, iRet - iValueBegin - 1);
		reqData.mapData.insert(make_pair(strKey, strValue));
	}
}
/************************************************************************/
/* ��ѯ����������                                                     */
/************************************************************************/
void CEmsServerMain::QueryHandle(DWORD dwId, const REQUEST_DATA & reqData)
{
	if (reqData.enFirst == NO_FIRST)  return;
	switch (reqData.enFirst)
	{
	case USER_MSG:
		UserMsgHandle(dwId, reqData);
		break;
	case QUESTION_MSG:
		//����������ͻ�����Ҫ��ÿ�Ŀ���ÿ�Ŀ�����͵�������Ϣ
		if (reqData.enSecond ==  SUBJECT && reqData.enThird == QUESTION)
		{
			ToClientSQMsg(dwId);
		}
		else if (reqData.enSecond == CCF_SUBJECT && reqData.enThird == PROGRAMME_QUESTION)
		{
			ToClientCCFProgramme(dwId);
		}
		break;
	case GRADE_MSG:GradeMsgHandle(dwId, reqData);
		break;
	case  EXAM_MSG:	GetExamDescription(dwId, reqData);		break;
	case APPLY_MSG:	
		if (reqData.enSecond == NO_SECOND && reqData.enThird == NO_THIRD)
		{
			GetApplyMsg(dwId, reqData);
		}
			break;
	case CHECK_MSG:CheckMsgHandle(dwId, reqData);
		break;
	default:
		break;
	}
}
/************************************************************************/
/* ���ͻ��˷��ؿ�Ŀ����������Ϣ                                         */
/************************************************************************/
void CEmsServerMain::ToClientSQMsg(DWORD dwId)
{
	char * szSql = "select s_name,qt_name from t_subject as s, t_question_type as q, t_subject_question_type as sqt where sqt.s_id = s.s_id and sqt.q_id = q.qt_id ";
	char * szRet = new char[0xffff];
	memset(szRet,0,0xffff);
	std::string strRes = std::to_string(SUBJECT_QUESTION_RET);
	if (strRes.size() < 2) strRes = "0" + strRes;
	strcat(szRet, strRes.c_str());
	QueryDB(szSql, szRet);
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	delete[] szRet;
}
/************************************************************************/
/* ���ݿ��ѯ����                                                       */
/************************************************************************/
void CEmsServerMain::QueryDB(char * szSql, char * szRet, bool bAddSplit)
{
	std::vector<char *> vcFields;
	int iResult = mysql_query(g_msConnection, szSql);
	if (iResult == 0 )
	{
		MYSQL_RES * sqlRes = mysql_store_result(g_msConnection);
		MYSQL_FIELD * fd;
		MYSQL_ROW sqlRow;
		int nFields = mysql_num_fields(sqlRes);
		while (fd = mysql_fetch_field(sqlRes))
		{
			vcFields.push_back(fd->name);
		}
		while (sqlRow = mysql_fetch_row(sqlRes))
		{
			for (int i = 0; i < nFields; ++i)
			{
				strcat(szRet, "DB_Name:");
				strcat(szRet, vcFields[i]);
				strcat(szRet, "DB_Value:");
				strcat(szRet, sqlRow[i]);
			}
			if (bAddSplit)
			{
				strcat(szRet, SPLIT_FLAG);
			}
			
		}
	}
}
/************************************************************************/
/* �ͻ�������������                                                   */
/************************************************************************/
void CEmsServerMain::AddHandle(DWORD dwId, const REQUEST_DATA & reqData)
{
	ADDITION_FIRST first = reqData.enFirst;
	switch (first)
	{
	case NO_FIRST:break;
	case USER_MSG:break;
	case QUESTION_MSG:QuestionToStore(dwId, reqData);	 break;
	case GRADE_MSG:break;
	case  EXAM_MSG:AddExamMsg(dwId, reqData);			break;
	default:
		break;
	}
}
/************************************************************************/
/* ��������һ����Ŀ�����⵼����Ӧ�����                                 */
/************************************************************************/
void CEmsServerMain::QuestionToStore(DWORD dwId, const REQUEST_DATA & reqData)
{
	ADDITION_SECOND second = reqData.enSecond;
	switch (second)
	{
	case NO_SECOND:break;
	case SUBJECT:break;
	case CCF_SUBJECT:ImportCCFStore(dwId, reqData); break;
	default:
		break;
	}
}
/************************************************************************/
/* ����CCF���                                                          */
/************************************************************************/
void CEmsServerMain::ImportCCFStore(DWORD dwId, const REQUEST_DATA & reqData)
{
	char * szSql = new char[MAX_MSG_SIZE];
	memset(szSql, 0, MAX_MSG_SIZE);
	strcat(szSql, "insert into t_ccf(");
	char * szSqlEnd = new char[MAX_MSG_SIZE];
	memset(szSqlEnd, 0, MAX_MSG_SIZE);
	strcat(szSqlEnd, "values('");
	char * szRet = new char[MAX_PATH];
	memset(szRet, 0, MAX_PATH);
	std::map<std::string, std::string>::const_iterator it;
	for (it = reqData.mapData.begin(); true; )
	{
		strcat(szSql, (it->first).c_str());
		strcat(szSqlEnd, it->second.c_str());

		++it;
		if (it == reqData.mapData.end())	break;
		strcat(szSql, ",");
		strcat(szSqlEnd, "','");
	}
	strcat(szSql, ") ");
	strcat(szSqlEnd, "');");
	strcat(szSql, szSqlEnd);
	AddDB(szSql, szRet, MAX_PATH);
	/*char * szToClient = new char[MAX_PATH];
	memset(szToClient, 0, MAX_PATH);
	std::string strRet = std::to_string(OPERATOR_RET);
	if (strRet.size() == 1)
	{
		strRet = "0" + strRet;
	}
	strRet = strRet + "�ɹ�����" + szRet + "����¼";
	szToClient = (char*)strRet.c_str();
	MessageBoxA(NULL, szToClient, "", MB_OK);
	g_netServer.SendToClient(dwId, szToClient, strlen(szToClient));
	delete szToClient;*/
	delete[] szSqlEnd;
	delete[] szSql;
	delete[] szRet;
}
/************************************************************************/
/* �����ݿ��������                                                     */
/************************************************************************/
void CEmsServerMain::AddDB(char * szSql, char * szRet, int iLen)
{
	mysql_query(g_msConnection, szSql);
	my_ulonglong nCnt = mysql_affected_rows(g_msConnection);
	sprintf_s(szRet, iLen, "%d", nCnt);
}
/************************************************************************/
/* ���ؿͻ���CCF����                                                    */
/************************************************************************/
void CEmsServerMain::ToClientCCFProgramme(DWORD dwId)
{
	char *szSql = "select ������,��������,ʱ������,�ڴ�����,�������� from t_ccf";
	char * szBuf = new char[MAX_MSG_SIZE];
	memset(szBuf, 0, MAX_MSG_SIZE);
	QueryDB(szSql, szBuf,true);
	std::string strRes = std::to_string(CCF_PROGRAMME_RET);
	if (strRes.size() < 2) strRes = "0" + strRes;
	strRes += szBuf;
	char  * szRet = const_cast<char*>(strRes.c_str());
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	delete[] szBuf;
}
/************************************************************************/
/* ���ӿ�����Ϣ                                                         */
/************************************************************************/
void CEmsServerMain::AddExamMsg(DWORD dwId, const REQUEST_DATA & reqData)
{
	std::map<std::string, std::string>::const_iterator itMap;
	itMap = reqData.mapData.find("��Ŀ");
	std::string strSubject = itMap->second;
	itMap = reqData.mapData.find("��ʼʱ��");
	std::string strStartTime = itMap->second;
	itMap = reqData.mapData.find("����ʱ��");
	std::string strEndTime = itMap->second;
	char  szSql[MAX_PATH] = { 0 };//;
	strcat(szSql, "select s_id from t_subject where s_name='");
	strcat(szSql, strSubject.c_str());
	strcat(szSql, "';");
	char szRet[MAX_PATH] = { 0 };
	QueryDB(szSql, szRet);
	std::string strSubjectId = szRet;
	strSubjectId = strSubjectId.substr(strSubjectId.find_last_of(":")+1);//�õ���ĿID
	memset(szSql, 0, strlen(szSql));
	strcat(szSql, "select count(*) from t_exam where s_id=");
	strcat(szSql, strSubjectId.c_str());
	strcat(szSql, " and e_startTime <= '");
	strcat(szSql, strStartTime.c_str());
	strcat(szSql, "' and e_endTime >= '");
	strcat(szSql, strStartTime.c_str());
	strcat(szSql, "';");
	memset(szRet, 0, MAX_PATH);
	QueryDB(szSql, szRet);
	std::string strExistNum = szRet;
	strExistNum = strExistNum.substr(strExistNum.find_last_of(":") + 1);//�ÿ�Ŀ�Ŀ���ʱ���Ƿ��г�ͻ
	memset(szRet, 0, MAX_PATH);
	Ret2String(ADD_EXAM_RET, szRet);
	if (strExistNum != "0")//���Գ�ͻ
	{
		strcat(szRet, "0");
		g_netServer.SendToClient(dwId, szRet, strlen(szRet));
		return;
	}
	memset(szSql, 0, MAX_PATH);
	strcat(szSql, "insert into t_exam(e_startTime,e_endTime,s_id) values('");
	strcat(szSql, strStartTime.c_str());
	strcat(szSql, "','");
	strcat(szSql, strEndTime.c_str());
	strcat(szSql, "',");
	strcat(szSql, strSubjectId.c_str());
	strcat(szSql, ");");
	char szBuf[0xff] = { 0 };
	AddDB(szSql, szBuf, 0xff);
	if (strcpy(szBuf, "1") == 0)
	{
		strcat(szRet, "1");//��ӿ�����Ϣʧ��
		g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	}
	else
	{
		strcat(szRet, "9");//��ӿ�����Ϣ�ɹ�
		g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	}
}
/************************************************************************/
/* ���ߺ�������������ת��Ϊ�ַ���������Ϊchar                           */
/************************************************************************/
void CEmsServerMain::Ret2String(RESULT_TYPE type, char * szRet)
{
	_itoa(type, szRet, 10);
	if (type / 10 == 0)
	{//typeΪ1λ��
		szRet[1] = szRet[0];
		szRet[0] = '0';
	}
}
/************************************************************************/
/* ���ߺ����������Ծ�������Ϣ                                           */
/************************************************************************/
int CEmsServerMain::GetExamDescription(DWORD dwId, const REQUEST_DATA & reqData)
{
	std::map<std::string, std::string>::const_iterator itMap = reqData.mapData.find("��Ŀ");
	char szSql[0xff] = { 0 };
	strcat(szSql, "select s_description from t_subject where s_name = '");
	strcat(szSql, itMap->second.c_str());
	strcat(szSql, "';");
	char szRet[0xff] = { 0 };
	Ret2String(EXAM_DESCRIPTION_RET, szRet);
	QueryDB(szSql, szRet);
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	return 0;
}
/************************************************************************/
/* ��Ӧ�ͻ�������������Ϣ                                               */
/************************************************************************/
void CEmsServerMain::GetApplyMsg(DWORD dwId, const REQUEST_DATA & reqData)
{
	std::string strSql= "SELECT		ue.id AS ���Ա��,		ue.u_number AS ѧ��,		u.u_username AS ����,		u.u_class AS ���ڰ༶,		s.s_name AS ���Կ�Ŀ,		e.e_startTime AS ��ʼʱ��,		e.e_endTime AS ����ʱ��		FROM		t_user_exam ue,		t_exam e,		t_subject s,		t_user u		WHERE		e.e_id = ue.e_id		AND s.s_id = e.s_id		AND u.u_number = ue.u_number	AND ue.ue_applyStatus = 1 ";
	std::map<std::string, std::string>::const_iterator it;
	it = reqData.mapData.find("��Ŀ");
	if (it != reqData.mapData.end() && (!it->second.empty() || it->second != ""))
	{
		strSql = strSql + "  AND s.s_name = '" + it->second+"' ";
	}
	it = reqData.mapData.find("ѧ��");
	if (it != reqData.mapData.end() && (!it->second.empty() || it->second != ""))
	{
		strSql = strSql + "  AND ue.u_number like '" + it->second + "%' ";
	}

	strSql += " ORDER BY ue.id;";
	char * szRet = new char[MAX_MSG_SIZE];
	memset(szRet, 0, MAX_MSG_SIZE);
	Ret2String(APPLY_RET, szRet);
	QueryDB(const_cast<char*>(strSql.c_str()), szRet, true);
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	delete[] szRet;
}
/************************************************************************/
/* ����������Ϣ���Ƿ�ͨ������                                           */
/************************************************************************/
void CEmsServerMain::UpdateApplyMsg(DWORD dwId, const REQUEST_DATA & reqData)
{
	std::string strApplyId = "0", strPass = "false";
	std::map<std::string, std::string>::const_iterator itMap;
	itMap = reqData.mapData.find("applyId");
	if (itMap != reqData.mapData.end())
	{
		strApplyId = itMap->second;
	}
	itMap = reqData.mapData.find("pass");
	if (itMap != reqData.mapData.end())
	{
		strPass = itMap->second;
	}
	std::vector<std::string> vsApplyId;
	StringAppId2Vector(strApplyId, vsApplyId);
	std::string strSql = ""; 
	if (strPass == "true")
	{
		strSql.append("UPDATE t_user_exam		SET ue_applyStatus = 2		WHERE		1 = 1 ");
	}
	else if (strPass == "false")
	{
		strSql.append("UPDATE t_user_exam		SET ue_applyStatus = 3		WHERE		1 = 1 ");
	}
	for each (std::string applyId in vsApplyId)
	{
		UpdateDB((strSql + " and id=" + applyId + ";").c_str());
	}
}
/************************************************************************/
/* ����������                                                         */
/************************************************************************/
void CEmsServerMain::UpdateHandle(DWORD dwId, const REQUEST_DATA & reqData)
{
	ADDITION_FIRST first = reqData.enFirst;
	switch (first)
	{
	case NO_FIRST:break;
	case APPLY_MSG:
		if (reqData.enSecond ==APPLY_HANDLE && reqData.enThird == NO_THIRD)
		{
			UpdateApplyMsg(dwId, reqData);
		}
		break;
	case USER_MSG:
		if (reqData.enSecond == UPDATE_PASS && reqData.enThird == NO_THIRD)
		{
			UpdatePassword(dwId, reqData);
		}
		break;
	default:
		break;
	}
}
/************************************************************************/
/* ��string��ApplyIdת��Ϊvector                                        */
/************************************************************************/
void CEmsServerMain::StringAppId2Vector(std::string strAppId, std::vector<std::string> & vsAppId)
{
	int iPos = strAppId.find(",");
	if (iPos == std::string::npos)
	{
		vsAppId.push_back(strAppId);
		return;
	}
	vsAppId.push_back(strAppId.substr(0, iPos));
	StringAppId2Vector(strAppId.substr(iPos + 1), vsAppId);
}
/************************************************************************/
/* ���ݿ���²���                                                       */
/************************************************************************/
int CEmsServerMain::UpdateDB(const char * szSql)
{

	int iResult = mysql_query(g_msConnection, szSql);
	if (iResult != 0)
	{
		return -1;
	}
	iResult = (int)mysql_affected_rows(g_msConnection);
	return iResult;
}

/************************************************************************/
/* �����Ծ���Ϣ����                                                     */
/************************************************************************/
void CEmsServerMain::CheckMsgHandle(DWORD dwId, const REQUEST_DATA & reqData)
{
	if (reqData.enSecond == USER_ANSWER && reqData.enThird == NO_THIRD)
	{
		std::map<std::string, std::string>::const_iterator it;
		it = reqData.mapData.find("��Ŀ");
		if (it != reqData.mapData.end())
		{
			std::string strSubject = it->second;
			it = reqData.mapData.find("����");
			if (it != reqData.mapData.end())
			{
				GetUserAnswer(dwId, strSubject, it->second);
			}
		}
	}else if (reqData.enSecond == ORIGIN_ANSWER && reqData.enThird == NO_THIRD)
	{
		std::map<std::string, std::string>::const_iterator it;
		it = reqData.mapData.find("questionId");
		if (it != reqData.mapData.end())
		{
			GetAnswer(dwId, it->second);
		}
	}else if (reqData.enSecond == CHECK_NEXT && reqData.enThird == NO_THIRD)
	{
		std::map<std::string, std::string>::const_iterator it;
		it = reqData.mapData.find("questionId");
		std::string strQuestionId = it->second;
		it = reqData.mapData.find("applyId");
		std::string strApplyId = it->second;
		it = reqData.mapData.find("score");
		std::string strScore = it->second;
		it = reqData.mapData.find("subject");
		std::string strSubject = it->second;
		it = reqData.mapData.find("type");
		std::string strType = it->second;
		SetScoreAndGetNext(dwId, strApplyId, strQuestionId, strScore, strSubject, strType);
	}
}

/************************************************************************/
/* �����û���                                                         */
/************************************************************************/
void CEmsServerMain::GetUserAnswer(DWORD dwId, std::string strSubject, std::string strType)
{
	std::string strSql = "SELECT		qss.ue_id AS applyId,		qss.e_id AS questionId,		qss.e_question AS question,		qss.cq_answer AS answer,		qt.qt_score AS score		FROM		t_qss qss,		t_question_type qt,		t_subject s,		t_user_exam ue,		t_exam e,		t_subject_question_type sqt		WHERE		qss.cq_check = 0		AND qss.ue_id = ue.id		AND ue.e_id = e.e_id		AND e.s_id = s.s_id		AND qt.qt_id = sqt.q_id		AND sqt.s_id = s.s_id		AND s.s_name = '";
	strSql.append(strSubject).append("' AND qt.qt_name = '").append(strType).append("' LIMIT 0,		1");
	char * szRet = new char[MAX_MSG_SIZE];
	memset(szRet, 0, MAX_MSG_SIZE);
	Ret2String(USER_ANSWER_RET, szRet);
	QueryDB(const_cast<char*>(strSql.c_str()), szRet, true);
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	delete[] szRet;

}
/************************************************************************/
/* ���زο���                                                         */
/************************************************************************/
void CEmsServerMain::GetAnswer(DWORD dwId, std::string strQuestionId)
{
	std::string strSql = "SELECT `�ο���` as answer from t_ccf where id="+strQuestionId;
	char * szRet = new char[MAX_MSG_SIZE];
	memset(szRet, 0, MAX_MSG_SIZE);
	Ret2String(ORIGIN_ANSWER_RET, szRet);
	QueryDB(const_cast<char*>(strSql.c_str()), szRet, true);
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	delete[] szRet;
}

/************************************************************************/
/* ���óɼ���������һ����                                               */
/************************************************************************/
void CEmsServerMain::SetScoreAndGetNext(DWORD dwId, std::string applyId, std::string strQuestionId, std::string strScore, std::string strSubject, std::string strType)
{
	std::string strSql = "UPDATE t_qss SET cq_check = 1, cq_score =";
	strSql.append(strScore).append("  where ue_id=").append(applyId).append("  AND e_id = ").append(strQuestionId);
	UpdateDB(strSql.c_str());
	strSql.clear();
	GetUserAnswer(dwId, strSubject, strType);
}

/************************************************************************/
/* �ɼ���Ϣ��Ϣ����                                                     */
/************************************************************************/
void CEmsServerMain::GradeMsgHandle(DWORD dwId, const REQUEST_DATA & reqData)
{
	if (reqData.enSecond == NO_SECOND && reqData.enThird == NO_THIRD)
	{
		std::map<std::string, std::string>::const_iterator it;
		it = reqData.mapData.find("subject");
		std::string strSubject;
		std::string strNumber;
		std::string strStartTime;
		if (it != reqData.mapData.end())
		{
			strSubject = it->second;
		}
		it = reqData.mapData.find("number");
		if (it != reqData.mapData.end())
		{
			strNumber = it->second;
		}
		it = reqData.mapData.find("startTime");
		if (it != reqData.mapData.end())
		{
			strStartTime = it->second;
		}
		GetGrade(dwId, strSubject, strNumber, strStartTime);
	}
}

/************************************************************************/
/* �õ��ɼ���Ϣ                                                         */
/************************************************************************/
void CEmsServerMain::GetGrade(DWORD dwId, std::string strSubject, std::string strNumber, std::string strStartTime)
{
	std::string strSql = "SELECT \
		u.u_number AS number,	\
		u.u_username AS username,	\
		s.s_name AS subjectName,	\
		e.e_startTime AS startTime,	\
		e.e_endTime AS endTime,	\
		sum(qss.cq_score) AS score	\
		FROM	\
		t_qss qss,	\
		t_exam e,	\
		t_user_exam ue,	\
		t_subject s,	\
		t_user u	\
		WHERE	\
		qss.ue_id = ue.id	\
		AND ue.e_id = e.e_id	\
		AND ue.u_number = u.u_number	\
		AND e.s_id = s.s_id";
	if (!strSubject.empty())
	{
		strSql.append(" AND s.s_name = '").append(strSubject).append("' ");
	}
	if (!strNumber.empty())
	{
		strSql.append(" AND u.u_number like '").append(strNumber).append("%' ");
	}
	if (!strStartTime.empty())
	{
		strSql.append(" AND e.e_endTime >  '").append(strStartTime).append("' ");
	}
	strSql.append(" GROUP BY	\
		qss.ue_id	\
		ORDER BY	\
		u.u_number");
	char * szRet = new char[MAX_MSG_SIZE];
	memset(szRet, 0, MAX_MSG_SIZE);
	Ret2String(GRADE_RET, szRet);
	QueryDB(const_cast<char*>(strSql.c_str()), szRet, true);
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	delete[] szRet;
}

/************************************************************************/
/* �û���Ϣ����                                                         */
/************************************************************************/
void CEmsServerMain::UserMsgHandle(DWORD dwId, const REQUEST_DATA & reqData)
{
	if (reqData.enSecond == NO_SECOND && reqData.enThird == NO_THIRD)
	{
		std::map<std::string, std::string>::const_iterator it;
		it = reqData.mapData.find("username");
		std::string strUsername = it->second;
		it = reqData.mapData.find("password");
		std::string strPassword = it->second;
		LoginValidate(dwId, strUsername, strPassword);
	}
}

/************************************************************************/
/* ��¼��֤                                                             */
/************************************************************************/
void CEmsServerMain::LoginValidate(DWORD dwId, std::string strUsername, std::string strPassword)
{
	std::string strSql = "SELECT	\
		*	\
		FROM	\
		t_user	\
		WHERE	\
		u_role = '��ʦ'";
	strSql.append(" AND u_number = '").append(strUsername).append("' AND u_password = '").append(strPassword).append("'");
	char * szRet = new char[MAX_MSG_SIZE];
	memset(szRet, 0, MAX_MSG_SIZE);
	QueryDB(const_cast<char*>(strSql.c_str()), szRet);
	char szSend[0xff] = { 0 };
	Ret2String(LOGIN_RET, szSend);
	if (strlen(szRet) > 0)
	{
		strcat(szSend, "success");
	}
	else
	{
		strcat(szSend, "fail");
	}
	g_netServer.SendToClient(dwId, szSend, strlen(szSend));
	delete[] szRet;
}

/************************************************************************/
/* ��������                                                             */
/************************************************************************/
void CEmsServerMain::UpdatePassword(DWORD dwId, const REQUEST_DATA & reqData)
{
	std::map<std::string, std::string>::const_iterator it;
	it = reqData.mapData.find("username");
	std::string strUsername = it->second;
	it = reqData.mapData.find("password");
	std::string strPass = it->second;
	std::string strSql = "UPDATE t_user	\
						 		SET u_password = '";
	strSql.append(strPass).append("' WHERE	\
									  		u_number = '");
	strSql.append(strUsername).append("'");
	int iResult = UpdateDB(strSql.c_str());
	char * szRet = new char[0xff];
	memset(szRet, 0, 0xff);
	Ret2String(OPERATOR_RET, szRet);
	if (iResult > 0)
	{
		strcat(szRet, "success");
	}
	else
	{
		strcat(szRet, "fail");
	}
	g_netServer.SendToClient(dwId, szRet, strlen(szRet));
	delete[] szRet;

}

/************************************************************************/
/* �����������ʼ���ص�����                                             */
/************************************************************************/
void ServerNetCallBack(DWORD dwId, LPVOID lParam, int iLen)
{
	REQUEST_DATA reqData;
	CEmsServerMain serverLogic;
	char * szParam = (char*)lParam;
	std::string strParam = szParam;
	int iBegin = 0;
	int iEnd = strParam.find(SPLIT_FLAG, iBegin);
	if (iEnd != -1)
	{
		while (true)
		{
			reqData.mapData.clear();
			std::string strCurParam = strParam.substr(iBegin, iEnd - iBegin);
			serverLogic.String2RequestData((char*)strCurParam.c_str(), reqData);
			switch (reqData.enAction)
			{
			case ADD:serverLogic.AddHandle(dwId, reqData);		break;
			case DEL:
				break;
			case QUERY:serverLogic.QueryHandle(dwId, reqData);		break;
			case UPDATE:
				break;
			default:
				break;
			}
			iBegin = iEnd + strlen(SPLIT_FLAG);
			iEnd = strParam.find(SPLIT_FLAG, iBegin);
			if (iEnd == -1)
			{
				break;
			}
		}
		std::string strRet = std::to_string(IMPORT_RET);
		if (strRet.size() == 1)
		{
			strRet = "0" + strRet;
		}
		strRet += "�����������";
		char * szToClient = (char*)strRet.c_str();
		g_netServer.SendToClient(dwId, szToClient, strlen(szToClient));
		return;
	}
	serverLogic.String2RequestData((char*)lParam, reqData);
	switch (reqData.enAction)
	{
	case ADD:serverLogic.AddHandle(dwId, reqData);		break;
	case DEL:
		break;
	case QUERY:serverLogic.QueryHandle(dwId, reqData);		break;
	case UPDATE:
		serverLogic.UpdateHandle(dwId, reqData);
		break;
	default:
		break;
	}
}
