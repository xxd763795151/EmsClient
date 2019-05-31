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
/* ��ʼ����������������                                                 */
/************************************************************************/
bool CEmsNet::InitServer(unsigned uPort, NetCallBack func)
{
	//��ʼ������
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);

	//������ɶ˿�
	HANDLE hCompletionPort = INVALID_HANDLE_VALUE;
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	//����CPU�����߳���
	SYSTEM_INFO stSysInfo;
	GetSystemInfo(&stSysInfo);//���ϵͳ��Ϣ

	SERVER_DATA data;
	data.func = func;
	data.hCompletionPort = hCompletionPort;
	for (size_t i = 0; i < stSysInfo.dwNumberOfProcessors; ++i)
	{
		CreateThread(NULL, 0, ServerWorkerThread, (void*)&data, 0, 0);
	}

	//����SOCKET
	//g_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	g_sServer = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_sServer == SOCKET_ERROR)
	{
		std::cout << "�������绷��ʧ�ܡ�����" << std::endl;
		return false;
	}

	//��SOCKET
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;					//Э��
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//���������κ�
	addr.sin_port = htons(uPort);
	bind(g_sServer, (sockaddr*)&addr, sizeof(SOCKADDR_IN));

	//����
	listen(g_sServer, CLIENT_NUM);//�����������ӽ����Ŀͻ��˵ĸ���

	CreateThread(NULL, 0, AcceptConnectThread, (void*)hCompletionPort, 0, 0);
	std::cout << "���������翪��......." << std::endl;
	return true;
}
/************************************************************************/
/* ��ʼ���ͻ���                                                         */
/************************************************************************/
bool CEmsNet::InitClient(unsigned uPort, NetCallBack func, char *pAddr)
{
	//��ʼ������
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);

	//����SOCKET
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	g_sClient = sClient;
	if (sClient == -1)
	{
		std::cout << "�������绷��ʧ�ܡ�����" << std::endl;
		return false;
	}

	//���ӷ�����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(uPort);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(pAddr);

	if (connect(sClient, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		//����ʧ��
		std::cout << "���ӷ�����ʧ��....." << std::endl;
		return false;
	}

	//����һ�� �߳������ͷ�����ͨ��
	CreateThread(NULL, 0, ClientWorkerThread, func, 0, 0);
	return true;
}
/************************************************************************/
/* ��ͻ��˷�����Ϣ                                                     */
/************************************************************************/
void CEmsNet::SendToClient(DWORD dwId, LPVOID lParam, int iLen)
{
	send(dwId, (char*)lParam, iLen, 0);
}
/************************************************************************/
/* ��������˷�����Ϣ                                                   */
/************************************************************************/
void CEmsNet::SendToServer(LPVOID lParam, int iLen)
{
	send(g_sClient, (char *)lParam, iLen, 0);
}
/************************************************************************/
/* �رշ�����SOCKET                                                     */
/************************************************************************/
void CEmsNet::CloseServerSocket()
{
	closesocket(g_sServer);
}

/************************************************************************/
/* �������������߳�                                                     */
/************************************************************************/
DWORD WINAPI ServerWorkerThread(LPVOID lParam)
{
	SERVER_DATA data = *((SERVER_DATA*)lParam);
	HANDLE hCompletionPort = data.hCompletionPort;
	NetCallBack func = data.func;
	EMS_OPERATOR_DATA *lpPerIOData = NULL;
	DWORD dwTransferred; //�����������ֽ�
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
			//std::cout << "�ͻ���: ["<< it->second <<"] ��������Ͽ�����..." << std::endl;
			SYSTEMTIME sys;
			GetLocalTime(&sys);
			std::string strAddr = it->second;
			char * szAddr = (char*)strAddr.c_str();
			printf("%4d/%02d/%02d %02d:%02d:%02d  �ͻ��ˣ�[%s] ��������Ͽ�����...\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, szAddr);
			g_mapSocket.erase(it);
			delete lpPerIOData;
		}
		else
		{
			//��������
			func(sClient, lpPerIOData->szMessage, dwTransferred);
			//����һ��WSARecv
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
/* �������������ӵ��߳�                                                 */
/************************************************************************/
DWORD WINAPI AcceptConnectThread(LPVOID lParam)
{
	HANDLE hCompletionPort = (HANDLE)lParam;
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(SOCKADDR_IN);
	while (true)
	{//��ѭ�������߳�����
		SOCKET sClient = accept(g_sServer, (sockaddr*)&clientAddr, &iLen);
		char * szAddr = inet_ntoa(clientAddr.sin_addr);
		
//#ifdef _DEBUG
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d/%02d/%02d %02d:%02d:%02d  �ͻ��ˣ�[%s] ���ӵ�������...\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, szAddr);
		
		g_mapSocket.insert(std::make_pair(sClient, szAddr));
//#endif
		//�������ӹ�����SOCKET���뵽��ɶ˿���
		CreateIoCompletionPort((HANDLE)sClient, hCompletionPort, (DWORD)sClient, 0);

		//����һ��WSARecv(����receive)
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
/* �ͻ��˹����߳�                                                       */
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
			MessageBox(NULL, _T("�ͷ������Ͽ�����"), _T("����"), MB_OK | MB_ICONERROR);
			return 0;
		}
		func(0, szBuf, iLen);
	}
	return 0;
}
