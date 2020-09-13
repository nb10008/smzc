#include "StdAfx.h"
#include "PulseFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_pulse.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\pulse_define.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "SkillMgr.h"
#include "IconCutStatic.h"
#include "ItemProtoData.h"
#include "CurrencyMgr.h"
#include "..\WorldDefine\currency_define.h"
#include "GUIEffectMgr.h"
#include "ItemButton.h"
#include "QuestMgr.h"

#define CUTSTATIC_PIECES 100

bool tagPulseStatic::IsContain(GUIWnd* pWnd)
{
	if(pWnd == (GUIWnd*)pICSpic || pWnd == (GUIWnd*)pStSelected)
		return true;

	return false;
}

bool tagPulseStatic::IsSeclected()
{	
	return !pStSelected->IsInvisible();
}
VOID tagPulseStatic::Selected()
{
	if(pStSelected->IsInvisible())
		pStSelected->SetInvisible(FALSE);
}

VOID tagPulseStatic::DeSelected()
{
	pStSelected->SetInvisible(TRUE);
}

VOID tagPulseStatic::SetScaleValue(DWORD dwScale,bool bSetCur /*= false*/)
{
	if(P_VALID(pICSpic))
	{
		if(bSetCur)
			pICSpic->SetCurrValue(dwScale);

		pICSpic->SetScaleValue(dwScale);
	}
		
}


PulseFrame::PulseFrame(void):m_trunk(this)
{
	m_pWnd = NULL;
}

PulseFrame::~PulseFrame(void)
{
}

BOOL PulseFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

 	m_data.LoadFromFile();
	PulseData::Inst()->LoadFromFile();
 	m_pCmdMgr->Register( "NS_ActivePulse",					(NETMSGPROC)m_trunk.sfp2(&PulseFrame::OnNS_ActivePulse),					_T("NS_ActivePulse"));
 	m_pCmdMgr->Register( "NS_PulseCanLearnNum",		(NETMSGPROC)m_trunk.sfp2(&PulseFrame::OnNS_PulseCanLearnNum),		_T("NS_PulseCanLearnNum"));
 
 	m_pFrameMgr->RegisterEventHandle(_T("OpenPulse"),        (FRAMEEVENTPROC)m_trunk.sfp1( &PulseFrame::OnOpenPulse));
	m_pFrameMgr->RegisterEventHandle(_T("ClosePulse"),        (FRAMEEVENTPROC)m_trunk.sfp1( &PulseFrame::OnClosePulse));
	m_pFrameMgr->RegisterEventHandle(_T("NotCheckDistance"),        (FRAMEEVENTPROC)m_trunk.sfp1( &PulseFrame::OnNotCheckDistance));
 
  	m_escapeTime = 0;
  	m_dwNPCID = 0;
  	m_ePage = EP_P_FIRE;
  	m_nCurPos = GT_INVALID;
	m_nCurrActiveTime = 0;
	IsCheckDistance=TRUE;

  	m_pWnd->SetInvisible(TRUE);
 	m_pPbFire->SetState(EGUIBS_PushDown);
	m_pStItem->RefreshItem();
	m_pStItemBack->SetInvisible(TRUE);
	m_pStItem2->RefreshItem();
	m_pStItemBack2->SetInvisible(TRUE);
	m_bCanSend = true;

	return TRUE;
}

BOOL PulseFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister( "NS_ActivePulse",							(NETMSGPROC)m_trunk.sfp2(&PulseFrame::OnNS_ActivePulse) );
	m_pCmdMgr->UnRegister( "NS_PulseCanLearnNum",				(NETMSGPROC)m_trunk.sfp2(&PulseFrame::OnNS_PulseCanLearnNum) );

	m_pFrameMgr->UnRegisterEventHandler(_T("OpenPulse"),          (FRAMEEVENTPROC)m_trunk.sfp1(&PulseFrame::OnOpenPulse));
	m_pFrameMgr->UnRegisterEventHandler(_T("ClosePulse"),          (FRAMEEVENTPROC)m_trunk.sfp1(&PulseFrame::OnClosePulse));
	m_pFrameMgr->UnRegisterEventHandler(_T("NotCheckDistance"),          (FRAMEEVENTPROC)m_trunk.sfp1(&PulseFrame::OnNotCheckDistance));

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	return bRet;
}

