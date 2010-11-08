// StudyGDI+View.h : CStudyGDIView 类的接口
//


#pragma once


class CStudyGDIView : public CView
{
protected: // 仅从序列化创建
	CStudyGDIView();
	DECLARE_DYNCREATE(CStudyGDIView)

// 属性
public:
	CStudyGDIDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CStudyGDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ULONG_PTR		m_pGdiToken;
// 生成的消息映射函数
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

#ifndef _DEBUG  // StudyGDI+View.cpp 的调试版本
inline CStudyGDIDoc* CStudyGDIView::GetDocument() const
   { return reinterpret_cast<CStudyGDIDoc*>(m_pDocument); }
#endif

