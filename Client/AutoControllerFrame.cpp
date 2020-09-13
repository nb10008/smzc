#include "StdAfx.h"
#include "AutoControllerFrame.h"
#include "LocalPlayer.h"
#include "ItemEvent.h"
#include "SkillMgr.h"
#include "ItemMgr.h"
#include "CombatEvent.h"
#include "WorldPickerFrame.h"
#include "Item.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_AutoFight.h"
#include "..\WorldDefine\specfunc_define.h"
#include "..\WorldDefine\AutoFight_define.h"
#include "ItemEvent.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CombatActionChecker.h"
#include "PlayerNameTab.h"
#include "SkillButton.h"
#include "SkillEvent.h"
#include "WorldPickerFrame.h"
#include "DragBtn.h"
#include "ItemFrame.h"
#include "CombatEvent.h"
#include "Container.h"
#include "ItemTimerMgr.h"
#include "ServerTime.h"

class CmpMonsters
{
public:
	Vector3 refPos;
	bool operator()( Role *pRole1,Role *pRole2 )
	{
		float dist1 = Vec3DistSq( refPos, pRole1->GetPos() );
		float dist2 = Vec3DistSq( refPos, pRole2->GetPos() );
		return dist1 < dist2;
	}
};

CONST INT	SELECT_RANGE = 60;
CONST DWORD MIN_BLOCK_TIME = 5000;
CONST DWORD MAX_IDLE_TIME = 5000;
CONST int	ITEM_BUTTON_COUNT = 2;
CONST int	SKILL_BUTTON_COUNT = 6;
CONST DWORD	PATHFING_TIME = 2000;

AutoControllerFrame::AutoControllerFrame(void):
m_Trunk(this),
eState(EAS_NONE),
m_bAutoSelectMonster(true),
m_bAutoBaoQi(true),
m_bPointStand(false),
m_LockTargetID(0),
m_dwCurrentSkillTime(0),
m_bFristOpen(true),
m_n64ID(0),
m_dwTimeSpace(0),
m_dwTargetHP(0),
m_dwIdleTime(0),
m_dwTimeEclipse(1000),
m_dwTimeMark(0),
m_dwUseExp(0),
m_dwLeftTime(0)
{
	m_staticPoint	= Vector3(0,0,0);
	m_LastFightPoint = Vector3(0,0,0);
}

AutoControllerFrame::~AutoControllerFrame(void)
{
}

BOOL AutoControllerFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam );


	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&AutoControllerFrame::OnWnd_AutoCombat));
	m_pGUI->RegisterEventHandler( m_pWndSetting->GetFullName().c_str(),   m_Trunk.sfp1(&AutoControllerFrame::OnWnd_Setting));
	m_pGUI->RegisterEventHandler( m_pWndMin->GetFullName().c_str(),   m_Trunk.sfp1(&AutoControllerFrame::OnWnd_Min));

	m_pFrameMgr->RegisterEventHandle(_T("tagHitTargetEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnHitTargetEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPSkillEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnUpdateLPSkillEvent));	
	m_pFrameMgr->RegisterEventHandle( _T("tagSpecItemEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnOpenAutoHook));
	m_pFrameMgr->RegisterEventHandle(_T("tagShowDeadMenuEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnShowDeadMenuEvent));
	m_pCmdMgr->Register("NS_UseZhanDouFu",	(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_UseZhanDouFu),	_T("NS_UseZhanDouFu"));
	m_pCmdMgr->Register("NS_ZhanDouFuTimeOut",	(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_ZhanDouFuTimeOut),	_T("NS_ZhanDouFuTimeOut"));
	m_pCmdMgr->Register("NS_SyncAutoFightState",	(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_SyncAutoFightState),	_T("NS_SyncAutoFightState"));
	m_pCmdMgr->Register("NS_RoleExpChange",		(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_RoleExpChange),		_T("NS_RoleExpChange"));

	m_pFrameMgr->RegisterEventHandle( _T("tagRolePickEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnRolePickEvent));

	m_dwTimeEclipse = m_pSession->GetAutoFightTimeSpace();

	return bRet;
}

BOOL AutoControllerFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	SaveSettingToLocalFile();
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPSkillEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnUpdateLPSkillEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagSpecItemEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnOpenAutoHook));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagShowDeadMenuEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnShowDeadMenuEvent));
	m_pCmdMgr->UnRegister("NS_UseZhanDouFu",			(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_UseZhanDouFu));
	m_pCmdMgr->UnRegister("NS_ZhanDouFuTimeOut",			(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_ZhanDouFuTimeOut));
	m_pCmdMgr->UnRegister("NS_SyncAutoFightState",			(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_SyncAutoFightState));
	m_pCmdMgr->UnRegister("NS_RoleExpChange",		(NETMSGPROC)m_Trunk.sfp2(&AutoControllerFrame::OnNS_RoleExpChange));

	m_pFrameMgr->UnRegisterEventHandler( _T("tagRolePickEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnRolePickEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagHitTargetEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&AutoControllerFrame::OnHitTargetEvent));
	
	if(P_VALID(GetObj("ZhanDouFuTimeOut")))
		KillObj("ZhanDouFuTimeOut");

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	if (P_VALID(m_pWndSetting))
	{
		m_pGUI->AddToDestroyList(m_pWndSetting);
		m_pWndSetting = NULL;
	}
	if (P_VALID(m_pWndMin))
	{
		m_pGUI->AddToDestroyList(m_pWndMin);
		m_pWndMin = NULL;
	}
	return bRet;
}

BOOL AutoControllerFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\on_hook.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\skill0", "ClassName", "SkillButton");
	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\skill1", "ClassName", "SkillButton");
	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\skill2", "ClassName", "SkillButton");
	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\skill3", "ClassName", "SkillButton");
	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\skill4", "ClassName", "SkillButton");
	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\skill5", "ClassName", "SkillButton");

	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\item1", "ClassName", "ItemButton");
	m_pGUI->ChangeXml( &element, "on_hook_wnd\\on_hook_back\\item0", "ClassName", "ItemButton");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd )  )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	strPath = vEngine::g_strLocalPath + _T("\\ui\\onhook_setting.xml");
	m_pWndSetting	=	m_pGUI->CreateFromFile( "VFS_System", strPath.c_str());
	if( !P_VALID( m_pWndSetting )  )
	{
		m_pGUI->DestroyWnd(m_pWndSetting);
		m_pWndSetting = NULL;		
		return FALSE;
	}

	strPath = vEngine::g_strLocalPath + _T("\\ui\\onhook_fold.xml");
	m_pWndMin	=	m_pGUI->CreateFromFile( "VFS_System", strPath.c_str());
	if( !P_VALID( m_pWndMin )  )
	{
		m_pGUI->DestroyWnd(m_pWndMin);
		m_pWndMin = NULL;		
		return FALSE;
	}

	TCHAR tbuff[128];
	for (int i = 0; i < MAX_SKILL; ++i)
	{
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("on_hook_back\\skill%d"), i, i);
		m_pBtSkills[i]  =   static_cast<SkillButton*>(m_pWnd->GetChild(tbuff));          // 技能
	}
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		_sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("on_hook_back\\item%d"), i, i);
		m_pBtItems[i]  =   static_cast<ItemButton*>(m_pWnd->GetChild(tbuff));         // 物品
	}

	m_pBnAutoSetting     =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("on_hook_back\\setting_button")));                    // 设置
	m_pBnStartCombat     =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("on_hook_back\\fighting_button")));                    // 开始打架

	m_pBnMinSize		=   static_cast<GUIButton*>(m_pWnd->GetChild(_T("on_hook_back\\minimize_button")));                    
	m_pBnClose			=   static_cast<GUIButton*>(m_pWnd->GetChild(_T("on_hook_back\\close_button")));                

	m_pStLeftTime	=   static_cast<GUIButton*>(m_pWnd->GetChild(_T("state\\time")));
	m_pStStartTime	=   static_cast<GUIButton*>(m_pWnd->GetChild(_T("state\\time0")));
	m_pStExpHaveGot	=   static_cast<GUIButton*>(m_pWnd->GetChild(_T("state\\exp")));
	
	//setting
	m_pPbItem0Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoheal_back\\openbutt0")));                
	m_pPbItem1Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoheal_back\\openbutt1")));          

	m_pPbSkill0Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill0_back\\openbutt0")));                
	m_pPbSkill1Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill1_back\\openbutt0")));                
	m_pPbSkill2Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill2_back\\openbutt0")));                
	m_pPbSkill3Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill3_back\\openbutt0")));                
	m_pPbSkill4Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill4_back\\openbutt0")));                
	m_pPbSkill5Active	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill5_back\\openbutt0")));                
	
	m_pPbAutoSelectActive	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\autoselectbutt")));                
	m_pPbAutoBaoQiActive	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\autoburstbutt")));                
	m_pPbStaticPointActive	=   static_cast<GUIPushButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\holdbutt")));                


	m_pBnSaveSettings		=   static_cast<GUIButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\savebutton"))); 
	m_pBnCloseSetting		=   static_cast<GUIButton*>(m_pWndSetting->GetChild(_T("setting_backpic\\closebutt"))); 			

	m_pEd_Item0Percent	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoheal_back\\word2\\comboback\\num0")));
	m_pEd_Item1Percent	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoheal_back\\word4\\comboback\\num0")));

	m_pEd_Item0Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoheal_back\\word2\\comboback\\num1")));
	m_pEd_Item1Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoheal_back\\word4\\comboback\\num1")));

	m_pEd_Skill0Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill0_back\\word2\\comboback\\num0")));
	m_pEd_Skill1Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill1_back\\word2\\comboback\\num0")));
	m_pEd_Skill2Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill2_back\\word2\\comboback\\num0")));
	m_pEd_Skill3Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill3_back\\word2\\comboback\\num0")));
	m_pEd_Skill4Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill4_back\\word2\\comboback\\num0")));
	m_pEd_Skill5Time	=   static_cast<GUIEditBox*>(m_pWndSetting->GetChild(_T("setting_backpic\\autoskill_back\\skill5_back\\word2\\comboback\\num0")));

	m_pBnMin			=   static_cast<GUIButton*>(m_pWndMin->GetChild(_T("onhook_fold_pic"))); 	

	

	m_pWnd->SetInvisible( TRUE );
	m_pWndSetting->SetInvisible(TRUE);
	m_pWndMin->SetInvisible( TRUE );

	m_pWnd->SetTopMost( TRUE );

	SetDefaultCombatUI();
	SetDefaultSettingUI();
	RefreshSettingToUI();

	return TRUE;
}

