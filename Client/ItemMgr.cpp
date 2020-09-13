#include "StdAfx.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "Container.h"
#include "Item.h"
#include "ItemProtoData.h"
#include "ItemEvent.h"
#include "ItemFrame.h"
#include "StorageFrame.h"
#include "..\WorldDefine\msg_func_npc.h"
#include "QuestMgr.h"
#include "CurrencyMgr.h"
#include "CombatSysUtil.h"
#include "LocalPlayer.h"
#include "MallMgr.h"
#include "..\WorldDefine\msg_spec_item.h"
#include "..\WorldDefine\msg_compose.h"
#include "EffectMgr.h"
#include "OldPlayerComeBackFrame.h"
#include "..\WorldDefine\msg_activity.h"
#include "..\WorldDefine\msg_player_preventlost.h"
#include "..\WorldDefine\msg_fashion.h"
#include "ItemTimerMgr.h"
#include "ClothespressData.h"
#include ".\RoleEvent.h"
#include "HolyManMgr.h"
#include "SaveAccount.h"

ItemMgr::ItemMgr(void):m_Trunk(this)
{
	m_pPocket		= NULL;
	m_pEquipment	= NULL;
	m_pStorge		= NULL;
	m_pThrone		= NULL;
	m_pQuest		= NULL;
    m_pGuildStore   = NULL;
	m_pClothespress = NULL;
	m_pTempContainer = NULL;
	m_pWarBag		= NULL;

	m_bNotEnoughYuanbao = false;
	m_bNotEnoughSilver	= false;
	m_bUseSilverExtend	= false;
	m_bNotInitBaiBaoRecord	= true;
	m_eEncryptState			= EPES_Null;
}

ItemMgr::~ItemMgr(void)
{
}

ItemMgr g_ItemMgr;
ItemMgr* ItemMgr::Inst()
{
	return &g_ItemMgr;
}

VOID ItemMgr::Update()
{

}

VOID ItemMgr::Init()
{
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pUtil	= TObjRef<Util>();

	m_pPocket = new ItemContainer(EICT_Bag,SPACE_ONE_BAG,SPACE_ONE_BAG);
	m_pEquipment = new ItemContainer(EICT_Equip,EEP_End,EEP_End);
	m_pStorge = new ItemContainer(EICT_RoleWare,SPACE_ONE_WARE,SPACE_ONE_WARE);
	m_pThrone = new ItemContainer(EICT_Baibao,SPACE_BAIBAO_BAG,SPACE_BAIBAO_BAG);
	m_pQuest = new ItemContainer(EICT_Quest,SPACE_QUEST_BAG,SPACE_QUEST_BAG);
    m_pGuildStore = new ItemContainer(EICT_GuildWare, SPACE_ONE_GUILDWARE, MAX_GUILDWARE_NUM);
	m_pClothespress = new ItemContainer(EICT_Wardrobe, SPACE_ONE_GUILDWARE, MAX_GUILDWARE_NUM);
	m_pTempContainer = new ItemContainer(EICT_TempBag, 1,1);	//就给一个空间
	m_pShengLingEquip = new ItemContainer(EICT_HolyManEquip, SHENGLING_EQUIP_BLANK, 1);
	m_pWarBag = new ItemContainer(EICT_WarBag, SPACE_WAR_BAG, 1);

	m_mapSuitNum.Clear();
	m_mapLonghunOn.Clear();
	m_mapCurStartTime.Clear();
	// 百宝袋记录初始化
	m_bNotInitBaiBaoRecord = true;
	m_listBaiBaoRecord.Clear();
	LoadBeGoldStoneItemTable();
	LoadTrumpLevelTable();
	
	m_pCmdMgr->Register("NS_DressUpAnotherRole",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_NetDressUpAnotherRole),	_T("NS_DressUpAnotherRole") );
	m_pCmdMgr->Register("NS_SyncFashionInfo",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_UpdateFashionInfo),		_T("NS_SyncFashionInfo") );
	m_pCmdMgr->Register("NS_PlayerRegression",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_PlayerRegression),		_T("NS_PlayerRegression") );
	m_pCmdMgr->Register("NS_GetRoleInitState_Item",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerItem),		_T("NS_GetRoleInitState_Item"));
	m_pCmdMgr->Register("NS_GetRoleInitState_ItemCDTime",	(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerItemCD),	_T("NS_GetRoleInitState_ItemCDTime"));
	m_pCmdMgr->Register("NS_GetRoleInitState_Suit",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerSuit),		_T("NS_GetRoleInitState_Suit"));
	m_pCmdMgr->Register("NS_GetRoleInitState_Longhun",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerLonghun),	_T("NS_GetRoleInitState_Longhun"));
		
	m_pCmdMgr->Register("NS_Equip",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipment),			_T("NS_Equip"));
	m_pCmdMgr->Register("NS_Unequip",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetUnEquipment),			_T("NS_Unequip"));
	m_pCmdMgr->Register("NS_SwapWeapon",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSwapWeapon),			_T("NS_SwapWeapon"));
	
	m_pCmdMgr->Register("NS_ItemPosChange",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemPosChange),		_T("NS_ItemPosChange"));
	m_pCmdMgr->Register("NS_ItemPosChangeEx",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemPosChangeEx),		_T("NS_ItemPosChangeEx"));
	m_pCmdMgr->Register("NS_NewItemAdd",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewItemAdd),			_T("NS_NewItemAdd"));
	m_pCmdMgr->Register("NS_NewEquipAdd",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewEquipAdd),			_T("NS_NewEquipAdd"));
	m_pCmdMgr->Register("NS_NewFabaoAdd",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewTrumpAdd),			_T("NS_NewFabaoAdd"));
	m_pCmdMgr->Register("NS_NewHolyAdd",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewHolyAdd),			_T("NS_NewHolyAdd"));
	m_pCmdMgr->Register("NS_NewHolyEquipAdd",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewHolyEquipAdd),		_T("NS_NewHolyEquipAdd"));
	m_pCmdMgr->Register("NS_ItemAdd",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemAdd),				_T("NS_ItemAdd"));
	m_pCmdMgr->Register("NS_ItemRemove",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemRemove),			_T("NS_ItemRemove"));
	m_pCmdMgr->Register("NS_ItemBind",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemBind),			_T("NS_ItemBind"));
	m_pCmdMgr->Register("NS_ItemBindStatus",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemBindStatus),		_T("NS_ItemBindStatus"));
	
	m_pCmdMgr->Register("NS_EquipChange",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipChange),			_T("NS_EquipChange"));
	m_pCmdMgr->Register("NS_EquipSingleChange",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipSingleChange),			_T("NS_EquipSingleChange"));
	m_pCmdMgr->Register("NS_SoulCrystalChg",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetHunJingChange),			_T("NS_SoulCrystalChg"));
	m_pCmdMgr->Register("NS_FabaoChange",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetTrumpChange),			_T("NS_FabaoChange"));
	m_pCmdMgr->Register("NS_ItemCDUpdate",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemCDUpdate),		_T("NS_ItemCDUpdate"));
	m_pCmdMgr->Register("NS_ShengLingChg",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetShengLingChange),		_T("NS_ShengLingChg"));
	m_pCmdMgr->Register("NS_HolyEquipChg",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetShengLingEquipChange), _T("NS_HolyEquipChg"));

	m_pCmdMgr->Register("NS_BagExtend",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetBagExtend),			_T("NS_BagExtend"));
	m_pCmdMgr->Register("NS_WareExtend",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetWareExtend),			_T("NS_WareExtend"));
	
	m_pCmdMgr->Register("NS_SuitNum",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSuitNumChange),		_T("NS_SuitNum"));
	m_pCmdMgr->Register("NS_LongHunOn",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipLonghunOn),		_T("NS_LongHunOn"));
	m_pCmdMgr->Register("NS_LongHunOff",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipLonghunOff),		_T("NS_LongHunOff"));
	
	m_pCmdMgr->Register("NS_IdentifyEquip",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetIdentifyEquip),		_T("NS_IdentifyEquip"));
	m_pCmdMgr->Register("NS_Abrase",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetAbraseWeapon),		_T("NS_Abrase"));
	m_pCmdMgr->Register("NS_RWNewessChange",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetWeaponNewnessChange), _T("NS_RWNewessChange"));
    m_pCmdMgr->Register("NS_DyeFashion",		    (NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetDyeFashion),          _T("NS_DyeFashion"));
	
	m_pCmdMgr->Register("NS_BagPsd",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetBagPassword),			_T("NS_BagPsd"));
	
	m_pCmdMgr->Register("NS_ItemReorder",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetContainerReorder),	_T("NS_ItemReorder"));
	m_pCmdMgr->Register("NS_ItemReorderEx",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetContainerReorderEx),  _T("NS_ItemReorderEx"));

	m_pCmdMgr->Register("NS_InitBaiBaoRecord",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitBaiBaoRecord),	_T("NS_InitBaiBaoRecord"));
	m_pCmdMgr->Register("NS_SingleBaiBaoRecord",	(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSingleBaiBaoRecord),  _T("NS_SingleBaiBaoRecord"));
	m_pCmdMgr->Register("NS_WareOpen",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetWareOpen),			_T("NS_WareOpen") );
	m_pCmdMgr->Register("NS_BeGoldStone",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetBeGoldStone),			_T("NS_BeGoldStone"));
	m_pCmdMgr->Register("NS_RaiseEquipPotval",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetUpgradeMaxPot),		_T("NS_RaiseEquipPotval"));
	m_pCmdMgr->Register("NS_DetachOrAttackSoul_Equip",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSoulChange),	_T("NS_DetachOrAttackSoul_Equip"));
	m_pCmdMgr->Register("NS_GuiYuan",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetGuiYuan),				_T("NS_GuiYuan"));
	m_pCmdMgr->Register("NS_FairyPowder",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetFairyPowder),			_T("NS_FairyPowder"));
	
	m_pCmdMgr->Register("NS_LockItem",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_LockItem),			_T("NS_LockItem"));
	m_pCmdMgr->Register("NS_UnlockItemNotice",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_UnLockItem),			_T("NS_UnlockItemNotice"));
	m_pCmdMgr->Register("NS_EquipSignature",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_EquipSignature),	_T("NS_EquipSignature") );
	m_pCmdMgr->Register("NS_SyncOpenChaseRemainTime",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_SyncOpenChaseRemainTime),	_T("NS_SyncOpenChaseRemainTime") );
	m_pCmdMgr->Register("NS_ForceLootChase",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_ForceLootChase),	_T("NS_ForceLootChase") );

	m_pCmdMgr->Register("NS_FairyRaiseIntelligenceMax",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_FairyRaiseIntelligenceMax),	_T("NS_FairyRaiseIntelligenceMax") );
	m_pCmdMgr->Register("NS_FairyDoRaiseIntelligence",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_FairyDoRaiseIntelligence),	_T("NS_FairyDoRaiseIntelligence") );
	m_pCmdMgr->Register("NS_NewSoulCrystalAdd",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_NewSoulCrystalAdd),	_T("NS_NewSoulCrystalAdd") );
	m_pCmdMgr->Register("NS_DiamondEquipGrowIM",	(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_DiamondEquipGrowIM), _T("NS_DiamondEquipGrowIM"));
	m_pCmdMgr->Register("NS_EquipPerfectGrow",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_EquipPerfectGrow), _T("NS_EquipPerfectGrow"));
	m_pCmdMgr->Register("NS_XiPurpleStar",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_XiPurpleStar), _T("NS_XiPurpleStar"));
	m_pCmdMgr->Register("NS_ShengLingCoValueChg",	(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_ShengLingCoValueChg), _T("NS_ShengLingCoValueChg"));

	m_pFrameMgr->RegisterEventHandle(_T("Bag_Extend"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventBagExtend));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_BagExtend"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventBagExtendCheck));
	m_pFrameMgr->RegisterEventHandle(_T("Ware_Extend"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventWareExtend));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_WareExtend"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventWareExtendCheck));
}

