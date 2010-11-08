// DetoursView.h : CDetoursView 类的接口
//


#pragma once


class CDetoursView : public CHtmlView
{
protected: // 仅从序列化创建
	CDetoursView();
	DECLARE_DYNCREATE(CDetoursView)

// 属性
public:
	CDetoursDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CDetoursView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DetoursView.cpp 的调试版本
inline CDetoursDoc* CDetoursView::GetDocument() const
   { return reinterpret_cast<CDetoursDoc*>(m_pDocument); }
#endif