BOOL PulseFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\god_blood.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	TCHAR buffer[256];
	string sTempName;
	
	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood_win\\god_blood\\fire\\icon%d\\fill"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &ele, sTempName.c_str(), "ClassName", "IconCutStatic");
	}

	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood_win\\god_blood\\water\\icon%d\\fill"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &ele, sTempName.c_str(), "ClassName", "IconCutStatic");
	}

	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood_win\\god_blood\\lei\\icon%d\\fill"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &ele, sTempName.c_str(), "ClassName", "IconCutStatic");
	}

	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood_win\\god_blood\\wine\\icon%d\\fill"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &ele, sTempName.c_str(), "ClassName", "IconCutStatic");
	}
	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood_win\\god_blood\\element\\icon%d\\fill"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &ele, sTempName.c_str(), "ClassName", "IconCutStatic");
	}
	m_pGUI->ChangeXml(&ele, "god_blood_win\\god_blood\\back\\iconback\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "god_blood_win\\god_blood\\back\\iconback0\\icon", "ClassName", "ItemButton");


	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pBnActive =  (GUIButton*)m_pWnd->GetChild(_T("god_blood\\seebutt"));
	m_pBnClose	=  (GUIButton*)m_pWnd->GetChild(_T("god_blood\\closebutton"));

	m_pPbFire=  (GUIPushButton*)m_pWnd->GetChild(_T("god_blood\\bloodchoose\\blood0"));
	m_pPbWater=  (GUIPushButton*)m_pWnd->GetChild(_T("god_blood\\bloodchoose\\blood1"));
	m_pPbThunder=  (GUIPushButton*)m_pWnd->GetChild(_T("god_blood\\bloodchoose\\blood2"));
	m_pPbWind=  (GUIPushButton*)m_pWnd->GetChild(_T("god_blood\\bloodchoose\\blood3"));
	m_pPbElement=  (GUIPushButton*)m_pWnd->GetChild(_T("god_blood\\bloodchoose\\blood4"));


	m_pPcFire =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\fire"));
	m_pPcWater=  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\water"));
	m_pPcThunder=  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\lei"));
	m_pPcWind=  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\wine"));
	m_pPcElement=  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\element"));


	m_pStName =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\back\\skillname"));
	m_pStLevels =  (GUIStaticEx*)m_pWnd->GetChild(_T("god_blood\\back\\learn"));
	m_pStNextDesc =  (GUIStaticEx*)m_pWnd->GetChild(_T("god_blood\\back\\nexteffect"));
	m_pStGold =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\back\\goldnum"));
	m_pStSilver =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\back\\silvernum"));
	m_pStItem =  (ItemButton*)m_pWnd->GetChild(_T("god_blood\\back\\iconback\\icon"));
	m_pStItem2 = (ItemButton*)m_pWnd->GetChild(_T("god_blood\\back\\iconback0\\icon"));
	m_pStSuccessRate =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\back\\num"));

	m_pStItemBack=  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\back\\iconback"));
	m_pStItemBack2 =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\back\\iconback0"));

	m_pStCurrGold =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\goldnum"));
	m_pStCurrSilver =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\silvernum"));
	m_pSTodayAcitve =  (GUIStatic*)m_pWnd->GetChild(_T("god_blood\\num"));
	
	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood\\fire\\icon%d"), i);
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_stPulseFire[i].pICSpic = (IconCutStatic*)pTempWud->GetChild(_T("fill"));
		m_stPulseFire[i].pStSelected = (GUIStatic*)m_stPulseFire[i].pICSpic->GetChild(_T("choose"));
		m_stPulseFire[i].pICSpic->SetCutNums(CUTSTATIC_PIECES);
		m_stPulseFire[i].SetScaleValue(0,true);
	}

	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood\\water\\icon%d"), i);
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_stPulseWater[i].pICSpic = (IconCutStatic*)pTempWud->GetChild(_T("fill"));
		m_stPulseWater[i].pStSelected = (GUIStatic*)m_stPulseWater[i].pICSpic->GetChild(_T("choose"));
		m_stPulseWater[i].pICSpic->SetCutNums(CUTSTATIC_PIECES);
		m_stPulseWater[i].SetScaleValue(0,true);
	}

	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood\\lei\\icon%d"), i);
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_stPulseThunder[i].pICSpic = (IconCutStatic*)pTempWud->GetChild(_T("fill"));
		m_stPulseThunder[i].pStSelected = (GUIStatic*)m_stPulseThunder[i].pICSpic->GetChild(_T("choose"));
		m_stPulseThunder[i].pICSpic->SetCutNums(CUTSTATIC_PIECES);
		m_stPulseThunder[i].SetScaleValue(0,true);
	}

	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood\\wine\\icon%d"), i);
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_stPulseWind[i].pICSpic = (IconCutStatic*)pTempWud->GetChild(_T("fill"));
		m_stPulseWind[i].pStSelected = (GUIStatic*)m_stPulseWind[i].pICSpic->GetChild(_T("choose"));
		m_stPulseWind[i].pICSpic->SetCutNums(CUTSTATIC_PIECES);
		m_stPulseWind[i].SetScaleValue(0,true);
	}

	for (int i =0; i<MAX_PULSE_PERPAGE;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_blood\\element\\icon%d"), i);
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_stPulseElement[i].pICSpic = (IconCutStatic*)pTempWud->GetChild(_T("fill"));
		m_stPulseElement[i].pStSelected = (GUIStatic*)m_stPulseElement[i].pICSpic->GetChild(_T("choose"));
		m_stPulseElement[i].pICSpic->SetCutNums(CUTSTATIC_PIECES);
		m_stPulseElement[i].SetScaleValue(0,true);
	}

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&PulseFrame::EventHandler));

	return TRUE;
}