BOOL AutoControllerFrame::EscCancel()
{
	if (!m_pWndSetting->IsInvisible())
	{
		m_pWndSetting->SetInvisible( TRUE );
		return TRUE;
	}
	if (!m_pWnd->IsInvisible())
	{
		m_pWnd->SetInvisible( TRUE );
		m_pWndMin->SetInvisible( FALSE );
		return TRUE;
	}

	return FALSE;
}

VOID AutoControllerFrame::Update()
{
	m_dwCurrentSkillTime += Kernel::Inst()->GetDeltaTimeDW();
	if (m_dwCurrentSkillTime > 600 )
	{
		UpdateCombat();
	}
	m_dwTimeMark += Kernel::Inst()->GetDeltaTimeDW();
	if (m_dwTimeMark > 1000)
	{
		UpdateUsetimeInfo();
		m_dwTimeMark = 0;
	}
	UpdateCDtimes();
}
BOOL AutoControllerFrame::OnWnd_AutoCombat(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if (pWnd == m_pBnStartCombat)
		{
			if (EAS_NONE == eState)
			{
				m_staticPoint = RoleMgr::Inst()->GetLocalPlayer()->GetPos();
				StartAutoCombating();
				m_pBnStartCombat->SetText(g_StrTable[_T("StopAutoCombating")]);
			}
			else
			{
				EndAutoCombating();
				m_pBnStartCombat->SetText(g_StrTable[_T("StartAutoCombating")]);
			}
		}
		if (pWnd == m_pBnMinSize)
		{
			m_pWnd->SetInvisible( TRUE );
			m_pWndMin->SetInvisible( FALSE );
			m_pWndMin->FlipToTop();
		}
		if (pWnd == m_pBnClose)
		{
			m_pWnd->SetInvisible( TRUE );
			EndAutoCombating();
			m_pBnStartCombat->SetText(g_StrTable[_T("StartAutoCombating")]);
		}
		if (pWnd == m_pBnAutoSetting)
		{
			if(m_pWndSetting->IsInvisible())
			{
				m_pWndSetting->SetInvisible(FALSE);
				m_pWndSetting->FlipToTop();
			}
			else
			{	
				m_pWndSetting->SetInvisible(TRUE);
			}
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		if (pWnd->GetName().substr(0, 4) == _T("item"))
		{
			if( EDT_Item != (EDragType)pEvent->dwParam1 )
				break;


			for (int i = 0; i < MAX_ITEM; ++i)
			{
				if (m_pBtItems[i] == pWnd)
				{
					INT nPos = (INT16)LOWORD(pEvent->dwParam2);
					Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos);
					if (P_VALID(pItem))
					{
						// 只能是消耗品
						if (	(pItem->GetItemTypeEx() != EITE_Resume)
							||	(pItem->GetProto()->nPrepareTime > 0)
							||	(pItem->GetProto()->byMinUseLevel > RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel()) 
							)
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_AutoFight_Itemlawlessly")]);
							break;
						}
						int nItemCount = 0;
						ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
						nItemCount = pContainer->GetItemQuantity(pItem->GetItemTypeID());

						m_pBtItems[i]->RefreshItem( pItem->GetItemTypeID(),nItemCount ,pItem->GetItemQuality());
						m_itemData[i].dwItemID = pItem->GetItemTypeID();
						m_itemData[i].n16Index = nPos;//todo 容器中位置移动
						m_itemData[i].n64ID	= pItem->GetItemId();
					}
					break;
				}
			}
		}
		if (pWnd->GetName().substr(0, 5) == _T("skill"))
		{
			if( EDT_Skill != (EDragType)pEvent->dwParam1 )
				break;

			for (int i = 0; i < MAX_SKILL; ++i)
			{
				if (m_pBtSkills[i] == pWnd)
				{
					const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(pEvent->dwParam2);
					if(!P_VALID(pSkillData))
						return false;

					//--被动技能不能使用
					if(pSkillData->pProto->eUseType==ESUT_Passive)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("AutoCombat_CanPutbeidongSKILL")]);
						break;
					}
				
					if( ESSTE_Produce == pSkillData->pProto->nType2 )
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("AutoCombat_CanPutProductSKILL")]);
						break;
					}

					if(pSkillData->dwTypeID == SkillMgr::Inst()->GetNormalAttackTypeID())
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("AutoCombat_CanPutNormalSKILL")]);
						break;
					}
