#pragma once

#include "EmsClientMain.h"

#define USER_NAME_OFFSET 50

class CEmsLoginWnd : public CWindowWnd, public INotifyUI
{
public:
	CEmsLoginWnd();
	~CEmsLoginWnd();

public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual void    Notify(TNotifyUI& msg);
	UINT GetClassStyle();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnFinalMessage(HWND hWnd);
	void Init();
	void OnPrepare();
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLoginMsg(LPARAM lParam);

private:
	void Login();
	void AddUserNameRecord();
	void ReadUserName();
private:
	CPaintManagerUI m_pmUI;
	CEmsClientMain m_mainLogic;

private:
	CButtonUI * m_pLoginBtn;			//µÇÂ¼
	CEditUI * m_pUsername;				//ÕÊºÅ
	CEditUI * m_pPassword;				//ÃÜÂë

	std::list<std::wstring> m_lsName;	//ÓÃ»§Ãû
};