BOOL PulseFrame::EscCancel()
{
 	if (m_pWnd->IsInvisible())
 	{
 		return FALSE;
 	} 
 	else
 	{
 		m_pWnd->SetInvisible(TRUE);
		m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Pulse_UI"), this ) );
 		return TRUE;
 	}
	return FALSE;
}

VOID PulseFrame::Update()
{
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			if(P_VALID(m_pWnd))
				m_pWnd->SetInvisible(TRUE);
			return;
		}
		if(IsCheckDistance)//当IsCheckDistance为真时才检测距离
		{
			if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
			{
				if(P_VALID(m_pWnd))
					m_pWnd->SetInvisible(TRUE);
			}
		}
		
	}
}

DWORD PulseFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBnClose)
			{
				m_pWnd->SetInvisible(TRUE);
				m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Pulse_UI"), this ) );
			}
			else if (pWnd == m_pBnActive)
			{
				ActivePulse();
			}

		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pPbFire)
			{	
				m_nCurPos = GT_INVALID;
				m_ePage = EP_P_FIRE;
				ClearSelectedMark();
				m_pPcFire->SetInvisible(FALSE);
				m_pPcWater->SetInvisible(TRUE);
				m_pPcThunder->SetInvisible(TRUE);
				m_pPcWind->SetInvisible(TRUE);
				m_pPcElement->SetInvisible(TRUE);
				
				UpdateDesc();
			}
			else if (pWnd == m_pPbWater)
			{
				m_nCurPos = GT_INVALID;
				m_ePage = EP_P_WATER;
				ClearSelectedMark();
				m_pPcFire->SetInvisible(TRUE);
				m_pPcWater->SetInvisible(FALSE);
				m_pPcThunder->SetInvisible(TRUE);
				m_pPcWind->SetInvisible(TRUE);
				m_pPcElement->SetInvisible(TRUE);
				
				UpdateDesc();
			}
			else if (pWnd == m_pPbThunder)
			{
				m_nCurPos = GT_INVALID;
				m_ePage = EP_P_THUNDER;
				ClearSelectedMark();
				m_pPcFire->SetInvisible(TRUE);
				m_pPcWater->SetInvisible(TRUE);
				m_pPcThunder->SetInvisible(FALSE);
				m_pPcWind->SetInvisible(TRUE);
				m_pPcElement->SetInvisible(TRUE);

				UpdateDesc();
			}
			else if (pWnd == m_pPbWind)
			{
				m_nCurPos = GT_INVALID;
				m_ePage = EP_P_WIND;
				ClearSelectedMark();
				m_pPcFire->SetInvisible(TRUE);
				m_pPcWater->SetInvisible(TRUE);
				m_pPcThunder->SetInvisible(TRUE);
				m_pPcWind->SetInvisible(FALSE);
				m_pPcElement->SetInvisible(TRUE);

				UpdateDesc();
			}
			else if (pWnd == m_pPbElement)
			{
				m_nCurPos = GT_INVALID;
				m_ePage = EP_P_ELEMENT;
				ClearSelectedMark();
				m_pPcFire->SetInvisible(TRUE);
				m_pPcWater->SetInvisible(TRUE);
				m_pPcThunder->SetInvisible(TRUE);
				m_pPcWind->SetInvisible(TRUE);
				m_pPcElement->SetInvisible(FALSE);

				UpdateDesc();
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (pWnd == m_pStItem)
			{
				tagPulseClientProto* pProto = NULL;
				if (EP_P_FIRE == m_ePage )
					pProto = m_data.FindFirePulseByPos(m_nCurPos);
				else if(EP_P_WATER == m_ePage)
					pProto = m_data.FindWaterPulseByPos(m_nCurPos);
				else if(EP_P_THUNDER == m_ePage)
					pProto = m_data.FindThunderPulseByPos(m_nCurPos);
				else if(EP_P_WIND == m_ePage)
					pProto = m_data.FindWindPulseByPos(m_nCurPos);
				else if(EP_P_ELEMENT == m_ePage)
					pProto = m_data.FindElementPulseByPos(m_nCurPos);

				if (P_VALID(pProto))
				{
					const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
					DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel : pProto->dwSkillID * 100 + 1;

					tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);
					if (P_VALID(pProto2))
					{
						const tagItemProto* pProto3 = ItemProtoData::Inst()->FindItemProto(pProto2->dwItemID);
						if (P_VALID(pProto3))
						{
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pProto2->dwItemID,pProto3->byQuality).c_str());
						}
					}
				}
				break;
			}
			else if(pWnd == m_pStItem2)
			{
				tagPulseClientProto* pProto = NULL;
				if (EP_P_FIRE == m_ePage )
					pProto = m_data.FindFirePulseByPos(m_nCurPos);
				else if(EP_P_WATER == m_ePage)
					pProto = m_data.FindWaterPulseByPos(m_nCurPos);
				else if(EP_P_THUNDER == m_ePage)
					pProto = m_data.FindThunderPulseByPos(m_nCurPos);
				else if(EP_P_WIND == m_ePage)
					pProto = m_data.FindWindPulseByPos(m_nCurPos);
				else if(EP_P_ELEMENT == m_ePage)
					pProto = m_data.FindElementPulseByPos(m_nCurPos);

				if (P_VALID(pProto))
				{
					const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
					DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel : pProto->dwSkillID * 100 + 1;

					tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);
					if (P_VALID(pProto2))
					{
						const tagItemProto* pProto3 = ItemProtoData::Inst()->FindItemProto(pProto2->dwItemID);
						if (P_VALID(pProto3))
						{
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pProto2->dwItemID2,pProto3->byQuality).c_str());
						}
					}
				}
				break;
			}
			
			if (EP_P_FIRE == m_ePage )
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseFire[i].IsContain(pWnd))
					{
						tagPulseClientProto* pProto = m_data.FindFirePulseByPos(i);
						if (P_VALID(pProto))
						{
							const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
							DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel : pProto->dwSkillID * 100 + 1;

							tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);
							if (P_VALID(pProto2))
							{
								tstring strTip = ToolTipCreator::Inst()->GetPulseTips(pProto2->dwId);
								ShowTip(pWnd, strTip.c_str());
							}
							
						}

						return 0;
					}
				}

			}
			else if (EP_P_WATER == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseWater[i].IsContain(pWnd))
					{
						tagPulseClientProto* pProto = m_data.FindWaterPulseByPos(i);
						if (P_VALID(pProto))
						{
							const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
							DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel : pProto->dwSkillID * 100 + 1;

							tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);
							if (P_VALID(pProto2))
							{
								tstring strTip = ToolTipCreator::Inst()->GetPulseTips(pProto2->dwId);
								ShowTip(pWnd, strTip.c_str());
							}

						}

						return 0;
					}
				}
			}
			else if (EP_P_THUNDER == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseThunder[i].IsContain(pWnd))
					{
						tagPulseClientProto* pProto = m_data.FindThunderPulseByPos(i);
						if (P_VALID(pProto))
						{
							const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
							DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel : pProto->dwSkillID * 100 + 1;

							tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);
							if (P_VALID(pProto2))
							{
								tstring strTip = ToolTipCreator::Inst()->GetPulseTips(pProto2->dwId);
								ShowTip(pWnd, strTip.c_str());
							}

						}

						return 0;
					}
				}
			}
			else if (EP_P_WIND == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseWind[i].IsContain(pWnd))
					{
						tagPulseClientProto* pProto = m_data.FindWindPulseByPos(i);
						if (P_VALID(pProto))
						{
							const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
							DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel : pProto->dwSkillID * 100 + 1;

							tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);
							if (P_VALID(pProto2))
							{
								tstring strTip = ToolTipCreator::Inst()->GetPulseTips(pProto2->dwId);
								ShowTip(pWnd, strTip.c_str());
							}

						}

						return 0;
					}
				}
			}
			else if (EP_P_ELEMENT == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseElement[i].IsContain(pWnd))
					{
						tagPulseClientProto* pProto = m_data.FindElementPulseByPos(i);
						if (P_VALID(pProto))
						{
							const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
							DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel : pProto->dwSkillID * 100 + 1;

							tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);
							if (P_VALID(pProto2))
							{
								tstring strTip = ToolTipCreator::Inst()->GetPulseTips(pProto2->dwId);
								ShowTip(pWnd, strTip.c_str());
							}

						}

						return 0;
					}
				}
			}
		}
		break;
	case EGUIE_DragStart:
	case EGUIE_Drag:
		{
			if (EP_P_FIRE == m_ePage )
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseFire[i].IsContain(pWnd))
					{
						if(!CanClick(i))	return 0;

						m_nCurPos = i;

						if(!m_stPulseFire[i].pStSelected->IsInvisible() && m_stPulseFire[i].pStSelected == pWnd)
							ActivePulse();
						else
						{
							ClearSelectedMark();
							m_stPulseFire[i].Selected();
						}
							
						UpdateDesc();
						return 0;
					}
				}

			}
			else if (EP_P_WATER == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseWater[i].IsContain(pWnd))
					{
						if(!CanClick(i))	return 0;

						m_nCurPos = i;

						if(!m_stPulseWater[i].pStSelected->IsInvisible() && m_stPulseWater[i].pStSelected == pWnd)
							ActivePulse();
						else
						{
							ClearSelectedMark();
							m_stPulseWater[i].Selected();
						}

						UpdateDesc();
						return 0;
					}
				}
			}
			else if (EP_P_THUNDER == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseThunder[i].IsContain(pWnd))
					{
						if(!CanClick(i))	return 0;

						m_nCurPos = i;

						if(!m_stPulseThunder[i].pStSelected->IsInvisible() && m_stPulseThunder[i].pStSelected == pWnd)
							ActivePulse();
						else
						{
							ClearSelectedMark();
							m_stPulseThunder[i].Selected();
						}

						UpdateDesc();
						return 0;
					}
				}
			}
			else if (EP_P_WIND == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseWind[i].IsContain(pWnd))
					{
						if(!CanClick(i))	return 0;

						m_nCurPos = i;

						if(!m_stPulseWind[i].pStSelected->IsInvisible() && m_stPulseWind[i].pStSelected == pWnd)
							ActivePulse();
						else
						{
							ClearSelectedMark();
							m_stPulseWind[i].Selected();
						}

						UpdateDesc();
						return 0;
					}
				}
			}
			else if (EP_P_ELEMENT == m_ePage)
			{
				for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
				{
					if (m_stPulseElement[i].IsContain(pWnd))
					{
						if(!CanClick(i))	return 0;

						m_nCurPos = i;

						if(!m_stPulseElement[i].pStSelected->IsInvisible() && m_stPulseElement[i].pStSelected == pWnd)
							ActivePulse();
						else
						{
							ClearSelectedMark();
							m_stPulseElement[i].Selected();
						}

						UpdateDesc();
						return 0;
					}
				}
			}
		}
		break;
	}
	return 0;
}