// 					tstring picName = SkillMgr::Inst()->GetSkillIconPath(pEvent->dwParam2);
// 					m_pBtSkills[i]->SetPicFileName(picName);
					m_pBtSkills[i]->SetPicColor(0xFFFFFFFF);
					m_pBtSkills[i]->ReflashSkill(pEvent->dwParam2);
					m_skillData[i].dwSkillID = pEvent->dwParam2;
					break;
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		if (pWnd->GetName().substr(0, 4) == _T("item"))
		{
			for (int i = 0; i < MAX_ITEM; ++i)
			{
				if (m_pBtItems[i] == pWnd)
				{
					Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_itemData[i].n64ID);
					if (P_VALID(pItem))
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
					}
					break;
				}
			}
		}
		if (pWnd->GetName().substr(0, 5) == _T("skill"))
		{
			for (int i = 0; i < MAX_SKILL; ++i)
			{
				if (m_pBtSkills[i] == pWnd)
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetSkillTips(m_skillData[i].dwSkillID).c_str());
					break;
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
				

			if (pWnd->GetName().substr(0, 4) == _T("item"))
			{
				//只有战斗符不在使用的时候可以拖拽技能
				if (eState != EAS_NONE)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("AutoCombat_CantChangeItem")]);
					break;
				}

				for (int i = 0; i < MAX_ITEM; ++i)
				{
					if (m_pBtItems[i] == pWnd)
					{
						Item* pItem = ItemMgr::Inst()->GetConItemByID(m_itemData[i].eType,m_itemData[i].n64ID);
						if(!P_VALID(pItem))
							break;
						
						DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
						if (P_VALID(pDrag))
						{
							POINT pt;
							::GetCursorPos(&pt);

							pEvent->dwParam1 = EDT_Item;
							pEvent->dwParam2 = MAKELONG(pItem->GetPos(), pItem->GetItemQuantity());
							pEvent->dwParam3 = pItem->GetItemTypeID();
							pEvent->dwParam4 = MAKELONG(pt.x, pt.y);

							pDrag->OnEvent(pEvent);
							// 需要隐藏Tip
							ShowTip(pWnd, NULL);

							m_pBtItems[i]->RefreshItem();
							m_itemData[i].dwItemID = GT_INVALID;
							m_itemData[i].n64ID = GT_INVALID;
						}
						break;
					}
				}
			}
			if (pWnd->GetName().substr(0, 5) == _T("skill"))
			{
				//只有战斗符不在使用的时候可以拖拽技能
				if (eState != EAS_NONE)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("AutoCombat_CantChangeSkill")]);
					break;
				}
			
				for (int i = 0; i < MAX_SKILL; ++i)
				{
					if (m_pBtSkills[i] == pWnd)
					{
						const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(m_skillData[i].dwSkillID);
						if(!P_VALID(pSkillData))
							break;

						DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
						if (P_VALID(pDrag))
						{
							pEvent->dwParam1 = EDT_Skill;
							pEvent->dwParam2 = m_skillData[i].dwSkillID;
							pDrag->OnEvent(pEvent);
							// 需要隐藏Tip
							ShowTip(pWnd, NULL);

							m_pBtSkills[i]->ReflashSkill(GT_INVALID);
							m_pBtSkills[i]->SetPicColor(0xFFFFFF);
							m_skillData[i].dwSkillID = GT_INVALID;
						}
						break;
					}
				}
			}
		}
		break;
	default:
		break;
	}


	return TRUE;

}
BOOL AutoControllerFrame::OnWnd_Setting(tagGUIEvent* pEvent)
{
	GUIWnd* m_pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(m_pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if (m_pWnd == m_pBnSaveSettings)
		{
			SaveSetting();
			m_pWndSetting->SetInvisible(TRUE);
		}
		if (m_pWnd == m_pBnCloseSetting)
		{
			m_pWndSetting->SetInvisible(TRUE);
		}
		break;
	default:
		break;
	}


	return TRUE;
}

VOID AutoControllerFrame::StartAutoCombating()
{
	LocalPlayer* pSelf = RoleMgr::Inst()->GetLocalPlayer();
	if (!pSelf)
		return;

	const DWORD RoleStateMask=ERS_Stall;
	if(pSelf->GetRoleState()&RoleStateMask)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Cant_Baitan_AutoCombat")]);
		return;
	}
		

	m_pBnAutoSetting->SetEnable( FALSE );
	m_pWndSetting->SetInvisible( TRUE );
	m_pBnStartCombat->SetEnable( FALSE );

	tagNC_SyncAutoFightState cmd;
	cmd.n64ItemID = m_n64ID;
	cmd.eAutoFightState = EAFS_Start;
	TObjRef<NetSession>()->Send(&cmd);

	m_dwUseExp = 0;
	
}

VOID AutoControllerFrame::EndAutoCombating()
{
	m_pBnAutoSetting->SetEnable( TRUE );
	m_pBnStartCombat->SetEnable( FALSE );

	tagNC_SyncAutoFightState cmd;
	cmd.n64ItemID = m_n64ID;
	cmd.eAutoFightState = EAFS_End;
	TObjRef<NetSession>()->Send(&cmd);

	m_LockTargetID = 0;
	eState = EAS_NONE;
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		plp->SetHookState(false);
	}

	
}

VOID AutoControllerFrame::RefreashCombatUI()
{

	
}

VOID AutoControllerFrame::SetDefaultCombatUI()
{
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		m_pBtItems[i]->RefreshItem();
	}
	for (int i = 0; i < MAX_SKILL; ++i)
	{
		m_pBtSkills[i]->ReflashSkill(GT_INVALID);
	}
}

bool AutoControllerFrame::SearchNextMonster()
{

	const map<DWORD,Role*>& mapRole = RoleMgr::Inst()->GetRoleMap();
	list<Role*> listRoles;
	for( map<DWORD,Role*>::const_iterator cItr = mapRole.begin(); cItr != mapRole.end(); cItr++ )
	{
		if( !cItr->second->IS_KIND_OF(NPC))
			continue;

		NPC* pNPC = (NPC*)(cItr->second);
		if( !pNPC->IsMonster() )
			continue;

		//如果定点打怪，根据距离筛掉怪物
		if(m_bPointStand)
		{
			FLOAT testData = Vec3Dist(m_staticPoint,cItr->second->GetPos());
			if (Vec3Dist(m_staticPoint,cItr->second->GetPos()) > SELECT_RANGE*TILE_SCALE )
				continue;
		}

		bool blocked = false;
		for (list<DWORD>::iterator iter = m_lsBlockIDList.begin();
			iter != m_lsBlockIDList.end(); ++iter)
		{
			if (cItr->first == (*iter))
			{
				blocked = true;
				break;
			}
		}

		if (blocked)
			continue;

		listRoles.push_back( cItr->second );
	}

	
	if(listRoles.empty())
		return false;
	
	// 按距离排序
	CmpMonsters CmpFuc;
	CmpFuc.refPos = RoleMgr::Inst()->GetLocalPlayer()->GetPos();
	listRoles.sort( CmpFuc );

	Role* pRole = listRoles.front();
	if (!P_VALID(pRole))
		return false;

// 	if (m_dwBlockID == pRole->GetID())//如果是被卡死的怪物
// 	{
// 		listRoles.pop_front();
// 		if (listRoles.empty())
// 		{
// 			return false;
// 		}
// 		pRole = listRoles.front();
// 		if (!P_VALID(pRole))
// 			return false;
// 
// 	}

	m_LockTargetID = pRole->GetID();
	//m_dwBlockID = GT_INVALID;
	
	tagRolePickEvent evt( _T("tagRolePickEvent"), this );
	evt.eType = EWPT_LClick;
	evt.dwRoleID = m_LockTargetID;
	m_pMgr->SendEvent( &evt );

	m_LastFightPoint = pRole->GetPos();

	Role* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return false;
	//先寻路
	if (!UseAutoSkill() && !m_bPointStand)
	{
		tagMouseMoveEvent pEvent;
		pEvent.start = pLp->GetPos();
		pEvent.end.x   = static_cast<FLOAT>(pRole->GetPos().x);	//格子坐标转换为世界坐标
		pEvent.end.z   = static_cast<FLOAT>(pRole->GetPos().z);	//格子坐标转换为世界坐标
		pEvent.end.y   = static_cast<FLOAT>(pRole->GetPos().y);
		pEvent.bUserCtrl=false;
		pEvent.validDist = 0;
		pLp->OnGameEvent(&pEvent);
	}
	eState = EAS_COMBAT;

	return true;
}


bool AutoControllerFrame::UseAutoSkill()
{
	LocalPlayer* pLp = (LocalPlayer*)RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return false;

	if (pLp->IsSkillState() && !pLp->IsNormalSkillState())
		return false;
	
	bool skilled = false;
	for (int i = 0; i< MAX_SKILL;i++)
	{
		tagAutoSkillData data = m_skillData[i];
		FLOAT fCDCount = 0.0f;
		FLOAT fRestoreRatio = 0.0f;

		SkillMgr::Inst()->GetSkillCDTime(MTransSkillID(data.dwSkillID), fCDCount, fRestoreRatio);
	

		if (!data.bIsActive)//没有激活
			continue;

		if (data.nCurrentTime < data.nTimeDelay || fCDCount > 0)//时间没到 或者说技能CD中
			continue;

		if (data.dwSkillID == 0)
			continue;

		if(!CombatActionChecker::Inst()->IfCanUseSkill(data.dwSkillID,m_LockTargetID,false))
			continue;

		SkillMgr::Inst()->OnUseSkill(data.dwSkillID);
		m_skillData[i].nCurrentTime = 0;
		return true;	
	}
	// 检测是否正在使用普通技能中
	if (pLp->IsSkillState() && pLp->IsNormalSkillState())
		return true;

	DWORD skillID = SkillMgr::Inst()->GetNormalAttackTypeID();
	if(!CombatActionChecker::Inst()->IfCanUseSkill(skillID,m_LockTargetID,false))
		return false;

	SkillMgr::Inst()->OnUseSkill(skillID);
		return true;
}

