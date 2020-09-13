#include "StdAfx.h"
#include "HolyManMgr.h"
#include "HolyManFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "CombatSysUtil.h"
#include "ItemMgr.h"
#include "Item.h"
#include "Container.h"
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\msg_yuanshen.h"
#include "..\WorldDefine\compose_define.h"
#include "ItemProtoData.h"
#include "ItemEvent.h"
#include "VirtualRole.h"

HolyManMgr::HolyManMgr(void)
:m_trunk(this)
,m_bHolyAwake(FALSE)
,m_bHolyManActive(FALSE)
/*,m_dwCurHolyManValue(0)*/
,m_bIsUpdateExp(FALSE)
,m_bLoginInit(TRUE)
{
}

HolyManMgr::~HolyManMgr(void)
{
}

HolyManMgr* HolyManMgr::Inst()
{
	static HolyManMgr inst;
	return &inst;
}

void HolyManMgr::Init()
{

	m_pCmdMgr->Register("NS_SoulEquip",		(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_SoulEquip),		_T("NS_SoulEquip") );
	m_pCmdMgr->Register("NS_UnSoulEquip",	(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_UnSoulEquip),	_T("NS_UnSoulEquip") );
	m_pCmdMgr->Register("NS_GetSoulInfo",	(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_GetSoulInfo),	_T("NS_GetSoulInfo") );
	m_pCmdMgr->Register("NS_ActivateHolySoul",	(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_ActivateHolySoul),	_T("NS_ActivateHolySoul") );
	m_pCmdMgr->Register("NS_HolySoulAttChange",	(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_HolySoulAttChange),	_T("NS_HolySoulAttChange") );
	m_pCmdMgr->Register("NS_UpdateHolySoulExp",(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_UpdateHolySoulExp), _T("NS_UpdateHolySoulExp"));

	m_bLoginInit = TRUE;
	m_bHolyAwake = FALSE;
	/*	m_dwCurHolyManValue = 0;*/
	m_bHolyManActive = FALSE;

	ClearHolyManInfoMap();
	ClearHolyComposeMap();
	ClearHolyRebornMap();
	ClearHolyChiselMap();
	ClearHolyInlayMap();
	LoadFromFile();

}

void HolyManMgr::Destroy()
{

	m_pCmdMgr->UnRegister( "NS_SoulEquip",			(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_SoulEquip) );
	m_pCmdMgr->UnRegister( "NS_UnSoulEquip",		(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_UnSoulEquip) );
	m_pCmdMgr->UnRegister( "NS_GetSoulInfo",		(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_GetSoulInfo) );
	m_pCmdMgr->UnRegister( "NS_ActivateHolySoul",		(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_ActivateHolySoul) );
	m_pCmdMgr->UnRegister( "NS_HolySoulAttChange",		(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_HolySoulAttChange) );
	m_pCmdMgr->UnRegister("NS_UpdateHolySoulExp",(NETMSGPROC)m_trunk.sfp2(&HolyManMgr::OnNS_UpdateHolySoulExp));

	ClearHolyManInfoMap();
	ClearHolyComposeMap();
	ClearHolyRebornMap();
	ClearHolyChiselMap();
	ClearHolyInlayMap();
}

