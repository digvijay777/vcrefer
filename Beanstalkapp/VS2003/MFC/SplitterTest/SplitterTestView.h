// SplitterTestView.h : CSplitterTestView ��Ľӿ�
//


#pragma once


class CSplitterTestView : public CView
{
protected: // �������л�����
	CSplitterTestView();
	DECLARE_DYNCREATE(CSplitterTestView)

// ����
public:
	CSplitterTestDoc* GetDocument() const;

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
	virtual ~CSplitterTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SplitterTestView.cpp �ĵ��԰汾
inline CSplitterTestDoc* CSplitterTestView::GetDocument() const
   { return reinterpret_cast<CSplitterTestDoc*>(m_pDocument); }
#endif

