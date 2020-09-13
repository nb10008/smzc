#pragma once
#include "AvatarAniMixer.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\ItemDefine.h"

class Item;
struct tagNS_SoulEquip;
struct tagNS_UnSoulEquip;
struct tagNS_GetSoulInfo;
struct tagNS_ActivateHolySoul;
struct tagNS_HolySoulAttChange;
struct tagStarEnhanceProto;
struct tagNS_UpdateHolySoulExp;

struct tagHolyModelInfo
{
	DWORD	dwHolyManID;
	DWORD	dwSoulValue;
	tstring		strModelMen;
	tstring		strModelWomen;
	tstring		strModelEffect;
	INT			nModelScale;
};

struct tagUpdateHolyManEvent : public tagGameEvent
{
	//为false表明全部刷新
	EHolyEquipAttType eAttType;  //只刷新元神值
	tagUpdateHolyManEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) 
		: tagGameEvent(szEventName,pSenderFrame)
		,eAttType(EHEAT_NULL)
	{}
};

enum EHolyEquipType		//元神装备 type ex
{
	EHET_NULL				= 0,				//无类型
	EHET_TIANHUN		= 42,			//天魂
	EHET_DIHUN				= 43,			//地魂
	EHET_MINGHUN		= 44,			//命魂
	EHET_TIANCHONG	= 45,			//天冲
	EHET_LINGHUI			= 46,			//灵慧
	EHET_QIPO				= 47,			//气魄
	EHET_LIPO				= 48,			//力魄
	EHET_ZHONGSHU		= 49,			//中枢
	EHET_JINGPO			= 50,			//精魄
	EHET_YINGPO			= 51,			//英魄
};

//本地玩家元神管理
class HolyManMgr
{
public:
	HolyManMgr(void);
	~HolyManMgr(void);

	enum
	{ 
		HolyMan_SkillID1			=	99905,		//元神召唤技能 id  LV1
		//HolyMan_SkillID2			=	11126,		//元神召唤技能 id	LV2
		//HolyMan_SkillID3			=	11127,		//元神召唤技能 id	LV3
	};

	enum HolyCombatStateCheckType //战斗状态不能操作类型
	{
		EHCSCT_EQUIP		= 0,		//换装
		EHCSCT_AFFUSE	= 1,		//灌注
		EHCSCT_REBORN	= 2,		//转生
	};

	static HolyManMgr* Inst();
	void Init();
	void Destroy();

	//本地玩家是否元神觉醒
	BOOL IsHolyAwake(); 

	VOID CheckAwakeState();

	VOID AddSoulExp(INT64 AddValue);

	tstring GetMatchCombatAction(tstring strAct);

	EHolyEquipType IndexToType(INT nIndex);
	INT TypeToIndex( EHolyEquipType eType);

	//判断一个装备是否为元神装备
	BOOL IsHolyManEquip(DWORD dwTypeID);
	//获得元神装备加成类型名
	tstring GetHolyEquipAttTypeName(DWORD dwTypeID);
	//获得元神装备加成类型
	EHolyEquipAttType GetHolyEquipAttType(DWORD dwTypeID);
	//获得元神装备加成类型
	INT	GetHolyEquipAttValue(DWORD dwTypeID);
	//装备元神装备位置
	EHolyEquipType GetHolyManEquipPos(DWORD dwTypeID);
	//战斗状态检查
	BOOL IsCombatState( HolyCombatStateCheckType eType = EHCSCT_EQUIP );
	//穿装备  ePos是元神装备栏位置 n16Pos是背包位置
	VOID SendMsg_HolyEquip(EHolyEquipPos ePos, INT16 n16Pos);
	//脱装备  ePos是元神装备栏位置 n16Pos是背包位置
	VOID SendMsg_UnHolyEquip(EHolyEquipPos ePos,INT16 n16Pos=GT_INVALID);
	//刷新元神属性值信息
	VOID UpdateSoulAttValue( EHolyEquipAttType eType = EHEAT_NULL );

	//由 性别 得到对应的元神模型信息
	tstring GetHolyManSkelPath(DWORD dwModelID,BYTE  bySex);
	tstring GetHolyManSkelName(DWORD dwModelID,BYTE  bySex);
	float	  GetModelScale(DWORD dwHolyManID);

