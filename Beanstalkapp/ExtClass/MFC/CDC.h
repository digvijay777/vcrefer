#pragma once


template <class T>
class TSelectObject : public T
{
private:
	CDC*		m_pHostDC;
	T*			m_pOldObject;
public:
	TSelectObject(CDC* pDC)
	{
		m_pOldObject = NULL;
		m_pHostDC = pDC;
	}
	
	~TSelectObject()
	{
	}

public:
	void SelectToDC()
	{
		if(NULL == m_pOldObject)
			m_pOldObject = m_pHostDC->SelectObject(this);
		else
			m_pHostDC->SelectObject(this);
	}

	void SelectOutDC()
	{
		if(NULL != m_pOldObject)
			m_pHostDC->SelectObject(m_pOldObject);

		m_pOldObject = NULL;
	}
};