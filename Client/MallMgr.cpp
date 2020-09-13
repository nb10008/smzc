#include "stdafx.h"
#include "MallMgr.h"
#include "MallFrame.h"
#include "CombatSysUtil.h"
#include "ItemProtoData.h"
#include "MallGiftFrame.h"
#include "YuanbaoFrame.h"
#include "GroupBuyFrame.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\creature_define.h"
#include "..\WorldDefine\AvatarAtt.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\role_att.h"
#include "EasyBuyFrame.h"
#include "ToolTipFrame.h"
#include "QuestEvent.h"
#include "CreatureData.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "LevelPromoteFrame.h"
#include "ClientConfigInCpk.h"
#include "SaveAccount.h"

const TCHAR* szProtoConfigPath = _T("data\\local\\zhCN\\attdata\\recommending_item.xml");
const INT MAX_PLAYER_LEVEL = 255;
struct mallPackCmp
{
	bool operator()( const tagMallPackProto *pPack1, const tagMallPackProto *pPack2 )
	{
		return pPack1->byDummy[0] > pPack2->byDummy[0]; 
	}
};

MallMgr::MallMgr( void ):m_Trunk(this)
{
	m_pFrameMgr				= NULL;
	m_pUtil					= NULL;
	m_pCmdMgr				= NULL;
	m_pSession				= NULL;
	m_pMallFrame			= NULL;
    m_pYuanbaoFrame			= NULL;
	m_dwFreeGetTime			= GT_INVALID;
	m_pGroupBuyFrame		= NULL;
	m_dwGroupBuyTime		= GT_INVALID;
	m_bOpen					= FALSE;
	m_dwMallTime			= 0;
	m_pPromoteItemShow     =NULL;
	m_nMaxPromotePage		= 1;

	m_pLevelPromote			= NULL;
	m_bCanUseBag			= TRUE;
	ClearGiftPlanMap();
}

MallMgr::~MallMgr( void )
{

}

MallMgr g_MallMgr;
MallMgr* MallMgr::Inst()
{
	return &g_MallMgr;
}

static BYTE * s_byMallOfferProto = NULL;
static BOOL bMallOpenFirst = TRUE;

VOID MallMgr::Init()
{
	m_pUtil = TObjRef<Util>();
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();

	//--注册游戏事件
	m_pFrameMgr->RegisterEventHandle(_T("Open_Mall"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_Mall));
	m_pFrameMgr->RegisterEventHandle(_T("Close_Mall"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnClose_Mall));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Task"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_TaskEasyBuyFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Package"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_PackageEasyBuyFrame));
    m_pFrameMgr->RegisterEventHandle(_T("Open_Sociality"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_SocialEasyBuyFrame));
	//m_pFrameMgr->RegisterEventHandle(_T("Open_Ware"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_WareEasyBuyFrame));
	m_pFrameMgr->RegisterEventHandle(_T("tagStartNPCTalkEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_ComposeEasyBuyFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Pet"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_PetEasyBuyFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Dower"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_RoleSkillEasyBuyFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Role"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_RoleInfoEasyBuyFrame));
	m_pFrameMgr->RegisterEventHandle(_T("EnterWar"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnEventEnterWar));
	m_pFrameMgr->RegisterEventHandle(_T("EnterNormalMap"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnEnterNormalMap));

	//--注册网络消息
	m_pCmdMgr->Register("NS_OpenMall",			(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_OpenMall),		_T("NS_OpenMall"));
	m_pCmdMgr->Register("NS_MallItem",			(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallItem),		_T("NS_MallItem"));
	m_pCmdMgr->Register("NS_MallFreeItem",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallFreeItem),	_T("NS_MallFreeItem"));
	m_pCmdMgr->Register("NS_MallPack",			(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallPack),		_T("NS_MallPack"));
	m_pCmdMgr->Register("NS_MallUpdateItem",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallUpdateItem),_T("NS_MallUpdateItem"));
	m_pCmdMgr->Register("NS_MallUpdatePack",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallUpdatePack),_T("NS_MallUpdatePack"));
	m_pCmdMgr->Register("NS_MallUpdate",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallUpdate),	_T("NS_MallUpdate"));
	m_pCmdMgr->Register("NS_GetSimGPInfo",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_GetSimGPInfo),	_T("NS_GetSimGPInfo"));
	//m_pCmdMgr->Register("NS_WareOpen",			(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_OpenWareEasyBuyFrame), _T("NS_WareOpen") );
	m_pCmdMgr->Register("NS_MallRecommendingItem",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_GetPromoteData),	_T("NS_MallRecommendingItem"));
	m_pCmdMgr->Register("NS_SyncMallOfferProto",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_SyncMallOfferProto),	_T("NS_SyncMallOfferProto"));

	if(P_VALID(GetObj("EasyBuyItemInfo")))
		KillObj("EasyBuyItemInfo");
	m_pVarContainer = CreateObj("EasyBuyItemInfo", "VarContainer");
	LoadEasyBuyItem();

	m_dwMallTime			= 0; //上下线时间置0
	//LoadPromoteData(); //<! f计划推荐商品放入服务器端
	m_pMallFrame = (MallFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("Mall"), _T("MallFrame"), 0);

	ClearGiftPlanMap();
	CreateLevelPromote();
	for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		m_bLevelBtnGlint[i] = FALSE;
		m_bLevelCanBuy[i] = FALSE;
	}

	bMallOpenFirst = TRUE;
}