VOID AutoControllerFrame::UseAutoItem()
{
	//HP
	if (m_itemData[0].bIsActive)
	{
		Role* pLp = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(pLp))
			return;

		if (m_itemData[0].nCurrentTime > m_itemData[0].nTimeDelay)
		{
			Item* pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(m_itemData[0].dwItemID);
			if(P_VALID(pItem))
			{
				FLOAT k = pLp->GetAttribute(ERA_HP);
				FLOAT m = pLp->GetAttribute(ERA_MaxHP);
				FLOAT l = k/m*100;
				if ( l < m_itemData[0].nUsePercent)
				{
					/*tagClickItemEvent event(_T("tagClickItemEvent"),this);
					event.eType = m_itemData[0].eType;
					event.n64ItemID = m_itemData[0].n64ID;
					event.dwTypeID = m_itemData[0].dwItemID;
					event.n16Index = m_itemData[0].n16Index;
					m_pFrameMgr->SendEvent(&event);*/

					//--检查玩家状态
					const DWORD RoleStateMask=ERS_Stall;
					if(RoleMgr::Inst()->GetLocalPlayer()->GetRoleState()&RoleStateMask)
						return;

					//检查物品的状态
					if( pItem->IsItemLocked() )
					{
						tagDWORDTime dwUnLockTime = pItem->GetUnLockTime();
						if( dwUnLockTime == 0 )
							return;

						tagDWORDTime dwCurTime = GetCurrentDWORDTime();
						if( CompareDwordTime( dwCurTime,dwUnLockTime ) > 0 )
						{
							pItem->SetIMLocked( false );
						}
					}

					// 优先判断是否是特殊功用物品
					EItemSpecFunc eType = pItem->GetItemSpecFunc();
					if(eType != EISF_Null)
					{
						return;
					}
					// 判断物品是否可以使用
					else if(pItem->IsUseable())
					{		
						DWORD dwTypeID = pItem->GetItemTypeID();
						FLOAT fCDCount = 0.0f;
						FLOAT fRestoreRatio = 0.0f;
						ItemMgr::Inst()->GetItemCurCDTime(dwTypeID, fCDCount, fRestoreRatio);

						if(fCDCount > 0)
							return;

						tagUseItemEvent e(_T("tagUseItemEvent"), this);
						e.dwItemTypeID = pItem->GetItemTypeID();
						e.n64ItemID = pItem->GetItemId();
						m_pFrameMgr->SendEvent(&e);
					}
					Item* pItem2 = ItemMgr::Inst()->GetPocketFirstItemByTypeID(m_itemData[0].dwItemID);
					int nItemCount = 0;
					ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
					nItemCount = pContainer->GetItemQuantity(pItem2->GetItemTypeID());

					m_pBtItems[0]->RefreshItem(pItem2->GetItemTypeID(),nItemCount-1 ,pItem2->GetItemQuality());
					if (pItem2->GetItemTypeID() == m_itemData[1].dwItemID)
					{
						m_pBtItems[1]->RefreshItem(pItem2->GetItemTypeID(),nItemCount-1 ,pItem2->GetItemQuality());
					}
				}
				
			}
			else
			{
				m_pBtItems[0]->RefreshItem();
			}
			m_itemData[0].nCurrentTime = 0;
		}
		
	}
	//MP
	if(m_itemData[1].bIsActive)
	{
		Role* pLp = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(pLp))
			return;

		if (m_itemData[1].nCurrentTime > m_itemData[1].nTimeDelay)
		{
			Item* pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(m_itemData[1].dwItemID);
			if(P_VALID(pItem))
			{
				FLOAT k = pLp->GetAttribute(ERA_MP);
				FLOAT m = pLp->GetAttribute(ERA_MaxMP);
				FLOAT l = k/m*100;
				if ( l < m_itemData[1].nUsePercent)
				{

					//--检查玩家状态
					const DWORD RoleStateMask=ERS_Stall;
					if(RoleMgr::Inst()->GetLocalPlayer()->GetRoleState()&RoleStateMask)
						return;
					/*tagClickItemEvent event(_T("tagClickItemEvent"),this);
					event.eType = m_itemData[1].eType;
					event.n16Index = m_itemData[1].n16Index;
					event.dwTypeID = m_itemData[1].dwItemID;
					event.n64ItemID = m_itemData[1].n64ID;
					m_pFrameMgr->SendEvent(&event);*/

					//检查物品的状态
					if( pItem->IsItemLocked() )
					{
						tagDWORDTime dwUnLockTime = pItem->GetUnLockTime();
						if( dwUnLockTime == 0 )
							return;

						tagDWORDTime dwCurTime = GetCurrentDWORDTime();
						if( CompareDwordTime( dwCurTime,dwUnLockTime ) > 0 )
						{
							pItem->SetIMLocked( false );
						}
					}

					// 优先判断是否是特殊功用物品
					EItemSpecFunc eType = pItem->GetItemSpecFunc();
					if(eType != EISF_Null)
					{
						return;
					}
					// 判断物品是否可以使用
					else if(pItem->IsUseable())
					{		

						DWORD dwTypeID = pItem->GetItemTypeID();
						FLOAT fCDCount = 0.0f;
						FLOAT fRestoreRatio = 0.0f;
						ItemMgr::Inst()->GetItemCurCDTime(dwTypeID, fCDCount, fRestoreRatio);

						if(fCDCount > 0)
							return;

						tagUseItemEvent e(_T("tagUseItemEvent"), this);
						e.dwItemTypeID = pItem->GetItemTypeID();
						e.n64ItemID = pItem->GetItemId();
						m_pFrameMgr->SendEvent(&e);
					}
					Item* pItem2 = ItemMgr::Inst()->GetPocketFirstItemByTypeID(m_itemData[1].dwItemID);
					int nItemCount = 0;
					ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
					nItemCount = pContainer->GetItemQuantity(pItem2->GetItemTypeID());

					m_pBtItems[1]->RefreshItem(pItem2->GetItemTypeID(),nItemCount-1 ,pItem2->GetItemQuality());
					if (pItem2->GetItemTypeID() == m_itemData[0].dwItemID)
					{
						m_pBtItems[0]->RefreshItem(pItem2->GetItemTypeID(),nItemCount-1 ,pItem2->GetItemQuality());
					}

				}
			}
			else
			{
				m_pBtItems[1]->RefreshItem();
			}
			m_itemData[1].nCurrentTime = 0;
		}
		
	}
}

VOID AutoControllerFrame::GoBack()
{
	
	LocalPlayer* pSelf = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pSelf))
		return;

	if (m_staticPoint == Vector3(0,0,0))
		return;

	if (m_staticPoint == pSelf->GetPos())
		return;


	tagMouseMoveEvent pEvent;
	pEvent.start = pSelf->GetPos();
	pEvent.end.x   = static_cast<FLOAT>(m_staticPoint.x);	//格子坐标转换为世界坐标
	pEvent.end.z   = static_cast<FLOAT>(m_staticPoint.z);	//格子坐标转换为世界坐标
	pEvent.end.y   = static_cast<FLOAT>(m_staticPoint.y);
	pEvent.bUserCtrl=false;
	pEvent.validDist = 0;
	pSelf->OnGameEvent(&pEvent);
}

VOID AutoControllerFrame::GoBackToLastPositionSeeMonster()
{
	LocalPlayer* pSelf = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pSelf))
		return;


	if (m_LastFightPoint == Vector3(0,0,0))
		return;

	if (m_LastFightPoint == pSelf->GetPos())
		return;

	tagMouseMoveEvent pEvent;
	pEvent.start = pSelf->GetPos();
	pEvent.end.x   = static_cast<FLOAT>(m_LastFightPoint.x);	//格子坐标转换为世界坐标
	pEvent.end.z   = static_cast<FLOAT>(m_LastFightPoint.z);	//格子坐标转换为世界坐标
	pEvent.end.y   = static_cast<FLOAT>(m_LastFightPoint.y);
	pEvent.bUserCtrl=false;
	pEvent.validDist = 0;
	pSelf->OnGameEvent(&pEvent);
}

