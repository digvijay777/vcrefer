// BatchLiteEmailView.h : CBatchLiteEmailView 类的接口
//


#pragma once


class CBatchLiteEmailView : public CHtmlView
{
protected: // 仅从序列化创建
	CBatchLiteEmailView();
	DECLARE_DYNCREATE(CBatchLiteEmailView)

// 属性
public:
	CBatchLiteEmailDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CBatchLiteEmailView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
//	afx_msg void OnSendmail();
	afx_msg void OnSendmail();
};

#ifndef _DEBUG  // BatchLiteEmailView.cpp 的调试版本
inline CBatchLiteEmailDoc* CBatchLiteEmailView::GetDocument() const
   { return reinterpret_cast<CBatchLiteEmailDoc*>(m_pDocument); }
#endif