VOID MallMgr::Update()
{

}

VOID MallMgr::Destroy()
{
	//--注销游戏事件
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Mall"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_Mall));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_Mall"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnClose_Mall));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Task"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_TaskEasyBuyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Package"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_PackageEasyBuyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Sociality"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_SocialEasyBuyFrame));
	//m_pFrameMgr->UnRegisterEventHandler(_T("Open_Ware"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_WareEasyBuyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagStartNPCTalkEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_ComposeEasyBuyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Pet"),       (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_PetEasyBuyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Dower"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_RoleSkillEasyBuyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Role"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnOpen_RoleInfoEasyBuyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("EnterWar"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnEventEnterWar));
	m_pFrameMgr->UnRegisterEventHandler(_T("EnterNormalMap"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MallMgr::OnEnterNormalMap));


	//--注销网络消息
	m_pCmdMgr->UnRegister("NS_OpenMall",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_OpenMall));
	m_pCmdMgr->UnRegister("NS_MallItem",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallItem));
	m_pCmdMgr->UnRegister("NS_MallFreeItem",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallFreeItem));
	m_pCmdMgr->UnRegister("NS_MallPack",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallPack));
	m_pCmdMgr->UnRegister("NS_MallUpdateItem",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallUpdateItem));
	m_pCmdMgr->UnRegister("NS_MallUpdatePack",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallUpdatePack));
	m_pCmdMgr->UnRegister("NS_MallUpdate",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_MallUpdate));
	m_pCmdMgr->UnRegister("NS_GetSimGPInfo",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_GetSimGPInfo));
	//m_pCmdMgr->UnRegister("NS_WareOpen",		(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_OpenWareEasyBuyFrame) );
	m_pCmdMgr->UnRegister("NS_MallRecommendingItem",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_GetPromoteData));
	m_pCmdMgr->UnRegister("NS_SyncMallOfferProto",	(NETMSGPROC)m_Trunk.sfp2(&MallMgr::OnNS_SyncMallOfferProto));
	
	m_dwMallTime			= 0;
	m_pMallFrame = NULL;
	ClearMallItem();
	ClearMallPack();
	m_vecUIItem.clear();
	ClearGroupBuyInfo();
	ClearGiftPlanMap();
	
	if(P_VALID(GetObj("EasyBuyItemInfo")))
		KillObj("EasyBuyItemInfo");

	map<INT, tagMallItemPrototeGood>::iterator it=m_mapMallPromoteItem.begin();
	while(it!=m_mapMallPromoteItem.end())
	{
		it = m_mapMallPromoteItem.erase(it);
	}
	m_mapMallPromoteItem.clear();

	SAFE_DELETE(m_pPromoteItemShow);
	map<DWORD, tagMallPromoteItemShow>::iterator it2=m_mapMallPromoteItemShow.begin();
	while(it2!=m_mapMallPromoteItemShow.end())
	{
		it2 = m_mapMallPromoteItemShow.erase(it2);
	}
	m_mapMallPromoteItemShow.clear();

	SAFE_DELETEA(s_byMallOfferProto);
}

void MallMgr::ClearMallItem()
{
	map<DWORD, tagMallItemProto>::iterator it=m_mapMallItemEx.begin();
	while(it!=m_mapMallItemEx.end())
	{
		it = m_mapMallItemEx.erase(it);
	}
	m_mapMallItemEx.clear();
	m_mapMallItem.Clear();
	m_listItem.clear();
}

void MallMgr::ClearMallPack()
{
	map<DWORD, tagMallPackProto>::iterator it=m_mapPackItemEx.begin();
	while(it!=m_mapPackItemEx.end())
	{
		it = m_mapPackItemEx.erase(it);
	}
	m_mapPackItemEx.clear();
	m_mapPackItem.Clear();
	m_listPack.clear();
}

void MallMgr::ClearGroupBuyInfo()
{
	map<INT64, tagGroupPurchase>::iterator it = m_mapGgroupPur.begin();
	while( it!=m_mapGgroupPur.end() )
	{
		it = m_mapGgroupPur.erase(it);
	}
	m_mapGgroupPur.clear();
}

// 打开商城界面
DWORD MallMgr::OnOpen_Mall(tagGameEvent* pEvent)
{
	if (!ClientConfigInCpk::Inst()->IsMallOpen())
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CPK_CFG_mall_close")]);
		return 0;
	}
	if( P_VALID(m_pMallFrame) )
	{
		if( m_mapMallItemEx.size()!=0
			|| m_mapPackItemEx.size()!=0)
		{
			tagNC_MallUpdate msg;
			msg.dwMallTime = m_dwMallTime;
			m_pSession->Send(&msg);
		}
		else
		{
			tagNC_MallGet msg;
			m_pSession->Send(&msg);
		}
		m_bOpen = TRUE;

		if( bMallOpenFirst )
		{
			bMallOpenFirst = FALSE;
			InitMallOfferProto((tagNS_SyncMallOfferProto*)s_byMallOfferProto,0);
		}
	}

	return 0;
}

