// SmallSkinTest3View.h : CSmallSkinTest3View ��Ľӿ�
//


#pragma once


class CSmallSkinTest3View : public CView
{
protected: // �������л�����
	CSmallSkinTest3View();
	DECLARE_DYNCREATE(CSmallSkinTest3View)

// ����
public:
	CSmallSkinTest3Doc* GetDocument() const;

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
	virtual ~CSmallSkinTest3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SmallSkinTest3View.cpp �ĵ��԰汾
inline CSmallSkinTest3Doc* CSmallSkinTest3View::GetDocument() const
   { return reinterpret_cast<CSmallSkinTest3Doc*>(m_pDocument); }
#endif

