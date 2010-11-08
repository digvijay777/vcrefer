// Detours.h : Detours 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h” 
#endif

#include "resource.h"       // 主符号
//////////////////////////////////////////////////////////////////////////
// 导入外部库
#include "../../../common/mfc/TabToolBar.h"
#include "../../../common/Detours/detours.h"

//////////////////////////////////////////////////////////////////////////
// 申名全局变量
extern class CDetoursApp theApp;


//////////////////////////////////////////////////////////////////////////
// 自定义消息宏
#define UM_FRAME			(WM_USER+100)
#define UM_DOCUMENT			(WM_USER+200)
#define UM_VIEW				(WM_USER+300)

#define UM_FRAME_UPDATETABLE		UM_FRAME+1

#define UM_VIEW_ADDITEM				UM_VIEW+1
//////////////////////////////////////////////////////////////////////////
// 操作宏
#define COMPAREINCLUDE(mask, in)	(((mask) & (in)) == (in))

//////////////////////////////////////////////////////////////////////////
// APP类
class CDetoursApp : public CWinApp
{
public:
	CDetoursApp();


	// 重写
public:
	virtual BOOL InitInstance();

	// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};