VOID ItemMgr::Destroy()
{
	if(NULL != m_pPocket)
	{
		m_pPocket->Destroy();
		SAFE_DEL(m_pPocket);
	}
	if(NULL != m_pThrone)
	{
		m_pThrone->Destroy();
		SAFE_DEL(m_pThrone);
	}
	if(NULL != m_pEquipment)
	{
		m_pEquipment->Destroy();
		SAFE_DEL(m_pEquipment);
	}
	if(NULL != m_pStorge)
	{
		m_pStorge->Destroy();
		SAFE_DEL(m_pStorge);
	}
	if(NULL != m_pQuest)
	{
		m_pQuest->Destroy();
		SAFE_DEL(m_pQuest);
	}
    if (NULL != m_pGuildStore)
    {
        m_pGuildStore->Destroy();
        SAFE_DEL(m_pGuildStore);
    }
	if (NULL != m_pClothespress)
	{
		m_pClothespress->Destroy();
		SAFE_DEL(m_pClothespress);
	}

	if(NULL != m_pTempContainer)
	{
		m_pTempContainer->Destroy();
		SAFE_DEL(m_pTempContainer);
	}

	if(NULL != m_pWarBag)
	{
		m_pWarBag->Destroy();
		SAFE_DEL(m_pWarBag);
	}

	if(P_VALID(GetObj("GoldItemInfo")))
		KillObj("GoldItemInfo");

	if(P_VALID(GetObj("TrumpLevel")))
		KillObj("TrumpLevel");

	if( P_VALID(GetObj("XiHunSuccess")) )
		KillObj("XiHunSuccess");

	if( P_VALID(GetObj("FuHunSuccess")) )
		KillObj("FuHunSuccess");

	if(P_VALID(GetObj("BaoXiangForceLost")))
		KillObj("BaoXiangForceLost");

	m_pCmdMgr->UnRegister("NS_DressUpAnotherRole",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_NetDressUpAnotherRole));
	m_pCmdMgr->UnRegister("NS_SyncFashionInfo",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_UpdateFashionInfo));
	m_pCmdMgr->UnRegister("NS_PlayerRegression",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_PlayerRegression));
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Item",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerItem));
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_ItemCDTime",	(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerItemCD));
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Suit",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerSuit));
	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Longhun",	(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitLocalPlayerLonghun));
	
	m_pCmdMgr->UnRegister("NS_Equip",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipment));
	m_pCmdMgr->UnRegister("NS_Unequip",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetUnEquipment));
	m_pCmdMgr->UnRegister("NS_SwapWeapon",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSwapWeapon));

	m_pCmdMgr->UnRegister("NS_ItemPosChange",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemPosChange));
	m_pCmdMgr->UnRegister("NS_ItemPosChangeEx",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemPosChangeEx));
	m_pCmdMgr->UnRegister("NS_NewItemAdd",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewItemAdd));
	m_pCmdMgr->UnRegister("NS_NewEquipAdd",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewEquipAdd));
	m_pCmdMgr->UnRegister("NS_NewFabaoAdd",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewTrumpAdd));
	m_pCmdMgr->UnRegister("NS_NewHolyAdd",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewHolyAdd));
	m_pCmdMgr->UnRegister("NS_NewHolyEquipAdd",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetNewHolyEquipAdd));
	m_pCmdMgr->UnRegister("NS_ItemAdd",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemAdd));
	m_pCmdMgr->UnRegister("NS_ItemRemove",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemRemove));
	m_pCmdMgr->UnRegister("NS_ItemBind",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemBind));
	m_pCmdMgr->UnRegister("NS_ItemBindStatus",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemBindStatus));
	m_pCmdMgr->UnRegister("NS_EquipChange",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipChange));
	m_pCmdMgr->UnRegister("NS_EquipSingleChange",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipSingleChange));
	m_pCmdMgr->UnRegister("NS_SoulCrystalChg",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetHunJingChange));
	m_pCmdMgr->UnRegister("NS_FabaoChange",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetTrumpChange));
	m_pCmdMgr->UnRegister("NS_ItemCDUpdate",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetItemCDUpdate));
	m_pCmdMgr->UnRegister("NS_ShengLingChg",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetShengLingChange));
	m_pCmdMgr->UnRegister("NS_HolyEquipChg",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetShengLingEquipChange));

	m_pCmdMgr->UnRegister("NS_BagExtend",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetBagExtend));
	m_pCmdMgr->UnRegister("NS_WareExtend",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetWareExtend));

	m_pCmdMgr->UnRegister("NS_SuitNum",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSuitNumChange));
	m_pCmdMgr->UnRegister("NS_LongHunOn",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipLonghunOn));
	m_pCmdMgr->UnRegister("NS_LongHunOff",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetEquipLonghunOff));

	m_pCmdMgr->UnRegister("NS_IdentifyEquip",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetIdentifyEquip));
	m_pCmdMgr->UnRegister("NS_Abrase",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetAbraseWeapon));
	m_pCmdMgr->UnRegister("NS_RWNewessChange",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetWeaponNewnessChange));
	m_pCmdMgr->UnRegister("NS_DyeFashion",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetDyeFashion));
	
	m_pCmdMgr->UnRegister("NS_BagPsd",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetBagPassword));
	
	m_pCmdMgr->UnRegister("NS_ItemReorder",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetContainerReorder));
	m_pCmdMgr->UnRegister("NS_ItemReorderEx",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetContainerReorderEx));

	m_pCmdMgr->UnRegister("NS_InitBaiBaoRecord",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetInitBaiBaoRecord));
	m_pCmdMgr->UnRegister("NS_SingleBaiBaoRecord",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSingleBaiBaoRecord));
	m_pCmdMgr->UnRegister("NS_WareOpen",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetWareOpen) );
	m_pCmdMgr->UnRegister("NS_BeGoldStone",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetBeGoldStone) );
	m_pCmdMgr->UnRegister("NS_RaiseEquipPotval",		(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetUpgradeMaxPot) );
	m_pCmdMgr->UnRegister("NS_DetachOrAttackSoul_Equip",(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetSoulChange) );
	m_pCmdMgr->UnRegister("NS_GuiYuan",					(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetGuiYuan) );
	m_pCmdMgr->UnRegister("NS_FairyPowder",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::NetFairyPowder));

	m_pCmdMgr->UnRegister("NS_LockItem",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_LockItem) );
	m_pCmdMgr->UnRegister("NS_UnlockItemNotice",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_UnLockItem) );
	m_pCmdMgr->UnRegister("NS_EquipSignature",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_EquipSignature) );
	m_pCmdMgr->UnRegister("NS_SyncOpenChaseRemainTime",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_SyncOpenChaseRemainTime) );
	m_pCmdMgr->UnRegister("NS_ForceLootChase",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_ForceLootChase) );

	m_pCmdMgr->UnRegister("NS_FairyRaiseIntelligenceMax",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_FairyRaiseIntelligenceMax) );
	m_pCmdMgr->UnRegister("NS_FairyDoRaiseIntelligence",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_FairyDoRaiseIntelligence) );
	m_pCmdMgr->UnRegister("NS_NewSoulCrystalAdd",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_NewSoulCrystalAdd) );
	m_pCmdMgr->UnRegister("NS_DiamondEquipGrowIM",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_DiamondEquipGrowIM));
	m_pCmdMgr->UnRegister("NS_EquipPerfectGrow",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_EquipPerfectGrow));
	m_pCmdMgr->UnRegister("NS_XiPurpleStar",				(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_XiPurpleStar));
	m_pCmdMgr->UnRegister("NS_ShengLingCoValueChg",			(NETMSGPROC)m_Trunk.sfp2(&ItemMgr::OnNS_ShengLingCoValueChg));

	m_pFrameMgr->UnRegisterEventHandler(_T("Bag_Extend"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventBagExtend));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_BagExtend"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventBagExtendCheck));
	m_pFrameMgr->UnRegisterEventHandler(_T("Ware_Extend"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventWareExtend));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_WareExtend"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemMgr::OnEventWareExtendCheck));

	TMap<DWORD, tagFashionInfo*>::TMapIterator end = m_mapFashion.End();
	for (TMap<DWORD, tagFashionInfo*>::TMapIterator iter = m_mapFashion.Begin(); iter != end; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	m_mapFashion.Clear();
}

tstring ItemMgr::GetItemIconPath(DWORD dwTypeID)
{
	TCHAR szPath[X_LONG_NAME] = {0};
	const tagItemDisplayInfo* pDisplay = ItemProtoData::Inst()->FindItemDisplay(dwTypeID);
	if(!P_VALID(pDisplay))
		return szPath;
	if(MIsEquipment(dwTypeID))
	{
		const tagItemProto *pProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
		if(P_VALID(pProto))
		{
			if(pProto->eSexLimit == ESL_Man)
				_sntprintf(szPath, sizeof(szPath), _T("data\\ui\\Icon\\%s.tga"), pDisplay->szMaleIcon);
			else
				_sntprintf(szPath, sizeof(szPath), _T("data\\ui\\Icon\\%s.tga"), pDisplay->szFemaleIcon);
		}
	}
	else
		_sntprintf(szPath, sizeof(szPath), _T("data\\ui\\Icon\\%s.tga"), pDisplay->szMaleIcon);
	
	return szPath;
}


DWORD ItemMgr::NetInitLocalPlayerItem(tagNS_GetRoleInitState_Item* pNetCmd,DWORD)
{
	size_t nNum = ClothespressData::Inst()->GetAllFashionNum();
	m_pPocket->SetMaxSize(pNetCmd->n16SzBag);
	m_pStorge->SetMaxSize(pNetCmd->n16SzRoleWare);
	m_pClothespress->SetMaxSize(nNum);
	//接收前先清空数据
	m_pPocket->Destroy();
	m_pEquipment->Destroy();
	m_pStorge->Destroy();
	m_pThrone->Destroy();
	m_pQuest->Destroy();
	m_pClothespress->Destroy();
	m_pTempContainer->Destroy();

	for(INT i=0,pos=0; i<pNetCmd->nNum; ++i)
	{
		tagItem item;
		memcpy(&item, pNetCmd->byData+pos, sizeof(tagItem));
		if(MIsEquipment(item.dwTypeID))
		{
			const tagItemProto *pProto = ItemProtoData::Inst()->FindItemProto(item.dwTypeID);
			if(!P_VALID(pProto))
				return 0;
			if( pProto->eType == EIT_FaBao )
			{
				tagFabao trump;
				memcpy(&trump, pNetCmd->byData+pos, sizeof(tagFabao));
				pos += sizeof(tagFabao);
				Equipment* pTrump = new Equipment(trump);
				AddNewItem(pTrump);
			}
			else if (pProto->eType == EIT_SoulCrystal)
			{
				tagSoulCrystal hunjing;
				memcpy(&hunjing, pNetCmd->byData+pos, sizeof(tagSoulCrystal));
				pos += sizeof(tagSoulCrystal);
				Equipment* pTrump = new Equipment(hunjing);
				AddNewItem(pTrump);
			}
			else if(pProto->eType == EIT_Holy)
			{
				tagHolyMan holyman;
				memcpy(&holyman, pNetCmd->byData+pos, sizeof(tagHolyMan));
				pos += sizeof(tagHolyMan);
				Equipment* pHolyMan = new Equipment(holyman);
				AddNewItem(pHolyMan);
			}
			else if(pProto->eType == EIT_HolyEquip)
			{
				tagHolyEquip holyEquip;
				memcpy(&holyEquip, pNetCmd->byData+pos, sizeof(tagHolyEquip));
				pos += sizeof(tagHolyEquip);
				Equipment* pHolyEquip = new Equipment(holyEquip);
				AddNewItem(pHolyEquip);
			}
			else
			{
				tagEquip equip;
				memcpy(&equip, pNetCmd->byData+pos, sizeof(tagEquip));
				pos += sizeof(tagEquip);
				Equipment* pEquip = new Equipment(equip);
				AddNewItem(pEquip);
			}	
		}
		else
		{
			pos += sizeof(tagItem);
			Item* pItem = new Item(item);
			AddNewItem(pItem);
		}
	}
	
	// 物品更新后发送武器切换，方便快捷栏同步
	tagGameEvent e(_T("Weapon_Swap"), NULL);
	m_pFrameMgr->SendEvent(&e);


	tagNC_GetRoleInitState m;
	m.eType = ERIT_FrindAndEnemy;
	m_pSession->Send(&m);

	//在这里判断一下用不用填手机号吧~
	if(SaveAccount::Inst()->GetIsNeedSetTelNum())
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("PlayerTelFrame"), _T("PlayerTelFrame"), 0);
	}
	return 0;
}


DWORD ItemMgr::NetInitLocalPlayerItemCD( tagNS_GetRoleInitState_ItemCDTime* pNetCmd, DWORD )
{
	m_mapCurStartTime.Clear();

	for(INT i=0,pos=0; i<pNetCmd->nNum; ++i)
	{
		tagCDTime time;
		memcpy(&time, pNetCmd->byData+pos, sizeof(tagCDTime));
		const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(time.dwTypeID);
		if(P_VALID(pProto))
		{
			// 计算当前冷却时间和该物品冷却时间的差值，计算物品的开始计时间
			INT nIncMilliSecond = (INT)(time.dwTime - pProto->dwCooldownTime);
			DWORD dwStartTime = Kernel::Inst()->GetAccumTimeDW() + nIncMilliSecond;
			m_mapCurStartTime.Add(time.dwTypeID, dwStartTime);
		}
		
		pos += sizeof(tagCDTime);
	}

	return 0;
}



VOID ItemMgr::AddNewItem( Item* pItem )
{
	BOOL bRet = FALSE;// 添加物品的结果

	ItemContainer* pItemCon = GetContainer(pItem->GetConType());
	
	if(P_VALID(pItemCon))
	{
		bRet = pItemCon->Add(pItem);
	}
	
	if(bRet)// 添加成功发送游戏事件
	{
		tagUpdateLPItemEvent e(_T("ItemEvent_UpdateItem"),NULL);
		e.eType = pItem->GetConType();
		e.n16Index = pItem->GetPos();
		e.dwTypeID = pItem->GetItemTypeID();
		m_pFrameMgr->SendEvent(&e);

		const tagItemProto* pItemProto = pItem->GetProto();
		if(P_VALID(pItemProto)	&& pItemProto->eSpecFunc == EISF_ZhanDouFu && pItemProto->nSpecFuncVal1 == EAFSFV2_OMG)
			ItemTimerMgr::Inst()->SetItemRemainTime(pItem->GetItemId(),pItem->GetData()->dwSpecVal1);
	}
	else // 没有成功则删除物品
	{
		SAFE_DEL(pItem);

		// 发送给服务器
	}
}

DWORD ItemMgr::NetEquipment( tagNS_Equip* pNetCmd, DWORD )
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(plp))
			return 0;

		Equipment* m_pEquip = GetCurEquipByID(pNetCmd->n64Serial);
		if (P_VALID(m_pEquip))
		{
			const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay( m_pEquip->GetProto()->dwTypeID );
			const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto( m_pEquip->GetProto()->dwTypeID );
			if (!P_VALID(pInfo) && !P_VALID(pProto))
				return 0;

			BYTE level = m_pEquip->GetEquip()->byFlareVal;

			tstring strEffectName = EffectMgr::Inst()->GetGlowName( pInfo->szBladeGlowSFX,level );

			if (EEP_Head == pProto->eEquipPos)
			{	
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 Head") ,EffectMgr::EBET_Head);
			}
			else if (EEP_Body == pProto->eEquipPos)
			{
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 Spine2") ,EffectMgr::EBET_Chest);
			}
			else if (EEP_Legs == pProto->eEquipPos)
			{
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 R Calf") ,EffectMgr::EBET_Leg_R);
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 L Calf") ,EffectMgr::EBET_Leg_L);
			}
			else if (EEP_Wrist == pProto->eEquipPos)
			{
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 R Forearm") ,EffectMgr::EBET_Wrist_R);
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 L Forearm") ,EffectMgr::EBET_Wrist_L);
			}
			else if (EEP_Feet == pProto->eEquipPos)
			{
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 R Foot") ,EffectMgr::EBET_Foot_R);
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 L Foot") ,EffectMgr::EBET_Foot_L);
			}
			else if (EEP_Back == pProto->eEquipPos)
			{
				EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),strEffectName.c_str(), _T("Bip01 Spine2") ,EffectMgr::EBET_Mantle);
			}
		}
	}
	if(0 != pNetCmd->dwErrorCode)
	{
		ShowItemErrorMsg(pNetCmd->dwErrorCode);
	}

	return 0;
}