DWORD PulseFrame::OnNS_ActivePulse( tagNS_ActivePulse* pMsg, DWORD dwParam )
{
	m_bCanSend = true;
	m_pBnActive->SetEnable(true);

	if ( E_Pulse_Success == pMsg->dwErrorCode)
	{
		m_nCurrActiveTime = pMsg->nLearnTime;
		UpdateUI();

		GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
		GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui013"),pDesktop->GetView(),pDesktop->GetSize(),GUIEffectMgr::EET_EnforcSucess);

		return 0;
	}

	switch(pMsg->dwErrorCode)
	{
	case E_Pulse_NoItem:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Pulse_NoItem")]);
		break;
	case E_Pulse_NoMoney:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Pulse_NoMoeny")]);
		break;
	case E_Pulse_LessLevel:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_Lesslevel")]);
		break;
	case E_Pulse_NoPerSkill:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_NoPerSkill")]);
		break;
	case E_Pulse_LessGodLevel:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_LessGodLevel")]);
		break;
	case E_Pulse_MaxLevel:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Pulse_max_level")]);
		break;
	case E_Pulse_NoProto:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_NoProto")]);
		break;
	case E_Pulse_MaxTimePerDay:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_MaxTimePerDay")]);
			m_nCurrActiveTime = pMsg->nLearnTime;
			UpdateUI();
		}
		break;
	case E_Pulse_Fail:
		{
			m_nCurrActiveTime = pMsg->nLearnTime;
			UpdateUI();

			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_Fail")]);
			GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
			GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui014"),pDesktop->GetView(),pDesktop->GetSize(),GUIEffectMgr::EET_EnforcFail);
		}
		break;
	}
	return 0;
}

