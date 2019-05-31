#ifndef __PUBLIC_DEF__
#define __PUBLIC_DEF__


#include "stdafx.h"

/************************************************************************/
/* option�ṹ��                                                         */
/************************************************************************/
typedef struct tagOption
{
	TCHAR * szName;
	TCHAR * szText;
	CDuiString strOptionId;
	bool bVisible;
}OPTIONS, *POPTONS;
/************************************************************************/
/* ִ�ж�������                                                         */
/************************************************************************/
typedef enum enAction
{
	NO_ACTION,
	ADD,
	DEL,
	QUERY,
	UPDATE
}ACTION;
/************************************************************************/
/* ������������1                                                         */
/************************************************************************/
typedef enum enAdditionFirst
{
	NO_FIRST,
	USER_MSG,
	QUESTION_MSG,
	GRADE_MSG,
	EXAM_MSG,
	APPLY_MSG,
	CHECK_MSG
}ADDITION_FIRST;
/************************************************************************/
/* ������������2                                                        */
/************************************************************************/
typedef enum enAdditionSecond
{
	NO_SECOND,
	SUBJECT,
	CCF_SUBJECT,
	JAVA_SUBJECT,
	APPLY_HANDLE,
	USER_ANSWER,
	ORIGIN_ANSWER,
	CHECK_NEXT,
	UPDATE_PASS
}ADDITION_SECOND;
/************************************************************************/
/* ������������3                                                        */
/************************************************************************/
typedef enum enAdditionThird
{
	NO_THIRD,
	QUESTION,
	PROGRAMME_QUESTION,
	SEL_QUESTION
}ADDITION_THIRD;
/************************************************************************/
/* �������ݸ�ʽ                                                         */
/************************************************************************/
typedef struct tagRequestData
{
	ACTION enAction;							//����
	ADDITION_FIRST enFirst;						//��������1
	ADDITION_SECOND enSecond;					//��������2
	ADDITION_THIRD enThird;						//��������3
	std::map<std::wstring, std::wstring> mapData;			//����
	LPVOID lParam;								//�����ֶ�
}REQUEST_DATA;
/************************************************************************/
/* ���ؽ������                                                         */
/************************************************************************/
typedef enum enResultType
{
	NO_RESULT,
	USER_RET,
	SUBJECT_QUESTION_RET,
	GRADE_RET,
	OPERATOR_RET,
	SUCCESS_RET,
	FAIL_RET,
	IMPORT_RET,
	CCF_PROGRAMME_RET,
	ADD_EXAM_RET,
	EXAM_DESCRIPTION_RET,
	APPLY_RET,
	USER_ANSWER_RET,
	ORIGIN_ANSWER_RET,
	LOGIN_RET
}RESULT_TYPE;
/************************************************************************/
/* �������ݸ�ʽ                                                         */
/************************************************************************/
typedef struct tagResponseData
{
	RESULT_TYPE enType;
	std::map<std::wstring, std::wstring> mapData;			//����

}RESPONSE_DATA;


//�Ծ�w�ָ����
#define SPLIT_FLAG "@@##%%&&&&%%##@@"
#define SPLIT_FLAGW (_T("@@##%%&&&&%%##@@"))
#define RES_KW  L"DB_Name:"
#define RES_VW  L"DB_Value:"
#endif
