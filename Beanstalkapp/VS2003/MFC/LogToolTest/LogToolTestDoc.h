// LogToolTestDoc.h :  CLogToolTestDoc ��Ľӿ�
//


#pragma once

class CLogToolTestDoc : public CDocument
{
protected: // �������л�����
	CLogToolTestDoc();
	DECLARE_DYNCREATE(CLogToolTestDoc)

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
	virtual ~CLogToolTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