BOOL HolyManMgr::LoadFromFile()
{
	//角色动作与原神动作映射表
	m_mapActionSyn.clear();
	tstring path = g_strLocalPath +  _T("\\attdata\\action_match.xml");
	const TCHAR* szPathName = path.c_str();
	list<tstring> strNameList;
	TObjRef<VarContainer> varName = CreateObj( "ActionName", "VarContainer" );
	if(!varName->Load("VFS_System", szPathName, "id", &strNameList))
	{
		IMSG(_T("Load file action_match.xml failed\r\n"));
		return FALSE; 
	}
	for(list<tstring>::iterator it=strNameList.begin(); it!=strNameList.end(); ++it)
	{
		tstring strRoleAction = varName->GetString(_T("role_action"), it->c_str(), _T("")) ;
		tstring strSoulAction = varName->GetString(_T("soul_action"),  it->c_str(), _T("")) ;
		m_mapActionSyn.insert(make_pair(strRoleAction, strSoulAction));
	}

	KillObj("ActionName");

	//元神模型表
// 	tstring path1 =  _T("data\\system\\attdata\\soul_att_def.xml");
// 	const TCHAR* szPathName1 = path1.c_str();
// 	list<tstring> strNameList1;
// 	TObjRef<VarContainer> varName1 = CreateObj( "HolyModelInfo", "VarContainer" );
// 	if(!varName1->Load("VFS_System", szPathName1, "id", &strNameList1))
// 	{
// 		IMSG(_T("Load file soul_att_def.xml failed\r\n"));
// 		return FALSE; 
// 	}
// 	for(list<tstring>::iterator it=strNameList1.begin(); it!=strNameList1.end(); ++it)
// 	{
// 		tagHolyModelInfo*  pData	= new tagHolyModelInfo;
// 		pData->dwHolyManID		= (DWORD)varName1->GetDword( _T("id"),	 it->c_str(), GT_INVALID );
// 		pData->dwSoulValue		= (DWORD)varName1->GetDword( _T("max_soul"),	 it->c_str(), 0 );
// 		pData->strModelMen		= (tstring)varName1->GetString(_T("male_model"), it->c_str(), _T("")) ;
// 		pData->strModelWomen	= (tstring)varName1->GetString(_T("famale_model"),  it->c_str(), _T("")) ;
// 		pData->strModelEffect		= (tstring)varName1->GetString(_T("attach_sfx"),  it->c_str(), _T("")) ;
// 		pData->nModelScale		=	(INT) varName1->GetInt( _T("size"),	it->c_str(), 1 );
// 		m_mapHolyManInfo.insert(make_pair( pData->dwHolyManID, pData ));
// 	}
// 	KillObj("HolyModelInfo");
// 
// 	//元神模型表
// 	tstring path2 =  _T("data\\system\\attdata\\star_enhance.xml");
// 	const TCHAR* szPathName2 = path2.c_str();
// 	list<tstring> strNameList2;
// 	TObjRef<VarContainer> varName2 = CreateObj( "HolyCompose", "VarContainer" );
// 	if(!varName2->Load("VFS_System", szPathName2, "id", &strNameList2))
// 	{
// 		IMSG(_T("Load file star_enhance.xml failed\r\n"));
// 		return FALSE; 
// 	}
// 	for(list<tstring>::iterator it=strNameList2.begin(); it!=strNameList2.end(); ++it)
// 	{
// 		tagStarEnhanceProto*  pData	= new tagStarEnhanceProto;
// 		pData->byID						= (BYTE)varName2->GetInt( _T("id"),	it->c_str(), 0 );			//元神装备的星级
// 		pData->dwCostItemId		= (DWORD)varName2->GetDword( _T("costitemid"),	 it->c_str(), GT_INVALID );	//强化星级消耗的道具的ID
// 		pData->nCostItemNum	= (UINT16)varName2->GetInt( _T("costitemnum"),	it->c_str(), 0 );	//强化星级消耗的道具的数量
// 		pData->nRate					= (UINT16)varName2->GetInt( _T("rate"),	it->c_str(), 0 );			//强化对应星级的成功几率
// 		pData->byFailLevel			= (BYTE)varName2->GetInt( _T("faillevel"),	it->c_str(), 0 );	//强化星级失败后退回的等级
// 		pData->dwCost				= (DWORD)varName2->GetDword( _T("cost"),	 it->c_str(), 0 );			//强化星级消耗的金钱数
// 		pData->nStunt					= (UINT)varName2->GetInt( _T("stunt"),	it->c_str(), 0 );			//绝技伤害加成
// 		pData->nHitRate				= (UINT)varName2->GetInt( _T("hitrate"),	it->c_str(), 0 );		//命中加成
// 		pData->nCritRate				= (UINT)varName2->GetInt( _T("critrate"),	it->c_str(), 0 );		//致命加成
// 		pData->nCritaMount		= (UINT)varName2->GetInt( _T("critamount"),	it->c_str(), 0 );	//致命量加成
// 		pData->nExAttack				= (UINT)varName2->GetInt( _T("exattack"),	it->c_str(), 0 );		//外功攻击加成
// 		pData->nInAttack				= (UINT)varName2->GetInt( _T("inattack"),	it->c_str(), 0 );		//内功攻击加成
// 		pData->nAccuRate			= (UINT)varName2->GetInt( _T("accurate"),	it->c_str(), 0 );		//精准加成
// 		pData->nExDamage			= (UINT)varName2->GetInt( _T("exdamage"),	it->c_str(), 0 );		//额外伤害加成
// 		pData->nNegToughness	= (UINT)varName2->GetInt( _T("neglecttoughness"),	it->c_str(), 0 );	//忽视韧性加成
// 		pData->nMorale				= (UINT)varName2->GetInt( _T("morale"),	it->c_str(), 0 );		//士气加成
// 
// 		m_mapHolyCompose.insert(make_pair( pData->byID, pData ));
// 	}
// 	KillObj("HolyCompose");
// 
// 	//元神转生概率表
// 	tstring path3 =  _T("data\\system\\attdata\\soul_reborn.xml");
// 	const TCHAR* szPathName3 = path3.c_str();
// 	list<tstring> strNameList3;
// 	TObjRef<VarContainer> varName3 = CreateObj( "HolyReborn", "VarContainer" );
// 	if(!varName3->Load("VFS_System", szPathName3, "costitemnum", &strNameList3))
// 	{
// 		IMSG(_T("Load file soul_reborn.xml failed\r\n"));
// 		return FALSE; 
// 	}
// 	for(list<tstring>::iterator it=strNameList3.begin(); it!=strNameList3.end(); ++it)
// 	{
// 		INT	nLv1Rate		= (INT)varName3->GetInt( _T("rate1"),	it->c_str(), 0 );			 
// 		INT	nLv2Rate		= (INT)varName3->GetInt( _T("rate2"), it->c_str(), 0 );	 
// 		INT	nItemNum	= (INT)varName3->GetInt( _T("costitemnum"),	it->c_str(), -1 );		
// 		tagHolySoulReborn  data;
// 		data.byCostNum  =  (BYTE)nItemNum;
// 		data.nRate1			= nLv1Rate/100;
// 		data.nRate2			= nLv2Rate/100;
// 		m_mapHolyRebornRate.insert(make_pair( nItemNum, data ));
// 	}
// 	KillObj("HolyReborn");
// 
// 	//元神装备凿孔表
// 	tstring path4 =  _T("data\\system\\attdata\\soul_equip_hole.xml");
// 	const TCHAR* szPathName4 = path4.c_str();
// 	list<tstring> strNameList4;
// 	TObjRef<VarContainer> varName4 = CreateObj( "HolyChisel", "VarContainer" );
// 	if(!varName4->Load("VFS_System", szPathName4, "id", &strNameList4))
// 	{
// 		IMSG(_T("Load file soul_equip_hole.xml failed\r\n"));
// 		return FALSE; 
// 	}
// 	for(list<tstring>::iterator it=strNameList4.begin(); it!=strNameList4.end(); ++it)
// 	{
// 		tagHolySoulEquipHole  data;
// 		data.byID					= (BYTE)varName4->GetInt( _T("id"),	it->c_str(), 0 );
// 		data.nRate				= (INT)varName4->GetInt( _T("rate"), it->c_str(), 0 );	 
// 		data.nCostMoney	= (INT)varName4->GetInt( _T("cost"), it->c_str(), 0 );	 
// 		m_mapHolyChisel.insert(make_pair( data.byID, data ));
// 	}
// 	KillObj("HolyChisel");
// 
// 	//元神装镶嵌孔表
// 	tstring path5 =  _T("data\\system\\attdata\\soul_equip_inlay.xml");
// 	const TCHAR* szPathName5 = path5.c_str();
// 	list<tstring> strNameList5;
// 	TObjRef<VarContainer> varName5 = CreateObj( "HolyInlay", "VarContainer" );
// 	if(!varName5->Load("VFS_System", szPathName5, "id", &strNameList5))
// 	{
// 		IMSG(_T("Load file soul_equip_inlay.xml failed\r\n"));
// 		return FALSE; 
// 	}
// 	for(list<tstring>::iterator it=strNameList5.begin(); it!=strNameList5.end(); ++it)
// 	{
// 		tagHolySoulEquipInlay  data;
// 		data.byID					= (BYTE)varName5->GetInt( _T("id"),	it->c_str(), 0 );
// 		data.nRate				= (INT)varName5->GetInt( _T("rate"), it->c_str(), 0 );	 
// 		data.nCostMoney	= (INT)varName5->GetInt( _T("cost"), it->c_str(), 0 );	 
// 		m_mapHolyInlay.insert(make_pair( data.byID, data ));
// 	}
// 	KillObj("HolyInlay");

	return TRUE;
}

