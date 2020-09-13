#include "StdAfx.h"
#include "StyleActionButton.h"
#include "ShortcutBarFrame.h"
#include "DragBtn.h"

#define RED_COLOR_OFFSET 0x000A0000
#define GREEN_COLOR_OFFSET 0x00000A00

StyleActionButton::StyleActionButton(void)
{

}

StyleActionButton::~StyleActionButton(void)
{
}

VOID StyleActionButton::Destroy()
{
	ButtonEx::Destroy();
}

BOOL StyleActionButton::Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement)
{
	BOOL bRes = ButtonEx::Init(pSystem, pFather, pXmlElement);

	return bRes;
}


VOID StyleActionButton::SendEvent(tagGUIEvent* pEvent)
{
	GUIButton::SendEvent(pEvent);
}

//处理快捷按钮接收的GUI事件
VOID StyleActionButton::OnEvent(tagGUIEvent* pEvent)
{
	if(pEvent->eEvent == EGUISCB_Drop)
	{
		tagPoint pt ( LOWORD( pEvent->dwParam4 ), HIWORD( pEvent->dwParam4 ) );
		if(this->PointInWnd(pt))
		{
			tagGUIEvent event = tagGUIEvent(this, (EGUIEvent)EGUISCB_Drop);
			event.dwParam1 = pEvent->dwParam1;
			event.dwParam2 = pEvent->dwParam2;
			event.dwParam3 = pEvent->dwParam3;
			event.dwParam4 = pEvent->dwParam4;
			SendEvent(&event);
		}
	}
}

BOOL StyleActionButton::OnInputMessage(tagGUIInputMsg* pMsg)
{
	if( m_bDisable )
		return GUIStatic::OnInputMessage(pMsg);

	switch( pMsg->dwType )
	{
		// 鼠标左右键操作为点击使用
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		if( m_pSystem->GetMousePoint() == this )
		{
			tagGUIEvent event(this, EGUIE_Click);
			SendEvent(&event);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if( m_pSystem->GetMousePoint() == this )
			{ 
				if( (MK_LBUTTON & pMsg->dwParam1) 
					&& PointInWnd(pMsg->pt) == FALSE )
				{
					tagGUIEvent event(this, EGUIE_Drag, pMsg);
					event.dwParam1 = EDT_StyleAction;
					event.dwParam2 = m_BtnData.GetData1();
					event.dwParam3 = m_BtnData.GetData2();
					event.dwParam4 = MAKELONG( pMsg->pt.x, pMsg->pt.y);	
					SendEvent(&event);
					return true;
				}	
			}
			SetRefresh();
			//return GUIButton::OnInputMessage(pMsg);
		}
		break;	
	}
	return GUIStatic::OnInputMessage(pMsg);
}

VOID StyleActionButton::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	DrawImg();

	// 调用基类渲染
	ButtonEx::Render();
}

VOID StyleActionButton::Update()
{
	ButtonEx::Update();
}
