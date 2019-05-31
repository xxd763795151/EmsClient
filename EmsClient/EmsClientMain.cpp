#include "stdafx.h"
#include "EmsClientMain.h"
#include "CSpreadSheet.h"
#include "PublicDef.h"
#include "CStringConvert.h"
CEmsClientMain::CEmsClientMain()
{
	m_pClientNet = CEmsNet::CreateInstance();
}


CEmsClientMain::~CEmsClientMain()
{
}
/************************************************************************/
/* 客户端环境配置初始化                                                                     */
/************************************************************************/
int CEmsClientMain::Init()
{
	int iRet = SUCCESS;
	do
	{
		char szAddr[0xff] = { 0 };
		::GetPrivateProfileStringA("net", "net.addr", "127.0.0.1", szAddr, 0xff, "..\\conf\\client.ini");
		int iPort = ::GetPrivateProfileIntA("net", "net.port", SERVER_PORT, "..\\conf\\client.ini");
		bool bInitClient = m_pClientNet->InitClient(iPort, ClientNetCallBack, szAddr);
		if (!bInitClient)
		{
			iRet = NET_ERR;
			break;
		}
	} while (false);

	return iRet;
}
/************************************************************************/
/* 试题导入题库                                                         */
/************************************************************************/
int CEmsClientMain::ImportQuestion(TCHAR * szFileName, TCHAR* szSheet, LPCTSTR szSubject, LPCTSTR szType)
{
	char * szSend = new char[MAX_MSG_SIZE];
	memset(szSend, 0, MAX_MSG_SIZE);
	char * szBuf = new char[MAX_MSG_SIZE];
	REQUEST_DATA reqData;
	reqData.enAction = ADD;
	reqData.enFirst = QUESTION_MSG;
	CSpreadSheet ss(szFileName, szSheet);
	int iRowLen = ss.GetTotalRows();
	int iCloumLen = ss.GetTotalColumns();
	CStringArray strArrFields, strArrValue;
	ss.GetFieldNames(strArrFields);
	int iNums = strArrFields.GetSize();
	for (int i = 1; i < iRowLen;++i)
	{
		strArrValue.RemoveAll();
		reqData.mapData.clear();
		ss.ReadRow(strArrValue, i+1);
		for (int j = 0; j < iNums; ++j)
		{
			std::wstring strKey = strArrFields.GetAt(j);
			std::wstring strValue = strArrValue.GetAt(j);
			reqData.mapData.insert(make_pair(strKey, strValue));
		}
		reqData.enSecond = GetSecond(szSubject);
		reqData.enThird = GetThird(szType);
		ZeroMemory(szBuf, MAX_MSG_SIZE);
		RequestData2String(reqData, szBuf, MAX_MSG_SIZE);
		strcat(szSend, szBuf);
		strcat(szSend, SPLIT_FLAG);
		
		//send(m_pClientNet->GetClientSocket(), szBuf, strlen(szBuf), 0);
	/*	DWORD dwErr = WSAGetLastError();
		TCHAR szErr[0xf] = {0};
		wsprintf(szErr, _T("%d"), dwErr);
		MessageBox(NULL, szErr, _T("ss"), MB_OK);*/
		//Sleep(100);
		//Sleep(2000);
		;
	}
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	delete[] szBuf;
	delete[] szSend;;
	return SUCCESS;
}
/************************************************************************/
/* 将请求的数据内容格式转换为字符串形式                                 */
/************************************************************************/
void CEmsClientMain::RequestData2String(const REQUEST_DATA& reqData, char * szBuf, const int& iLen)
{
	string strAction = std::to_string(reqData.enAction);
	if (strAction.size() < 2) strAction = "0" + strAction;
	string strFirst = std::to_string(reqData.enFirst);
	if (strFirst.size() < 2) strFirst = "0" + strFirst;
	string strSecond = std::to_string(reqData.enSecond);
	if (strSecond.size() < 2) strSecond = "0" + strSecond;
	string strThird = std::to_string(reqData.enThird);
	if (strThird.size() < 2) strThird = "0" + strThird;

	strcat(szBuf, strAction.c_str());
	strcat(szBuf, strFirst.c_str());
	strcat(szBuf, strSecond.c_str());
	strcat(szBuf, strThird.c_str());

	/*sprintf_s(szBuf, iLen, "Request_Action:%d,Request_Frist:%d,Request_Second:%d,Request_Third:%d", reqData.enAction, reqData.enFirst, reqData.enSecond, reqData.enThird);
	strcat(szBuf, ",Request_Data:");*/

	if (!reqData.mapData.empty())
	{
		for (std::map<std::wstring, std::wstring>::const_iterator it = reqData.mapData.begin(); it != reqData.mapData.end(); it++)
		{
			std::string tmpKey = U2A(it->first);
			std::string tmpValue = U2A(it->second);
			std::string key = "Request_Data_Key:" + tmpKey;
			std::string value = "Request_Data_Value:" + tmpValue;
			strcat(szBuf, ",");
			strcat(szBuf, key.c_str());
			strcat(szBuf, ",");
			strcat(szBuf, value.c_str());
		}
	}
	else
	{
		strcat(szBuf, ",NULL");
	}
}
/************************************************************************/
/* 返回所有课程名，以及对应课程所有题型                                 */
/************************************************************************/
int CEmsClientMain::GetSubjectNameAndQuestionName()
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = QUESTION_MSG;
	reqData.enSecond = SUBJECT;
	reqData.enThird = QUESTION;
	char * szBuf = new char[MAX_MSG_SIZE];
	ZeroMemory(szBuf, MAX_MSG_SIZE);
	RequestData2String(reqData, szBuf, MAX_MSG_SIZE);
	send(m_pClientNet->GetClientSocket(), szBuf, strlen(szBuf), 0);
	delete[] szBuf;
	return SUCCESS;
}
/************************************************************************/
/* 返回附加属性3                                                        */
/************************************************************************/
ADDITION_THIRD CEmsClientMain::GetThird(LPCTSTR szThird)
{
	if (_tcsstr(szThird, _T("编程")))
	{
		return PROGRAMME_QUESTION;
	}
	else if (_tcsstr(szThird, _T("单选")))
	{
		return SEL_QUESTION;
	}
	return NO_THIRD;
}
/************************************************************************/
/* 返回附加属性2                                                        */
/************************************************************************/
ADDITION_SECOND CEmsClientMain::GetSecond(LPCTSTR szSecond)
{
	if (_tcsstr(szSecond, _T("CCF")))
	{
		return CCF_SUBJECT;
	}else if (_tcsstr(szSecond,_T("java")))
	{
		return JAVA_SUBJECT;
	}
	return NO_SECOND;
}
/************************************************************************/
/* 试题浏览                                                             */
/************************************************************************/
int CEmsClientMain::ScanQuestion(LPCTSTR szSubject, LPCTSTR szType)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = QUESTION_MSG;
	reqData.enSecond = GetSecond(szSubject);
	reqData.enThird = GetThird(szType);

	char szSend[0xff] = { 0 };
	RequestData2String(reqData, szSend, 0xff);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);

	return SUCCESS;
}


