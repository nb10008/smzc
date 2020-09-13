#pragma once
#include "..\WorldDefine\msg_digbao.h"

class IconStatic;
class ItemButton;
class TreasureChestFrame : 
	public GameFrame
{	
public:
	TreasureChestFrame( void );
	virtual ~TreasureChestFrame( void );
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	bool CanUseAnother(){return  m_eOperateState == EOperateState_NULL ;}

public:
	static const int					sci_ItemNum = 16;	

private:
	BOOL EventHandler(tagGUIEvent* pEvent);

	// 初始化宝箱界面
	void InitTreasureChestUI();
	void RefreshUI();
	void SetTreasureChestUI();

	//--游戏事件
	DWORD OnEOSAaginEvent(tagMsgBoxEvent* pGameEvent);

	//--网络消息
	DWORD OnNS_StartTreasureChest(tagNS_StartTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_TreasureChest(tagNS_TreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_StopTreasureChest(tagNS_StopTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_AgainTreasureChest(tagNS_AgainTreasureChest* pMsg, DWORD dwParam);
	DWORD OnNS_ChestGetItem(tagNS_ChestGetItem* pMsg, DWORD dwParam);

	//--操作状态
	enum EOperateState
	{
		EOperateState_NULL		= 0,
		EOperateState_Begin,
		EOperateState_Stop,
		EOperateState_Again,
	};
	void SetOperateState( EOperateState eState );
	void SetCaption(DWORD dwChestTypeID);
	void SendStopTreasureMsg();
	void SendGetTreasureMsg();

private:
	
	TSFPTrunk<TreasureChestFrame>		m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<GameFrameMgr>				m_pFrameMgr;

	GUIWnd*								m_pWnd;
	GUIStatic*							m_pPatWaption;
	GUIButton*							m_pBtnMFunction;
	//GUIButton*							m_pBtnAgain;
	GUIButton*							m_pBtnGetItem;
	IconStatic*							m_pStcAuto[sci_ItemNum];
	ItemButton*							m_pStcItem[sci_ItemNum];
	IconStatic*							m_pLastAuto;
	GUIStatic*							m_pStcFinalBack;
	ItemButton*							m_pStcFinalItem;

	EOperateState						m_eOperateState;				//操作状态
	DWORD								m_dwRunTime;
	DWORD								m_dwStartTime;					//开始时间(1分钟后停止)
	DWORD								m_dwStopTime;					//停止的时间
	
	DWORD								m_dwItemTypeID;					//获得的物品
	INT									m_nItemCount;					//获得物品个数
	INT									m_nCurPos;						// 当前选中的物品位置
	INT									m_nFinalPos;					// 最终应该选中的物品位置
	BOOL								m_bCircleAgain;					// 是否需要再转一圈

	struct tagTreasure
	{
		DWORD dwTypeId;
		INT nNum;
		BYTE byQuality;
		tagTreasure()
		{
			dwTypeId = GT_INVALID;
			nNum = 0;
			byQuality = 0;
		}
	};
	//INT									m_dwStopCount;					//停止记数
	vector<tagTreasure>					m_vecItem;						//宝箱里的物品
	INT64								m_dwChestID;					// 宝箱ID
	
};