// 关闭商城界面
DWORD MallMgr::OnClose_Mall(tagGameEvent* pEvent)
{
	if( P_VALID(m_pMallFrame) )
	{
		m_pMallFrame->ShowWnd(FALSE);
		m_bOpen	 = FALSE;
		tagGameEvent event(_T("Close_Mall_UI"), NULL);
		m_pFrameMgr->SendEvent(&event);
		tagGameEvent eventGift(_T("Close_Gift_UI"), NULL);
		m_pFrameMgr->SendEvent(&eventGift);
	}
	CreateLevelPromote();
	return 0;
}

tagMallItemProto* MallMgr::GetMallItemByID( DWORD dwID )
{
	return m_mapMallItem.Peek(dwID);
}

tagMallPackProto* MallMgr::GetMallPackByID(DWORD dwID)
{
	return m_mapPackItem.Peek(dwID);
}

tagMallItemProto* MallMgr::GetMallItemByTypeid( DWORD dwTypeID )
{
	if( !m_listItem.empty() )
	{
		list<tagMallItemProto*>::iterator it;
		for( it = m_listItem.begin(); it != m_listItem.end(); it++ )
		{
			if( (*it)->dwTypeID == dwTypeID )
			{
				return *it;
			}
		}
	}
	return NULL;
}

DWORD MallMgr::OnNS_OpenMall( tagNS_OpenMall* pMsg, DWORD dwParam )
{
	if(pMsg->dwErrorCode == E_Success)
	{
		if( !P_VALID(m_pMallFrame) )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(_T("mall create fail!\n"));
			return 0;
		}

		m_dwMallTime = pMsg->dwMallTime;
		m_pMallFrame->ClassifiedMallItem();
		m_pMallFrame->UpdateAvatar();
		m_pMallFrame->ShowWnd(m_bOpen);

	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(_T("mall open fail!\n"));
	}
	return 0;
}

DWORD MallMgr::OnNS_MallItem( tagNS_MallItem* pMsg, DWORD dwParam )
{
	ClearMallItem();
	tagMallItemProto* pItem = (tagMallItemProto*)pMsg->byData;
	for(int i=0; i<pMsg->nItemNum; i++)
	{
		const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(pItem[i].dwTypeID);
		if( P_VALID(pItemProto) )
		{
			m_mapMallItemEx.insert(make_pair(pItem[i].dwID, pItem[i]));
		}
	}
	map<DWORD, tagMallItemProto>::iterator it = m_mapMallItemEx.begin();
	for( ; it!=m_mapMallItemEx.end(); ++it )
	{
		m_mapMallItem.Add(it->first, &(it->second));
		if( it->second.byRank!=0 && it->second.byRank<MAX_PromoteItem+1 )
		{
			m_pProItem[it->second.byRank-1] = &(it->second);
		}
	}
	m_mapMallItem.ExportAllValue( m_listItem );
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("UpdateMallDataEvent"), NULL));
	return 0;
}

DWORD MallMgr::OnNS_MallFreeItem( tagNS_MallFreeItem* pMsg, DWORD dwParam )
{
	m_MallFreeItem = pMsg->freeItem;
	return 0;
}

DWORD MallMgr::OnNS_MallPack( tagNS_MallPack* pMsg, DWORD dwParam )
{
	ClearMallPack();
	tagMallPackProto* pPack = (tagMallPackProto*)pMsg->byData;
	for(int i=0; i<pMsg->nPackNum; i++)
	{
		m_mapPackItemEx.insert(make_pair(pPack[i].dwID, pPack[i]));
	}
	map<DWORD, tagMallPackProto>::iterator it = m_mapPackItemEx.begin();
	for( ; it!=m_mapPackItemEx.end(); ++it )
	{
		m_mapPackItem.Add(it->first, &(it->second));
	}
	m_mapPackItem.ExportAllValue( m_listPack );
	m_listPack.sort( mallPackCmp() );
	return 0;
}

DWORD MallMgr::OnNS_MallUpdate( tagNS_MallUpdate* pMsg, DWORD dwParam )
{
	m_dwMallTime = pMsg->dwMallTime;
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("UpdateMallDataEvent"), NULL));
	return 0;
}

DWORD MallMgr::OnNS_MallUpdateItem( tagNS_MallUpdateItem* pMsg, DWORD dwParam )
{
	tagMallUpdate* pUdateItem = (tagMallUpdate*)pMsg->byData;
	for(int i=0; i<pMsg->nItemNum; i++)
	{
		map<DWORD, tagMallItemProto>::iterator it = m_mapMallItemEx.find(pUdateItem[i].dwID);
		if( it!=m_mapMallItemEx.end() )
		{
			it->second.nNum = pUdateItem[i].byRemainNum;
		}
	}
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("UpdateMallDataEvent"), NULL));
	return 0;
}

