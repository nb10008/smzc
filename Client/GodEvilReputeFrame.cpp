#include "StdAfx.h"
#include "GodEvilReputeFrame.h"
#include <sstream>
#include "TalentButton.h"
#include "Item.h"
#include "GameClientDefine.h"
#include "ItemMgr.h"
#include "CombatSysUtil.h"
#include "ItemProtoData.h"
#include "ItemButton.h"
#include "ReputeMgr.h"
#include "..\WorldDefine\msg_compose.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "SkillProtoData.h"
#include "IconStatic.h"
#include "ItemButton.h"
#include "SkillMgr.h"
#include "ItemMgr.h"
#include "Item.h"
#include "ItemEvent.h"
#include "CombatSysUtil.h"
#include "ItemFrame.h"
#include "RoleMgr.h"
#include "QuestMgr.h"
#include "..\WorldDefine\currency_define.h"
namespace
{
	enum 
	{
		E_SOUL_FIELD_ITEM = 100, //  只能放入战灵契约
		E_SOUL_FIELD_NOITEM = 101, //  物品栏为空
	};

	
}
GodEvilReputeFrame::GodEvilReputeFrame(void) : m_Trunk(this)
, m_pWnd(NULL)
, m_pWndSpiritSKill(NULL)
, m_pWndSpiritEquip(NULL)
, m_pPbnSpiritSKill(NULL)
, m_pPbnSpiritEquip(NULL)
, m_pStcGESoul(NULL)
, m_pBtnSysClose(NULL)
, m_pPgrRepute(NULL)
, m_pWnd_SkillOP(NULL)
, m_pStcOP_SKill(NULL)
, m_pPchOP_SKillName(NULL)
, m_pBtnOP_Item(NULL)
, m_pStcOP_ActiveSpirit(NULL)
, m_pStcOP_ActiveSilver(NULL)
, m_pStcOP_RenewSpirit(NULL)
, m_pStcOP_RenewSilver(NULL)
, m_pBtnOP_Active(NULL)
, m_pBtnOP_Renew(NULL)
, m_pBtnOP_SysClose(NULL)
, m_eType(ERPUT_God)
, m_dwCurChooseSkillID(NULL)
, m_pStcOP_SkillDesc(NULL)
, m_pStcOP_SkillState(NULL)
, m_pStcOP_SkillLimitTime(NULL)
, m_lastUpdateTime(NULL)
//------------

{
	memset(m_pLstBtnSkill,NULL,sizeof(m_pLstBtnSkill));
	memset(m_dwLstSkill,NULL,sizeof(m_dwLstSkill));
	memset(m_pLstStcSkillInfo,NULL,sizeof(m_pLstStcSkillInfo));
	memset(m_pLstStcReputeLevelReq,NULL,sizeof(m_pLstStcReputeLevelReq));
}

GodEvilReputeFrame::~GodEvilReputeFrame(void)
{
}


BOOL GodEvilReputeFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	/*! 注册网络事件 */
	//m_pCmdMgr->Register("NS_SynTotalStrength",  (NETMSGPROC)m_Trunk.sfp2(&ReputeFrame::OnNS_SynTotalStrength),      _T("NS_SynTotalStrength"));
	m_pFrameMgr->RegisterEventHandle(_T("ReputeEvent_RefreshGEInfo"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnEventRefreshGEInfo));
	m_pFrameMgr->RegisterEventHandle(_T("Close_GESkillOPWnd"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnEventCloseGESkillOPWnd));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_Move2GESkillRenew"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnEventMove2GESkillRenew));

	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_HunJingReset_Check"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnMsgBox_HunJingReset_Check));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_HunJingLevelUp_Check"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnMsgBox_HunJingLevelUp_Check));

	m_pCmdMgr->Register("NS_SoulCrystalGrow",				(NETMSGPROC)m_Trunk.sfp2(&GodEvilReputeFrame::OnNS_SoulCrystalGrow),	_T("NS_SoulCrystalGrow"));
	m_pCmdMgr->Register("NS_SoulCrystalRenew",				(NETMSGPROC)m_Trunk.sfp2(&GodEvilReputeFrame::OnNS_SoulCrystalRenew),	_T("NS_SoulCrystalRenew"));

	m_pItem = NULL;
	m_pEquip = NULL;


	return bRet;
}

BOOL GodEvilReputeFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pFrameMgr->UnRegisterEventHandler(_T("ReputeEvent_RefreshGEInfo"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnEventRefreshGEInfo));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_GESkillOPWnd"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnEventCloseGESkillOPWnd));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2GESkillRenew"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnEventMove2GESkillRenew));

	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_HunJingReset_Check"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnMsgBox_HunJingReset_Check));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_HunJingLevelUp_Check"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&GodEvilReputeFrame::OnMsgBox_HunJingLevelUp_Check));

	m_pCmdMgr->UnRegister("NS_SoulCrystalGrow",			(NETMSGPROC)m_Trunk.sfp2(&GodEvilReputeFrame::OnNS_SoulCrystalGrow));
	m_pCmdMgr->UnRegister("NS_SoulCrystalRenew",			(NETMSGPROC)m_Trunk.sfp2(&GodEvilReputeFrame::OnNS_SoulCrystalRenew));

	if (P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);

	if (P_VALID(m_pWnd_SkillOP))
		m_pGUI->AddToDestroyList(m_pWnd_SkillOP);

	if(P_VALID(GetObj("HunJingReset_Check")))
		KillObj("HunJingReset_Check");

	if(P_VALID(GetObj("HunJingLevelUp_Check")))
		KillObj("HunJingLevelUp_Check");

	return bRet;
}