VOID AutoControllerFrame::UpdateCombat()
{
	Role* pLpd = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLpd))
		return;

	if (pLpd->IsDead())
		return;

	if ( EAS_COMBAT == eState)
	{
		
		Role* pRole = RoleMgr::Inst()->FindRole(m_LockTargetID);
		if (!P_VALID(pRole))
		{
			if (m_bPointStand)
			{
				GoBack();
			}else{
				//GoBackToLastPositionSeeMonster();
			}
			eState = EAS_SEARCHING;
		}
		else
		{
			Role* pLp = RoleMgr::Inst()->GetLocalPlayer();
			if(!P_VALID(pLp))
				return;

			FLOAT testData = Vec3Dist(m_staticPoint,pRole->GetPos());
			if (Vec3Dist(pLp->GetPos(),pRole->GetPos()) > SELECT_RANGE*TILE_SCALE
				|| pRole->IsDead())
			{
				m_LockTargetID = 0;
				if (m_bPointStand)
				{
					GoBack();
				}else{
					//GoBackToLastPositionSeeMonster();
				}
				eState = EAS_SEARCHING;
				return;
			}
			m_dwTargetHP = pRole->GetAttribute(ERA_HP);//获得当前怪物HP。为了防止卡死使用
		}

		BlockCheck();
		UseAutoItem();

		
		pRole = RoleMgr::Inst()->FindRole(m_LockTargetID);
		if (!P_VALID(pRole))
			return;

		if (!CombatActionChecker::Inst()->RayCollideTarget(pRole))
		{
			IdleCheck();
// 			eState = EAS_PATHFING;
// 			m_dwPathfingTimeEclipse = 0;
			return;
		}

		if(!UseAutoSkill())
		{
			//GoToMonsterCurrentPosition();
		}
		if (m_bAutoBaoQi)
		{
			AutoBaoQi();
		}
			
		
	}
	if ( EAS_SEARCHING == eState)
	{
		if(!SearchNextMonster())
		{
			if (m_bPointStand)
			{
				GoBack();
			}
		}
		UseAutoItem();
		UseAutoSkill();
	}

	if ( EAS_NOT_ALL_AUTO == eState)
	{
		UseAutoItem();
		if (!MonsterIsValid(m_LockTargetID))
		{
			if (m_bPointStand)
			{
				GoBack();
			}
		}

		BlockCheck();
		if(!UseAutoSkill())
		{
			//GoToMonsterCurrentPosition();
		}
		if (m_bAutoBaoQi)
		{
			AutoBaoQi();
		}
	}

	if ( EAS_PATHFING == eState)
	{
		m_dwPathfingTimeEclipse += Kernel::Inst()->GetDeltaTimeDW();
		if (m_dwPathfingTimeEclipse > PATHFING_TIME)
		{
			eState = EAS_SEARCHING;
		}
	}
}

VOID AutoControllerFrame::SetDefaultSettingUI()
{
	m_pEd_Item0Percent->SetText(_T("50"));
	m_pEd_Item1Percent->SetText(_T("50"));

	m_pEd_Item0Time->SetText(_T("1"));
	m_pEd_Item1Time->SetText(_T("1"));

	m_pEd_Skill0Time->SetText(_T("1"));
	m_pEd_Skill1Time->SetText(_T("1"));
	m_pEd_Skill2Time->SetText(_T("1"));
	m_pEd_Skill3Time->SetText(_T("1"));
	m_pEd_Skill4Time->SetText(_T("1"));
	m_pEd_Skill5Time->SetText(_T("1"));

	m_pPbAutoSelectActive->SetState(EGUIBS_PushDown);
}

VOID AutoControllerFrame::SaveSetting()
{

	int percentOne = _tcstol(m_pEd_Item0Percent->GetText(),    NULL, 10);
	int percentTwo = _tcstol(m_pEd_Item1Percent->GetText(),	NULL, 10);
	
	if(percentOne > 100)
	{
		percentOne = 100;
		m_pEd_Item0Percent->SetText(_T("100"));
	}
	else if ( percentOne <= 1)
	{
		percentOne = 1;
		m_pEd_Item0Percent->SetText(_T("1"));
	}
	
	if (percentTwo > 100)
	{
		percentTwo = 100;
		m_pEd_Item1Percent->SetText(_T("100"));
	}
	else if ( percentTwo <= 1)
	{
		percentTwo = 1;
		m_pEd_Item1Percent->SetText(_T("1"));
	}
	m_itemData[0].nUsePercent	= percentOne;
	m_itemData[1].nUsePercent	= percentTwo;

	int item0TimeDeley	= (_tcstol(m_pEd_Item0Time->GetText(),    NULL, 10));
	if (item0TimeDeley > 9999)
	{
		item0TimeDeley = 9999;
		m_pEd_Item0Time->SetText(_T("9999"));
	}
	else if (item0TimeDeley <= 1)
	{
		item0TimeDeley = 1;
		m_pEd_Item0Time->SetText(_T("1"));
	}
	m_itemData[0].nTimeDelay = item0TimeDeley*1000;

	int item1TimeDeley = (_tcstol(m_pEd_Item1Time->GetText(),    NULL, 10));
	if (item1TimeDeley > 9999)
	{
		item1TimeDeley = 9999;
		m_pEd_Item1Time->SetText(_T("9999"));
	}
	else if (item1TimeDeley <= 1)
	{
		item1TimeDeley = 1;
		m_pEd_Item1Time->SetText(_T("1"));
	}
	m_itemData[1].nTimeDelay	= item1TimeDeley*1000;

	int	skill0TimeDeley=	(_tcstol(m_pEd_Skill0Time->GetText(),    NULL, 10));
	int skill1TimeDeley=	(_tcstol(m_pEd_Skill1Time->GetText(),    NULL, 10));
	int	skill2TimeDeley=	(_tcstol(m_pEd_Skill2Time->GetText(),    NULL, 10));
	int	skill3TimeDeley=	(_tcstol(m_pEd_Skill3Time->GetText(),    NULL, 10));
	int	skill4TimeDeley=	(_tcstol(m_pEd_Skill4Time->GetText(),    NULL, 10));
	int	skill5TimeDeley=	(_tcstol(m_pEd_Skill5Time->GetText(),    NULL, 10));

	if (skill0TimeDeley > 9999)
	{
		skill0TimeDeley = 9999;
		m_pEd_Skill0Time->SetText(_T("9999"));
	}
	else if (skill0TimeDeley <= 1)
	{
		skill0TimeDeley = 1;
		m_pEd_Skill0Time->SetText(_T("1"));
	}

	if (skill1TimeDeley > 9999)
	{
		skill1TimeDeley = 9999;
		m_pEd_Skill1Time->SetText(_T("9999"));
	}
	else if (skill1TimeDeley <= 1)
	{
		skill1TimeDeley = 1;
		m_pEd_Skill1Time->SetText(_T("1"));
	}

	if (skill2TimeDeley > 9999)
	{
		skill2TimeDeley = 9999;
		m_pEd_Skill2Time->SetText(_T("9999"));
	}
	else if (skill2TimeDeley <= 1)
	{
		skill2TimeDeley = 1;
		m_pEd_Skill2Time->SetText(_T("1"));
	}

	if (skill3TimeDeley > 9999)
	{
		skill3TimeDeley = 9999;
		m_pEd_Skill3Time->SetText(_T("9999"));
	}
	else if (skill3TimeDeley <= 1)
	{
		skill3TimeDeley = 1;
		m_pEd_Skill3Time->SetText(_T("1"));
	}

	if (skill4TimeDeley > 9999)
	{
		skill4TimeDeley = 9999;
		m_pEd_Skill4Time->SetText(_T("9999"));
	}
	else if (skill4TimeDeley <= 1)
	{
		skill4TimeDeley = 1;
		m_pEd_Skill4Time->SetText(_T("1"));
	}

	if (skill5TimeDeley > 9999)
	{
		skill5TimeDeley = 9999;
		m_pEd_Skill5Time->SetText(_T("9999"));
	}
	else if (skill5TimeDeley <= 1)
	{
		skill5TimeDeley = 1;
		m_pEd_Skill5Time->SetText(_T("1"));
	}

	
	m_skillData[0].nTimeDelay	=	skill0TimeDeley*1000;
	m_skillData[1].nTimeDelay	=	skill1TimeDeley*1000;
	m_skillData[2].nTimeDelay	=	skill2TimeDeley*1000;
	m_skillData[3].nTimeDelay	=	skill3TimeDeley*1000;
	m_skillData[4].nTimeDelay	=	skill4TimeDeley*1000;
	m_skillData[5].nTimeDelay	=	skill5TimeDeley*1000;

	m_itemData[0].bIsActive = (EGUIBS_PushDown == m_pPbItem0Active->GetState()) ? true : false;
	m_itemData[1].bIsActive = (EGUIBS_PushDown == m_pPbItem1Active->GetState()) ? true : false;

	m_skillData[0].bIsActive= (EGUIBS_PushDown == m_pPbSkill0Active->GetState()) ? true : false;
	m_skillData[1].bIsActive= (EGUIBS_PushDown == m_pPbSkill1Active->GetState()) ? true : false;
	m_skillData[2].bIsActive= (EGUIBS_PushDown == m_pPbSkill2Active->GetState()) ? true : false;
	m_skillData[3].bIsActive= (EGUIBS_PushDown == m_pPbSkill3Active->GetState()) ? true : false;
	m_skillData[4].bIsActive= (EGUIBS_PushDown == m_pPbSkill4Active->GetState()) ? true : false;
	m_skillData[5].bIsActive= (EGUIBS_PushDown == m_pPbSkill5Active->GetState()) ? true : false;

	m_bAutoSelectMonster	=	(EGUIBS_PushDown == m_pPbAutoSelectActive->GetState()) ? true : false;
	m_bAutoBaoQi			=	(EGUIBS_PushDown == m_pPbAutoBaoQiActive->GetState()) ? true : false;
	m_bPointStand			=	(EGUIBS_PushDown == m_pPbStaticPointActive->GetState()) ? true : false;


}

VOID AutoControllerFrame::UpdateCDtimes()
{
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		if (!m_itemData[i].bIsActive)
			continue;

		if (NULL == m_itemData[i].dwItemID)
			continue;

		m_itemData[i].nCurrentTime += Kernel::Inst()->GetDeltaTimeDW();
	}
	for (int i = 0; i < MAX_SKILL; ++i)
	{
		if (!m_skillData[i].bIsActive)
			continue;

		if (NULL == m_skillData[i].dwSkillID)
			continue;

		m_skillData[i].nCurrentTime += Kernel::Inst()->GetDeltaTimeDW();
	}
}

DWORD AutoControllerFrame::OnOpenAutoHook(tagSpecItemEvent* pEvent)
{
	if (EISF_ZhanDouFu == pEvent->eSpecFuncSrc )
	{
		tagNC_UseZhanDouFu cmd;
		cmd.n64ItemID = pEvent->n64Serial;
		m_pSession->Send(&cmd);

		m_n64ID = pEvent->n64Serial;
	}
	return 0;
}

DWORD AutoControllerFrame::OnNS_UseZhanDouFu(tagNS_UseZhanDouFu* pMsg,DWORD pParam)
{

	switch (pMsg->dwErrorCode)
	{
	case E_AutoFight_UseZhanDouFu_Success:
		{
			if (m_bFristOpen)
			{
				ReadSettingFromFile();
				RefreshSettingToUI();
				m_bFristOpen = false;
			}
			m_pWnd->SetInvisible(FALSE);
			m_pWnd->FlipToTop();
			m_pWndMin->SetInvisible(TRUE);
		
			m_dwUseExp = 0;
			Item* pItem = ItemMgr::Inst()->GetPocketItemByID(pMsg->n64ItemID);
			if (P_VALID(pItem))
			{
				if ( 0 == pItem->GetData()->dw1stGainTime )
				{
					pItem->GetData()->dw1stGainTime = pMsg->dw1stGainTime;
				}
				m_sGetTime = ServerTime::Inst()->CalCurrentServerDwordTime();
				m_dwLeftTime = 0;
				INT nPostSecord = (INT)CalcTimeDiff(ServerTime::Inst()->CalCurrentServerDwordTime(), pItem->GetData()->dw1stGainTime);
				if(nPostSecord >= 0)
				{
					INT nTimeLimit = (INT)pItem->GetProto()->dwTimeLimit - nPostSecord;
					if(nTimeLimit > 0)
					{
						m_dwLeftTime = nTimeLimit;
					}
				}
				ItemTimerMgr::Inst()->SetItemRemainTime(pItem->GetItemId(),pMsg->dwLeftTicks);
			}


		}
		break;
	case E_AutoFight_No_ZhanDouFu:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_AutoFight_No_ZhanDouFu")]);
		break;
	case E_AutoFight_ZhanDouFu_TimeOut:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_AutoFight_ZhanDouFu_TimeOut")]);
		break;
	}

	
	return 0;
}

DWORD AutoControllerFrame::OnNS_ZhanDouFuTimeOut(tagNS_ZhanDouFuTimeOut* pMsg,DWORD pParam)
{
	if (pMsg->n64ItemID == m_n64ID)	// 只停止当前正在使用的战斗符
	{
		m_pWnd->SetInvisible(TRUE);
		m_pWndSetting->SetInvisible(TRUE);

		m_LockTargetID = 0;
		eState = EAS_NONE;
		LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
		if (P_VALID(plp))
		{
			plp->SetHookState(false);
		}

		m_pBnStartCombat->SetText(g_StrTable[_T("StartAutoCombating")]);
		m_pBnStartCombat->SetEnable( TRUE );

		m_n64ID = 0;

		if(P_VALID(GetObj("ZhanDouFuTimeOut")))
			KillObj("ZhanDouFuTimeOut");

		CreateObj("ZhanDouFuTimeOut", "MsgBoxEx");
		TObjRef<MsgBoxEx>("ZhanDouFuTimeOut")->Init( _T(""), g_StrTable[_T("ZhanDouFuTimeOut_1")],NULL ,MBF_OK,FALSE);
	}
	else{
		if(P_VALID(GetObj("ZhanDouFuTimeOut")))
			KillObj("ZhanDouFuTimeOut");

		CreateObj("ZhanDouFuTimeOut", "MsgBoxEx");
		TObjRef<MsgBoxEx>("ZhanDouFuTimeOut")->Init( _T(""), g_StrTable[_T("ZhanDouFuTimeOut_2")],NULL ,MBF_OK,FALSE);
	}
	
	return 0;

}