VOID PulseFrame::ActivePulse()
{
	if(m_nCurPos== GT_INVALID)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Pulse_choose_noone")]);
		return;
	}

	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(plp))	return;

	if(plp->GetRoleLevel() < MIN_PULSE_LEVEL)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_Lesslevel")]);
		return;
	}

	if (m_nCurrActiveTime >= MAX_PULSE_TIMEONEDAY)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_MaxTimePerDay")]);
		return;
	}

	tagPulseClientProto* pProto = NULL;
	if (EP_P_FIRE == m_ePage )
		pProto = m_data.FindFirePulseByPos(m_nCurPos);
	else if(EP_P_WATER == m_ePage)
		pProto = m_data.FindWaterPulseByPos(m_nCurPos);
	else if(EP_P_THUNDER == m_ePage)
		pProto = m_data.FindThunderPulseByPos(m_nCurPos);
	else if(EP_P_WIND == m_ePage)
		pProto = m_data.FindWindPulseByPos(m_nCurPos);
	else if(EP_P_ELEMENT == m_ePage)
		pProto = m_data.FindElementPulseByPos(m_nCurPos);

	if (P_VALID(pProto))
	{
		const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
		if(P_VALID(pData) && pData->nLevel >= pProto->dwMaxLevel)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Pulse_max_level")]);
			return;
		}

		DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel +1 : pProto->dwSkillID * 100 + 1;

		tagPulseClientProto* pPulseProto = m_data.FindPulseBySkillID(dwSkillTypeID);
		if (P_VALID(pPulseProto))
		{
			if (plp->GetRoleLevel() < pPulseProto->dwLevelLock)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_Lesslevel")]);
				return;
			}

			if (plp->GetAttribute(ERA_God_Godhead) < pPulseProto->dwGodLock)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_LessGodLevel")]);
				return;
			}

			if(m_bCanSend)
			{
				tagNC_ActivePulse cmd;
				cmd.dwPulseID = pPulseProto->dwId;
				m_pSession->Send(&cmd);

				m_bCanSend = false;
				m_pBnActive->SetEnable(false);
			}

		}
	}

	

}

