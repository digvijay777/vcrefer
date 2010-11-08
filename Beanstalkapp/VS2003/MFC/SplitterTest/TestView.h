#pragma once



// CTestView form view

class CTestView : public CFormView
{
	DECLARE_DYNCREATE(CTestView)

protected:
	CTestView();           // protected constructor used by dynamic creation
	virtual ~CTestView();

public:
	enum { IDD = IDD_TESTVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


