// LogToolTestView.h : CLogToolTestView 类的接口
//


#pragma once


class CLogToolTestView : public CView
{
protected: // 仅从序列化创建
	CLogToolTestView();
	DECLARE_DYNCREATE(CLogToolTestView)

// 属性
public:
	CLogToolTestDoc* GetDocument() const;

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
	virtual ~CLogToolTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // LogToolTestView.cpp 的调试版本
inline CLogToolTestDoc* CLogToolTestView::GetDocument() const
   { return reinterpret_cast<CLogToolTestDoc*>(m_pDocument); }
#endif

