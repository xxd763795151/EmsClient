// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#ifndef _DEBUG
#define _AFXDLL
#endif

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
#define _CRT_SECURE_NO_WARNINGS			//取消VC的安全警告
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define LOGIN_WIN_CLASS _T("LoginWnd")
#define EMS_MAIN_WND _T("EmsMainWnd")

//自定义消息
#define WM_SHOW_SUBJECT_QUESTION WM_USER+1000
#define WM_IMPORT_RESPONSE WM_USER+1001
#define WM_SCAN_QUESTION WM_USER+1002
#define WM_ADD_EXAM_RET   WM_USER+1003
#define WM_EXAM_DESCRIPTION WM_USER+1004
#define WM_APPLY_MSG  WM_USER+1005
#define WM_USER_ANSWER_MSG WM_USER+1006
#define WM_ORIGIN_ANSWER_MSG WM_USER+1007
#define WM_GRADE_SHOW_MSG WM_USER+1008
#define WM_LOGIN_MSG WM_USER+1009
// Windows 头文件: 
//#include <windows.h>
//注释掉windows.h，配置以下使用MFC类库
#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers 
#include <afxwin.h> // MFC core and standard components 
#include <afxext.h> // MFC extensions 
#include <afxdisp.h> // MFC Automation classes 
#include <afxdtctl.h> // MFC support for Internet Explorer 4 Common Controls 
#ifndef _AFX_NO_AFXCMN_SUPPORT 
#include <afxcmn.h> // MFC support for Windows Common Controls 
#endif // _AFX_NO_AFXCMN_SUPPORT 
//MFC类库配置结束

//检测内存泄露
#include "ExcludeGdiPlus.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#ifdef _DEBUG
//#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#else
//#define DEBUG_CLIENTBLOCK
//#endif
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#ifdef _DEBUG
//#define new DEBUG_CLIENTBLOCK
//#endif

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <map>
#include <string>
#include <vector>
#include <list>
#include "UILib.h"
using namespace DuiLib;
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

// TODO:  在此处引用程序需要的其他头文件
