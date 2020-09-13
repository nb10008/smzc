#include "StdAfx.h"
#include "CombatFrame_EventCreater.h"
#include "CombatEvent.h"
#include "CombatSysUtil.h"
#include "CombatActionChecker.h"
#include "RoleMgr.h"
#include "WorldPickerFrame.h"
#include "BeAttackFrame.h"
#include "LocalPlayer.h"
#include "Player.h"
#include "NPC.h"
#include "SkillMgr.h"
#include "GroundItemMgr.h"
#include "DigBaoMgr.h"
#include "QuestMgr.h"
#include "MapMgr.h"
#include "UIEvent.h"
#include "..\WorldDefine\loot.h"
#include "..\WorldDefine\msg_loot.h"

CONST DWORD KEY_UP = 0;
CONST DWORD KEY_BACK = 1;
CONST DWORD KEY_LEFT = 2;
CONST DWORD KEY_RIGHT = 3;
CONST DWORD KEY_JUMP = 4;
CONST DWORD KEY_PICK = 5;
CONST DWORD KEY_SWITCH_TARGET = 6;

CONST DWORD TAB_SELECT_RANGE = 44;

CombatFrame_EventCreater::CombatFrame_EventCreater(void):
m_trunk(this),
m_dwAutoClearTimeMark(0),
m_dwLastTabTime(0)
{
}

CombatFrame_EventCreater::~CombatFrame_EventCreater(void)
{
}

