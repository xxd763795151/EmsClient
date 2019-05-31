#include "stdafx.h"
#include "EmsNet.h"
#include <iostream>

SOCKET g_sServer = INVALID_SOCKET;
SOCKET g_sClient = INVALID_SOCKET;
std::map<SOCKET, char*> g_mapSocket;
CEmsNet::CEmsNet()
{
	m_sClient = INVALID_SOCKET;
}


CEmsNet::~CEmsNet()
{
}
/************************************************************************/
/* 初始化服务器网络连接                                                 */
/************************************************************************/
bool CEmsNet::InitServer(unsigned uPort, NetCallBack func)
{
	//初始化网络
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);

	//创建完成端口
	HANDLE hCompletionPort = INVALID_HANDLE_VALUE;
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	//创建CPU数的线程数
	SYSTEM_INFO stSysInfo;
	GetSystemInfo(&stSysInfo);//获得系统信息

	SERVER_DATA data;
	data.func = func;
	data.hCompletionPort = hCompletionPort;
	for (size_t i = 0; i < stSysInfo.dwNumberOfProcessors; ++i)
	{
		CreateThread(NULL, 0, ServerWorkerThread, (void*)&data, 0, 0);
	}

	//创建SOCKET
	//g_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	g_sServer = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_sServer == SOCKET_ERROR)
	{
		std::cout << "创建网络环境失败。。。" << std::endl;
		return false;
	}

	//绑定SOCKET
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;					//协议
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//连进来的任何
	addr.sin_port = htons(uPort);
	bind(g_sServer, (sockaddr*)&addr, sizeof(SOCKADDR_IN));

	//监听
	listen(g_sServer, CLIENT_NUM);//监听允许连接进来的客户端的个数

	CreateThread(NULL, 0, AcceptConnectThread, (void*)hCompletionPort, 0, 0);
	std::cout << "服务器网络开启......." << std::endl;
	return true;
}
/************************************************************************/
/* 初始化客户端                                                         */
/************************************************************************/
bool CEmsNet::InitClient(unsigned uPort, NetCallBack func, char *pAddr)
{
	//初始化网络
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);

	//创建SOCKET
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	g_sClient = sClient;
	if (sClient == -1)
	{
		std::cout << "创建网络环境失败。。。" << std::endl;
		return false;
	}

	//连接服务器
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(uPort);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(pAddr);

	if (connect(sClient, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		//连接失败
		std::cout << "连接服务器失败....." << std::endl;
		return false;
	}

	//创建一个 线程用来和服务器通信
	CreateThread(NULL, 0, ClientWorkerThread, func, 0, 0);
	return true;
}
/************************************************************************/
/* 向客户端发送消息                                                     */
/************************************************************************/
void CEmsNet::SendToClient(DWORD dwId, LPVOID lParam, int iLen)
{
	send(dwId, (char*)lParam, iLen, 0);
}
/************************************************************************/
/* 向服务器端发送消息                                                   */
/************************************************************************/
void CEmsNet::SendToServer(LPVOID lParam, int iLen)
{
	send(g_sClient, (char *)lParam, iLen, 0);
}
/************************************************************************/
/* 关闭服务器SOCKET                                                     */
/************************************************************************/
void CEmsNet::CloseServerSocket()
{
	closesocket(g_sServer);
}

/************************************************************************/
/* 服务器工作者线程                                                     */
/************************************************************************/
DWORD WINAPI ServerWorkerThread(LPVOID lParam)
{
	SERVER_DATA data = *((SERVER_DATA*)lParam);
	HANDLE hCompletionPort = data.hCompletionPort;
	NetCallBack func = data.func;
	EMS_OPERATOR_DATA *lpPerIOData = NULL;
	DWORD dwTransferred; //传过来多少字节
	SOCKET sClient;
	while (true)
	{
		GetQueuedCompletionStatus(
			hCompletionPort,
			&dwTransferred,
			(PULONG_PTR)&sClient,
			(LPOVERLAPPED*)&lpPerIOData,
			INFINITE);
		DWORD dwErr = WSAGetLastError();
		if (dwTransferred == -1)
		{
			return 0;
		}
		if (dwTransferred == 0)
		{
			std::map<SOCKET, char*>::iterator it = g_mapSocket.find(sClient);
			//std::cout << "客户端: ["<< it->second <<"] 与服务器断开连接..." << std::endl;
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			std::string strAddr = it->second;
			char * szAddr = (char*)strAddr.c_str();
			printf("%4d/%02d/%02d %02d:%02d:%02d  客户端：[%s] 与服务器断开连接...\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, szAddr);
			g_mapSocket.erase(it);
			delete lpPerIOData;
		}
		else
		{
			//正常接收
			func(sClient, lpPerIOData->szMessage, dwTransferred);
			//激活一个WSARecv
			memset(lpPerIOData, 0, sizeof(EMS_OPERATOR_DATA));
			lpPerIOData->buffer.len = MAX_MSG_SIZE;
			lpPerIOData->buffer.buf = lpPerIOData->szMessage;
			WSARecv(sClient,
				&lpPerIOData->buffer,
				1,
				&lpPerIOData->recvLen,
				&lpPerIOData->flags,
				&lpPerIOData->overlap,
				NULL);
		}
	}
	return 0;
}
/************************************************************************/
/* 服务器接受连接的线程                                                 */
/************************************************************************/
DWORD WINAPI AcceptConnectThread(LPVOID lParam)
{
	HANDLE hCompletionPort = (HANDLE)lParam;
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(SOCKADDR_IN);
	while (true)
	{//死循环接收线程连接
		SOCKET sClient = accept(g_sServer, (sockaddr*)&clientAddr, &iLen);
		char * szAddr = inet_ntoa(clientAddr.sin_addr);
		
//#ifdef _DEBUG
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d/%02d/%02d %02d:%02d:%02d  客户端：[%s] 连接到服务器...\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, szAddr);
		
		g_mapSocket.insert(std::make_pair(sClient, szAddr));
//#endif
		//把新连接过来的SOCKET加入到完成端口中
		CreateIoCompletionPort((HANDLE)sClient, hCompletionPort, (DWORD)sClient, 0);

		//激活一个WSARecv(代替receive)
		EMS_OPERATOR_DATA *lpPerIOData = new EMS_OPERATOR_DATA;
		memset(lpPerIOData, 0, sizeof(EMS_OPERATOR_DATA));
		lpPerIOData->buffer.len = MAX_MSG_SIZE;
		lpPerIOData->buffer.buf = lpPerIOData->szMessage;
		WSARecv(sClient,
			&lpPerIOData->buffer,
			1,
			&lpPerIOData->recvLen,
			&lpPerIOData->flags,
			&lpPerIOData->overlap,
			NULL);
	}
	return 0;
}
/************************************************************************/
/* 客户端工作线程                                                       */
/************************************************************************/
DWORD WINAPI ClientWorkerThread(LPVOID lParam)
{
	NetCallBack func = (NetCallBack)lParam;
	char szBuf[MAX_MSG_SIZE] = { 0 };
	while (true)
	{
		int iLen;
		if ((iLen = recv(g_sClient, szBuf, MAX_MSG_SIZE, 0)) == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("和服务器断开连接"), _T("错误"), MB_OK | MB_ICONERROR);
			return 0;
		}
		func(0, szBuf, iLen);
	}
	return 0;
}
