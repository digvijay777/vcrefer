// FontTestView.h : CFontTestView ��Ľӿ�
//


#pragma once


class CFontTestView : public CView
{
protected: // �������л�����
	CFontTestView();
	DECLARE_DYNCREATE(CFontTestView)

// ����
public:
	CFontTestDoc* GetDocument() const;

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
	virtual ~CFontTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // FontTestView.cpp �еĵ��԰汾
inline CFontTestDoc* CFontTestView::GetDocument() const
   { return reinterpret_cast<CFontTestDoc*>(m_pDocument); }
#endif

