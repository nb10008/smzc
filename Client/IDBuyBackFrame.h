#pragma once
#include "ComposeStatic.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "..\WorldDefine\ItemDefine.h"
#include "ActiveProgress.h"

struct tagNC_ReceiveGift;
struct tagNS_ReceiveGift;
struct tagNC_GetLiLianExp;
struct tagNS_GetLiLianExp;
struct tagNS_LiLianChange;

class IDButBackFrame : public GameFrame
{
private:
	enum EDeComposeMsg
	{
		EDeCompose_NULL = 0,
		EDeCompose_SendMsg,		
		EDeCompose_Waiting,
		EDeCompose_RecvMsg,
	};

	enum EWhichFrame	//打开哪个Frame...
	{
		EFrameNULL = 0,
		EFrameHaveReceive,		//已经领取
		EFrameIDbuyBack,		//账号回购
		EFrameReceiveGift,		//领取奖励
		EFrameLiLianDu,			//历练度
	};
public:
	IDButBackFrame();
	~IDButBackFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnViewLiLian(tagGameEvent* pEvent);
	DWORD OnReceiveGift(tagGameEvent* pEvent);
	DWORD OnIDBuyBack(tagGameEvent* pEvent);
	DWORD OnHaveReceive(tagGameEvent* pEvent);

	DWORD OnNS_GetLiLianExp(tagNS_GetLiLianExp* pCmd, DWORD);
	DWORD OnNS_ReceiveGift(tagNS_ReceiveGift* pCmd, DWORD);
	DWORD OnNS_LiLianChange(tagNS_LiLianChange* pCmd, DWORD);

	VOID CloseAllWnd();
	VOID SetInfo();

private:
	TSFPTrunk<IDButBackFrame>				m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*									m_pWndLiLianDu;								//带完成度的那个
	IconStatic*								m_pIStcItemIcon[MAX_GIFT_ITEM_NUM];			//物品图标
	GUIStatic*								m_pStcItemName[MAX_GIFT_ITEM_NUM];			//物品名称
	ActiveProgress*							m_pProProcess;								//总进度条	
	ActiveProgress*							m_pProCurProcess;							//当前进度条
	IconStatic*								m_pIStcItemShowIcon[MAX_GIFT_ITEM_SHOW_NUM];//显示完成度高的奖励
	GUIStatic*								m_pStcItemShowName[MAX_GIFT_ITEM_SHOW_NUM];	//
	GUIStatic*								m_pStcDesc[MAX_GIFT_ITEM_SHOW_NUM];			//物品描述
	GUIButton*								m_pBtndLiLianDuClose;								//关闭按钮
	GUIStatic*								m_pStcCompleteDesc;							//总完成度
	GUIStatic*								m_pStcCurCompleteDesc;						//当前完成度

	GUIWnd*									m_pWndReceiveGift;							//领取奖励的那个~
	IconStatic*								m_pIStcCurItem[MAX_GIFT_ITEM_NUM];			//当前奖励物品图标
	GUIStatic*								m_pStcCurItemName[MAX_GIFT_ITEM_NUM];		//当前奖励物品名字
	IconStatic*								m_pIStcNextItem[MAX_GIFT_ITEM_NUM];			//下个奖励物品图标
	GUIStatic*								m_pStcNextItemName[MAX_GIFT_ITEM_NUM];		//下个奖励物品名字
	GUIButton*								m_pBtnOk;									//确定领取
	GUIButton*								m_pBtnReceiveClose;							//关闭

	GUIWnd*									m_pWndIDbuyBack;							//账号回收界面
	IconStatic*								m_pIStcBuyBackItemIcon[MAX_GIFT_ITEM_NUM];	//物品图标
	GUIStatic*								m_pStcBuyBackItemName[MAX_GIFT_ITEM_NUM];	//物品名称
	GUIButton*								m_pBtnBuyBackOk;							//确认收回账号
	GUIButton*								m_pBtnBuyBackReceiveGigt;					//领取礼品
	GUIButton*								m_pBtnBuyThink;								//再想想~
	GUIButton*								m_pBtnBuyClose;								//关闭按钮
	IconStatic*								m_pIStcBuyBackShowItem[MAX_GIFT_ITEM_SHOW_NUM];		
	GUIStatic*								m_pIStcBuyBackShowItemName[MAX_GIFT_ITEM_SHOW_NUM];
	GUIStatic*								m_pStcButBackDesc[MAX_GIFT_ITEM_SHOW_NUM];

	GUIWnd*									m_pWndHaveReceive;							//重复领取
	IconStatic*								m_pIStcHaveReceiveItemIcon[MAX_GIFT_ITEM_NUM];	//下一阶段奖励图标..
	GUIStatic*								m_pStcReceiveItemName[MAX_GIFT_ITEM_NUM];	//物品名字
	GUIButton*								m_pBtnHaveReceiveClose;						//关闭按钮
	GUIButton*								m_pBtnHaveReceiveOK;						//确定按钮
	GUIStatic*								m_pStcHaveReceiveTips;						//那个提示

	DWORD									m_dwCurLiLianExp;							//当前历练值
	INT										m_nGetGiftNum;								//领取礼包次数
	tagIDBuyBackEx*							m_IDBuyBackEx;
	DWORD									m_dwMaxExp;

	EWhichFrame								m_eOpenWhictFrame;
};