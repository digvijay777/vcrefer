#pragma once


// CDetRegView ��ͼ

class CDetRegView : public CListView
{
	DECLARE_DYNCREATE(CDetRegView)

protected:
	CDetRegView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDetRegView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


