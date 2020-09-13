#include "stdafx.h"
#include "ItemProtoData.h"


ItemProtoData::ItemProtoData()
{
}

ItemProtoData::~ItemProtoData()
{
	map<DWORD, tagItemProto>::iterator iterItem;
	for(iterItem = m_mapItemProto.begin(); iterItem != m_mapItemProto.end(); ++iterItem)
	{
		tagItemProto item = iterItem->second;
		if(P_VALID(item.pDisplayInfo))
		{
			SAFE_DEL(item.pDisplayInfo);
		}
	}
	m_mapItemProto.clear();
	
	map<DWORD, tagEquipProto>::iterator iterEquip;
	for(iterEquip = m_mapEquipProto.begin(); iterEquip != m_mapEquipProto.end(); ++iterEquip)
	{
		tagEquipProto equip = iterEquip->second;
		if(P_VALID(equip.pDisplayInfo))
		{
			SAFE_DEL(equip.pDisplayInfo);
		}
	}
	m_mapEquipProto.clear();

	map<DWORD, tagFabaoProto>::iterator iterTrump;
	for(iterTrump = m_mapTrumpProto.begin(); iterTrump != m_mapTrumpProto.end(); ++iterTrump)
	{
		tagFabaoProto trump = iterTrump->second;
		if(P_VALID(trump.pDisplayInfo))
		{
			trump.pDisplayInfo = NULL;
		}
	}
	m_mapTrumpProto.clear();
}

ItemProtoData g_itemproto;
ItemProtoData* ItemProtoData::Inst()
{
	return &g_itemproto;
}

