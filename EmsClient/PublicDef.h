#ifndef __PUBLIC_DEF__
#define __PUBLIC_DEF__


#include "stdafx.h"

/************************************************************************/
/* option结构体                                                         */
/************************************************************************/
typedef struct tagOption
{
	TCHAR * szName;
	TCHAR * szText;
	CDuiString strOptionId;
	bool bVisible;
}OPTIONS, *POPTONS;
/************************************************************************/
/* 执行动作常量                                                         */
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
/* 动作附加属性1                                                         */
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
/* 动作附加属性2                                                        */
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
/* 动作附加属性3                                                        */
/************************************************************************/
typedef enum enAdditionThird
{
	NO_THIRD,
	QUESTION,
	PROGRAMME_QUESTION,
	SEL_QUESTION
}ADDITION_THIRD;
/************************************************************************/
/* 请求数据格式                                                         */
/************************************************************************/
typedef struct tagRequestData
{
	ACTION enAction;							//动作
	ADDITION_FIRST enFirst;						//动作属性1
	ADDITION_SECOND enSecond;					//动作属性2
	ADDITION_THIRD enThird;						//动作属性3
	std::map<std::wstring, std::wstring> mapData;			//数据
	LPVOID lParam;								//保留字段
}REQUEST_DATA;
/************************************************************************/
/* 返回结果类型                                                         */
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
/* 返回数据格式                                                         */
/************************************************************************/
typedef struct tagResponseData
{
	RESULT_TYPE enType;
	std::map<std::wstring, std::wstring> mapData;			//数据

}RESPONSE_DATA;


//试卷w分隔标记
#define SPLIT_FLAG "@@##%%&&&&%%##@@"
#define SPLIT_FLAGW (_T("@@##%%&&&&%%##@@"))
#define RES_KW  L"DB_Name:"
#define RES_VW  L"DB_Value:"
#endif
