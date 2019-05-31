#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
#define MAX_MSG_SIZE 204800
#define CLIENT_NUM   10
//进行消息处理的回调函数
typedef void(*NetCallBack)(DWORD dwId, LPVOID lParam, int iLen);

typedef struct tagServerData
{
	HANDLE hCompletionPort;
	NetCallBack func;
}SERVER_DATA;

typedef struct tagEmsOperatorData
{
	WSAOVERLAPPED overlap;
	WSABUF buffer;
	char szMessage[MAX_MSG_SIZE];
	DWORD recvLen;
	DWORD flags;
}EMS_OPERATOR_DATA;

class CEmsNet
{
public:
	CEmsNet();
	~CEmsNet();


public:
	bool InitServer(unsigned uPort, NetCallBack func);
	bool InitClient(unsigned uPort, NetCallBack func, char *pAddr);
	void SendToClient(DWORD dwId, LPVOID lParam, int iLen);
	void SendToServer(LPVOID lParam, int iLen);
	void CloseServerSocket();
private:
	
	SOCKET m_sClient;
	
};
extern SOCKET g_sServer;
extern SOCKET g_sClient;
DWORD WINAPI ServerWorkerThread(LPVOID lParam);
DWORD WINAPI AcceptConnectThread(LPVOID lParam);
DWORD WINAPI ClientWorkerThread(LPVOID lParam);