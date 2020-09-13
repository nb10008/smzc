#pragma once

#include "buttonex.h"
/** class StyleActionButton
	brief 个性化动作按钮
*/
class StyleActionButton :
	public ButtonEx
{
public:
	StyleActionButton(void);
	~StyleActionButton(void);

	virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
	virtual VOID Render();
	virtual VOID Update();
	virtual VOID Destroy();
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
	virtual VOID SendEvent(tagGUIEvent* pEvent);
	virtual VOID OnEvent(tagGUIEvent* pEvent); // 接受事件	

private:

private:
	TObjRef<GameFrameMgr>		m_pFrameMgr;		//frame管理器

};
