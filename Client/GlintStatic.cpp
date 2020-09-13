#include "StdAfx.h"
#include "GlintStatic.h"

GlintStatic::GlintStatic()
{
	m_bShowGlint = false;
	m_bShowInterval = false;
	m_strGlintText = _T("");
	m_dwGlintInterval = 400;	//ºÁÃë
	m_pGlintPic = NULL;
}

GlintStatic::~GlintStatic()
{

}

BOOL GlintStatic::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRet = GUIStatic::Init( pSystem, pFather, pXmlElement );

	m_dwGlintTextColor = ~m_dwTextColor;
	m_strGlintText = m_strText;
	m_dwGlintPicColor = m_dwPicColor;
	return bRet;
}

VOID GlintStatic::Destroy()
{
	GUIStatic::Destroy();

	if( P_VALID(m_pGlintPic) )
	{
		m_pRender->DestroyImage( m_pGlintPic );
		m_pGlintPic = NULL;
	}
}

VOID GlintStatic::Update()
{
	GUIStatic::Update();
	SetRefresh( TRUE );

	if( !m_bShowGlint )
		return;

	DWORD dwCurrentTime = timeGetTime();
	if( (dwCurrentTime - m_dwLastGlint) < m_dwGlintInterval )
	{
		m_bShowInterval = true;
	}
	else if( (dwCurrentTime - m_dwLastGlint) <= (m_dwGlintInterval*2) )
	{
		m_bShowInterval = false;
	} 
	else
	{
		m_dwLastGlint = dwCurrentTime;
		m_bShowInterval = true;
	}
}

VOID GlintStatic::Render()
{
	GUIStatic::Render();

	if(m_bShowGlint)
		DrawGlint();
}

void GlintStatic::DrawGlint()
{
	if( m_bShowInterval )
	{
		if( P_VALID(m_pGlintPic) )
		{
			m_pRender->Draw(m_pCanvas, m_pGlintPic, m_dwGlintPicColor, m_eAlphaWrite);
		}
		else
		{
			m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite );
		}

		tstring str = m_strText;
		if( !m_strGlintText.empty() )
			tstring str = m_strGlintText;

		tstring sub(_T("\\n"));
		while(tstring::npos != str.find(sub))
			str.replace(str.find(sub), sub.size(), _T("\n"));

		m_pRender->Text(m_pCanvas, str, m_pFont, m_dwGlintTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
		
	}
}