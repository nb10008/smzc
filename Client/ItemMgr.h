#pragma once
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\msg_guild.h"
#include "..\WorldDefine\fashion_define.h"
class ItemContainer;
class Item;
class Equipment;
class NetSession;
struct tagNS_BagExtend;
struct tagNS_WareExtend;
struct tagNS_SuitNum;
struct tagNS_Abrase;
struct tagItemContainerExtend;
struct tagNS_LockItem;
struct tagNS_EquipSignature;
struct tagNS_RaiseEquipPotval;
struct tagNS_DetachOrAttackSoul_Equip;
struct tagNS_GuiYuan;
struct tagNS_FairyPowder;
struct tagNS_SyncOpenChaseRemainTime;
struct tagNS_ForceLootChase;
struct tagNS_UnlockItemNotice;
struct tagNS_PlayerRegression;
struct tagNS_SyncFashionInfo;
struct tagNS_Rehanding;
struct tagNS_DressUpAnotherRole;
struct tagNS_FairyDoRaiseIntelligence;
struct tagNS_FairyRaiseIntelligenceMax;
struct tagNS_FabaoChange;
struct tagNS_NewSoulCrystalAdd;
struct tagNS_DiamondEquipGrowIM;
struct tagNS_EquipPerfectGrow;
struct tagNS_XiPurpleStar;
struct tagNS_ShengLingCoValueChg;

#define DefaultIcon _T("data\\ui\\Common\\L_diban-b.bmp")
/** \struct tagBeGoldStoneItemTable
	\breif 点金石物品表
*/
struct tagBeGoldStoneItemTable
{
	DWORD dwSrcItemID;
	DWORD dwDestItemID;
	DWORD dwGoldStoneType;
};

/** \struct tagBaiBaoRecordC
	\breif	百宝袋单条记录客户端存储结构
*/
struct tagBaiBaoRecordC
{
	EBaiBaoRecordType	eType;
	DWORD				dwTypeID;
	DWORD				dwRoleID;
	tagDWORDTime		sTime;
	tstring				strWords;
};

/**	\class ItemMgr
	\brief 物品管理器

*/
class ItemMgr
{
public:
	enum EPocketEncryptState// 行囊密码状态
	{
		EPES_Null			=	-1,		
		EPES_EnterPassword	=	0,		// 键入行囊密码
		EPES_SetupPassword	=	1,		// 设置行囊密码
		EPES_ModifyPassword	=	2,		// 修改行囊密码
		EPES_CancelPassword	=	3		// 取消行囊密码
	};
public:
	ItemMgr();
	~ItemMgr();

	static ItemMgr* Inst();


	VOID Init();
	VOID Update();
	VOID Destroy();
		
	// 获取各个容器指针
	ItemContainer* GetPocket(){return m_pPocket;}
	ItemContainer* GetThrone(){return m_pThrone;}
	ItemContainer* GetEquipment(){return m_pEquipment;}
	ItemContainer* GetStorge(){return m_pStorge;}
	ItemContainer* GetQuest(){return m_pQuest;}
    ItemContainer* GetGuildStore(){return m_pGuildStore;}
	ItemContainer* GetClothespress(){return m_pClothespress;}
	ItemContainer* GetTempCpntainer(){return m_pTempContainer;}
	ItemContainer* GetShengLingEquip(){return m_pShengLingEquip;}
	ItemContainer* GetWarBag() {return m_pWarBag;}
	
	// 根据模板ID获取物品的图标
	tstring GetItemIconPath(DWORD dwTypeID);

	// 获取背包中的同一个TypeID物品的第一个
	Item* GetPocketFirstItemByTypeID(DWORD dwTypeID);
	// 获取背包中的物品
	Item* GetPocketItem(INT16 n16Pos);
	// 获得背包中的物品
	Item* GetPocketItemByID(INT64 n64ID);
	// 获取装备栏中的装备
	Equipment* GetCurEquip(EEquipPos ePos);
	// 获取装备栏中的装备
	Equipment* GetCurEquipByID(INT64 n64ID);
	// 获取相应容器中的物品
	Item* GetConItem(EItemConType ePos, INT16 n16Pos);
	// 获取相应容器中的物品
	Item* GetConItemByID(EItemConType ePos, INT64 n64ID);
	// 判断装备栏是否为空
	BOOL IsNotEquip(EEquipPos ePos);
	// 获取相应容器相应页第一个空位
	INT16 GetConBlank(EItemConType ePos, INT nIndex = 0);

	// 获取某套装当前件数
	INT8 GetSuitNum(DWORD dwSuitID){return m_mapSuitNum.Peek(dwSuitID);}
	// 获取某件装备龙魂能力是否激活
	bool GetLonghun(EEquipPos ePos, INT64 id);