BOOL HolyManMgr::IsHolyAwake() 
{
	BOOL bAwake = FALSE;
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( P_VALID(pLp) )
		bAwake = pLp->IsPlayerHolyAwake();

	//VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(dwLpID);
	//if ( P_VALID(pVp) )
	//	bAwake = ! pVp->IsHide();
	return bAwake;
}

VOID HolyManMgr::CheckAwakeState()
{
	if ( m_bHolyAwake && m_bHolyManActive)
	{
		if ( (m_SoulAttInfo.SoulAttEx).nCurSoulValue<=0)
		{
			DWORD dwLpId = RoleMgr::Inst()->GetLocalPlayerID();
			VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(dwLpId);
			if ( P_VALID(pVp) )
			{
				tagVirtualRoleEvent event;
				event.ePlayAct = EPAT_DISPPEAR;
				pVp->OnGameEvent(&event);
			}
		}
	}
}

VOID HolyManMgr::AddSoulExp(INT64 AddValue) 
{ 
	tagSoulAttEx soulAttEx = m_SoulAttInfo.SoulAttEx;
	if ( soulAttEx.n64LevelExp + AddValue < soulAttEx.n64MaxExp )
		(m_SoulAttInfo.SoulAttEx).n64LevelExp +=AddValue;
}

tstring HolyManMgr::GetMatchCombatAction(tstring strAct)
{ 
	if ( !strAct.empty() )
	{
		std::map<tstring,tstring>::iterator it = m_mapActionSyn.find(strAct);
		if ( it != m_mapActionSyn.end() )
		{
			tstring str = it->second;
			if ( !str.empty() )
				return it->second;	
		}
	}
	return _T("");
}