DWORD ItemMgr::NetUnEquipment( tagNS_Unequip* pNetCmd, DWORD )
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(plp))
		return 0;

	if (pNetCmd->dwErrorCode == E_Success)
	{
		Equipment* m_pEquip = NULL;
		if (EEP_Head == pNetCmd->eType)
		{
			m_pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Head);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Head);
			
		}
		else if (EEP_Body == pNetCmd->eType)
		{
			m_pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Body);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Chest);
		}
		else if (EEP_Back == pNetCmd->eType)
		{
			m_pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Back);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Mantle);
		}
		else if (EEP_Wrist == pNetCmd->eType)
		{
			m_pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Wrist);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Wrist_R);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Wrist_L);
		}
		else if (EEP_Legs == pNetCmd->eType)
		{
			m_pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Legs);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Leg_R);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Leg_L);
		}
		else if (EEP_Feet == pNetCmd->eType)
		{
			m_pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Feet);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Foot_R);
			EffectMgr::Inst()->StopRoleEquipEffect(plp->GetID(),EffectMgr::EBET_Foot_L);
		}
		else if(EEP_Holy == pNetCmd->eType)
		{
			tagGameEvent evt(_T("ShengLingUnEquipEvent"), NULL);
			m_pFrameMgr->SendEvent(&evt);
		}
	}
	if(0 != pNetCmd->dwErrorCode)
	{
		ShowItemErrorMsg(pNetCmd->dwErrorCode);
	}
	return 0;
}

DWORD ItemMgr::NetSwapWeapon( tagNS_SwapWeapon* pNetCmd, DWORD )
{
	if(0 != pNetCmd->dwErrorCode)
	{
		ShowItemErrorMsg(pNetCmd->dwErrorCode);
	}
	return 0;
}

BOOL ItemMgr::NetItemPosChange( tagNS_ItemPosChange* pNetCmd, DWORD )
{
	
	BOOL bRet = TRUE;

	//查找所在容器，没有则返回
	ItemContainer* pItemCon = GetContainer(pNetCmd->eConType);
	if(!P_VALID(pItemCon))
		return FALSE;
	
	//创建几个临时变量
	Item* pItem1 = NULL;
	Item* pItem2 = NULL;
	Item* pTempItem = NULL;
	DWORD dwTypeID1 = GT_INVALID;
	DWORD dwTypeID2 = GT_INVALID;
	//物品一没有找到
	pItem1 = pItemCon->GetItem(pNetCmd->n64Serial1);
	if(!P_VALID(pItem1))
		return FALSE;

	dwTypeID1 = pItem1->GetItemTypeID();
	
	if(GT_INVALID == pNetCmd->n64Serial2)
	{
		dwTypeID2 = dwTypeID1;

		pTempItem = pItemCon->Remove(pItem1->GetPos());
		if(pTempItem != pItem1)
			return FALSE;

		if(0 == pNetCmd->n16Num1)//如果数量为零直接删除
		{
			SAFE_DEL(pItem1);
		}
		else
		{
			pItem1->SetPos(pNetCmd->n16PosDst1);
			pItem1->SetItemQuantity(pNetCmd->n16Num1);
			bRet = bRet && pItemCon->Add(pItem1);
		}
	}
	else
	{
		if(pNetCmd->bCreateItem)
		{
			pTempItem = pItemCon->Remove(pItem1->GetPos());
			if(pTempItem != pItem1)
				return FALSE;
			
			if(0 == pNetCmd->n16Num1)//如果数量为零直接删除
			{
				SAFE_DEL(pItem1);
			}
			else
			{
				pItem1->SetPos(pNetCmd->n16PosDst1);
				bRet = bRet && pItemCon->Add(pItem1);
				pItem1->SetItemQuantity(pNetCmd->n16Num1);
			}
				//根据物品一创造物品二
			if(MIsEquipment(pItem1->GetItemTypeID()))
			{
				pItem2 = new Equipment(*((Equipment*)pItem1));
			}
			else
			{
				pItem2 = new Item(*pItem1);
			}

			dwTypeID2 = pItem2->GetItemTypeID();

			pItem2->SetItemId(pNetCmd->n64Serial2);
			pItem2->SetPos(pNetCmd->n16PosDst2);	
			bRet = bRet && pItemCon->Add(pItem2);
			pItem2->SetItemQuantity(pNetCmd->n16Num2);
		}
		else
		{
			pItem2 = pItemCon->GetItem(pNetCmd->n64Serial2);
			if(!P_VALID(pItem2))
				return FALSE;

			dwTypeID2 = pItem2->GetItemTypeID();
			
			Item* pTempItem = pItemCon->Remove(pItem1->GetPos());
			if(pTempItem != pItem1)
				return FALSE;
				pTempItem = pItemCon->Remove(pItem2->GetPos());
			if(pTempItem != pItem2)
				return FALSE;

			if(0 == pNetCmd->n16Num1)//如果数量为零直接删除
			{
				SAFE_DEL(pItem1);
			}
			else
			{
				pItem1->SetPos(pNetCmd->n16PosDst1);
				pItem1->SetItemQuantity(pNetCmd->n16Num1);
				bRet = bRet && pItemCon->Add(pItem1);
			}

			pItem2->SetPos(pNetCmd->n16PosDst2);
			pItem2->SetItemQuantity(pNetCmd->n16Num2);
			bRet = bRet && pItemCon->Add(pItem2);
		}
	}
	
	if(bRet)
	{
		tagUpdateLPItemEvent event(_T("ItemEvent_UpdateItem"),NULL);
		event.eType = pNetCmd->eConType;
		event.n16Index = pNetCmd->n16PosDst1;
		event.dwTypeID = dwTypeID1;
		m_pFrameMgr->SendEvent(&event);

		event.n16Index = pNetCmd->n16PosDst2;
		event.dwTypeID = dwTypeID2;
		m_pFrameMgr->SendEvent(&event);

		if( EICT_Equip == pNetCmd->eConType 
			&& (EEP_RightHand == pNetCmd->n16PosDst1 || EEP_LeftHand == pNetCmd->n16PosDst1
			|| EEP_RightHand == pNetCmd->n16PosDst2 || EEP_LeftHand == pNetCmd->n16PosDst2) )
		{
			tagGameEvent e(_T("Weapon_Swap"), NULL);
			m_pFrameMgr->SendEvent(&e);
		}
	}
	return bRet;
}

BOOL ItemMgr::NetItemPosChangeEx( tagNS_ItemPosChangeEx* pNetCmd, DWORD )
{
	
	BOOL bRet = TRUE;
	
	ItemContainer* pItemConSrc1 = GetContainer(pNetCmd->eConTypeSrc1);
	ItemContainer* pItemConSrc2 = GetContainer(pNetCmd->eConTypeSrc2);
	ItemContainer* pItemConDst1 = GetContainer(pNetCmd->eConTypeDst1);
	ItemContainer* pItemConDst2 = GetContainer(pNetCmd->eConTypeDst2);
	if( !P_VALID(pItemConSrc1) || !P_VALID(pItemConSrc2)
		|| !P_VALID(pItemConDst1) || !P_VALID(pItemConDst2) )
		return FALSE;
	
	Item* pItem1 = NULL;
	Item* pItem2 = NULL;
	Item* pTempItem = NULL;
	DWORD dwTypeID1 = GT_INVALID;
	DWORD dwTypeID2 = GT_INVALID;
	INT16 n16Index1 = 0;
	INT16 n16Index2 = 0;



	if(GT_INVALID != pNetCmd->n64Serial1)
	{
		pItem1 = pItemConSrc1->GetItem(pNetCmd->n64Serial1);
		if(!P_VALID(pItem1))
			return FALSE;

		pTempItem = pItemConSrc1->Remove(pItem1->GetPos());
		if(pTempItem != pItem1)
			return FALSE;

		dwTypeID1 = pItem1->GetItemTypeID();
		n16Index2 = pItem1->GetPos();
	}

	if(GT_INVALID != pNetCmd->n64Serial2)
	{
		pItem2 = pItemConSrc2->GetItem(pNetCmd->n64Serial2);
		if(!P_VALID(pItem2))
			return FALSE;

		pTempItem = pItemConSrc2->Remove(pItem2->GetPos());
		if(pTempItem != pItem2)
			return FALSE;

		dwTypeID2 = pItem2->GetItemTypeID();
		n16Index1 = pItem2->GetPos();
	}

	if(0 == pNetCmd->n16Num1)//如果数量为零直接删除
	{
		SAFE_DEL(pItem1);
	}
	
	if(P_VALID(pItem1))
	{
		pItem1->SetConType(pNetCmd->eConTypeDst1);
		pItem1->SetPos(pNetCmd->n16PosDst1);
		pItem1->SetItemQuantity(pNetCmd->n16Num1);
		bRet = bRet && pItemConDst1->Add(pItem1);
	}

	if(P_VALID(pItem2))
	{
		pItem2->SetConType(pNetCmd->eConTypeDst2);
		pItem2->SetPos(pNetCmd->n16PosDst2);
		pItem2->SetItemQuantity(pNetCmd->n16Num2);
		bRet = bRet && pItemConDst2->Add(pItem2);
	}

	if(bRet)
	{
		n16Index1 = pNetCmd->n16PosDst1 == GT_INVALID ? n16Index1 : pNetCmd->n16PosDst1;
		n16Index2 = pNetCmd->n16PosDst2 == GT_INVALID ? n16Index2 : pNetCmd->n16PosDst2;

		tagUpdateLPItemEvent event(_T("ItemEvent_UpdateItem"),NULL);
		event.eType = pNetCmd->eConTypeDst1;
		event.n16Index = n16Index1;
		event.dwTypeID = dwTypeID1;
		m_pFrameMgr->SendEvent(&event);

		event.eType	= pNetCmd->eConTypeDst2; 	
		event.n16Index = n16Index2;
		event.dwTypeID = dwTypeID2;
		m_pFrameMgr->SendEvent(&event);

		if(EICT_Bag == pNetCmd->eConTypeDst1)
		{
			event.eType	= EICT_Bag;
			event.n16Index = n16Index1;
			event.dwTypeID = dwTypeID2;
			m_pFrameMgr->SendEvent(&event);
		}
		else if(EICT_Bag == pNetCmd->eConTypeDst2)
		{
			event.eType = EICT_Bag;
			event.n16Index = n16Index2;
			event.dwTypeID = dwTypeID1;
			m_pFrameMgr->SendEvent(&event);
		}

		if(EICT_HolyEquip == pNetCmd->eConTypeDst1)
		{
			event.eType	= EICT_HolyEquip;
			event.n16Index = n16Index1;
			event.dwTypeID = dwTypeID2;
			m_pFrameMgr->SendEvent(&event);
		}
		else if(EICT_HolyEquip == pNetCmd->eConTypeDst2)
		{
			event.eType = EICT_HolyEquip;
			event.n16Index = n16Index2;
			event.dwTypeID = dwTypeID1;
			m_pFrameMgr->SendEvent(&event);
		}


		if( (EICT_Equip == pNetCmd->eConTypeSrc1 || EICT_Equip == pNetCmd->eConTypeSrc2) 
			&& (EEP_RightHand == n16Index1 || EEP_LeftHand == n16Index1
			|| EEP_RightHand == n16Index2 || EEP_LeftHand == n16Index2) )
		{
			tagGameEvent e(_T("Weapon_Swap"), NULL);
			m_pFrameMgr->SendEvent(&e);
		}

	}

	return bRet;
}

DWORD ItemMgr::NetNewItemAdd( tagNS_NewItemAdd* pNetCmd, DWORD )
{
	
	Item* pItem = new Item(pNetCmd->Item);
	if(P_VALID(pItem))
	{
		AddNewItem(pItem);
		
		//发送游戏事件
		ASSERT(P_VALID(pItem));
		tagCombatMsgForItemChange event(_T("CombatMsg_ItemAdd"), NULL);
		event.dwTypeID = pNetCmd->Item.dwTypeID;
		event.n16Num = pNetCmd->Item.n16Num;
		event.nQuality = pItem->GetItemQuality();
		m_pFrameMgr->SendEvent(&event);

	}
	return 0;
}

DWORD ItemMgr::NetNewEquipAdd( tagNS_NewEquipAdd* pNetCmd, DWORD )
{
	
	Equipment* pEquip = new Equipment(pNetCmd->Equip);
	if(P_VALID(pEquip))
	{
		AddNewItem(pEquip);

		//发送游戏事件
		ASSERT(P_VALID(pEquip));
		tagCombatMsgForItemChange event(_T("CombatMsg_ItemAdd"), NULL);
		event.dwTypeID = pEquip->GetItemTypeID();
		event.n16Num = pEquip->GetItemQuantity();
		event.nQuality = pEquip->GetItemQuality();
		m_pFrameMgr->SendEvent(&event);
	
	}
	return 0;
}

DWORD ItemMgr::NetNewTrumpAdd( tagNS_NewFabaoAdd* pNetCmd, DWORD )
{
	Equipment* pEquip = new Equipment(pNetCmd->fabao);
	if(P_VALID(pEquip))
	{
		AddNewItem(pEquip);

		if(pEquip->GetConType() != EICT_TempBag)	//临时背包就不发了~
		{
			//发送游戏事件
			ASSERT(P_VALID(pEquip));
			tagCombatMsgForItemChange event(_T("CombatMsg_ItemAdd"), NULL);
			event.dwTypeID = pEquip->GetItemTypeID();
			event.n16Num = pEquip->GetItemQuantity();
			event.nQuality = pEquip->GetItemQuality();
			m_pFrameMgr->SendEvent(&event);
		}


	}
	return 0;
}

