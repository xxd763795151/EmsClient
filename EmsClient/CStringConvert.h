#ifndef __STRING_CONVER_H__
#define __STRING_CONVER_H__
#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

string U2A(const wstring& str)//Unicode×Ö·û×ªAscii×Ö·û</span>  
{
	string strDes;
	if (str.empty())
		goto __end;
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	if (0 == nLen)
		goto __end;
	char* pBuffer = new char[nLen + 1];
	memset(pBuffer, 0, nLen + 1);
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), str.size(), pBuffer, nLen, NULL, NULL);
	pBuffer[nLen] = '\0';
	strDes.append(pBuffer);
	delete[] pBuffer;
__end:
	return strDes;
}

wstring A2U(const string& str)//Ascii×Ö·û×ª  
{
	wstring strDes;
	if (str.empty())
		goto __end;
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	if (0 == nLen)
		goto __end;
	wchar_t* pBuffer = new wchar_t[nLen + 1];
	memset(pBuffer, 0, nLen + 1);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), pBuffer, nLen);
	pBuffer[nLen] = '\0';
	strDes.append(pBuffer);
	delete[] pBuffer;
__end:
	return strDes;
}

string U2Utf(const wstring& wstrUnicode)//Unicode×ªutf8    
{
	string strRet;
	if (wstrUnicode.empty())
		return strRet;
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wstrUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nLen + 1];
	pBuffer[nLen] = '\0';
	nLen = WideCharToMultiByte(CP_UTF8, 0, wstrUnicode.c_str(), -1, pBuffer, nLen, NULL, NULL);
	strRet.append(pBuffer);
	delete[] pBuffer;
	return strRet;
}

wstring Utf2U(const string &str)//utf8×ªUnicode  
{
	int u16Len = ::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), (int)str.size(), NULL, 0);
	wchar_t* wstrBuf = new wchar_t[u16Len + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, str.c_str(), (int)str.size(), wstrBuf, u16Len);
	wstrBuf[u16Len] = L'\0';
	wstring wStr;
	wStr.assign(wstrBuf, u16Len);
	delete[] wstrBuf;
	return wStr;
}
//·Ö¸î×Ö·û´®  
bool SplitString(const wstring& strSource, const wstring& strFlag, vector<wstring>& paramList)
{
	if (strSource.empty() || strFlag.empty())
		return false;
	paramList.clear();
	size_t nBeg = 0;
	size_t nFind = strSource.find(strFlag, nBeg);
	if (nFind == std::wstring::npos)
		paramList.push_back(strSource);
	else
	{
		while (true)
		{
			if (nFind != nBeg)
				paramList.push_back(strSource.substr(nBeg, nFind - nBeg));
			nBeg = nFind + strFlag.size();
			if (nBeg == strSource.size())
				break;
			nFind = strSource.find(strFlag, nBeg);
			if (nFind == std::wstring::npos)
			{
				paramList.push_back(wstring(strSource.begin() + nBeg, strSource.end()));
				break;
			}
		}
	}
	return true;
}
//URL±àÂë  
//string UrlEncode(const string& strSrc)
//{
//	string strDes;
//	for (size_t i = 0; i < strSrc.size(); ++i)
//	{
//		BYTE ch = (BYTE)strSrc[i];
//		if (isalnum(ch) || ch == '-' || ch == '_' || ch == '.' || ch == '~')
//			strDes += ch;
//		else if (ch == ' ')
//			strDes += '+';
//		else
//		{
//			strDes += '%';
//			strDes += ToHex((ch >> 4));
//			strDes += ToHex(ch % 16);
//		}
//	}
//	return strDes;
//}
//URL½âÂë  
//string UrlDecode(const string& strSrc)
//{
//	string strDes;
//	for (size_t i = 0; i < strSrc.size(); i++)
//	{
//		BYTE ch = strSrc[i];
//		if (ch == '+')
//			strDes += ' ';
//		else if (ch == '%')
//		{
//			BYTE h = FromHex((unsigned char)strSrc[++i]);
//			BYTE l = FromHex((unsigned char)strSrc[++i]);
//			strDes += (h << 4) + l;
//		}
//		else strDes += ch;
//	}
//	return strDes;
//}
//Ìæ»»×Ö·û´®  
wstring StrReplaceW(const wstring& strContent, const wstring& strTag, const wstring& strReplace)
{
	size_t nBegin = 0, nFind = 0;
	nFind = strContent.find(strTag, nBegin);
	if (nFind == wstring::npos)
		return strContent;
	size_t nTagLen = strTag.size();
	wstring strRet;
	while (true)
	{
		strRet.append(strContent.begin() + nBegin, strContent.begin() + nFind);
		strRet.append(strReplace);
		nBegin = nFind + nTagLen;
		nFind = strContent.find(strTag, nBegin);
		if (nFind == wstring::npos)
		{
			strRet.append(strContent.begin() + nBegin, strContent.end());
			break;
		}
	}
	return strRet;
}

string StrReplaceA(const string& strContent, const string& strTag, const string& strReplace)
{
	size_t nBegin = 0, nFind = 0;
	nFind = strContent.find(strTag, nBegin);
	if (nFind == string::npos)
		return strContent;
	size_t nTagLen = strTag.size();
	string strRet;
	while (true)
	{
		strRet.append(strContent.begin() + nBegin, strContent.begin() + nFind);
		strRet.append(strReplace);
		nBegin = nFind + nTagLen;
		nFind = strContent.find(strTag, nBegin);
		if (nFind == string::npos)
		{
			strRet.append(strContent.begin() + nBegin, strContent.end());
			break;
		}
	}
	return strRet;
}

#endif