/************************************************************************/
/* string 转换为vector<map<string,string>>形式                          */
/************************************************************************/
void CEmsClientMain::String2VecMap(std::wstring strData, std::vector<map<std::wstring, std::wstring> > &vecMap)
{
	
	if (!strData.empty() && strData != _T(""))
	{
		int iBegin = 0;
		int iEnd = strData.find(SPLIT_FLAGW, iBegin);

		
		std::wstring strKeyFind = RES_KW;
		std::wstring strValueFind = RES_VW;
		std::wstring strSplitFlag = SPLIT_FLAGW;
		
		while (true)
		{
			size_t kPos = 0, vPos = 0;
			int iRet = 0;
			std::map<std::wstring, std::wstring> mapString;
			std::wstring strCurData = strData.substr(iBegin, iEnd - iBegin);
			iRet = strCurData.find(strKeyFind, kPos);
			while (true)
			{
				kPos = iRet + strKeyFind.size();
				vPos = strCurData.find(strValueFind, iRet);
				std::wstring strKey = strCurData.substr(kPos, vPos - kPos);
				iRet = strCurData.find(strKeyFind, vPos);
				if (iRet == -1)
				{
					int iValueBegin = vPos + strValueFind.size();
					std::wstring strValue = strCurData.substr(iValueBegin, strCurData.size() - iValueBegin);
					mapString.insert(std::make_pair(strKey, strValue));
					break;
				}
				int iValueBegin = vPos + strValueFind.size();
				std::wstring strValue = strCurData.substr(iValueBegin, iRet - iValueBegin);
				mapString.insert(std::make_pair(strKey, strValue));
			}
			vecMap.push_back(mapString);
			iBegin = iEnd + _tcslen(SPLIT_FLAGW);
			iEnd = strData.find(SPLIT_FLAGW, iBegin);
			if (iEnd == -1)
			{
				break;
			}
		}
		
	}

}
/************************************************************************/
/* 增加考试信息                                                         */
/************************************************************************/
int CEmsClientMain::AddExamMsg(LPCTSTR szSubject, LPCTSTR szStartTime, LPCTSTR szEndTime)
{
	REQUEST_DATA reqData;
	reqData.enAction = ADD;
	reqData.enFirst = EXAM_MSG;
	reqData.mapData.clear();
	reqData.mapData.insert(make_pair(_T("科目"), szSubject));
	reqData.mapData.insert(make_pair(_T("开始时间"), szStartTime));
	reqData.mapData.insert(make_pair(_T("结束时间"), szEndTime));
	char szSend[MAX_PATH] = { 0 };
	RequestData2String(reqData, szSend, MAX_PATH);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	return SUCCESS;
}
/************************************************************************/
/* 向服务器发送获得试卷描述的请求                                       */
/************************************************************************/
int CEmsClientMain::GetExamDescription(LPCTSTR szSubject)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = EXAM_MSG;
	reqData.enSecond = NO_SECOND;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(make_pair(_T("科目"), szSubject));
	char szSend[MAX_PATH] = { 0 };
	RequestData2String(reqData, szSend, MAX_PATH);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	return SUCCESS;
}
/************************************************************************/
/* 获得申请信息                                                         */
/************************************************************************/
int CEmsClientMain::GetApplyMsg(LPCTSTR szSubject, LPCTSTR szNumber)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = APPLY_MSG;
	reqData.enSecond = NO_SECOND;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("科目"), szSubject));
	reqData.mapData.insert(std::make_pair(_T("学号"), szNumber));

	char szSend[MAX_PATH] = { 0 };
	RequestData2String(reqData, szSend, MAX_PATH);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	return SUCCESS;

}
/************************************************************************/
/* 申请通过与否处理                                                     */
/************************************************************************/
void CEmsClientMain::HandleApplyExam(std::vector<std::wstring> & m_vsApplyId, int iOffset /*= -1*/, bool bPass /*= true*/)
{
	std::wstring strApplyId = _T("");
	if (m_vsApplyId.empty())
	{
		return;
	}
	if (iOffset == -1)
	{
		for each (std::wstring var in m_vsApplyId)
		{
			strApplyId.append(var).append(_T(","));
		}
		strApplyId.pop_back();
	}
	else 
	{
		strApplyId.append(m_vsApplyId[iOffset]);
	}
	REQUEST_DATA reqData;
	reqData.enAction = UPDATE;
	reqData.enFirst = APPLY_MSG;
	reqData.enSecond = APPLY_HANDLE;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("applyId"), strApplyId));
	if (bPass)
	{
		reqData.mapData.insert(std::make_pair(_T("pass"), _T("true")));
	}
	else
	{
		reqData.mapData.insert(std::make_pair(_T("pass"), _T("false")));
	}
	char * szSend = new char[MAX_MSG_SIZE];
	ZeroMemory(szSend, MAX_MSG_SIZE);
	RequestData2String(reqData, szSend, MAX_MSG_SIZE);
	
	if (iOffset == -1)
	{
		m_vsApplyId.clear();
	}
	else
	{
		std::vector<std::wstring>::iterator it = m_vsApplyId.begin()+iOffset;
		m_vsApplyId.erase(it);
	}
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	delete[] szSend;

}