DWORD ItemMgr::NetNewHolyAdd(tagNS_NewHolyAdd* pNetCmd, DWORD)
{
	Equipment* pEquip = new Equipment(pNetCmd->holy);
		if(P_VALID(pEquip))
		{
			AddNewItem(pEquip);
			tagCombatMsgForItemChange event(_T("CombatMsg_ItemAdd"), NULL);
			event.dwTypeID = pEquip->GetItemTypeID();
			event.n16Num = pEquip->GetItemQuantity();
			event.nQuality = pEquip->GetItemQuality();
			m_pFrameMgr->SendEvent(&event);
		}
		return 0;
}

DWORD ItemMgr::NetNewHolyEquipAdd(tagNS_NewHolyEquipAdd* pNetCmd, DWORD)
{
	Equipment* pEquip = new Equipment(pNetCmd->holyEquip);
	if(P_VALID(pEquip))
	{
		AddNewItem(pEquip);
		tagCombatMsgForItemChange event(_T("CombatMsg_ItemAdd"), NULL);
		event.dwTypeID = pEquip->GetItemTypeID();
		event.n16Num = pEquip->GetItemQuantity();
		event.nQuality = pEquip->GetItemQuality();
		m_pFrameMgr->SendEvent(&event);
	}
	return 0;
}

DWORD ItemMgr::NetItemAdd( tagNS_ItemAdd* pNetCmd, DWORD )
{
	ItemContainer* pItemCon = GetContainer(pNetCmd->eConType);
	if(P_VALID(pItemCon))
	{
		Item* pItem = pItemCon->GetValue(pNetCmd->n16Index);
		if(P_VALID(pItem))
		{
			DWORD dwTypeID = pItem->GetItemTypeID();
			INT nQuantity = pNetCmd->n16Num - pItem->GetItemQuantity();

			pItem->SetItemQuantity(pNetCmd->n16Num);

			//发送游戏事件
			tagCombatMsgForItemChange event(_T("CombatMsg_ItemAdd"), NULL);
			event.dwTypeID = dwTypeID;
			event.n16Num = nQuantity;
			event.nQuality = pItem->GetItemQuality();
			m_pFrameMgr->SendEvent(&event);

			tagUpdateLPItemEvent e(_T("ItemEvent_UpdateItem"),NULL);
			e.eType = pNetCmd->eConType;
			e.n16Index = pNetCmd->n16Index;
			e.dwTypeID = dwTypeID;
			m_pFrameMgr->SendEvent(&e);
		}
		
	}

	return 0;
}

DWORD ItemMgr::NetItemRemove( tagNS_ItemRemove* pNetCmd, DWORD )
{
	ItemContainer* pItemCon = GetContainer(pNetCmd->eConType);
	if(P_VALID(pItemCon))
	{
		Item* pItem = pItemCon->GetValue(pNetCmd->n16Index);
		if(P_VALID(pItem) && pNetCmd->n64Serial == pItem->GetItemId())
		{
			DWORD dwTypeID = pItem->GetItemTypeID();
			INT nQuantity = pItem->GetItemQuantity() - pNetCmd->n16Num;
			
			if(pNetCmd->n16Num > 0)
			{
				//如果是可多次使用的物品，n16Num是可使用的次数
				if(pItem->GetProto()->nMaxUseTimes > 1)
				{
					pItem->SetItemUseTimes(pNetCmd->n16Num);
					nQuantity = 0;
				}
				else
				{
					pItem->SetItemQuantity(pNetCmd->n16Num);
				}
			}
			else
			{
				Item* pItem1 = pItemCon->Remove(pItem->GetPos());
				if(pItem == pItem1)
				{
					SAFE_DEL(pItem);
				}
			}

			//发送游戏事件
			if(nQuantity > 0)
			{
				tagCombatMsgForItemChange event(_T("CombatMsg_ItemRemove"), NULL);
				event.dwTypeID = dwTypeID;
				event.n16Num = nQuantity;
				m_pFrameMgr->SendEvent(&event);
			}

			tagUpdateLPItemEvent e(_T("ItemEvent_UpdateItem"),NULL);
			e.eType = pNetCmd->eConType;
			e.n16Index = pNetCmd->n16Index;
			e.dwTypeID = dwTypeID;
			m_pFrameMgr->SendEvent(&e);

			if (EICT_Equip == pNetCmd->eConType)
			{
				m_pEquipment->Remove(pNetCmd->n16Index);
			}

			HolyManMgr::Inst()->UpdateSoulAttValue();
			
		}
	}

	return 0;
}

DWORD ItemMgr::NetItemBind( tagNS_ItemBind* pNetCmd, DWORD )
{
	ItemContainer* pItemCon = GetContainer(pNetCmd->eConType);
	if(P_VALID(pItemCon))
	{
		Item* pItem = pItemCon->GetItem(pNetCmd->n64Serial);
		if (P_VALID(pItem))
		{
			pItem->SetItemBind();
			pItem->SetTradeable(FALSE);
		}
	}

	return 0;
}

// 设置物品的绑定状态
DWORD ItemMgr::NetItemBindStatus(tagNS_ItemBindStatus* pNetCmd, DWORD)
{
	ItemContainer* pItemCon = GetContainer(pNetCmd->eConType);
	if(P_VALID(pItemCon))
	{
		Item* pItem = pItemCon->GetItem(pNetCmd->n64Serial);
		if (P_VALID(pItem))
		{
			pItem->SetItemBindStatus(pNetCmd->byStatus);
			if ( pNetCmd->byStatus == EBS_Unbind )
			{
				pItem->SetTradeable(TRUE);				
			}
			else
			{
				pItem->SetTradeable(FALSE);					
			}			
		}
	}

	return 0;
}

DWORD ItemMgr::NetEquipChange( tagNS_EquipChange* pNetCmd, DWORD )
{
	//--如果物品位置有效且为装备
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(pNetCmd->n64Serial);
    if (!P_VALID(pEquip))
    {
        pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_Equip, pNetCmd->n64Serial);
    }

	//--如果有该装备且装备ID与服务器传过来的装备ID相符
	if(P_VALID(pEquip) && MIsEquipment(pEquip->GetItemTypeID()))
	{
		//--更新装备数据
		pEquip->UpdateEquipment(pNetCmd->equipSpec);
		tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
		eve.eType = pEquip->GetConType();
		eve.n16Index = pEquip->GetPos();
		m_pFrameMgr->SendEvent(&eve);
	}
	return 0;
}

DWORD ItemMgr::NetEquipSingleChange(tagNS_EquipSingleChange* pMsg, DWORD pPrama)
{
	//--如果物品位置有效且为装备
	if ( !P_VALID(pMsg))
		return 0;

	tagEquipSingleSpec		equipSingleSpec	= pMsg->equipSingleSpec;
	INT64							n64EquipId			= equipSingleSpec.n64Serial;
	EEquipSignleSpecType  eSingleType			= equipSingleSpec.eEquipSingleSpecType;
	DWORD						dwValue				= equipSingleSpec.dwVal;
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID( n64EquipId );
	if (!P_VALID(pEquip))
	{
		pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_Equip, n64EquipId );
	}

	//--如果有该装备且装备ID与服务器传过来的装备ID相符
	if(P_VALID(pEquip) && MIsEquipment(pEquip->GetItemTypeID()))
	{
		//--更新装备数据
		pEquip->UpdateSingleEquipment(eSingleType, dwValue);

		tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
		eve.eType = pEquip->GetConType();
		eve.n16Index = pEquip->GetPos();
		m_pFrameMgr->SendEvent(&eve);
	}

	return 0;
}

DWORD ItemMgr::NetTrumpChange( tagNS_FabaoChange* pNetCmd, DWORD )
{
	//--如果物品位置有效且为装备
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(pNetCmd->n64Serial);
	if (!P_VALID(pEquip))
	{
		pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_Equip, pNetCmd->n64Serial);
	}

	//--如果有该装备且装备ID与服务器传过来的装备ID相符
	if(P_VALID(pEquip) && MIsEquipment(pEquip->GetItemTypeID()))
	{
		//--更新装备数据
		pEquip->UpdateEquipment(pNetCmd->fabaoSpec);
		tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
		eve.eType = pEquip->GetConType();
		eve.n16Index = pEquip->GetPos();
		m_pFrameMgr->SendEvent(&eve);
	}
	return 0;
}

DWORD ItemMgr::NetHunJingChange(tagNS_SoulCrystalChg* pNetCmd, DWORD)
{
	//--如果物品位置有效且为装备
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(pNetCmd->n64Serial);
	if (!P_VALID(pEquip))
	{
		pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_Equip, pNetCmd->n64Serial);
	}

	//--如果有该装备且装备ID与服务器传过来的装备ID相符
	if(P_VALID(pEquip) && MIsEquipment(pEquip->GetItemTypeID()))
	{
		//--更新装备数据
		pEquip->UpdateEquipment(pNetCmd->info);
		tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
		eve.eType = pEquip->GetConType();
		eve.n16Index = pEquip->GetPos();
		m_pFrameMgr->SendEvent(&eve);
	}
	return 0;
}

DWORD ItemMgr::NetItemCDUpdate( tagNS_ItemCDUpdate* pNetCmd, DWORD )
{
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pNetCmd->dwTypeID);
	if(P_VALID(pProto))
	{
		// 计算当前冷却时间和该物品冷却时间的差值，计算物品的开始计时间
		INT nIncMilliSecond = (INT)(pNetCmd->dwCDTime - pProto->dwCooldownTime);
		DWORD dwStartTime = Kernel::Inst()->GetAccumTimeDW() + nIncMilliSecond;
		if(m_mapCurStartTime.IsExist(pNetCmd->dwTypeID))
			m_mapCurStartTime.ChangeValue(pNetCmd->dwTypeID, dwStartTime);
		else 
			m_mapCurStartTime.Add(pNetCmd->dwTypeID, dwStartTime);
		
		tagGameEvent eve(_T("tagUpdateItemCDEvent"), NULL);
		m_pFrameMgr->SendEvent(&eve);
	}
	return 0;
}

DWORD ItemMgr::NetAbraseWeapon( tagNS_Abrase* pNetCmd, DWORD )
{
	if(E_Success == pNetCmd->dwErrorCode)
	{
		Item* pItem = m_pPocket->GetItem(pNetCmd->n64WeaponSerial);
		if (P_VALID(pItem))
		{
			pItem->SetItemUseTimes(pNetCmd->nAttackTimes);
		}
        else
        {
            pItem = m_pEquipment->GetItem(pNetCmd->n64WeaponSerial);
            pItem->SetItemUseTimes(pNetCmd->nAttackTimes);
        }

		//发送游戏事件
        if (P_VALID(pItem))
        {
            tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
            eve.eType = pItem->GetConType();
            eve.n16Index = pItem->GetPos();
            m_pFrameMgr->SendEvent(&eve);
        }
		
	}
	else
	{

	}
	return 0;
}


DWORD ItemMgr::NetWareOpen( tagNS_WareOpen* pNetCmd, DWORD )
{
	if( E_Success == pNetCmd->dwErrorCode )
	{
        // 打开仓库界面
        /**ItemFrame* pItemFrame = (ItemFrame*)m_pFrameMgr->GetFrame(_T("Item"));
        if (!P_VALID(pItemFrame))
            return 0;

        StorageFrame* pFrame = (StorageFrame*)m_pFrameMgr->GetFrame(_T("Storage"));
        if (!P_VALID(pFrame))
        {
            pFrame = (StorageFrame*)m_pFrameMgr->CreateFrame(_T("Item"), _T("Storage"), _T("StorageFrame"), 0);
        }

        if(P_VALID(pFrame))
        {
            pFrame->SetWalkWareEnable(true);
            pItemFrame->SetCurStrategy(EIST_ItemStorage);
			//MallMgr::Inst()->OpenEasyBuyFrame( _T("World"), _T("StorageEasyBuyFrame"), "\\desktop\\wh_win", EBUT_storageUI, "OuterRightTop", "storageui" );
        }*/
		
		QuestMgr::Inst()->OpenWalkWare();
	}
	return 0;
}


ItemContainer* ItemMgr::GetContainer( EItemConType eType )
{
	switch(eType)
	{
	case EICT_Bag://行囊
		return m_pPocket;
	case EICT_Baibao://百宝袋
		return m_pThrone;
	case EICT_Equip://装备
		return m_pEquipment;
	case EICT_Quest://任务
		return m_pQuest;
	case EICT_RoleWare://仓库
		return m_pStorge;
    case EICT_GuildWare:// 帮派仓库
        return m_pGuildStore;
	case EICT_Wardrobe:// 衣橱
		return m_pClothespress;
	case EICT_TempBag:
		return m_pTempContainer;
	case EICT_HolyManEquip:
		return m_pShengLingEquip;
	case EICT_WarBag:
		return m_pWarBag;
	default://如果不是以上容器则直接删除
		return NULL;
	}
	return NULL;
}

Item* ItemMgr::GetPocketItem( INT16 n16Pos )
{
	Item* pItem = m_pPocket->GetValue(n16Pos);
	if(!P_VALID(pItem))
		pItem = m_pWarBag->GetValue(n16Pos);
	return pItem;
}

Item* ItemMgr::GetPocketItemByID( INT64 n64ID )
{
	Item* pItem = m_pPocket->GetItem(n64ID);
	if(!P_VALID(pItem))
		pItem = m_pWarBag->GetItem(n64ID);

	return pItem;
}

Equipment* ItemMgr::GetCurEquip( EEquipPos ePos )
{
	return (Equipment*)m_pEquipment->GetValue((INT16)ePos);
}

Equipment* ItemMgr::GetCurEquipByID( INT64 n64ID )
{
	return (Equipment*)m_pEquipment->GetItem(n64ID);
}