BOOL GodEvilReputeFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\sw.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml(&element, "sw\\sw\\back\\k2\\itemback\\item", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "sw\\sw\\back\\k2\\itemback1\\item", "ClassName", "ItemButton");



	for (INT i = 0; i < GODEVIL_SKILL_NUM; i++)
	{
		CHAR szTmp[X_LONG_NAME]={0};
		_snprintf(szTmp,X_LONG_NAME,("sw\\sw\\k1\\itemback%d\\item"),i);
		m_pGUI->ChangeXml(&element,szTmp,("ClassName"),("TalentButton"));
	}

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"),&element);

	m_pWndSpiritSKill		= m_pWnd->GetChild(_T("sw\\k1"));
	m_pWndSpiritEquip		= m_pWnd->GetChild(_T("sw\\back"));
	m_pPbnSpiritSKill		= (GUIPushButton*)m_pWnd->GetChild(_T("sw\\bagchoose\\bagchoose1"));
	m_pPbnSpiritSKill->SetState(EGUIBS_PushDown);
	m_pPbnSpiritEquip		= (GUIPushButton*)m_pWnd->GetChild(_T("sw\\bagchoose\\bagchoose2"));
	m_pStcGESoul			= (GUIStatic*)m_pWnd->GetChild(_T("sw\\word6"));
	m_pBtnSysClose			= (GUIButton*)m_pWnd->GetChild(_T("sw\\closebutton"));

	Reload_GESkillUI(m_pWnd);
	Reload_GEEquipUI(m_pWnd);

	m_pWnd->SetInvisible(TRUE);

	XmlElement element1;
	tstring strPath1 = vEngine::g_strLocalPath + _T("\\ui\\mgs.xml");
	if( !m_pGUI->LoadXml( &element1, "VFS_System", strPath1.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml(&element1,"mgs\\mgs\\back\\tub\\item","ClassName","ItemButton");
	m_pGUI->ChangeXml(&element1,"mgs\\mgs\\tub\\item","ClassName","IconStatic");

	m_pWnd_SkillOP			 = m_pGUI->CreateWnd(_T("\\desktop"),&element1);

	m_pStcOP_SKill			 = (IconStatic*)m_pWnd_SkillOP->GetChild(_T("mgs\\tub\\item"));
	m_pPchOP_SKillName		 = (GUIPatch*)m_pWnd_SkillOP->GetChild(_T("mgs"));
	m_pBtnOP_Item			 = (ItemButton*)m_pWnd_SkillOP->GetChild(_T("mgs\\back\\tub\\item"));
	m_pStcOP_ActiveSpirit    = (GUIStatic*)m_pWnd_SkillOP->GetChild(_T("mgs\\word5"));
	m_pStcOP_ActiveSilver    = (GUIStatic*)m_pWnd_SkillOP->GetChild(_T("mgs\\word7"));
	m_pStcOP_RenewSpirit     = (GUIStatic*)m_pWnd_SkillOP->GetChild(_T("mgs\\back\\word2"));
	m_pStcOP_RenewSilver     = (GUIStatic*)m_pWnd_SkillOP->GetChild(_T("mgs\\back\\word4"));
	m_pBtnOP_Active			 = (GUIButton*)m_pWnd_SkillOP->GetChild(_T("mgs\\butt1"));
	m_pBtnOP_Renew			 = (GUIButton*)m_pWnd_SkillOP->GetChild(_T("mgs\\butt2"));
	m_pBtnOP_SysClose		 = (GUIButton*)m_pWnd_SkillOP->GetChild(_T("mgs\\closebutt"));
	m_pStcOP_SkillDesc		 = (GUIStaticEx*)m_pWnd_SkillOP->GetChild(_T("mgs\\word1"));
	m_pStcOP_SkillState      = (GUIStatic*)m_pWnd_SkillOP->GetChild(_T("mgs\\word2"));
	m_pStcOP_SkillLimitTime  = (GUIStatic*)m_pWnd_SkillOP->GetChild(_T("mgs\\word3"));

	m_pWnd_SkillOP->SetInvisible(TRUE);


	// 这些控件里的"神"可能需要改成"魔"
	m_lstSwapWnd.push_back(m_pPbnSpiritSKill);
	m_lstSwapWnd.push_back(m_pPbnSpiritEquip);
	m_lstSwapWnd.push_back(m_pStcGESoul);
	m_lstSwapWnd.push_back((GUIStatic*)m_pWnd->GetChild((_T("sw"))));
	m_lstSwapWnd.push_back((GUIStatic*)m_pWnd_SkillOP->GetChild((_T("mgs\\word6"))));
	m_lstSwapWnd.push_back((GUIStatic*)m_pWnd_SkillOP->GetChild((_T("mgs\\back\\word3"))));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&GodEvilReputeFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndSpiritSKill->GetFullName().c_str(), m_Trunk.sfp1(&GodEvilReputeFrame::EventHandler_Skill));
	m_pGUI->RegisterEventHandler(m_pWnd_SkillOP->GetFullName().c_str(), m_Trunk.sfp1(&GodEvilReputeFrame::EventHandler_Skill));
	m_pGUI->RegisterEventHandler(m_pWndSpiritEquip->GetFullName().c_str(), m_Trunk.sfp1(&GodEvilReputeFrame::EventHandler_Equip));

	return TRUE;
}

