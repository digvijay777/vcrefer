// SmallSkinTest3Doc.h :  CSmallSkinTest3Doc ��Ľӿ�
//


#pragma once

class CSmallSkinTest3Doc : public CDocument
{
protected: // �������л�����
	CSmallSkinTest3Doc();
	DECLARE_DYNCREATE(CSmallSkinTest3Doc)

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
	virtual ~CSmallSkinTest3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


