// ControlBarDoc.h :  CControlBarDoc ��Ľӿ�
//


#pragma once

class CControlBarDoc : public CDocument
{
protected: // �������л�����
	CControlBarDoc();
	DECLARE_DYNCREATE(CControlBarDoc)

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
	virtual ~CControlBarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