BOOL ItemMgr::IsNotEquip( EEquipPos ePos )
{
	Item* pItem = m_pEquipment->GetValue((INT16)ePos);
	return !P_VALID(pItem);
}

Item* ItemMgr::GetConItem( EItemConType ePos, INT16 n16Pos )
{
	ItemContainer* pCon = GetContainer(ePos);
	if(!P_VALID(pCon))
		return NULL;

	return pCon->GetValue(n16Pos);
}

Item* ItemMgr::GetConItemByID( EItemConType ePos, INT64 n64ID )
{
	ItemContainer* pCon = GetContainer(ePos);
	if(!P_VALID(pCon))
		return NULL;

	return pCon->GetItem(n64ID);
}

INT64 ItemMgr::GetSpecfuncItemID( EItemSpecFunc eType )
{
	INT64 n64Ret = GT_INVALID;
	if(P_VALID(m_pPocket))
	{
		n64Ret = m_pPocket->GetSpecFuncItemID(eType);
	}
	return n64Ret;
}

EItemTypeEx ItemMgr::GetHandWeaponType(EEquipPos ePos/*=EEP_RightHan*/)
{
	if( !P_VALID(m_pEquipment) )
		return EITE_Null;
	Item* pItem = m_pEquipment->GetValue((INT16)ePos);
	if(P_VALID(pItem) && MIsEquipment(pItem->GetItemTypeID()))
	{
		Equipment* pEquip = (Equipment*)pItem;
		return pEquip->GetItemTypeEx();
	}
	return EITE_Null;
}

VOID ItemMgr::GetItemCurCDTime( DWORD dwTypeID, FLOAT& fCDCount, FLOAT& fRestoreRatio )
{
	DWORD dwStartTime = m_mapCurStartTime.Peek(dwTypeID);
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
	if(GT_VALID(dwStartTime) && P_VALID(pProto))
	{
		FLOAT fCDTime = (FLOAT)pProto->dwCooldownTime;
		//离上一次该类物品使用的时间
		INT nPassMilliSecord = Kernel::Inst()->GetAccumTimeDW() - dwStartTime;
		//倒计时时间
		fCDCount = fCDTime - nPassMilliSecord;	
		//倒计时旋转律
		fRestoreRatio = (fCDTime - fCDCount) / fCDTime + 0.000001f;
	}
}

INT16 ItemMgr::GetConBlank( EItemConType ePos, INT nIndex /*= 0*/ )
{
	ItemContainer* pCon = GetContainer(ePos);
	if(!P_VALID(pCon))
		return GT_INVALID;

	return pCon->GetBlank(nIndex);
}

DWORD ItemMgr::OnEventBagExtend( tagItemContainerExtend* pGameEvent )
{
	ASSERT(P_VALID(m_pPocket));
	INT nMaxSize = m_pPocket->MaxSize();
	if (nMaxSize >= SPACE_ALL_BAG)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CantExtend")]);
		return 0;
	}

	// 先重置状态位
	m_bNotEnoughYuanbao = false;
	m_bNotEnoughSilver	= false;
	
	// 计算需要消耗金钱还是元宝
	TCHAR szTemp[X_LONG_NAME] = {0};
	INT64 nExtendSilver = 0;
	INT nExtendYuanbao = 0;
	INT nExtendTimes = MCalBagExTimes(nMaxSize);
	if (pGameEvent->bUseSilver)
	{
		nExtendSilver = MCalBagExSilver(nExtendTimes);
		// 先判断金钱是否足够，等确认以后再提示
		if(nExtendSilver > CurrencyMgr::Inst()->GetBagSilver())
			m_bNotEnoughSilver = true;

		_sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("BagExtend_Check")], 
			CurrencyMgr::Inst()->GetSilverDisplay(nExtendSilver).c_str());
	}
	else
	{
		nExtendYuanbao = MCalBagExYuanBao(nExtendTimes);
		
		// 先判断元宝是否足够，等确认以后再提示
		if(nExtendYuanbao > CurrencyMgr::Inst()->GetBagYuanbao())
			m_bNotEnoughYuanbao = true;

		_sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("BagExtend_Check")], 
			CurrencyMgr::Inst()->GetYuanbaoDisplay(nExtendYuanbao).c_str());
	}
	// 记录消耗的是什么
	m_bUseSilverExtend = pGameEvent->bUseSilver;
	if (GetObj("BagExtendBox"))
		KillObj("BagExtendBox");

	CreateObj("BagExtendBox", "MsgBox");
	
	if( pGameEvent->bWalkWare )
		TObjRef<MsgBox>("BagExtendBox")->SetDwordUserData( 1 );
	else
		TObjRef<MsgBox>("BagExtendBox")->SetDwordUserData( 0 );

	TObjRef<MsgBox>("BagExtendBox")->Init(_T(""), szTemp, _T("MsgBox_BagExtend"));

	return 0;
}

DWORD ItemMgr::OnEventWareExtend( tagItemContainerExtend* pGameEvent )
{
	ASSERT(P_VALID(m_pStorge));
	INT nMaxSize = m_pStorge->MaxSize();
	if (nMaxSize >= SPACE_ALL_WARE)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CantExtend")]);
		return 0;
	}
	
	// 先重置状态位
	m_bNotEnoughYuanbao = false;
	m_bNotEnoughSilver	= false;

	// 计算需要消耗金钱还是元宝
	TCHAR szTemp[X_LONG_NAME] = {0};
	INT64 nExtendSilver = 0;
	INT nExtendYuanbao = 0;
	INT nExtendTimes = MCalWareExTimes(nMaxSize);
	if (pGameEvent->bUseSilver)
	{
		nExtendSilver = MCalWareExSilver(nExtendTimes);

		// 先判断金钱是否足够，等确认以后再提示
		if(nExtendSilver > CurrencyMgr::Inst()->GetBagSilver())
			m_bNotEnoughSilver = true;

		_sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("WareExtend_Check")], 
			CurrencyMgr::Inst()->GetSilverDisplay(nExtendSilver).c_str());
	}
	else
	{
		nExtendYuanbao = MCalWareExYuanBao(nExtendTimes);

		// 先判断元宝是否足够，等确认以后再提示
		if(nExtendYuanbao > CurrencyMgr::Inst()->GetBagYuanbao())
			m_bNotEnoughYuanbao = true;

		_sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("WareExtend_Check")], 
			CurrencyMgr::Inst()->GetYuanbaoDisplay(nExtendYuanbao).c_str());
	}
	// 记录消耗的是什么
	m_bUseSilverExtend = pGameEvent->bUseSilver;
	if (GetObj("WareExtendBox"))
		KillObj("WareExtendBox");

	CreateObj("WareExtendBox", "MsgBox");
	
	if( pGameEvent->bWalkWare )
		TObjRef<MsgBox>("WareExtendBox")->SetDwordUserData( 1 );
	else
		TObjRef<MsgBox>("WareExtendBox")->SetDwordUserData( 0 );

	TObjRef<MsgBox>("WareExtendBox")->Init(_T(""), szTemp, _T("MsgBox_WareExtend"));

	return 0;
}

DWORD ItemMgr::OnEventBagExtendCheck( tagMsgBoxEvent* pGameEvent )
{
	if (MBF_OK == pGameEvent->eResult)
	{
		//if(m_bNotEnoughSilver)
		//{
		//	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CurrencyError_NotEnoughSilver")]);
		//	return 0;
		//}
		//else if(m_bNotEnoughYuanbao)
		//{
		//	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CurrencyError_NotEnoughYuanbao")]);
		//	return 0;
		//}

        Item* pItem = GetFirstSpecFuncItemFromPocket(EISF_BagExtend);
        if (!P_VALID(pItem))
        {
            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BagExtend_Error")]);
            return 0;
        }

		tagNC_BagExtend m;
		m.n64Serial = pItem->GetItemId();
		DWORD dwWalkWare = TObjRef<MsgBox>("BagExtendBox")->GetDwordUserData();
		if( dwWalkWare == 1 )
			m.dwNPCID = GT_INVALID;
		else
			m.dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
		m_pSession->Send( &m );
	}
	return 0;
}

DWORD ItemMgr::OnEventWareExtendCheck( tagMsgBoxEvent* pGameEvent )
{
	if (MBF_OK == pGameEvent->eResult)
	{
		//if(m_bNotEnoughSilver)
		//{
		//	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CurrencyError_NotEnoughSilver")]);
		//	return 0;
		//}
		//else if(m_bNotEnoughYuanbao)
		//{
		//	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CurrencyError_NotEnoughYuanbao")]);
		//	return 0;
		//}

        Item* pItem = GetFirstSpecFuncItemFromPocket(EISF_RoleWareExtend);
        if (!P_VALID(pItem))
        {
            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("WareExtend_Error")]);
            return 0;
        }

		tagNC_WareExtend m;
		m.n64Serial = pItem->GetItemId();
		DWORD dwWalkWare = TObjRef<MsgBox>("WareExtendBox")->GetDwordUserData();
		if( dwWalkWare == 1 )
			m.dwNPCID = GT_INVALID;
		else
			m.dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
		m_pSession->Send( &m );
	}
	return 0;
}

DWORD ItemMgr::NetBagExtend( tagNS_BagExtend* pNetCmd, DWORD )
{
	if (E_Success == pNetCmd->dwErrorCode)
	{
		ASSERT(P_VALID(m_pPocket));
		m_pPocket->SetMaxSize(pNetCmd->n16BagNum);

		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Success_ExternedBag")]);

		tagGameEvent event(_T("ItemEvent_BagExtend"),NULL);
		m_pFrameMgr->SendEvent(&event);
	}
	else if (E_Con_Cannot_Extend == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Enough_ExternedBag")]);
	}
	return 0;
}

DWORD ItemMgr::NetWareExtend( tagNS_WareExtend* pNetCmd, DWORD )
{
	if (E_Success == pNetCmd->dwErrorCode)
	{
		ASSERT(P_VALID(m_pStorge));
		m_pStorge->SetMaxSize(pNetCmd->n16WareNum);

		tagGameEvent event(_T("ItemEvent_WareExtend"),NULL);
		m_pFrameMgr->SendEvent(&event);
	}
	else if (E_Con_Cannot_Extend == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Enough_ExternedWareHouse")]);
	}
	return 0;
}

DWORD ItemMgr::NetSuitNumChange( tagNS_SuitNum* pNetCmd, DWORD )
{
	if (m_mapSuitNum.IsExist(pNetCmd->dwSuitID))
	{
		m_mapSuitNum.ChangeValue(pNetCmd->dwSuitID, pNetCmd->n8Num);
	}
	else
	{
		m_mapSuitNum.Add(pNetCmd->dwSuitID, pNetCmd->n8Num);
	}
	return 0;
}

VOID ItemMgr::ShowItemErrorMsg( DWORD dwErrorCode )
{
	if( (dwErrorCode >= E_Item_LevelLimit && dwErrorCode <= E_Item_AttA_Limit)
		|| (dwErrorCode >= E_BagPsd_Exist && dwErrorCode <= E_BagPsd_Error)
		|| (dwErrorCode >= E_Equip_OnFailed && dwErrorCode <= E_Equip_CondenseLimit) )
	{
		TCHAR szTmp[X_SHORT_NAME] = {0};
		_sntprintf(szTmp,sizeof(szTmp)/sizeof(TCHAR), _T("ItemMsgError%d"), dwErrorCode);
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);
	}
}

VOID ItemMgr::ShowSpecItemErrorMsg( DWORD dwErrorCode )
{
	if( (dwErrorCode >= E_ScriptItem_ItemInvalid && dwErrorCode <= E_ScriptItem_DesRoleError) )
	{
		TCHAR szTmp[X_SHORT_NAME] = {0};
		_sntprintf(szTmp,sizeof(szTmp)/sizeof(TCHAR), _T("SpecItemMsgError%d"), dwErrorCode);
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);
	}
}
DWORD ItemMgr::NetBagPassword( tagNS_BagPsd* pNetCmd, DWORD )
{
	if (E_Success == pNetCmd->dwErrorCode)
	{
		// 设置当前密码状态
		RoleMgr::Inst()->GetLocalPlayer()->SetRoleStateEx(pNetCmd->dwRoleStateEx);
		
		switch (m_eEncryptState)
		{
		case EPES_EnterPassword:
			{
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("Open_Package"), NULL));
			}
			break;
		case EPES_SetupPassword:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("PocketEncrypt_PromptSetup")]);
			}
			break;
		case EPES_ModifyPassword:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("PocketEncrypt_PromptModify")]);
			}
			break;
		case EPES_CancelPassword:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("PocketEncrypt_PromptCancel")]);
			}
			break;
		}
		// 处理完了就置为默认值
		m_eEncryptState = EPES_Null;
	}
	else if (E_BagPsd_OK == pNetCmd->dwErrorCode)
	{
		if(P_VALID(GetObj("ModPsdSecond")))
			KillObj("ModPsdSecond");

		CreateObj("ModPsdSecond", "MsgInputBox");
		TObjRef<MsgInputBox>("ModPsdSecond")->Init(
			g_StrTable[_T("PocketEncrypt_CaptionModify")],g_StrTable[_T("PocketEncrypt_TextNewPassword")],
			_T("MsgBox_ModPsdSecond"),GT_INVALID, MBIT_Password);
	}
	else
	{
		ShowItemErrorMsg(pNetCmd->dwErrorCode);
		if(!RoleMgr::Inst()->GetLocalPlayer()->GetRoleStateEx(ERSE_BagPsdPass))
		{
			m_pFrameMgr->SendEvent(&tagGameEvent(_T("Close_Package_UI"), NULL));
		}
	}
	
	return 0;
}

