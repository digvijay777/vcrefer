// StudyGDI+View.cpp : CStudyGDIView ���ʵ��
//

#include "stdafx.h"
#include "StudyGDI.h"

#include "StudyGDIDoc.h"
#include "StudyGDIView.h"
#include ".\studygdiview.h"


// math
#include <math.h>
// GDI+
#include <gdiplus.h>
#include <Gdiplusinit.h>
#include <Gdiplusgraphics.h>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudyGDIView

IMPLEMENT_DYNCREATE(CStudyGDIView, CView)

BEGIN_MESSAGE_MAP(CStudyGDIView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(IDM_DRAWLINE, OnDrawline)
	ON_COMMAND(IDM_SHOWTEXT, OnShowtext)
	ON_COMMAND(IDM_DRAWRANG, OnDrawrang)
	ON_COMMAND(IDM_DRAWCURVE, OnDrawcurve)
	ON_COMMAND(IDM_SHOWIMAGE, OnShowimage)
	ON_COMMAND(IDM_PEN_CREATE, OnPenCreate)
	ON_COMMAND(IDM_PEN_ROTATE, OnPenRotate)
	ON_COMMAND(IDM_PEN_LIENCAP, OnPenLiencap)
	ON_COMMAND(IDM_PEN_LINEPOINT, OnPenLinepoint)
	ON_COMMAND(IDM_VIEW_FRESH, OnViewFresh)
	ON_COMMAND(IDM_PEN_ALPHA, OnPenAlpha)
	ON_COMMAND(IDM_BRUSH_SOLID, OnBrushSolid)
	ON_COMMAND(IDM_BRUSH_HATCH, OnBrushHatch)
	ON_COMMAND(IDM_BRUSH_ORIGIN, OnBrushOrigin)
	ON_COMMAND(IDM_BRUSH_TEXTURE, OnBrushTexture)
	ON_COMMAND(IDM_BRUSH_GRADIENT, OnBrushGradient)
	ON_COMMAND(IDM_FONT_FAMILY, OnFontFamily)
	ON_COMMAND(IDM_FONT_BORDER, OnFontBorder)
END_MESSAGE_MAP()

// CStudyGDIView ����/����

CStudyGDIView::CStudyGDIView()
{
	// TODO: �ڴ˴���ӹ������
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &gdiplusStartupInput, NULL);
}

CStudyGDIView::~CStudyGDIView()
{
	GdiplusShutdown(m_pGdiToken);
}

BOOL CStudyGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ������
	// ��ʽ

	return CView::PreCreateWindow(cs);
}

// CStudyGDIView ����

void CStudyGDIView::OnDraw(CDC* /*pDC*/)
{
	CStudyGDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CStudyGDIView ��ӡ

BOOL CStudyGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CStudyGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡǰ��Ӷ���ĳ�ʼ��
}

void CStudyGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��ӡ������������
}


// CStudyGDIView ���

#ifdef _DEBUG
void CStudyGDIView::AssertValid() const
{
	CView::AssertValid();
}

void CStudyGDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStudyGDIDoc* CStudyGDIView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStudyGDIDoc)));
	return (CStudyGDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CStudyGDIView ��Ϣ�������

// ����
void CStudyGDIView::OnDrawline()
{
	CDC*		pDC								= GetDC();
	Graphics	graphics(pDC->m_hDC);

	//////////////////////////////////////////////////////////////////////////
	Pen			red(Color(255, 255, 0, 0), 3);
	Pen			blue(Color(100, 0, 0, 255), 50);

	graphics.DrawLine(&red, 50, 100, 700, 100);
	graphics.DrawLine(&blue, 10, 150, 700, 150);

	//////////////////////////////////////////////////////////////////////////
	Pen			blackPen(Color(255, 0, 0, 0), 3);
	PointF		point1(10.0f, 10.0f);
	PointF		point2(10.0f, 100.0f);
	PointF		point3(50.0f, 50.0f);
	PointF		point4(10.0f, 10.0f);
	PointF		point[4] = {point1, point2, point3, point4};
	PointF*		pPoint	= point;

	graphics.DrawLines(&blackPen, pPoint, 4);

}

// ��ʾ�ı�
void CStudyGDIView::OnShowtext()
{
	CDC*		pDC								= GetDC();
	Graphics	graphics(pDC->m_hDC);

	//////////////////////////////////////////////////////////////////////////
	Pen			pen(Color(255, 0, 0, 255));
	SolidBrush	brush(Color(255, 0, 0, 255));
	FontFamily	fontFamily(L"����");
	Font		font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	CRect		rect;

	GetClientRect(&rect);
	PointF		pointF(rect.right / 2, rect.bottom / 2);
	graphics.DrawString(L"GDI+����ʾ��", -1, &font, pointF, &brush);

	//////////////////////////////////////////////////////////////////////////
	WCHAR		string[256];
	wcscpy(string, L"Hello GDI+");
	Font		myFont(L"Arial", 26);
	RectF		layoutRect(10.0f, 10.0f, 200.0f, 50.0f);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	SolidBrush	blackBrush(Color(255, 0, 0, 0));
	graphics.DrawString(string,
		wcslen(string),
		&myFont,
		layoutRect,
		&format,
		&blackBrush);
	graphics.DrawRectangle(&Pen(Color::Green, 3), layoutRect);
}

// ���ƾ���
void CStudyGDIView::OnDrawrang()
{
	CDC*		pDC								= GetDC();
	Graphics	graphics(pDC->m_hDC);
	Pen			blackPen(Color(255, 0, 0, 0), 3);
	RectF		rect1(10.0f, 10.0f, 100.0f, 50.0f);
	RectF		rect2(40.0f, 40.0f, 100.0f, 50.0f);
	RectF		rect3(80.0f, 4.0f, 50.0f, 100.0f);
	RectF		rects[] = {rect1, rect2, rect3};
	RectF*		pRects = rects;

	graphics.DrawRectangles(&blackPen, pRects, sizeof(rects) / sizeof(RectF));
}

// ���Ʊ���������
void CStudyGDIView::OnDrawcurve()
{
	CDC*		pDC								= GetDC();
	Graphics	graphics(pDC->m_hDC);
	Pen			greenPen(Color::Green, 3);
	Pen			redPen(Color::Red, 3);
	Point		point1(100, 100);
	Point		point2(200, 50);
	Point		point3(700, 10);
	Point		point7(500, 100);
	Point		curvePoints[7] = {
		point1,
		point2,
		point3,
		point7};

	// ��������
	graphics.DrawCurve(&greenPen, curvePoints, 7);
	// ʹ�ú�ɫ���ʻ�������ǿ��Ϊ1.3������
	graphics.DrawCurve(&redPen, curvePoints, 7, 1.3f);
	// �������ߵĶ����
	SolidBrush	redBrush(Color::Red);
	graphics.FillEllipse(&redBrush, Rect(95, 95, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(195, 75, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(395, 5, 10, 10));
	graphics.FillEllipse(&redBrush, Rect(795, 95, 10, 10));
}

// ��ʾͼƬ
void CStudyGDIView::OnShowimage()
{
	CDC*		pDC								= GetDC();
	Graphics	graphics(pDC->m_hDC);
	Image		image(L"x:\\downlog\\11.jpg");
	graphics.DrawImage(&image, 100, 100, image.GetWidth(), image.GetHeight());

}

// ��������
void CStudyGDIView::OnPenCreate()
{
	Graphics	graphics(GetDC()->m_hDC);

	//////////////////////////////////////////////////////////////////////////
	Image		image(L"X:\\downlog\\1.jpg");
	TextureBrush tBrush(&image);
	Pen			texturedPen(&tBrush, 30);

	graphics.DrawEllipse(&texturedPen, 300, 20, 500, 300);
	//////////////////////////////////////////////////////////////////////////
	Pen			pen(Color(255, 0, 0, 255), 5);
	for(int j = 0; j < 5; j++)
	{
		pen.SetDashStyle((DashStyle)j);
		graphics.DrawLine(&pen, 10, 30 * j + 20, 300, 30 * j + 20);
	}

	REAL		dashVals[4] = {
		5.0f, // �߳�5������
		2.0f, // ���2������
		15.0f, // �߳�15������
		4.0f // ���4������
	};

	pen.SetDashPattern(dashVals, 4);
	pen.SetColor(Color::Red);
	graphics.DrawLine(&pen, 10, 30*j+40, 300, 30*j+40);

}

// ������ת
void CStudyGDIView::OnPenRotate()
{
	Graphics	graphics(GetDC()->m_hDC);
	Pen			pen2(Color::Blue, 5);
	// ��ֱ������6����ˮƽ�����ϲ���
	pen2.ScaleTransform(1, 6); 
	graphics.DrawEllipse(&pen2, 50, 350, 200, 200);
	// ������������ת60�ȣ�120�ȣ�180��
	pen2.RotateTransform(60, MatrixOrderAppend);
	graphics.DrawEllipse(&pen2, 50, 50, 200, 200);
	pen2.RotateTransform(60, MatrixOrderAppend);
	graphics.DrawEllipse(&pen2, 300, 50, 200, 200);
	pen2.RotateTransform(60, MatrixOrderAppend);
	graphics.DrawEllipse(&pen2, 550, 50, 200, 200);	
}

// ��ð
void CStudyGDIView::OnPenLiencap()
{
	Graphics	graphics(GetDC()->m_hDC);
	GraphicsPath startPath, EndPath;
	startPath.AddRectangle(Rect(-10, -5, 20, 10));
	EndPath.AddLine(0, -20, 10, 0);
	EndPath.AddLine(0, -20, -10, 0);
	EndPath.AddLine(0, -10, 10, 0);
	EndPath.AddLine(0, -10, -10, 0);

	Rect rect(10, -5, 20, 10);
	Pen pen(Color(255, 0, 0, 255), 2);
	CustomLineCap custCap(NULL, &startPath);
	CustomLineCap endCap(NULL, &EndPath);
	pen.SetCustomStartCap(&custCap);
	pen.SetCustomEndCap(&endCap);
	graphics.DrawLine(&pen, 20, 30, 300, 30);
	for(float f = 0.0f; f < 6.28f; f += 0.15f)
	{
		graphics.DrawLine(&pen, 300.0f, 300.0f, 300+200.0f*cos(f), 300+200.0f*sin(f));
	}
}

// ����=>���ӵ�
void CStudyGDIView::OnPenLinepoint()
{
	Graphics	graphics(GetDC()->m_hDC);
	Pen pen(Color(255, 0, 0, 255), 25);
	
	// ���ӷ�ʽΪԲ��
	pen.SetLineJoin(LineJoinRound);
	graphics.DrawRectangle(&pen, 20, 20, 150, 100);
	// ���ӷ�ʽΪб��
	pen.SetLineJoin(LineJoinBevel);
	graphics.DrawRectangle(&pen, 300, 20, 150, 100);
	// ����4�ֲ�ͬ��ֱ�����ӵ�
	Point pt[3] = {
		Point(20, 220),
		Point(100, 200),
		Point(50, 280)
	};
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	// б��
	pen.SetLineJoin(LineJoinMiter);
	graphics.DrawLines(&pen, pt, 3);
	pt[0].X += 100;
	pt[1].X += 100;
	pt[2].X += 100;
	// б��
	pen.SetLineJoin(LineJoinBevel);
	graphics.DrawLines(&pen, pt, 3);
	pt[0].X += 100;
	pt[1].X += 100;
	pt[2].X += 100;
	// Բ��
	pen.SetLineJoin(LineJoinRound);
	graphics.DrawLines(&pen, pt, 3);
	pt[0].X += 100;
	pt[1].X += 100;
	pt[2].X += 100;
	// ����б��
	pen.SetLineJoin(LineJoinMiterClipped);
	graphics.DrawLines(&pen, pt, 3);
}

// ˢ��
void CStudyGDIView::OnViewFresh()
{
	Invalidate();
}

// ͸������
void CStudyGDIView::OnPenAlpha()
{
	Graphics	graphics(GetDC()->m_hDC);
	Pen			blue(Color(255, 0, 0, 255));
	Pen			red(Color(255, 255, 0, 0));
	// ��������
	int y = 256;
	for(int x = 0; x < 256; x+=5)
	{
		graphics.DrawLine(&blue, 0, y, x, 0);
		graphics.DrawLine(&red, 256, x, y, 256);
		y -= 5;
		Sleep(120);
	}
	for(y = 0; y < 256; y++)
	{
		Pen pen(Color(y, 0, 255, 0));
		graphics.DrawLine(&pen, 0, y, 256, y);
		Sleep(20);
	}
	for(x = 0; x < 256; x++)
	{
		Pen pen(Color(x, 255, 0, 255));
		graphics.DrawLine(&pen, x, 100, x, 200);
		Sleep(20);
	}
}

// ��ɫ��ˢ 
void CStudyGDIView::OnBrushSolid()
{
	Graphics	graphics(GetDC()->m_hDC);
	SolidBrush	greenBrush(Color(255, 0, 255, 0));
	int cx, cy;
	CRect rect;
	GetClientRect(rect);
	// �Ե�ǰ���ڵ����ĵ������Ҷ����
	cx = rect.Width() / 2;
	cy = rect.Height() / 2;
	// ���á�Ҷ����
	int LeafLenght = 100;
	// ����ҶƬ���� = 2* LeafNum
	int LeafNum = 5;
	float PI=3.14f;
	int x2, y2;
	int x,y,r;
	// ����һ��ͼ��·����������������Ҷ���ߵı߽���
	GraphicsPath tmpPath(FillModeAlternate);
	Pen pen(Color::Gray, 1);
	Pen pen1(Color::Green);
	graphics.DrawLine(&pen, 0, cy, cx*2, cy);
	graphics.DrawLine(&pen, cx, 0, cx, cy*2);
	// �������߽߱�����
	for(float i = 0.0f; i < PI*2+0.1f; i += PI/180)
	{
		r = abs(LeafLenght * cos(LeafNum * i));
		x = r*cos(i);
		y = r*sin(i);
		x2 = cx+x;
		y2 = cy+y;
		graphics.DrawLine(&pen1, x2, y2, x2-1, y2-1);
		tmpPath.AddLine(x2, y2, x2, y2);
		Sleep(20);
	}
	graphics.FillPath(&greenBrush, &tmpPath);
}

// Ӱ�߻�ˢ
void CStudyGDIView::OnBrushHatch()
{
	Graphics		graphics(GetDC()->m_hDC);
	Color			black(255, 0, 0, 0);
	Color			white(255, 255, 255, 255);
	SolidBrush		redBrush(Color::Red);
	Pen				pen(Color::Green, 1);
	CRect			rect;
	GetClientRect(&rect);
	int				width = rect.Width();
	int				column_count = (int)width / 40;
	int				rol = 0;
	int				column = 0;
	CStringW		string;
	WCHAR			wideChar[2];
	Font			myFont(L"Arial", 16);

	// �ڵ�ǰ����ʹ��53�ַ���Ӱ�߻�ˢ���
	for(int i = 0; i < 53; i++)
	{
		// ���һ���ѻ�����ɣ�����
		if(rol > column_count - 1)
		{
			column += 1;
			rol = 0;
		}

		HatchBrush brush_tmp(HatchStyle(i), black, white);
		graphics.FillRectangle(&brush_tmp, rol*40, column * 40, 35, 35);
		graphics.DrawRectangle(&pen, rol*40, column*40, 35, 35);

		string.Format(L"%d", i);
		RectF layoutRect(rol*40, column*40, 35, 35);
		StringFormat format;
		format.SetLineAlignment(StringAlignmentCenter);
		format.SetAlignment(StringAlignmentCenter);
		graphics.DrawString(string.GetBuffer(),
			string.GetLength(), &myFont, layoutRect, &format, &redBrush);
		rol += 1;
	}
}

// ���û�ˢԭ��
void CStudyGDIView::OnBrushOrigin()
{
	Graphics	graphics(GetDC()->m_hDC);
	Color		black(255, 0, 0, 0);
	Color		white(255, 255, 255, 255);
	HatchBrush	hatchBrush(HatchStyle(20), Color::Black, Color::White);

	// ��Ĭ��ԭ�㻭�˸�����
	for(int i = 0; i < 8; i++)
	{
		graphics.FillRectangle(&hatchBrush, 0, i*50, 100, 50);
	}
	// �ò�ͬ��ԭ�㻭����
	for(int i = 0; i < 8; i++)
	{
		graphics.SetRenderingOrigin(i, 0);
		graphics.FillRectangle(&hatchBrush, 200, i*50, 100, 50);
	}
}

// ����ˢ
void CStudyGDIView::OnBrushTexture()
{
	Graphics		graphics(GetDC()->m_hDC);
	Pen				pen(Color::Red, 2);
	SolidBrush		brush(Color::Black);
	Font			myFont(L"����", 20);
	RectF			rect1(10, 10, 200, 200);
	RectF			rect2(210, 10, 200, 200);
	RectF			rect3(410, 10, 200, 200);
	Image			image(L"X:\\downlog\\11.jpg");

	// Ĭ�Ϸ�ʽ
	TextureBrush	tBrush(&image);
	graphics.FillEllipse(&tBrush, rect1);
	graphics.DrawEllipse(&pen, rect1);
	graphics.DrawString(L"ͼƬԭʼ��С", 6, &myFont, PointF(40, 220), NULL, &brush);

	// ֻʹ�û涨ͼƬ�Ĳ���
	TextureBrush	tBrush2(&image, Rect(125, 70, 80, 100));
	graphics.FillEllipse(&tBrush2, rect2);
	graphics.DrawEllipse(&pen, rect2);
	graphics.DrawString(L"ʹ�ò��ֽ�ͼ", 6, &myFont, PointF(240, 220), NULL, &brush);

	// ����
	TextureBrush	tBrush3(&image);
	tBrush3.SetTransform(&Matrix(0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f));
	graphics.FillEllipse(&tBrush3, rect3);
	graphics.DrawEllipse(&pen, rect3);
	graphics.DrawString(L"������СͼƬ", 6, &myFont, PointF(440, 220), NULL, &brush);
}

// ���Խ��仭ˢ
void CStudyGDIView::OnBrushGradient()
{
	Graphics		graphics(GetDC()->m_hDC);

	// ��ֱ���򲻽���ɫ�ʽ���
	LinearGradientBrush linGrBrush(
		Point(0, 0),
		Point(40, 0),
		Color::Red,
		Color::Blue);
	// �Խ��߱任
	LinearGradientBrush linGrBrush2(
		Point(0, 0),
		Point(40, 40),
		Color::Red,
		Color::Blue);

	graphics.FillRectangle(&linGrBrush, 0, 0, 200, 200);
	graphics.FillRectangle(&linGrBrush2, 240, 0, 200, 200);

	Pen pen(Color::Gray, 1);
	for(int i = 0; i < 5; i++)
	{
		graphics.DrawRectangle(&pen, 
			240+i*40, i*40, 40, 40);
	}
}
// ö������
void CStudyGDIView::OnFontFamily()
{
	Graphics		graphics(GetDC()->m_hDC);
	SolidBrush		brush(Color(255, 0, 0, 0));

	FontFamily		fontFamily(L"Arial");
	Font			font(&fontFamily, 12, FontStyleRegular, UnitPixel);

	int				count	= 0;
	int				found	= 0;

	WCHAR			familyName[50];
	WCHAR*			familyList			= NULL;
	FontFamily		pFontFamily[500];
	CRect			rect;
	
	GetClientRect(&rect);
	RectF			r(0, 0, rect.Width(), rect.Height());
	InstalledFontCollection		install;

	count = install.GetFamilyCount();
	install.GetFamilies(count, pFontFamily, &found);
	familyList = new WCHAR[count * sizeof(familyName)];
	wcscpy(familyList, L"");
	for(int j = 0; j < count; j++)
	{
		pFontFamily[j].GetFamilyName(familyName);
		wcscat(familyList, familyName);
		wcscat(familyList, L", ");
	}
	graphics.DrawString(familyList, -1, &font, r, NULL, &brush);
	delete [] familyList;
}
// ��Ե����
void CStudyGDIView::OnFontBorder()
{
	Graphics		graphics(GetDC()->m_hDC);

	FontFamily		fontFamily(L"Arial");
	Font			font(&fontFamily, 60, FontStyleRegular, UnitPixel);

	int RenderingHint[] =
	{
		TextRenderingHintSystemDefault, // ��ϵͳ��ͬ
		TextRenderingHintSingleBitPerPixelGridFit, // ��������ݣ�ʹ������ƥ��
		TextRenderingHintSingleBitPerPixel, // ��������ݣ���ʹ������ƥ��
		TextRenderingHintAntiAliasGridFit, // ������ݣ�ʹ������ƥ��
		TextRenderingHintAntiAlias, // ������ݣ���ʹ������
		TextRenderingHintClearTypeGridFit // Һ����ʾ����ʹ��ClearType��ǿ����
	};

	for(int i = 0; i < 6; i++)
	{
		graphics.SetTextRenderingHint((TextRenderingHint)RenderingHint[i]);
		graphics.DrawString(L"Render", -1, &font, PointF(0,0), &SolidBrush(Color(255, 0, 0, 0)));
		graphics.TranslateTransform(0, font.GetHeight(0.0f));
	}
}
