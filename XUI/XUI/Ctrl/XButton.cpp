#include "StdAfx.h"

#include "XButton.h"
#include "..\..\..\XLib\inc\interfaceS\string\StringCode.h"
#include "..\Base\XResourceMgr.h"

X_IMPLEMENT_FRAME_XML_RUNTIME(CXButton)

CXFrame * CXButton::CreateFrameFromXML(X_XML_NODE_TYPE xml, CXFrame *pParent)
{
 	if (!xml)
 		return NULL;
 
	X_XML_ATTR_TYPE attr = NULL;

	IXImage *pFace = NULL;
	BOOL bDisabled = FALSE;
	
	attr = xml->first_attribute("bg", 0, false);
	if (attr)
		xml->remove_attribute(attr);

	pFace = CXFrameXMLFactory::BuildImage(xml, "face", "face_type", "stretch", "face_part_");

	attr = xml->first_attribute("disabled", 0, false);
	if (attr && !StrCmpIA(attr->value(), "true"))
		bDisabled = TRUE;

 	CXButton *pFrame = new CXButton();
	pFrame->Create(pParent, CXFrameXMLFactory::BuildRect(xml), FALSE,
		bDisabled, pFace, (CXFrame::WIDTH_MODE)CXFrameXMLFactory::GetWidthMode(xml), (CXFrame::HEIGHT_MODE)CXFrameXMLFactory::GetHeightMode(xml));

 	return pFrame;
}


CXButton::CXButton(void)
	: m_bMouseIn(FALSE),
	m_bDisabled(TRUE)
{
}

BOOL CXButton::Create(  CXFrame * pFrameParent, const CRect &rc /*= CRect(0, 0, 0, 0)*/, BOOL bVisible /*= FALSE*/ ,
					  BOOL bDisabled /* = FALSE*/, IXImage *pBackground /*= NULL*/, 
					  WIDTH_MODE aWidthMode /*= WIDTH_MODE_NOT_CHANGE*/, HEIGHT_MODE aHeightMode/* = HEIGHT_MODE_NOT_CHANGE*/)
{
	BOOL bRtn = __super::Create(pFrameParent, rc, bVisible, aWidthMode, aHeightMode);

	m_bDisabled = bDisabled;
	m_state = bDisabled ? BTN_DISABLED : BTN_NORMAL;

	if (!pBackground)
		pBackground = CXResourceMgr::GetImage(_T("img/ctrl/button.9.png"));

	ATLASSERT(!pBackground || pBackground->GetImageWidth() % 4 == 0);

	delete SetBackground(pBackground);

	return bRtn;
}

VOID CXButton::Destroy()
{
	m_bMouseIn = FALSE;
	m_bDisabled = TRUE;

	__super::Destroy();
}

IXImage * CXButton::SetBackground( IXImage * pDrawBackground )
{
	ATLASSERT(!pDrawBackground || pDrawBackground->GetImageWidth() % 4 == 0);

	IXImage *pRtn = __super::SetBackground(pDrawBackground);
	RefreashButtonFace();

	return pRtn;
}

VOID CXButton::RefreashButtonFace()
{
	IXImage *pBackground = GetBackground();

	if (!pBackground)
		return;

	ATLASSERT(pBackground->GetImageWidth() % 4 == 0);

	pBackground->SetSrcRect(CRect( CPoint(m_state * pBackground->GetImageWidth() / 4, 0), 
		CSize(pBackground->GetImageWidth() / 4, pBackground->GetImageHeight())));
}

LRESULT CXButton::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL& bCancelBabble )
{
	bHandled = TRUE;

	if (m_bDisabled)
		return 0;

	m_state = BTN_DOWN;
	RefreashButtonFace();

	InvalidateRect();

	return 0;
}

LRESULT CXButton::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL& bCancelBabble )
{
	bHandled = TRUE;

	if (m_bDisabled)
		return 0;

	BOOL bThrowClick = (m_state == BTN_DOWN);

	m_state = m_bMouseIn ? BTN_HOVER : BTN_NORMAL;
	RefreashButtonFace();

	InvalidateRect();

	if (bThrowClick)
		ThrowEvent(EVENT_BUTTON_CLICKED, 0, 0);

	return 0;
}

LRESULT CXButton::OnMouseEnter( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL& bCancelBabble )
{
	m_bMouseIn = TRUE;

	if (m_bDisabled)
		return 0;

	m_state = BTN_HOVER;
	RefreashButtonFace();

	InvalidateRect();

	return 0;
}

LRESULT CXButton::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL& bCancelBabble )
{
	m_bMouseIn = FALSE;

	if (m_bDisabled)
		return 0;

	m_state = BTN_NORMAL;
	RefreashButtonFace();

	InvalidateRect();

	return 0;
}

BOOL CXButton::EnableButton( BOOL bEnable /*= TRUE*/ )
{
	if ((!bEnable && m_bDisabled) || (bEnable && !m_bDisabled))
		return TRUE;

	m_bDisabled = !bEnable;

	if (m_bDisabled)
		m_state = BTN_DISABLED;
	else
		m_state = m_bMouseIn ? BTN_HOVER: BTN_NORMAL;
	RefreashButtonFace();

	InvalidateRect();

	return TRUE;
}

INT CXButton::CalculateAdaptBackgroundWidth()
{
	return __super::CalculateAdaptBackgroundWidth() / 4;
}


