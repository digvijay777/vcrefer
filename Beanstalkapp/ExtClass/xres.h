#pragma once

namespace XRes
{
	// �Ի������
	// ��һ�޶��ĶԻ���
	int		AloneMsgbox(HWND hWnd, LPCSTR lpText, LPCSTR lpTitle, UINT nType);
	int		TimeoutMsgbox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpTitle, UINT nType, UINT nTimeout = INFINITE);
}