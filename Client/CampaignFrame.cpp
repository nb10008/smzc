#include "StdAfx.h"
#include "CampaignFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "CampaignResultFrame.h"
#include "../WorldDefine/msg_god.h"
#include "CombatSysUtil.h"
#include "ClergyProtoData.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"

CampaignFrame::CampaignFrame(void):m_Trunk(this)
{
}

CampaignFrame::~CampaignFrame(void)
{
}

BOOL CampaignFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_Campaign",		(NETMSGPROC)m_Trunk.sfp2(&CampaignFrame::OnNS_Campaign),		_T("NS_Campaign"));
	m_pCmdMgr->Register( "NS_CampaignInfo",		(NETMSGPROC)m_Trunk.sfp2(&CampaignFrame::OnNS_CampaignInfo),		_T("NS_CampaignInfo"));

	m_pFrameMgr->RegisterEventHandle(_T("OpenCampaingResultFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&CampaignFrame::OnOpenCampaingResultFrame));


	m_eCurrType = ECGT_Null;
	m_escapeTime = 0;
	m_dwNPCID = 0;
	return TRUE;
}
BOOL CampaignFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister( "NS_Campaign",							(NETMSGPROC)m_Trunk.sfp2(&CampaignFrame::OnNS_Campaign) );
	m_pCmdMgr->UnRegister( "NS_CampaignInfo",							(NETMSGPROC)m_Trunk.sfp2(&CampaignFrame::OnNS_CampaignInfo) );

	m_pFrameMgr->UnRegisterEventHandler(_T("OpenCampaingResultFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&CampaignFrame::OnOpenCampaingResultFrame));
	

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	return bRet;
}
BOOL CampaignFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\god_campaigner.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\hps\\2-5\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\fss\\2-5\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\tys\\2-5\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\zs\\2-5\\state", "ClassName", "GUIStaticEx");

	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\hps\\f_1st\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\fss\\f_1st\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\tys\\f_1st\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\zs\\f_1st\\state", "ClassName", "GUIStaticEx");

	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\hps\\m_1st\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\fss\\m_1st\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\tys\\m_1st\\state", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "campaigner_win\\back\\zs\\m_1st\\state", "ClassName", "GUIStaticEx");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&CampaignFrame::EventHandler));

	
	for (int i = EGT_Wars; i <EGT_Num; i++)
	{
		GUIWnd* pTempWnd = NULL;
		GUIWnd* pTempWnd2 = NULL;
		if(EGT_Wars == i )		{	pTempWnd = m_pWnd->GetChild(_T("back\\zs"));	pTempWnd2 = m_pWnd->GetChild(_T("back\\zs0"));}
		if(EGT_Suns == i )			{	pTempWnd = m_pWnd->GetChild(_T("back\\tys"));		pTempWnd2 = m_pWnd->GetChild(_T("back\\tys0"));}
		if(EGT_Manas == i )		{	pTempWnd = m_pWnd->GetChild(_T("back\\fss"));		pTempWnd2 = m_pWnd->GetChild(_T("back\\fss0"));}
		if(EGT_Peaces == i )		{	pTempWnd = m_pWnd->GetChild(_T("back\\hps"));		pTempWnd2 = m_pWnd->GetChild(_T("back\\hps0"));}

		m_CampaignGods[i].pStSelected = (GUIPatch*)pTempWnd->GetChild(_T("select"));
		m_CampaignGods[i].pStMale = (GUIStaticEx*)pTempWnd->GetChild(_T("m_1st\\state"));
		m_CampaignGods[i].pStFemale = (GUIStaticEx*)pTempWnd->GetChild(_T("f_1st\\state"));
		m_CampaignGods[i].pStCommon = (GUIStaticEx*)pTempWnd->GetChild(_T("2-5\\state"));
		m_CampaignGods[i].pPic = (GUIStatic*)pTempWnd->GetChild(_T("pic"));

		m_CampaignGods[i].pStCampaignNum = (GUIStatic*)pTempWnd2->GetChild(_T("num"));
		m_CampaignGods[i].pStStrength = (GUIStatic*)pTempWnd2->GetChild(_T("num0"));

		m_CampaignGods[i].pPBack = (GUIPatch*)pTempWnd;
		m_CampaignGods[i].pStTextName = (GUIStatic*)pTempWnd->GetChild(_T("word"));
		m_CampaignGods[i].pStTextMale1 = (GUIStatic*)pTempWnd->GetChild(_T("m_1st\\condition"));
		m_CampaignGods[i].pStTextMale2 = (GUIStatic*)pTempWnd->GetChild(_T("m_1st\\name"));
		m_CampaignGods[i].pStTextFemale1 = (GUIStatic*)pTempWnd->GetChild(_T("f_1st\\condition"));
		m_CampaignGods[i].pStTextFemale2 = (GUIStatic*)pTempWnd->GetChild(_T("f_1st\\name"));
		m_CampaignGods[i].pStTextCommon1 = (GUIStatic*)pTempWnd->GetChild(_T("2-5\\condition"));
		m_CampaignGods[i].pStTextCommon2 = (GUIStatic*)pTempWnd->GetChild(_T("2-5\\name"));

	}

	m_pBnClose  = (GUIButton*)m_pWnd->GetChild(_T("back\\closebutton"));
	m_pBnCampaign  = (GUIButton*)m_pWnd->GetChild(_T("back\\attendbutt"));
	m_pBnShowLastResult  = (GUIButton*)m_pWnd->GetChild(_T("back\\seebutt"));
	m_pBnTitle = (GUIStatic*)m_pWnd->GetChild(_T("back\\state"));

	for (int i =0 ;i < ECGT_Num; i++)
	{
		m_CampaignGods[i].pStSelected->SetInvisible(TRUE);
	}

	m_pWnd->SetInvisible(TRUE);

	return TRUE;
}
BOOL CampaignFrame::EscCancel()
{
	if (m_pWnd->IsInvisible())
	{
		return false;
	} 
	else
	{
		m_pWnd->SetInvisible(TRUE);
		m_eCurrType = ECGT_Null;
		return true;
	}
	return true;
}


BOOL CampaignFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBnClose)
			{
				m_pWnd->SetInvisible(TRUE);
				m_eCurrType = ECGT_Null;
			}
			else if (pWnd == m_pBnCampaign)
			{
				if ( ECGT_Null == m_eCurrType)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Campaign_NotChooseType")]);
					break;
				}
				tagNC_Campaign cmd;
				cmd.eType = m_eCurrType;
				m_pSession->Send(&cmd);
			}
			else if (pWnd == m_pBnShowLastResult)
			{
				CampaignResultFrame *pFrame = (CampaignResultFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("CampaignResult") );
				if( !P_VALID(pFrame) )
				{
					pFrame = (CampaignResultFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("CampaignResult"), _T("CampaignResultFrame"), 0 );
					pFrame->AcquireInfo(ECGT_War);
					pFrame->SetInvisiable(FALSE);
				}	
				else
				{
					pFrame->AcquireInfo(ECGT_War);
					pFrame->SetInvisiable(FALSE);
				}
			}
			else
			{
				for (int i =0 ;i < ECGT_Num; i++)
				{

					if (pWnd == m_CampaignGods[i].pPic	|| 
						pWnd == m_CampaignGods[i].pStMale	||
						pWnd == m_CampaignGods[i].pStFemale  ||
						pWnd == m_CampaignGods[i].pStCommon	||
						pWnd == m_CampaignGods[i].pPBack	||
						pWnd == m_CampaignGods[i].pStTextName	||
						pWnd == m_CampaignGods[i].pStTextMale1	||
						pWnd == m_CampaignGods[i].pStTextMale2	||
						pWnd == m_CampaignGods[i].pStTextFemale1	||
						pWnd == m_CampaignGods[i].pStTextFemale2	||
						pWnd == m_CampaignGods[i].pStTextCommon1	||
						pWnd == m_CampaignGods[i].pStTextCommon2	)
					{
						m_CampaignGods[i].pStSelected->SetInvisible(FALSE);
						m_CampaignGods[i].pStSelected->FlipToTop();
						m_eCurrType = (ECampaignGodType)i;
					}
					else
						m_CampaignGods[i].pStSelected->SetInvisible(TRUE);	
				}
			}
		}
		break;
	case EGUIE_DragStart:
		{
			for (int i =0 ;i < ECGT_Num; i++)
			{

				if (pWnd == m_CampaignGods[i].pPic	|| 
					pWnd == m_CampaignGods[i].pStMale	||
					pWnd == m_CampaignGods[i].pStFemale  ||
					pWnd == m_CampaignGods[i].pStCommon	||
					pWnd == m_CampaignGods[i].pPBack	||
					pWnd == m_CampaignGods[i].pStTextName	||
					pWnd == m_CampaignGods[i].pStTextMale1	||
					pWnd == m_CampaignGods[i].pStTextMale2	||
					pWnd == m_CampaignGods[i].pStTextFemale1	||
					pWnd == m_CampaignGods[i].pStTextFemale2	||
					pWnd == m_CampaignGods[i].pStTextCommon1	||
					pWnd == m_CampaignGods[i].pStTextCommon2	)
				{
					m_CampaignGods[i].pStSelected->SetInvisible(FALSE);
					m_eCurrType = (ECampaignGodType)i;
				}
				else
					m_CampaignGods[i].pStSelected->SetInvisible(TRUE);	
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

DWORD CampaignFrame::CalculateStrangth(int nType)
{
	DWORD result =0;
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(plp))
		return result;

	if (EGT_Wars == nType)
	{
		result = plp->GetAttribute(ERA_MaxHP)*2 + plp->GetAttribute(ERA_MaxMP) + plp->GetAttribute(ERA_ExAttack)*3 +  plp->GetAttribute(ERA_InAttack) \
			+  plp->GetAttribute(ERA_ExDefense)*2 +  plp->GetAttribute(ERA_InDefense) +  plp->GetAttribute(ERA_AttackTec) +  plp->GetAttribute(ERA_DefenseTec) \
			+  plp->GetAttribute(ERA_Regain_Addtion);
	}
	else if (EGT_Suns == nType)
	{
		result = plp->GetAttribute(ERA_MaxHP) + plp->GetAttribute(ERA_MaxMP) + plp->GetAttribute(ERA_ExAttack)*3 +  plp->GetAttribute(ERA_InAttack) \
			+  plp->GetAttribute(ERA_ExDefense)*2 +  plp->GetAttribute(ERA_InDefense) +  plp->GetAttribute(ERA_AttackTec)*3 +  plp->GetAttribute(ERA_DefenseTec) \
			+  plp->GetAttribute(ERA_Regain_Addtion);
	}
	else if (EGT_Manas == nType)
	{
		result = plp->GetAttribute(ERA_MaxHP) + plp->GetAttribute(ERA_MaxMP)*2 + plp->GetAttribute(ERA_ExAttack) +  plp->GetAttribute(ERA_InAttack)*3 \
			+  plp->GetAttribute(ERA_ExDefense)+  plp->GetAttribute(ERA_InDefense)*2  +  plp->GetAttribute(ERA_AttackTec) +  plp->GetAttribute(ERA_DefenseTec) \
			+  plp->GetAttribute(ERA_Regain_Addtion);
	}
	else if (EGT_Peaces == nType)
	{
		result = plp->GetAttribute(ERA_MaxHP)*2 + plp->GetAttribute(ERA_MaxMP) + plp->GetAttribute(ERA_ExAttack) +  plp->GetAttribute(ERA_InAttack)*2 \
			+  plp->GetAttribute(ERA_ExDefense) +  plp->GetAttribute(ERA_InDefense) +  plp->GetAttribute(ERA_AttackTec) +  plp->GetAttribute(ERA_DefenseTec) \
			+  plp->GetAttribute(ERA_Regain_Addtion)*5;
	}

	return result;
}

DWORD CampaignFrame::OnNS_Campaign(tagNS_Campaign* pNetCmd,DWORD)
{
	if ( E_Success == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Campaign_success")]);
	}
	else if (E_Enroll_Clergy_LevelLimited == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Campaign_LevelLimited")]);
	}
	else if (E_Enroll_Clergy_SexLimited == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Campaign_SexLimited")]);
	}
	else if (E_Enroll_Clergy_ClassLimited == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Campaign_ClassLimited")]);
	}
	else if (E_Enroll_Clergy_GodheadLimited == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Campaign_GodheadLimited")]);
	}
	else if (E_Enroll_Clergy_AttValueLimited == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Campaign_AttValueLimited")]);
	}
	return 0;
}

DWORD CampaignFrame::OnNS_CampaignInfo(tagNS_CampaignInfo* pNetCmd,DWORD)
{
	TCHAR szBuffer[X_LONG_NAME];
 	if (pNetCmd->eType == ECGT_Null)
 	{
 		m_pBnCampaign->SetEnable(TRUE);
		m_pBnTitle->SetText(g_StrTable[_T("Campaign_Kind_null")]);
 	}
 	else
 	{
 		m_pBnCampaign->SetEnable(FALSE);
		_stprintf(szBuffer,_T("Campaign_Kind_%d"), (int)pNetCmd->eType);
		m_pBnTitle->SetText(g_StrTable[szBuffer]);
 	}
 
 	
 	for (int i =0 ;i < ECGT_Num; i++)
 	{
 		_stprintf(szBuffer, _T("%d"), pNetCmd->dwNum[i]);
 		m_CampaignGods[i].pStCampaignNum->SetText(szBuffer);
 		_stprintf(szBuffer, _T("%d"), CalculateStrangth(i));
 		m_CampaignGods[i].pStStrength->SetText(szBuffer);
 
 		m_CampaignGods[i].pStSelected->SetInvisible(TRUE);
 	}
	
	RefreshSatisfyUI();
	m_pWnd->SetInvisible(FALSE);

	return 0;
}
void CampaignFrame::RefreshSatisfyUI()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))	return;
	DWORD dwClassVal;
	GetClassEnumBinary(pLp->GetPlayerClass(), dwClassVal);

	const tagClergyName* pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_WarWinner);	// 战神
	if(P_VALID(pClergyName))
	{
		if( pClergyName->eAttType != ERA_Null  && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue  
			&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
			&& (dwClassVal & pClergyName->dwClassLimit))
		{
			m_CampaignGods[EGT_Wars].pStCommon->SetText(g_StrTable[_T("Campaign_Satisfy")]);
			m_CampaignGods[EGT_Wars].pStCommon->SetTextColor(0xff00ff00);
		}
		else
		{
			m_CampaignGods[EGT_Wars].pStCommon->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
			m_CampaignGods[EGT_Wars].pStCommon->SetTextColor(0xffff0000);
		}
	}
	pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_SunWinner);	// 太阳神
	if(P_VALID(pClergyName))
	{
		if( pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue 
			&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
			&& (dwClassVal & pClergyName->dwClassLimit))
		{
			m_CampaignGods[EGT_Suns].pStCommon->SetText(g_StrTable[_T("Campaign_Satisfy")]);
			m_CampaignGods[EGT_Suns].pStCommon->SetTextColor(0xff00ff00);
		}
		else
		{
			m_CampaignGods[EGT_Suns].pStCommon->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
			m_CampaignGods[EGT_Suns].pStCommon->SetTextColor(0xffff0000);
		}
	}
	pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_ManaWinner);	// 法师神
	if(P_VALID(pClergyName))
	{
		if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue 
			&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
			&& (dwClassVal & pClergyName->dwClassLimit))
		{
			m_CampaignGods[EGT_Manas].pStCommon->SetText(g_StrTable[_T("Campaign_Satisfy")]);
			m_CampaignGods[EGT_Manas].pStCommon->SetTextColor(0xff00ff00);
		}
		else
		{
			m_CampaignGods[EGT_Manas].pStCommon->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
			m_CampaignGods[EGT_Manas].pStCommon->SetTextColor(0xffff0000);
		}
	}
	pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_PeaceWinner);	// 和平神
	if(P_VALID(pClergyName))
	{
		if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType )>= pClergyName->dwAttValue 
			&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
			&& (dwClassVal & pClergyName->dwClassLimit))
		{
			m_CampaignGods[EGT_Peaces].pStCommon->SetText(g_StrTable[_T("Campaign_Satisfy")]);
			m_CampaignGods[EGT_Peaces].pStCommon->SetTextColor(0xff00ff00);
		}
		else
		{
			m_CampaignGods[EGT_Peaces].pStCommon->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
			m_CampaignGods[EGT_Peaces].pStCommon->SetTextColor(0xffff0000);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(pLp->GetPlayerSex())
	{
		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_WarMen);
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Wars].pStMale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Wars].pStMale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Wars].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Wars].pStMale->SetTextColor(0xffff0000);
			}
		}
		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_SunMen);	
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Suns].pStMale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Suns].pStMale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Suns].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Suns].pStMale->SetTextColor(0xffff0000);
			}
		}
		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_ManaMen);	
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Manas].pStMale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Manas].pStMale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Manas].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Manas].pStMale->SetTextColor(0xffff0000);
			}
		}
		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_PeaceMen);	
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType )>= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Peaces].pStMale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Peaces].pStMale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Peaces].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Peaces].pStMale->SetTextColor(0xffff0000);
			}
		}

		m_CampaignGods[EGT_Wars].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Wars].pStFemale->SetTextColor(0xffff0000);
		m_CampaignGods[EGT_Suns].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Suns].pStFemale->SetTextColor(0xffff0000);
		m_CampaignGods[EGT_Manas].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Manas].pStFemale->SetTextColor(0xffff0000);
		m_CampaignGods[EGT_Peaces].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Peaces].pStFemale->SetTextColor(0xffff0000);
	}
	else
	{
		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_WarWomen);	
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType )>= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Wars].pStFemale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Wars].pStFemale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Wars].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Wars].pStFemale->SetTextColor(0xffff0000);
			}
		}
		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_SunWomen);	
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Suns].pStFemale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Suns].pStFemale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Suns].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Suns].pStFemale->SetTextColor(0xffff0000);
			}
		}

		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_ManaWomen);	
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType) >= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Manas].pStFemale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Manas].pStFemale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Manas].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Manas].pStFemale->SetTextColor(0xffff0000);
			}
		}

		pClergyName = ClergyProtoData::Inst()->FindClergyProto((DWORD)ECC_PeaceWomen);
		if(P_VALID(pClergyName))
		{
			if(pClergyName->eAttType != ERA_Null && pLp->GetAttribute(pClergyName->eAttType)>= pClergyName->dwAttValue 
				&& pLp->GetAttribute(ERA_God_Godhead)>= pClergyName->nGodheadLimit && pLp->GetRoleLevel() >= pClergyName->nLevelLimit 
				&& (dwClassVal & pClergyName->dwClassLimit))
			{
				m_CampaignGods[EGT_Peaces].pStFemale->SetText(g_StrTable[_T("Campaign_Satisfy")]);
				m_CampaignGods[EGT_Peaces].pStFemale->SetTextColor(0xff00ff00);
			}
			else
			{
				m_CampaignGods[EGT_Peaces].pStFemale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
				m_CampaignGods[EGT_Peaces].pStFemale->SetTextColor(0xffff0000);
			}
		}

		m_CampaignGods[EGT_Wars].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Wars].pStMale->SetTextColor(0xffff0000);
		m_CampaignGods[EGT_Suns].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Suns].pStMale->SetTextColor(0xffff0000);
		m_CampaignGods[EGT_Manas].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Manas].pStMale->SetTextColor(0xffff0000);
		m_CampaignGods[EGT_Peaces].pStMale->SetText(g_StrTable[_T("Campaign_UnSatisfy")]);
		m_CampaignGods[EGT_Peaces].pStMale->SetTextColor(0xffff0000);
	}

}

void CampaignFrame::ShowFrameMsg()
{
	tagNC_CampaignInfo cmd;
	m_pSession->Send(&cmd);
	m_escapeTime = 0;
}

VOID CampaignFrame::Update()
{
	/*
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			m_pWnd->SetInvisible(TRUE);
			return;
		}
		if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) )
		{
			m_pWnd->SetInvisible(TRUE);
		}
	}
	*/
}

DWORD CampaignFrame::OnOpenCampaingResultFrame(tagGameEvent* pEvent)
{
	if (pEvent->strEventName == _T("OpenCampaingResultFrame"))
	{
		CampaignResultFrame *pFrame = (CampaignResultFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("CampaignResult") );
		if( !P_VALID(pFrame) )
		{
			pFrame = (CampaignResultFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("CampaignResult"), _T("CampaignResultFrame"), 0 );
			pFrame->AcquireInfo(ECGT_War);
			pFrame->SetInvisiable(FALSE);
		}	
		else
		{
			pFrame->AcquireInfo(ECGT_War);
			pFrame->SetInvisiable(FALSE);
		}
	}
	return 0;
}

