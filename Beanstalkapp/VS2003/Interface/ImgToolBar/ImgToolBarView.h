// ImgToolBarView.h : CImgToolBarView ��Ľӿ�
//


#pragma once


class CImgToolBarView : public CView
{
protected: // �������л�����
	CImgToolBarView();
	DECLARE_DYNCREATE(CImgToolBarView)

// ����
public:
	CImgToolBarDoc* GetDocument() const;

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
	virtual ~CImgToolBarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ImgToolBarView.cpp �ĵ��԰汾
inline CImgToolBarDoc* CImgToolBarView::GetDocument() const
   { return reinterpret_cast<CImgToolBarDoc*>(m_pDocument); }
#endif

