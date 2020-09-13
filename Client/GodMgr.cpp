#include "StdAfx.h"
#include "GodMgr.h"
#include "CondensedFrame.h"
#include "CondensedProtoData.h"
#include "ClergyFrame.h"
#include "CombatEvent.h"
#include "..\WorldDefine\skill_define.h"
#include "SkillProtoData.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CondensedProtoData.h"
#include "ClergyProtoData.h"
#include "GodRelationFrame.h"
#include "CampaignFrame.h"
#include "..\WorldDefine\creature_define.h"
#include "CreatureData.h"
#include "QuestEvent.h"
#include "CombatSysUtil.h"

GodMgr::GodMgr(void):m_trunk(this)
{
	m_pCondensedFrame = NULL;
	m_pClergyFrame = NULL;
	m_pGodRelationFrame = NULL;
}

GodMgr::~GodMgr(void)
{
}

GodMgr g_godMgr;
GodMgr* GodMgr::Inst()
{
	return &g_godMgr;
}

void GodMgr::Init()
{
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pUtil	= TObjRef<Util>();

	//创建凝聚Frame
	m_pCondensedFrame = (CondensedFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("CondensedFrame"), _T("CondensedFrame"), 0);
	CondensedProtoData::Inst()->LoadFromFile();
	ClergyProtoData::Inst()->LoadFromFile();
	m_pClergyFrame    = (ClergyFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("ClergyFrame"),    _T("ClergyFrame"),    0);
	m_pGodRelationFrame = (GodRelationFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("GodRelationFrame"), _T("GodRelationFrame"),  0);

	//--注册网络消息处理函数
	//m_pCmdMgr->Register("",				(NETMSGPROC)m_trunk.sfp2(&GodMgr:: ),			_T(""));
	
	// 注册游戏事件
	//m_pFrameMgr->RegisterEventHandle(_T("tagUseSkillEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&GodMgr::OnUseSkillEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagStartNPCTalkEvent"),    (FRAMEEVENTPROC)m_trunk.sfp1(&GodMgr::OnStartNPCTalkEvent));
	m_pFrameMgr->RegisterEventHandle(_T("OpenCampaingFrame"), (FRAMEEVENTPROC)m_trunk.sfp1(&GodMgr::OnOpenCampaingFrame));

	m_pCmdMgr->Register("NS_UseLimitedItem", (NETMSGPROC)m_trunk.sfp2(&GodMgr::OnNS_UseLimitedItem), _T("NS_UseLimitedItem"));
	m_sGodAreaData.LoadFromFile();
}	

void GodMgr::Destroy()
{
	//--注册网络消息处理函数
	//m_pCmdMgr->UnRegister("",			(NETMSGPROC)m_trunk.sfp2(&GodMgr::));
	
	// 注销游戏事件
	//m_pFrameMgr->UnRegisterEventHandler(_T("tagUseSkillEvent"), (FRAMEEVENTPROC)m_trunk.sfp1(&GodMgr::OnUseSkillEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagStartNPCTalkEvent"),		(FRAMEEVENTPROC)m_trunk.sfp1(&GodMgr::OnStartNPCTalkEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("OpenCampaingFrame"), (FRAMEEVENTPROC)m_trunk.sfp1(&GodMgr::OnOpenCampaingFrame));

	m_pCmdMgr->UnRegister("NS_UseLimitedItem", (NETMSGPROC)m_trunk.sfp2(&GodMgr::OnNS_UseLimitedItem));
}

DWORD GodMgr::OnUseSkillEvent(tagUseSkillEvent* pEvent)
{
	const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(pEvent->dwSkillID);
	if(!P_VALID(pSkill))
		return 0;
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(pSkill->nType3 == ESSTE2_GodCondense)
	{
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("OnOpenCondensed"), NULL));
	}
	return 0;
}

DWORD GodMgr::OnStartNPCTalkEvent(tagStartNPCTalkEvent* pEvt)
{
	const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pEvt->typeID);
	if( pCreatureProto->eFunctionType == EFNPCT_GodBidNPC )
	{
		
	} 
	return 0;
}

const tagGodAreaName* GodMgr::GetAreaDataByID(const DWORD& dwID)
{
	return m_sGodAreaData.GetAreaDataByID(dwID);
}

DWORD GodMgr::OnOpenCampaingFrame(tagGameEvent* pEvent)
{
	CampaignFrame *pFrame = (CampaignFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("Campaign") );
	if( P_VALID(pFrame) )
	{
		pFrame->ShowFrameMsg();
		//pFrame->SetNpcID(pEvt->globalID);
	}
	return 0;
}
int GodMgr::GetFreePickAreaNumByGodHead(int nGodHead)
{
	return m_sGodAreaData.GetFreePickAreaNumByGodHead(nGodHead);
}
void GodMgr::GetFreePickAreaListByNum(int num ,TList<DWORD>& list)
{
	m_sGodAreaData.GetFreePickAreaListByNum(num,list);
}
DWORD GodMgr::OnNS_UseLimitedItem(tagNS_UseLimitedItem* pCmd,DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		switch(pCmd->dwErrorCode)
		{
		case E_UseItem_ItemCanNotUseByClergy:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("UseItem_ItemCanNotUseByClergy")]);
			break;
		case E_UseItem_ScriptError:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("UseItem_ScriptError")]);
			break;
		case E_UseItem_ItemNotExist:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("UseItem_ItemNotExist")]);
			break;
		case E_UseItem_ItemCanNotUseByFaithValue:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("UseItem_ItemCanNotUseByFaithValue")]);
			break;
		}
	}
	return 0;
}