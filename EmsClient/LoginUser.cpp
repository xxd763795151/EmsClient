#include "stdafx.h"
#include "LoginUser.h"


CLoginUser * CLoginUser::m_pThis = NULL;
CLoginUser::CLoginUser()
{
}


CLoginUser::~CLoginUser()
{
	//DestroyLoginUser();
}

CLoginUser * CLoginUser::GetLoginUser()
{
	if (m_pThis == NULL)
	{
		m_pThis = new CLoginUser;
	}
	return m_pThis;
}

void CLoginUser::DestroyLoginUser()
{
	if (m_pThis != NULL)
	{
		delete m_pThis;
	}
}