void ItemProtoData::LoadFromFile()
{
	
	LPCTSTR szItemProto = _T("data\\system\\attdata\\item_proto.xml");
	tstring strPath1 = g_strLocalPath + _T("\\attdata\\item_name.xml");
	LPCTSTR szItemDisplay = strPath1.c_str(); 
	LPCTSTR szEquipProto = _T("data\\system\\attdata\\equip_proto.xml");
	tstring strPath2 = g_strLocalPath + _T("\\attdata\\equip_name.xml");
	LPCTSTR szEquipDisplay = strPath2.c_str();
	LPCTSTR szSuitProto = _T("data\\system\\attdata\\equip_suit.xml");
	tstring strPath3 = g_strLocalPath + _T("\\attdata\\equip_suit_name.xml");
	LPCTSTR szSuitDisplay = strPath3.c_str();

	LPCTSTR szFairyFruit = _T("data\\system\\attdata\\fabaofruit_proto.xml");
	LPCTSTR szTrumpProto = _T("data\\system\\attdata\\MagicWeapon.xml");
	LPCTSTR szTreasureProto = _T("data\\system\\attdata\\clantreasure_proto.xml");
    LPCTSTR szPetEquipProto = _T("data\\system\\attdata\\pet_equip_proto.xml");
	LPCTSTR szEquipQlty = _T("data\\system\\attdata\\equip_qlty_effect.xml");
	//zxzhang  读取装备鉴定几率的原始属性表
	LPCTSTR szEquipQltyPct = _T("data\\system\\attdata\\equip_qlty_pct.xml");

    LPCTSTR szSpinProto = _T("data\\system\\attdata\\spin_proto.xml");
    LPCTSTR szSpinRate = _T("data\\system\\attdata\\spin_rate_proto.xml");

	tstring strPath4 = g_strLocalPath + _T("\\attdata\\wuxing_name.xml");
	LPCTSTR szTrumpWuXingAward = strPath4.c_str(); 

	LPCTSTR szFairySkills = _T("data\\local\\zhCN\\attdata\\stunt_name.xml");

	LPCTSTR szFairyExpBall = _T("data\\system\\attdata\\expball.xml");

	LPCTSTR szIDBuyBack = _T("data\\system\\attdata\\Idbuyback.xml");
	tstring strPath5 = g_strLocalPath + _T("\\attdata\\Idbuybackex_name.xml");
	LPCTSTR szIDBuyBackEx = strPath5.c_str();

	LPCTSTR szEquipGrow = _T("data\\system\\attdata\\equip_levelup.xml");
	
	LPCTSTR szFairyUpgrade = _T("data\\system\\attdata\\fairy_upgrade.xml");//add by xp 妖精提升次数表

	LPCTSTR szFairy_smelting = _T("data\\system\\attdata\\Fairy_smelting.xml");

	LPCTSTR szShengLing = _T("data\\system\\attdata\\HolyGods_proto.xml");
	tstring strPath6 = g_strLocalPath + _T("\\attdata\\HolyGods_name.xml");
	LPCTSTR szShengLingEx = strPath6.c_str();

	LPCTSTR szShengLingLeveup = _T("data\\system\\attdata\\HolyGods_growup.xml");
	LPCTSTR szHolyEquipProto = _T("data\\system\\attdata\\holy_equip_proto.xml");
	LPCTSTR szHolyEquipEnhance = _T("data\\system\\attdata\\Holy_empowerment.xml");

	LPCTSTR szWarReward = _T("data\\system\\attdata\\Rewards_point.xml");

	list<tstring> ItemProtoFieldList;
	list<tstring> ItemDisplayFieldList;
	list<tstring> EquipProtoFieldList;
	list<tstring> EquipDisplayFieldList;
	list<tstring> SuitProtoFieldList;
	list<tstring> SuitDisplayFieldList;

	list<tstring> TrumpProtoFieldList;
	list<tstring> TreasureProtoFieldList;
    list<tstring> PetEquipProtoList;
	list<tstring> EquipQltyList;
	list<tstring> EquipQltyPctList;
    list<tstring> SpinProtoList;
    list<tstring> SpinRateList;
	list<tstring> TrumpWuxingAwardList;
	list<tstring> FairyFruitList;
	list<tstring> FairySkillsList;
	list<tstring> FairyExpBallList;
	list<tstring> IDButBackList;
	list<tstring> IDBuyBackListEx;
	list<tstring> EquipGrowList;
	list<tstring>::iterator iter;
	list<tstring> FairyAddtimesList;//add by xp 妖精增加次数列表
	list<tstring> FairySmeltingList;	//妖精熔炼
	list<tstring> ShengLingList;	//圣灵属性表
	list<tstring> ShengLingListEx;
	list<tstring> ShengLingLevelUpList;	//圣灵升级表
	list<tstring> HolyEquipProtoList;	//圣灵装备表
	list<tstring> HolyEquipEnhanceList;	//圣灵强化道具表
	list<tstring> WarRewardList;		//战场奖励表
	
	//---------------------------------------------------------------------------------
	//读取item_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarItemProto = CreateObj("ItemProto", "VarContainer");
	if(!VarItemProto->Load("VFS_System", szItemProto, "id", &ItemProtoFieldList))
		IMSG(_T("Load file item_proto.xml failed\r\n"));

	for(iter = ItemProtoFieldList.begin(); iter != ItemProtoFieldList.end(); ++iter)
	{
		tagItemProto item;
		item.dwTypeID			 =						VarItemProto->GetDword( _T("id"),			(*iter).c_str(), GT_INVALID );
		item.eType				 =	(EItemType)			VarItemProto->GetDword( _T("Type"),			(*iter).c_str(), EIT_Null );
		item.eTypeEx			 =	(EItemTypeEx)		VarItemProto->GetDword( _T("TypeEx"),		(*iter).c_str(), EITE_Null );
		item.eTypeReserved		 =	(EItemTypeReserved)	VarItemProto->GetDword( _T("TypeReserved"),	(*iter).c_str(), EITR_Null );
		item.eStuffType			 =	(EStuffType)		VarItemProto->GetDword( _T("StuffType"),	(*iter).c_str(), EST_Null );
		item.eSpecFunc			 =	(EItemSpecFunc)		VarItemProto->GetDword( _T("SpecFunc"),		(*iter).c_str(), EISF_Null );
		item.nSpecFuncVal1		 =	(INT)				VarItemProto->GetDword( _T("SpecFuncVal1"),	(*iter).c_str(), GT_INVALID );
		item.nSpecFuncVal2		 =	(INT)				VarItemProto->GetDword( _T("SpecFuncVal2"),	(*iter).c_str(), GT_INVALID );
		item.byLevel			 =	(BYTE)				VarItemProto->GetDword( _T("Level"),		(*iter).c_str(), 0 );
		item.byQuality			 =	(BYTE)				VarItemProto->GetDword( _T("Quality"),		(*iter).c_str(), 0 );
		item.byBindType			 =	(BYTE)				VarItemProto->GetDword( _T("BindType"),		(*iter).c_str(), 0 );
		item.dwQuestID			 =						VarItemProto->GetDword( _T("QuestID"),		(*iter).c_str(), GT_INVALID );
		item.nBasePrice			 =	(INT)				VarItemProto->GetDword( _T("BasePrice"),	(*iter).c_str(), 0 );
		item.dwTimeLimit		 =						VarItemProto->GetDword( _T("TimeLimit"),	(*iter).c_str(), GT_INVALID );
		item.nMaxUseTimes		 =	(INT)				VarItemProto->GetDword( _T("MaxUseTimes"),	(*iter).c_str(), GT_INVALID );
		item.n16MaxLapNum		 =	(INT16)				VarItemProto->GetDword( _T("MaxLapNum"),	(*iter).c_str(), GT_INVALID	);
		item.n16MaxHoldNum		 =	(INT16)				VarItemProto->GetDword( _T("MaxHoldNum"),	(*iter).c_str(), GT_INVALID );
		item.n16Enmity			 =	(INT16)				VarItemProto->GetDword( _T("Enmity"),		(*iter).c_str(), GT_INVALID );
		item.bNeedBroadcast		 =	(BOOL)				VarItemProto->GetDword( _T("NeedBroadcast"),(*iter).c_str(), false ) ? true : false;
		item.bCanSell			 =	(BOOL)				VarItemProto->GetDword( _T("CanSell"),		(*iter).c_str(), false ) ? true : false;
		item.eClanRepute		 =	(EReputationType)	VarItemProto->GetDword( _T("ZoneRepType"),	(*iter).c_str(), ERT_NULL );
		item.nClanReputeVal		 =	(INT32)				VarItemProto->GetDword( _T("ZoneRepVal"),	(*iter).c_str(), 0 );
		item.eOtherClanRepute	 =	(EReputationType)	VarItemProto->GetDword( _T("OtherRepType"),	(*iter).c_str(), ERT_NULL );
		item.nOtherClanReputeVal =	(INT32)				VarItemProto->GetDword( _T("OtherRepVal"),	(*iter).c_str(), 0 );
		item.dwBuffID0			 =						VarItemProto->GetDword( _T("BuffID0"),		(*iter).c_str(), GT_INVALID );
		item.dwTriggerID0		 =						VarItemProto->GetDword( _T("TriggerID0"),	(*iter).c_str(), GT_INVALID );
		item.dwBuffID1			 =						VarItemProto->GetDword( _T("BuffID1"),		(*iter).c_str(), GT_INVALID );
		item.dwTriggerID1		 =						VarItemProto->GetDword( _T("TriggerID1"),	(*iter).c_str(), GT_INVALID );
		item.dwCooldownTime		 =						VarItemProto->GetDword( _T("CooldownTime"),	(*iter).c_str(), GT_INVALID );
		item.eOPType			 =	(ESkillOPType)		VarItemProto->GetDword( _T("OpType"),		(*iter).c_str(), ESOPT_NUll );
		item.fOPDist			 =						VarItemProto->GetFloat( _T("OpDist"),		(*iter).c_str(), 0.0f );
		item.fOPRadius			 =						VarItemProto->GetFloat( _T("OpRaduis"),		(*iter).c_str(), 0.0f );
		item.bInterruptMove		 =	(BOOL)				VarItemProto->GetDword( _T("InterruptMove"),(*iter).c_str(), false ) ? true : false;
		item.nPrepareTime		 =	(INT)				VarItemProto->GetDword( _T("PrepareTime"),	(*iter).c_str(), 0 );
		item.eSexLimit			 =	(ESexLimit)			VarItemProto->GetDword( _T("SexLimit"),		(*iter).c_str(), ESL_Null );
		item.byMinUseLevel		 =	(BYTE)				VarItemProto->GetDword( _T("MinUseLevel"),	(*iter).c_str(), 1 );
		item.byMaxUseLevel		 =	(BYTE)				VarItemProto->GetDword( _T("MaxUseLevel"),	(*iter).c_str(), GT_INVALID);
		item.dwTargetLimit		 =						VarItemProto->GetDword( _T("TargetLimit"),	(*iter).c_str(), GT_INVALID);
		item.dwSpecBuffLimitID	 =						VarItemProto->GetDword( _T("BuffLimitID"),	(*iter).c_str(), GT_INVALID);
		//item.bFriendly		 =	(BOOL)				VarItemProto->GetDword( _T("Friendly"),		(*iter).c_str(), false ) ? true : false;
		//item.bHostile			 =	(BOOL)				VarItemProto->GetDword( _T("Hostile"),		(*iter).c_str(), false ) ? true : false;
		//item.bIndependent		 =	(BOOL)				VarItemProto->GetDword( _T("Independent"),	(*iter).c_str(), false ) ? true : false;
		item.dwStateLimit		 =						VarItemProto->GetDword(  _T("StateLimit"),	(*iter).c_str(), 0 );
		item.bMoveable			 =	(BOOL)				VarItemProto->GetDword( _T("moveable"),		(*iter).c_str(), FALSE );
		//more
		item.ePickType = (EPickupType)			VarItemProto->GetDword( _T("picktype"),		(*iter).c_str(), ESL_Null );
		item.bCanMove2Ware = VarItemProto->GetDword( _T("can_save"),		(*iter).c_str(), false );

		m_mapItemProto.insert(make_pair(item.dwTypeID, item));
	}

	KillObj("ItemProto");


	
	//---------------------------------------------------------------------------------
	//读取item_name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarItemDisplay = CreateObj("ItemDisplay", "VarContainer");
	if(!VarItemDisplay->Load("VFS_System", szItemDisplay, "id", &ItemDisplayFieldList))
		IMSG(_T("Load file item_name.xml failed\r\n"));

	for(iter = ItemDisplayFieldList.begin(); iter != ItemDisplayFieldList.end(); ++iter)
	{
		DWORD dwID = VarItemDisplay->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagItemDisplayInfo* pDisplay = new tagItemDisplayInfo;
		_tcsncpy( pDisplay->szName,			VarItemDisplay->GetString(_T("name"),			(*iter).c_str(), _T("")), sizeof(pDisplay->szName)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szType,			VarItemDisplay->GetString(_T("display_type"),	(*iter).c_str(), _T("")), sizeof(pDisplay->szType)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szDesc,			VarItemDisplay->GetString(_T("desc"),			(*iter).c_str(), _T("")), sizeof(pDisplay->szDesc)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szMaleIcon,		VarItemDisplay->GetString(_T("male_icon"),		(*iter).c_str(), _T("")), sizeof(pDisplay->szMaleIcon)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szActionMod,	VarItemDisplay->GetString(_T("action_model"),	(*iter).c_str(), _T("")), sizeof(pDisplay->szActionMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szSourceEffect, VarItemDisplay->GetString(_T("source_effect"),	(*iter).c_str(), _T("")), sizeof(pDisplay->szSourceEffect)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szDestEffect,	VarItemDisplay->GetString(_T("dest_effect"),	(*iter).c_str(), _T("")), sizeof(pDisplay->szDestEffect)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szPrepareMod,	VarItemDisplay->GetString(_T("prepare_action"),	(*iter).c_str(), _T("")), sizeof(pDisplay->szPrepareMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szDropMod,		VarItemDisplay->GetString(_T("drop_model"),		(*iter).c_str(), _T("")), sizeof(pDisplay->szDropMod)/sizeof(TCHAR));
		
		pDisplay->bActionBlend		= (BOOL)VarItemDisplay->GetDword( _T("action_blend"),		(*iter).c_str(),	FALSE );

		//more

		tagItemProto* pItem = FindInMap(m_mapItemProto, dwID);
		if(P_VALID(pItem))
		{
			pItem->pDisplayInfo = pDisplay;
		}
		else
		{
			SAFE_DEL(pDisplay);
		}
	}
	
	KillObj("ItemDisplay");



	//---------------------------------------------------------------------------------
	//读取equip_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarEquipProto = CreateObj("EquipProto", "VarContainer");
	if(!VarEquipProto->Load("VFS_System", szEquipProto, "id", &EquipProtoFieldList))
		IMSG(_T("Load file equip_proto.xml failed\r\n"));

	for(iter = EquipProtoFieldList.begin(); iter != EquipProtoFieldList.end(); ++iter)
	{
		tagEquipProto equip;
		equip.dwTypeID			  =						VarEquipProto->GetDword( _T("id"),			(*iter).c_str(), GT_INVALID );
		equip.eType				  =	(EItemType)			VarEquipProto->GetDword( _T("Type"),		(*iter).c_str(), EIT_Null );
		equip.eTypeEx			  =	(EItemTypeEx)		VarEquipProto->GetDword( _T("TypeEx"),		(*iter).c_str(), EITE_Null );
		equip.eTypeReserved		  =	(EItemTypeReserved)	VarEquipProto->GetDword( _T("TypeReserved"),(*iter).c_str(), EITR_Null );
		equip.byLevel			  =	(BYTE)				VarEquipProto->GetDword( _T("Level"),		(*iter).c_str(), 0 );
		equip.byQuality			  =	(BYTE)				VarEquipProto->GetDword( _T("Quality"),		(*iter).c_str(), 0 );
		equip.byBindType		  =	(BYTE)				VarEquipProto->GetDword( _T("BindType"),	(*iter).c_str(), 0 );
		equip.nBasePrice		  =	(INT)				VarEquipProto->GetDword( _T("BasePrice"),	(*iter).c_str(), GT_INVALID );
		equip.dwTimeLimit		  =						VarEquipProto->GetDword( _T("TimeLimit"),	(*iter).c_str(), GT_INVALID );
		equip.nMaxUseTimes		  =	(INT)				VarEquipProto->GetDword( _T("MaxUseTimes"),	(*iter).c_str(), GT_INVALID );
		equip.n16MaxLapNum		  =	(INT16)				VarEquipProto->GetDword( _T("MaxLapNum"),	(*iter).c_str(), GT_INVALID	);
		equip.n16MaxHoldNum		  =	(INT16)				VarEquipProto->GetDword( _T("MaxHoldNum"),	(*iter).c_str(), GT_INVALID );
		equip.eSexLimit			  =	(ESexLimit)			VarEquipProto->GetDword( _T("SexLimit"),	(*iter).c_str(), ESL_Null );
		equip.byMinUseLevel		  =	(BYTE)				VarEquipProto->GetDword( _T("MinUseLevel"),	(*iter).c_str(), 1 );
		equip.byMaxUseLevel		  =	(BYTE)				VarEquipProto->GetDword( _T("MaxUseLevel"),	(*iter).c_str(), GT_INVALID);

		equip.eEquipPos			  =	(EEquipPos)			VarEquipProto->GetDword( _T("EquipPos"),	(*iter).c_str(), EEP_Start );
		equip.eWeaponPos		  =	(EWeaponPos)		VarEquipProto->GetDword( _T("WeaponPos"),	(*iter).c_str(), EWP_NULL );
		equip.n16WuHun			  =	(INT16)				VarEquipProto->GetDword( _T("WuHun"),		(*iter).c_str(), 0 );
		equip.n16MinDmg			  =	(INT16)				VarEquipProto->GetDword( _T("MinDmg"),		(*iter).c_str(), 0 );
		equip.n16MaxDmg			  =	(INT16)				VarEquipProto->GetDword( _T("MaxDmg"),		(*iter).c_str(), 0 );
		equip.n16Armor			  =	(INT16)				VarEquipProto->GetDword( _T("Armor"),		(*iter).c_str(), 0 );
		equip.nMaxPotVal		  =	(INT)				VarEquipProto->GetDword( _T("MaxPotval"),	(*iter).c_str(), 0 );
		equip.nPotVal			  =	(INT)				VarEquipProto->GetDword( _T("Potval"),		(*iter).c_str(), 0 );
		equip.nPotIncTimes		  =	(INT)				VarEquipProto->GetDword( _T("PotIncTimes"),		(*iter).c_str(), 0 );

		equip.n16Newness		  =	(INT16)				VarEquipProto->GetDword( _T("MaxNewness"),	(*iter).c_str(), 0 );
		equip.bNeedBroadcast	  =	(BOOL)				VarEquipProto->GetDword( _T("NeedBroadcast"),(*iter).c_str(), false ) ? true : false;
		equip.bCanSell			  =	(BOOL)				VarEquipProto->GetDword( _T("CanSell"),		(*iter).c_str(), false ) ? true : false;
		equip.eClanRepute		  =	(EReputationType)	VarEquipProto->GetDword( _T("ZoneRepType"),	(*iter).c_str(), ERT_NULL );
		equip.nClanReputeVal	  =	(INT32)				VarEquipProto->GetDword( _T("ZoneRepVal"),	(*iter).c_str(), 0 );
		equip.eOtherClanRepute	  =	(EReputationType)	VarEquipProto->GetDword( _T("OtherRepType"),(*iter).c_str(), ERT_NULL );
		equip.nOtherClanReputeVal =	(INT32)				VarEquipProto->GetDword( _T("OtherRepVal"),	(*iter).c_str(), 0 );
		equip.dwBuffID0			  =						VarEquipProto->GetDword( _T("BuffID0"),		(*iter).c_str(), GT_INVALID );
		equip.dwTriggerID0		  =						VarEquipProto->GetDword( _T("TriggerID0"),	(*iter).c_str(), GT_INVALID );
		equip.dwBuffID1			  =						VarEquipProto->GetDword( _T("BuffID1"),		(*iter).c_str(), GT_INVALID );
		equip.dwTriggerID1		  =						VarEquipProto->GetDword( _T("TriggerID1"),	(*iter).c_str(), GT_INVALID );
		equip.bSuckLimit		  =	(BOOL)				VarEquipProto->GetDword( _T("suck_limit"),	(*iter).c_str(), false ) ? true : false;
		equip.bOnlyScriptStrengthen		  =	(bool)				VarEquipProto->GetDword( _T("OnlyScriptStrengthen"),	(*iter).c_str(), 0 );
		equip.ePickType = (EPickupType)			VarEquipProto->GetDword( _T("picktype"),		(*iter).c_str(), ESL_Null );
		equip.dwCondenseLimit = VarEquipProto->GetDword( _T("godhead_limit"),		(*iter).c_str(), ESL_Null );
		TCHAR szTmp[X_SHORT_NAME]={0};
		for(int i=0; i<MAX_ROLEATT_BASE_EFFECT; ++i)
		{
			_stprintf(szTmp, _T("RoleAttType%d"), i);
			equip.BaseEffect[i].eRoleAtt =	(ERoleAttribute) VarEquipProto->GetDword( szTmp,		(*iter).c_str(), ERA_Null );
			_stprintf(szTmp, _T("RoleAttVal%d"), i);
			equip.BaseEffect[i].nValue	 =	(INT)			 VarEquipProto->GetDword( szTmp,		(*iter).c_str(), 0 );
		}

		for(int i=0; i<MAX_PEREQUIP_SUIT_NUM; ++i)
		{
			_stprintf(szTmp, _T("SuitID%d"),		i+1);
			equip.dwSuitID[i]		 =					VarEquipProto->GetDword( szTmp, (*iter).c_str(), GT_INVALID );
			_stprintf(szTmp, _T("SuitMinQlty%d"),	i+1);
			equip.bySuitMinQlty[i]	 = (BYTE)			VarEquipProto->GetInt(	 szTmp, (*iter).c_str(), 0 );
		}

		equip.byLongfuAtt[EWX_Metal] = (BYTE)			VarEquipProto->GetInt(	 _T("Metal"),		(*iter).c_str(), 0 );
		equip.byLongfuAtt[EWX_Wood]  = (BYTE)			VarEquipProto->GetInt(	 _T("Wood"),		(*iter).c_str(), 0 );
		equip.byLongfuAtt[EWX_Water] = (BYTE)			VarEquipProto->GetInt(	 _T("Water"),		(*iter).c_str(), 0 );
		equip.byLongfuAtt[EWX_Fire]  = (BYTE)			VarEquipProto->GetInt(	 _T("Fire"),		(*iter).c_str(), 0 );
		equip.byLongfuAtt[EWX_Soil]  = (BYTE)			VarEquipProto->GetInt(	 _T("Soil"),		(*iter).c_str(), 0 );
		equip.dwColor				 =					VarEquipProto->GetDword( _T("Corlor"),		(*iter).c_str(), 0 );
		equip.bCanDye				 = (BOOL)			VarEquipProto->GetInt(	 _T("CanDye"),		(*iter).c_str(), false ) ? true : false;

		equip.n16AttALimit[ERA_Physique]	= (INT16)	VarEquipProto->GetInt( _T("PhysiqueLim"),	(*iter).c_str(), 0 );		
		equip.n16AttALimit[ERA_Strength]	= (INT16)	VarEquipProto->GetInt( _T("StrengthLim"),	(*iter).c_str(), 0 );
		equip.n16AttALimit[ERA_Pneuma]		= (INT16)	VarEquipProto->GetInt( _T("PneumaLim"),		(*iter).c_str(), 0 );
		equip.n16AttALimit[ERA_InnerForce]	= (INT16)	VarEquipProto->GetInt( _T("InnerForceLim"),	(*iter).c_str(), 0 );
		equip.n16AttALimit[ERA_Technique]	= (INT16)	VarEquipProto->GetInt( _T("TechniqueLim"),	(*iter).c_str(), 0 );
		equip.n16AttALimit[ERA_Agility]		= (INT16)	VarEquipProto->GetInt( _T("AgilityLim"),	(*iter).c_str(), 0 );
		//more

		m_mapEquipProto.insert(make_pair(equip.dwTypeID, equip));
	}

	KillObj("EquipProto");

	
	//---------------------------------------------------------------------------------
	//读取equip_name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarEquipDisplay = CreateObj("EquipDisplay", "VarContainer");
	if(!VarEquipDisplay->Load("VFS_System", szEquipDisplay, "id", &EquipDisplayFieldList))
		IMSG(_T("Load file equip_name.xml failed\r\n"));

	for(iter = EquipDisplayFieldList.begin(); iter != EquipDisplayFieldList.end(); ++iter)
	{
		DWORD dwID = VarEquipDisplay->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagItemDisplayInfo* pDisplay = new tagItemDisplayInfo;
		_tcsncpy( pDisplay->szName,				VarEquipDisplay->GetString( _T("name"),					(*iter).c_str(), _T("") ), sizeof(pDisplay->szName)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szType,				VarEquipDisplay->GetString( _T("display_type"),			(*iter).c_str(), _T("") ), sizeof(pDisplay->szType)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szDesc,				VarEquipDisplay->GetString( _T("desc2"),				(*iter).c_str(), _T("") ), sizeof(pDisplay->szDesc)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szDescEx,			VarEquipDisplay->GetString( _T("desc"),					(*iter).c_str(), _T("") ), sizeof(pDisplay->szDesc)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szMaleIcon,			VarEquipDisplay->GetString( _T("male_icon"),			(*iter).c_str(), _T("") ), sizeof(pDisplay->szMaleIcon)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szFemaleIcon,		VarEquipDisplay->GetString( _T("female_icon"),			(*iter).c_str(), _T("") ), sizeof(pDisplay->szFemaleIcon)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szMaleRightMod,		VarEquipDisplay->GetString( _T("male_right_model"),		(*iter).c_str(), _T("") ), sizeof(pDisplay->szMaleRightMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szFemaleRightMod,	VarEquipDisplay->GetString( _T("female_right_model"),	(*iter).c_str(), _T("") ), sizeof(pDisplay->szFemaleRightMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szMaleLeftMod,		VarEquipDisplay->GetString( _T("male_left_model"),		(*iter).c_str(), _T("") ), sizeof(pDisplay->szMaleLeftMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szFemaleLeftMod,	VarEquipDisplay->GetString( _T("female_left_model"),	(*iter).c_str(), _T("") ), sizeof(pDisplay->szFemaleLeftMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szDropMod,			VarEquipDisplay->GetString( _T("drop_model"),			(*iter).c_str(), _T("") ), sizeof(pDisplay->szDropMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szPertainMod,		VarEquipDisplay->GetString( _T("pertain_model"),		(*iter).c_str(), _T("") ), sizeof(pDisplay->szPertainMod)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szBladeFlareSFX,	VarEquipDisplay->GetString( _T("blade_flare_sfx"),		(*iter).c_str(), _T("") ), sizeof(pDisplay->szBladeFlareSFX)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szBladeGlowSFX,	    VarEquipDisplay->GetString( _T("blade_glow_sfx"),		(*iter).c_str(), _T("") ), sizeof(pDisplay->szBladeGlowSFX)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szAttachSFX,	    VarEquipDisplay->GetString( _T("attach_sfx"),			(*iter).c_str(), _T("") ), sizeof(pDisplay->szAttachSFX)/sizeof(TCHAR));
		_tcsncpy( pDisplay->szBladeBornSFX,     VarEquipDisplay->GetString( _T("blade_born_sfx"),		(*iter).c_str(), _T("") ), sizeof(pDisplay->szBladeBornSFX)/sizeof(TCHAR));

		pDisplay->ePertainModPos	= (EWeaponPos)	 VarEquipDisplay->GetDword( _T("pertain_model_pos"),(*iter).c_str(), EWP_NULL );
		pDisplay->eModelType		= (EEquipMdlType)VarEquipDisplay->GetDword( _T("model_type"),		(*iter).c_str(), EEMT_NULL );

		TCHAR szBuff[X_SHORT_NAME];
		for( int i = 0; i < MAX_EQUIP_ATTACH_NUM; i++ )
		{
			_stprintf( szBuff, _T("attach%d_pos"), i+1 );
			pDisplay->eAttachModelPos[i] = (EEquipAttachPos)VarEquipDisplay->GetDword( szBuff,	(*iter).c_str(), EEAP_NULL );
		}

		//more

		tagEquipProto* pEquip = FindInMap(m_mapEquipProto, dwID);
		if(P_VALID(pEquip))
		{
			pEquip->pDisplayInfo = pDisplay;
		}
		else
		{
			SAFE_DEL(pDisplay);
		}
		
	}

	KillObj("EquipDisplay");

	//---------------------------------------------------------------------------------
	//读取equip_suit
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarSuitProto = CreateObj("SuitProto", "VarContainer");
	if(!VarSuitProto->Load("VFS_System", szSuitProto, "id", &SuitProtoFieldList))
		IMSG(_T("Load file equip_suit.xml failed\r\n"));

	for(iter = SuitProtoFieldList.begin(); iter != SuitProtoFieldList.end(); ++iter)
	{
		tagSuitProtoClient suit;
		suit.dwID				=		VarSuitProto->GetDword(	_T("id"),			 (*iter).c_str(), GT_INVALID );
		suit.n8SpecEffectNum	= (INT8)VarSuitProto->GetInt(	_T("SpecEffectNum"), (*iter).c_str(), GT_INVALID );
		TCHAR szTemp[X_SHORT_NAME] = {0};
		for( int i = 0; i < MAX_SUIT_ATT_NUM; i++ )
		{
			_stprintf( szTemp, _T("TriggerID%d"),	i+1 );
			suit.dwTriggerID[i] =		VarSuitProto->GetDword( szTemp, (*iter).c_str(), GT_INVALID );
			_stprintf( szTemp, _T("BuffID%d"),		i+1 );
			suit.dwBuffID[i]	=		VarSuitProto->GetDword( szTemp, (*iter).c_str(), GT_INVALID );
			_stprintf( szTemp, _T("ActiveNum%d"),	i+1 );
			suit.n8ActiveNum[i]	= (INT8)VarSuitProto->GetInt(	szTemp, (*iter).c_str(), GT_INVALID );
		}

		m_mapSuitProto.insert(make_pair(suit.dwID, suit));
	}

	KillObj("SuitProto");

	//---------------------------------------------------------------------------------
	//读取equip_suit_name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarSuitDisplay = CreateObj("SuitDisplay", "VarContainer");
	if(!VarSuitDisplay->Load("VFS_System", szSuitDisplay, "id", &SuitDisplayFieldList))
		IMSG(_T("Load file equip_suit_name.xml failed\r\n"));

	for(iter = SuitDisplayFieldList.begin(); iter != SuitDisplayFieldList.end(); ++iter)
	{
		DWORD dwID = VarSuitDisplay->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagSuitProtoClient* pSuit = FindInMap(m_mapSuitProto, dwID);
		if (P_VALID(pSuit))
		{
			_tcsncpy( pSuit->szSuitName,		VarSuitDisplay->GetString( _T("name"),				(*iter).c_str(), _T("") ), sizeof(pSuit->szSuitName)/sizeof(TCHAR));
			_tcsncpy( pSuit->szSpecEffectMod,	VarSuitDisplay->GetString( _T("spec_effect_mod"),	(*iter).c_str(), _T("") ), sizeof(pSuit->szSpecEffectMod)/sizeof(TCHAR));
		}
	}

	KillObj("SuitDisplay");


	//---------------------------------------------------------------------------------
	//读取equip_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarTrumpProto = CreateObj("TrumpProto", "VarContainer");
	if(!VarTrumpProto->Load("VFS_System", szTrumpProto, "id", &TrumpProtoFieldList))
		IMSG(_T("Load file MagicWeapon.xml failed\r\n"));

	for(iter = TrumpProtoFieldList.begin(); iter != TrumpProtoFieldList.end(); ++iter)
	{
		// 精灵特效
		tagFairyEffect Effect;
		Effect.dwTypeID = VarTrumpProto->GetDword( _T("id"), (*iter).c_str(), GT_INVALID );
		for( int i = 0; i < MAX_FAIRY_SHAPE; i++)
		{
			TCHAR szTemp[X_SHORT_NAME] = {0};
			_sntprintf( szTemp, X_SHORT_NAME, _T("effects%d"), i+1);
			_tcsncpy( Effect.szFairyShape[i], VarTrumpProto->GetString(szTemp, (*iter).c_str(), _T("")), sizeof(Effect.szFairyShape[i])/sizeof(TCHAR));
		}
		_tcsncpy( Effect.szFirstPic, VarTrumpProto->GetString(_T("icon1"), (*iter).c_str(), _T("")), sizeof(Effect.szFirstPic)/sizeof(TCHAR));
		_tcsncpy( Effect.szSecondPic, VarTrumpProto->GetString(_T("icon2"), (*iter).c_str(), _T("")), sizeof(Effect.szSecondPic)/sizeof(TCHAR));
		_tcsncpy( Effect.szThirdPic, VarTrumpProto->GetString(_T("icon3"), (*iter).c_str(), _T("")), sizeof(Effect.szThirdPic)/sizeof(TCHAR));
		m_mapFairyEffect.insert( make_pair(Effect.dwTypeID, Effect ) );

		//
		tagFabaoProto Trump;
		Trump.dwTypeID			  =						VarTrumpProto->GetDword( _T("id"),			(*iter).c_str(), GT_INVALID );
		const tagEquipProto *pEquipProto = FindInMap(m_mapEquipProto, Trump.dwTypeID );
		if(P_VALID(pEquipProto))
			memcpy( &Trump, pEquipProto, sizeof(tagEquipProto) );
		Trump.eFarbaoType		  =	(ENUM_FabaoType)	VarTrumpProto->GetDword( _T("type"),		(*iter).c_str(), EFB_NULL );

		TCHAR szTmp[X_SHORT_NAME]={0};
		for(int i=0; i<MaxWuxingRewardNUM; ++i)
		{
			_stprintf(szTmp, _T("att_type%d"), i+1);
			Trump.eAttType[i]		  =	(ERoleAttribute)	VarTrumpProto->GetDword( szTmp,	(*iter).c_str(), ERA_Null );
			_stprintf(szTmp, _T("att_growing%d"), i+1);
			Trump.nAttGrowing[i]	  = (INT16)				VarTrumpProto->GetDword( szTmp,	(*iter).c_str(), 0 );
		}

		for(int i=0; i<MaxWuxingSlotNum; ++i)
		{
			_stprintf(szTmp, _T("slot_att_type%d"), i+1);
			Trump.eSlotAttType[i]	  =	(ERoleAttribute)	VarTrumpProto->GetDword( szTmp,	(*iter).c_str(), ERA_Null );
			_stprintf(szTmp, _T("slot_attval%d"), i+1);
			Trump.nSlotAttValue[i]	  = (INT)				VarTrumpProto->GetDword( szTmp,	(*iter).c_str(), 0 );
		}

		Trump.bCanReset			  =	(BOOL)				VarTrumpProto->GetDword( _T("can_reset"),	(*iter).c_str(), false ) ? true : false;
		Trump.bCanUse			  =	(BOOL)				VarTrumpProto->GetDword( _T("can_use"),		(*iter).c_str(), false ) ? true : false;
		Trump.bHasDefaultValue	  =	(BOOL)				VarTrumpProto->GetDword( _T("is_have_defaultval"),		(*iter).c_str(), false ) ? true : false;
		Trump.n16DefaultFabaoQulity	 = (INT16)			VarTrumpProto->GetDword( _T("defaultqua"),	(*iter).c_str(), GT_INVALID );
		Trump.n16DefaultWuxing	  =	(INT16)				VarTrumpProto->GetDword( _T("default_wuxing"),	(*iter).c_str(), GT_INVALID );
		Trump.bCanBirth			  =	(BOOL)				VarTrumpProto->GetDword( _T("can_birth"),		(*iter).c_str(), false ) ? true : false;

		//for(int i=0; i<MaxWuxingRewardNUM; ++i)
		//{
		//	_stprintf(szTmp, _T("wuxing_id%d"), i+1);
		//	Trump.nWuxingMatchReward[i]		  =	-1;//(INT)	VarTrumpProto->GetDword( szTmp,	(*iter).c_str(), GT_INVALID );
		//}


		for(int i=0; i<MaxWuxingSlotNum; ++i)
		{
			_stprintf(szTmp, _T("slot_wuxing%d"), i+1);
			Trump.eDefaultSlotType[i]	  =	(EWuXing)	VarTrumpProto->GetDword( szTmp,	(*iter).c_str(), EWX_Null );
		}

		Trump.nIdfQltyPct[0]	  =	(EWuXing)	VarTrumpProto->GetDword( _T("whi_pct"),	(*iter).c_str(), EIQ_Null );
		Trump.nIdfQltyPct[1]	  =	(EWuXing)	VarTrumpProto->GetDword( _T("yel_pct"),	(*iter).c_str(), EIQ_Null );
		Trump.nIdfQltyPct[2]	  =	(EWuXing)	VarTrumpProto->GetDword( _T("gre_pct"),	(*iter).c_str(), EIQ_Null );
		Trump.nIdfQltyPct[3]	  =	(EWuXing)	VarTrumpProto->GetDword( _T("blu_pct"),	(*iter).c_str(), EIQ_Null );
		Trump.nIdfQltyPct[4]	  =	(EWuXing)	VarTrumpProto->GetDword( _T("org_pct"),	(*iter).c_str(), EIQ_Null );
		Trump.nIdfQltyPct[5]	  =	(EWuXing)	VarTrumpProto->GetDword( _T("pur_pct"),	(*iter).c_str(), EIQ_Null );

		Trump.n16CanFusion		  = VarTrumpProto->GetDword(_T("Fairy_smelting"), (*iter).c_str(), 0);

		m_mapTrumpProto.insert(make_pair(Trump.dwTypeID, Trump));
	}

	KillObj("TrumpProto");

	//---------------------------------------------------------------------------------
	//读取treasure_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarTreasureProto = CreateObj("TreasureProto", "VarContainer");
	if(!VarTreasureProto->Load("VFS_System", szTreasureProto, "id", &TreasureProtoFieldList))
		IMSG(_T("Load file clantreasure_proto.xml failed\r\n"));

	for (iter = TreasureProtoFieldList.begin(); iter != TreasureProtoFieldList.end(); ++iter)
	{
		tagTreasureProto treasure;

		treasure.dwTreasureID	=	VarTreasureProto->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		treasure.dwItemID		=	VarTreasureProto->GetDword(_T("itemid"), (*iter).c_str(), GT_INVALID);
		treasure.nConNeed		=	VarTreasureProto->GetInt(_T("act_clancon_need"), (*iter).c_str(), 0);
		treasure.eCtype			=	(ECLanType)VarTreasureProto->GetInt(_T("clantype"), (*iter).c_str(), GT_INVALID);
		treasure.dwShopID		=	VarTreasureProto->GetDword(_T("shopid"), (*iter).c_str(), GT_INVALID);

		m_mapTreasureProto.insert(make_pair(treasure.dwTreasureID, treasure));
		m_mapTreasureProtoByItemID.insert(make_pair(treasure.dwItemID, treasure));
	}

	KillObj("TreasureProto");

    //---------------------------------------------------------------------------------
    //读取PetEquip_proto
    //---------------------------------------------------------------------------------
    TObjRef<VarContainer> VarPetEquipProto = CreateObj("PetEquipProto", "VarContainer");
    if(!VarPetEquipProto->Load("VFS_System", szPetEquipProto, "id", &PetEquipProtoList))
        IMSG(_T("Load file pet_equip_proto.xml failed\r\n"));

    for (iter = PetEquipProtoList.begin(); iter != PetEquipProtoList.end(); ++iter)
    {
        tagPetEquipProto petEquip;

        petEquip.dwTypeID       =   VarPetEquipProto->GetDword(_T("id"), iter->c_str(), GT_INVALID);
        petEquip.nStep          =   VarPetEquipProto->GetInt(_T("step"), iter->c_str(), GT_INVALID);
        petEquip.nGrade         =   VarPetEquipProto->GetInt(_T("grade"), iter->c_str(), GT_INVALID);
        petEquip.nType          =   VarPetEquipProto->GetInt(_T("type"), iter->c_str(), GT_INVALID);
        petEquip.bTypeUnique    =   VarPetEquipProto->GetInt(_T("unique_same_type"), iter->c_str(), GT_INVALID) == 0 ? FALSE : TRUE;
        petEquip.bUnique        =   VarPetEquipProto->GetInt(_T("unique"), iter->c_str(), GT_INVALID) == 0 ? FALSE : TRUE;
        petEquip.nPetAtt[0]     =   VarPetEquipProto->GetInt(_T("att1"), iter->c_str(), GT_INVALID);
        petEquip.nPetAttMod[0]  =   VarPetEquipProto->GetInt(_T("mod1"), iter->c_str(), GT_INVALID);
        petEquip.nPetAtt[1]     =   VarPetEquipProto->GetInt(_T("att2"), iter->c_str(), GT_INVALID);
        petEquip.nPetAttMod[1]  =   VarPetEquipProto->GetInt(_T("mod2"), iter->c_str(), GT_INVALID);
        petEquip.nPetAtt[2]     =   VarPetEquipProto->GetInt(_T("att3"), iter->c_str(), GT_INVALID);

        m_mapPetEquipProtoByID.insert(make_pair(petEquip.dwTypeID, petEquip));
    }

    KillObj("PetEquipProto");

	//---------------------------------------------------------------------------------
	//读取equip_qlty_effect
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarEquipQlty = CreateObj("EquipQlty", "VarContainer");
	if(!VarEquipQlty->Load("VFS_System", szEquipQlty, "id", &EquipQltyList))
		IMSG(_T("Load file equip_qlty_effect.xml failed\r\n"));

	for (iter = EquipQltyList.begin(); iter != EquipQltyList.end(); ++iter)
	{	
		INT qlty			= VarEquipQlty->GetInt(_T("id"), iter->c_str(), 0);	
		tagEquipQltyEffect m;
		m.fWeaponFactor		= VarEquipQlty->GetFloat(_T("WeaponFactor"), iter->c_str(), 1.0);
		m.fArmorFactor		= VarEquipQlty->GetFloat(_T("ArmorFactor"),	 iter->c_str(), 1.0);
		m.fPotFactor		= VarEquipQlty->GetFloat(_T("PotFactor"),	 iter->c_str(), 1.0);
		m.nMaxStrengthenLevel = VarEquipQlty->GetInt(_T("streng_level_limit"),	 iter->c_str(), 0);
		//more
		
		m_mapEquipQlty.insert(make_pair(qlty, m));
	}

	KillObj("EquipQlty");

	//---------------------------------------------------------------------------------
	// zxzhang 2010-1-8 读取 装备鉴定几率的属性表
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarEquipQltyPct = CreateObj("EquipQltyPct", "VarContainer");
	if(!VarEquipQltyPct->Load("VFS_System", szEquipQltyPct, "id", &EquipQltyPctList))
		IMSG(_T("Load file equip_qlty_pct.xml failed\r\n"));

	for (iter = EquipQltyPctList.begin(); iter != EquipQltyPctList.end(); ++iter)
	{	
		tagEquipQltyPct m;
		m.dwTypeID	= VarEquipQltyPct->GetInt(_T("id"), iter->c_str(), 0);	
		
		int i=0;
		m.nEquipQltyPct[i++]		= VarEquipQltyPct->GetInt(_T("White"),	 iter->c_str(), 0);
		m.nEquipQltyPct[i++]		= VarEquipQltyPct->GetInt(_T("Yellow"),	 iter->c_str(), 0);
		m.nEquipQltyPct[i++]		= VarEquipQltyPct->GetInt(_T("Green"),	 iter->c_str(), 0);
		m.nEquipQltyPct[i++]		= VarEquipQltyPct->GetInt(_T("Blue"),	 iter->c_str(), 0);
		m.nEquipQltyPct[i++]		= VarEquipQltyPct->GetInt(_T("Orange"),	 iter->c_str(), 0);
		m.nEquipQltyPct[i++]		= VarEquipQltyPct->GetInt(_T("Violet"),	 iter->c_str(), 0);

		m_mapEquipQltyPct.insert(make_pair(m.dwTypeID, m));
	}

	KillObj("EquipQltyPct");

    //---------------------------------------------------------------------------------
    // 读取时装精纺原型数据
    //---------------------------------------------------------------------------------
    TObjRef<VarContainer> VarSpinProto = CreateObj("FashionSpin", "VarContainer");
    if(!VarSpinProto->Load("VFS_System", szSpinProto, "id", &SpinProtoList))
        IMSG(_T("Load file spin_proto.xml failed\r\n"));

    for (iter = SpinProtoList.begin(); iter != SpinProtoList.end(); ++iter)
    {
        DWORD spinNull      = VarSpinProto->GetDword(_T("raw_fashion"),     iter->c_str(), GT_INVALID);
        DWORD spinBasic     = VarSpinProto->GetDword(_T("basic_spin"),      iter->c_str(), GT_INVALID);
        DWORD spinMiddle    = VarSpinProto->GetDword(_T("middle_spin"),     iter->c_str(), GT_INVALID);
        DWORD spinSenior    = VarSpinProto->GetDword(_T("senior_spin"),     iter->c_str(), GT_INVALID);
        ESuitAttType spinSuitType    = (ESuitAttType)VarSpinProto->GetInt(_T("suit_att_type"),     iter->c_str(), 0);

        m_mapSpinSuitType.insert(make_pair(spinBasic, spinSuitType));
        m_mapSpinSuitType.insert(make_pair(spinMiddle, spinSuitType));
        m_mapSpinSuitType.insert(make_pair(spinSenior, spinSuitType));

        tagSpinProtoList spinProto;
        spinProto.dwBasic   = spinBasic;
        spinProto.dwMiddle  = spinMiddle;
        spinProto.dwSenior  = spinSenior;

        m_mapDirector.insert(make_pair(make_pair(spinNull, spinSuitType), spinProto));

        m_SetSpinBasic.insert(spinBasic);
        m_setSpinMiddle.insert(spinMiddle);
        m_setSpinSenior.insert(spinSenior);
    }

    for (iter = SpinProtoList.begin(); iter != SpinProtoList.end(); ++iter)
    {
        DWORD spinNull      = VarSpinProto->GetDword(_T("raw_fashion"),     iter->c_str(), GT_INVALID);
        DWORD spinBasic     = VarSpinProto->GetDword(_T("basic_spin"),      iter->c_str(), GT_INVALID);
        DWORD spinMiddle    = VarSpinProto->GetDword(_T("middle_spin"),     iter->c_str(), GT_INVALID);
        DWORD spinSenior    = VarSpinProto->GetDword(_T("senior_spin"),     iter->c_str(), GT_INVALID);
        ESuitAttType spinSuitType    = (ESuitAttType)VarSpinProto->GetInt(_T("suit_att_type"),     iter->c_str(), 0);

        m_mapSpinNull.insert(make_pair(make_pair(spinNull, spinSuitType), spinBasic));
        for (int i = ESSAT_Brave; i < ESSAT_End; ++i)
        {
            map<pair<DWORD, ESuitAttType>, tagSpinProtoList>::iterator iterDirect;
            iterDirect = m_mapDirector.find(make_pair(spinNull, (ESuitAttType)i));
            if (iterDirect != m_mapDirector.end())
            {
                m_mapSpinBasic.insert(make_pair(make_pair(spinBasic, (ESuitAttType)i), iterDirect->second.dwMiddle));
                m_mapSpinMiddle.insert(make_pair(make_pair(spinMiddle, (ESuitAttType)i), iterDirect->second.dwSenior));
            }
        }
    }

    KillObj("FashionSpin");

    //---------------------------------------------------------------------------------
    // 读取时装精纺机率数据
    //---------------------------------------------------------------------------------
    TObjRef<VarContainer> VarSpinRate = CreateObj("FashionSpinRate", "VarContainer");
    if(!VarSpinRate->Load("VFS_System", szSpinRate, "id", &SpinRateList))
        IMSG(_T("Load file spin_rate_proto.xml failed\r\n"));

    for (iter = SpinRateList.begin(); iter != SpinRateList.end(); ++iter)
    {
        int itemNum        = VarSpinRate->GetInt(_T("item_num"),         iter->c_str(), 0);
        int basicRate      = VarSpinRate->GetInt(_T("basic_rate"),       iter->c_str(), 0);
        int middleRate     = VarSpinRate->GetInt(_T("middle_rate"),      iter->c_str(), 0);
        int seniorRate     = VarSpinRate->GetInt(_T("senior_rate"),      iter->c_str(), 0);

        m_mapSpinRate.insert(make_pair(make_pair(itemNum, ESpinStep_Basic), basicRate));
        m_mapSpinRate.insert(make_pair(make_pair(itemNum, ESpinStep_Middle), middleRate));
        m_mapSpinRate.insert(make_pair(make_pair(itemNum, ESpinStep_Senior), seniorRate));
    }

    KillObj("FashionSpinRate");

	//---------------------------------------------------------------------------------
	//读取五行奖励
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarTrumpWuXingAward = CreateObj("TrumpAward", "VarContainer");
	if(!VarTrumpWuXingAward->Load("VFS_System", szTrumpWuXingAward, "id", &TrumpWuxingAwardList))
		IMSG(_T("Load file wuxing_name.xml failed\r\n"));

	for(iter = TrumpWuxingAwardList.begin(); iter != TrumpWuxingAwardList.end(); ++iter)
	{
		tagTrumpWuXingAward TrumpAward;
		TrumpAward.dwTypeID = VarTrumpWuXingAward->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		_tcsncpy( TrumpAward.szName,			VarTrumpWuXingAward->GetString(_T("name"),			(*iter).c_str(), _T("")), sizeof(TrumpAward.szName)/sizeof(TCHAR));
		_tcsncpy( TrumpAward.szDesc,			VarTrumpWuXingAward->GetString(_T("discription"),			(*iter).c_str(), _T("")), sizeof(TrumpAward.szDesc)/sizeof(TCHAR));
		m_mapTrumpWuXingAward.insert( make_pair(TrumpAward.dwTypeID, TrumpAward ) );
	}

	KillObj("TrumpAward");

	//---------------------------------------------------------------------------------
	//读取妖精水果
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarFairyFruit = CreateObj("FairyFruit", "VarContainer");
	if(!VarFairyFruit->Load("VFS_System", szFairyFruit, "id", &FairyFruitList))
		IMSG(_T("Load file fabaofruit.xml failed\r\n"));

	for(iter = FairyFruitList.begin(); iter != FairyFruitList.end(); ++iter)
	{
		tagFairyFruit FairyFruit;
		FairyFruit.dwTypeID = VarFairyFruit->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		FairyFruit.byType	=(BYTE)VarFairyFruit->GetDword( _T("add_type"),	(*iter).c_str(), 0 );
		FairyFruit.byMin	=(BYTE)VarFairyFruit->GetDword( _T("lower_limit"),	(*iter).c_str(), 0 );
		FairyFruit.byMax	=(BYTE)VarFairyFruit->GetDword( _T("higher_limit"),	(*iter).c_str(), 0 );
		FairyFruit.byValue	=(BYTE)VarFairyFruit->GetDword( _T("increment"),	(*iter).c_str(), 0 );
		m_mapFairyFruit.insert( make_pair(FairyFruit.dwTypeID, FairyFruit ) );
	}

	KillObj("FairyFruit");



	//---------------------------------------------------------------------------------
	//读取妖精妖精特技
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarFairySkills = CreateObj("FairySkills", "VarContainer");
	if(!VarFairySkills->Load("VFS_System", szFairySkills, "id", &FairySkillsList))
		IMSG(_T("Load file stunt_name.xml failed\r\n"));

	for(iter = FairySkillsList.begin(); iter != FairySkillsList.end(); ++iter)
	{
		tagFairySkill FairySkill;
		FairySkill.dwTypeID = VarFairySkills->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		FairySkill.nType = VarFairySkills->GetInt(_T("type"), (*iter).c_str(), GT_INVALID-1);
		_tcsncpy( FairySkill.szName,			VarFairySkills->GetString(_T("name"),			(*iter).c_str(), _T("")), sizeof(FairySkill.szName)/sizeof(TCHAR));
		_tcsncpy( FairySkill.szDesc,			VarFairySkills->GetString(_T("desc"),			(*iter).c_str(), _T("")), sizeof(FairySkill.szDesc)/sizeof(TCHAR));
		m_mapFairySkill.insert(make_pair(FairySkill.dwTypeID, FairySkill));
	}

	KillObj("FairySkills");
	//---------------------------------------------------------------------------------
	//读取妖精提升次数表 add by xp
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarFairyUpgradeTimes = CreateObj("FairyUpgradeTimes", "VarContainer");
	if(!VarFairyUpgradeTimes->Load("VFS_System", szFairyUpgrade, "id", &FairyAddtimesList))
		IMSG(_T("Load file fairy upgrade.xml failed\r\n"));
	for(iter = FairyAddtimesList.begin(); iter != FairyAddtimesList.end(); ++iter)
	{
		tagFairyUpgradeTimes FairyUpgradeTimes;
		FairyUpgradeTimes.dwID = VarFairyUpgradeTimes->GetDword(_T("id"), (*iter).c_str(), 0);
		FairyUpgradeTimes.byType	=(BYTE)VarFairyUpgradeTimes->GetDword( _T("type"),	(*iter).c_str(), 0 );
		FairyUpgradeTimes.n32Money	=(INT32)VarFairyUpgradeTimes->GetDword( _T("money"),	(*iter).c_str(), 0 );
		
		m_mapFairyUpgradeTimes.insert( make_pair(FairyUpgradeTimes.dwID, FairyUpgradeTimes ) );
	}

	KillObj("FairyUpgradeTimes");

	//---------------------------------------------------------------------------------
	//读取妖精经验球
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarFairyExpBall = CreateObj("FairyExpBall", "VarContainer");
	if(!VarFairyExpBall->Load("VFS_System", szFairyExpBall, "id", &FairyExpBallList))
		IMSG(_T("Load file expball.xml failed\r\n"));

	for(iter = FairyExpBallList.begin(); iter != FairyExpBallList.end(); ++iter)
	{
		DWORD dwTypeID;
		dwTypeID = VarFairyExpBall->GetDword(_T("item_id"), (*iter).c_str(), GT_INVALID);
		m_vecFairyExpBall.push_back(dwTypeID);
	}

	KillObj("FairyExpBall");

	TCHAR szText[128] = {0};
	//---------------------------------------------------------------------------------
	// 账号回购
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarIDBuyBack = CreateObj("IDBuyBack", "VarContainer");
	if(!VarIDBuyBack->Load("VFS_System", szIDBuyBack, "id", &IDButBackList))
		IMSG(_T("Load file Idbuyback.xml failed\r\n"));

	for(iter = IDButBackList.begin(); iter != IDButBackList.end(); ++iter)
	{
		tagIDBuyBack IDBuyBack;
		IDBuyBack.dwID = VarIDBuyBack->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		IDBuyBack.dwExp = VarIDBuyBack->GetDword(_T("exp"), (*iter).c_str(), GT_INVALID);
		IDBuyBack.dwGiftID = VarIDBuyBack->GetDword(_T("gift"), (*iter).c_str(), GT_INVALID);
		for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
		{
			_stprintf(szText, _T("itemID%d"), i+1);
			IDBuyBack.dwItemID[i] = VarIDBuyBack->GetDword(szText, (*iter).c_str(), GT_INVALID);
		}

		m_mapIDBuyBack.insert(make_pair(IDBuyBack.dwID, IDBuyBack));
	}
	//---------------------------------------------------------------------------------
	// 蛋疼的账号回购显示
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarIDBuyBackEx = CreateObj("IDBuyBackEx", "VarContainer");
	if(!VarIDBuyBackEx->Load("VFS_System", szIDBuyBackEx, "id", &IDBuyBackListEx))
		IMSG(_T("Load file Idbuybackex_name.xml failed\r\n"));

	for(iter = IDBuyBackListEx.begin(); iter != IDBuyBackListEx.end(); ++iter)
	{
		m_tagIDBuyBackEx.dwID = VarIDBuyBackEx->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM;i++)
		{
			_stprintf(szText, _T("itemid%d"), i+1);
			m_tagIDBuyBackEx.dwItemID[i] = VarIDBuyBackEx->GetDword(szText, (*iter).c_str(), GT_INVALID);
			_stprintf(szText, _T("desc%d"), i+1);
			m_tagIDBuyBackEx.strDesc[i] = VarIDBuyBackEx->GetString(szText, (*iter).c_str(), _T(""));
		}
	}

	KillObj("IDBuyBack");

	//---------------------------------------------------------------------------------
	//读取equipGrowProto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarEquipGrowProto = CreateObj("EquipGrowProto", "VarContainer");
	if(!VarEquipGrowProto->Load("VFS_System", szEquipGrow, "id", &EquipGrowList))
		IMSG(_T("Load file equip_levelup.xml failed\r\n"));

	for (iter = EquipGrowList.begin(); iter != EquipGrowList.end(); ++iter)
	{
		tagEquipGrowName EquipGrow;

		EquipGrow.dwID = VarEquipGrowProto->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		//成长属性强度
		EquipGrow.byStrength = VarEquipGrowProto->GetDword(_T("strength"), (*iter).c_str(), GT_INVALID);
		//成长经验
		for(int i = 0; i < EQUIP_GROW_MAX_LEVEL; i++)
		{
			_stprintf(szText, _T("exp_%d"), i+1);
			EquipGrow.dwExp[i] = VarEquipGrowProto->GetDword(szText, (*iter).c_str(), GT_INVALID);
		}
		//属性类型,开启等级
		for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
		{
			_stprintf(szText, _T("att%d_type"), i+1);
			EquipGrow.eAtt[i] = (ERoleAttribute)VarEquipGrowProto->GetDword(szText, (*iter).c_str(), ERA_Null);
			_stprintf(szText, _T("att%d_open_level"), i+1);
			EquipGrow.byAttOpenLevel[i] = (BYTE)VarEquipGrowProto->GetDword(szText, (*iter).c_str(), 0);
		}
		//属性数值5*11
		for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
		{
			for(int j = 0; j < EQUIP_GROW_MAX_LEVEL; j++)
			{
				_stprintf(szText, _T("att%d_value%d"), i+1, j+1);
				EquipGrow.dwAttValue[i][j] = VarEquipGrowProto->GetDword(szText, (*iter).c_str(), GT_INVALID);
			}
		}
		m_mapEquipGrow.insert(make_pair(EquipGrow.dwID, EquipGrow));
	}

	KillObj("EquipGrowProto");

	//---------------------------------------------------------------------------------
	//读取妖精熔炼表
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarFairyFusionRetProto = CreateObj("FairyFusionRetProto", "VarContainer");
	if(!VarFairyFusionRetProto->Load("VFS_System", szFairy_smelting, "id", &FairySmeltingList))
		IMSG(_T("Load file Fairy_smelting.xml failed\r\n"));

	for(iter = FairySmeltingList.begin(); iter != FairySmeltingList.end(); ++iter)
	{
		tagFairyFusionRetProto FairyFusionRet;
		FairyFusionRet.dwID				= VarFairyFusionRetProto->GetDword( _T("id"),					(*iter).c_str(), GT_INVALID );
		FairyFusionRet.byLowScore		= VarFairyFusionRetProto->GetDword( _T("lower_limit"),			(*iter).c_str(), GT_INVALID );
		FairyFusionRet.byHighScore		= VarFairyFusionRetProto->GetDword( _T("higher_limit"),			(*iter).c_str(), GT_INVALID );
		FairyFusionRet.dwPinkRet		= VarFairyFusionRetProto->GetDword( _T("type_pro1"),			(*iter).c_str(), GT_INVALID );
		FairyFusionRet.dwPurpleRet		= VarFairyFusionRetProto->GetDword( _T("type_pro2"),			(*iter).c_str(), GT_INVALID );
		FairyFusionRet.dwOrangeRet		= VarFairyFusionRetProto->GetDword( _T("type_pro3"),			(*iter).c_str(), GT_INVALID );
		FairyFusionRet.dwFailRet		= VarFairyFusionRetProto->GetDword( _T("type_pro4"),			(*iter).c_str(), GT_INVALID );

		m_mapFairyFusionRet.insert(make_pair(FairyFusionRet.dwID, FairyFusionRet));
	}

	KillObj("FairyFusionRetProto");
	//----------------------------------------------------------------------------------
	//圣灵属性表
	//----------------------------------------------------------------------------------
	TObjRef<VarContainer> VarShengLingProto = CreateObj("ShengLingProto", "VarContainer");
	if(!VarShengLingProto->Load("VFS_System", szShengLing, "id", &ShengLingList))
		IMSG(_T("Load file HolyGods_proto.xml failed\r\n"));

	for(iter = ShengLingList.begin(); iter != ShengLingList.end(); ++iter)
	{
		tagSoulAttName ShengLingName;
		ShengLingName.dwTypeID				= VarShengLingProto->GetDword( _T("id"),					(*iter).c_str(), GT_INVALID );
		//ShengLingName.dwCoValue			= VarShengLingProto->GetDword( _T(""),					(*iter).c_str(), 0);	//默契不知道要不要

		ShengLingName.nDevourNum			= VarShengLingProto->GetDword(_T("devour_Num"),				(*iter).c_str(),	0);
		ShengLingName.n16EquipmentNumber	= VarShengLingProto->GetDword(_T("equipment_number"),		(*iter).c_str(),	0);
		ShengLingName.nHolyMaxvalue			= VarShengLingProto->GetDword(_T("holy_maxvalue"),			(*iter).c_str(),	0);
		ShengLingName.n16CallConsumption	= VarShengLingProto->GetDword(_T("call_consumption"),		(*iter).c_str(),	0);
		ShengLingName.nHolyDmg				= VarShengLingProto->GetDword(_T("holy_dmg"),				(*iter).c_str(),	0);
		ShengLingName.nHolyDef				= VarShengLingProto->GetDword(_T("holy_def"),				(*iter).c_str(),	0);
		ShengLingName.nHolyValue			= VarShengLingProto->GetDword(_T("holy_value"),				(*iter).c_str(),	0);
		ShengLingName.nCrit					= VarShengLingProto->GetDword(_T("crit"),					(*iter).c_str(),	0);
		ShengLingName.n16HolyCritRate		= VarShengLingProto->GetDword(_T("Holy_Crit_Rate"),			(*iter).c_str(),	0);
		ShengLingName.nExDamage				= VarShengLingProto->GetDword(_T("ExDamage"),				(*iter).c_str(),	0);
		ShengLingName.nAttackTec			= VarShengLingProto->GetDword(_T("AttackTec"),				(*iter).c_str(),	0);
		ShengLingName.nNeglectToughness		= VarShengLingProto->GetDword(_T("neglect_toughness"),		(*iter).c_str(),	0);

		m_mapShengLingName.insert(make_pair(ShengLingName.dwTypeID, ShengLingName));
	}

	KillObj("ShengLingProto");

	//----------------------------------------------------------------------------------
	//圣灵属性 name表
	//----------------------------------------------------------------------------------
	TObjRef<VarContainer> VarShengLingDisplay = CreateObj("ShengLingDisplay", "VarContainer");
	if(!VarShengLingDisplay->Load("VFS_System", szShengLingEx, "id", &ShengLingListEx))
		IMSG(_T("Load file HolyGods_proto.xml failed\r\n"));

	for(iter = ShengLingListEx.begin(); iter != ShengLingListEx.end(); ++iter)
	{
		DWORD dwID = VarShengLingDisplay->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagSoulAttName* pShengLingName = FindInMap(m_mapShengLingName, dwID);
		if (P_VALID(pShengLingName))
		{
			_tcsncpy( pShengLingName->szIcon_Low,		VarShengLingDisplay->GetString( _T("icon1"),				(*iter).c_str(), _T("") ), sizeof(pShengLingName->szIcon_Low)/sizeof(TCHAR));
			_tcsncpy( pShengLingName->szIcon_High,		VarShengLingDisplay->GetString( _T("icon2"),				(*iter).c_str(), _T("") ), sizeof(pShengLingName->szIcon_High)/sizeof(TCHAR));
			_tcsncpy( pShengLingName->szMode_Low,		VarShengLingDisplay->GetString( _T("model_1"),				(*iter).c_str(), _T("") ), sizeof(pShengLingName->szMode_Low)/sizeof(TCHAR));
			_tcsncpy( pShengLingName->szMode_High,		VarShengLingDisplay->GetString( _T("model_2"),				(*iter).c_str(), _T("") ), sizeof(pShengLingName->szMode_High)/sizeof(TCHAR));
			_tcsncpy( pShengLingName->szEffect_Low,		VarShengLingDisplay->GetString( _T("effects1"),				(*iter).c_str(), _T("") ), sizeof(pShengLingName->szEffect_Low)/sizeof(TCHAR));
			_tcsncpy( pShengLingName->szEffect_High,	VarShengLingDisplay->GetString( _T("effects2"),				(*iter).c_str(), _T("") ), sizeof(pShengLingName->szEffect_High)/sizeof(TCHAR));
			pShengLingName->dwHighEatNum = VarShengLingDisplay->GetDword(_T("SeniorNomber"),		(*iter).c_str(),	0);
			pShengLingName->dwScal = VarShengLingDisplay->GetDword(_T("size"),		(*iter).c_str(),	100);
		}
	}

	KillObj("ShengLingDisplay");
	//----------------------------------------------------------------------------------
	//圣灵升级表
	//----------------------------------------------------------------------------------
	TObjRef<VarContainer> VarShengLingLevelUp = CreateObj("ShengLingLevelUpProto", "VarContainer");
	if(!VarShengLingLevelUp->Load("VFS_System", szShengLingLeveup, "id", &ShengLingLevelUpList))
		IMSG(_T("Load file HolyGods_growup.xml failed\r\n"));

	for(iter = ShengLingLevelUpList.begin(); iter != ShengLingLevelUpList.end(); ++iter)
	{
		tagHolyLevelUpProto ShengLingLevelUp;
		ShengLingLevelUp.dwID				= VarShengLingLevelUp->GetDword( _T("id"),					(*iter).c_str(), GT_INVALID );
		ShengLingLevelUp.dwDmgMin			= VarShengLingLevelUp->GetDword(_T("dm_min"),				(*iter).c_str(), GT_INVALID);			//伤害最小值
		ShengLingLevelUp.dwDmgMax			= VarShengLingLevelUp->GetDword(_T("dm_max"),				(*iter).c_str(), GT_INVALID);			//伤害最大值
		ShengLingLevelUp.dwDefMin			= VarShengLingLevelUp->GetDword(_T("df_min"),				(*iter).c_str(), GT_INVALID);			//防御最小值
		ShengLingLevelUp.dwDefMax			= VarShengLingLevelUp->GetDword(_T("df_max"),				(*iter).c_str(), GT_INVALID);			//防御最大值
		ShengLingLevelUp.dwLingNengMin		= VarShengLingLevelUp->GetDword(_T("holy_min"),				(*iter).c_str(), GT_INVALID);			//灵能最小值
		ShengLingLevelUp.dwLingNengMax		= VarShengLingLevelUp->GetDword(_T("holy_max"),				(*iter).c_str(), GT_INVALID);			//灵能最大值
		ShengLingLevelUp.dwCritMin			= VarShengLingLevelUp->GetDword(_T("Crit_min"),				(*iter).c_str(), GT_INVALID);			//致命最小值
		ShengLingLevelUp.dwCritMax			= VarShengLingLevelUp->GetDword(_T("Crit_max"),				(*iter).c_str(), GT_INVALID);			//致命最大值
		ShengLingLevelUp.dwCritValueMin		= VarShengLingLevelUp->GetDword(_T("Crit_Rate_min"),		(*iter).c_str(), GT_INVALID);			//致命量最小值
		ShengLingLevelUp.dwCritValueMax		= VarShengLingLevelUp->GetDword(_T("Crit_Rate_max"),		(*iter).c_str(), GT_INVALID);			//致命量最大值
		ShengLingLevelUp.dwDmgAddMin		= VarShengLingLevelUp->GetDword(_T("ExDamage_min"),			(*iter).c_str(), GT_INVALID);			//伤害加深最小值
		ShengLingLevelUp.dwDmgAddMax		= VarShengLingLevelUp->GetDword(_T("ExDamage_max"),			(*iter).c_str(), GT_INVALID);			//伤害加深最大值
		ShengLingLevelUp.dwAttackTecMin		= VarShengLingLevelUp->GetDword(_T("AttackTec_min"),		(*iter).c_str(), GT_INVALID);			//精准最小值
		ShengLingLevelUp.dwAttackTecMax		= VarShengLingLevelUp->GetDword(_T("AttackTec_max"),		(*iter).c_str(), GT_INVALID);			//进准最大值
		ShengLingLevelUp.dwNeglectMin		= VarShengLingLevelUp->GetDword(_T("neglect_min"),			(*iter).c_str(), GT_INVALID);			//灵巧最小值
		ShengLingLevelUp.dwNeglectMax		= VarShengLingLevelUp->GetDword(_T("neglect_max"),			(*iter).c_str(), GT_INVALID);			//灵巧最大值

		m_mapShengLingLevelUp.insert(make_pair(ShengLingLevelUp.dwID, ShengLingLevelUp));
	}

	KillObj("ShengLingLevelUpProto");
	//----------------------------------------------------------------------------------
	//圣纹属性表
	//----------------------------------------------------------------------------------
	TObjRef<VarContainer> VarHolyEquipProto = CreateObj("HolyEquipProto", "VarContainer");
	if(!VarHolyEquipProto->Load("VFS_System", szHolyEquipProto, "id", &HolyEquipProtoList))
		IMSG(_T("Load file holy_equip_proto.xml failed\r\n"));

	for(iter = HolyEquipProtoList.begin(); iter != HolyEquipProtoList.end(); ++iter)
	{
		tagHolyEquipProto HolyEquip;
		HolyEquip.dwTypeID				= VarHolyEquipProto->GetDword(_T("id"),				(*iter).c_str(),		GT_INVALID);
		HolyEquip.n16MaxLapNum			= VarHolyEquipProto->GetDword(_T("max_lap_num"),	(*iter).c_str(),		0);
		HolyEquip.nHolyLevel			= VarHolyEquipProto->GetDword(_T("holy_level"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[0].nValue	= VarHolyEquipProto->GetDword(_T("holy_val_1"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[1].nValue	= VarHolyEquipProto->GetDword(_T("holy_val_2"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[2].nValue	= VarHolyEquipProto->GetDword(_T("holy_val_3"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[3].nValue	= VarHolyEquipProto->GetDword(_T("holy_val_4"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[4].nValue	= VarHolyEquipProto->GetDword(_T("holy_val_5"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[5].nValue	= VarHolyEquipProto->GetDword(_T("holy_val_6"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[6].nValue	= VarHolyEquipProto->GetDword(_T("holy_val_7"),		(*iter).c_str(),		0);

		HolyEquip.BaseEffect[0].eHolyAtt	= (EHolyAttType)VarHolyEquipProto->GetDword(_T("holy_att_1"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[1].eHolyAtt	= (EHolyAttType)VarHolyEquipProto->GetDword(_T("holy_att_2"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[2].eHolyAtt	= (EHolyAttType)VarHolyEquipProto->GetDword(_T("holy_att_3"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[3].eHolyAtt	= (EHolyAttType)VarHolyEquipProto->GetDword(_T("holy_att_4"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[4].eHolyAtt	= (EHolyAttType)VarHolyEquipProto->GetDword(_T("holy_att_5"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[5].eHolyAtt	= (EHolyAttType)VarHolyEquipProto->GetDword(_T("holy_att_6"),		(*iter).c_str(),		0);
		HolyEquip.BaseEffect[6].eHolyAtt	= (EHolyAttType)VarHolyEquipProto->GetDword(_T("holy_att_7"),		(*iter).c_str(),		0);
	
		m_mapHolyEquipProto.insert(make_pair(HolyEquip.dwTypeID, HolyEquip));
	}

	KillObj("HolyEquipProto");
	//----------------------------------------------------------------------------------
	//圣纹强化道具表
	//----------------------------------------------------------------------------------
	TObjRef<VarContainer> VarHolyEquipEnhance = CreateObj("HolyEquipEnhance", "VarContainer");
	if(!VarHolyEquipEnhance->Load("VFS_System", szHolyEquipEnhance, "id", &HolyEquipEnhanceList))
		IMSG(_T("Load file Holy_empowerment.xml failed\r\n"));

	for(iter = HolyEquipEnhanceList.begin(); iter != HolyEquipEnhanceList.end(); ++iter)
	{
		tagHolyEquipEnhance HolyEquipEnhance;
		HolyEquipEnhance.dwID				= VarHolyEquipEnhance->GetDword(_T("id"),					(*iter).c_str(),		GT_INVALID);					//id
		HolyEquipEnhance.dwPowMin[0]		= VarHolyEquipEnhance->GetDword(_T("DmPowMin"),				(*iter).c_str(),				0);			//伤害最小值
		HolyEquipEnhance.dwPowMax[0]		= VarHolyEquipEnhance->GetDword(_T("DmPowMax"),				(*iter).c_str(),				0);			//伤害最大值
		HolyEquipEnhance.dwPowMin[1]		= VarHolyEquipEnhance->GetDword(_T("DfPowMin"),				(*iter).c_str(),				0);			//防御最小值
		HolyEquipEnhance.dwPowMax[1]		= VarHolyEquipEnhance->GetDword(_T("DfPowMax"),				(*iter).c_str(),				0);			//防御最大值
		HolyEquipEnhance.dwPowMin[2]		= VarHolyEquipEnhance->GetDword(_T("CritPowMin"),			(*iter).c_str(),				0);			//致命最小值
		HolyEquipEnhance.dwPowMax[2]		= VarHolyEquipEnhance->GetDword(_T("CritPowMax"),			(*iter).c_str(),				0);			//致命最大值
		HolyEquipEnhance.dwPowMin[3]		= VarHolyEquipEnhance->GetDword(_T("CritRatePowMin"),		(*iter).c_str(),				0);		//致命量最小值
		HolyEquipEnhance.dwPowMax[3]		= VarHolyEquipEnhance->GetDword(_T("CritRatePowMax"),		(*iter).c_str(),				0);		//致命量最大值
		HolyEquipEnhance.dwPowMin[4]		= VarHolyEquipEnhance->GetDword(_T("ExDmPowMin"),			(*iter).c_str(),				0);		//伤害加深最小值
		HolyEquipEnhance.dwPowMax[4]		= VarHolyEquipEnhance->GetDword(_T("ExDmPowMax"),			(*iter).c_str(),				0);		//伤害加深最大值
		HolyEquipEnhance.dwPowMin[5]		= VarHolyEquipEnhance->GetDword(_T("AttTecPowMin"),			(*iter).c_str(),				0);	//精准最小值
		HolyEquipEnhance.dwPowMax[5]		= VarHolyEquipEnhance->GetDword(_T("AttTecPowMax"),			(*iter).c_str(),				0);	//进准最大值
		HolyEquipEnhance.dwPowMin[6]		= VarHolyEquipEnhance->GetDword(_T("NegPowMin"),			(*iter).c_str(),				0);		//灵巧最小值
		HolyEquipEnhance.dwPowMax[6]		= VarHolyEquipEnhance->GetDword(_T("NegPowMax"),			(*iter).c_str(),				0);		//灵巧最大值
		HolyEquipEnhance.dwLingNengPow		= VarHolyEquipEnhance->GetDword(_T("HolyPointPow"),			(*iter).c_str(),				0);	//减少的消耗灵能
		
		m_mapHolyEquipEnhance.insert(make_pair(HolyEquipEnhance.dwID, HolyEquipEnhance));
	}

	KillObj("HolyEquipEnhance");

	//----------------------------------------------------------------------------------
	//战场奖励表
	//----------------------------------------------------------------------------------
	TObjRef<VarContainer> VarWarReward = CreateObj("WarReward", "VarContainer");
	if(!VarWarReward->Load("VFS_System", szWarReward, "id", &WarRewardList))
		IMSG(_T("Load file Rewards_point.xml failed\r\n"));

	for(iter = WarRewardList.begin(); iter != WarRewardList.end(); ++iter)
	{
		tagWarReward WarReward;
		WarReward.dwID				= VarWarReward->GetDword(_T("id"),					(*iter).c_str(),		GT_INVALID);					//id
		WarReward.dwExpReward		= VarWarReward->GetDword(_T("exp_point"),			(*iter).c_str(),		GT_INVALID);
		WarReward.dwMoneyReward		= VarWarReward->GetDword(_T("money_point"),			(*iter).c_str(),		GT_INVALID);
		m_mapWarReward.insert(make_pair(WarReward.dwID, WarReward));
	}

	KillObj("WarReward");
}

tstring ItemProtoData::FindTrumpAwardByID( DWORD dwID )
{
	map<DWORD, tagTrumpWuXingAward>::const_iterator iter;
	iter = m_mapTrumpWuXingAward.find(dwID);
	if( iter != m_mapTrumpWuXingAward.end() )
		return iter->second.szDesc;
	else
		return _T("");
}

tstring ItemProtoData::GetFairyEffect( DWORD dwTypeID, INT nLevel )
{
	map<DWORD, tagFairyEffect>::const_iterator iter = m_mapFairyEffect.find(dwTypeID);
	if( iter != m_mapFairyEffect.end() )
	{
		if( nLevel == 100 )
			return iter->second.szFairyShape[MAX_FAIRY_SHAPE-1];
		else
			return iter->second.szFairyShape[nLevel/10];
	}
	else
		return _T("");
}

tstring ItemProtoData::GetFairyPic( DWORD dwTypeID, INT nLevel )
{
	map<DWORD, tagFairyEffect>::const_iterator iter = m_mapFairyEffect.find(dwTypeID);
	if( iter != m_mapFairyEffect.end() )
	{
		if( nLevel < 20 )
			return iter->second.szFirstPic;
		else if( nLevel < 60 )
			return iter->second.szSecondPic;
		else
			return iter->second.szThirdPic;
	}
	else
		return _T("");
}

const tagPetEquipProto* ItemProtoData::FindPetEquipByItemID( DWORD dwID )
{
    const tagItemProto* pItem = FindItemProto(dwID);
    if (P_VALID(pItem))
    {
        return FindPetEquipByEquipID(pItem->nSpecFuncVal1);
    }
    else
    {
        return NULL;
    }
}

const tagPetEquipProto* ItemProtoData::FindPetEquipByEquipID( DWORD dwID )
{
    map<DWORD, tagPetEquipProto>::const_iterator it;
    it = m_mapPetEquipProtoByID.find(dwID);
    if (it != m_mapPetEquipProtoByID.end())
    {
        return &it->second;
    }
    else
    {
        return NULL;
    }
}

ESuitAttType ItemProtoData::FindFashionSuitType( DWORD dwTypeID )
{
    map<DWORD, ESuitAttType>::iterator it = m_mapSpinSuitType.find(dwTypeID);
    if (it != m_mapSpinSuitType.end())
    {
        return it->second;
    }

    return ESSAT_Null;
}

ESpinStep ItemProtoData::FindFashionSpinStep( DWORD dwTypeID )
{
    set<DWORD>::iterator itBasic = m_SetSpinBasic.find(dwTypeID);
    if (itBasic != m_SetSpinBasic.end())
    {
        return ESpinStep_Basic;
    }

    set<DWORD>::iterator itMiddle = m_setSpinMiddle.find(dwTypeID);
    if (itMiddle != m_setSpinMiddle.end())
    {
        return ESpinStep_Middle;
    }

    set<DWORD>::iterator itSenior = m_setSpinSenior.find(dwTypeID);
    if (itSenior != m_setSpinSenior.end())
    {
        return ESpinStep_Senior;
    }

    return ESpinStep_Null;
}

DWORD ItemProtoData::FindNextFashionSpinID( DWORD dwTypeID, ESuitAttType eType )
{
    map<pair<DWORD, ESuitAttType>, DWORD>::iterator it = m_mapSpinNull.find(make_pair(dwTypeID, eType));
    if (it != m_mapSpinNull.end())
    {
        return it->second;
    }

    it = m_mapSpinBasic.find(make_pair(dwTypeID, eType));
    if (it != m_mapSpinBasic.end())
    {
        return it->second;
    }

    it = m_mapSpinMiddle.find(make_pair(dwTypeID, eType));
    if (it != m_mapSpinMiddle.end())
    {
        return it->second;
    }

    return GT_INVALID;
}

int ItemProtoData::FindFashionSpinRate( int itemCount, ESpinStep eStep )
{
    map<pair<int, ESpinStep>, int>::iterator it = m_mapSpinRate.find(make_pair(itemCount, eStep));
    if (it != m_mapSpinRate.end())
    {
        return it->second;
    }

    return 0;
}

const tagFairyFruit* ItemProtoData::FindFairyFruit(DWORD dwTypeID)
{
	map<DWORD, tagFairyFruit>::const_iterator iter = m_mapFairyFruit.find(dwTypeID);
	if( iter != m_mapFairyFruit.end() )
		return &iter->second;
	return NULL;
}

const tagFairySkill* ItemProtoData::FindFairySkill(DWORD dwTypeID)
{
	map<DWORD, tagFairySkill>::const_iterator iter = m_mapFairySkill.find(dwTypeID);
	if( iter != m_mapFairySkill.end() )
		return &iter->second;
	return NULL;
}

const tagFairyUpgradeTimes* ItemProtoData::FindFairyUpgradeTimes(DWORD dwTypeID)//add by xp
{
	map<DWORD, tagFairyUpgradeTimes>::const_iterator iter = m_mapFairyUpgradeTimes.find(dwTypeID);
	if( iter != m_mapFairyUpgradeTimes.end() )
		return &iter->second;
	return NULL;
}//add end