EHolyEquipType HolyManMgr::IndexToType(INT nIndex)
{
	switch (nIndex)
	{
	case 0:		return EHET_TIANCHONG;
	case 1:		return EHET_LINGHUI;
	case 2:		return EHET_QIPO;
	case 3:		return EHET_LIPO;
	case 4:		return EHET_ZHONGSHU;
	case 5:		return EHET_JINGPO;
	case 6:		return EHET_YINGPO;
	case 7:		return EHET_TIANHUN;
	case 8:		return EHET_DIHUN;
	case 9:		return EHET_MINGHUN;
	default:		return EHET_NULL;
	}
	return EHET_NULL; 
}

INT HolyManMgr::TypeToIndex( EHolyEquipType eType)
{
	switch (eType)
	{
	case EHET_TIANCHONG:	return 0;
	case EHET_LINGHUI:			return 1;
	case EHET_QIPO:				return 2;
	case EHET_LIPO:				return 3;
	case EHET_ZHONGSHU:	return 4;
	case EHET_JINGPO:			return 5;
	case EHET_YINGPO:			return 6;
	case EHET_TIANHUN:		return 7;
	case EHET_DIHUN:			return 8;
	case EHET_MINGHUN:		return 9 ;
	default:		 						return GT_INVALID;
	}
	return 0; 
}

//判断一个装备是否为元神装备
BOOL HolyManMgr::IsHolyManEquip(DWORD dwTypeID)
{
// 	const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto( dwTypeID );
// 	if ( P_VALID(pProto) )
// 	{
// 		if ( pProto->eTypeEx>=EHET_TIANHUN && pProto->eTypeEx<=EHET_YINGPO )
// 			return TRUE;
// 	}
	return FALSE;
}

//获得元神装备加成类型名
tstring HolyManMgr::GetHolyEquipAttTypeName(DWORD dwTypeID)
{
// 	const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto( dwTypeID );
// 	if ( P_VALID(pProto) && IsHolyManEquip(dwTypeID) )
// 	{
// 		INT nType = INT(pProto->BaseEffect[0].eRoleAtt);
// 		tstringstream tssIndex;
// 		tssIndex<<_T("HolyMan_Tips")<<nType;
// 		tstring strType = g_StrTable[tssIndex.str().c_str()];
// 
// 		return strType;
// 	}
	return _T("");
}

