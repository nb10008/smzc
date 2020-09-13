#include "StdAfx.h"
#include "ChangeStatic.h"

ChangeStatic::ChangeStatic(void)
{
}

ChangeStatic::~ChangeStatic(void)
{
}

VOID ChangeStatic::Update()
{
	if (m_bLoopIng)
	{
		m_dwTimeDelat += Kernel::Inst()->GetDeltaTimeDW();
		if (m_dwTimeDelat > m_dwTimeSpace)
		{
			if (m_it == m_vecPointList.end())
				m_it = m_vecPointList.begin();

			m_lLeft = m_it->x;
			m_lTop = m_it->y;

			++m_it;

			m_dwTimeDelat = 0;
		}
		
	}
	else
	{
		m_dwTimeDelat = 0;
	}

	// äÖÈ¾µ×Í¼
	tagPoint			tempSize;
	tempSize.x = m_iTexW;
	tempSize.y = m_iTexH;

	tagPoint			tempView_pic;	
	tempView_pic.x =	m_lLeft;
	tempView_pic.y =	m_lTop;

	m_rcPic.SetLTWH(tempView_pic,tempSize);

	m_pPic->rc.SetLTWH(tempView_pic,tempSize);
	m_pPic->ptSize = tempSize;
	SetRefresh(TRUE);

	GUIWnd::Update();
}

VOID ChangeStatic::InitData()
{
	m_bLoopIng = false;
	m_it =m_vecPointList.begin();

	m_lLeft = m_it->x;
	m_lTop = m_it->y;
	m_dwTimeDelat = 0;

	// äÖÈ¾µ×Í¼
	tagPoint			tempSize;
	tempSize.x = m_iTexW;
	tempSize.y = m_iTexH;

	tagPoint			tempView_pic;	
	tempView_pic.x =	m_lLeft;
	tempView_pic.y =	m_lTop;

	m_rcPic.SetLTWH(tempView_pic,tempSize);

	m_pPic->rc.SetLTWH(tempView_pic,tempSize);
	m_pPic->ptSize = tempSize;
	SetRefresh(TRUE);
}