DWORD MallMgr::OnNS_MallUpdatePack( tagNS_MallUpdatePack* pMsg, DWORD dwParam )
{
	tagMallUpdate* pUdatePack = (tagMallUpdate*)pMsg->byData;
	for(int i=0; i<pMsg->nItemNum; i++)
	{
		map<DWORD, tagMallPackProto>::iterator it = m_mapPackItemEx.find(pUdatePack[i].dwID);
		if( it!=m_mapPackItemEx.end() )
		{
			it->second.nNum = pUdatePack[i].byRemainNum;
		}
	}
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("UpdateMallDataEvent"), NULL));
	return 0;
}

tagMallItemProto* MallMgr::GetMallProItem(int nIndex)
{
	if( nIndex>=0 && nIndex<MAX_PromoteItem )
		return m_pProItem[nIndex];
	else
		return NULL;
}

void MallMgr::LoadPromoteData()
{
	LPCTSTR szPromoteProto = szProtoConfigPath;
	list<tstring> PromoteProtoFieldList;
	list<tstring>::iterator iter;
	//---------------------------------------------------------------------------------
	//读取recommending_item
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarPromoteProto = CreateObj("PromoteProto", "VarContainer");
	if(!VarPromoteProto->Load("VFS_System", szPromoteProto, "Id", &PromoteProtoFieldList))	
	{
		IMSG(_T("Load file promote_proto.xml failed\r\n"));
		KillObj("PromoteProto");
		return ;
	}
	for(iter = PromoteProtoFieldList.begin(); iter != PromoteProtoFieldList.end(); ++iter)
	{
		tagMallItemPrototeGood  promote;
		promote.id       =  (INT)VarPromoteProto->GetDword(_T("Id"),	    (*iter).c_str(), 0 );
		promote.iMinLevel =	(INT)VarPromoteProto->GetDword( _T("MinLevel"),	(*iter).c_str(), 0 );
		promote.iMaxLevel =	(INT)VarPromoteProto->GetDword( _T("MaxLevel"),	(*iter).c_str(), 0 );

		for (int i=0;i<MAX_PromoteItemNum;i++)
		{
			tstringstream sstemp;
			sstemp<<_T("Item")<<i+1;
			//LPCTSTR  strTemp = sstemp.str().c_str();
			promote.dwItemID[i] = (INT)VarPromoteProto->GetDword( sstemp.str().c_str(),	(*iter).c_str(), 0 );
		}
		m_mapMallPromoteItem.insert(make_pair(promote.id, promote));
	}
	KillObj("PromoteProto");
}

VOID MallMgr::PromoteItemShowMgr()
{
	//匹配人物等级,获得对应等级的商品ItemID
	INT nRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
 
	map<INT, tagMallItemPrototeGood> mapPromote = MallMgr::Inst()->GetPromoteItem();
	if (mapPromote.empty())
	{
		IMSG(_T("Map is empty! "));
		return;
	}
	map<INT, tagMallItemPrototeGood>::iterator iter = mapPromote.begin(); 
	for (;iter!=mapPromote.end();++iter)
	{
		//得到初次小于的最大登记值，即得到等级所属范围
		INT maxLevel = iter->second.iMaxLevel; 
		if ( nRoleLevel <= maxLevel )
		{
			break;
		}
	}
	const tagMallItemPrototeGood pForLevelPromoteItem = iter->second; //记录下对应位置

	INT page = 1;
	INT num = 0;
	TMap<DWORD, tagMallItemProto*> mapAll = MallMgr::Inst()->GetMallProto();
	TMap<DWORD, tagMallItemProto*>::TMapIterator it = mapAll.Begin();
	tagMallItemProto* pProto=NULL;

	if (!m_mapMallPromoteItemShow.empty())
	{
		map<DWORD, tagMallPromoteItemShow>::iterator it2=m_mapMallPromoteItemShow.begin();
		while(it2!=m_mapMallPromoteItemShow.end())
		{
			it2 = m_mapMallPromoteItemShow.erase(it2);
		}
		m_mapMallPromoteItemShow.clear();
	}

	while (it != mapAll.End())
	{
		mapAll.PeekNext(it,pProto);
		if( !P_VALID(pProto)) 
		{
			continue;
		}
		DWORD  dwProItemID = pProto->dwTypeID;//->dwID;
		for (int i = 0;  i < MAX_PromoteItemNum ; i++)
		{
			//记录需要显示的ItemID的页数、个数和对应Item
			if ( dwProItemID == pForLevelPromoteItem.dwItemID[i] )
			{
				map<DWORD, tagMallPromoteItemShow>::iterator iterIfIn = m_mapMallPromoteItemShow.find(dwProItemID);
				if ( iterIfIn!=m_mapMallPromoteItemShow.end() ) //说明在列表中已经有同类物品了
				{
					break;
				}
				//超过一页最大显示量则翻页
				if ( num >= MALL_MAX_PROMOTE_ITEM_PerPage )
				{
					page++;
					num = 1;
				}
				else
				{
					num++;
				}
				tagMallPromoteItemShow  promoteItemShow;
				promoteItemShow.iNum = num;
				promoteItemShow.iPage = page;
				promoteItemShow.dwProItemID = dwProItemID;
				m_mapMallPromoteItemShow.insert(make_pair(dwProItemID,promoteItemShow));
				if (m_nMaxPromotePage<page) //记录下最大页数
				{
					m_nMaxPromotePage = page;
				}
			}
		}
	}
}