BOOL GodEvilReputeFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	SetStrategy(EIST_ItemPocket);
	return TRUE;
}

VOID GodEvilReputeFrame::Update()
{  
	if (timeGetTime() - m_lastUpdateTime > 2000)
	{
		m_lastUpdateTime = timeGetTime();
		if (!m_pWnd->IsInvisible() && RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID()) )
		{
			m_pWnd->SetInvisible(TRUE);
			m_pWnd_SkillOP->SetInvisible(TRUE);
			m_pFrameMgr->AddToDestroyList(this);
			SetStrategy(EIST_ItemPocket);
		}
	}

}

VOID GodEvilReputeFrame::OpenGEReputeFrame(BYTE byType)
{
	if (!P_VALID(m_pWnd))
		return;

	m_eType = (EReputeType)byType;
	if (byType == 0)
	{
		m_pWnd->SetInvisible(FALSE);
	}
	else
	{
		SwapGodToEvil();
		m_pWnd->SetInvisible(FALSE);
	}

	RefreshGEInfo();
}

VOID GodEvilReputeFrame::SwapGodToEvil()
{
	for (UINT i =0 ; i < m_lstSwapWnd.size(); i++)
	{
		GUIStatic* pStc = m_lstSwapWnd[i];
		if (!P_VALID(pStc))
			continue;

		tstring strText = pStc->GetText();
		tstring::size_type nPos = strText.find(g_StrTable[_T("God_Text")]);
		if (nPos != tstring::npos)
		{
			strText.replace(nPos,1,g_StrTable[_T("Evil_Text")]);
			pStc->SetText(strText.c_str());
		}
	}
	m_pStHunNumName->SetText(g_StrTable[_T("HunJing_MoHunNum")]);

	m_pStHunType->SetText(g_StrTable[_T("HunJing_MoHun")]);
}

BOOL GodEvilReputeFrame::Reload_GESkillUI(GUIWnd* pWnd)
{
	BOOL bRet = TRUE;
	if (!P_VALID(pWnd))
		return FALSE;

	m_pPgrRepute = (GUIProgress*)pWnd->GetChild(_T("sw\\k1\\progress"));

	TCHAR szTmp[X_LONG_NAME]={0};
	for (INT i = 0; i < GODEVIL_SKILL_LEVEL - 1; i++)
	{
		_sntprintf(szTmp,X_LONG_NAME,_T("sw\\k1\\num%d"),i);
		m_pLstStcReputeLevelReq[i] = (GUIStatic*)pWnd->GetChild(szTmp);
	}
	for (INT i = 0; i < GODEVIL_SKILL_NUM; i++)
	{
		_sntprintf(szTmp,X_LONG_NAME,_T("sw\\k1\\itemback%d\\item"),i);
		m_pLstBtnSkill[i] = (TalentButton*)pWnd->GetChild(szTmp);
		m_pLstBtnSkill[i]->SetIcon(strIconDefault.c_str());
		_sntprintf(szTmp,X_LONG_NAME,_T("sw\\k1\\word%d"),i);
		m_pLstStcSkillInfo[i] = (GUIStatic*)pWnd->GetChild(szTmp);
	}

	return bRet;
}

