#include "StdAfx.h"
#include "RollStatic.h"

#define TIGER_TEX_W 96
#define TIGER_TEX_H 125
#define Start_Speed 25


RollStatic::RollStatic(void)
{
}

RollStatic::~RollStatic(void)
{
}

VOID RollStatic::InitData()
{
	m_lLeft = 0;
	m_lTop = (rand() % (5)) * TIGER_TEX_H;
	m_iTexW = TIGER_TEX_W;
	m_iTexH = TIGER_TEX_H;

	m_lSpeed = 0;
	m_lLoopValue = TIGER_TEX_H * 5;
	
	m_eState = ERS_WaitForStart;

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
VOID RollStatic::Update()
{
 	if (ERS_WaitForStart == m_eState)	return;
 
	if(m_lSpeed <=1)
		m_lSpeed = 1;
	
 	if(m_lTop >= m_lLoopValue)
	{
		m_lTop = m_lTop - m_lLoopValue;

			// ¼õËÙ½×¶Î
		if (ERS_SlowStopping == m_eState)
			m_lSpeed --;
	}
 	else
 		m_lTop+=m_lSpeed;

	if (ERS_SlowStopping == m_eState && m_lSpeed <= 3)
	{
		if(m_iStopStep == m_lTop)
		{
			m_eState = ERS_WaitForStart;
		}
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


VOID RollStatic::Stop(int step /*= 0*/)
{
	m_iStopStep = step;
	m_eState = ERS_SlowStopping;
}

VOID RollStatic::Start()
{
	m_eState = ERS_Starting;
	m_lSpeed = Start_Speed + (rand() % (6));
}

VOID RollStatic::StopImmediately(int step/* = 0*/)
{
	m_iStopStep = step;
	m_lTop = m_iStopStep * TIGER_TEX_H;
	m_eState = ERS_WaitForStart;
	m_lSpeed = 0;

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