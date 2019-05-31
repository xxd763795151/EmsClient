// EmsServer.cpp : 定义控制台应用程序的入口点。
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
		printf("服务器已运行...\n");
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
			printf("退出？y/n");
			c = _getch();
			if (c=='y')
			{
				break;
			}
			else
			{
				printf("已取消退出动作...\n");
			}
			
		}
	}
	emsServerMain.Shutdown();
	return 0;
}