void MallMgr::CreateMallGiftFrame()
{
	MallGiftFrame *pFrame = (MallGiftFrame*)m_pFrameMgr->GetFrame(_T("MallGift"));
	if( !P_VALID(pFrame) )
	{
		pFrame = (MallGiftFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("MallGift"), _T("MallGiftFrame"), 0);
		if( P_VALID(pFrame) )
			pFrame->SetBuyNum();
	}
	else 
	{
		pFrame->ShowGiftFrame(TRUE);
	}
}

void MallMgr::SetMallGiftFrameMaxBuyNum(INT nMaxBuyNum)
{
	MallGiftFrame *pFrame = (MallGiftFrame*)m_pFrameMgr->GetFrame(_T("MallGift"));
	if( P_VALID(pFrame) )
	{
		pFrame->SetMaxBuyNum(nMaxBuyNum);
	}
}

void MallMgr::InitMallGiftFrameNum()
{
	MallGiftFrame *pFrame = (MallGiftFrame*)m_pFrameMgr->GetFrame(_T("MallGift"));
	if( P_VALID(pFrame) )
	{
		pFrame->SetBuyNum();
	}
}

INT16 MallMgr::GetEquipDisplayPos(INT16 n16EquipPos)
{
	// 饰品不需要显示
	if(EEP_Finger1 == n16EquipPos || EEP_Finger2 == n16EquipPos
		|| EEP_Waist == n16EquipPos || EEP_Neck == n16EquipPos)
	{
		return GT_INVALID;
	}

	// 转换为显示位置
	switch(n16EquipPos)
	{
		// 武器
	case EEP_RightHand:
		return EAE_RWeapon;
		break;
	case EEP_LeftHand:
		return EAE_LWeapon;
		break;

		// 时装
	case EEP_FashionHead:
		return EAE_Head;
		break;
// 	case EEP_FashionFace:
// 		return EAE_Face;
// 		break;
	case EEP_FashionBody:
		return EAE_Upper;
		break;
// 	case EEP_FashionLegs:
// 		return EAE_Lower;
// 		break;
	case EEP_FashionBack:
		return EAE_Back;
		break;
	case EEP_FashionWrist:
		return EAE_Glove;
		break;
	case EEP_FashionFeet:
		return EAE_Boot;
		break;

		// 防具
	case EEP_Head:
		return EAE_Head;
		break;
// 	case EEP_Face:
// 		return EAE_Face;
// 		break;
	case EEP_Body:
		return EAE_Upper;
		break;
	case EEP_Legs:
		return EAE_Lower;
		break;
	case EEP_Back:
		return EAE_Back;
		break;
	case EEP_Wrist:
		return EAE_Glove;
		break;
	case EEP_Feet:
		return EAE_Boot;
		break;
	}
	return GT_INVALID;
}

void MallMgr::CreateYuanBaoFrame(LPCTSTR szFatherName)
{
    YuanbaoFrame* pFrame = (YuanbaoFrame*)m_pFrameMgr->GetFrame(_T("YuanbaoFrame"));
    if (!P_VALID(pFrame))
    {
        pFrame = (YuanbaoFrame*)m_pFrameMgr->CreateFrame(szFatherName, _T("YuanbaoFrame"), _T("YuanbaoFrame"), 0);
        if (P_VALID(pFrame))
            m_pYuanbaoFrame = pFrame;
        else
            IMSG(_T("Create YuanbaoFrame failed"));
    }
    else
    {
        m_pYuanbaoFrame = pFrame;
    }
}

void MallMgr::CreateGroupBuyFrame( LPCTSTR szFatherName )
{
	GroupBuyFrame* pFrame = (GroupBuyFrame*)m_pFrameMgr->GetFrame(_T("GroupBuy"));
	if (!P_VALID(pFrame))
	{
		pFrame = (GroupBuyFrame*)m_pFrameMgr->CreateFrame(szFatherName, _T("GroupBuy"), _T("GroupBuyFrame"), 0);
		if (P_VALID(pFrame))
			m_pGroupBuyFrame = pFrame;
		else
			IMSG(_T("Create GroupBuyFrame failed"));
	}
	else
	{
		m_pGroupBuyFrame = pFrame;
	}
}

void MallMgr::SendGetGroupBuyInfo()
{
	tagNC_GetSimGPInfo msg;
	m_pSession->Send(&msg);
}