DWORD GodEvilReputeFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnSysClose)
			{
				m_pWnd->SetInvisible(TRUE);
				m_pWnd_SkillOP->SetInvisible(TRUE);
				m_pFrameMgr->AddToDestroyList(this);
				SetStrategy(EIST_ItemPocket);
			}
			else if (pWnd == m_pBtnResetAtt)
			{

				if (m_pBtnNoConfirmations->GetState() == EGUIBS_PushDown)
				{
					ResetAblity();
				}
				else
				{
					if(P_VALID(GetObj("HunJingReset_Check")))
						KillObj("HunJingReset_Check");

					CreateObj("HunJingReset_Check", "MsgBox");
					TObjRef<MsgBox>("HunJingReset_Check")->Init(_T(""), g_StrTable[_T("HunJingReset_Check")], _T("MsgBox_HunJingReset_Check"));
				}
				

				
			}
			else if (pWnd == m_pBtnLevelUp)
			{
				if (m_pBtnNoConfirmations->GetState() == EGUIBS_PushDown)
				{
					LevelUp();
				}
				else
				{
					if(P_VALID(GetObj("HunJingLevelUp_Check")))
						KillObj("HunJingLevelUp_Check");

					CreateObj("HunJingLevelUp_Check", "MsgBox");
					TObjRef<MsgBox>("HunJingLevelUp_Check")->Init(_T(""), g_StrTable[_T("HunJingLevelUp_Check")], _T("MsgBox_HunJingLevelUp_Check"));
				}
				
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pEvent->dwParam1 != 1)
				break;

			if (pWnd == m_pPbnSpiritEquip)
			{
				m_pWndSpiritSKill->SetInvisible(TRUE);
				m_pWndSpiritEquip->SetInvisible(FALSE);
			}
			else if (pWnd == m_pPbnSpiritSKill)
			{
				m_pWndSpiritSKill->SetInvisible(FALSE);
				m_pWndSpiritEquip->SetInvisible(TRUE);
				RefreshGESKillInfo();
			}
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		{
			if( EDT_Item != (EDragType)pEvent->dwParam1 )
				break;
			if (pWnd == m_pItemSpirit)
			{
				INT nPos = (INT16)LOWORD(pEvent->dwParam2);
				Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(!P_VALID(pItem))
					break;
				const tagItemProto	*pItemProto = ItemProtoData::Inst()->FindItemProto(pItem->GetItemTypeID());
				if(P_VALID(pItemProto) &&  pItemProto->eSpecFunc == EISF_HunShiJingHua )
				{
					m_pItem = pItem;
					m_pItemSpirit->RefreshItem(pItem->GetItemTypeID(),pItem->GetItemQuantity(),pItem->GetItemQuality());
					RefreshRateInfo();
				}
				else
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HunJing_NotHunShi")]);
				}
			}
			else if (pWnd == m_pItemSpiritCrystal)
			{
				INT nPos = (INT16)LOWORD(pEvent->dwParam2);
				Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(!P_VALID(pItem))
					break;
				const tagItemProto	*pItemProto = ItemProtoData::Inst()->FindItemProto(pItem->GetItemTypeID());
				if(P_VALID(pItemProto) &&  MIsEquipment(pItemProto->dwTypeID) &&  pItemProto->eType == EIT_SoulCrystal )
				{	
					if ((m_eType == ERPUT_God && pItemProto->eTypeEx == EITE_SoulCrystalGod)
						||(m_eType == ERPUT_Evil && pItemProto->eTypeEx == EITE_SoulCrystalEvil))
					{
						m_pEquip = pItem;
						const tagEquipSpec *pEquipSpec = ((Equipment*)pItem)->GetEquip();
						if(P_VALID(pEquipSpec))
							m_pItemSpiritCrystal->RefreshItem(pItem->GetItemTypeID(),0,pEquipSpec->byQuality);
						RefreshEquipUI(m_pEquip);
					}
					else
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HunJing_TypeNotMat")]);
					
				}
				else
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HunJing_NoEquip")]);
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (pWnd == m_pItemSpirit)
			{
				if(P_VALID(m_pItem))
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItem).c_str());
			}
			else if (pWnd == m_pItemSpiritCrystal)
			{
				if(P_VALID(m_pEquip))
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pEquip).c_str());
				}
					
			}
		}
		break;
	}
	return 0;
}

DWORD GodEvilReputeFrame::EventHandler_Skill( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if ( pWnd == m_pBtnOP_SysClose )
			{
				CloseSKillOPWnd();
			}
			else if( pWnd == m_pBtnOP_Active)
			{
				ReputeMgr::Inst()->SendNC_ActiveSoulSkill(m_dwCurChooseSkillID);
				CloseSKillOPWnd();
			}
			else if (pWnd == m_pBtnOP_Renew)
			{
				if (P_VALID(m_pBtnOP_Item->GetBtnData().GetData3()))
				{
					ReputeMgr::Inst()->SendNC_ContinueSoulSkill(m_dwCurChooseSkillID,m_pBtnOP_Item->GetBtnData().GetData3());
					CloseSKillOPWnd();
				}
				else
					ReputeMgr::Inst()->ShowGEMsgCode(E_SOUL_FIELD_NOITEM);

			}
			else if (pWnd == m_pBtnOP_Item)
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pBtnOP_Item->GetBtnData().GetData3());
				if (P_VALID(pItem))
				{
					SetRenewItem(NULL);
				}
			}
			else
			{
				for (INT i = 0; i < GODEVIL_SKILL_NUM; i++)
				{
					const tagForceSkill* pForce = ReputeMgr::Inst()->GetForceSkillInfo(m_dwLstSkill[i]);
					if (pWnd == m_pLstBtnSkill[i]
						&& P_VALID(pForce) 
						&& pForce->dwReputeLevel <= ReputeMgr::Inst()->GetGEReputeLevel(m_eType))
					{
						ShowSkillOPWnd(m_dwLstSkill[i]);
						m_dwCurChooseSkillID = m_dwLstSkill[i];
					}
				}
			}
			
			
		}
		break;
	case EGUISCB_Drop:
		{
			if (pWnd == m_pBtnOP_Item)
			{
				Item* pItem = ItemMgr::Inst()->GetConItem(EICT_Bag,LOWORD(pEvent->dwParam2));
				if (P_VALID(pItem))
				{
					SetRenewItem(pItem);
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (pWnd == m_pPgrRepute)
			{
				TCHAR szTmp[X_LONG_NAME] = {0};
				_sntprintf(szTmp,X_LONG_NAME,g_StrTable[_T("GodEvil_ReputeNum")],ReputeMgr::Inst()->GetGERepute(m_eType));
				ShowTip(pEvent->msg.pt - tagPoint(0,25), szTmp);
			}
			else if (pWnd == m_pBtnOP_Item)
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pBtnOP_Item->GetBtnData().GetData3());
				if (P_VALID(pItem))
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem->GetItemTypeID()).c_str());
				}	
			}
			else
			{
				for (INT i = 0; i < GODEVIL_SKILL_NUM; i++)
				{
					if (m_pLstBtnSkill[i] == pWnd && P_VALID(m_dwLstSkill[i]))
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetGESkillTips(m_dwLstSkill[i]).c_str());
					}
				}
			}
		}
		break;
	}
	return 0;
}

