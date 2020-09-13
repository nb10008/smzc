#pragma once
enum REBForChatInput
{
	EXUIE_ContextChange = EGUIE_User + 1024,
	EXUIE_ShowItemOrEquip,
};
class RichEditBoxForChatInput :
	public GUIRichEditBox
{
public:
	RichEditBoxForChatInput(void);
	~RichEditBoxForChatInput(void);
	//处理释放的物品
	virtual VOID OnEvent(tagGUIEvent* pEvent); 

	virtual BOOL OnInputMessage(tagGUIInputMsg* pMsg);

	/*
	设置是否展示拖放进空间的物品或者装备
	*/
	void SetShowItemOrEquip(bool show){m_bShowItemOrEquip = show ;};
private:
	bool m_bShowItemOrEquip;	//是否展示拖拽进来的物品或者装备
};
