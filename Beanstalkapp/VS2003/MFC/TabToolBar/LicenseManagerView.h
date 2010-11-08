// LicenseManagerView.h : CLicenseManagerView ��Ľӿ�
//


#pragma once

class CLicenseManagerView : public CView
{
protected: // �������л�����
	CLicenseManagerView();
	DECLARE_DYNCREATE(CLicenseManagerView)

// ����
public:
	CLicenseManagerDoc* GetDocument() const;

// ����
public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CLicenseManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl		m_wndTabCtrl;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // LicenseManagerView.cpp �ĵ��԰汾
inline CLicense2Doc* CLicenseManagerView::GetDocument() const
   { return reinterpret_cast<CLicense2Doc*>(m_pDocument); }
#endif

