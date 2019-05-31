// EmsClient.cpp : ����Ӧ�ó������ڵ㡣
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
		MessageBox(NULL, _T("����������"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("../skin"));
	CPaintManagerUI::SetResourceZip(_T("EmsClientRes.zip"));
	
	//CoInitialize��Windows�ṩ��API�������������� Windows�Ե��̵߳ķ�ʽ����com����Ӧ�ó������com�⺯������CoGetMalloc���ڴ���亯����֮ǰ�����ʼ��com�⡣
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;
	g_clientMain.Init();

	//��¼
	CEmsLoginWnd * pLoginWnd = new CEmsLoginWnd;
	pLoginWnd->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
	pLoginWnd->CenterWindow();
	pLoginWnd->ShowModal();

	//����������
	CEmsMainWnd* pFrame = new CEmsMainWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("���Թ���ϵͳ"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	//_CrtDumpMemoryLeaks;
	return 0;
}