	// 获取冷却时间
	VOID GetItemCurCDTime(DWORD dwTypeID, FLOAT& fCDCount, FLOAT& fRestoreRatio);

	// 获取相应特殊功用类型的物品ID
	INT64 GetSpecfuncItemID(EItemSpecFunc eType);
	// 获取当前主手武器类型
	EItemTypeEx GetHandWeaponType(EEquipPos ePos=EEP_RightHand);
	
	// 显示物品相关错误码
	VOID ShowItemErrorMsg(DWORD dwErrorCode);

	//显示特殊物品相关错误码，目前只限制在17到21的错误码，
	//后人要像使用请记得修改限制范围。17以前请不要使用，以免冲突。
	VOID ShowSpecItemErrorMsg(DWORD dwErrorCode);

	// 获取百宝袋记录列表
	VOID GetBaiBaoList(TList<tagBaiBaoRecordC> &listRecord);
	
	// 设置当前行囊加密的状态，方便客户端处理服务器发送的消息NS_BagPsd
	void SetPocketEncryptState(EPocketEncryptState eState){m_eEncryptState = eState;}

    // 获取帮派仓库
    void OnGetGuildWare(tagNS_GetGuildWare* pMsg);
    // 清空帮派仓库
    void ClearGuildWare();

	//brief 从背包中得到第一个特殊功能的物品
	Item* GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc );

	// 材料是否在点金石物品表中
	bool IsInGoldStoneTable( DWORD dwItemId, DWORD dwGoldStoneType );

	/** \brief 从背包中得到第一个特殊功能的物品
		\param DWORD dwItemTypeID 可能不同的物品有 同样的特殊功能，比如普通鱼饵和高级鱼饵
		\param bEnableType 开不开启第二个参数，默认是不开启的
	*/
	Item* GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc, DWORD dwItemTypeID, BOOL bEnableType = FALSE );

	//以随身仓库的形式打开仓库
	void OpenWalkWare();

	// 得到法宝的等级
	INT GetTrumpLevel( INT nDengJie );
	// 得到法宝的当前经验
	INT GetTrumpCurExp( INT nDengJie );
	// 法宝升级到下一等需要的经验值
	INT GetTrumpNextLevelExp( INT nDengJie );
	INT GetTrumpRefineExp( INT nPlayerLevel );
	INT GetTrumpMaxExp();
	VOID GetEquipTrumpLevel(INT nExp, INT &nPlayerLevel, INT &DengJie );

	/** \ 强行使物品走CD时间
	*/
	VOID FouceUpdateItemCDTime(DWORD dwItemTypeID);
	VOID FouceUpdateItemCDTime(DWORD dwItemTypeID,DWORD dwRemianSeconds);

	TMap<DWORD, tagFashionInfo*>& GetFashionMap() { return m_mapFashion; }

	// 获取衣橱中的时装
	Equipment* GetFashion( DWORD dwTypeID, BOOL bDIY = FALSE);
	// 获得衣橱中的时装
	Equipment* GetFashion(INT64 n64ID);

	VOID GetAllSuitableFashion( bool bMan, vector<DWORD> &vecFashion );

	// 获取一套时装的气质
	BYTE GetSuitMettle(DWORD dwSuitIndex);