DWORD MallMgr::OnNS_GetSimGPInfo( tagNS_GetSimGPInfo* pMsg, DWORD dwParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			m_nGroupPurExp = pMsg->nGroupPurchase;
			INT nNum = pMsg->nGPInfoNum;
			for( int i=0; i<nNum; ++i )
			{	
				tagSimGPInfo* pInfo = &pMsg->simData[i];
				if( (INT)(pInfo->dwRemainTime)<=0 || pInfo->dwRemainTime==GT_INVALID )
					continue;
				tagGroupPurchase gp;
				gp.dwGuildID			= pInfo->dwGuildID;	
				gp.dwRoleID				= pInfo->dwRoleID;
				gp.dwMallID				= pInfo->dwMallID;
				gp.nPrice				= pInfo->nPrice;
				gp.nParticipatorNum		= pInfo->nParticipatorNum;
				gp.nRequireNum			= pInfo->nRequireNum;
				gp.dwRemainTime			= pInfo->dwRemainTime;
				INT64 n64temp = gp.dwRoleID;
				INT64 n64Key = (n64temp << 32) | gp.dwMallID;
				m_mapGgroupPur.insert(make_pair(n64Key, gp));				
			}
			if( P_VALID(m_pGroupBuyFrame) )
			{
				m_pGroupBuyFrame->RefreshGroupBuyInfo();
			}
		}
		break;
	}
	return 0;
}

DWORD MallMgr::OnNS_GetPromoteData(tagNS_MallRecommendingItem* pMsg, DWORD dwParam)
{
	INT nInfoNum = pMsg->nItemNum;
	tagRecommendingItemInfo *pRecInfo = (tagRecommendingItemInfo*)pMsg->byData;
	for ( INT i=0; i<nInfoNum; ++i)
	{
		tagMallItemPrototeGood  promote;
		promote.id			= pRecInfo[i].dwID;
		promote.iMinLevel   = pRecInfo[i].n16MinLevel;
		if ( i==nInfoNum-1 )
			promote.iMaxLevel   = MAX_PLAYER_LEVEL;
		else
			promote.iMaxLevel   = pRecInfo[i].n16MaxLevel;

		promote.dwItemID[0] = pRecInfo[i].dwItem1TypeID;
		promote.dwItemID[1] = pRecInfo[i].dwItem2TypeID;
		promote.dwItemID[2] = pRecInfo[i].dwItem3TypeID;
		promote.dwItemID[3] = pRecInfo[i].dwItem4TypeID;
		promote.dwItemID[4] = pRecInfo[i].dwItem5TypeID;
		promote.dwItemID[5] = pRecInfo[i].dwItem6TypeID;
		promote.dwItemID[6] = pRecInfo[i].dwItem7TypeID;
		promote.dwItemID[7] = pRecInfo[i].dwItem8TypeID;
		promote.dwItemID[8] = pRecInfo[i].dwItem9TypeID;
		promote.dwItemID[9] = pRecInfo[i].dwItem10TypeID;

		m_mapMallPromoteItem.insert(make_pair(promote.id, promote));
	}
	return 0;
}

tagGroupPurchase* MallMgr::GetGroupBuyInfoByID( INT64 dwID )
{
	map<INT64, tagGroupPurchase>::iterator it =m_mapGgroupPur.find(dwID);
	
	if( it!=m_mapGgroupPur.end() )
	{
		return &(it->second);
	}

	return NULL;
}

void MallMgr::DeleGroupBuyInfoByID(INT64 dwID)
{
	map<INT64, tagGroupPurchase>::iterator it =m_mapGgroupPur.find(dwID);

	if( it!=m_mapGgroupPur.end() )
	{
		m_mapGgroupPur.erase(it);
	}
}

void MallMgr::RefreshGroupBuyInfoByID( INT64 dwID, INT nNum )
{
	map<INT64, tagGroupPurchase>::iterator it =m_mapGgroupPur.find(dwID);

	if( it!=m_mapGgroupPur.end() )
	{
		it->second.nParticipatorNum = nNum;
	}
}

void MallMgr::LoadEasyBuyItem()
{
	const TCHAR* szEasyBuyFile = _T("data\\system\\attdata\\easy_buy_item_info.xml");
	list<tstring> EasyBuyItemList;
	m_pVarContainer->Load( "VFS_System", szEasyBuyFile, "id", &EasyBuyItemList );
	list<tstring>::iterator iter = EasyBuyItemList.begin();

	for(; iter!=EasyBuyItemList.end(); iter++)
	{
		tagUIItem EasyBuyItemInfo;
		EasyBuyItemInfo.dwTypeID = m_pVarContainer->GetDword( _T("typeid"), (*iter).c_str(), GT_INVALID );
		EasyBuyItemInfo.dwUIType = m_pVarContainer->GetDword( _T("type"), (*iter).c_str(), GT_INVALID );
		m_vecUIItem.push_back( EasyBuyItemInfo );
	}
}

