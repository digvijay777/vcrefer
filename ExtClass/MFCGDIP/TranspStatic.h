/*
 *	Í¸Ã÷StaticÀà
 */
#pragma once
#include "MfcGdipAfx.h"

class CTranspStatic : public CStatic
{
	DECLARE_DYNAMIC(CTranspStatic)

public:
	CTranspStatic();
	virtual ~CTranspStatic();

private:
	CBrush			m_brushBkgnd;
	COLORREF		m_colTextColor;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);

public:
	void			SetTextColor(COLORREF col);
};


