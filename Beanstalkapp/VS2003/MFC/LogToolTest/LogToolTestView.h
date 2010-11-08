// LogToolTestView.h : CLogToolTestView ��Ľӿ�
//


#pragma once


class CLogToolTestView : public CView
{
protected: // �������л�����
	CLogToolTestView();
	DECLARE_DYNCREATE(CLogToolTestView)

// ����
public:
	CLogToolTestDoc* GetDocument() const;

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
	virtual ~CLogToolTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // LogToolTestView.cpp �ĵ��԰汾
inline CLogToolTestDoc* CLogToolTestView::GetDocument() const
   { return reinterpret_cast<CLogToolTestDoc*>(m_pDocument); }
#endif