VOID MallMgr::OpenEasyBuyFrame( LPCTSTR szFather, LPCTSTR szFrame, LPCSTR szAlign, EasyBuyUIType enUIType, LPCSTR alignType, LPCSTR szControlName/* = NULL*/ )
{
	if (!ClientConfigInCpk::Inst()->IsMallOpen())
		return;

	EasyBuyFrame *pFrame = (EasyBuyFrame*)TObjRef<GameFrameMgr>()->GetFrame( szFrame );
	if( !P_VALID(pFrame) )
	{
		pFrame = (EasyBuyFrame*)TObjRef<GameFrameMgr>()->CreateFrame( szFather, szFrame, _T("EasyBuyFrame"), 0 );
	}
	pFrame->ReloadUI(szAlign, alignType, szControlName);
	pFrame->Show( enUIType );
}

DWORD MallMgr::OnOpen_TaskEasyBuyFrame(tagGameEvent* pEvent)
{
	EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("QuestEasyBuyFrame"));
	if(!P_VALID(pFrame))
		OpenEasyBuyFrame( _T("World"), _T("QuestEasyBuyFrame"), "\\desktop\\task", EBUT_TaskUI, "OuterRightTop","questui" );
	return 0;
}

DWORD MallMgr::OnOpen_PackageEasyBuyFrame(tagGameEvent* pEvent)
{
	if(!m_bCanUseBag)
		return 0;

	if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleStateEx(ERSE_BagPsdPass) )
	{
		EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("PocketEasyBuyFrame"));
		if(!P_VALID(pFrame))
			OpenEasyBuyFrame( _T("World"), _T("PocketEasyBuyFrame"), "\\desktop\\bagback",EBUT_BagUI , "OuterLeftTop","pocketui" );
	}
		
	return 0;
}

DWORD MallMgr::OnOpen_SocialEasyBuyFrame(tagGameEvent* pEvent)   //好友列表的便捷购买
{
	EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("SocialEasyBuyFrame"));
	if(!P_VALID(pFrame))
		OpenEasyBuyFrame( _T("World"), _T("SocialEasyBuyFrame"), "\\desktop\\friwin",EBUT_SocialUI , "OuterLeftTop","socialui" );
	return 0;

}

/**DWORD MallMgr::OnOpen_WareEasyBuyFrame(tagGameEvent* pEvent)
{
	OpenEasyBuyFrame( _T("World"), _T("StorageEasyBuyFrame"), "\\desktop\\wh_win", EBUT_storageUI, "OuterRightTop", "storageui" );
	return 0;
}*/

/**DWORD MallMgr::OnNS_OpenWareEasyBuyFrame( tagNS_WareOpen* pMsg, DWORD )
{
	if( E_Success == pMsg->dwErrorCode )
	{
		OpenEasyBuyFrame( _T("World"), _T("StorageEasyBuyFrame"), "\\desktop\\wh_win", EBUT_storageUI, "OuterRightTop", "storageui" );
	}
	return 0;
}*/

DWORD MallMgr::OnOpen_PetEasyBuyFrame(tagGameEvent* pEvent)
{
	EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("PetEasyBuyFrame"));
	if( !P_VALID(pFrame) )
		OpenEasyBuyFrame( _T("World"), _T("PetEasyBuyFrame"), "\\desktop\\pet_win", EBUT_PetUI, "OuterLeftTop", "petui" );
	return 0;
}

DWORD MallMgr::OnOpen_RoleSkillEasyBuyFrame(tagGameEvent* pEvent)
{
	EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("RoleSkillEasyBuyFrame"));
	if(!P_VALID(pFrame) && SaveAccount::Inst()->GetLoginMode() != 11)	//韩国也不要这个....
		OpenEasyBuyFrame( _T("World"), _T("RoleSkillEasyBuyFrame"), "\\desktop\\skillwin", EBUT_SkillUI, "OuterLeftTop", "RoleSkillui" );
	return 0;
}

DWORD MallMgr::OnOpen_RoleInfoEasyBuyFrame(tagGameEvent* pEvent)
{
	EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("RoleInfoEasyBuyFrame"));
	if(!P_VALID(pFrame))
		OpenEasyBuyFrame( _T("World"), _T("RoleInfoEasyBuyFrame"), "\\desktop\\ra_win", EBUT_AttUI, "OuterRightTop", "RoleInfoui" );
	return 0;
}


DWORD MallMgr::OnOpen_ComposeEasyBuyFrame(tagStartNPCTalkEvent* pEvent)
{
	const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pEvent->typeID);
	if( pCreatureProto->eFunctionType==EFNPCT_Posy || 
		pCreatureProto->eFunctionType==EFNPCT_Engrave || 
		pCreatureProto->eFunctionType==EFNPCT_Quench )
	{
		EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame(_T("ComposeEasyBuyFrame"));
		if(!P_VALID(pFrame))
			OpenEasyBuyFrame( _T("World"), _T("ComposeEasyBuyFrame"), "\\desktop\\composewin", EBUT_ConsolidateUI, "OuterRightTop", "composeui" );
		else
			pFrame->Show(EBUT_ConsolidateUI);
	}
	return 0;
}


