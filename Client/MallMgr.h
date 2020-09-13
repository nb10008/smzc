#pragma once
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\msg_mall.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\easybuy.h"
/** class MallMgr
	brief 商城管理器
*/

struct tagStartNPCTalkEvent;
class NetSession;
class MallFrame;
class YuanbaoFrame;
class GroupBuyFrame;
class LevelPromoteFrame;
struct tagNS_SyncMallOfferProto;

//1元计划
struct tagOpenLevelPromoteFrame : public tagGameEvent
{
	INT nIndex;
	tagOpenLevelPromoteFrame(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		,nIndex(GT_INVALID)
	{} 
};

class MallMgr
{
public:
	typedef std::vector<const tagMallOfferProto2Client*> VecGiftPlan;

public:
	MallMgr(void);
	~MallMgr(void);
	static MallMgr* Inst();

	VOID Init();
	VOID Update();
	VOID Destroy();

	//--处理网络消息
	DWORD OnNS_OpenMall(tagNS_OpenMall* pMsg, DWORD dwParam);
	DWORD OnNS_MallItem(tagNS_MallItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallFreeItem(tagNS_MallFreeItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallPack(tagNS_MallPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdateItem(tagNS_MallUpdateItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdatePack(tagNS_MallUpdatePack* pMsg, DWORD dwParam);
	DWORD OnNS_MallUpdate(tagNS_MallUpdate* pMsg, DWORD dwParam);
	DWORD OnNS_GetSimGPInfo(tagNS_GetSimGPInfo* pMsg, DWORD dwParam);
	//DWORD OnNS_OpenWareEasyBuyFrame(tagNS_WareOpen* pMsg, DWORD dwParam);
	DWORD OnNS_GetPromoteData(tagNS_MallRecommendingItem* pMsg, DWORD dwParam);

	//--处理游戏事件
	DWORD OnOpen_Mall(tagGameEvent* pEvent);
	DWORD OnClose_Mall(tagGameEvent* pEvent);
	DWORD OnOpen_TaskEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_PackageEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_SocialEasyBuyFrame(tagGameEvent* pEvent);
	//DWORD OnOpen_WareEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_ComposeEasyBuyFrame(tagStartNPCTalkEvent* pEvent);
	DWORD OnOpen_PetEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_RoleSkillEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnOpen_RoleInfoEasyBuyFrame(tagGameEvent* pEvent);
	DWORD OnEventEnterWar(tagGameEvent* pEvent);
	DWORD OnEnterNormalMap(tagGameEvent* pEvent);

	//--
	TMap<DWORD, tagMallItemProto*>& GetMallProto()	{ return m_mapMallItem; }
	TMap<DWORD, tagMallPackProto*>& GetPackItem()	{ return m_mapPackItem; }
	list<tagMallPackProto*> &GetMallPack() { return m_listPack; }
	tagMallFreeItem* GetMallFreeItem()				{ return &m_MallFreeItem; }
	tagMallItemProto* GetMallProItem(int nIndex);
	map<INT64, tagGroupPurchase>& GetGroupBuyInfo() { return m_mapGgroupPur; }
	vector<tagUIItem> &GetEasyBuyItem() { return m_vecUIItem; }

	//--
	INT GetMallProtoSize() { return m_mapMallItem.Size(); }
	INT GetPackProtoSize() { return m_mapPackItem.Size(); } 
	INT GetGroupBuyInfoSize() { return m_mapGgroupPur.size(); }

	//--
	tagMallItemProto* GetMallItemByID(DWORD dwID);
	tagMallPackProto* GetMallPackByID(DWORD dwID);
	tagMallItemProto* GetMallItemByTypeid(DWORD dwTypeID);
	tagGroupPurchase* GetGroupBuyInfoByID(INT64 dwID);
	void DeleGroupBuyInfoByID(INT64 dwID);
	void RefreshGroupBuyInfoByID(INT64 dwID, INT nNum);


	//--免费领取的时间
	DWORD GetFreeGetTime()const { return m_dwFreeGetTime; }
	void SetFreeGetTime(DWORD dwTime) { m_dwFreeGetTime = dwTime; } 

	//--
	void ClearMallItem();
	void ClearMallPack();
	void ClearGroupBuyInfo();

	//商城frame
	MallFrame* GetMallFrame() { return m_pMallFrame; }

	//推荐商品
	void LoadPromoteData(); //读取推荐商品数据XML
	map<INT, tagMallItemPrototeGood>& GetPromoteItem()	{ return m_mapMallPromoteItem; }
	VOID PromoteItemShowMgr();//根据人物等级筛选所要显示的推荐商品
	map<DWORD, tagMallPromoteItemShow>&	GetProItemShow() { return m_mapMallPromoteItemShow; }    

	
	//--团购
	void CreateGroupBuyFrame(LPCTSTR szFatherName);
	GroupBuyFrame* GetGroupBuyFrame() { return m_pGroupBuyFrame; }
	void SendGetGroupBuyInfo();
	INT GetGroupPurExp()const { return m_nGroupPurExp; }
	void SetGroupPurExp(INT32 exp)  { m_nGroupPurExp = exp; }

	//--赠送窗品
	void CreateMallGiftFrame();
	void SetMallGiftFrameMaxBuyNum(INT nMaxBuyNum);
	void InitMallGiftFrameNum();
    //! 元宝交易
    void CreateYuanBaoFrame(LPCTSTR szFatherName);

	// 装备栏和换装属性换算接口
	INT16 GetEquipDisplayPos(INT16 n16EquipPos);

	//--显示tips用到的时间计算
	DWORD GetDay(DWORD& dwTimeDiff)
	{
		return dwTimeDiff / dwOneDay;
	}
	DWORD GetHour(DWORD& dwTimeDiff, DWORD& dwDay)
	{
		return (dwTimeDiff - (dwDay*dwOneDay)) / dwOneHour;
	}
	DWORD GetMin(DWORD& dwTimeDiff, DWORD& dwDay, DWORD& dwHour)
	{
		return (dwTimeDiff - (dwHour*dwOneHour) - (dwDay*dwOneDay)) / 60;
	}

	VOID OpenEasyBuyFrame( LPCTSTR szFather, LPCTSTR szFrame, LPCSTR szAlign, EasyBuyUIType enUIType, LPCSTR alignType, LPCSTR szControlName/* = NULL*/ );
	//获得推荐商品最大页数
	INT GetMaxPromotePage(){ return m_nMaxPromotePage;}


	//------------------1元计划------------------
	std::vector<DWORD>& GetGiftPlanList(INT level);
	
	VOID  SetIndexForClint( INT index , BOOL b );
	//查询是否已点击过（不闪烁），从0开始计数
	BOOL  InquireIndexForClint( INT index );

	VOID  SetIndexForBuy( INT index ,BOOL b);
	//查询是否已买购过（不能买），从0开始计数
	BOOL  InquireIndexForBuy( INT index);

	BOOL  CreateLevelPromote( );
	//---------------------------------------------


private:

	void LoadEasyBuyItem();
	//--------1元计划----------
	BOOL LoadPlanFile();
	VOID ClearGiftPlanMap();
	DWORD OnNS_SyncMallOfferProto(tagNS_SyncMallOfferProto* pMsg, DWORD dwParam);

	DWORD InitMallOfferProto(tagNS_SyncMallOfferProto* pMsg, DWORD dwParam);
private:
	TSFPTrunk<MallMgr>					m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<VarContainer>				m_pVarContainer;
		
	MallFrame*							m_pMallFrame;
    YuanbaoFrame*                       m_pYuanbaoFrame;
	GroupBuyFrame*						m_pGroupBuyFrame;

	map<DWORD, tagMallItemProto>		m_mapMallItemEx;			//全部非打包商品
	map<DWORD, tagMallPackProto>		m_mapPackItemEx;			//全部打包商品

	TMap<DWORD, tagMallItemProto*>		m_mapMallItem;			
	TMap<DWORD, tagMallPackProto*>		m_mapPackItem;
	list<tagMallPackProto*>				m_listPack;
	list<tagMallItemProto*>				m_listItem;
	tagMallItemProto*					m_pProItem[MAX_PromoteItem];//热门商品

	map<INT, tagMallItemPrototeGood>	m_mapMallPromoteItem;			//读取出相应推荐商品
	tagMallPromoteItemShow*				m_pPromoteItemShow;               
	map<DWORD, tagMallPromoteItemShow>	m_mapMallPromoteItemShow;       //用于推荐商品的显示

	tagMallFreeItem						m_MallFreeItem;				//免费领取的商品
	DWORD								m_dwFreeGetTime;			//免费领取的时间
	DWORD								m_dwMallTime;				//商城时间	
	DWORD								m_dwGroupBuyTime;			//团购时间
	BOOL								m_bOpen;

	//--以下为团购
	map<INT64, tagGroupPurchase>		m_mapGgroupPur;				//团购的数据(以商城商品TypdID为Key)
	INT									m_nGroupPurExp;				//帮派团购指数
	vector<tagUIItem>					m_vecUIItem;				// 各界面所挂载的商品

	static const DWORD					dwOneDay  = 24 * 60 * 60;
	static const DWORD					dwOneHour = 60 * 60;

	INT									m_nMaxPromotePage;

	//1元计划
	VecGiftPlan							m_vecGiftPlan;
	BOOL								m_bLevelBtnGlint[LEVEL_NUM_CHOOSE];
	BOOL								m_bLevelCanBuy[LEVEL_NUM_CHOOSE];
	std::vector<DWORD>					m_vecGiftPlanByLevel;
	
	LevelPromoteFrame*					m_pLevelPromote;

	BOOL								m_bCanUseBag;
};