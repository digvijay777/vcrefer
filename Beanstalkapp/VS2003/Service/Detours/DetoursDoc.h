// DetoursDoc.h :  CDetoursDoc ��Ľӿ�
//


#pragma once

class CDetoursDoc : public CDocument
{
protected: // �������л�����
	CDetoursDoc();
	DECLARE_DYNCREATE(CDetoursDoc)

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
	virtual ~CDetoursDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