//获得元神装备加成类型
EHolyEquipAttType HolyManMgr::GetHolyEquipAttType(DWORD dwTypeID)
{
// 	EHolyEquipAttType eType = EHEAT_NULL;
// 	const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto( dwTypeID );
// 	if ( P_VALID(pProto) && IsHolyManEquip(dwTypeID) )
// 	{
// 		INT nType = INT((pProto->BaseEffect[0].eRoleAtt)+1)%1000;
// 		if ( (EHolyEquipAttType)nType>EHEAT_NULLEX && (EHolyEquipAttType)nType<EHEAT_End )
// 			eType = (EHolyEquipAttType)nType;
// 	}
	return EHEAT_NULL;
}

//获得元神装备加成类型
INT	HolyManMgr::GetHolyEquipAttValue(DWORD dwTypeID)
{
// 	INT nType = 0;
// 	const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto( dwTypeID );
// 	if ( P_VALID(pProto) && IsHolyManEquip(dwTypeID) )
// 		nType = INT(pProto->BaseEffect[0].nValue);

	return ERA_Null;
}

//装备元神装备位置
EHolyEquipType HolyManMgr::GetHolyManEquipPos(DWORD dwTypeID)
{
	const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto( dwTypeID );
	if ( P_VALID(pProto) )
	{
		if ( pProto->eTypeEx>=EHET_TIANHUN && pProto->eTypeEx<=EHET_YINGPO )
			return  EHolyEquipType(pProto->eTypeEx);
	}
	return EHET_NULL;
}

//战斗状态检查
BOOL HolyManMgr::IsCombatState( HolyCombatStateCheckType eType/* = EHCSCT_EQUIP*/ )
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID(pLp) )
		return TRUE;
	//如果本地玩家在备战状态或使用技能不容许换装
	DWORD dwErrIndex = 50 + DWORD(eType) ;
	if( pLp->IsSkillState() || pLp->IsReadyState() )
	{
		ShowErrorTips(dwErrIndex);
		return TRUE;
	}

	return FALSE;
}

//穿装备
VOID HolyManMgr::SendMsg_HolyEquip(EHolyEquipPos ePos, INT16 n16Pos)
{
// 	if (IsCombatState(EHCSCT_EQUIP))	return;
// 
// 	ItemContainer* pHolyEquip = ItemMgr::Inst()->GetHolyEquip();
// 	Item* pItem = ItemMgr::Inst()->GetPocketItem(n16Pos);
// 	if( P_VALID(pHolyEquip) && P_VALID(pItem) && ePos>EHEFI_Start && ePos<EHEFI_End )
// 	{
// 		EHolyEquipType eHEType = (EHolyEquipType)pItem->GetItemTypeEx();
// 		EHolyEquipPos eHEPos = (EHolyEquipPos)TypeToIndex( eHEType );
// 		if ( eHEType == GT_INVALID || eHEPos != ePos )
// 		{
// 			ShowErrorTips(21);
// 			//CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HolyMan_Tips21")]);
// 		}
// 		else
// 		{
// 			tagNC_SoulEquip msg;
// 			msg.ePosDst = ePos;
// 			msg.n64Serial = pItem->GetItemId();
// 			m_pSession->Send(&msg);
// 			m_bLoginInit = TRUE;
// 		}
// 	}
}

//脱装备
VOID HolyManMgr::SendMsg_UnHolyEquip(EHolyEquipPos ePos,INT16 n16Pos/*=GT_INVALID*/)
{
// 	if (IsCombatState(EHCSCT_EQUIP))	return;
// 
// 	ItemContainer* pHolyEquip = ItemMgr::Inst()->GetHolyEquip();
// 	if( P_VALID(pHolyEquip) )
// 	{
// 		Item* pItem = pHolyEquip->GetValue((INT16)ePos);
// 		if ( P_VALID(pItem) && ePos>EHEFI_Start && ePos<EHEFI_End )
// 		{
// 			tagNC_UnSoulEquip msg;
// 			msg.n16PosDst	= n16Pos;
// 			msg.n64Serial	= pItem->GetItemId();
// 			m_pSession->Send(&msg);
// 			m_bLoginInit = TRUE;
// 		}
// 	}

}

