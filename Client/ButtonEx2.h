#pragma once

enum EGUIEventEx
{
	EGUIE_RightClick = EGUIE_User + 100 + 1,
	EGUIE_DoubleClick,
};

// ÓÐÓÒ¼üºÍË«»÷µÄbutton
class ButtonEx2 :
	public GUIButton
{
public:
	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);
};