protected:
	//--网络消息处理函数
	// 初始化物品
	DWORD NetInitLocalPlayerItem(tagNS_GetRoleInitState_Item* pNetCmd, DWORD);
	// 初始化物品冷却
	DWORD NetInitLocalPlayerItemCD(tagNS_GetRoleInitState_ItemCDTime* pNetCmd, DWORD);
	// 初始化套装
	DWORD NetInitLocalPlayerSuit(tagNS_GetRoleInitState_Suit* pNetCmd, DWORD);
	// 初始化龙魂能力
	DWORD NetInitLocalPlayerLonghun(tagNS_GetRoleInitState_Longhun* pNetCmd, DWORD);
	// 穿装备返回
	DWORD NetEquipment(tagNS_Equip* pNetCmd, DWORD);
	// 脱装备返回
	DWORD NetUnEquipment(tagNS_Unequip* pNetCmd, DWORD);
	// 换武器返回
	DWORD NetSwapWeapon(tagNS_SwapWeapon* pNetCmd, DWORD);
	// 物品位置改变，同一容器内
	BOOL NetItemPosChange(tagNS_ItemPosChange* pNetCmd, DWORD);
	// 物品位置改变，不同容器之间
	BOOL NetItemPosChangeEx(tagNS_ItemPosChangeEx* pNetCmd, DWORD);
	// 新增一个物品
	DWORD NetNewItemAdd(tagNS_NewItemAdd* pNetCmd, DWORD);
	// 新增一个装备
	DWORD NetNewEquipAdd(tagNS_NewEquipAdd* pNetCmd, DWORD);
	// 新增一个法宝
	DWORD NetNewTrumpAdd(tagNS_NewFabaoAdd* pNetCmd, DWORD);
	// 新增一个圣灵
	DWORD NetNewHolyAdd(tagNS_NewHolyAdd* pNetCmd, DWORD);
	// 新增一个生灵装备
	DWORD NetNewHolyEquipAdd(tagNS_NewHolyEquipAdd* pNetCmd, DWORD);
	// 增加一个已知物品
	DWORD NetItemAdd(tagNS_ItemAdd* pNetCmd, DWORD);
	// 删除一个物品
	DWORD NetItemRemove(tagNS_ItemRemove* pNetCmd, DWORD);
	// 绑定物品
	DWORD NetItemBind(tagNS_ItemBind* pNetCmd, DWORD);
	// 设置物品的绑定状态
	DWORD NetItemBindStatus(tagNS_ItemBindStatus* pNetCmd, DWORD);

	// 装备属性改变
	DWORD NetEquipChange(tagNS_EquipChange* pNetCmd, DWORD);
	// 装备单个属性改变
	DWORD NetEquipSingleChange(tagNS_EquipSingleChange* pMsg, DWORD pPrama);
	// 法宝属性改变
	DWORD NetTrumpChange(tagNS_FabaoChange* pNetCmd, DWORD);
	// 魂晶属性改变
	DWORD NetHunJingChange(tagNS_SoulCrystalChg* pNetCmd, DWORD);
	// 圣灵属性改变
	DWORD NetShengLingChange(tagNS_ShengLingChg* pNetCmd, DWORD);
	// 圣灵装备属性改变
	DWORD NetShengLingEquipChange(tagNS_HolyEquipChg* pNetCmd, DWORD);
	// 物品CD改变
	DWORD NetItemCDUpdate(tagNS_ItemCDUpdate* pNetCmd, DWORD);
	// 背包扩容
	DWORD NetBagExtend(tagNS_BagExtend* pNetCmd, DWORD);
	// 仓库扩容
	DWORD NetWareExtend(tagNS_WareExtend* pNetCmd, DWORD);
	// 套装件数
	DWORD NetSuitNumChange(tagNS_SuitNum* pNetCmd, DWORD);
	// 鉴定装备
	DWORD NetIdentifyEquip(tagNS_IdentifyEquip* pNetCmd, DWORD);
	// 磨石打磨
	DWORD NetAbraseWeapon(tagNS_Abrase* pNetCmd, DWORD);
    // 时装染色
    DWORD NetDyeFashion(tagNS_DyeFashion* pNetCmd, DWORD);
	// 行囊加密返回
	DWORD NetBagPassword(tagNS_BagPsd* pNetCmd, DWORD);
	// 武器崭新度变化
	DWORD NetWeaponNewnessChange(tagNS_RWNewessChange* pNetCmd, DWORD);
	// 处理自动整理
	DWORD NetContainerReorder(tagNS_ItemReorder* pNetCmd, DWORD);
	DWORD NetContainerReorderEx(tagNS_ItemReorderEx* pNetCmd, DWORD);
	// 处理装备龙魂能力出发机制
	DWORD NetEquipLonghunOn(tagNS_LongHunOn* pNetCmd, DWORD);
	DWORD NetEquipLonghunOff(tagNS_LongHunOff* pNetCmd, DWORD);
	// 处理百宝袋记录功能
	DWORD NetInitBaiBaoRecord(tagNS_InitBaiBaoRecord* pNetCmd, DWORD);
	DWORD NetSingleBaiBaoRecord(tagNS_SingleBaiBaoRecord* pNetCmd, DWORD);

	//响应打开仓库
	DWORD NetWareOpen( tagNS_WareOpen* pNetCmd, DWORD );

	DWORD NetBeGoldStone( tagNS_BeGoldStone *pNetCmd, DWORD );

	DWORD NetUpgradeMaxPot( tagNS_RaiseEquipPotval *pNetCmd, DWORD );
	// 吸魂附魂
	DWORD NetSoulChange( tagNS_DetachOrAttackSoul_Equip *pNetCmd, DWORD );
	// 归元
	DWORD NetGuiYuan( tagNS_GuiYuan *pNetCmd, DWORD );
	// 妖精之粉
	DWORD NetFairyPowder( tagNS_FairyPowder *pNetCmd, DWORD );

	DWORD OnNS_LockItem( tagNS_LockItem *pNetCmd, DWORD );

	DWORD OnNS_UnLockItem( tagNS_UnlockItemNotice *pNetCmd, DWORD );

	DWORD OnNS_EquipSignature( tagNS_EquipSignature *pNetCmd, DWORD );

	//--处理游戏事件
	// 响应背包扩容
	DWORD OnEventBagExtend(tagItemContainerExtend* pGameEvent);
	// 响应仓库扩容
	DWORD OnEventWareExtend(tagItemContainerExtend* pGameEvent);
	// 确定背包扩容
	DWORD OnEventBagExtendCheck(tagMsgBoxEvent* pGameEvent);
	// 确认仓库扩容
	DWORD OnEventWareExtendCheck(tagMsgBoxEvent* pGameEvent);
	
	DWORD OnNS_SyncOpenChaseRemainTime(tagNS_SyncOpenChaseRemainTime* pNetCmd,DWORD);

	DWORD OnNS_ForceLootChase(tagNS_ForceLootChase* pNetCmd,DWORD);

	DWORD OnNS_FairyDoRaiseIntelligence(tagNS_FairyDoRaiseIntelligence* pNetCmd,DWORD);
	DWORD OnNS_FairyRaiseIntelligenceMax(tagNS_FairyRaiseIntelligenceMax* pNetCmd,DWORD);
	
	
	// 获取相应容器类型的指针
	ItemContainer* GetContainer(EItemConType eType);
	
	// 新增一个物品;
	VOID AddNewItem(Item* pItem);

	// 新增一条百宝袋记录
	VOID AddNewBaiBaoRecord(tagBaiBaoRecord* pRecord);

	// 老玩家回归
	DWORD OnNS_PlayerRegression( tagNS_PlayerRegression* pNetCmd, DWORD dwParam );

	// 初始和更新玩家时装信息
	DWORD OnNS_UpdateFashionInfo( tagNS_SyncFashionInfo* pNetCmd, DWORD dwParam );

	// 装扮好友
	DWORD OnNS_NetDressUpAnotherRole( tagNS_DressUpAnotherRole* pNetCmd, DWORD dwParam );

	// 魂晶
	DWORD OnNS_NewSoulCrystalAdd( tagNS_NewSoulCrystalAdd* pNetCmd, DWORD dwParam );

	// 使用宝石增幅或者洗属性道具返回的消息
	DWORD OnNS_DiamondEquipGrowIM(tagNS_DiamondEquipGrowIM* pNetCmd, DWORD dwParam);

	// 完美成长
	DWORD OnNS_EquipPerfectGrow(tagNS_EquipPerfectGrow* pNetCmd, DWORD dwParam);
	// 吸星大法
	DWORD OnNS_XiPurpleStar(tagNS_XiPurpleStar* pNetCmd, DWORD dwParam);
	// 更新圣灵默契值
	DWORD OnNS_ShengLingCoValueChg(tagNS_ShengLingCoValueChg* pNetCmd, DWORD);