//刷新元神属性值信息
VOID HolyManMgr::UpdateSoulAttValue( EHolyEquipAttType eType /*= EHEAT_NULL*/ )
{
	tagUpdateHolyManEvent evt( _T("tagUpdateHolyManEvent"), NULL );
	evt.eAttType = eType;
	m_pFrameMgr->SendEvent( &evt );
}

tstring HolyManMgr::GetHolyManSkelPath(DWORD dwModelID,BYTE  bySex)
{
	std::map<DWORD,const tagHolyModelInfo*>::iterator it = m_mapHolyManInfo.find(dwModelID);
	if( it != m_mapHolyManInfo.end() )
	{
		const	tagHolyModelInfo* pInfo = it->second;
		if (P_VALID(pInfo))
		{
			tstringstream tss;
			if ( bySex == 1)
				tss<<_T("data\\animation\\npc\\")<<pInfo->strModelMen<<_T("\\std.fskel");
			else
				tss<<_T("data\\animation\\npc\\")<<pInfo->strModelWomen<<_T("\\std.fskel");
			return	tss.str();
		}
	}
	return _T("");

}

tstring HolyManMgr::GetHolyManSkelName(DWORD dwModelID,BYTE  bySex)
{
	std::map<DWORD,const tagHolyModelInfo*>::iterator it = m_mapHolyManInfo.find(dwModelID);
	if( it != m_mapHolyManInfo.end() )
	{
		const tagHolyModelInfo* pInfo = it->second;
		if (P_VALID(pInfo))
		{
			tstringstream tss;
			if ( bySex == 1)
				return pInfo->strModelMen;
			else
				return pInfo->strModelWomen;
		}
	}
	return _T("");
}

float	  HolyManMgr::GetModelScale(DWORD dwHolyManID)
{
	const tagSoulAttName* pProto = ItemProtoData::Inst()->FindShenLingName(dwHolyManID);
	if(!P_VALID(pProto))
		return FALSE;

	return pProto->dwScal/100.f;
}

VOID HolyManMgr::ShowErrorTips(DWORD dwErr)
{
	INT nErr = (INT)dwErr;
	tstringstream tss;
	tss<<_T("HolyMan_Tips")<<nErr;
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()]);

}

DWORD HolyManMgr::OnNS_SoulEquip( tagNS_SoulEquip *pNetCmd, DWORD )
{
	DWORD dwErr = pNetCmd->dwErrorCode;
	ShowErrorTips(dwErr);
	m_bLoginInit = FALSE;
	return 0;
}

DWORD HolyManMgr::OnNS_UnSoulEquip( tagNS_UnSoulEquip *pNetCmd, DWORD )
{
	DWORD dwErr = pNetCmd->dwErrorCode;
	ShowErrorTips(dwErr);
	m_bLoginInit = FALSE;
	return 0;
}

DWORD HolyManMgr::OnNS_GetSoulInfo( tagNS_GetSoulInfo *pNetCmd, DWORD )
{
	SetHolyManAtt(pNetCmd->HolySoulInfo);

	if (!m_bLoginInit) //上线初始化不刷新经验计算
		m_bIsUpdateExp = TRUE;
	else
		m_bLoginInit = FALSE;

	UpdateSoulAttValue();

	ESoulAttOptType eOpt = ESAOType_CancelCall;
	if ( IsHolyAwake() )
		eOpt = ESAOType_Call;

	RoleMgr::Inst()->ReloadHolyManModel( RoleMgr::Inst()->GetLocalPlayerID() , (m_SoulAttInfo.SoulAttEx).dwSoulID, eOpt );

	return 0;
}

DWORD HolyManMgr::OnNS_ActivateHolySoul( tagNS_ActivateHolySoul *pNetCmd, DWORD )
{
	m_bHolyManActive = TRUE;
	ShowErrorTips(20);
	return 0;
}

