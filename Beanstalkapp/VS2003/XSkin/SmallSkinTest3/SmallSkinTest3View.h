// SmallSkinTest3View.h : CSmallSkinTest3View 类的接口
//


#pragma once


class CSmallSkinTest3View : public CView
{
protected: // 仅从序列化创建
	CSmallSkinTest3View();
	DECLARE_DYNCREATE(CSmallSkinTest3View)

// 属性
public:
	CSmallSkinTest3Doc* GetDocument() const;

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
	virtual ~CSmallSkinTest3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // SmallSkinTest3View.cpp 的调试版本
inline CSmallSkinTest3Doc* CSmallSkinTest3View::GetDocument() const
   { return reinterpret_cast<CSmallSkinTest3Doc*>(m_pDocument); }
#endif

