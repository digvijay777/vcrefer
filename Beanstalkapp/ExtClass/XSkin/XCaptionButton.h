#pragma once
#include <vector>

class CXCaptionButton
{
public:
	CXCaptionButton();
	~CXCaptionButton();
private:
	CBitmap		m_btImg;
	CRect		m_rect;		// 按钮图片以四张并排
	UINT		m_uID;		// 控件ID
public:
	BOOL	CreateSysButton(UINT uiBk, POINT pt, UINT ID);
	void	DrawButton();

public:
	static std::vector<CXCaptionButton *>		s_vctXCaptionButton;
public:
	static BOOL	XCaptionButtonWndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plRet);
};

#define CallXCaptionButtonProc(message, wParam, lParam) \
	LRESULT __result_XCaptionButton;\
	if(TRUE == CXSkinWnd::SkinWindowProc(msg, wParam, lParam, &__result_XCaptionButton))\
	{\
		return __result_XCaptionButton;\
	}\