VOID PulseFrame::UpdateUI(bool bfirst/* = false*/)
{
	for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
	{
		tagPulseClientProto* pProto = m_data.FindFirePulseByPos(i);
		if (P_VALID(pProto))
		{
			const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
			INT nLevel = P_VALID(pData) ? pData->nLevel : 0;
			
			m_stPulseFire[i].SetScaleValue(nLevel*CUTSTATIC_PIECES/pProto->dwMaxLevel,bfirst);
		}



		tagPulseClientProto* pProto2 = m_data.FindWaterPulseByPos(i);
		if (P_VALID(pProto2))
		{
			const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto2->dwSkillID);
			INT nLevel = P_VALID(pData) ? pData->nLevel : 0;

			m_stPulseWater[i].SetScaleValue(nLevel*CUTSTATIC_PIECES/pProto2->dwMaxLevel,bfirst);
		}

		tagPulseClientProto* pProto3 = m_data.FindThunderPulseByPos(i);
		if (P_VALID(pProto3))
		{
			const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto3->dwSkillID);
			INT nLevel = P_VALID(pData) ? pData->nLevel : 0;

			m_stPulseThunder[i].SetScaleValue(nLevel*CUTSTATIC_PIECES/pProto3->dwMaxLevel,bfirst);
		}

		tagPulseClientProto* pProto4 = m_data.FindWindPulseByPos(i);
		if (P_VALID(pProto4))
		{
			const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto4->dwSkillID);
			INT nLevel = P_VALID(pData) ? pData->nLevel : 0;

			m_stPulseWind[i].SetScaleValue(nLevel*CUTSTATIC_PIECES/pProto4->dwMaxLevel,bfirst);
		}

		tagPulseClientProto* pProto5 = m_data.FindElementPulseByPos(i);
		if (P_VALID(pProto5))
		{
			const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto5->dwSkillID);
			INT nLevel = P_VALID(pData) ? pData->nLevel : 0;

			m_stPulseElement[i].SetScaleValue(nLevel*CUTSTATIC_PIECES/pProto5->dwMaxLevel,bfirst);
		}

		
		
	}

	UpdateDesc();
}

DWORD PulseFrame::OnNS_PulseCanLearnNum( tagNS_PulseCanLearnNum* pMsg, DWORD dwParam )
{
	m_nCurrActiveTime = pMsg->dwNum;
	TCHAR szBuff[X_LONG_NAME];
	_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d / %d"), m_nCurrActiveTime, MAX_PULSE_TIMEONEDAY);
	if (P_VALID(m_pSTodayAcitve))
		m_pSTodayAcitve->SetText(szBuff);
	return 0;
}

