// EmsServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "EmsNet.h"
#include "EmsServerMain.h"
#include <conio.h>
int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hMutex = CreateMutexA(NULL, false, "EmsServer");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		printf("������������...\n");
		system("pause");
		return -1;
	}
	CEmsServerMain emsServerMain;
	emsServerMain.Init();
	while (true)
	{
		char c = _getch();
		if (c=='q')
		{
			printf("�˳���y/n");
			c = _getch();
			if (c=='y')
			{
				break;
			}
			else
			{
				printf("��ȡ���˳�����...\n");
			}
			
		}
	}
	emsServerMain.Shutdown();
	return 0;
}

