#include "stdafx.h"

#include "ButtonEx2.h"


BOOL ButtonEx2::OnInputMessage(tagGUIInputMsg* pMsg)
{
	if( m_bDisable )
	{
		if( pMsg->dwType == WM_LBUTTONDOWN || pMsg->dwType == WM_LBUTTONDBLCLK )
		{
			if( m_pSystem->GetMousePoint() == this ) // if( PointInWnd(pMsg->pt) )
			{
				m_pRender->PlaySound(_T("button_disable"), 0.7f);
			}
		}
		return GUIStatic::OnInputMessage(pMsg);
	}

	BOOL bRes = GUIStatic::OnInputMessage(pMsg);

	switch( pMsg->dwType )
	{
	case WM_RBUTTONDOWN:
		if( m_pSystem->GetMousePoint() == this ) // if( PointInWnd(pMsg->pt) )
		{
			m_eState = EGUIBS_PushDown;			
			SetRefresh();
			m_pRender->PlaySound(_T("button_down"), 0.7f);
			tagGUIEvent event(this, (EGUIEvent)EGUIE_RightClick, pMsg);
			SendEvent(&event);
		}		
		break;

	case WM_LBUTTONDBLCLK:
		if( m_pSystem->GetMousePoint() == this ) // if( PointInWnd(pMsg->pt) )
		{
			m_eState = EGUIBS_PushDown;			
			SetRefresh();
			m_pRender->PlaySound(_T("button_down"), 0.7f);
			tagGUIEvent event(this, (EGUIEvent)EGUIE_DoubleClick, pMsg);
			SendEvent(&event);
		}		
		break; 
	
	case WM_LBUTTONDOWN:	
		if( m_pSystem->GetMousePoint() == this ) // if( PointInWnd(pMsg->pt) )
		{
			m_eState = EGUIBS_PushDown;
			m_bHaveMouseKeyDown = true;
			SetRefresh();
			m_pRender->PlaySound(_T("button_down"), 0.7f);
		}		
		break;

	case WM_MOUSEMOVE:
		if( m_pSystem->GetMousePoint() == this )
		{ 
			if( (MK_LBUTTON & pMsg->dwParam1) && m_bHaveMouseKeyDown )
				m_eState = EGUIBS_PushDown;
			else
				m_eState = EGUIBS_HighLight;
		}
		else
		{
			m_eState = EGUIBS_PopUp;	// 鼠标按下后离开，按钮需要松开
		}
		SetRefresh();
		break;

	case WM_LBUTTONUP:
		if( m_bHaveMouseKeyDown && PointInWnd(pMsg->pt) )
		{
			tagGUIEvent event(this, EGUIE_Click, pMsg);
			SendEvent(&event);
			m_eState = EGUIBS_HighLight;
		}
		else
			m_eState = EGUIBS_PopUp;
		m_bHaveMouseKeyDown = false;
		SetRefresh();
		break;
	}

	return bRes;
}
