// testsdkView.h : CtestsdkView ��Ľӿ�
//


#pragma once


class CtestsdkView : public CView
{
protected: // �������л�����
	CtestsdkView();
	DECLARE_DYNCREATE(CtestsdkView)

// ����
public:
	CtestsdkDoc* GetDocument() const;

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
	virtual ~CtestsdkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // testsdkView.cpp �ĵ��԰汾
inline CtestsdkDoc* CtestsdkView::GetDocument() const
   { return reinterpret_cast<CtestsdkDoc*>(m_pDocument); }
#endif

