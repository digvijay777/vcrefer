#pragma once

namespace XRes
{
	// 对话框操作
	// 独一无二的对话框
	int		AloneMsgbox(HWND hWnd, LPCSTR lpText, LPCSTR lpTitle, UINT nType);
	int		TimeoutMsgbox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpTitle, UINT nType, UINT nTimeout = INFINITE);
}