DWORD HolyManMgr::OnNS_HolySoulAttChange( tagNS_HolySoulAttChange *pNetCmd, DWORD )
{	
	//元神默契值
	if ( EHEAT_Covalue == pNetCmd->eHolyEquipAttType ) 
	{
		m_SoulAttInfo.SoulAttEx.nCurCovalue = pNetCmd->dwVal;
		UpdateSoulAttValue(EHEAT_Covalue);
	}
	//元神值
	else if ( EHEAT_SoulValue == pNetCmd->eHolyEquipAttType )
	{
		m_SoulAttInfo.SoulAttEx.nCurSoulValue = pNetCmd->dwVal;
		UpdateSoulAttValue(EHEAT_SoulValue);
	}

	return 0;
}

VOID HolyManMgr::ClearHolyManInfoMap()
{
	std::map<DWORD,const tagHolyModelInfo*>::iterator it = m_mapHolyManInfo.begin();
	for ( ; it != m_mapHolyManInfo.end(); ++it )
		SAFE_DEL(it->second);

	m_mapHolyManInfo.clear();
}

VOID HolyManMgr::ClearHolyComposeMap()
{
	std::map<DWORD,const tagStarEnhanceProto*>::iterator it = m_mapHolyCompose.begin();
	for ( ; it != m_mapHolyCompose.end(); ++it )
		SAFE_DEL(it->second);

	m_mapHolyCompose.clear();
}

VOID HolyManMgr::SetHolyManAtt(const tagHolySoulInfo info )
{
	memcpy(&m_SoulAttInfo, (BYTE*)(&info) , sizeof(tagHolySoulInfo));
}

//获取元神值
INT HolyManMgr::GetHolyManValue()
{
	INT nValue = (m_SoulAttInfo.SoulAttEx).nCurSoulValue;
	if (nValue>0)
		return nValue;
	return 0;
}
//打开元神信息界面
VOID HolyManMgr::OpenHolyManFrame(BOOL bOpen/* = TRUE*/)
{
	HolyManFrame* pHolyMan = NULL;
	if ( P_VALID(m_pFrameMgr->GetFrame(_T("HolyManFrame"))) )
		pHolyMan =  (HolyManFrame*)m_pFrameMgr->GetFrame(_T("HolyManFrame"));
	else
		pHolyMan = (HolyManFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("HolyManFrame"), _T("HolyManFrame"), 0);
	if(P_VALID(pHolyMan))
	{
		if (pHolyMan->IsShowing())
			pHolyMan->ShowWnd(FALSE);
		else if ( bOpen )
			pHolyMan->ShowWnd(TRUE);	
	}
}

//元神装备强化
const tagStarEnhanceProto*	HolyManMgr::GetHolyComposeInfo(BYTE byLv)
{
	std::map<DWORD,const tagStarEnhanceProto*>::iterator it = m_mapHolyCompose.begin();
	for ( ; it != m_mapHolyCompose.end(); ++it )
	{
		const tagStarEnhanceProto* pProto = it->second;
		if ( P_VALID(pProto) && byLv==pProto->byID )
			return it->second;
	}
	return NULL;
}

INT	HolyManMgr::GetStarEnhanceAttValue(BYTE byLv, EHolyEquipAttType eAttType)
{
	std::map<DWORD,const tagStarEnhanceProto*>::iterator it = m_mapHolyCompose.begin();
	for ( ; it != m_mapHolyCompose.end(); ++it )
	{
		const tagStarEnhanceProto* pProto = it->second;
		if ( P_VALID(pProto) && byLv==pProto->byID )
		{
			switch (eAttType)
			{
			case EHEAT_Demage:		return pProto->nStunt;
			case EHEAT_Hit:				return pProto->nHitRate;
			case EHEAT_Crit:				return pProto->nCritRate;
			case EHEAT_CritAmount:	return pProto->nCritaMount;
			case EHEAT_Exattack:		return pProto->nExAttack;
			case EHEAT_InExattack:		return pProto->nInAttack;
			case EHEAT_Accurate:		return pProto->nAccuRate;
			case EHEAT_Exdamage:	return pProto->nExDamage;
			case EHEAT_Toughness:	return pProto->nNegToughness;
			case EHEAT_Morale:			return pProto->nMorale;
			}
		}
	}
	return 0;
}

