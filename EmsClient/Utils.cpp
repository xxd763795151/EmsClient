#include "stdafx.h"
#include <windows.h>
//�򿪱����ļ��Ի���  
#include<Commdlg.h>
//ѡ���ļ��жԻ���  
#include<Shlobj.h>  
#include "Utils.h"


CUtils::CUtils()
{
}


CUtils::~CUtils()
{
}
/************************************************************************/
/* ���ļ��Ի���                                                       */
/************************************************************************/
void CUtils::OpenFileDialog(TCHAR * szFileName)
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFilename[MAX_PATH] = { 0 };//���ڽ����ļ���  
	ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С  
	ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
	//ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//���ù���  
	ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0excel Flie(*.xls)\0*.xls\0\0");//���ù��� 
	ofn.nFilterIndex = 1;//����������  
	ofn.lpstrFile = szFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL  
	ofn.nMaxFile = sizeof(szFilename);//����������  
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��  
	ofn.lpstrTitle = TEXT("��ѡ��һ��excel�ļ�");//ʹ��ϵͳĬ�ϱ������ռ���  
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��  
	if (GetOpenFileName(&ofn))
	{
		_tcscpy(szFileName, szFilename);
	}
	return;
	
}

/************************************************************************/
/* �����ļ��Ի���                                                       */
/************************************************************************/
void CUtils::SaveFileDialog()
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFilename[MAX_PATH] = { 0 };//���ڽ����ļ���  
	ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С  
	ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
	ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//���ù���  
	ofn.nFilterIndex = 1;//����������  
	ofn.lpstrFile = szFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL  
	ofn.nMaxFile = sizeof(szFilename);//����������  
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��  
	ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���  
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//Ŀ¼������ڣ������ļ�ǰ��������  
	ofn.lpstrTitle = TEXT("���浽");//ʹ��ϵͳĬ�ϱ������ռ���  
	ofn.lpstrDefExt = TEXT("cpp");//Ĭ��׷�ӵ���չ��  
	if (GetSaveFileName(&ofn))
	{
		MessageBox(NULL, szFilename, TEXT("���浽"), 0);
	}
	else{
		MessageBox(NULL, TEXT("������һ���ļ���"), NULL, MB_ICONERROR);
	}
}
/************************************************************************/
/* ѡ���ļ��Ի���                                                       */
/************************************************************************/
void CUtils::ChoiceFileDialog()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
	bi.pszDisplayName = szBuffer;//�����ļ��еĻ�����  
	bi.lpszTitle = TEXT("ѡ��һ���ļ���");//����  
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (SHGetPathFromIDList(idl, szBuffer))
	{
		MessageBox(NULL, szBuffer, TEXT("��ѡ����ļ���"), 0);
	}
	else{
		MessageBox(NULL, TEXT("��ѡ��һ���ļ���"), NULL, MB_ICONERROR);
	}
}

/************************************************************************/
/* ���ַ���������old�滻Ϊreplace                                       */
/************************************************************************/
std::wstring CUtils::ReplaceAll(std::wstring & str, std::wstring old, std::wstring replace)
{
	for (string::size_type pos(0); pos != string::npos; pos += replace.length())
	{
		if ((pos = str.find(old, pos)) != string::npos)
		{
			str.replace(pos, old.length(), replace);
		}
		else  { break; }
	}
	return   str;
}


