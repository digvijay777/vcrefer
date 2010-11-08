// StudyGDI+View.h : CStudyGDIView ��Ľӿ�
//


#pragma once


class CStudyGDIView : public CView
{
protected: // �������л�����
	CStudyGDIView();
	DECLARE_DYNCREATE(CStudyGDIView)

// ����
public:
	CStudyGDIDoc* GetDocument() const;

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
	virtual ~CStudyGDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ULONG_PTR		m_pGdiToken;
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawline();
	afx_msg void OnShowtext();
	afx_msg void OnDrawrang();
	afx_msg void OnDrawcurve();
	afx_msg void OnShowimage();
	afx_msg void OnPenCreate();
	afx_msg void OnPenRotate();
	afx_msg void OnPenLiencap();
	afx_msg void OnPenLinepoint();
	afx_msg void OnViewFresh();
	afx_msg void OnPenAlpha();
	afx_msg void OnBrushSolid();
	afx_msg void OnBrushHatch();
	afx_msg void OnBrushOrigin();
	afx_msg void OnBrushTexture();
	afx_msg void OnBrushGradient();
	afx_msg void OnFontFamily();
	afx_msg void OnFontBorder();
};

#ifndef _DEBUG  // StudyGDI+View.cpp �ĵ��԰汾
inline CStudyGDIDoc* CStudyGDIView::GetDocument() const
   { return reinterpret_cast<CStudyGDIDoc*>(m_pDocument); }
#endif