//通过转生道具(老君仙露)ID 获得转生概率 
INT  HolyManMgr::GetRateByItemNum(INT nNum)
{
// 	std::map<INT,  tagHolySoulReborn>::iterator it = m_mapHolyRebornRate.find(nNum);
// 	if ( it != m_mapHolyRebornRate.end() ) 
// 	{
// 		INT nRebornLv = (m_SoulAttInfo.SoulAtt.nSoulAtt[ESAT_RebornLv])/10000 ;
// 		tagHolySoulReborn dataRate	= it->second;
// 		switch (nRebornLv)
// 		{
// 		case 1: return	dataRate.nRate1;
// 		case 2:	return	dataRate.nRate2;
// 		}
// 	}
	return 0;
}

INT  HolyManMgr::GetFullRateMinNum()
{
// 	INT nRebornLv = (m_SoulAttInfo.SoulAtt.nSoulAtt[ESAT_RebornLv])/10000 ;
// 	std::map<INT,  tagHolySoulReborn>::iterator it = m_mapHolyRebornRate.begin();
// 	for ( ; it != m_mapHolyRebornRate.end() ; ++it )
// 	{
// 		tagHolySoulReborn dataRate	= it->second;
// 		if ( 1 == nRebornLv && 100 <= dataRate.nRate1 )
// 			return it->first;
// 		else if ( 2 == nRebornLv && 100 <= dataRate.nRate2 )
// 			return it->first;
// 	}
	return 0;
}

//获得转生等级
INT HolyManMgr::GetRebornLevel()
{
// 	INT nRebornLv = (m_SoulAttInfo.SoulAtt.nSoulAtt[ESAT_RebornLv])/10000 ;
// 	return nRebornLv;
	return 0;
}

//获得转生等级
INT HolyManMgr::GetHolyManLevel()
{
// 	INT nLv = m_SoulAttInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel] ;
// 	return nLv;
	return 0;
}

VOID HolyManMgr::UpdateItem(Item **pItem)
{
	if(P_VALID(*pItem))
	{
		INT64 n64ID = (*pItem)->GetData()->n64Serial;
		Item *pTempItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if(!P_VALID(pTempItem))
			(*pItem) = NULL;
	}
}

BOOL HolyManMgr::IsHolyAwakeSkillID( DWORD dwID)
{
	if (	HolyMan_SkillID1 == dwID/* ||
									HolyMan_SkillID2 == dwID ||
									HolyMan_SkillID3 == dwID*/	)
									return TRUE;
	return FALSE;
}

DWORD HolyManMgr::OnNS_UpdateHolySoulExp(tagNS_UpdateHolySoulExp* pMsg, DWORD pPrama)
{
	tagSoulAttEx soulAttEx = m_SoulAttInfo.SoulAttEx;
	if ( pMsg->n64Exp < soulAttEx.n64MaxExp )
		(m_SoulAttInfo.SoulAttEx).n64LevelExp =pMsg->n64Exp;
	UpdateSoulAttValue();
	return 0;
}

//开凿孔位消耗
INT HolyManMgr::GetChiselCost(BYTE index)
{
	std::map<BYTE, tagHolySoulEquipHole>::iterator it = m_mapHolyChisel.find(index);
	if ( it != m_mapHolyChisel.end() )
		return (it->second).nCostMoney;
	return 0;
}

//开凿孔位几率
INT HolyManMgr::GetChiselRate(BYTE index)
{
	std::map<BYTE, tagHolySoulEquipHole>::iterator it = m_mapHolyChisel.find(index);
	if ( it != m_mapHolyChisel.end() )
		return (it->second).nRate;
	return 0;

}

//镶嵌孔位消耗
INT HolyManMgr::GetInlayCost(BYTE index)
{
	std::map<BYTE, tagHolySoulEquipInlay>::iterator it = m_mapHolyInlay.find(index);
	if ( it != m_mapHolyInlay.end() )
		return (it->second).nCostMoney;
	return 0;
}

//镶嵌孔位几率
INT HolyManMgr::GetInlayRate(BYTE index)
{
	std::map<BYTE, tagHolySoulEquipInlay>::iterator it = m_mapHolyInlay.find(index);
	if ( it != m_mapHolyInlay.end() )
		return (it->second).nRate;
	return 0;
}