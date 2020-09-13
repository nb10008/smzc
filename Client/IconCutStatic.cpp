#include "StdAfx.h"
#include "IconCutStatic.h"

IconCutStatic::IconCutStatic(void)
{
}

IconCutStatic::~IconCutStatic(void)
{
}

VOID IconCutStatic::Destroy()
{
	GUIStatic::Destroy();
}

VOID IconCutStatic::Update()
{
	
	GUIWnd::Update();
}
VOID IconCutStatic::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	if (m_CurrentValue < m_ScaleValue)
	{
		m_CurrentValue++;
	}
	else
	{
		m_ScaleValue = m_CurrentValue;
	}
 
 	// 渲染底图
 	tagPoint			tempView;	
 	tagPoint			tempSize;
 	tempSize.x = m_ptSize.x;
 	tempSize.y = m_ptSize.y*m_CurrentValue/m_CutNums;
 	tempView.x = m_ptView.x;
 	tempView.y = m_ptView.y + (m_ptSize.y - tempSize.y );
 	m_pCanvas->rc.SetLTWH(tempView, tempSize);
 
 	tagPoint			tempView_pic;	
 	tempView_pic.x = m_rcPic.left;
 	tempView_pic.y = m_rcPic.top + (m_ptSize.y - tempSize.y );
 	//m_rcPic.SetLTWH(tempView_pic,tempSize);
 	m_pPic->rc.SetLTWH(tempView_pic,tempSize);
 
 
 	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	// 调用基类渲染
	GUIWnd::Render();
}
