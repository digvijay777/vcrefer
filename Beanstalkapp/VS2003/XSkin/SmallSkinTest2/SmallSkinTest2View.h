// SmallSkinTest2View.h : CSmallSkinTest2View ��Ľӿ�
//


#pragma once


class CSmallSkinTest2View : public CView
{
protected: // �������л�����
	CSmallSkinTest2View();
	DECLARE_DYNCREATE(CSmallSkinTest2View)

// ����
public:
	CSmallSkinTest2Doc* GetDocument() const;

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
	virtual ~CSmallSkinTest2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SmallSkinTest2View.cpp �ĵ��԰汾
inline CSmallSkinTest2Doc* CSmallSkinTest2View::GetDocument() const
   { return reinterpret_cast<CSmallSkinTest2Doc*>(m_pDocument); }
#endif