DWORD PulseFrame::OnOpenPulse(tagGameEvent* pEvent)
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(plp))	return 0;

	if(plp->GetRoleLevel() < MIN_PULSE_LEVEL)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Pulse_Lesslevel")]);
		return 0;
	}
	ShowFrame(TRUE);
	return 0;
}

DWORD PulseFrame::OnClosePulse(tagGameEvent* pEvent)
{
	
	ShowFrame(FALSE);
	return 0;
}

DWORD PulseFrame::OnNotCheckDistance(tagGameEvent* pEvent)
{

	IsCheckDistance=FALSE;
	return 0;
}

VOID PulseFrame::UpdateDesc()
{
	m_pStItem->RefreshItem();
	m_pStItem2->RefreshItem();
	m_pStItemBack->SetInvisible(TRUE);
	m_pStItemBack2->SetInvisible(TRUE);
	TCHAR szBuff[X_LONG_NAME];
	tagPulseClientProto* pProto = NULL;
	if (EP_P_FIRE == m_ePage )
		pProto = m_data.FindFirePulseByPos(m_nCurPos);
	else if(EP_P_WATER == m_ePage)
		pProto = m_data.FindWaterPulseByPos(m_nCurPos);
	else if(EP_P_THUNDER == m_ePage)
		pProto = m_data.FindThunderPulseByPos(m_nCurPos);
	else if(EP_P_WIND == m_ePage)
		pProto = m_data.FindWindPulseByPos(m_nCurPos);
	else if(EP_P_ELEMENT == m_ePage)
		pProto = m_data.FindElementPulseByPos(m_nCurPos);

	if (P_VALID(pProto))
	{
		const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
		DWORD dwSkillTypeID = P_VALID(pData) ? pProto->dwSkillID * 100 + pData->nLevel + 1 : pProto->dwSkillID * 100 + 1;

		tagPulseClientProto* pProto2 = m_data.FindPulseBySkillID(dwSkillTypeID);		//下一级别
		tagPulseClientProto* pProto5 = NULL;
		if(dwSkillTypeID % 100 != 0)
		{
			DWORD dwCurSkillID = dwSkillTypeID - 1;
			pProto5 = m_data.FindPulseBySkillID(dwCurSkillID);	//当前级别
		}
		if (P_VALID(pProto2))
		{
			m_pStName->SetText(pProto2->szName);
			
			_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d / %d"),(P_VALID(pData) ? pData->nLevel : 0) , pProto2->dwMaxLevel);
			m_pStLevels->SetText(szBuff);

			_stprintf(szBuff, _T("%s%s"), g_StrTable[_T("NextLevel")], pProto2->szSkillDesc);
			m_pStNextDesc->SetText(szBuff);

			

			DWORD dwID = dwSkillTypeID;
			tagPulseClientProto* pProto4 = m_data.FindPulseBySkillID(dwID);
			if (P_VALID(pProto4))
			{
				_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d%%"),pProto4->dwSuccessRate/100 >0 ? pProto4->dwSuccessRate/100 : 1);
				m_pStSuccessRate->SetText(szBuff);

				_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),pProto4->dwMoney/10000);
				m_pStGold->SetText(szBuff);

				_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),pProto4->dwMoney%10000);
				m_pStSilver->SetText(szBuff);
			}
			else
			{
				m_pStSuccessRate->SetText(_T(""));
				m_pStGold->SetText(_T(""));
				m_pStSilver->SetText(_T(""));
			}
				
			

			const tagItemProto* pProto3 = ItemProtoData::Inst()->FindItemProto(pProto2->dwItemID);
			if (P_VALID(pProto3))
			{
				m_pStItem->RefreshItem(pProto2->dwItemID,pProto2->dwItemNum,pProto3->byQuality);
				m_pStItemBack->SetInvisible(FALSE);
			}
			else
			{
				m_pStItem->RefreshItem();
				m_pStItemBack->SetInvisible(TRUE);
			}

			const tagItemProto* pProtoTemp = ItemProtoData::Inst()->FindItemProto(pProto2->dwItemID2);
			if (P_VALID(pProtoTemp))
			{
				m_pStItem2->RefreshItem(pProto2->dwItemID2,pProto2->dwItemNum2,pProto3->byQuality);
				m_pStItemBack2->SetInvisible(FALSE);
			}
			else
			{
				m_pStItem2->RefreshItem();
				m_pStItemBack2->SetInvisible(TRUE);
			}

		}
		else if(P_VALID(pProto5))
		{
			m_pStName->SetText(pProto5->szName);

			_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d / %d"),(P_VALID(pData) ? pData->nLevel : 0) , pProto5->dwMaxLevel);
			m_pStLevels->SetText(szBuff);

			m_pStNextDesc->SetText(pProto5->szNextSkillDesc);

			m_pStItem->RefreshItem();
			m_pStItem2->RefreshItem();
			m_pStItemBack->SetInvisible(TRUE);
			m_pStItemBack2->SetInvisible(TRUE);

			m_pStSuccessRate->SetText(_T(""));
			m_pStGold->SetText(_T(""));
			m_pStSilver->SetText(_T(""));
		}

	}
	else
	{
		m_pStName->SetText(_T(""));
		m_pStLevels->SetText(_T(""));
		m_pStNextDesc->SetText(_T(""));
		m_pStGold->SetText(_T(""));
		m_pStSilver->SetText(_T(""));
		m_pStItem->RefreshItem();
		m_pStItem2->RefreshItem();
		m_pStItemBack->SetInvisible(TRUE);
		m_pStItemBack2->SetInvisible(TRUE);
		m_pStSuccessRate->SetText(_T(""));

	}


	INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
	int nGold = MSilver2DBGold(n64Silver);
	int nSilver = MSilver2DBSilver(n64Silver);
	_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), nGold);
	m_pStCurrGold->SetText(szBuff);
	_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), nSilver);
	m_pStCurrSilver->SetText(szBuff);

	_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d / %d"), m_nCurrActiveTime, MAX_PULSE_TIMEONEDAY);
	m_pSTodayAcitve->SetText(szBuff);

}