BOOL CombatFrame_EventCreater::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	//--注册热键处理函数
	m_pKeyMap->Register(KEY_UP, g_StrTable[_T("Hotkey_Up")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&CombatFrame_EventCreater::OnKey),DIK_W);
	m_pKeyMap->Register(KEY_BACK, g_StrTable[_T("Hotkey_Back")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&CombatFrame_EventCreater::OnKey),DIK_S);
	m_pKeyMap->Register(KEY_LEFT, g_StrTable[_T("Hotkey_Left")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&CombatFrame_EventCreater::OnKey),DIK_A);
	m_pKeyMap->Register(KEY_RIGHT, g_StrTable[_T("Hotkey_Right")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&CombatFrame_EventCreater::OnKey),DIK_D);
	m_pKeyMap->Register(KEY_JUMP, g_StrTable[_T("Hotkey_Jump")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&CombatFrame_EventCreater::OnKey),DIK_SPACE);
	m_pKeyMap->Register(KEY_PICK, g_StrTable[_T("Hotkey_Pick")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&CombatFrame_EventCreater::OnKey),DIK_GRAVE);
	m_pKeyMap->Register(KEY_SWITCH_TARGET, g_StrTable[_T("Hotkey_SwitchTarget")], (INPUTMAPEVENTHANDLE)m_trunk.sfp2(&CombatFrame_EventCreater::OnKey),DIK_TAB);

	//--注册游戏事件处理函数
	pMgr->RegisterEventHandle(_T("tagRolePickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnRolePickEvent));

	pMgr->RegisterEventHandle(_T("tagRoleRClickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnRoleRClickEvent) );

	pMgr->RegisterEventHandle(_T("tagScenePickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnScenePickEvent));

	pMgr->RegisterEventHandle(_T("tagGroundItemPickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnGroundItemPickEvent));

	pMgr->RegisterEventHandle(_T("tagMouseMoveEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnMouseMoveEvent));

	//--
	m_bFrontKeyDown=m_bBackKeyDown=m_bLeftKeyDown=m_bRightKeyDown=FALSE;
	m_curTargetRoleID=GT_INVALID;
	m_bSelectEnemy=false;

	return GameFrame::Init(pMgr,this,0);
}

BOOL CombatFrame_EventCreater::Destroy()
{
	//--注销热键处理函数
	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_Up")], false);
	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_Back")], false);
	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_Left")], false);
	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_Right")], false);
	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_Jump")], false);
	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_Pick")], false);
	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_SwitchTarget")], false);

	//--注销游戏事件处理函数
	m_pMgr->UnRegisterEventHandler(_T("tagRolePickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnRolePickEvent));

	m_pMgr->UnRegisterEventHandler(_T("tagRoleRClickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnRoleRClickEvent) );

	m_pMgr->UnRegisterEventHandler(_T("tagScenePickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnScenePickEvent));

	m_pMgr->UnRegisterEventHandler(_T("tagGroundItemPickEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnGroundItemPickEvent));

	m_pMgr->UnRegisterEventHandler(_T("tagMouseMoveEvent"),(FRAMEEVENTPROC)m_trunk.sfp1(&CombatFrame_EventCreater::OnMouseMoveEvent));

	return GameFrame::Destroy();
}

VOID CombatFrame_EventCreater::Update()
{
	UpdateAttackerMap();
}


DWORD CombatFrame_EventCreater::OnKey( DWORD dwID, BOOL bDown )
{
	LocalPlayer* pRole=RoleMgr::Inst()->GetLocalPlayer();

	//--生成跳跃游戏事件
	if( dwID==KEY_JUMP && bDown )
	{
		if(!CombatActionChecker::Inst()->IfCanMove(true))
			return false;

		tagJumpEvent event(_T("tagJumpEvent"),NULL);
		event.start=pRole->GetPos();
		pRole->OnGameEvent(&event);
		return 0;
	}

	//--生成拾取游戏事件
	if( dwID==KEY_PICK && bDown )
	{
		if(!CombatActionChecker::Inst()->IfCanPickGroundItem(true))
			return 0;

		const tagGroundItem* pItem = GroundItemMgr::Inst()->GetNearestCanPickUpItem( pRole->GetPos(), X_DEF_AUTO_PICKUP_DIST );
		if( P_VALID( pItem ) )
		{
			tagPickupItemEvent event(_T("tagPickupItemEvent"),NULL);
			event.n64ItemID = pItem->n64Serial;
			pRole->OnGameEvent(&event);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("PickGroundItemNone")] );
		}
		return 0;
	}

	//--生成选择目标游戏事件
	if( dwID==KEY_SWITCH_TARGET && bDown )
	{
		if(Kernel::Inst()->GetAccumTimeDW() - m_dwLastTabTime >= 500)	//至少500毫秒允许Tab一次怪
		{
			m_dwLastTabTime = Kernel::Inst()->GetAccumTimeDW();

			DWORD dwTarID = GetNextTarget();
			if( GT_INVALID != dwTarID )
			{
				tagRolePickEvent evt( _T("tagRolePickEvent"), this );
				evt.eType = EWPT_LClick;
				evt.dwRoleID = dwTarID;
				m_pMgr->SendEvent( &evt );
			}
		}
		
	}

	//--生成键盘移动游戏事件
	bool bKeyPressOrUp=false;
	if(dwID==KEY_UP)
	{
		m_bFrontKeyDown=bDown;
		bKeyPressOrUp=true;
	}
	else if(dwID==KEY_BACK)
	{
		m_bBackKeyDown=bDown;
		bKeyPressOrUp=true;
	}
	else if(dwID==KEY_LEFT)
	{
		m_bLeftKeyDown=bDown;
		bKeyPressOrUp=true;
	}
	else if(dwID==KEY_RIGHT)
	{
		m_bRightKeyDown=bDown;
		bKeyPressOrUp=true;
	}
	if(bKeyPressOrUp)
	{
		if(m_bFrontKeyDown||m_bBackKeyDown||m_bLeftKeyDown||m_bRightKeyDown)
		{
			tagKeyboardMoveEvent event(_T("tagKeyboardMoveEvent"),NULL);
			event.start=pRole->GetPos();
			if(m_bFrontKeyDown && m_bLeftKeyDown)
				event.dir=EWalkDir_LeftFront;
			else if(m_bFrontKeyDown &&m_bRightKeyDown)
				event.dir=EWalkDir_RightFront;
			else if(m_bLeftKeyDown && m_bBackKeyDown)
				event.dir=EWalkDir_LeftBack;
			else if(m_bBackKeyDown && m_bRightKeyDown)
				event.dir=EWalkDir_RightBack;
			else if(m_bFrontKeyDown)
				event.dir=EWalkDir_Front;
			else if(m_bBackKeyDown)
				event.dir=EWalkDir_Back;
			else if(m_bLeftKeyDown)
				event.dir=EWalkDir_Left;
			else if(m_bRightKeyDown)
				event.dir=EWalkDir_Right;
			else
				return 0;

			if(!CombatActionChecker::Inst()->IfCanMove(true))
				return false;

			pRole->OnGameEvent(&event);
		}
		else
		{
			if(!CombatActionChecker::Inst()->IfCanMove(true))
				return false;

			tagStopKeyboardMoveEvent stopEvent(_T("tagStopKeyboardMoveEvent"),NULL);
			pRole->OnGameEvent(&stopEvent);
		}

		return 0;
	}

	return 0;
}

DWORD CombatFrame_EventCreater::OnRolePickEvent( tagRolePickEvent* pEvent )
{
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	Role* pPickRole = NULL;

	if( GT_INVALID == pEvent->dwRoleID )
	{
	}
	else
	{
		pPickRole = RoleMgr::Inst()->FindRole( pEvent->dwRoleID );
		if( m_curTargetRoleID == pEvent->dwRoleID )
		{
			if(CombatActionChecker::Inst()->IfCanAttack(pEvent->dwRoleID))
			{
				DWORD normalSkillID=SkillMgr::Inst()->GetNormalAttackTypeID();

				if(CombatActionChecker::Inst()->IfCanUseSkill(normalSkillID,pEvent->dwRoleID,true))
				{
					// 产生普通攻击事件
					tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL); 
					event.dwTargetRoleID = pEvent->dwRoleID;
					event.dwSkillID = normalSkillID;
					pLocalPlayer->OnGameEvent(&event);
					if( RoleMgr::Inst()->IsEnemy(pEvent->dwRoleID) )
					{
						BeAttackFrame *pFrame = (BeAttackFrame*)m_pMgr->GetFrame(_T("BeAttack"));
						if(P_VALID(pFrame))
							pFrame->SetRenderFlag(FALSE);
					}
				}
			}
			else if( pPickRole->IS_KIND_OF(NPC) ) //NPC
			{
				NPC* pNPC=(NPC*)pPickRole;

				if(pNPC->IsNPC()||pNPC->IsNormalGameObject())//NPC对话
				{
					if(CombatActionChecker::Inst()->IfCanTalkNPC(true))
					{
						// 产生NPC对话事件
						tagTalkNPCEvent event(_T("tagTalkNPCEvent"),NULL);
						NPC *pRole=(NPC*)pPickRole;
						event.dwNPCID = pEvent->dwRoleID;
						event.dwTypeID = pRole->GetTypeID();
						pLocalPlayer->OnGameEvent(&event);
					}
				}
				else if(pNPC->IsGather())//可采集的资源
				{
					if( !P_VALID(pNPC->GetProto()) )
						return 0;

					DWORD skillID=SkillMgr::Inst()->GetGatherSkillTypeID(pNPC->GetProto()->nType3);
					if(skillID==GT_INVALID)
					{
						CombatSysUtil::Inst()->ShowActionCheckMsg(CombatActionChecker::EEC_NoSkill);
						return 0;
					}

					if(CombatActionChecker::Inst()->IfCanUseSkill(skillID,pEvent->dwRoleID,true))
					{
						// 产生采集技能事件
						tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL); 
						event.dwTargetRoleID = pEvent->dwRoleID;
						event.dwSkillID = skillID;
						pLocalPlayer->OnGameEvent(&event);
					}
				}
				else if(pNPC->IsChest())//挖宝的宝箱
				{
					if( !P_VALID(pNPC->GetProto()) )
						return 0;

					tagDigBaoEvent evt(_T("tagDigBaoEvent"), NULL);
					evt.dwTargetID = pEvent->dwRoleID;
					evt.dwTargetTypeID = pNPC->GetTypeID();
					TObjRef<GameFrameMgr>()->SendEvent(&evt);
				}
				else if(pNPC->IsMonster())//怪物
				{
					DWORD normalSkillID=SkillMgr::Inst()->GetNormalAttackTypeID();

					if(CombatActionChecker::Inst()->IfCanUseSkill(normalSkillID,pEvent->dwRoleID,true))
					{
						// 产生普通攻击事件
						tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL); 
						event.dwTargetRoleID = pEvent->dwRoleID;
						event.dwSkillID = normalSkillID;
						pLocalPlayer->OnGameEvent(&event);
					}
				}
				else if (pNPC->IsRemnant())
				{
					tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL);
					event.dwTargetRoleID = pEvent->dwRoleID;
					event.dwSkillID = 9000201;
					pLocalPlayer->OnGameEvent(&event);
				}
				else if(pNPC->IsFairySoul())
				{
					const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillData(FairyTrain_SKILL);
					if(!P_VALID(pSkillData))
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotTrainFairy")]);
						return 0;
					}
					tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL);
					event.dwTargetRoleID = pEvent->dwRoleID;
					event.dwSkillID = pSkillData->dwTypeID;
					pLocalPlayer->OnGameEvent(&event);
				}
				else if(pNPC->IsFairyChest())
				{
					const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillData(FAIRY_CHEST_SKILL);
					if(!P_VALID(pSkillData))
						return 0;
					tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL);
					event.dwTargetRoleID = pEvent->dwRoleID;
					event.dwSkillID = pSkillData->dwTypeID;
					pLocalPlayer->OnGameEvent(&event);
				}
				else if( pNPC->IsCommonInvesGO() || 
					( pNPC->IsQuestInvesGO() && QuestMgr::Inst()->GetQuery()->IsInvesObject(pNPC->GetTypeID()) ) )//可调查地物
				{
					tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL); 
					event.dwTargetRoleID = pEvent->dwRoleID;
					
					DWORD dwTypeID = pNPC->GetTypeID();
					// 如果是龙珠
					if (dwTypeID == 4900029)
					{
						// 如果不是进攻状态也不是防御状态,不可封印龙珠
						if (!pLocalPlayer->IsInState(ES_CSAttack) && !pLocalPlayer->IsInState(ES_CSDefence))
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Error1")]);
							return 0;
						}
						event.dwSkillID = 9001501;
					} 
					// 如果是阴阳柱
					else if (dwTypeID == 4900030 || dwTypeID == 4900043 || dwTypeID == 4900044 || dwTypeID == 4900045 || dwTypeID == 4900046)
					{
						// 如果不是进攻状态也不是防御状态,不可封印阴阳柱
						if (!pLocalPlayer->IsInState(ES_CSAttack) && !pLocalPlayer->IsInState(ES_CSDefence))
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Error2")]);
							return 0;
						}
						event.dwSkillID = 9001601;
					}
					else if(dwTypeID == 4900048)
					{
						// 如果不是进攻状态也不是防御状态,不可召唤守护神
						if (!pLocalPlayer->IsInState(ES_CSDefence))
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Error3")]);
							return 0;
						}
						event.dwSkillID = 9001801;
					}
					// 普通可调查地物
					else
					{
						event.dwSkillID = 9001001;//调查技能ID
					}
					pLocalPlayer->OnGameEvent(&event);
				}
				else if(pNPC->IsQuestTalkGameObject())
					//&& ( QuestMgr::Inst()->GetQuery()->IsUpdateNPC(pNPC->GetTypeID())
					//|| QuestMgr::Inst()->GetQuery()->IsCompleteNPC(pNPC->GetTypeID()) ) )//任务对话地物
				{
					// 产生NPC对话事件
					tagTalkNPCEvent event(_T("tagTalkNPCEvent"),NULL);
					NPC *pRole=(NPC*)pPickRole;
					event.dwNPCID = pEvent->dwRoleID;
					event.dwTypeID = pRole->GetTypeID();
					pLocalPlayer->OnGameEvent(&event);
				}
			}
		}
	}

	const DWORD lastTarget = m_curTargetRoleID;
	m_curTargetRoleID = pEvent->dwRoleID;
	
	//选中角色事件
	if( NULL != pPickRole && lastTarget != m_curTargetRoleID )
	{
		tagTargetSelectEvent event;
			pPickRole->OnGameEvent(&event);
	}


	//判断当前是否选中敌人
	m_bSelectEnemy=CombatActionChecker::Inst()->IfCanAttack(m_curTargetRoleID);

	return 0;
}

DWORD CombatFrame_EventCreater::OnRoleRClickEvent( tagRoleRClickEvent* pEvent )
{
	//角色ID无效
	if( GT_INVALID == pEvent->dwRoleID )
		return 0;

	//没有发现角色
	Role* pRole = RoleMgr::Inst()->FindRole( pEvent->dwRoleID );
	if( !P_VALID(pRole) )
		return 0;

	if( IS_PLAYER( pRole->GetID() ) )
	{
		if( pRole->GetID() == RoleMgr::Inst()->GetLocalPlayerID() )
			return 0;

		//发送右键菜单事件
		tagPlayerPopupMenu _event;
		_event.dwRoleID = pRole->GetID();
		_event.pt = pEvent->pt;
		m_pMgr->SendEvent( &_event );
	}

	//可以扩展

	return 0;
}

DWORD CombatFrame_EventCreater::OnScenePickEvent( tagScenePickEvent* pEvent )
{
	if(!CombatActionChecker::Inst()->IfCanMove(true))
		return false;

	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();

	// 产生鼠标移动事件
	tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );				  
	event.start = pLocalPlayer->GetPos();
	event.end	= pEvent->vPickPos;
	pLocalPlayer->OnGameEvent(&event);

	return 0;
}

DWORD CombatFrame_EventCreater::OnGroundItemPickEvent( tagGroundItemPickEvent* pEvent )
{
	if(!CombatActionChecker::Inst()->IfCanPickGroundItem(true))
		return 0;

	tagGroundItem* pItem = GroundItemMgr::Inst()->FindItem( pEvent->pGroundItem->n64Serial );
	if( !P_VALID(pItem) )
		return 0;

	if( !GroundItemMgr::Inst()->CanPickUp(pItem) )
	{
		TCHAR szKey[100];
		_sntprintf( szKey, 100, _T("PickGroundItemErr%u"), E_Loot_Belong_To_You );
		const TCHAR* szMsg = g_StrTable[szKey];
		CombatSysUtil::Inst()->ShowScreenCenterMsg( szMsg );
		return 0;
	}

	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();

	// 产生拾取事件
	tagPickupItemEvent event(_T("tagPickupItemEvent"),NULL);
	event.n64ItemID = pEvent->pGroundItem->n64Serial;
	pLocalPlayer->OnGameEvent(&event);

	return 0;
}

DWORD CombatFrame_EventCreater::OnMouseMoveEvent(tagMouseMoveEvent* pEvent)
{
	if(!CombatActionChecker::Inst()->IfCanMove(true))
		return false;

	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	pLocalPlayer->OnGameEvent(pEvent);

	return 0;
}


class CmpRoles
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

DWORD CombatFrame_EventCreater::GetNextTarget()
{
	if( NULL == MapMgr::Inst()->GetSceneGraph() )
		return GT_INVALID;

	CameraBase* pCam = MapMgr::Inst()->GetSceneGraph()->GetCamera();
	if( NULL == pCam )
		return GT_INVALID;

	Frustum frustum;
	frustum.Extract( pCam->GetView(), pCam->GetPrj() );
	const map<DWORD,Role*>& mapRole = RoleMgr::Inst()->GetRoleMap();

	// 从角色管理器中提取可切换的角色列表
	bool bHaveCurTarget = false;
	list<Role*> listRoles;
	//  优先判断攻击者map
	if(!m_mapAttackingRoleMap.empty())
	{
		for( map<DWORD,Role*>::iterator iter = m_mapAttackingRoleMap.begin(); iter!=m_mapAttackingRoleMap.end();iter++ )
		{
			Role *pRole = RoleMgr::Inst()->FindRole(iter->first);
			if( !P_VALID(pRole) || !CanSwitchTargetRole( iter->second, &frustum ) )
				continue;

			listRoles.push_back( iter->second );
			if ( iter->first == m_curTargetRoleID )
				bHaveCurTarget = true;
		}
	}
	else // 没有攻击者，但是存在敌对玩家
	{
		for( map<DWORD,Role*>::const_iterator cItr = mapRole.begin(); cItr != mapRole.end(); cItr++ )
		{
			if( !CanSwitchTargetRole( cItr->second, &frustum ) )
				continue;
			// 判断是不是玩家
			if( !cItr->second->IS_KIND_OF(Player))
				continue;
			listRoles.push_back( cItr->second );
			if( cItr->first == m_curTargetRoleID )
				bHaveCurTarget = true;
		}
	}
	// 没有攻击者，也不存在存在敌对玩家则按以前的模式进行选择
	if( 0 == listRoles.size() )
	{
		for( map<DWORD,Role*>::const_iterator cItr = mapRole.begin(); cItr != mapRole.end(); cItr++ )
		{
			if( !CanSwitchTargetRole( cItr->second, &frustum ) )
				continue;

			listRoles.push_back( cItr->second );
			if( cItr->first == m_curTargetRoleID )
				bHaveCurTarget = true;
		}
	}

	if( 0 == listRoles.size() )
		return GT_INVALID;

	// 按距离排序
	CmpRoles CmpFuc;
	CmpFuc.refPos = RoleMgr::Inst()->GetLocalPlayer()->GetPos();
	listRoles.sort( CmpFuc );

	// 删除距离值超过22的元素
	for (list<Role*>::iterator it = listRoles.begin();it != listRoles.end();++it)
	{
		if(Vec3Dist(CmpFuc.refPos,(*it)->GetPos()) > TAB_SELECT_RANGE*TILE_SCALE )
		{
			listRoles.erase(it,listRoles.end());
			break;
		}
	}

	if( 0 == listRoles.size() )
		return GT_INVALID;

	DWORD dwNextID = GT_INVALID;
	// 列表中含有当前选中的目标，取下一个
	if( bHaveCurTarget )
	{
		for( list<Role*>::iterator iter = listRoles.begin(); iter != listRoles.end(); iter++ )
		{
			Role *pRole=(*iter);
			if( (*iter)->GetID() == m_curTargetRoleID )
			{
				iter++;
				if( iter == listRoles.end() )
					dwNextID = listRoles.front()->GetID();
				else
					dwNextID = (*iter)->GetID();
				break;
			}
		}
	}
	else
	{
		dwNextID = listRoles.front()->GetID();
	}

	// 避免重复选中
	if( dwNextID == m_curTargetRoleID )
		return GT_INVALID;

	return dwNextID;
}

bool CombatFrame_EventCreater::CanSwitchTargetRole(Role* pRole,const Frustum* pFrustum)
{
	if(!P_VALID(pRole))
		return false;

	if( pRole->IsDead() || pRole->IsHide() || !pRole->IsAttributeInited() )
		return false;

	if( pRole->GetRoleCampType() == RoleMgr::Inst()->GetLocalPlayer()->GetRoleCampType() && pRole->GetRoleCampType() !=	ECamp_Null )
		return false;

	if( pRole->IS_KIND_OF(NPC) )
	{
		NPC* pNPC = (NPC*)pRole;
		if( !pNPC->IsMonster() )
			return false;
	}
	if( !CombatActionChecker::Inst()->IfCanAttack( pRole->GetID() ) )
		return false;
	if( !pRole->GetSceneNode()->IsInFrustum( pFrustum ) )
		return false;

	return true;
}
VOID  CombatFrame_EventCreater::ClearAttackingRoleMap()
{
	m_mapAttackingRoleMap.clear();
}
BOOL  CombatFrame_EventCreater::AddNewAttacker(DWORD dwSrcRoleID,DWORD dwTarRoleID)
{
	//判断攻击目标是否是本地玩家
	if ( dwTarRoleID ==RoleMgr::Inst()->GetLocalPlayerID())
	{
		Role* pRole = RoleMgr::Inst()->FindRole(dwSrcRoleID, true);
		if(!P_VALID(pRole))
			return FALSE;
		// 防止重复插入相同的攻击者
		map<DWORD,Role*>::iterator it = m_mapAttackingRoleMap.find(dwSrcRoleID);
		if (it == m_mapAttackingRoleMap.end())
		{
			m_mapAttackingRoleMap.insert(make_pair(dwSrcRoleID,pRole));
			return TRUE;
		}
	}

	return FALSE;
}
BOOL  CombatFrame_EventCreater::DeleteNewAttacker(DWORD dwSrcRoleID)
{
	Role* pRole = RoleMgr::Inst()->FindRole(dwSrcRoleID, true);
	if(!P_VALID(pRole))
		return FALSE;
	// 查看攻击者列表中是否有这个attacker
	map<DWORD,Role*>::iterator it = m_mapAttackingRoleMap.find(dwSrcRoleID);
	if (it != m_mapAttackingRoleMap.end())
	{
		m_mapAttackingRoleMap.erase(it);
		return TRUE;
	}
	return FALSE;
}

VOID CombatFrame_EventCreater::UpdateAttackerMap()
{
	if(m_mapAttackingRoleMap.empty())
		return;

	if (Kernel::Inst()->GetAccumTimeDW() - m_dwAutoClearTimeMark > SelectTargetClearTime)
	{
		ClearAttackingRoleMap();
		m_dwAutoClearTimeMark = Kernel::Inst()->GetAccumTimeDW();
	}
}
