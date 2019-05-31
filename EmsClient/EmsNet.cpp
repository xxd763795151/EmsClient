#include "stdafx.h"
#include "EmsNet.h"
#include <iostream>
SOCKET g_sServer = INVALID_SOCKET;
//SOCKET g_sClient = INVALID_SOCKET;

CEmsNet::CEmsNet()
{
}


CEmsNet::~CEmsNet()
{
	if (m_pThis != NULL)
	{
		delete m_pThis;
	}
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
	g_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	std::cout << "�������Ѿ��ɹ�����......." << std::endl;
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
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == -1)
	{
		MessageBox(NULL, _T("�������绷��ʧ��"), _T("����"), MB_OK | MB_ICONERROR);
		return false;
	}

	//���ӷ�����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(uPort);
	serverAddr.sin_addr.S_un.S_addr = inet_addr(pAddr);

	if (connect(m_sClient, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		//����ʧ��
		MessageBox(NULL, _T("���ӷ�����ʧ��"), _T("����"), MB_OK | MB_ICONERROR);
		ExitProcess(0);
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
	send(m_sClient, (char *)lParam, iLen, 0);
}
/************************************************************************/
/* ���մӷ�����������                                                 */
/************************************************************************/
bool CEmsNet::RecvFromServer(SOCKET s, char* szBuf, int iLen)
{
	try
	{
		recv(s, szBuf, iLen, 0);
	}
	catch (...)
	{
		//e->
		DWORD dwErr = GetLastError();
	}
	
	
	return true;
}
/************************************************************************/
/* ��������                                                             */
/************************************************************************/
CEmsNet * CEmsNet::m_pThis = NULL;
CEmsNet * CEmsNet::CreateInstance()
{
	if (m_pThis == NULL)
	{
		return m_pThis = new CEmsNet;
	}
	return m_pThis;
	
}

SOCKET CEmsNet::GetClientSocket()
{
	return CEmsNet::m_pThis->m_sClient;
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

		if (dwTransferred == -1)
		{
			return 0;
		}
		if (dwTransferred == 0)
		{
			std::cout << "�ͻ��˶Ͽ������ˡ�����" << std::endl;
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

		//�������ӹ�����SOCKET���뵽��ɶ˿���
		CreateIoCompletionPort((HANDLE)sClient, hCompletionPort, (DWORD)sClient, 0);

		//����һ��WSARecv(����receive),���Ǹ�����ɶ˿ڣ�����Ϣ��ʱ���������
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
	SOCKET sClient = CEmsNet::CreateInstance()->GetClientSocket();
	NetCallBack func = (NetCallBack)lParam;
	char szBuf[MAX_MSG_SIZE] = { 0 };
	while (true)
	{
		int iLen;
		if ((iLen = recv(sClient, szBuf, MAX_MSG_SIZE, 0)) == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("�ͷ������Ͽ�����"), _T("����"), MB_OK | MB_ICONERROR);
			ExitProcess(0);
			return 0;
		}
		func(sClient, szBuf, iLen);
		Sleep(500);
		memset(szBuf, 0, MAX_MSG_SIZE);
	}
	return 0;
}
