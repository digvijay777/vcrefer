// ControlBarView.h : CControlBarView ��Ľӿ�
//


#pragma once


class CControlBarView : public CView
{
protected: // �������л�����
	CControlBarView();
	DECLARE_DYNCREATE(CControlBarView)

// ����
public:
	CControlBarDoc* GetDocument() const;

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
	virtual ~CControlBarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnClickTest1();
	afx_msg void OnUpdateTest1(CCmdUI *);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ControlBarView.cpp �ĵ��԰汾
inline CControlBarDoc* CControlBarView::GetDocument() const
   { return reinterpret_cast<CControlBarDoc*>(m_pDocument); }
#endif

