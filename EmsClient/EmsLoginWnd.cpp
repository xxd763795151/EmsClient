#include "stdafx.h"
#include "EmsLoginWnd.h"
#include "LoginUser.h"
#include <algorithm>


CEmsLoginWnd::CEmsLoginWnd()
{
}


CEmsLoginWnd::~CEmsLoginWnd()
{
}

LPCTSTR CEmsLoginWnd::GetWindowClassName() const
{
	return LOGIN_WIN_CLASS;
}

void CEmsLoginWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn")) { ExitProcess(0); return; }
		/*else if (msg.pSender->GetName() == _T("loginBtn")) { Close(); return; }*/
		else if (msg.pSender == m_pLoginBtn){
			Login();
		}
	}
	else if (msg.sType == _T("itemselect")) {
		if (msg.pSender->GetName() == _T("accountcombo")) {
			CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("accountedit")));
			if (pAccountEdit) pAccountEdit->SetText(msg.pSender->GetText());
		}
	}
}

LRESULT CEmsLoginWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_LOGIN_MSG:		lRes = OnLoginMsg(lParam);	break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pmUI.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CEmsLoginWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}

void CEmsLoginWnd::Init()
{
	CComboUI* pAccountCombo = static_cast<CComboUI*>(m_pmUI.FindControl(_T("accountcombo")));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pmUI.FindControl(_T("accountedit")));
	if (pAccountCombo && pAccountEdit) pAccountEdit->SetText(pAccountCombo->GetText());
	pAccountEdit->SetFocus();
	m_pLoginBtn = static_cast<CButtonUI*>(m_pmUI.FindControl(_T("loginBtn")));
	m_pUsername = pAccountEdit;
	m_pPassword = static_cast<CEditUI*>(m_pmUI.FindControl(_T("pwdedit")));
	AddUserNameRecord();
}

void CEmsLoginWnd::OnPrepare()
{

}

LRESULT CEmsLoginWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pmUI.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("login.xml"), (UINT)0, NULL, &m_pmUI);
	ASSERT(pRoot && "Failed to parse XML");
	m_pmUI.AttachDialog(pRoot);
	m_pmUI.AddNotifier(this);

	Init();
	return 0;
}

UINT CEmsLoginWnd::GetClassStyle()
{
	return UI_CLASSSTYLE_DIALOG;;
}

LRESULT CEmsLoginWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CEmsLoginWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CEmsLoginWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CEmsLoginWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pmUI.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right 
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pmUI.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CEmsLoginWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pmUI.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

/************************************************************************/
/* 登录                                                                 */
/************************************************************************/
void CEmsLoginWnd::Login()
{
	CDuiString strUserName = m_pUsername->GetText();
	CDuiString strPassword = m_pPassword->GetText();
	CLoginUser *pLoginUser = CLoginUser::GetLoginUser();
	pLoginUser->SetUserName(strUserName.GetData());
	pLoginUser->SetPassword(strPassword.GetData());
	m_mainLogic.Login(strUserName.GetData(), strPassword.GetData());
}

/************************************************************************/
/* 登录消息处理                                                         */
/************************************************************************/
LRESULT CEmsLoginWnd::OnLoginMsg(LPARAM lParam)
{
	char * szRet = (char*)lParam;
	if (strcmp(szRet, "success") == 0)
	{
		USES_CONVERSION;
		CLoginUser *pLoginUser = CLoginUser::GetLoginUser();
		if (find(m_lsName.begin(), m_lsName.end(), pLoginUser->GetUsername()) == m_lsName.end())
		{
			m_lsName.push_back(pLoginUser->GetUsername());
		}
		if (m_lsName.size() > 5)
		{
			m_lsName.pop_front();
		}
		HANDLE hFile = CreateFile(_T("username.dat"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE
			, 0, CREATE_ALWAYS, 0, 0);
		size_t iOffset = 0;
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			TCHAR szName[USER_NAME_OFFSET+1] = { 0 };
			DWORD dwWrite = 0;
			for each (std::wstring var in m_lsName)
			{
				_tcscpy(szName, var.c_str());
				char * szBuf = W2A(szName);
				WriteFile(hFile, szBuf, USER_NAME_OFFSET, &dwWrite, NULL);
				iOffset += USER_NAME_OFFSET;
				SetFilePointer(hFile, USER_NAME_OFFSET, 0, FILE_CURRENT);
			}
			
			CloseHandle(hFile);
		}
		Close();
	}
	else
	{
		MessageBox(NULL, _T("帐号或密码错误"), _T("登录错误"), MB_OK | MB_ICONWARNING);
	}

	return 0;
}

/************************************************************************/
/* 添加用户名到下拉框                                                   */
/************************************************************************/
void CEmsLoginWnd::AddUserNameRecord()
{
	ReadUserName();
	CComboUI* pAccountCombo = static_cast<CComboUI*>(m_pmUI.FindControl(_T("accountcombo")));
	for each (std::wstring var in m_lsName)
	{
		CListLabelElementUI * pLabel = new CListLabelElementUI;
		pLabel->SetText(var.c_str());
		pAccountCombo->Add(pLabel);
	}
}

/************************************************************************/
/* 读取用户名                                                           */
/************************************************************************/
void CEmsLoginWnd::ReadUserName()
{
	m_lsName.clear();
	HANDLE hFile = CreateFile(_T("username.dat"), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ
		, 0, OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		USES_CONVERSION;
		size_t iOffset = 0;
		DWORD dwHeighSize = 0;
		DWORD dwSize = GetFileSize(hFile ,&dwHeighSize);
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		while (iOffset < dwSize)
		{
			char szName[USER_NAME_OFFSET+1] = { 0 };
			DWORD dwRead = 0;
			ReadFile(hFile, szName, USER_NAME_OFFSET, &dwRead, NULL);
			if (strlen(szName) == 0)
			{
				break;
			}
			m_lsName.push_back((A2W(szName)));
			iOffset += USER_NAME_OFFSET;
			SetFilePointer(hFile, USER_NAME_OFFSET, 0, FILE_CURRENT);
		}
		CloseHandle(hFile);
	}
}


