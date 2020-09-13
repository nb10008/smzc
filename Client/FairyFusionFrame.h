#pragma once
#include "ComposeStatic.h"
#include "ActiveProgress.h"
#include "Item.h"
#include "IconStatic.h"

struct tagStartNPCTalkEvent;
struct tagItemPosChangeExEvent;
struct tagNS_FairyFusion;
struct tagNS_FairyFusionResult;
struct tagNS_GetFusionFairy;
struct tagUpdateLPItemEvent;

class FairyFusionFrame :
	public GameFrame
{
private:

public:
	FairyFusionFrame();
	~FairyFusionFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	VOID RefreshRetUI();
	VOID ClearData();

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnStartNPCTalkEvent(tagStartNPCTalkEvent* pGameEvent);
	DWORD OnEventProduceAddItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnOpenFairyFusion(tagGameEvent* pEvent);
	DWORD OnBeginFusionEvent(tagMsgBoxEvent* pEvent);
	DWORD OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pEvent);

	DWORD OnNS_FairyFusion(tagNS_FairyFusion* pMsg, LPVOID pPrama);
	DWORD OnNS_GetFusionFairy(tagNS_GetFusionFairy* pMsg, LPVOID pPrama);

	VOID SendGetFairyMsg(bool isAutoSend);

	BYTE GetFushionScoreByQuality(BYTE byQuality)
	{
		if(byQuality == 6) 			//粉色
			return 100; 
		else if(byQuality == 5) 	//紫色
			return 50; 
		else if(byQuality == 4)		//橙色
			return 15;

		return 0;
	}

	//DWORD OnNS_GetKeyCodeGift(tagNS_GetKeyCodeGift* pMsg, DWORD pPrama);
private:
	TSFPTrunk<FairyFusionFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	ComposeStatic*				m_pFirstFairyIcon;
	ComposeStatic*				m_pSecondFairyIcon;
	ComposeStatic*				m_pIMIcon;
	GUIButton*					m_pBtnClose;
	GUIButton*					m_pBtnFusion;
	GUIStatic*					m_pStcTip;			//熔炼前的提示信息
	GUIStatic*					m_pStcRetTip;		//熔炼时几率tip
	GUIStatic*					m_pStcPinkRet;		//粉色成功率
	GUIStatic*					m_pStcPurpleRet;	//紫色成功率
	GUIStatic*					m_pStcOrangeRet;	//橙色成功率
	GUIStatic*					m_pStcIMNum;		//道具数量
	GUIButton*					m_pBtnUp;			//增加
	GUIButton*					m_pBtnDown;			//减少
	ActiveProgress*				m_pProgress;		//进度条

	DWORD						m_escapeTime;
	INT64						m_n64FirstID;
	INT64						m_n64SecondID;
	INT64						m_n64IMID;
	BYTE						m_byIMNum;
	BOOL						m_bSendMsg;

	Item*						m_pLockFirstItem;	//锁定的两只妖精的指针
	Item*						m_pLockSecondItem;
	Item*						m_pLockIMItem;		//锁定IM道具


	GUIWnd*						m_pWndGetFairy;		//获取妖精面板
	IconStatic*					m_pIStcFirstFairy;	//第一只妖精
	IconStatic*					m_pIStcSecondFairy;	//第二只妖精
	IconStatic*					m_pIStcFirstFairyPic;//底图
	IconStatic*					m_pIStcSecondFairyPic;//底图
	GUIButton*					m_pBtnRecive;		//领取妖精
	GUIButton*					m_pBtnCloseGetWnd;	//关闭按钮

	BOOL						m_bChoseFirst;		//是否选主妖精
	Equipment*					m_pResultFairy;		//生成的妖精
	INT64						m_n64TmepFirstFairy;//主妖精id缓存

};

#define NULLIconPic _T("data\\ui\\Common\\L_diban-b.bmp")
//默认底图
#define DefaultIconPic _T("data\\ui\\common\\l_icon.bmp")
//选中底图
#define SelectIconPic _T("data\\ui\\common\\l_icon-l")


