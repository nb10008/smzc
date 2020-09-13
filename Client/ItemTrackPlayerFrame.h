#pragma once
#include "gameframe.h"
#include "Item.h"

struct tagNS_TrackPlayerPos;
struct tagNS_Move2Player4Track;
struct tagNS_GetPositionAfterTrack;

class ItemTrackPlayerFrame : public GameFrame
{
	enum EItemTrackStep
	{
		EStep_Null,						//其实状态
		EStep_ShowInputMsgBox,			//输入玩家名字
		EStep_Wait,						//等待服务器消息
		EStep_ShowTransferMsgBox,		//询问玩家决定是否 传送
		EStep_Loading,					//正在传送
		EStep_RTSHowPlayerInfo,			//实时显示目标玩家的坐标
		EStep_End						//结束
	};

	typedef std::pair<tstring,tstring> MapName;

public:
	ItemTrackPlayerFrame(void);
	~ItemTrackPlayerFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual VOID Update();

	bool SetTrackItem( Item* pItem )
	{
		if( !P_VALID(pItem) )
			return false;

		if( pItem->GetItemSpecFunc() != EISF_PlayerTrack )
			return false;

		m_pItem = pItem;
		m_n64ItemID = pItem->GetItemId();
		m_TrackItemSpecVal = pItem->GetProto()->nSpecFuncVal1;

		m_step = EStep_ShowInputMsgBox;
		return true;
	};

private:

	//追踪玩家位置 消息处理
	DWORD OnNS_TrackPlayerPos( tagNS_TrackPlayerPos *pNetCmd, DWORD );

	//传送错误处理
	DWORD OnNS_TrackPlayerTransfer( tagNS_Move2Player4Track *pNetCmd, DWORD );

	// 传送之后要每隔10秒向服务器发送一个消息，获得玩家的最新位置
	// 连续两次
	DWORD OnNS_GetPositionAfterTrack( tagNS_GetPositionAfterTrack *pNetCmd, DWORD );

	//响应玩家名字输入框
	DWORD OnTryToTrackPlayer( tagMsgInputBoxEvent* pGameEvent );
	
	// 玩家使用追踪道具进行传送
	DWORD OnEvent_PlayerTrackMove2Player( tagMsgBoxEvent* pGameEvent );

private:

	/** \检查是否改提示 是否传送
		\param 如果可以传送，使用的物品指针
		\return 是否可以传送
	*/
	bool CheckTransfer( Item* & pTransferItem );
	
	/**
		\ 得到根据地图的ID,和位置 得到地图名称和区域名称
		\ return 返回地图是否是常规地图 EMT_Normal
	*/
	MapName GetMapName( DWORD dwMapId, Vector3 pos, LPBOOL pNormal );
	
	/**
		\ 检查此状态是否 能够被传送，能被传送返回 true，否则返回false
	*/
	bool CheckLPState();

	//传送到目标地图
	void GotoNewMap( DWORD dwMapID, Vector3 vecPos,Vector3 vecFaceto );

private:
	TSFPTrunk<ItemTrackPlayerFrame> m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;
	TObjRef<GameFrameMgr>	m_pFrameMgr;

	EItemTrackStep			m_step;							//追踪操作当前的阶段
	
	//使用的追踪道具的64位ID，追踪阶段表示追踪道具的ID, 传送阶段和传送后都表示具有传送功能的追踪道具的id
	//m_pItem 也一样
	INT64					m_n64ItemID;					
	Item*					m_pItem;
	INT32					m_TrackItemSpecVal;				//追踪道具的类型，能否传送，默认是不能传送
	
	TCHAR					m_szPlayerName[X_LONG_NAME];	//要追踪的玩家的名字

	INT						m_nTellPositionAfterTransfer;	//提示计数器, 传送之后要每隔10秒提示一下 目标玩家的位置,总共两次
	DWORD					m_dwTimer;						//时间计数器，隔10秒提示一次

	DWORD					m_dwTargetRoleID;				//目标玩家的ID
};
