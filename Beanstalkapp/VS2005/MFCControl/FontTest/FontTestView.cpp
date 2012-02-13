// FontTestView.cpp : CFontTestView ���ʵ��
//

#include "stdafx.h"
#include "FontTest.h"

#include "FontTestDoc.h"
#include "FontTestView.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFontTestView

IMPLEMENT_DYNCREATE(CFontTestView, CView)

BEGIN_MESSAGE_MAP(CFontTestView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFontTestView ����/����

CFontTestView::CFontTestView()
{
	// TODO: �ڴ˴���ӹ������

}

CFontTestView::~CFontTestView()
{
}

BOOL CFontTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CFontTestView ����

void CFontTestView::OnDraw(CDC* pDC)
{
	CFontTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	Gdiplus::Graphics			graphic(pDC->GetSafeHdc());
	Gdiplus::Font				font(L"����", 24, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	int							RenderingHint[]					= {
		Gdiplus::TextRenderingHintSystemDefault
		, Gdiplus::TextRenderingHintSingleBitPerPixelGridFit
		, Gdiplus::TextRenderingHintSingleBitPerPixel
		, Gdiplus::TextRenderingHintAntiAliasGridFit
		, Gdiplus::TextRenderingHintAntiAlias
		, Gdiplus::TextRenderingHintClearTypeGridFit
	};


	for(int i = 0; i < 6; i++)
	{
		graphic.SetTextRenderingHint((Gdiplus::TextRenderingHint)RenderingHint[i]);
		graphic.DrawString( L"�����Ѱ�ȫ���� 2Сʱ30����", -1, &font, Gdiplus::PointF(0.0f, 0.0f), &Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0)) );
		graphic.TranslateTransform(0.0f, font.GetHeight(0.0f));
	}
	// ˽������
	Gdiplus::PrivateFontCollection		pfc;
	Gdiplus::FontFamily					family[2];
	int									numfamilies			= 0;
	WCHAR								szFontName[64]		= {0};
	pfc.AddFontFile(L"΢���ź�.ttf");
	int count = pfc.GetFamilyCount();
	pfc.GetFamilies(count, family, &numfamilies);
	family[0].GetFamilyName(szFontName);
	Gdiplus::Font						font2(szFontName, 22, NULL, Gdiplus::UnitPixel, &pfc);

	for(int i = 0; i < 6; i++)
	{
		graphic.SetTextRenderingHint((Gdiplus::TextRenderingHint)RenderingHint[i]);
		graphic.DrawString( L"�����Ѱ�ȫ���� 2Сʱ30����", -1, &font2, Gdiplus::PointF(0.0f, 0.0f), &Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0)) );
		graphic.TranslateTransform(0.0f, font2.GetHeight(0.0f));
	}
	// �ҵ�����
	int					nf		= open("gnw-ghome.ttf", O_RDONLY);
	struct stat			st		= {0};
	BYTE*				pBuff	= NULL;
	Gdiplus::PrivateFontCollection		pfc2;
	if(-1 == nf)
		return;
	fstat(nf, &st);
	pBuff = new BYTE[st.st_size];
	read(nf, pBuff, st.st_size);
	close(nf);
	pfc2.AddMemoryFont(pBuff, st.st_size);
	delete [] pBuff;
	Gdiplus::FontFamily					family2[2];
	WCHAR								szFontName2[64]		= {0};
	
	numfamilies = 0;
	count = pfc2.GetFamilyCount();
	pfc2.GetFamilies(count, family2, &numfamilies);
	family2[0].GetFamilyName(szFontName2);
	Gdiplus::Font						font3(szFontName2, 22, NULL, Gdiplus::UnitPixel, &pfc2);

	for(int i = 0; i < 6; i++)
	{
		graphic.SetTextRenderingHint((Gdiplus::TextRenderingHint)RenderingHint[i]);
		graphic.DrawString( L"�����Ѱ�ȫ���� 2Сʱ30����-����", -1, &font3, Gdiplus::PointF(0.0f, 0.0f), &Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0)) );
		graphic.TranslateTransform(0.0f, font3.GetHeight(0.0f));
	}
}


// CFontTestView ��ӡ

BOOL CFontTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CFontTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CFontTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CFontTestView ���

#ifdef _DEBUG
void CFontTestView::AssertValid() const
{
	CView::AssertValid();
}

void CFontTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFontTestDoc* CFontTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFontTestDoc)));
	return (CFontTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CFontTestView ��Ϣ�������
