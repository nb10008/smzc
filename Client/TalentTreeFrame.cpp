#include "stdafx.h"
#include "TalentTreeFrame.h"
#include "TalentFrame.h"
#include "..\WorldDefine\msg_talent.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "SkillMgr.h"
#include "GameClientDefine.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "TalentTree.h"
#include "RoleEvent.h"
#include "SkillEvent.h"
#include "GlintButton.h"
#include "BuyTalentFrame.h"
#include "ItemMgr.h"
#include "CombatSysUtil.h"
#include "SkillProtoData.h"

TalentTreeFrame::TalentTreeFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pStcAvail = NULL;
	m_pStcBidden = NULL;
	for (INT i=0; i<X_MAX_TALENT_PER_ROLE; ++i)
	{
		m_pPbnTalent[i]=NULL;
	}

	m_nCurBidden = GT_INVALID;
	m_eCurTalent = ETT_Null;
	m_nCurAvail = GT_INVALID;
	m_nCurTalentNum = GT_INVALID;
	m_pBtnCleanTalent = NULL;
}

TalentTreeFrame::~TalentTreeFrame()
{
	
}

BOOL TalentTreeFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPSkillEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPSkillEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPLevel"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPLevel));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPAttEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPTalentPoint"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPTalentPoint));
	m_pFrameMgr->RegisterEventHandle(_T("SkillEvent_ClearTalent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnEventClearTalent));
	m_pFrameMgr->RegisterEventHandle(_T("CleanTalentConfirm_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnMsgClearTalent));
	m_pFrameMgr->RegisterEventHandle(_T("tagRoleClassChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPClassEvent));
	
	return bRet;
}

BOOL TalentTreeFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	// 天赋树数据释放
	m_TalentTree.Destroy();

	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPSkillEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPSkillEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPLevel"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPLevel));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPAttEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPTalentPoint"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPTalentPoint));
	m_pFrameMgr->UnRegisterEventHandler(_T("SkillEvent_ClearTalent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnEventClearTalent));
	m_pFrameMgr->UnRegisterEventHandler(_T("CleanTalentConfirm_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnMsgClearTalent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagRoleClassChange"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TalentTreeFrame::OnUpdateLPClassEvent));

	return bRet;
}

BOOL TalentTreeFrame::ReloadUI()
{
	m_pWnd = ((TalentFrame*)m_pFather)->GetCaption();
	m_pWndLearnBack = ((TalentFrame*)m_pFather)->GetLeanBack();
	
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TalentTreeFrame::EventHandler));
	//m_pGUI->RegisterEventHandler(m_pWndLearnBack->GetFullName().c_str(), m_Trunk.sfp1(&TalentTreeFrame::EventHandler));

	m_pStcBidden				=	(GUIStatic*)m_pWndLearnBack->GetChild( _T("num0") );
	m_pStcAvail					=	(GUIStatic*)m_pWndLearnBack->GetChild( _T("num1") );	
	m_pBtnCleanTalent			=	(GUIButton*)m_pWndLearnBack->GetChild( _T("refresh") );

	
	//m_pBtnSimpleMode			=   (GUIPushButton*)m_pWnd->GetChild( _T("opt") );

	m_pPbnTalent[0]		=	(GUIPushButton*)m_pWnd->GetChild( _T("winopt0\\optbutt0") );
	m_pPbnTalent[1]		=	(GUIPushButton*)m_pWnd->GetChild( _T("winopt0\\optbutt1") );
	
	// 天赋树数据创建
	m_TalentTree.Create( m_pWnd->GetChild(_T("skillback\\skillttreewin\\skilltreebakpic")), m_pGUI );

	m_pPbnTalent[1]->SetInvisible(TRUE);
	return TRUE;
}

DWORD TalentTreeFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch ( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if(P_VALID(m_pBtnCleanTalent) && m_pBtnCleanTalent->GetID() == pEvent->dwWndID)
			{
				// 洗点
				if(P_VALID(GetObj("CleanTalentMsgBox")))
					KillObj("CleanTalentMsgBox");

				CreateObj("CleanTalentMsgBox", "MsgBox");
				TObjRef<MsgBox>("CleanTalentMsgBox")->Init(_T(""), g_StrTable[_T("CleanTalentConfirm")], _T("CleanTalentConfirm_Event"));
			}
			else if (_tcsnccmp(pWnd->GetName().c_str(), _T("talentleaf"), 10) == 0)
			{
				tagTalentLeaf* pLeaf = m_TalentTree.GetCurLeaf(pWnd->GetID());
				if(P_VALID(pLeaf))
				{
					if(pLeaf->bActive)
					{
						if(0 == pLeaf->nLearnLevel)
						{
							tagNC_LearnSkill m;
							m.dwSkillID = pLeaf->dwTalentLeaf;
							m.dwNPCID = GT_INVALID;
							m.n64ItemID = GT_INVALID;
							m_pSession->Send(&m);
						}
						else
						{
							tagNC_LevelUpSkill m;
							m.dwSkillID = pLeaf->dwTalentLeaf;
							m.dwNPCID = GT_INVALID;
							m.n64ItemID = GT_INVALID;
							m_pSession->Send(&m);
						}
					}
				}
			}			
		}
		break;
	case EGUIE_PushButtonDown:
		{
			//设置显示模式
			/*if( pWnd == m_pBtnSimpleMode )
			{
				m_TalentTree.SetSimpleDisplayMode( EGUIBS_PushDown == m_pBtnSimpleMode->GetState() );
			}*/

			//不是按下不处理
			if ( pEvent->dwParam1 != 1 )
				return 0;

			if( pWnd == m_pPbnTalent[0] )
			{
				m_eCurTalent = ETT_FreshHand;	
				m_TalentTree.SetCurPage(ETT_FreshHand);
			}
			else if(pWnd == m_pPbnTalent[1])
			{
				m_eCurTalent = SkillMgr::Inst()->GetMainTalentType();
				m_TalentTree.SetCurPage(m_eCurTalent);
			}
			else return 0;
			int nTalentCount = SkillMgr::Inst()->GetTalentPoint(m_eCurTalent );
			SetBidden( nTalentCount );			
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("talentleaf"), 10) == 0)
			{
				tagTalentLeaf* pLeaf = m_TalentTree.GetCurLeaf( pWnd->GetID() );
				if( P_VALID(pLeaf) )
				{
					/*if( m_pBtnSimpleMode->GetState() == EGUIBS_PushDown && 0 != pLeaf->dwIconCtrlId && !SkillMgr::Inst()->IsSkillLearned( pLeaf->dwIconCtrlId ) )
						break;*/

					DWORD skillID = pLeaf->dwTalentLeaf;
					if(skillID < 10000)
					{
						const tagSkillBoxProto* pSkillBox = SkillProtoData::Inst()->FindSkillBoxProto(skillID);
						int nIndex = 0, nUseIndex = GT_INVALID;
						for(; nIndex < tagSkillBoxProto::E_SBP_Size; nIndex++ )
						{
							if( pSkillBox->dwSkillIDs[nIndex] != 0 )
							{
								const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillData(pSkillBox->dwSkillIDs[nIndex]);
								if( P_VALID(pSkillData) && pSkillData->pProto->nWeaponLimit!=0 
									&& (pSkillData->pProto->nWeaponLimit==ItemMgr::Inst()->GetHandWeaponType()) )
								{
									nUseIndex = nIndex;
									break;
								}
							}
						}
						if(GT_INVALID == nUseIndex && nIndex == tagSkillBoxProto::E_SBP_Size)
							nUseIndex = 0;
						if(P_VALID(pSkillBox) && pSkillBox->bCanBeLearned)	
							skillID = pSkillBox->dwSkillIDs[nUseIndex];
					}
					tstring strTips;
					if(0 == pLeaf->nCurLevel)
					{
						strTips = ToolTipCreator::Inst()->GetTalentTips(MTransSkillTypeID(skillID, 1));
					}
					else
					{
						strTips = ToolTipCreator::Inst()->GetTalentTips(MTransSkillTypeID(skillID, pLeaf->nCurLevel));
					}
					ShowTip(pWnd, strTips.c_str());	
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	default :
		break;
	}
	return 0;
}

DWORD TalentTreeFrame::OnUpdateLPSkillEvent( tagUpdateLPSkillEvent* pGameEvent )
{
	m_TalentTree.UpdateTalentLeaf(pGameEvent->dwID, pGameEvent->bAdd);
	RefreshTextColor();
	return 0;
}

DWORD TalentTreeFrame::OnUpdateLPLevel( tagUpdateLPLevel* pGameEvent )
{
	m_TalentTree.OnRoleLevelUp(pGameEvent->nCurLevel, pGameEvent->nInc);
	return 0;
}

DWORD TalentTreeFrame::OnUpdateLPAttEvent( tagUpdateLPAttEvent* pGameEvent )
{
	if(ERA_TalentPoint == pGameEvent->eType)
	{
		INT nPoint = RoleMgr::Inst()->GetLocalPlayer()->GetAttribute(pGameEvent->eType);
		if(0 == nPoint && 0 != m_nCurBidden)
		{
			m_TalentTree.OnNoTalentPoint();
		}
		else if(0 != nPoint)
		{
			m_TalentTree.OnHaveTalentPoint();
		}
		SetAvail(nPoint);
	}
	return 0;
}

DWORD TalentTreeFrame::OnUpdateLPTalentPoint( tagUpdateLPTalentPoint* pGameEvent )
{
	m_TalentTree.OnTalentPointChange(pGameEvent->eType, pGameEvent->nPoint);
	if(MAX_BATTLE_TALENT_COUNT == SkillMgr::Inst()->GetBattleTalentCount())
	{
		m_TalentTree.OnNoTalentTypeCanChoose();
	}
	if(pGameEvent->eType == m_eCurTalent)
	{
		SetBidden(pGameEvent->nPoint);
	}
	m_nCurTalentNum = pGameEvent->nMaxTalent;

	RefreshTextColor();
	return 0;
}

VOID TalentTreeFrame::SetBidden( INT nValue )
{
	m_nCurBidden = nValue;

	TCHAR szTmp[X_SHORT_NAME]={0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
	m_pStcBidden->SetText(szTmp);
	m_pStcBidden->SetRefresh();
}

VOID TalentTreeFrame::SetAvail( INT nValue )
{
	TCHAR szTmp[X_SHORT_NAME]={0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nValue);
	m_pStcAvail->SetText(szTmp);
	m_pStcAvail->SetRefresh();
	m_nCurAvail = nValue;
}

VOID TalentTreeFrame::OnTalentTreeOpen()
{
	if(NULL == m_nCurAvail)
	{
		m_TalentTree.OnNoTalentPoint();
	}
    
	if(MAX_BATTLE_TALENT_COUNT == SkillMgr::Inst()->GetBattleTalentCount())
	{
		m_TalentTree.OnNoTalentTypeCanChoose();
	}

	EClassType classType = RoleMgr::Inst()->GetLocalPlayer()->GetPlayerClass();
	if (classType >= EV_Blade && classType <= EV_TianYi)
	{
		tstringstream ts;
		ts << _T("TalentName") << (SkillMgr::Inst()->GetMainTalentType() - ETT_Blade);
		m_pPbnTalent[1]->SetText(g_StrTable[ts.str().c_str()]);
		m_pPbnTalent[1]->SetInvisible(FALSE);
		m_pPbnTalent[1]->SetState(EGUIBS_PushDown);		
	}
	//没有职业只显示一页
	else
	{
		m_pPbnTalent[0]->SetState(EGUIBS_PushDown);	
		m_pPbnTalent[1]->SetInvisible(TRUE);
	}

	//m_pBtnSimpleMode->SetState( EGUIBS_PushDown );

	//将已经透过点的系的按钮变成绿色
	RefreshTextColor();
	INT nLevle = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	if( nLevle < 20 )
		m_pBtnCleanTalent->SetEnable(FALSE);
	else
		m_pBtnCleanTalent->SetEnable(TRUE);
}

DWORD TalentTreeFrame::OnEventClearTalent( tagGameEvent* pGameEvent )
{
	m_TalentTree.OnClearAllTalent();
	RefreshTextColor();

	return 0;
}

void TalentTreeFrame::RefreshTextColor()
{
	for( int i = 0; i < X_MAX_TALENT_PER_ROLE; i++ )
	{
		m_pPbnTalent[i]->SetTextColor( 0xFFFFF7E0 );
		m_pPbnTalent[i]->SetPushTextColor( 0xFFFFF7E0 );
		m_pPbnTalent[i]->SetMoveTextColor( 0xFFFFF7E0 );
	}
}

DWORD TalentTreeFrame::OnMsgClearTalent(tagMsgBoxEvent* pGameEvent)
{
	if(pGameEvent->eResult == MBF_OK)
	{
		// 使用道具
		INT64 n64ID = ItemMgr::Inst()->GetSpecfuncItemID(EISF_RemoveTalentPt);
		if(GT_VALID(n64ID))
		{
			tagNC_ClearTalent m;
			m.n64ItemID = n64ID;
			m.eType = SkillMgr::Inst()->GetMainTalentType();
			m_pSession->Send(&m);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CleanTalent_Err")]);	
		}
	}
	return 0;
}

DWORD TalentTreeFrame::OnUpdateLPClassEvent(tagRoleClassChange* pGameEvent)
{
	EClassType classType = RoleMgr::Inst()->GetLocalPlayer()->GetPlayerClass();
	if (classType >= EV_Blade && classType <= EV_TianYi)
	{
		tstringstream ts;
		ts << _T("TalentName") << (SkillMgr::Inst()->GetMainTalentType() - ETT_Blade);
		m_pPbnTalent[1]->SetText(g_StrTable[ts.str().c_str()]);
		m_pPbnTalent[1]->SetInvisible(FALSE);
		m_pPbnTalent[1]->SetState(EGUIBS_PushDown);		
	}
	//没有职业只显示一页
	else
	{
		m_pPbnTalent[0]->SetState(EGUIBS_PushDown);	
		m_pPbnTalent[1]->SetInvisible(TRUE);
	}
	m_TalentTree.OnRoleClassChange(pGameEvent->eClass, pGameEvent->eOld);
	return 0;
}