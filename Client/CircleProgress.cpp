#include "StdAfx.h"
#include "CircleProgress.h"
#include "math.h"

#ifndef PI
#define PI 3.1415926536f
#endif
CircleProgress::CircleProgress(void):
m_fLastTime(0.0f),
m_nCircleCount(0),
m_fLastValue(0.0f),
m_fLastAngel(0.0f)
{
}

CircleProgress::~CircleProgress(void)
{

}

BOOL CircleProgress::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	if( !GUIProgress::Init( pSystem, pFather, pXmlElement ) )
		return FALSE;
	for (UINT i = 0; i < m_vecPicList.size(); i++)
	{
		tagCircleImage* pic = m_vecPicList[i];
		m_pRender->DestroyImage(pic->img);
		SAFE_DEL(pic);
	}
	m_vecPicList.clear();
	return TRUE;
}

VOID CircleProgress::Destroy()
{
	for (UINT i = 0; i < m_vecPicList.size(); i++)
	{
		tagCircleImage* pic = m_vecPicList[i];
		m_pRender->DestroyImage(pic->img);
		SAFE_DEL(pic);
	}
	m_vecPicList.clear();
	GUIProgress::Destroy();
}

VOID CircleProgress::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	FLOAT fTime = Kernel::Inst()->GetAccumTime();

	// 渲染底图
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	FLOAT fDis = fTime - m_fLastTime;
	fDis *= (m_fCurrentValue - m_fLastValue)*0.8f;
	fDis  = max(fDis,0.02f);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite, m_fLastAngel + fDis, 1.0f);
	m_fLastValue = m_fCurrentValue;
	m_fLastAngel = m_fLastAngel + fDis;
	m_fLastTime = fTime;

	// 固定角度旋转方式
	//FLOAT fAn = m_nCircleCount*PI/4.0f;
	//if (fTime - m_fLastTime > 0.02f)
	//{
	//	fAn = ++m_nCircleCount*PI/4.0f;
	//	m_fLastTime = fTime;
	//}

	// 渲染文字
	TCHAR LoadPro[8] = {0};
	_sntprintf(LoadPro,sizeof(LoadPro)/sizeof(TCHAR),_T("%d%%"),(INT)((m_fCurrentValue/m_fMaxValue)*100.0f));
	tstring str = LoadPro;
	
	m_pCanvas->rc = m_rcText + m_ptView;
	m_pRender->Text(m_pCanvas, str, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);

	// 渲染其他可旋转的图片
	for (UINT i = 0; i < m_vecPicList.size(); i++)
	{
		tagCircleImage *cirImg = m_vecPicList[i];
		m_pCanvas->rc =cirImg->rcDest + m_ptView;
		if (cirImg->bIsClockwise)
		{
			m_pRender->Draw(m_pCanvas, cirImg->img, m_dwPicColor, m_eAlphaWrite, fTime*cirImg->fSpeed,1.0f);
		}
		else
		{
			m_pRender->Draw(m_pCanvas, cirImg->img, m_dwPicColor, m_eAlphaWrite, -fTime*cirImg->fSpeed,1.0f);
		}
	}
	
	// 调用基类渲染
	GUIWnd::Render();
}

VOID CircleProgress::AddNewPic(const tstring strPath, tagRect rcDest, tagRect picRect, BOOL isClockwise /* = TRUE */, FLOAT fSpeed /* = 0 */)
{
	tagCircleImage *newPic = new tagCircleImage;
	newPic->rcDest = rcDest;
	newPic->bIsClockwise = isClockwise;
	newPic->fSpeed = fSpeed;
	newPic->img = m_pRender->CreateImage(strPath,picRect);

	m_vecPicList.push_back(newPic);
}