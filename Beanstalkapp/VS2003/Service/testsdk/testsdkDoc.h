// testsdkDoc.h :  CtestsdkDoc ��Ľӿ�
//


#pragma once

class CtestsdkDoc : public CDocument
{
protected: // �������л�����
	CtestsdkDoc();
	DECLARE_DYNCREATE(CtestsdkDoc)

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
	virtual ~CtestsdkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


