#include "StdAfx.h"
#include "BlinkStatic.h"

extern const TCHAR* BlinkPic;
BlinkStatic::BlinkStatic(void)
{
}

BlinkStatic::~BlinkStatic(void)
{
}

void BlinkStatic::StartBlink()
{
	m_rcPic.SetLTWH(m_pPtStart,m_pPtSizeOfPic);
	m_row = 0;
	m_column = 0;
	m_bBlinking = true;

	SetInvisible(FALSE);
}

VOID BlinkStatic::Update()
{
	if(!m_bBlinking)		return;

	tagPoint tempPt;
	tempPt.Set(m_pPtStart.x + m_column*m_pPtSizeOfPic.x,m_pPtStart.y + m_row*m_pPtSizeOfPic.y);
	m_rcPic.SetLTWH(tempPt,m_pPtSizeOfPic);
// 	m_eAlphaWrite = EGUIAW_Write;
// 	m_dwPicColor = 0xFFFFFFFF;
	m_pPic->rc = m_rcPic;
	m_pPic->ptSize = m_pPtSizeOfPic;
	SetRefresh(TRUE);

	m_column++;
	if(m_column >=  (DWORD)sqrt(m_dwPicLoopNum))
	{
		m_column = 0;
		m_row++;
	}

	if(	m_row >=  (DWORD)sqrt(m_dwPicLoopNum))
	{
		SetInvisible(TRUE);
		m_bBlinking = false;
		m_row = 0;
		m_column = 0;
		return;
	}

	GUIWnd::Update();
		
}