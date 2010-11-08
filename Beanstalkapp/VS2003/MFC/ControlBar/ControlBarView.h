// ControlBarView.h : CControlBarView 类的接口
//


#pragma once


class CControlBarView : public CView
{
protected: // 仅从序列化创建
	CControlBarView();
	DECLARE_DYNCREATE(CControlBarView)

// 属性
public:
	CControlBarDoc* GetDocument() const;

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
	virtual ~CControlBarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnClickTest1();
	afx_msg void OnUpdateTest1(CCmdUI *);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ControlBarView.cpp 的调试版本
inline CControlBarDoc* CControlBarView::GetDocument() const
   { return reinterpret_cast<CControlBarDoc*>(m_pDocument); }
#endif

