// LicenseManagerView.h : CLicenseManagerView 类的接口
//


#pragma once

class CLicenseManagerView : public CView
{
protected: // 仅从序列化创建
	CLicenseManagerView();
	DECLARE_DYNCREATE(CLicenseManagerView)

// 属性
public:
	CLicenseManagerDoc* GetDocument() const;

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
	virtual ~CLicenseManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl		m_wndTabCtrl;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // LicenseManagerView.cpp 的调试版本
inline CLicense2Doc* CLicenseManagerView::GetDocument() const
   { return reinterpret_cast<CLicense2Doc*>(m_pDocument); }
#endif