	VOID	ShowErrorTips(DWORD dwErr);

	VOID	ActiveHolyMan(){ m_bHolyManActive = TRUE;}
	BOOL	IsActiveHolyMan() { return m_bHolyManActive;}

	//元神属性值
	VOID SetHolyManAtt(const tagHolySoulInfo info );
	const tagHolySoulInfo&	GetHolyManAtt(){ return m_SoulAttInfo; }
	//获取元神值
	INT GetHolyManValue();
	//打开元神信息界面
	VOID OpenHolyManFrame(BOOL bOpen = TRUE);

	BOOL HasUpdate(){ return  m_bIsUpdateExp;}
	VOID  NoUpdate(){ m_bIsUpdateExp = FALSE; }

	//元神装备强化,参数为装备冲星等级
	const tagStarEnhanceProto*	GetHolyComposeInfo(BYTE byLv);
	INT	GetStarEnhanceAttValue(BYTE byLv, EHolyEquipAttType eAttType);

	//通过转生道具(老君仙露)ID 获得转生概率 
	INT  GetRateByItemNum(INT nNum);
	//获得当前转生等级达到100概率转生所需最少数量
	INT  GetFullRateMinNum();
	//获得转生等级
	INT  GetRebornLevel();
	//获得元神等级
	INT  GetHolyManLevel();
	//更新item
	VOID UpdateItem(Item **pItem);
	//是否是元神觉醒技能
	BOOL IsHolyAwakeSkillID( DWORD dwID);
	//是否需要获得元神初始化信息
	VOID SetHolyInitState( BOOL b){ m_bLoginInit =b; }
	//开凿孔位消耗
	INT GetChiselCost(BYTE index);
	//开凿孔位几率
	INT GetChiselRate(BYTE index);
	//镶嵌孔位消耗
	INT GetInlayCost(BYTE index);
	//镶嵌孔位几率
	INT GetInlayRate(BYTE index);

private:
	BOOL LoadFromFile();

	DWORD OnNS_SoulEquip( tagNS_SoulEquip *pNetCmd, DWORD );
	DWORD OnNS_UnSoulEquip( tagNS_UnSoulEquip *pNetCmd, DWORD );
	DWORD OnNS_GetSoulInfo( tagNS_GetSoulInfo *pNetCmd, DWORD );
	DWORD OnNS_ActivateHolySoul( tagNS_ActivateHolySoul *pNetCmd, DWORD );
	DWORD OnNS_HolySoulAttChange( tagNS_HolySoulAttChange *pNetCmd, DWORD );
	DWORD OnNS_UpdateHolySoulExp(tagNS_UpdateHolySoulExp* pMsg, DWORD pPrama);

	VOID ClearHolyManInfoMap();
	VOID ClearHolyComposeMap();
	VOID HolyManMgr::ClearHolyRebornMap()
	{
		m_mapHolyRebornRate.clear();
	}
	VOID HolyManMgr::ClearHolyChiselMap()
	{
		m_mapHolyChisel.clear();
	}

	VOID HolyManMgr::ClearHolyInlayMap()
	{
		m_mapHolyInlay.clear();
	}


private:

	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TSFPTrunk<HolyManMgr>		m_trunk;

	BOOL										m_bHolyAwake;  // local player 的元神状态是否觉醒

	//角色动作与原神动作映射
	std::map<tstring,tstring>			m_mapActionSyn; 

	std::map<DWORD,const tagHolyModelInfo*>  m_mapHolyManInfo;

	BOOL										m_bHolyManActive; //15级以后激活元神

	tagHolySoulInfo		m_SoulAttInfo;

	BOOL								m_bIsUpdateExp; //是否已经刷新经验计算

	BOOL								m_bLoginInit;//是否是上线初始化

	std::map<DWORD,const tagStarEnhanceProto*> m_mapHolyCompose;

	std::map<INT, tagHolySoulReborn>			m_mapHolyRebornRate;

	std::map<BYTE, tagHolySoulEquipHole>			m_mapHolyChisel;

	std::map<BYTE, tagHolySoulEquipInlay>			m_mapHolyInlay;


};