/************************************************************************/
/* 获得试题及用户的答案                                                 */
/************************************************************************/
int CEmsClientMain::GetQuestionAndUserAnswer(LPCTSTR szSubject, LPCTSTR szType)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = CHECK_MSG;
	reqData.enSecond = USER_ANSWER;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("科目"), szSubject));
	reqData.mapData.insert(std::make_pair(_T("题型"), szType));
	char szSend[0xff] = { 0 };
	RequestData2String(reqData, szSend, 0xff);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	return 0;
}

/************************************************************************/
/* 通过试题ID获得其答案                                                 */
/************************************************************************/
void CEmsClientMain::GetAnswerById(std::wstring strQuestionId)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = CHECK_MSG;
	reqData.enSecond = ORIGIN_ANSWER;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("questionId"), strQuestionId));
	char szSend[0xff] = { 0 };
	RequestData2String(reqData, szSend, 0xff);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	return ;
}

/************************************************************************/
/* 检查下一道题                                                         */
/************************************************************************/
int CEmsClientMain::GetCheckNextAnswer(std::wstring strApplyId, std::wstring strQuestionId, std::wstring strScore, std::wstring strSubject, std::wstring strType)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = CHECK_MSG;
	reqData.enSecond = CHECK_NEXT;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("applyId"), strApplyId));
	reqData.mapData.insert(std::make_pair(_T("questionId"), strQuestionId));
	reqData.mapData.insert(std::make_pair(_T("score"), strScore));
	reqData.mapData.insert(std::make_pair(_T("subject"), strSubject));
	reqData.mapData.insert(std::make_pair(_T("type"), strType));
	char szSend[0xfff] = { 0 };
	RequestData2String(reqData, szSend, 0xfff);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	return 0;
}

