#pragma once


// CDetRegView 视图

class CDetRegView : public CListView
{
	DECLARE_DYNCREATE(CDetRegView)

protected:
	CDetRegView();           // 动态创建所使用的受保护的构造函数
	virtual ~CDetRegView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


