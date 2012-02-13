// FontTestView.cpp : CFontTestView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CFontTestView 构造/析构

CFontTestView::CFontTestView()
{
	// TODO: 在此处添加构造代码

}

CFontTestView::~CFontTestView()
{
}

BOOL CFontTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFontTestView 绘制

void CFontTestView::OnDraw(CDC* pDC)
{
	CFontTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	Gdiplus::Graphics			graphic(pDC->GetSafeHdc());
	Gdiplus::Font				font(L"宋体", 24, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
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
		graphic.DrawString( L"今天已安全上网 2小时30分钟", -1, &font, Gdiplus::PointF(0.0f, 0.0f), &Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0)) );
		graphic.TranslateTransform(0.0f, font.GetHeight(0.0f));
	}
	// 私有字体
	Gdiplus::PrivateFontCollection		pfc;
	Gdiplus::FontFamily					family[2];
	int									numfamilies			= 0;
	WCHAR								szFontName[64]		= {0};
	pfc.AddFontFile(L"微软雅黑.ttf");
	int count = pfc.GetFamilyCount();
	pfc.GetFamilies(count, family, &numfamilies);
	family[0].GetFamilyName(szFontName);
	Gdiplus::Font						font2(szFontName, 22, NULL, Gdiplus::UnitPixel, &pfc);

	for(int i = 0; i < 6; i++)
	{
		graphic.SetTextRenderingHint((Gdiplus::TextRenderingHint)RenderingHint[i]);
		graphic.DrawString( L"今天已安全上网 2小时30分钟", -1, &font2, Gdiplus::PointF(0.0f, 0.0f), &Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0)) );
		graphic.TranslateTransform(0.0f, font2.GetHeight(0.0f));
	}
	// 我的字体
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
		graphic.DrawString( L"今天已安全上网 2小时30分钟-测试", -1, &font3, Gdiplus::PointF(0.0f, 0.0f), &Gdiplus::SolidBrush(Gdiplus::Color(255, 0, 0, 0)) );
		graphic.TranslateTransform(0.0f, font3.GetHeight(0.0f));
	}
}


// CFontTestView 打印

BOOL CFontTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFontTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFontTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CFontTestView 诊断

#ifdef _DEBUG
void CFontTestView::AssertValid() const
{
	CView::AssertValid();
}

void CFontTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFontTestDoc* CFontTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFontTestDoc)));
	return (CFontTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CFontTestView 消息处理程序