VOID MallMgr::ClearGiftPlanMap()
{
	VecGiftPlan::iterator iter = m_vecGiftPlan.begin();
	for ( ; iter != m_vecGiftPlan.end(); ++iter )
	{
		SAFE_DEL(*iter);
	}
	m_vecGiftPlan.clear();
}

DWORD MallMgr::InitMallOfferProto(tagNS_SyncMallOfferProto* pMsg, DWORD dwParam)
{
	INT nLpLevel = 1;
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(pLP))
	{
		nLpLevel = pLP->GetRoleLevel();
		if (!pMsg->byBuy50LvlItemFlag)
		{
			if (nLpLevel>=50)
			{
				m_bLevelBtnGlint[0] = TRUE;
			}
			m_bLevelCanBuy[0] = TRUE;
		}
		if (!pMsg->byBuy60LvlItemFlag)
		{
			if (nLpLevel>=60)
			{
				m_bLevelBtnGlint[1] = TRUE;
			}
			m_bLevelCanBuy[1] = TRUE;
		}
		if (!pMsg->byBuy70LvlItemFlag)
		{
			if (nLpLevel>=70)
			{
				m_bLevelBtnGlint[2] = TRUE;
			}
			m_bLevelCanBuy[2] = TRUE;
		}
		if (!pMsg->byBuy80LvlItemFlag)
		{
			if (nLpLevel>=80)
			{
				m_bLevelBtnGlint[3] = TRUE;
			}
			m_bLevelCanBuy[3] = TRUE;
		}
	}
	ClearGiftPlanMap();
	for (int i=0; i<(int)pMsg->dwMallOfferCnt; ++i)
	{
		tagMallOfferProto2Client *pGiftPlan = new tagMallOfferProto2Client;
		pGiftPlan->nLevel	= pMsg->MallOfferProto[i].nLevel;
		pGiftPlan->dwTypeID = pMsg->MallOfferProto[i].dwTypeID;
		m_vecGiftPlan.push_back(pGiftPlan);	

	}
	return 0;
}

DWORD MallMgr::OnNS_SyncMallOfferProto(tagNS_SyncMallOfferProto* pMsg, DWORD dwParam)
{
	if( !P_VALID(pMsg) )
		return GT_INVALID;

	if( pMsg->dwSize > 0 )
	{
		s_byMallOfferProto = new BYTE[pMsg->dwSize];
		memcpy(s_byMallOfferProto,pMsg,pMsg->dwSize);
		return 0;
	}


	return 0;
}

std::vector<DWORD>& MallMgr::GetGiftPlanList(INT level)
{
	m_vecGiftPlanByLevel.clear();
	VecGiftPlan::iterator iter = m_vecGiftPlan.begin();
	for ( ; iter!=m_vecGiftPlan.end(); ++iter )
	{
		DWORD dwItemID	= (*iter)->dwTypeID;
		INT   nLevel	= (*iter)->nLevel;
		if ( nLevel == level)
		{
			m_vecGiftPlanByLevel.push_back(dwItemID);
		}
	}

	return m_vecGiftPlanByLevel;

}

BOOL MallMgr::InquireIndexForClint( INT index)
{
	if ( index >= LEVEL_NUM_CHOOSE || index <0 )
	{
		return FALSE;
	}
	return m_bLevelBtnGlint[index];
}

BOOL MallMgr::InquireIndexForBuy( INT index)
{
	if ( index >= LEVEL_NUM_CHOOSE || index <0 )
	{
		return FALSE;
	}
	return m_bLevelCanBuy[index];
}

VOID MallMgr::SetIndexForClint( INT index ,BOOL b)
{
	if ( index >= LEVEL_NUM_CHOOSE || index <0 )
	{
		return ;
	}
	m_bLevelBtnGlint[index] = b;
	if (P_VALID(m_pMallFrame))
	{
		m_pMallFrame->SetGiftPlan();
	}
}

VOID MallMgr::SetIndexForBuy( INT index ,BOOL b)
{
	if ( index >= LEVEL_NUM_CHOOSE || index <0 )
	{
		return ;
	}
	m_bLevelCanBuy[index] = b;
}


BOOL MallMgr::CreateLevelPromote( )
{	
	m_pLevelPromote = (LevelPromoteFrame*) m_pFrameMgr->GetFrame(_T("LevelPromoteFrame"));
	if (!P_VALID(m_pLevelPromote))
		m_pLevelPromote = (LevelPromoteFrame*) m_pFrameMgr->CreateFrame(_T("World"), _T("LevelPromoteFrame"), _T("LevelPromoteFrame"),0);

	if (P_VALID(m_pLevelPromote))
		m_pLevelPromote->ShowWnd(FALSE);

	return TRUE;
}

DWORD MallMgr::OnEventEnterWar(tagGameEvent* pEvent)
{
	m_bCanUseBag = FALSE;

	return 0;
}

DWORD MallMgr::OnEnterNormalMap(tagGameEvent* pEvent)
{
	m_bCanUseBag = TRUE;

	return 0;
}