DWORD ItemMgr::NetWeaponNewnessChange( tagNS_RWNewessChange* pNetCmd, DWORD )
{
	Item* pItem = m_pEquipment->GetValue(EEP_RightHand);
	if(P_VALID(pItem))
	{
		pItem->SetItemUseTimes(pNetCmd->nAttackTimes);
	}

	//发送游戏事件
	tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
	eve.eType = pItem->GetConType();
	eve.n16Index = pItem->GetPos();
	m_pFrameMgr->SendEvent(&eve);
	return 0;
}

DWORD ItemMgr::NetContainerReorder( tagNS_ItemReorder* pNetCmd, DWORD )
{
	ItemContainer* pItemCon = GetContainer(pNetCmd->eConType);
	if(P_VALID(pItemCon))
	{
		if(E_Success == pNetCmd->dwErrorCode)
		{
			TMap<INT16, Item*> TempMap;
			INT pos = 0;
			for(INT i=0; i<pNetCmd->n16ItemNum; ++i)
			{
				INT16 n16OldPos;
				memcpy(&n16OldPos, pNetCmd->n16OldIndex+pos, sizeof(INT16));
				Item* pItem = pItemCon->Remove(n16OldPos);
				if(P_VALID(pItem))
				{
					pItem->SetPos(pos);
					TempMap.Add(n16OldPos, pItem);				
				}
				pos++;
			}

			Item* pTemp = NULL;
			INT16 n16Pos = 0;
			TempMap.ResetIterator();
			while(TempMap.PeekNext(n16Pos,pTemp))
			{
				pItemCon->Add(pTemp);
				tagUpdateLPItemEvent event(_T("ItemEvent_UpdateItem"), NULL);
				event.eType = pNetCmd->eConType;
				event.n16Index = n16Pos;
				event.dwTypeID = pTemp->GetItemTypeID();
				m_pFrameMgr->SendEvent(&event);

				event.n16Index = pTemp->GetPos();
				m_pFrameMgr->SendEvent(&event);
			}
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("PocketSettle_SettleFinish")]);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PocketSettle_SettleFailed")]);
		}
		pItemCon->Unlock(true);
	}
	return 0;
}

DWORD ItemMgr::NetContainerReorderEx( tagNS_ItemReorderEx* pNetCmd, DWORD )
{
	ItemContainer* pItemCon = GetContainer(pNetCmd->eConType);
	if(P_VALID(pItemCon))
	{
		if(E_Success == pNetCmd->dwErrorCode)
		{
			TMap<INT16, Item*> TempMap;
			INT pos = 0;
			for(INT i=0; i<pNetCmd->n16ItemNum; ++i)
			{
				tagItemOrder m;
				memcpy(&m, pNetCmd->byData+pos, sizeof(tagItemOrder));
				Item* pItem = pItemCon->Remove(m.n16OldIndex);
				if(P_VALID(pItem))
				{
					pItem->SetPos(m.n16NewIndex);
					TempMap.Add(m.n16OldIndex, pItem);
				}
				
				pos += sizeof(tagItemOrder);
			}
			
			Item* pTemp = NULL;
			INT16 n16Pos = 0;
			TempMap.ResetIterator();
			while(TempMap.PeekNext(n16Pos,pTemp))
			{
				pItemCon->Add(pTemp);
				tagUpdateLPItemEvent event(_T("ItemEvent_UpdateItem"), NULL);
				event.eType = pNetCmd->eConType;
				event.n16Index = n16Pos;
				event.dwTypeID = pTemp->GetItemTypeID();
				m_pFrameMgr->SendEvent(&event);

				event.n16Index = pTemp->GetPos();
				m_pFrameMgr->SendEvent(&event);
			}
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("PocketSettle_SettleFinish")]);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PocketSettle_SettleFailed")]);
		}
		pItemCon->Unlock(true);
	}
	return 0;
}

DWORD ItemMgr::NetIdentifyEquip( tagNS_IdentifyEquip* pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode != E_Success)
	{
		if(pNetCmd->dwErrorCode == E_Item_LevelLimit)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_IdentifyLevelLimit")]);
		}
		else
		{
			ShowItemErrorMsg(pNetCmd->dwErrorCode);
		}
	}
	return 0;
}

DWORD ItemMgr::NetInitLocalPlayerSuit( tagNS_GetRoleInitState_Suit* pNetCmd, DWORD )
{
	m_mapSuitNum.Clear();

	for(INT i=0,pos=0; i<pNetCmd->nSuitNum; ++i)
	{
		tagSuitInit item;
		memcpy(&item, pNetCmd->byData+pos, sizeof(tagSuitInit));
		m_mapSuitNum.Add(item.dwSuitID, (INT8)item.nEquipNum);
		
		pos += sizeof(tagSuitInit);
	}
	return 0;
}

DWORD ItemMgr::NetInitLocalPlayerLonghun( tagNS_GetRoleInitState_Longhun* pNetCmd, DWORD )
{
	m_mapLonghunOn.Clear();

	for(INT i=0; i<pNetCmd->n8Num; ++i)
	{
		m_mapLonghunOn.Add(pNetCmd->n16EquipPos[i], true);
	}
	return 0;
}

DWORD ItemMgr::NetEquipLonghunOn( tagNS_LongHunOn* pNetCmd, DWORD )
{
	INT16 n16Pos = m_mapLonghunOn.Peek(pNetCmd->n16EquipPos);
	if(GT_VALID(n16Pos))
	{
		m_mapLonghunOn.ChangeValue(pNetCmd->n16EquipPos, true);
	}
	else
	{
		m_mapLonghunOn.Add(pNetCmd->n16EquipPos, true);
	}
	return 0;
}

DWORD ItemMgr::NetEquipLonghunOff( tagNS_LongHunOff* pNetCmd, DWORD )
{
	m_mapLonghunOn.Erase(pNetCmd->n16EquipPos);

	return 0;
}

bool ItemMgr::GetLonghun( EEquipPos ePos, INT64 id )
{
	Item* pItem = m_pEquipment->GetValue((INT16)ePos);
	
	if(!P_VALID(pItem) || pItem->GetItemId() != id)
		return false;

	return m_mapLonghunOn.Peek((INT16)ePos) == GT_INVALID ? false : true;
}

DWORD ItemMgr::NetDyeFashion( tagNS_DyeFashion* pNetCmd, DWORD )
{

    switch (pNetCmd->dwErrCode)
    {
    case E_Success:
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DyeFashionErr5")]);
    	break;
    case E_Dye_OtherItem:
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DyeFashionErr1")]);
        break;
    case E_Dye_Exist:
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DyeFashionErr2")]);
        break;
    case E_Dye_LvLimit:
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("DyeFashionErr3")]);
        break;
    default:
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DyeFashionErr4")]);
    }

    return 0;
}

DWORD ItemMgr::NetInitBaiBaoRecord( tagNS_InitBaiBaoRecord* pNetCmd, DWORD )
{
	// 清空列表
	m_listBaiBaoRecord.Clear();

	for(INT i=0, pos=0; i<pNetCmd->n16Num; ++i)
	{
		tagBaiBaoRecord* pRecord = (tagBaiBaoRecord*)(pNetCmd->byData+pos);
		AddNewBaiBaoRecord(pRecord);

		pos += pRecord->n16Size;
	}

	// 发送游戏事件
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("ItemEvent_BaiBaoRecord"), NULL));
	
	return 0;
}

DWORD ItemMgr::NetSingleBaiBaoRecord( tagNS_SingleBaiBaoRecord* pNetCmd, DWORD )
{
	AddNewBaiBaoRecord(&pNetCmd->sRecord);

	// 发送游戏事件
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("ItemEvent_BaiBaoRecord"), NULL));

	return 0;
}

VOID ItemMgr::AddNewBaiBaoRecord( tagBaiBaoRecord* pRecord )
{
	if(!P_VALID(pRecord))
		return;
	
	// 赋值
	tagBaiBaoRecordC sRecord;
	sRecord.dwRoleID	= pRecord->dwRoleID;
	sRecord.eType		= (EBaiBaoRecordType)pRecord->n16Type;
	sRecord.dwTypeID	= pRecord->dwTypeID;
	sRecord.sTime		= (tagDWORDTime)pRecord->dwTime;
	sRecord.strWords	= pRecord->szWords;
	// 存入列表
	m_listBaiBaoRecord.PushBack(sRecord);
}

VOID ItemMgr::GetBaiBaoList( TList<tagBaiBaoRecordC> &listRecord )
{
	// 如果没有初始化，则发送请求
	if(m_bNotInitBaiBaoRecord)
	{
		tagNC_InitBaiBaoRecord cmd;
		m_pSession->Send(&cmd);
		
		// 不再请求第二次
		m_bNotInitBaiBaoRecord = false;
	}

	listRecord = m_listBaiBaoRecord;
}

void ItemMgr::ClearGuildWare()
{
    ASSERT(P_VALID(m_pGuildStore));
    TMap<INT64, Item*> mapContainer;
    m_pGuildStore->Clear(mapContainer);

    INT64 n64ItemId = 0;
    Item* pTemp = NULL;

    mapContainer.ResetIterator();
    while (mapContainer.PeekNext(n64ItemId, pTemp))
    {
        if (P_VALID(pTemp))
        {
            pTemp->SetOperable(true);
            SAFE_DEL(pTemp);
        }
    }
}

void ItemMgr::OnGetGuildWare( tagNS_GetGuildWare* pMsg )
{
    ASSERT(P_VALID(m_pGuildStore));

    m_pGuildStore->SetMaxSize(pMsg->nSzGuildWare);
    
    size_t offset = 0;
    for (int i = 0; i < pMsg->nItemNum; ++i)
    {
        tagGuildWareUpdate* pData = (tagGuildWareUpdate*)(pMsg->byData + offset);
        
        switch (pData->eType)
        {
        case EGWU_Change:
            {
                tagItem* pItemData = (tagItem*)(pData->byData+1);
                
                // 先移除原位置
                 Item* pItemRemove = m_pGuildStore->Remove(pItemData->n16Index);
                 if (P_VALID(pItemRemove))
                    SAFE_DEL(pItemRemove);
                 Item* pItemRepetition = m_pGuildStore->GetItem(pItemData->n64Serial);
                 if (P_VALID(pItemRepetition))
                 {
                    pItemRemove = m_pGuildStore->Remove(pItemRepetition->GetPos());
                    if (P_VALID(pItemRemove))
                        SAFE_DEL(pItemRemove);
                 }

                 // 插入新的
                if (MIsEquipment(pItemData->dwTypeID))
                {
					if(*pData->byData == EIT_FaBao)
					{
						tagFabao* pFabao = (tagFabao*)pItemData;
						Equipment* pEquip = new Equipment(*pFabao);
						m_pGuildStore->Add(pEquip);
					}
					else if (*pData->byData == EIT_SoulCrystal)
					{
						tagSoulCrystal* pFabao = (tagSoulCrystal*)pItemData;
						Equipment* pEquip = new Equipment(*pFabao);
						m_pGuildStore->Add(pEquip);
					}
					else if(*pData->byData == EIT_Holy)
					{
						tagHolyMan* pHolyMan = (tagHolyMan*)pItemData;
						Equipment* pEquip = new Equipment(*pHolyMan);
						m_pGuildStore->Add(pEquip);
					}
					else if(*pData->byData == EIT_HolyEquip)
					{
						tagHolyEquip* pHolyEquip = (tagHolyEquip*)pItemData;
						Equipment* pEquip = new Equipment(*pHolyEquip);
						m_pGuildStore->Add(pEquip);
					}
					else
					{
						tagEquip* pEquipData = (tagEquip*)pItemData;
						Equipment* pEquip = new Equipment(*pEquipData);
						m_pGuildStore->Add(pEquip);
					}
                }
                else
                {
                    Item* pItem = new Item(*pItemData);
                    m_pGuildStore->Add(pItem);
                }
            }
        	break;
        case EGWU_Delete:
            {
                Item* pItemRemove = m_pGuildStore->Remove(pData->nIndex);
                if (P_VALID(pItemRemove))
                    SAFE_DEL(pItemRemove);
            }
            break;
        }

        offset += pData->Size();
        
    }

    // 发送游戏事件
    tagGameEvent e(_T("InitGuildWareEvent"), NULL);
    m_pFrameMgr->SendEvent(&e);
}

Item* ItemMgr::GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc )
{
	return m_pPocket->GetSpecFuncItem( EICT_Bag, eItemSpecFunc );
}	

void ItemMgr::LoadBeGoldStoneItemTable()
{
	if(P_VALID(GetObj("GoldItemInfo")))
		KillObj("GoldItemInfo");
	m_pVarContainer = CreateObj("GoldItemInfo", "VarContainer");
	const TCHAR* szItemTable = _T("data\\system\\attdata\\BeGoldStone_Info.xml");
	list<tstring> listItemTable;
	m_pVarContainer->Load("VFS_System", szItemTable, "id", &listItemTable);
	list<tstring>::iterator iter = listItemTable.begin();

	for( ; iter!=listItemTable.end(); iter++ )
	{
		tagBeGoldStoneItemTable GoldItemInfo;
		GoldItemInfo.dwSrcItemID = m_pVarContainer->GetDword( _T("SrcItemID"), (*iter).c_str(), GT_INVALID );
		GoldItemInfo.dwDestItemID = m_pVarContainer->GetDword( _T("DestItemID"), (*iter).c_str(), GT_INVALID );
		GoldItemInfo.dwGoldStoneType = m_pVarContainer->GetDword( _T("GoldStoneType"), (*iter).c_str(), GT_INVALID );
		m_vecBeGoldStoneItemTable.push_back( GoldItemInfo );
	}
}