VOID GodEvilReputeFrame::SetRenewItem(Item* pItem)
{
	if (P_VALID(pItem) && P_VALID(pItem->GetProto()))
	{
		if (pItem->GetProto()->eSpecFunc == EISF_Soul_Skill)
		{
			m_pBtnOP_Item->RefreshItem(pItem->GetItemTypeID(),1,pItem->GetItemQuality(),pItem->GetItemId());
			pItem->SetOperable(FALSE);
		}
		else
			ReputeMgr::Inst()->ShowGEMsgCode(E_SOUL_FIELD_ITEM);

	}
	else
	{
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pBtnOP_Item->GetBtnData().GetData3());
		if (P_VALID(pItem))
		{
			pItem->SetOperable(TRUE);
		}
		m_pBtnOP_Item->RefreshItem();
	}
}

VOID GodEvilReputeFrame::RefreshGEInfo()
{
	TCHAR szTmp[X_LONG_NAME] = {0};
	_sntprintf(szTmp,X_LONG_NAME,_T("%d"),ReputeMgr::Inst()->GetGESoul(m_eType));
	m_pStcGESoul->SetText(szTmp);

	if (!m_pWndSpiritSKill->IsInvisible())
		RefreshGESKillInfo();
}

FLOAT GodEvilReputeFrame::CalcReputeProgress()
{

	DWORD dwLevel = ReputeMgr::Inst()->GetGEReputeLevel(m_eType);
	FLOAT fBase = (FLOAT)dwLevel * 0.2f;

	DWORD dwCurLevelPT = ReputeMgr::Inst()->GetForceLevelPoint(dwLevel);
	DWORD dwNextLevelPT = ReputeMgr::Inst()->GetForceLevelPoint(min(dwLevel + 1,GODEVIL_SKILL_LEVEL - 1));
	FLOAT fCur = (FLOAT)(ReputeMgr::Inst()->GetGERepute(m_eType) - dwCurLevelPT) / (FLOAT)(dwNextLevelPT - dwCurLevelPT);

	return fBase + fCur * 0.2f;
	
}