VOID AutoControllerFrame::SaveSettingToLocalFile()
{
	tstring strPath = Kernel::Inst()->GetWorkPath();
	DWORD dwLPID = RoleMgr::Inst()->GetLocalPlayerID();
	tstring strLocalName = PlayerNameTab::Inst()->FindNameByID(dwLPID);
	if( strLocalName.empty() )
	return;

	strPath += _T("config\\");
	strPath += strLocalName;
	strPath += _T("\\");

	TCHAR DirName[256];
	const TCHAR* p = strPath.c_str();
	TCHAR* q = DirName;	

	while(*p)
	{
		if ((_T('\\') == *p) || (_T('/') == *p))
		{
			if (_T(':') != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = _T('\0');
	}
	CreateDirectory(DirName, NULL);

	strPath += _T("autofight.dat");

	FILE* fp = NULL;
	fp=_tfopen( strPath.c_str(), _T("wb") );

	if( fp != NULL)
	{
		DWORD dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
		fwrite(&dwRoleID, sizeof(DWORD), 1, fp);
		for(int j=0; j<ITEM_BUTTON_COUNT; j++)
		{
			const tagAutoItemData itemData = m_itemData[j];
			fwrite(&itemData, sizeof(tagAutoItemData), 1, fp);
		}
		for(int i=0; i<SKILL_BUTTON_COUNT; i++)
		{
			const tagAutoSkillData skillData = m_skillData[i];
			fwrite(&skillData, sizeof(tagAutoSkillData), 1, fp);
		}

		fwrite(&m_bAutoSelectMonster, sizeof(bool), 1, fp);
		fwrite(&m_bAutoBaoQi, sizeof(bool), 1, fp);
		fwrite(&m_bPointStand, sizeof(bool), 1, fp);

		fclose(fp);
	}
}

VOID AutoControllerFrame::ReadSettingFromFile()
{
	DWORD dwLPID = RoleMgr::Inst()->GetLocalPlayerID();
	tstring strPath = Kernel::Inst()->GetWorkPath();
	tstring strLocalName = PlayerNameTab::Inst()->FindNameByID(dwLPID);
	strPath += _T("config\\");
	strPath += strLocalName;
	strPath += _T("\\");
	strPath += _T("autofight.dat");

	FILE *fp= _tfopen( strPath.c_str(), _T("rb") );
	if( fp != NULL )
	{
		DWORD dwRoleID = GT_INVALID;
		fread(&dwRoleID, sizeof(DWORD), 1, fp);
		if(dwRoleID!=dwLPID)
			return;

		for(int j=0; j<ITEM_BUTTON_COUNT; j++)
		{
			tagAutoItemData itemData;
			fread(&itemData, sizeof(tagAutoItemData), 1, fp);
			memcpy(&m_itemData[j], &itemData, sizeof(tagAutoItemData));
		}
		for(int i=0; i<SKILL_BUTTON_COUNT; i++)
		{
			tagAutoSkillData skillData;
			fread(&skillData, sizeof(tagAutoSkillData), 1, fp);
			memcpy(&m_skillData[i], &skillData, sizeof(tagAutoSkillData));
		}

		fread(&m_bAutoSelectMonster, sizeof(bool), 1, fp);
		fread(&m_bAutoBaoQi, sizeof(bool), 1, fp);
		fread(&m_bPointStand, sizeof(bool), 1, fp);
	}	

}

VOID AutoControllerFrame::RefreshSettingToUI()
{
	TCHAR szText[64];
	
	for (int i = 0; i < MAX_ITEM; ++i)
	{
		if (0 == i)
		{
			if(m_itemData[i].bIsActive)
				m_pPbItem0Active->SetState(EGUIBS_PushDown);
			
			_stprintf( szText, _T("%d"),  m_itemData[i].nUsePercent);
			m_pEd_Item0Percent->SetText(szText);
			_stprintf( szText, _T("%d"),  m_itemData[i].nTimeDelay/1000);
			m_pEd_Item0Time->SetText(szText);
		}
			
		if (1 == i)
		{
			if(m_itemData[i].bIsActive)
				m_pPbItem1Active->SetState(EGUIBS_PushDown);

			_stprintf( szText, _T("%d"),  m_itemData[i].nUsePercent);
			m_pEd_Item1Percent->SetText(szText);

			_stprintf( szText, _T("%d"),  m_itemData[i].nTimeDelay/1000);
			m_pEd_Item1Time->SetText(szText);
		}

		Item* pItem = ItemMgr::Inst()->GetConItemByID(m_itemData[i].eType,m_itemData[i].n64ID);
		if(P_VALID(pItem))
		{	
			m_pBtItems[i]->RefreshItem(pItem->GetItemTypeID(),pItem->GetItemQuantity() ,pItem->GetItemQuality());
		}

	}


	for (int i = 0; i < MAX_SKILL; ++i)
	{

		const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(MTransSkillID(m_skillData[i].dwSkillID));
		if (P_VALID(pSkillData))
		{
			m_pBtSkills[i]->ReflashSkill(m_skillData[i].dwSkillID);
		}
		else
		{
			m_pBtSkills[i]->ReflashSkill(GT_INVALID);
			m_pBtSkills[i]->SetPicColor(0xFFFFFF);
		}
		
		if ( 0 == i)
		{
			if(m_skillData[i].bIsActive)
				m_pPbSkill0Active->SetState(EGUIBS_PushDown);
	
			_stprintf( szText, _T("%d"),  m_skillData[i].nTimeDelay/1000);
			m_pEd_Skill0Time->SetText(szText);
		} 
		else if(1 == i)
		{
			if(m_skillData[i].bIsActive)
				m_pPbSkill1Active->SetState(EGUIBS_PushDown);

			_stprintf( szText, _T("%d"),  m_skillData[i].nTimeDelay/1000);
			m_pEd_Skill1Time->SetText(szText);
		}
		else if(2 == i)
		{
			if(m_skillData[i].bIsActive)
				m_pPbSkill2Active->SetState(EGUIBS_PushDown);

			_stprintf( szText, _T("%d"),  m_skillData[i].nTimeDelay/1000);
			m_pEd_Skill2Time->SetText(szText);
		}
		else if(3 == i)
		{
			if(m_skillData[i].bIsActive)
				m_pPbSkill3Active->SetState(EGUIBS_PushDown);

			_stprintf( szText, _T("%d"), m_skillData[i].nTimeDelay/1000);
			m_pEd_Skill3Time->SetText(szText);
		}
		else if(4 == i)
		{
			if(m_skillData[i].bIsActive)
				m_pPbSkill4Active->SetState(EGUIBS_PushDown);

			_stprintf( szText, _T("%d"),  m_skillData[i].nTimeDelay/1000);
			m_pEd_Skill4Time->SetText(szText);
		}
		else if(5 == i)
		{
			if(m_skillData[i].bIsActive)
				m_pPbSkill5Active->SetState(EGUIBS_PushDown);

			_stprintf( szText, _T("%d"),  m_skillData[i].nTimeDelay/1000);
			m_pEd_Skill5Time->SetText(szText);
		}

	}

	if(m_bAutoSelectMonster)   //自动选怪
		m_pPbAutoSelectActive->SetState( EGUIBS_PushDown, FALSE ,FALSE);
	else
		m_pPbAutoSelectActive->SetState( EGUIBS_PopUp, FALSE ,FALSE );

	if(m_bAutoBaoQi)			//自动爆气
		m_pPbAutoBaoQiActive->SetState( EGUIBS_PushDown, FALSE ,FALSE );
	else
		m_pPbAutoBaoQiActive->SetState( EGUIBS_PopUp, FALSE  ,FALSE);

	if(m_bPointStand )			//定点打怪
		m_pPbStaticPointActive->SetState( EGUIBS_PushDown, FALSE  ,FALSE);
	else
		m_pPbStaticPointActive->SetState( EGUIBS_PopUp, FALSE  ,FALSE);


}

DWORD AutoControllerFrame::OnNS_SyncAutoFightState(tagNS_SyncAutoFightState* pMsg,DWORD pParam)
{
	if (pMsg->eAutoFightState == EAFS_Start)
	{
		if (m_bAutoSelectMonster)
		{
			eState = EAS_SEARCHING;
		}
		else
		{
			eState = EAS_NOT_ALL_AUTO;
		}
		
		LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
		if (P_VALID(plp))
		{
			plp->SetHookState(true);
		}
	}
	if (pMsg->eAutoFightState == EAFS_End)
	{
		m_LockTargetID = 0;
		eState = EAS_NONE;
		LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
		if (P_VALID(plp))
		{
			plp->SetHookState(false);
		}
	}
	if (pMsg->eAutoFightState == EAFS_Close)	//战斗符道具消失了
	{
		if (pMsg->n64ItemID == m_n64ID)	// 判断是不是当前正在使用的战斗符，不对其他战斗符执行此操作
		{
			m_LockTargetID = 0;
			eState = EAS_NONE;
			LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
			if (P_VALID(plp))
			{
				plp->SetHookState(false);
			}

			m_pBnStartCombat->SetText(g_StrTable[_T("StartAutoCombating")]);
			m_pBnAutoSetting->SetEnable( TRUE );
			m_pWnd->SetInvisible(TRUE);
			m_pWndSetting->SetInvisible(TRUE);
			m_pWndMin->SetInvisible(TRUE);
		}
	}

	//防止玩家狂点开始修炼按钮~
	m_pBnStartCombat->SetEnable( TRUE );
	return 0;
}

BOOL AutoControllerFrame::OnWnd_Min(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if (pWnd == m_pBnMin)
		{
			m_pWnd->SetInvisible( FALSE);
			m_pWnd->FlipToTop();
			m_pWndSetting->SetInvisible( TRUE );
			m_pWndMin->SetInvisible( TRUE );
		}
		break;
	default:
		break;
	}

	return TRUE;
}

DWORD AutoControllerFrame::OnUpdateLPSkillEvent( tagUpdateLPSkillEvent* pGameEvent )
{
	//--只更新主动技能
	if( ESUT_Active == pGameEvent->eType )
	{
		for (int i = 0; i < MAX_SKILL; ++i)
		{
			DWORD id =  MTransSkillID(m_skillData[i].dwSkillID);
			if (id == pGameEvent->dwID)
			{
				FLOAT fCDCount = 0.0f;
				FLOAT fRestoreRatio = 0.0f;

				m_pBtSkills[i]->ReflashSkill(m_skillData[i].dwSkillID);
				SkillMgr::Inst()->GetSkillCDTime(id, fCDCount, fRestoreRatio);
				m_pBtSkills[i]->SetCDTime(fCDCount);
				m_pBtSkills[i]->SetRestoreRatio(fRestoreRatio);
			}
		}
	}
	
	return 0;
}

VOID AutoControllerFrame::BlockCheck()
{
	Role* pRole = RoleMgr::Inst()->FindRole(m_LockTargetID);
	if (!P_VALID(pRole))
		return;
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return;

	// 目标血量没有下降并且玩家 && 不是处在技能释放状态 && 不在移动状态
// 	if (m_dwTargetHP ==  pRole->GetAttribute(ERA_HP) &&
// 		!pLp->IsSkillState() &&
// 		!pLp->IsMoveState())
	if(		(pLp->IsPursueState() && pLp->IsSkillState())
		||	(pLp->IsStandState() && !pLp->IsSkillState()))
	{
		if(pRole->GetAttribute(ERA_MaxHP) ==  pRole->GetAttribute(ERA_HP))//目标满血
		{
			m_dwTimeSpace += Kernel::Inst()->GetDeltaTimeDW();
		}
	}
	else
	{
		m_dwTimeSpace = 0;
	}

	if (m_dwTimeSpace > MIN_BLOCK_TIME)//表明被卡死
	{
		//m_dwBlockID = m_LockTargetID;
		m_lsBlockIDList.push_back(m_LockTargetID);
		if (m_bPointStand)
		{
			GoBack();
		}
		else
		{
			tagMouseMoveEvent pEvent;
			pEvent.start = pLp->GetPos();
			pEvent.end.x   = static_cast<FLOAT>(pRole->GetPos().x);	//格子坐标转换为世界坐标
			pEvent.end.z   = static_cast<FLOAT>(pRole->GetPos().z);	//格子坐标转换为世界坐标
			pEvent.end.y   = static_cast<FLOAT>(pRole->GetPos().y);
			pEvent.bUserCtrl=false;
			pEvent.validDist = 0;
			pLp->OnGameEvent(&pEvent);
		}		
		eState = EAS_SEARCHING;
		m_dwTimeSpace = 0;
	}

}

VOID AutoControllerFrame::IdleCheck()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return;

	Role* pRole = RoleMgr::Inst()->FindRole(m_LockTargetID);
	if (!P_VALID(pRole))
		return;

	if(pLp->IsIdleState())
	{
		m_dwIdleTime += Kernel::Inst()->GetDeltaTimeDW();
	}
	

	if (m_dwIdleTime > MAX_IDLE_TIME )//处于空闲状态超过限定时间就去上次打怪的地方
	{
		m_lsBlockIDList.push_back(m_LockTargetID);
		if (m_bPointStand)
		{
			GoBack();
		}
		else
		{
			tagMouseMoveEvent pEvent;
			pEvent.start = pLp->GetPos();
			pEvent.end.x   = static_cast<FLOAT>(pRole->GetPos().x);	//格子坐标转换为世界坐标
			pEvent.end.z   = static_cast<FLOAT>(pRole->GetPos().z);	//格子坐标转换为世界坐标
			pEvent.end.y   = static_cast<FLOAT>(pRole->GetPos().y);
			pEvent.bUserCtrl=false;
			pEvent.validDist = 0;
			pLp->OnGameEvent(&pEvent);
		}		
		eState = EAS_PATHFING;
		m_dwPathfingTimeEclipse = 0;
		m_dwIdleTime = 0;
	}

}

