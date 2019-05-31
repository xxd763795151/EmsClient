#include "stdafx.h"
#include <windows.h>
//打开保存文件对话框  
#include<Commdlg.h>
//选择文件夹对话框  
#include<Shlobj.h>  
#include "Utils.h"


CUtils::CUtils()
{
}


CUtils::~CUtils()
{
}
/************************************************************************/
/* 打开文件对话框                                                       */
/************************************************************************/
void CUtils::OpenFileDialog(TCHAR * szFileName)
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFilename[MAX_PATH] = { 0 };//用于接收文件名  
	ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
	ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
	//ofn.lpstrFilter = TEXT("所有文件\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//设置过滤  
	ofn.lpstrFilter = TEXT("所有文件\0*.*\0excel Flie(*.xls)\0*.xls\0\0");//设置过滤 
	ofn.nFilterIndex = 1;//过滤器索引  
	ofn.lpstrFile = szFilename;//接收返回的文件名，注意第一个字符需要为NULL  
	ofn.nMaxFile = sizeof(szFilename);//缓冲区长度  
	ofn.lpstrInitialDir = NULL;//初始目录为默认  
	ofn.lpstrTitle = TEXT("请选择一个excel文件");//使用系统默认标题留空即可  
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项  
	if (GetOpenFileName(&ofn))
	{
		_tcscpy(szFileName, szFilename);
	}
	return;
	
}

/************************************************************************/
/* 保存文件对话框                                                       */
/************************************************************************/
void CUtils::SaveFileDialog()
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFilename[MAX_PATH] = { 0 };//用于接收文件名  
	ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
	ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
	ofn.lpstrFilter = TEXT("所有文件\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//设置过滤  
	ofn.nFilterIndex = 1;//过滤器索引  
	ofn.lpstrFile = szFilename;//接收返回的文件名，注意第一个字符需要为NULL  
	ofn.nMaxFile = sizeof(szFilename);//缓冲区长度  
	ofn.lpstrInitialDir = NULL;//初始目录为默认  
	ofn.lpstrTitle = TEXT("请选择一个文件");//使用系统默认标题留空即可  
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//目录必须存在，覆盖文件前发出警告  
	ofn.lpstrTitle = TEXT("保存到");//使用系统默认标题留空即可  
	ofn.lpstrDefExt = TEXT("cpp");//默认追加的扩展名  
	if (GetSaveFileName(&ofn))
	{
		MessageBox(NULL, szFilename, TEXT("保存到"), 0);
	}
	else{
		MessageBox(NULL, TEXT("请输入一个文件名"), NULL, MB_ICONERROR);
	}
}
/************************************************************************/
/* 选择文件对话框                                                       */
/************************************************************************/
void CUtils::ChoiceFileDialog()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
	bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区  
	bi.lpszTitle = TEXT("选择一个文件夹");//标题  
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (SHGetPathFromIDList(idl, szBuffer))
	{
		MessageBox(NULL, szBuffer, TEXT("你选择的文件夹"), 0);
	}
	else{
		MessageBox(NULL, TEXT("请选择一个文件夹"), NULL, MB_ICONERROR);
	}
}

/************************************************************************/
/* 将字符串的所有old替换为replace                                       */
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


