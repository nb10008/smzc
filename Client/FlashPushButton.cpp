#include "stdafx.h"
#include "FlashPushButton.h"

FlashButton::FlashButton( void )
{
	m_bFlash = false;
	m_bTextFlash = false;
}

FlashButton::~FlashButton( void )
{

}

BOOL FlashButton::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bRet = GUIButton::Init(pSystem, pFather, pXmlElement);
	return bRet;
}

VOID FlashButton::Destroy()
{
	GUIButton::Destroy();
}


VOID FlashButton::Update()
{
	GUIButton::Update();

	if( !m_bInvisible && m_bFlash )
	{
		// 修改A值
		Color4ub PicColor = 0xFF000000 | m_dwPicColor;
		Color4ub TextColor = 0xFF000000 | m_dwTextColor;
		TextColor.A = PicColor.A = (BYTE)( 255*fabs(cos(Kernel::Inst()->GetAccumTime()*2)) );
		m_dwPicColor = PicColor;
		if(m_bTextFlash)
			m_dwTextColor = TextColor;
		SetRefresh(true);
	}
}

void FlashButton::SetFlash(bool bVal, bool bText)
{
	if( !bVal )
	{
		m_dwPicColor |= 0xFF000000;
		if(!bText)
			m_dwTextColor |= 0xFF000000;
	}
	m_bFlash = bVal;
	m_bTextFlash = bText;
};


FlashPushButton::FlashPushButton( void )
{
	m_bFlash = false;
}

FlashPushButton::~FlashPushButton( void )
{

}

BOOL FlashPushButton::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bRet = GUIPushButton::Init(pSystem, pFather, pXmlElement);
	return bRet;
}

VOID FlashPushButton::Destroy()
{
	GUIPushButton::Destroy();
}


VOID FlashPushButton::Update()
{
	GUIPushButton::Update();

	if( !m_bInvisible && m_bFlash )
	{
		// 修改A值
		Color4ub color = 0xFF000000 | m_dwPicColor;
		Color4ub Textcolor = 0xFF000000 | m_dwTextColor;
		Textcolor.A = color.A = (BYTE)( 255*fabs(cos(Kernel::Inst()->GetAccumTime()*8)) );
		m_dwPicColor = color;
		m_dwTextColor = Textcolor;
		SetRefresh(true);
	}
}

void FlashPushButton::SetFlash(bool bVal)
{
	if( !bVal )
	{
		m_dwPicColor |= 0xFF000000;
		//m_dwTextColor |= 0xFF000000;
	}
	m_bFlash = bVal;
};


FlashStatic::FlashStatic( void )
{
	m_bTextFlash = false;
}

FlashStatic::~FlashStatic( void )
{

}

BOOL FlashStatic::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bRet = GUIStatic::Init(pSystem, pFather, pXmlElement);
	return bRet;
}

VOID FlashStatic::Destroy()
{
	GUIStatic::Destroy();
}


VOID FlashStatic::Update()
{
	GUIStatic::Update();

	if( !m_bInvisible )
	{
		// 修改A值
		Color4ub TextColor = 0xFF000000 | m_dwTextColor;
		TextColor.A = (BYTE)( 255*fabs(cos(Kernel::Inst()->GetAccumTime()*4)) );
		if(m_bTextFlash)
			m_dwTextColor = TextColor;
		SetRefresh(true);
	}
}

void FlashStatic::SetFlash(bool bText)
{
	if(!bText)
		m_dwTextColor |= 0xFF000000;
	m_bTextFlash = bText;
};
