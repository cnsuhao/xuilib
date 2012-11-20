#include "StdAfx.h"
#include "XTextGdiPlus.h"

CXTextGdiPlus::CXTextGdiPlus(void)
	: m_dcBuffer(NULL), m_hBufferOldBmp(NULL), m_rcDst(0, 0, 0, 0),
	m_strText(_T("")),
	m_strFontName(_T("Arial")), m_nSize(13), m_FontStyle(Gdiplus::FontStyleRegular),
	m_ColorR(0),
	m_ColorG(0),
	m_ColorB(0),
	m_AlignmentH(Gdiplus::StringAlignmentNear), m_AlignmentV(Gdiplus::StringAlignmentNear),
	m_FormatFlags((Gdiplus::StringFormatFlags)0),
	m_Rendering(Gdiplus::TextRenderingHintSingleBitPerPixelGridFit),
	m_cAlpha(254)
{
}

CXTextGdiPlus::~CXTextGdiPlus(void)
{
}

BOOL CXTextGdiPlus::Draw(HDC hDC, const CRect &rcDraw)
{
	if (!m_dcBuffer && !RefreashBufferDC(hDC))
		return FALSE;

	BLENDFUNCTION blend = {AC_SRC_OVER, 0, m_cAlpha, AC_SRC_ALPHA};

	CRect rcDrawReal(0, 0, 0, 0);
	if (!::IntersectRect(&rcDrawReal, &rcDraw, &m_rcDst) || rcDrawReal.IsRectEmpty())
		return TRUE;

	::AlphaBlend(hDC, rcDrawReal.left, rcDrawReal.top, rcDrawReal.Width(), rcDrawReal.Height(), 
		m_dcBuffer, rcDrawReal.left - m_rcDst.left, rcDrawReal.top - m_rcDst.top, rcDrawReal.Width(), rcDrawReal.Height(), 
		blend);

	return TRUE;
}

BOOL CXTextGdiPlus::SetAlpha(BYTE cAlpha)
{
	m_cAlpha = cAlpha;
	return TRUE;
}

BOOL CXTextGdiPlus::SetDstRect(const CRect &rcDst)
{
	if (rcDst == m_rcDst)
		return TRUE;

	ReleaseBufferDC();
	m_rcDst = rcDst;

	return TRUE;
}

BOOL CXTextGdiPlus::SetText(LPCTSTR szText)
{
	if (!szText)
		return FALSE;

	if (m_strText == szText)
		return TRUE;

	ReleaseBufferDC();
	m_strText = szText;
	return TRUE;
}

BOOL CXTextGdiPlus::SetFont(LPCTSTR szFontName, int nSize, Gdiplus::FontStyle style)
{
	if (!szFontName || 0 == nSize)
		return FALSE;

	if (m_strFontName == szFontName && m_nSize == nSize && m_FontStyle == style)
		return TRUE;

	ReleaseBufferDC();
	m_strFontName = szFontName;
	m_nSize = nSize;
	m_FontStyle = style;

	return TRUE;
}

BOOL CXTextGdiPlus::SetColor(BYTE alpha, BYTE r, BYTE g, BYTE b)
{
	if (alpha == 255)
		alpha = 254;

	if (alpha == m_cAlpha && r == m_ColorR && g == m_ColorG && b == m_ColorB)
		return TRUE;

	ReleaseBufferDC();
	m_cAlpha = alpha;
	m_ColorR = r;
	m_ColorG = g;
	m_ColorB = b;
	return TRUE;
}

BOOL CXTextGdiPlus::SetRendering(Gdiplus::TextRenderingHint Rendering)
{
	if (m_Rendering == Rendering)
		return TRUE;

	ReleaseBufferDC();
	m_Rendering = Rendering;
	return TRUE;
}

BOOL CXTextGdiPlus::SetAlignment(Gdiplus::StringAlignment AlignmentH, Gdiplus::StringAlignment AlignmentV)
{
	if (m_AlignmentH == AlignmentH && m_AlignmentV == AlignmentV)
		return TRUE;

	ReleaseBufferDC();
	m_AlignmentH = AlignmentH;
	m_AlignmentV = AlignmentV;
	return TRUE;
}

BOOL CXTextGdiPlus::SetFormatFlags(Gdiplus::StringFormatFlags FormatFlags)
{
	if (m_FormatFlags == FormatFlags)
		return TRUE;

	ReleaseBufferDC();
	m_FormatFlags = FormatFlags;
	return TRUE;
}

VOID CXTextGdiPlus::ReleaseBufferDC()
{
	if (m_dcBuffer)
	{
		HGDIOBJ hMyBitmap = ::SelectObject(m_dcBuffer, m_hBufferOldBmp);
		m_hBufferOldBmp = NULL;
		::DeleteObject(hMyBitmap);
		hMyBitmap = NULL;
		::DeleteDC(m_dcBuffer);
		m_dcBuffer = NULL;
	}
}

BOOL CXTextGdiPlus::RefreashBufferDC(HDC hDCSrc)
{	
	ReleaseBufferDC();

	m_dcBuffer = ::CreateCompatibleDC(hDCSrc);
	m_hBufferOldBmp = ::SelectObject(m_dcBuffer, (HGDIOBJ)Util::CreateDIBSection32(m_rcDst.Width(), m_rcDst.Height()));

	Gdiplus::Graphics graph(m_dcBuffer);
	graph.SetTextRenderingHint(m_Rendering);

#ifdef _UNICODE
	Gdiplus::FontFamily fontFamily(m_strFontName);  
#else
	Gdiplus::FontFamily fontFamily(XLibS::StringCode::ConvertAnsiStrToWideStr(m_strFontName));
#endif
	Gdiplus::Font font(&fontFamily, m_nSize, m_FontStyle, Gdiplus::UnitPixel);  
	Gdiplus::StringFormat stringformat;
	stringformat.SetAlignment(m_AlignmentH);
	stringformat.SetLineAlignment(m_AlignmentV);
	stringformat.SetFormatFlags(m_FormatFlags);
	stringformat.SetTrimming(Gdiplus::StringTrimmingEllipsisWord);
	
	Gdiplus::SolidBrush brush(Gdiplus::Color(m_cAlpha, m_ColorR, m_ColorG, m_ColorB));

#ifdef _UNICODE
	graph.DrawString(m_strText, -1, &font, Gdiplus::RectF(0, 0, m_rcDst.Width(), m_rcDst.Height()), &stringformat, &brush);
#else
	graph.DrawString(XLibS::StringCode::ConvertAnsiStrToWideStr(m_strText), -1, &font, Gdiplus::RectF(0, 0, m_rcDst.Width(), m_rcDst.Height()), &stringformat, &brush);
#endif
	
	return TRUE;
}

CString CXTextGdiPlus::GetText()
{
	return m_strText;
}