VOID PulseFrame::ClearSelectedMark()
{
	if (m_ePage == EP_P_FIRE)
	{
		for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
		{
			m_stPulseFire[i].DeSelected();		
		}
	}
	else if (m_ePage == EP_P_WATER)
	{
		for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
		{
			m_stPulseWater[i].DeSelected();		
		}
	}
	else if (m_ePage == EP_P_THUNDER)
	{
		for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
		{
			m_stPulseThunder[i].DeSelected();		
		}
	}
	else if (m_ePage == EP_P_WIND)
	{
		for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
		{
			m_stPulseWind[i].DeSelected();		
		}
	}
	else if (m_ePage == EP_P_ELEMENT)
	{
		for (int i = 0; i <MAX_PULSE_PERPAGE ; i++)
		{
			m_stPulseElement[i].DeSelected();		
		}
	}
		
}

bool PulseFrame::CanClick(int pos)
{
	if(pos <0 || pos >= MAX_PULSE_PERPAGE )	return false;
	tagPulseClientProto* pProto = NULL;
	if (EP_P_FIRE == m_ePage )
		pProto = m_data.FindFirePulseByPos(pos);
	else if(EP_P_WATER == m_ePage)
		pProto = m_data.FindWaterPulseByPos(pos);
	else if(EP_P_THUNDER == m_ePage)
		pProto = m_data.FindThunderPulseByPos(pos);
	else if(EP_P_WIND == m_ePage)
		pProto = m_data.FindWindPulseByPos(pos);
	else if(EP_P_ELEMENT == m_ePage)
		pProto = m_data.FindElementPulseByPos(pos);

	if (P_VALID(pProto))
	{
		const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
		if(P_VALID(pData) )
			return true;
		

		DWORD dwSkillTypeID = pProto->dwSkillID * 100 + 1;

		tagPulseClientProto* pPulseProto = m_data.FindPulseBySkillID(dwSkillTypeID);
		if (P_VALID(pPulseProto))
		{
			if( GT_INVALID != pPulseProto->dwPerSkill1)
			{
				const tagSkillData* pPreSkill = SkillMgr::Inst()->GetSkillData(pPulseProto->dwPerSkill1/100);
				if( !P_VALID(pPreSkill))
					return false;

				if(pPreSkill->nLevel< pPulseProto->dwPerSkill1%100)
					return false;
			}

			if( GT_INVALID != pPulseProto->dwPerSkill2)
			{
				const tagSkillData* pPreSkill = SkillMgr::Inst()->GetSkillData(pPulseProto->dwPerSkill2/100);
				if( !P_VALID(pPreSkill))
					return false;

				if(pPreSkill->nLevel< pPulseProto->dwPerSkill2%100)
					return false;
			}

			if( GT_INVALID != pPulseProto->dwPerSkill3)
			{
				const tagSkillData* pPreSkill = SkillMgr::Inst()->GetSkillData(pPulseProto->dwPerSkill3/100);
				if( !P_VALID(pPreSkill))
					return false;

				if(pPreSkill->nLevel< pPulseProto->dwPerSkill3%100)
					return false;
			}

			return true;
		}
		else
			return false;
	}
	else
		return false;
}