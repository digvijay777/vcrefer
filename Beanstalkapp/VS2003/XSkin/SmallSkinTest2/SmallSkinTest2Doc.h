// SmallSkinTest2Doc.h :  CSmallSkinTest2Doc ��Ľӿ�
//


#pragma once

class CSmallSkinTest2Doc : public CDocument
{
protected: // �������л�����
	CSmallSkinTest2Doc();
	DECLARE_DYNCREATE(CSmallSkinTest2Doc)

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
	virtual ~CSmallSkinTest2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