VOID GodEvilReputeFrame::RefreshGESKillInfo()
{
	TCHAR szTmp[X_LONG_NAME] = {0};
	for (INT i = 0; i < GODEVIL_SKILL_LEVEL - 2; i++)
	{
		_sntprintf(szTmp,X_LONG_NAME,_T("%d"),ReputeMgr::Inst()->GetForceLevelPoint(i + 1));
		m_pLstStcReputeLevelReq[i]->SetText(szTmp);
	}

	m_pPgrRepute->SetMaxValue(100.f);
	m_pPgrRepute->SetValue(CalcReputeProgress()*100.f,TRUE);


	memset(m_dwLstSkill,NULL,sizeof(m_dwLstSkill));
	for (UINT i = 0; i < GODEVIL_SKILL_LEVEL; i++)
	{
		vector<DWORD> lstSkill = ReputeMgr::Inst()->GetLocalPlayerForceSkillList(m_eType,i);
		for (UINT j = 0; j < lstSkill.size(); j++)
		{
			if (GODEVIL_SKILL_LEVEL_INDEX[i] + j < GODEVIL_SKILL_NUM)
				m_dwLstSkill[GODEVIL_SKILL_LEVEL_INDEX[i] + j] = lstSkill.at(j);
			else
				IMSG(_T("Force_skill Error"));
		}
	}
	for (INT i = 0; i < GODEVIL_SKILL_NUM; i++)
	{
		const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(m_dwLstSkill[i]);
		const tagForceSkill*	   pForce = ReputeMgr::Inst()->GetForceSkillInfo(m_dwLstSkill[i]);
		
		if (P_VALID(pProto) && P_VALID(pForce))
		{
			m_pLstBtnSkill[i]->SetIcon(SkillMgr::Inst()->GetSkillIconPath(m_dwLstSkill[i]));

			if ( pForce->dwReputeLevel <= ReputeMgr::Inst()->GetGEReputeLevel(m_eType) )
				m_pLstBtnSkill[i]->SetPicColor(TALENTBTN_COLOR[1]);
			else
				m_pLstBtnSkill[i]->SetPicColor(TALENTBTN_COLOR[0]);

			TCHAR szText[X_LONG_NAME] = {0};
			const tagSkillData* pData  = SkillMgr::Inst()->GetSkillDataByTypeID(m_dwLstSkill[i]);
			if (P_VALID(pData))
			{
				m_pLstStcSkillInfo[i]->SetTextColor(0xFF00FF00);
				INT nDays = pData->dwLimitTime / (3600 * 24);
				INT nHours = (pData->dwLimitTime / 3600) % 24;
				if (pForce->bForever || nDays > 1000)
					m_pLstStcSkillInfo[i]->SetText(g_StrTable[_T("GodEvil_Actived")]);
				else
				{
					if (nDays > 0)
					{
						TCHAR szTmp[X_LONG_NAME] = {0};
						_sntprintf(szTmp,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime_Days")],nDays);
						_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime")],szTmp,nHours);
					}
					else
					{
						if (nHours >0)
							_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime")],_T(""),nHours);
						else
							_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime")],g_StrTable[_T("GodEvil_LimitTime_Hour")],1);
					}
					m_pLstStcSkillInfo[i]->SetText(szText);
				}
			}
			else
			{
				m_pLstStcSkillInfo[i]->SetTextColor(0xFFFF0000);
				m_pLstStcSkillInfo[i]->SetText(g_StrTable[_T("GodEvil_UnActive")]);
			}
		}

	}
}

VOID GodEvilReputeFrame::ShowSkillOPWnd(DWORD dwSkillID)
{
	m_pWnd_SkillOP->SetInvisible(FALSE);
	m_pWnd_SkillOP->FlipToTop();
	SetStrategy(EIST_GESkillRenew);
	m_pBtnOP_Renew->SetEnable(false);
	m_pBtnOP_Active->SetEnable(false);


	m_pStcOP_SkillLimitTime->SetText(_T(""));
	const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(dwSkillID);
	const tagForceSkill*	   pForce = ReputeMgr::Inst()->GetForceSkillInfo(dwSkillID);
	if (P_VALID(pProto) && P_VALID(pForce))
	{
		m_pStcOP_SKill->SetPic(SkillMgr::Inst()->GetSkillIconPath(dwSkillID));
		m_pPchOP_SKillName->SetText(pProto->szName);
		m_pStcOP_SkillDesc->SetText(pProto->szDesc);

		
		TCHAR szText[X_LONG_NAME] = {0};

		_sntprintf(szText,X_LONG_NAME,_T("%d"),pForce->dwActvNeedSoul);
		m_pStcOP_ActiveSpirit->SetText(szText);
		tstring strMoney = _T("");
		if (MSilver2DBGold(pForce->dwActvNeedMoney) > 0)
		{
			_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("Golden_Text")],MSilver2DBGold(pForce->dwActvNeedMoney));
			strMoney = strMoney + szText;
		}
		if (MSilver2DBSilver(pForce->dwActvNeedMoney) > 0)
		{
			_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("Silver_Text")],MSilver2DBSilver(pForce->dwActvNeedMoney));
			strMoney = strMoney + szText;
		}
		m_pStcOP_ActiveSilver->SetText(strMoney.c_str());


		_sntprintf(szText,X_LONG_NAME,_T("%d"),pForce->dwRenewNeedSoul);
		m_pStcOP_RenewSpirit->SetText(szText);
		strMoney = _T("");
		if (MSilver2DBGold(pForce->dwRenewNeedMoney) > 0)
		{
			_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("Golden_Text")],MSilver2DBGold(pForce->dwRenewNeedMoney));
			strMoney = strMoney + szText;
		}
		if (MSilver2DBSilver(pForce->dwRenewNeedMoney) > 0)
		{
			_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("Silver_Text")],MSilver2DBSilver(pForce->dwRenewNeedMoney));
			strMoney = strMoney + szText;
		}
		m_pStcOP_RenewSilver->SetText(strMoney.c_str());

		const tagSkillData* pData  = SkillMgr::Inst()->GetSkillDataByTypeID(dwSkillID);
		if (P_VALID(pData))
		{
			INT nDays = pData->dwLimitTime / (3600 * 24);
			INT nHours = (pData->dwLimitTime / 3600) % 24;

			if (pForce->bForever || nDays > 1000)
			{
				m_pStcOP_SkillState->SetText(g_StrTable[_T("GodEvil_Actived")]);
				m_pStcOP_SkillLimitTime->SetInvisible(TRUE);
			}
			else
			{
				m_pStcOP_SkillLimitTime->SetInvisible(FALSE);
				m_pBtnOP_Renew->SetEnable(true);

				if (nDays > 0)
				{
					TCHAR szTmp[X_LONG_NAME] = {0};
					_sntprintf(szTmp,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime_Days")],nDays);
					_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime")],szTmp,nHours);
				}
				else
				{
					if (nHours >0)
						_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime")],_T(""),nHours);
					else
						_sntprintf(szText,X_LONG_NAME,g_StrTable[_T("GodEvil_LimitTime")],g_StrTable[_T("GodEvil_LimitTime_Hour")],1);
				}
				m_pStcOP_SkillLimitTime->SetText(szText);
			}
		}
		else
			m_pBtnOP_Active->SetEnable(true);

	}
}

DWORD GodEvilReputeFrame::OnEventRefreshGEInfo(tagGameEvent* pGameEvent)
{
	RefreshGEInfo();
	return 0;
}

DWORD GodEvilReputeFrame::OnEventCloseGESkillOPWnd(tagGameEvent* pGameEvent)
{
	CloseSKillOPWnd();

	return 0;
}

DWORD GodEvilReputeFrame::OnEventMove2GESkillRenew(tagItemPosChangeExEvent* pGameEvent)
{
	if (P_VALID(m_pBtnOP_Item))
	{
		Item* pItem = ItemMgr::Inst()->GetConItem(EICT_Bag,pGameEvent->n16IndexSrc);
		if (P_VALID(pItem))
		{
			SetRenewItem(pItem);
		}
		
	}

	return 0;
}

