#include "stdafx.h"
#include "IconStatic.h"
#include "GameClientDefine.h"

IconStatic::IconStatic( void )
{

}

IconStatic::~IconStatic( void )
{

}

BOOL IconStatic::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bRet = GUIStatic::Init(pSystem, pFather, pXmlElement);
	return bRet;
}

VOID IconStatic::Destroy()
{
	GUIStatic::Destroy();
}

BOOL IconStatic::OnInputMessage( tagGUIInputMsg* pMsg )
{
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:
		{
			GUIWnd* wnd = m_pSystem->GetMousePoint();
			if(wnd == this)
			{
				tagGUIEvent event(this, EGUIE_Drag, pMsg);
				SendEvent(&event);
			}
		}
		break;
	case WM_RBUTTONUP:
		{
			tagGUIEvent event(this, EGUIE_Click);
			SendEvent(&event);
		}
		break;
	case WM_MOUSEMOVE:
		{
			
		}
		break;

	default:
		break;
	}

	return GUIStatic::OnInputMessage(pMsg);
}

VOID IconStatic::OnEvent(tagGUIEvent* pEvent)
{
	if(pEvent->eEvent == EGUISCB_Drop)
	{
		tagPoint pt ( LOWORD( pEvent->dwParam4 ), HIWORD( pEvent->dwParam4 ) );
		GUIWnd* pWnd = m_pSystem->GetDesktop()->GetWnd(pt);
		if(this->PointInWnd(pt) && P_VALID(pWnd) && this == pWnd)
		{
			tagGUIEvent event = tagGUIEvent(this, (EGUIEvent)EGUISCB_Drop);
			event.dwParam1 = pEvent->dwParam1;
			event.dwParam2 = pEvent->dwParam2;
			event.dwParam3 = pEvent->dwParam3;
			event.dwParam4 = pEvent->dwParam4;
			SendEvent(&event);
		}
	}
	GUIWnd::OnEvent(pEvent);
}