void ItemMgr::LoadTrumpLevelTable()
{
	if(P_VALID(GetObj("TrumpLevel")))
		KillObj("TrumpLevel");
	m_pVarContainer = CreateObj("TrumpLevel", "VarContainer");
	const TCHAR* szTrumpTable = _T("data\\system\\attdata\\level_fabao.xml");
	list<tstring> listTrumpTable;
	m_pVarContainer->Load("VFS_System", szTrumpTable, "id", &listTrumpTable);
	list<tstring>::iterator iter = listTrumpTable.begin();

	for( ; iter!=listTrumpTable.end(); iter++ )
	{
		tagFabaoLevelProto TrumpInfo;
		TrumpInfo.nID = m_pVarContainer->GetDword( _T("id"), (*iter).c_str(), GT_INVALID );
		TrumpInfo.nMinUseLevel = m_pVarContainer->GetDword( _T("ues_level_limit"), (*iter).c_str(), GT_INVALID );
		TrumpInfo.nLevelUpWhated = m_pVarContainer->GetDword( _T("exp_next_level"), (*iter).c_str(), GT_INVALID );
		TrumpInfo.nMinValue = m_pVarContainer->GetDword( _T("exp_total"), (*iter).c_str(), GT_INVALID );
		TrumpInfo.fCoefficientOfGrowing = m_pVarContainer->GetFloat( _T("coefficient_of_growing"), (*iter).c_str(), 0.0f );
		m_vecTrumpLevel.push_back( TrumpInfo );
	}
}

bool ItemMgr::IsInGoldStoneTable( DWORD dwItemId, DWORD dwGoldStoneType )
{
	for( size_t i = 0; i < m_vecBeGoldStoneItemTable.size(); i++ )
	{
		if( dwItemId == m_vecBeGoldStoneItemTable[i].dwSrcItemID &&
			dwGoldStoneType == m_vecBeGoldStoneItemTable[i].dwGoldStoneType )
		{
			return true;
		}
	}
	return false;
}

Item* ItemMgr::GetFirstSpecFuncItemFromPocket( EItemSpecFunc eItemSpecFunc, DWORD dwItemTypeID, BOOL bEnableType )
{
	if( !bEnableType )
	{
		return m_pPocket->GetSpecFuncItem( EICT_Bag, eItemSpecFunc );
	}
	else
	{
		return m_pPocket->GetSpecFuncItem( EICT_Bag, eItemSpecFunc, dwItemTypeID );
	}
}


DWORD ItemMgr::NetBeGoldStone( tagNS_BeGoldStone *pNetCmd, DWORD )
{

	switch (pNetCmd->dwErrCode)
	{
	case E_Success:
	case E_GoldStone_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GoldStoneSuccess")]);
		break;
	case E_GoldStone_MissingMatch:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GoldStoneMissingMatch")]);
		break;
	case E_GoldStone_SrcItemNotExist:		
	case E_GoldStone_NoConfigFile:
	case E_GoldStone_CreateDestItemFail:
	case E_GoldStone_OtherError:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GoldStoneFail")]);
		break;
	case E_GoldStone_BagIsFull:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GoldStoneBagIsFull")]);
		break;
	}

	return 0;
}

DWORD ItemMgr::OnNS_UnLockItem( tagNS_UnlockItemNotice *pNetCmd, DWORD )
{
	EItemConType eType = (EItemConType)pNetCmd->byContainerType;
	ItemContainer* pItemCon = NULL;
	switch(eType)
	{
	case EICT_Bag:
		pItemCon = GetPocket();
		break;
	case EICT_Baibao:
		pItemCon = GetThrone();
		break;
	case EICT_Equip:
		pItemCon = GetEquipment();
		break;
	case EICT_RoleWare:
		pItemCon = GetStorge();
		break;
	case EICT_Quest:
		pItemCon = GetQuest();
		break;
	case EICT_GuildWare:
		pItemCon = GetGuildStore();
		break;
	default:
		break;
	}

	if (P_VALID(pItemCon))
	{
		Item* pItem = pItemCon->GetItem( pNetCmd->u64ItemID );
		if (P_VALID(pItem))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_SuccessAll")] );
			if ( P_VALID(pItem) )
			{
				pItem->SetIMLocked( false );
			}
		}
	}
	return 0;
}

DWORD ItemMgr::OnNS_LockItem( tagNS_LockItem *pNetCmd, DWORD )
{
	if( !P_VALID(pNetCmd) )
		return 0;

	BYTE byUnLock = pNetCmd->byType;
	switch ( pNetCmd->dwErrorCode )
	{
	case E_Success:
		{
			ItemContainer* pItemCon = GetPocket();
			if( !P_VALID(pItemCon) )
				break;
			Item* pItem = pItemCon->GetItem( pNetCmd->n64Serial );

			if( !byUnLock )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemLock_Success")] );
				if ( P_VALID(pItem) )
				{
					pItem->SetIMLocked( true );
				}
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_Success")] );
				if( P_VALID(pItem) )
				{
					pItem->SetUnLockTime( pNetCmd->dwUnlockTime );
				}
			}
		}
		break;
	case E_ItemLock_HaveLocked:
		{
			if( !byUnLock )
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemLock_HaveLocked")] );
		}
		break;
	case E_ItemLock_NoLocked:
		{
			if( byUnLock )
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_NoLocked")] );
		}
		break;
	case E_ItemLock_TargetError:
		{
			if( byUnLock )
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_TargetError")] );
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemLock_TargetError")] );
		}
		break;
	case E_ItemLock_TargetInvalid:
		{
			if( byUnLock )
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_TargetIsNotEquipment")] );
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemLock_TargetIsNotEquipment")] );
		}
		break;
	case E_ItemLock_HaveNotItem:
		{
			if( byUnLock )
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_HaveNotItem")] );
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemLock_HaveNotItem")] );
		}
		break;
	case E_ItemLock_OtherError:
		{
			if( byUnLock )
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_Failed")] );
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemLock_Failed")] );
		}
		break;
	default:
		break;
	}

	return 0;
}

DWORD ItemMgr::OnNS_EquipSignature( tagNS_EquipSignature *pNetCmd, DWORD )
{
	if( !P_VALID(pNetCmd) )
		return 0;

	switch ( pNetCmd->dwErrorCode )
	{
	case E_Success:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_SignatureSuccess")] );
		}
		break;
	case E_EquipSig_HaveNoItem:
		{
			//署名道具非法
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_IsNotEquipSigIM")] );
		}
		break;
	case E_EquipSig_NoTarget:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_NoTarget")] );
		}
		break;
	case E_EquipSig_NotEquip:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_NoEquip")] );
		}
		break;
	case E_EquipSig_ContentLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_ContentLimit")] );
		}
		break;
	default:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_Failed")] );
		}
		break;
	}

	return 0;
}

void ItemMgr::OpenWalkWare()
{
	// 打开仓库界面
	ItemFrame* pItemFrame = (ItemFrame*)m_pFrameMgr->GetFrame(_T("Item"));
	if (!P_VALID(pItemFrame))
		return;

	StorageFrame* pFrame = (StorageFrame*)m_pFrameMgr->GetFrame(_T("Storage"));
	if (!P_VALID(pFrame))
	{
		pFrame = (StorageFrame*)m_pFrameMgr->CreateFrame(_T("Item"), _T("Storage"), _T("StorageFrame"), 0);
	}

	if(P_VALID(pFrame))
	{
		pFrame->SetWalkWareEnable(true);
		pItemFrame->SetCurStrategy(EIST_ItemStorage);
		MallMgr::Inst()->OpenEasyBuyFrame( _T("World"), _T("StorageEasyBuyFrame"), "\\desktop\\wh_win", EBUT_storageUI, "OuterRightTop", "storageui" );
	}
}

// 玩家使用的法宝最小等级
INT ItemMgr::GetTrumpLevel( INT nDengJie )
{
	vector<tagFabaoLevelProto>::const_iterator iter = m_vecTrumpLevel.begin();
	for( iter; iter != m_vecTrumpLevel.end(); iter++ )
	{
		if( iter->nID == nDengJie )
			return iter->nMinUseLevel;
	}
	return GT_INVALID;
}

// 法宝达到当前等阶需要的修炼值
INT ItemMgr::GetTrumpCurExp( INT nDengJie )
{
	vector<tagFabaoLevelProto>::const_iterator iter = m_vecTrumpLevel.begin();
	for( iter; iter != m_vecTrumpLevel.end(); iter++ )
	{
		if( iter->nID == nDengJie )
			return iter->nMinValue;
	}
	return GT_INVALID;
}

// 法宝升到下一阶需要多少修炼值
INT ItemMgr::GetTrumpNextLevelExp( INT nDengJie )
{
	vector<tagFabaoLevelProto>::const_iterator iter = m_vecTrumpLevel.begin();
	for( iter; iter != m_vecTrumpLevel.end(); iter++ )
	{
		if( iter->nID == nDengJie )
			return iter->nLevelUpWhated;
	}
	return GT_INVALID;
}

// 法宝（可装备前）所需的修炼值
INT ItemMgr::GetTrumpRefineExp( INT nPlayerLevel )
{
	if( m_vecTrumpLevel.empty() )
		return GT_INVALID;
	vector<tagFabaoLevelProto>::const_iterator iter = m_vecTrumpLevel.begin();
	for( iter; iter != m_vecTrumpLevel.end(); iter++ )
	{
		if( iter->nMinUseLevel > nPlayerLevel )
			return iter->nMinValue - 1;
	}
	iter--;
	return iter->nMinValue + iter->nLevelUpWhated;
}

// 法宝最大修炼值
INT ItemMgr::GetTrumpMaxExp()
{
	if( m_vecTrumpLevel.empty() )
		return GT_INVALID;
	vector<tagFabaoLevelProto>::const_iterator iter = m_vecTrumpLevel.end();
	--iter;
	return iter->nLevelUpWhated + iter->nMinValue;
}

VOID ItemMgr::GetEquipTrumpLevel(INT nExp, INT &nPlayerLevel, INT &DengJie )
{
	if( m_vecTrumpLevel.empty() )
		return;
	vector<tagFabaoLevelProto>::const_iterator iter = m_vecTrumpLevel.begin();
	for( iter; iter != m_vecTrumpLevel.end(); iter++ )
	{
		if( iter->nMinValue > nExp )
		{
			--iter;
			nPlayerLevel = iter->nMinUseLevel;
			DengJie = iter->nID;
			return;
		}
	}
	--iter;
	nPlayerLevel = iter->nMinUseLevel;
	DengJie = iter->nID;
}

DWORD ItemMgr::NetUpgradeMaxPot( tagNS_RaiseEquipPotval *pNetCmd, DWORD )
{
	switch (pNetCmd->dwErrorCode)
	{
	case E_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("UpgradeMaxPotSucc")]);
		break;
	case E_UpgradeMaxPot_NoNeed:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("UpgradeMaxPot_NoNeed")]);
		break;
	case E_UpgradeMaxPot_EquipPotTooLow:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("UpgradeMaxPot_EquipPotTooLow")]);
		break;
	default:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("UpgradeMaxPot_Fail")]);
		break;
	}

	return 0;
}

DWORD ItemMgr::NetSoulChange( tagNS_DetachOrAttackSoul_Equip *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
	{
		ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
		Item *pItem = pContainer->GetItem(pNetCmd->n64ID);
		if(!P_VALID(pItem))
			return 0;
		const tagItemProto *pItemProto = pItem->GetProto();
		if(!P_VALID(pItemProto))
			return 0;
		if( pNetCmd->bDetachOrAttack )
		{
			if( P_VALID(GetObj("XiHunSuccess")) )
				KillObj("XiHunSuccess");
			CreateObj( "XiHunSuccess", "MsgBoxEx" );
			TCHAR szTemp[X_LONG_NAME] = {0};
			_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("XiHunSuccess")], pItemProto->nSpecFuncVal1 );
			TObjRef<MsgBoxEx>("XiHunSuccess")->Init( _T(""), szTemp );
		}
		else
		{
			const tagItemDisplayInfo *pInfo = pItemProto->pDisplayInfo;
			Equipment *pEquip = (Equipment*)pContainer->GetItem(pNetCmd->n64ID);
			if(P_VALID(pInfo))
			{
				if( P_VALID(GetObj("FuHunSuccess")) )
					KillObj("FuHunSuccess");
				CreateObj( "FuHunSuccess", "MsgBoxEx" );
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("FuHunSuccess")], pInfo->szName, pEquip->GetEquip()->byConsolidateLevel );
				TObjRef<MsgBoxEx>("FuHunSuccess")->Init( _T(""), szTemp );
			}
		}
	}
	else if (pNetCmd->dwErrorCode == E_OnlyCanStrengthenByScript)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_OnlyCanStrengthenByScript")]);
	}

	return 0;
}

DWORD ItemMgr::NetGuiYuan( tagNS_GuiYuan *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GuiYuanSuccess")]);
	else if (pNetCmd->dwErrorCode == E_OnlyCanStrengthenByScript	)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_OnlyCanStrengthenByScript")]);

	return 0;
}

DWORD ItemMgr::NetFairyPowder( tagNS_FairyPowder *pNetCmd, DWORD )
{
	switch(pNetCmd->dwErrorCode)
	{
	case E_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SetFairyQuaitySuccess")]);
		break;
	case E_Fabao_RaiseIntelligence_Failse:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SetFairyQuaityFail")]);
		break;
	case E_Fabao_RaiseIntelligence_NoChange:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SetFairyQuaityNoChange")]);
		break;
	case E_Fabao_RaiseIntelligence_AlreadyFull:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SetFairyQuaityAlreadyFull")]);
		break;
	case E_UseItem_CoolDowning:
		{

		}
		break;
	case E_Fabao_RaiseIntelligence_FairyPowderNotFit:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("E_Fabao_RaiseIntelligence_FairyPowderNotFit")]);
		break;
	default:
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SetFairyQuaityFail")]);
		break;
	}
	return 0;
}

VOID ItemMgr::FouceUpdateItemCDTime(DWORD dwItemTypeID)
{
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(dwItemTypeID);
	if(P_VALID(pProto))
	{
		DWORD dwStartTime = Kernel::Inst()->GetAccumTimeDW();
		if(m_mapCurStartTime.IsExist(dwItemTypeID))
			m_mapCurStartTime.ChangeValue(dwItemTypeID, dwStartTime);
		else 
			m_mapCurStartTime.Add(dwItemTypeID, dwStartTime);

		tagGameEvent eve(_T("tagUpdateItemCDEvent"), NULL);
		m_pFrameMgr->SendEvent(&eve);
	}
}

