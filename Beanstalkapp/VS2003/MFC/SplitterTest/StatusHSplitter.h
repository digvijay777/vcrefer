#pragma once


// CStatusHSplitter

class CStatusHSplitter : public CWnd
{
	DECLARE_DYNAMIC(CStatusHSplitter)

public:
	CStatusHSplitter();
	virtual ~CStatusHSplitter();

protected:
	DECLARE_MESSAGE_MAP()

public:
	int			m_nStatusHeight;		// ״̬��Ĭ�ϸ߶�
public:
	virtual BOOL Create(CWnd* pParent, int nRows, ...);
};


