#include "stdafx.h"
#include "PicStatic.h"

PicStatic::PicStatic( void )
{

}

PicStatic::~PicStatic( void )
{

}

BOOL PicStatic::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	BOOL bRet = GUIStatic::Init(pSystem, pFather, pXmlElement);
	return bRet;
}

VOID PicStatic::Destroy()
{
	GUIStatic::Destroy();
}

BOOL PicStatic::OnInputMessage( tagGUIInputMsg* pMsg )
{
	switch( pMsg->dwType )
	{
	case WM_LBUTTONDOWN:
		{
			tagGUIEvent event(this, EGUIE_Drag, pMsg);
			SendEvent(&event);
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
