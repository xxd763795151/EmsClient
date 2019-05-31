// EmsClient.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "EmsClient.h"
#include "EmsMainWnd.h"
#include "EmsClientMain.h"
#include "EmsLoginWnd.h"

CEmsClientMain g_clientMain;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	HANDLE hMutex = CreateMutex(NULL, false, _T("EmsClient"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, _T("程序已运行"), _T("提示"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("../skin"));
	CPaintManagerUI::SetResourceZip(_T("EmsClientRes.zip"));
	
	//CoInitialize是Windows提供的API函数，用来告诉 Windows以单线程的方式创建com对象。应用程序调用com库函数（除CoGetMalloc和内存分配函数）之前必须初始化com库。
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	g_clientMain.Init();

	//登录
	CEmsLoginWnd * pLoginWnd = new CEmsLoginWnd;
	pLoginWnd->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginWnd->CenterWindow();
	pLoginWnd->ShowModal();

	//创建主窗口
	CEmsMainWnd* pFrame = new CEmsMainWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("考试管理系统"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	//_CrtDumpMemoryLeaks;
	return 0;
}



