#pragma once

typedef <class T>
class TMemGraphic
{
public:
	TMemGraphic(HDC hDC)
		:m_graphics()
	{
		
	}
private:
	Gdiplus::Graphics		m_graphics;
};