private:
	void LoadBeGoldStoneItemTable();
	void LoadTrumpLevelTable();

private:
	TSFPTrunk<ItemMgr>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	TObjRef<NetCmdMgr>		m_pCmdMgr;
	TObjRef<NetSession>		m_pSession;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<VarContainer>	m_pVarContainer;

	ItemContainer*			m_pPocket;		// 背包
	ItemContainer*			m_pStorge;		// 仓库
	ItemContainer*			m_pEquipment;	// 装备栏
	ItemContainer*			m_pThrone;		// 百宝袋
	ItemContainer*			m_pQuest;		// 任务物品栏
    ItemContainer*          m_pGuildStore;  // 帮派仓库
	ItemContainer*          m_pClothespress;  // 衣橱
	ItemContainer*			m_pTempContainer;	//临时容器，妖精熔炼结果保存
	ItemContainer*			m_pShengLingEquip;	//圣灵装备容器
	ItemContainer*			m_pWarBag;			//战场背包

	TMap<DWORD, DWORD>		m_mapCurStartTime;		// 冷却时间开始表
	TMap<DWORD, INT8>		m_mapSuitNum;			// 套装件数
	TMap<DWORD, tagFashionInfo*>	m_mapFashion;		// 所有时装的信息
	TMap<INT16, INT8>		m_mapLonghunOn;			// 龙魂激活与否
	bool					m_bUseSilverExtend;		// 使用金钱扩充标志位
	bool					m_bNotEnoughYuanbao;	// 元宝不足标志位
	bool					m_bNotEnoughSilver;		// 金钱不足标志位
	bool					m_bNotInitBaiBaoRecord;	// 百宝袋更新标志位
	TList<tagBaiBaoRecordC>	m_listBaiBaoRecord;		// 百宝袋记录列表
	vector<tagBeGoldStoneItemTable> m_vecBeGoldStoneItemTable;	// 点金石物品表
	vector<tagFabaoLevelProto> m_vecTrumpLevel;

	EPocketEncryptState		m_eEncryptState;		// 行囊密码当前接收状态
};
