// EHomeListsDoc.h : CEHomeListsDoc ��Ľӿ�
//


#pragma once


class CEHomeListsDoc : public CDocument
{
protected: // �������л�����
	CEHomeListsDoc();
	DECLARE_DYNCREATE(CEHomeListsDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CEHomeListsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