VOID ItemMgr::FouceUpdateItemCDTime(DWORD dwItemTypeID,DWORD dwRemianSeconds)
{
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(dwItemTypeID);
	if(P_VALID(pProto))
	{
		DWORD dwStartTime = Kernel::Inst()->GetAccumTimeDW();
		FLOAT fCDTime = (FLOAT)pProto->dwCooldownTime;	// 毫秒
		DWORD dwPassTime = DWORD(fCDTime - dwRemianSeconds*1000);
		dwStartTime -= dwPassTime;

		if(m_mapCurStartTime.IsExist(dwItemTypeID))
			m_mapCurStartTime.ChangeValue(dwItemTypeID, dwStartTime);
		else 
			m_mapCurStartTime.Add(dwItemTypeID, dwStartTime);

		tagGameEvent eve(_T("tagUpdateItemCDEvent"), NULL);
		m_pFrameMgr->SendEvent(&eve);
	}
}

DWORD ItemMgr::OnNS_SyncOpenChaseRemainTime(tagNS_SyncOpenChaseRemainTime* pNetCmd,DWORD)
{
	Item* pItem = GetFirstSpecFuncItemFromPocket( EISF_TreasureSeizer );
	if (P_VALID(pItem))
	{
		FouceUpdateItemCDTime(pItem->GetItemTypeID(),pNetCmd->dwOpenChaseRemainTime);
	}
	return 0;
}

DWORD ItemMgr::OnNS_ForceLootChase(tagNS_ForceLootChase* pNetCmd,DWORD)
{
	//CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ForceLost_Chase")]);

	if(P_VALID(GetObj("BaoXiangForceLost")))
		KillObj("BaoXiangForceLost");

	CreateObj("BaoXiangForceLost", "MsgBoxEx");
	TObjRef<MsgBoxEx>("BaoXiangForceLost")->Init( _T(""), g_StrTable[_T("ForceLost_Chase")],NULL ,MBF_OK,FALSE);
	return 0;
}

Item* ItemMgr::GetPocketFirstItemByTypeID(DWORD dwTypeID)
{
	return m_pPocket->GetFirstItemByTypeID(dwTypeID);
}

DWORD ItemMgr::OnNS_PlayerRegression( tagNS_PlayerRegression* pNetCmd, DWORD dwParam )
{
	OldPlayerComeBackFrame *pFrame = (OldPlayerComeBackFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("OldPlayerComeBack") );
	if( !P_VALID(pFrame) )
		pFrame = (OldPlayerComeBackFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("OldPlayerComeBack"), _T("OldPlayerComeBackFrame"), 0 );
	if(P_VALID(pFrame))
		pFrame->Show(pNetCmd->dwItemTypeID, pNetCmd->nItemNum, pNetCmd->byQuality);
	return 0;
}

Equipment* ItemMgr::GetFashion(DWORD dwTypeID, BOOL bDIY /* = FALSE */)
{
	return (Equipment*)m_pClothespress->GetClothsepressItem(dwTypeID, bDIY);
}

Equipment* ItemMgr::GetFashion(INT64 n64ID)
{
	return (Equipment*)m_pClothespress->GetItem(n64ID);
}

DWORD ItemMgr::OnNS_UpdateFashionInfo( tagNS_SyncFashionInfo* pNetCmd, DWORD dwParam )
{
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLP))
		return 0;
	pLP->PlayFashionEffect(0);
	tagFashionInfo *pFashionInfo = (tagFashionInfo *)pNetCmd->FashionInfo;
	for( INT i = 0; i < pNetCmd->nSuitNum; i++ )
	{
		tagFashionInfo *pInfo = new tagFashionInfo(); 
		pInfo->byLevel = pFashionInfo[i].byLevel;
		pInfo->dwIndex = pFashionInfo[i].dwIndex;
		pInfo->eState = pFashionInfo[i].eState;
		if(m_mapFashion.IsExist(pInfo->dwIndex))
			m_mapFashion.ChangeValue(pInfo->dwIndex, pInfo);
		else 
			m_mapFashion.Add(pInfo->dwIndex, pInfo);

		if(pInfo->eState == EFS_UseBySelf)
		{
			BYTE nMettle = GetSuitMettle(pInfo->dwIndex);
			pLP->PlayFashionEffect(nMettle);
		}
	}

	tagGameEvent eve(_T("tagUpdateFashionInfo"), NULL);
	m_pFrameMgr->SendEvent(&eve);
	return 0;
}

DWORD ItemMgr::OnNS_NetDressUpAnotherRole( tagNS_DressUpAnotherRole* pNetCmd, DWORD dwParam )
{
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return 0;
	if(pLp->GetID() != pNetCmd->dwSrcRoleID)
		return 0;

	if(!pNetCmd->bAgree)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FriendRefuseYourRequest")]);
	else
	{
		switch(pNetCmd->dwErrorCode)
		{
		case EFEC_Success:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BeautyFriendSucc")]);
			break;
		case EFEC_FriendNotOnLine:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Social_ServerError_Outline")]);
			break;
		case EFEC_IsWaitingPlayActFriendReq:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BeautyFriendWaitFeedback")]);
			break;
		case EFEC_PlayActCountLimit:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BeautyFriendNumLimit")]);
			break;
		case EFEC_IsPlayActing:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FriendHasBeenBetauty")]);
			break;
		default:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BeautyFriendFail")]);
			break;
		}
	}
	return 0;
}

VOID ItemMgr::GetAllSuitableFashion( bool bMan, vector<DWORD> &vecSuit )
{
	if(m_mapFashion.Empty())
		return;

	vector<tagClothespressData> vecFashion;
	if(bMan)
		vecFashion = ClothespressData::Inst()->GetMaleFashionData();
	else
		vecFashion = ClothespressData::Inst()->GetFemaleFashionData();
		
	m_mapFashion.ResetIterator();
	DWORD dwSuitID = GT_INVALID;
	tagFashionInfo *pInfo = NULL;
	while(m_mapFashion.PeekNext(dwSuitID, pInfo))
	{
		if(P_VALID(pInfo) && pInfo->eState == EFS_Complete)
		{
			for( size_t i = 0; i < vecFashion.size(); i++)
			{
				if(vecFashion[i].nSuitId == dwSuitID)
				{
					vecSuit.push_back(dwSuitID);
					break;
				}
			}
		}
	}
	return;
}

BYTE ItemMgr::GetSuitMettle(DWORD dwSuitIndex)
{
	tagClothespressData *pData = ClothespressData::Inst()->GetFashionInfo(dwSuitIndex);
	if(!P_VALID(pData))
		return 0;

	BYTE byMettle = 0;
	BOOL bDIY = pData->dwSuitTypeID[0] == GT_INVALID ? TRUE :FALSE;
	if(bDIY)
	{
		for( INT16 i = 0; i < 5; i++ )
		{
			Equipment *pEquip = (Equipment*)m_pClothespress->GetValue(i);
			if(!P_VALID(pEquip))
				continue;
			const tagEquipSpec *pSpec = pEquip->GetEquip();
			byMettle += (BYTE)pSpec->n16Appearance;
		}
	}
	else
	{
		for( int i = 0; i < 5; i++ )
		{
			Equipment *pEquip = GetFashion(pData->dwSuitTypeID[i], bDIY);
			if(!P_VALID(pEquip))
				continue;
			const tagEquipSpec *pSpec = pEquip->GetEquip();
			byMettle += (BYTE)pSpec->n16Appearance;
		}
	}
	return byMettle;
}

DWORD ItemMgr::OnNS_FairyDoRaiseIntelligence(tagNS_FairyDoRaiseIntelligence* pNetCmd,DWORD)
{
	switch (pNetCmd->dwErrorCode)
	{
	case E_Fabao_RaiseIntelligence_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_RaiseIntelligence_Success")]);
	break;
	case E_Fabao_RaiseIntelligence_FairyPowderNotFit:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_RaiseIntelligence_FairyPowderNotFit")]);
		break;
	case E_Fabao_Spec98ItemNotExist:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Spec98ItemNotExist")]);
		break;
	case E_Fabao_Spec99ItemNotExist:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Spec99ItemNotExist")]);
		break;
	case E_Fabao_ItemNotFit2FairyIntellgence:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_ItemNotFit2FairyIntellgence")]);
		break;
	case E_Fabao_ItemUseFailed:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_ItemUseFailed")]);
		break;
	}
	return 0;
}
DWORD ItemMgr::OnNS_FairyRaiseIntelligenceMax(tagNS_FairyRaiseIntelligenceMax* pNetCmd,DWORD)
{
	switch (pNetCmd->dwErrorCode)
	{
	case E_Fabao_Spec98Item_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Spec98Item_Success")]);
		break;
	case E_Fabao_RaiseIntelligence_FairyPowderNotFit:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_RaiseIntelligence_FairyPowderNotFit")]);
		break;
	case E_Fabao_Spec98ItemNotExist:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Spec98ItemNotExist")]);
		break;
	case E_Fabao_Spec99ItemNotExist:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Spec99ItemNotExist")]);
		break;
	case E_Fabao_ItemNotFit2FairyIntellgence:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_ItemNotFit2FairyIntellgence")]);
		break;
	case E_Fabao_Spec98Item_AlreadyUse:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Spec98Item_AlreadyUse")]);
		break;
	case E_Fabao_Spec98Item_UseFailed:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Spec98Item_UseFailed")]);
		break;
	case E_Fabao_ItemUseFailed:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_ItemUseFailed")]);
		break;
	}

	return 0;
}

DWORD ItemMgr::OnNS_NewSoulCrystalAdd( tagNS_NewSoulCrystalAdd* pNetCmd, DWORD )
{

	Equipment* pEquip = new Equipment(pNetCmd->hunjing);
	if(P_VALID(pEquip))
	{
		AddNewItem(pEquip);

		//发送游戏事件
		ASSERT(P_VALID(pEquip));
		tagCombatMsgForItemChange event(_T("CombatMsg_ItemAdd"), NULL);
		event.dwTypeID = pEquip->GetItemTypeID();
		event.n16Num = pEquip->GetItemQuantity();
		event.nQuality = pEquip->GetItemQuality();
		m_pFrameMgr->SendEvent(&event);

	}
	return 0;
}

DWORD ItemMgr::OnNS_DiamondEquipGrowIM(tagNS_DiamondEquipGrowIM* pNetCmd, DWORD dwParam)
{
	if(pNetCmd->dwErrorCode == 0)
	{
		//判断不合法
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr49")]);
	}
	else if(pNetCmd->dwErrorCode == 1)
	{
		//操作成功
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("RockGod_success")]);
	}
	return 0;
}

DWORD ItemMgr::NetShengLingChange(tagNS_ShengLingChg* pNetCmd, DWORD)
{
	//--如果物品位置有效且为装备
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(pNetCmd->n64Serial);
	if (!P_VALID(pEquip))
	{
		pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_Equip, pNetCmd->n64Serial);
	}

	//--如果有该装备且装备ID与服务器传过来的装备ID相符
	if(P_VALID(pEquip) && MIsEquipment(pEquip->GetItemTypeID()))
	{
		//--更新装备数据
		pEquip->UpdateEquipment(pNetCmd->info);
		tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
		eve.eType = pEquip->GetConType();
		eve.n16Index = pEquip->GetPos();
		m_pFrameMgr->SendEvent(&eve);
	}

	return 0;
}

DWORD ItemMgr::NetShengLingEquipChange(tagNS_HolyEquipChg* pNetCmd, DWORD)
{
	//--如果物品位置有效且为装备
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_HolyManEquip, pNetCmd->n64Serial);
	if (!P_VALID(pEquip))
	{
		pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(pNetCmd->n64Serial);
	}

	//--如果有该装备且装备ID与服务器传过来的装备ID相符
	if(P_VALID(pEquip) && MIsEquipment(pEquip->GetItemTypeID()))
	{
		//--更新装备数据
		pEquip->UpdateEquipment(pNetCmd->info);
		tagUpdateLPItemEvent eve(_T("ItemEvent_EquipChange"),NULL);
		eve.eType = pEquip->GetConType();
		eve.n16Index = pEquip->GetPos();
		m_pFrameMgr->SendEvent(&eve);
	}

	return 0;
}

DWORD ItemMgr::OnNS_EquipPerfectGrow(tagNS_EquipPerfectGrow* pNetCmd, DWORD dwParam)
{
	switch(pNetCmd->dwErrorCode)
	{
	case E_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
		break;
	case E_EquipPerfectGrow_IMInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipPerfectGrow_IMInvalid")]);
		break;
	case E_EquipPerfectGrow_EquipInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipPerfectGrow_EquipInvalid")]);
		break;
	case E_EquipPerfectGrow_TimeLimit:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipPerfectGrow_TimeLimit")]);
		break;
	}

	return 0;
}

DWORD ItemMgr::OnNS_XiPurpleStar(tagNS_XiPurpleStar* pNetCmd, DWORD dwParam)
{
	switch(pNetCmd->dwErrorCode)
	{
	case E_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
		break;
	case E_EquipXiStar_IMInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipXiStar_IMInvalid")]);
		break;
	case E_EquipXiStar_EquipInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipXiStar_EquipInvalid")]);
		break;
	case E_EquipXiStar_NumNotEnough:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipXiStar_NumNotEnough")]);
		break;
	case E_EquipXStar_TimeLimitEquip:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipXStar_TimeLimitEquip")]);
		break;
		
	}
	return 0;
}

DWORD ItemMgr::OnNS_ShengLingCoValueChg(tagNS_ShengLingCoValueChg* pNetCmd, DWORD)
{
	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(P_VALID(pEquip))
	{
		tagHolySpec pSpec;
		memcpy(&pSpec, pEquip->GetHolySpec(), sizeof(tagHolySpec));
		pSpec.nCoValue = pNetCmd->nCoValue;
		pEquip->UpdateEquipment(pSpec);
	}

	return 0;
}