VOID GodEvilReputeFrame::CloseSKillOPWnd()
{
	if (P_VALID(m_pWnd_SkillOP))
	{
		m_pWnd_SkillOP->SetInvisible(TRUE);
		SetRenewItem(NULL);
		SetStrategy(EIST_ItemPocket);
	}
}
//--------------------军事分界------------------------------------------朝鲜省
//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
//--------------------38线----------------------------------------------
//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
//--------------------军事分界------------------------------------------韩国省
DWORD GodEvilReputeFrame::EventHandler_Equip( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		break;
	}
	return 0;
}

BOOL GodEvilReputeFrame::Reload_GEEquipUI(GUIWnd* pWnd)
{
	BOOL bRet = TRUE;
	m_pStShenHunNum= (GUIStatic*)pWnd->GetChild(_T("sw\\word6"));
	m_pWndSpiritEquip = (GUIWnd*)pWnd->GetChild(_T("sw\\back"));
	m_pItemSpiritCrystal = (ItemButton*	)pWnd->GetChild(_T("sw\\back\\k2\\itemback\\item"));//魂晶升级界面神魂晶
	m_pItemSpirit = (ItemButton*	)pWnd->GetChild(_T("sw\\back\\k2\\itemback1\\item"));			//魂石精华
	tstringstream tStr;
	for(int i = 0; i < SPIRIT_CRYSTAL_MAX_ATTRIBUTES; i++)
	{
		GUIWnd* pTempWnd = NULL;
		tStr.str(_T(""));
		tStr<<_T("sw\\back\\k2\\new\\listback")<<i;
		pTempWnd = (GUIPatch*)pWnd->GetChild(tStr.str().c_str());	//神魂晶属性
		m_pStAttrabutes[i].pStName = (GUIStatic*)pTempWnd->GetChild(_T("holidaytitle"));
		m_pStAttrabutes[i].pStDesc = (GUIStatic*)pTempWnd->GetChild(_T("word"));
	}
	m_pNextLevelNeedsItem = (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word3"));
	m_pNextLevelNeedsGold= (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word14"));
	m_pNextLevelNeedsSilver= (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word15"));


	m_pStName= (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word0"));	
	m_pStZiZhi= (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word10"));
	m_pStTotal= (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word12"));


	m_pNewAttProbability = (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word5"));	//开启新能力概率
	m_pUpdateAttProbability = (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word7"));//能力等级提升概率
	m_pBtnLevelUp = (GUIButton*)pWnd->GetChild(_T("sw\\back\\k2\\butt1"));				//提升等级 按钮
	m_pBtnResetAtt = (GUIButton*)pWnd->GetChild(_T("sw\\back\\k2\\butt2"));				//重置能力按钮
	m_pBtnNoConfirmations = (GUIPushButton*)pWnd->GetChild(_T("sw\\back\\k2\\optbutt"));	//没有二次确认

	m_pStHunNumName = (GUIStatic*)pWnd->GetChild(_T("sw\\word1"));
	m_pStHunType= (GUIStatic*)pWnd->GetChild(_T("sw\\back\\k2\\word13"));
	return bRet;
}


VOID GodEvilReputeFrame::RefreshEquipUI(Item* pEquip)
{
	
	if(!P_VALID(pEquip))	return;

	TCHAR szBuff[X_LONG_NAME];

	const tagItemProto	*pItemProto = ItemProtoData::Inst()->FindItemProto(pEquip->GetItemTypeID());
	if(P_VALID(pItemProto) && MIsEquipment(pItemProto->dwTypeID) &&  pItemProto->eType == EIT_SoulCrystal )
	{
		ClearUI();

		const tagSoulCrystalSpec *pSoulSpec = ((Equipment*)pEquip)->GetSoulCrystalSpec();
		if(!P_VALID(pSoulSpec))		return;


		if (P_VALID(pItemProto->pDisplayInfo))
		{
			_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%s Lv %d"),pItemProto->pDisplayInfo->szName,pSoulSpec->bySoulCrystalLvl );
			m_pStName->SetText(szBuff);
		}
		
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),pSoulSpec->bySoulCrystalQlty);
		m_pStZiZhi->SetText(szBuff);

		DWORD dwNeng = 0;
		for (int i = 0; i<SOUL_CRYSTAL_ATT_MAX_NUM ; i++ )
		{
			tagHunJingName* pHunJing =  ReputeMgr::Inst()->FindHunJingName( pSoulSpec->nSoulAttID[i]);
			if(!P_VALID(pHunJing))	continue;

			_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%s Lv%d"),pHunJing->szName,pHunJing->nLevel);
			m_pStAttrabutes[i].pStName->SetText(szBuff);
			if (pHunJing->nLevel == 10)
				m_pStAttrabutes[i].pStName->SetTextColor(0xffff402d);
			else if( (pHunJing->dwID / 100) %10 == 3)
				m_pStAttrabutes[i].pStName->SetTextColor(0xfffff717);
			else
				m_pStAttrabutes[i].pStName->SetTextColor(0xFF18FB16);
			m_pStAttrabutes[i].pStDesc->SetText(pHunJing->szDesc);
			dwNeng += pHunJing->nLevel;
		}

		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),dwNeng);
		m_pStTotal->SetText(szBuff);

		if (pSoulSpec->bySoulCrystalLvl < 10)
		{
			tagHunJingLevel* pHunJingLevel =  ReputeMgr::Inst()->FindHunJingLevelInfo(pSoulSpec->bySoulCrystalLvl+1);
			if(P_VALID(pHunJingLevel))
			{
				_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),pHunJingLevel->dwHunNum);
				m_pNextLevelNeedsItem->SetText(szBuff);
				if (ReputeMgr::Inst()->GetGESoul(m_eType) < pHunJingLevel->dwHunNum)
					m_pNextLevelNeedsItem->SetTextColor(0xFFFF0000);
				else
					m_pNextLevelNeedsItem->SetTextColor(0xFF00FF00);

				_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),pHunJingLevel->dwMoney/10000);
				m_pNextLevelNeedsGold->SetText(szBuff);

				_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),pHunJingLevel->dwMoney%10000);
				m_pNextLevelNeedsSilver->SetText(szBuff);
			}
			
		}
		
		RefreshRateInfo();
	}
}