bool AutoControllerFrame::MonsterIsValid(DWORD dwID)
{
	const map<DWORD,Role*>& mapRole = RoleMgr::Inst()->GetRoleMap();
	map<DWORD,Role*>::const_iterator cItr = mapRole.find(m_LockTargetID);
	if (cItr != mapRole.end())
	{
		if( !cItr->second->IS_KIND_OF(NPC))
			return false;

		NPC* pNPC = (NPC*)(cItr->second);
		if( !pNPC->IsMonster() )
			return false;

		////如果定点打怪，根据距离筛掉怪物
		//if(m_bPointStand)
		//{
		//	FLOAT testData = Vec3Dist(m_staticPoint,cItr->second->GetPos());
		//	if (Vec3Dist(m_staticPoint,cItr->second->GetPos()) > SELECT_RANGE*TILE_SCALE )
		//		return false;
		//}
		return true;
	}
	
	return false;
}

DWORD AutoControllerFrame::OnRolePickEvent( tagRolePickEvent* pEvent )
{
	if (eState == EAS_NOT_ALL_AUTO)
	{
		if( EWPT_LClick == pEvent->eType )
			m_LockTargetID = pEvent->dwRoleID;
	}
	return 0;
}

VOID AutoControllerFrame::GoToMonsterCurrentPosition()
{
	Role* pRole = RoleMgr::Inst()->FindRole(m_LockTargetID);
	if (!P_VALID(pRole))
		return;
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return;
	
	tagMouseMoveEvent pEvent;
	pEvent.start = pLp->GetPos();
	pEvent.end.x   = static_cast<FLOAT>(pRole->GetPos().x);	//格子坐标转换为世界坐标
	pEvent.end.z   = static_cast<FLOAT>(pRole->GetPos().z);	//格子坐标转换为世界坐标
	pEvent.end.y   = static_cast<FLOAT>(pRole->GetPos().y);
	pEvent.bUserCtrl=false;
	pEvent.validDist = 0;
	pLp->OnGameEvent(&pEvent);
	return;
}
DWORD AutoControllerFrame::OnHitTargetEvent(tagHitTargetEvent* pEvent)
{
	Role* pRole = RoleMgr::Inst()->FindRole(m_LockTargetID);
	if (!P_VALID(pRole))
		return 0;

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return 0;

	if (   pEvent->dwSrcRoleID == pLp->GetID()
		&& pEvent->dwTarRoleID == pRole->GetID())
	{
		m_lsBlockIDList.clear();
	}
	return 0;
}

VOID AutoControllerFrame::AutoBaoQi()
{
	if (EGUIBS_PushDown != m_pPbAutoBaoQiActive->GetState())
		return;

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return;

	INT Nuqi = pLp->GetAttribute(ERA_Rage);
	if ( Nuqi != 100)
		return;

	DWORD skillID = 1011301;	// 徒手
	if (pLp->GetRWeaponType() == EITE_Wand)
	{
		skillID = 1011401;
	}
	else if (pLp->GetRWeaponType() == EITE_Bow)
	{
		skillID = 1011501;
	}
	else if (pLp->GetRWeaponType() == EITE_Faqi)
	{
		skillID = 1011601;
	}
	else if (pLp->GetRWeaponType() == EITE_Qin)
	{
		skillID = 1011701;
	}
	SkillMgr::Inst()->OnUseSkill(skillID);
}

DWORD AutoControllerFrame::OnShowDeadMenuEvent( tagShowDeadMenuEvent* pEvent )
{
	if(IsAutoCombating())
	{
		EndAutoCombating();
		m_pBnStartCombat->SetText(g_StrTable[_T("StartAutoCombating")]);
	}
	return 0;
}

VOID AutoControllerFrame::UpdateUsetimeInfo()
{
	if (EAS_NONE == eState)
		m_dwUseTime = 0;
	else
		m_dwUseTime++;

	TCHAR szText[64];
	_stprintf( szText, _T("%d:%d:%d"), m_dwUseTime/(60*60)%24,m_dwUseTime/(60)%60,m_dwUseTime%60);
	m_pStStartTime->SetText(szText);

	if (m_dwLeftTime >0)
	{
		DWORD dwTempTime = m_dwLeftTime;
		INT nPostSecord = (INT)CalcTimeDiff(ServerTime::Inst()->CalCurrentServerDwordTime(), m_sGetTime);
		if(nPostSecord >= 0)
		{
			dwTempTime = m_dwLeftTime - nPostSecord;
			if(dwTempTime < 0)
				dwTempTime = 0;
		}

		if (dwTempTime/(60*60*24) >0)
		{
			_stprintf( szText, g_StrTable[_T("auto_fight_lefttime_day")], dwTempTime/(60*60*24));
			m_pStLeftTime->SetText(szText);
		}
		else if (dwTempTime/(60*60) >0)
		{
			_stprintf( szText, g_StrTable[_T("auto_fight_lefttime_hour")], dwTempTime/(60*60));
			m_pStLeftTime->SetText(szText);
		}
		else if (dwTempTime/(60) >0)
		{
			_stprintf( szText, g_StrTable[_T("auto_fight_lefttime_min")], dwTempTime/(60));
			m_pStLeftTime->SetText(szText);
		}
	}
}

DWORD AutoControllerFrame::OnNS_RoleExpChange( tagNS_RoleExpChange* pNetCmd, DWORD dwParam )
{
	if (EAS_NONE == eState)
		return 0;

	m_dwUseExp += pNetCmd->nChange;
	TCHAR szText[64];
	_stprintf( szText, _T("%d"), m_dwUseExp);
	m_pStExpHaveGot->SetText(szText);

	return 0;
}