/************************************************************************/
/* 成绩查询                                                             */
/************************************************************************/
int CEmsClientMain::GradeScan(LPCTSTR szSubject, LPCTSTR szNumber, LPCTSTR szStartTime)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = GRADE_MSG;
	reqData.enSecond = NO_SECOND;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("subject"), szSubject));
	reqData.mapData.insert(std::make_pair(_T("number"), szNumber));
	reqData.mapData.insert(std::make_pair(_T("startTime"), szStartTime));
	char szSend[0xfff] = { 0 };
	RequestData2String(reqData, szSend, 0xfff);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
	return 0;
}

/************************************************************************/
/* 用户登录                                                             */
/************************************************************************/
void CEmsClientMain::Login(LPCTSTR szUsername, LPCTSTR szPassword)
{
	REQUEST_DATA reqData;
	reqData.enAction = QUERY;
	reqData.enFirst = USER_MSG;
	reqData.enSecond = NO_SECOND;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("username"), szUsername));
	reqData.mapData.insert(std::make_pair(_T("password"), szPassword));
	char szSend[0xfff] = { 0 };
	RequestData2String(reqData, szSend, 0xfff);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
}

/************************************************************************/
/* 更新密码                                                             */
/************************************************************************/
void CEmsClientMain::UpdatePassword(LPCTSTR szUsername, LPCTSTR szPassword)
{
	REQUEST_DATA reqData;
	reqData.enAction = UPDATE;
	reqData.enFirst = USER_MSG;
	reqData.enSecond = UPDATE_PASS;
	reqData.enThird = NO_THIRD;
	reqData.mapData.clear();
	reqData.mapData.insert(std::make_pair(_T("username"), szUsername));
	reqData.mapData.insert(std::make_pair(_T("password"), szPassword));
	char szSend[0xfff] = { 0 };
	RequestData2String(reqData, szSend, 0xfff);
	send(m_pClientNet->GetClientSocket(), szSend, strlen(szSend), 0);
}



/************************************************************************/
/* 客户端网络回调函数                                                   */
/************************************************************************/
void ClientNetCallBack(DWORD dwId, LPVOID lParam, int iLen)
{
	char * szRet = (char*)lParam;
	std::string strResData = szRet;
	RESULT_TYPE resultType = (RESULT_TYPE)std::stoi(strResData.substr(0, 2));
	szRet = szRet + 2;
	HWND m_hWnd = ::FindWindow(EMS_MAIN_WND, NULL);
	HWND m_hLoginWnd = ::FindWindow(LOGIN_WIN_CLASS, NULL);
	switch (resultType)
	{
	case USER_RET:break;
	case SUBJECT_QUESTION_RET:
		PostMessage(m_hWnd, WM_SHOW_SUBJECT_QUESTION, (WPARAM)0, (LPARAM)szRet);
		break;
	case GRADE_RET:PostMessage(m_hWnd, WM_GRADE_SHOW_MSG, (WPARAM)0, (LPARAM)szRet);	break;
	case OPERATOR_RET:MessageBoxA(NULL, szRet, "提示", MB_OK | MB_ICONINFORMATION);		break;
	case SUCCESS_RET:break;
	case FAIL_RET:break;
	case IMPORT_RET:PostMessage(m_hWnd, WM_IMPORT_RESPONSE, (WPARAM)0, (LPARAM)szRet);	break;
	case CCF_PROGRAMME_RET:PostMessage(m_hWnd, WM_SCAN_QUESTION, (WPARAM)CCF_PROGRAMME_RET, (LPARAM)szRet);  break;
	case ADD_EXAM_RET:PostMessage(m_hWnd, WM_ADD_EXAM_RET, (WPARAM)0, (LPARAM)szRet);    break;
	case EXAM_DESCRIPTION_RET:PostMessage(m_hWnd, WM_EXAM_DESCRIPTION, (WPARAM)0, (LPARAM)szRet); 		break;
	case APPLY_RET:PostMessage(m_hWnd, WM_APPLY_MSG, (WPARAM)0, (LPARAM)szRet);			break;
	case USER_ANSWER_RET:PostMessage(m_hWnd, WM_USER_ANSWER_MSG, (WPARAM)0, (LPARAM)szRet);				break;
	case ORIGIN_ANSWER_RET: PostMessage(m_hWnd, WM_ORIGIN_ANSWER_MSG, (WPARAM)0, (LPARAM)szRet);			 break;
	case LOGIN_RET:if (m_hLoginWnd != NULL)
	{
		PostMessage(m_hLoginWnd, WM_LOGIN_MSG, (WPARAM)0, (LPARAM)szRet);
	}
			break;
	default:
		break;
	}
}