VOID GodEvilReputeFrame::RefreshRateInfo()
{

	if (!P_VALID(m_pEquip))		return;
	
	const tagSoulCrystalSpec *pSoulSpec = ((Equipment*)m_pEquip)->GetSoulCrystalSpec();
	if(!P_VALID(pSoulSpec))		return;

	TCHAR szBuff[X_LONG_NAME];

	DWORD dwNewAblity = 0;	// 开启新能力概率
	DWORD dwLevelUp = 0;		// 能力等级提升概率
	// 公式
	dwNewAblity =  2 * (DWORD)sqrt((double)pSoulSpec->bySoulCrystalQlty);
	dwLevelUp = pSoulSpec->bySoulCrystalQlty;

	if(P_VALID(m_pItem) && m_pItem->GetItemQuantity() > 0)
	{
		const tagItemProto	*pItemProto = ItemProtoData::Inst()->FindItemProto(m_pItem->GetItemTypeID());
		if(P_VALID(pItemProto) &&  pItemProto->eSpecFunc == EISF_HunShiJingHua )
		{
			dwNewAblity+= pItemProto->nSpecFuncVal1;
			dwLevelUp += pItemProto->nSpecFuncVal2;
			m_pItemSpirit->RefreshItem(pItemProto->dwTypeID,0,pItemProto->byQuality);
		}
		else
			m_pItemSpirit->RefreshItem();
	}
	else
	{
		m_pItem = NULL;
		m_pItemSpirit->RefreshItem();
	}
		
	

	if(dwNewAblity > 100)	dwNewAblity = 100;
	if(dwLevelUp > 100)	dwLevelUp = 100;

	_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d%%"),dwNewAblity);
	m_pNewAttProbability->SetText(szBuff);

	_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d%%"),dwLevelUp);
	m_pUpdateAttProbability->SetText(szBuff);


}

void GodEvilReputeFrame::LockItem( bool bOperable )
{
	if( P_VALID(m_pItem) )
		m_pItem->SetOperable(!bOperable);
}

VOID GodEvilReputeFrame::ResetAblity()
{
	if (!P_VALID(m_pEquip)) return;
	
	tagNC_SoulCrystalRenew cmd;
	cmd.n64Serial = m_pEquip->GetItemId();
	m_pSession->Send(&cmd);

}
VOID GodEvilReputeFrame::LevelUp()
{
	if (!P_VALID(m_pEquip)) return;

	tagNC_SoulCrystalGrow cmd;
	cmd.n64Serial = m_pEquip->GetItemId();
	if(P_VALID(m_pItem))	cmd.n64IMSerial = m_pItem->GetItemId();
	else cmd.n64IMSerial = GT_INVALID;
	m_pSession->Send(&cmd);
}

DWORD GodEvilReputeFrame::OnMsgBox_HunJingReset_Check(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult )
	{
		ResetAblity();
	}
	return 0;
}

DWORD GodEvilReputeFrame::OnMsgBox_HunJingLevelUp_Check(tagMsgBoxEvent* pGameEvent)
{

	if(MBF_OK == pGameEvent->eResult )
	{
		LevelUp();
	}
	return 0;
}

VOID GodEvilReputeFrame::ClearUI()
{
	m_pNewAttProbability->SetText(_T(""));
	m_pUpdateAttProbability->SetText(_T(""));
	m_pStName->SetText(_T(""));
	m_pStZiZhi->SetText(_T(""));
	m_pStTotal->SetText(_T(""));
	m_pNextLevelNeedsItem->SetText(_T(""));
	m_pNextLevelNeedsGold->SetText(_T(""));
	m_pNextLevelNeedsSilver->SetText(_T(""));
	for (int i = 0; i<SOUL_CRYSTAL_ATT_MAX_NUM ; i++ )
	{
		m_pStAttrabutes[i].pStName->SetText(_T(""));
		m_pStAttrabutes[i].pStDesc->SetText(_T(""));
	}
}

DWORD GodEvilReputeFrame::OnNS_SoulCrystalGrow(tagNS_SoulCrystalGrow* pCmd, DWORD)
{
	ClearUI();
	RefreshEquipUI(m_pEquip);
	return 0;
}
DWORD GodEvilReputeFrame::OnNS_SoulCrystalRenew(tagNS_SoulCrystalRenew* pCmd, DWORD)
{
	ClearUI();
	RefreshEquipUI(m_pEquip);
	return 0;
}