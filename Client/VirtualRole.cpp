#include "StdAfx.h"
#include "VirtualRole.h"
#include "FSM_RP.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CombatSysUtil.h"
#include "CombatEvent.h"
#include "ClientCamera.h"
#include "WorldFrame.h"
#include "AudioSys.h"
#include "MapMgr.h"
#include "EffectMgr.h"
#include "..\Cool3D\Util\CSVFile.h"
#include "HolyManMgr.h"
#include "GameSet.h"
#include "..\WorldDefine\msg_item.h"

IMP_RTTI(VirtualRole,Player);

VirtualRole::VirtualRole(void)
:m_bIsInitFSM(FALSE)
,m_eActionType(EPAT_NULL)
,m_eSoundType(EPST_NULL)
,m_dwEscapeTime(0)
,m_dwVRID(GT_INVALID)
,m_dwRoleID(GT_INVALID)
{
	/*for ( int i=0; i<EPST_TypeNum; ++i)
	{
		m_dwTimer[i] = 0;
	}*/
	m_dwTimer = 0;
}

VirtualRole::~VirtualRole(void)
{
}

void VirtualRole::InitFSM(DWORD dwRoleID)
{
	Player::InitFSM();
	m_bIsInitFSM = TRUE;
	m_dwRoleID = dwRoleID;
	m_bAttributeInited = true;
}

void VirtualRole::Update()
{
	FSM_RP*	pFSM = this->GetFSM();
	if ( P_VALID(pFSM) && IsInitFSM() && pFSM->IsDeadState() )
		this->Hide(TRUE);
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();//add by xp
		bool IsCallShengLingState=pPlayer->GetRoleState(ERS_CALLHOLY);//本地玩家是否处于召唤圣灵状态
		if( GameSet::Inst()->Get(EGS_ShowShengLing)&&IsCallShengLingState)
			this->Hide(FALSE);//显示圣灵
		else
			this->Hide(TRUE);//隐藏圣灵

	}
	else
	{
		Role* pRole = RoleMgr::Inst()->FindRole( m_dwRoleID);
		Player* pPlayer = (Player*)pRole;
		bool IsCallShengLingState1=pPlayer->GetRoleState(ERS_CALLHOLY);//远程玩家是否处于召唤圣灵状态
		if( GameSet::Inst()->Get(EGS_ShowShengLing)&&IsCallShengLingState1)
			this->Hide(FALSE);//显示圣灵
		else
			this->Hide(TRUE);//隐藏圣灵
	}//add end

	//--update action
	PlayAction();
	//--update pos
	SynchPos();
	//--update sound
	UpdateSound();
	//--update
	Player::Update();

}

void VirtualRole::OnGotoMap(const Vector3& pos,const Vector3 faceTo)
{
	//--设置位置
	//SetPos(pos);
	SynchPos();

	//--设置朝向
	float yaw=CalcYaw(faceTo);
	SetMoveYaw(yaw);
	SetYaw(yaw);

	//--状态机切换状态
	Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
	if ( !P_VALID(pR)  )  return;
	Player* pPlayer = NULL;
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
		pPlayer = (LocalPlayer*)pR;
	else if ( pR->IS_STATIC_CLASS(Player) )
		pPlayer = (Player*)pR;
	if ( P_VALID(pPlayer) )
	{
		FSM_RP*	pFSM = pPlayer->GetFSM();
		if ( P_VALID(pFSM) )
		{
			if ( pFSM->IsDeadState() )
				this->Hide(TRUE);
		}
	}

}

VOID VirtualRole::SynchPos()
{
	if ( m_bHide )
		return;

	Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
	if ( !P_VALID(pR)  )  return;

	Player* pPlayer = NULL;
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
		pPlayer = (LocalPlayer*)pR;
	else if ( pR->IS_STATIC_CLASS(Player) )
		pPlayer = (Player*)pR;

	if ( P_VALID(pPlayer)  )
	{
		//--位置和方向
		float fPara = HolyManMgr::Inst()->GetModelScale(m_dwVRID);
		Vector3 v3Pos = pPlayer->GetVRPos( fPara );
		SetPos(v3Pos);
	}
	
}

void VirtualRole::OnGameEvent(tagGameEvent* pGameEvent)
{
	Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
	if ( !P_VALID(pR)  )  return;
	Player* pPlayer = NULL;
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
		pPlayer = (LocalPlayer*)pR;
	else if ( pR->IS_STATIC_CLASS(Player) )
		pPlayer = (Player*)pR;

	FSM_RP*	pFSM = this->GetFSM();
	if ( P_VALID(pGameEvent) && P_VALID(pPlayer)&&P_VALID(pFSM) && IsInitFSM() )
	{
		if ( EPAT_CALL != m_eActionType && EPAT_DISPPEAR != m_eActionType ) //召唤和死亡态不能移动
		{
			if( pGameEvent->strEventName == _T("tagKeyboardMoveEvent") )  
			{
				tagKeyboardMoveEvent *pState = static_cast<tagKeyboardMoveEvent*>(pGameEvent);
				//--设置朝向
				ClientCamera* pCamera=GetWorldFrm()->GetCamera();
				Vector3 moveDir=CombatSysUtil::Inst()->CalcKeyMoveDir(pState->dir,pCamera->GetYaw());
				this->SetMoveYaw( CalcYaw( moveDir ) );
				if( pFSM->IfLowerCanSetYaw() )
					this->SetYaw(pPlayer->GetMoveYaw());

				//pPlayer->Update();
				SetState(EPAT_MOVE);
			}
			else if( pGameEvent->strEventName == _T("tagMouseMoveEvent") ) 
			{
				tagMouseMoveEvent *pState = static_cast<tagMouseMoveEvent*>(pGameEvent);
				//--设置朝向
				RotateVRole( pState->start, pState->end );
				//pPlayer->Update();
				SetState(EPAT_MOVE);
			}
			else if( pGameEvent->strEventName == _T("tagUseSkillEvent") )
			{
				tagUseSkillEvent *pState = static_cast<tagUseSkillEvent*>(pGameEvent);
				Role *pTarget = RoleMgr::Inst()->FindRole(pState->dwTargetRoleID);
				if (P_VALID(pTarget))
				{
					RotateVRole(pPlayer->GetPos(), pTarget->GetPos());
					
					Vector3 v3TarHread;
					pTarget->GetHeadPos(v3TarHread);
					this->FaceTo(v3TarHread);

					//pPlayer->Update();
					SetState(EPAT_FIGHT);
				}
			}
		}
		
		if ( pGameEvent->strEventName == _T("tagVirtualRoleEvent") )
		{
			tagVirtualRoleEvent *pEvent = static_cast<tagVirtualRoleEvent*>(pGameEvent);
			SetState(pEvent->ePlayAct);
			if ( EPAT_CALL == m_eActionType )
			{
				CallHolyManInit();
				Hide(FALSE);
				SetLocalSoundType(EPST_Calling);
			}
			else if ( EPAT_DISPPEAR == m_eActionType )
			{
				SetLocalSoundType(EPST_Disppear);
				//m_eSoundType = EPST_Disppear;
			}
			m_dwEscapeTime = Kernel::Inst()->GetAccumTimeDW();
		}
		else if ( pGameEvent->strEventName ==_T("tagBeAttackEvent") )
		{
			SetLocalSoundType(EPST_BeAttack);
			//m_eSoundType = EPST_BeAttack;
		}
		else if ( pGameEvent->strEventName ==_T("tagVRSkillEvent") )
		{
			SetLocalSoundType(EPST_Winning);
			//m_eSoundType = EPST_Winning;
		}

	}
}

void VirtualRole::OnNetCmd(tagNetCmd* pNetCmd)
{
	if ( EPAT_CALL == m_eActionType || EPAT_DISPPEAR == m_eActionType )
		return ;

	Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
	if ( !P_VALID(pR)  )  return;
	Player* pPlayer = NULL;
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
		pPlayer = (LocalPlayer*)pR;
	else if ( pR->IS_STATIC_CLASS(Player) )
		pPlayer = (Player*)pR;

	FSM_RP*	pFSM = this->GetFSM();
	if ( P_VALID(pNetCmd) && P_VALID(pPlayer)&&P_VALID(pFSM) && IsInitFSM() )
	{
		if ( EPAT_CALL != m_eActionType && EPAT_DISPPEAR != m_eActionType ) //召唤和死亡态不能使用技能
		{
			if ( pNetCmd->dwID==pNetCmd->Crc32("NS_Skill") )
			{
				tagNS_Skill* pCmd=(tagNS_Skill*)pNetCmd;
				//--设置朝向
				Role* pRSrc = RoleMgr::Inst()->FindRole( pCmd->dwSrcRoleID );
				Role* pRTar = RoleMgr::Inst()->FindRole( pCmd->dwTarRoleID ); 
				if ( P_VALID(pRSrc) && P_VALID(pRTar) )
				{
					RotateVRole(pRSrc->GetPos(), pRTar->GetPos());

					Vector3 v3TarHread;
					pRTar->GetHeadPos(v3TarHread);
					this->FaceTo(v3TarHread);
				}

				Player::Update();
				SetState(EPAT_FIGHT);

			}
			else if ( pNetCmd->dwID==pNetCmd->Crc32("NS_SyncWalk") )
			{	
				tagNS_SyncWalk *pState = (tagNS_SyncWalk*)pNetCmd; 
				if ( pState->dwRoleID != RoleMgr::Inst()->GetLocalPlayerID()/*!bSwim*/ ) // 本地不用这个消息同步
				{
					//--设置朝向
					RotateVRole( pState->srcPos, pState->dstPos );
					Player::Update();
					SetState(EPAT_MOVE);
				}
			}
// 			else if( pNetCmd->dwID == pNetCmd->Crc32("NS_CallShengLingDisappear"))
// 			{
// 				tagNS_CallShengLingDisappear* pDisCmd = (tagNS_CallShengLingDisappear*)pNetCmd;
// 				Hide(TRUE);
// 			}
		}
	}

}

VOID VirtualRole::FaceToTarget( Player* pSelf,Role* pTarget,BOOL bMoveable, BOOL bAniBlend )
{
	Vector3 dir=pTarget->GetPos()-pSelf->GetPos();

	if(bMoveable)
	{
		if(bAniBlend)//上下身分开
		{
			//--如果是站立状态，面向目标
			if(pSelf->IsStandState())
				pSelf->SetYaw(CalcYaw(dir));
			//--旋转上身
			Vector3 targetPos;
			pTarget->GetHeadPos(targetPos);
			pSelf->FaceTo(targetPos,FALSE);
		}
		else//滑动战斗
			pSelf->SetYaw(CalcYaw(dir));
	}
	else
		pSelf->SetYaw(CalcYaw(dir));

}

BOOL VirtualRole::RotateVRole( Vector3 srcPos,  Vector3 dstPos )
{
	Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
	if ( !P_VALID(pR)  )  return FALSE;
	Player* pPlayer = NULL;
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
		pPlayer = (LocalPlayer*)pR;
	else if ( pR->IS_STATIC_CLASS(Player) )
		pPlayer = (Player*)pR;

	FSM_RP*	pFSM = this->GetFSM();
	
	if ( !P_VALID(pFSM) || !IsInitFSM() || !P_VALID(pPlayer) )
		return FALSE;

	float fPara = HolyManMgr::Inst()->GetModelScale(m_dwVRID);
	Vector3  _srcPos = srcPos;  
	Vector3  _dstPos = dstPos;  
	_srcPos.y+= VR_HEIGHT_OFFSET*fPara;  
	_dstPos.y+= VR_HEIGHT_OFFSET*fPara;  
	Vector3 dir = _dstPos - _srcPos;
	this->SetMoveYaw(CalcYaw(dir));
	if( pFSM->IfLowerCanSetYaw() )
	{
		this->SetYaw(pPlayer->GetMoveYaw());
	}
	Player::Update();
	return TRUE;
}

VOID  VirtualRole::PlayAction()
{
	if ( m_bHide && EPAT_CALL != m_eActionType )
		return;

	Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
	if ( !P_VALID(pR)  )  return;
	
	Player* pPlayer = NULL;
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
		pPlayer = (LocalPlayer*)pR;
	else if ( pR->IS_STATIC_CLASS(Player) )
		pPlayer = (Player*)pR;

	FSM_RP*	pFSM = this->GetFSM();
	if ( !P_VALID(pFSM) || !IsInitFSM() || !P_VALID(pPlayer) )
		return ;
	//召唤2m进入站立动作，消失4m后淡出
	DWORD dwTimeLap = Kernel::Inst()->GetAccumTimeDW() - m_dwEscapeTime;
	if ( dwTimeLap > 2000 && EPAT_CALL==m_eActionType )
	{
		SetState(EPAT_STAND);
	}
	else 
	{
		switch(m_eActionType)
		{
		case EPAT_MOVE:		
			{
				//--播放动作
				BOOL bNotFighting = TransformIsTrackEnd( m_strActionName.c_str() ); 
				if ( bNotFighting ) //战斗动画优先播放完
				{
					const TCHAR* szTrackName= GetVRMoveTrackName( pPlayer->GetMoveSpeed() );
					this->TransformPlayTrack( szTrackName, true, false, 0.0f );
					//GetAniCtrl()->OpenTrack(_T("wlk1"), true, 0.0f, 0.2f);
				}
				SetState(EPAT_STAND);
			}
			break;
		case EPAT_FIGHT:		
			{
				Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
				RotateVRole(this->GetPos(), pR->GetPos());

				if(_tcscmp(m_strActionName.c_str(), _T("std")) == 0)
					this->TransformPlayTrack( m_strActionName.c_str(), true, false, 0.0f );
				else
					this->TransformPlayTrack( m_strActionName.c_str(), false, false, 0.0f );
				SetState(EPAT_STAND);
				SetLocalSoundType(EPST_Fighting);
				//m_eSoundType = EPST_Fighting;
			}
			break;
		case EPAT_CALL:		
			{
				const tagAvatarAtt lpAtt = pPlayer->GetAvatarAtt();
				const TCHAR* szTrackName = _T("call"); 	
				SynchPos();
				bool bifCanPlay = TransformIsTrackEnd( szTrackName );
				if(bifCanPlay)
					this->TransformPlayTrack( szTrackName, false, false, 0.0f );

				//消失音效特殊处理
				DWORD dwEscape = Kernel::Inst()->GetAccumTimeDW() - m_dwTimer;
				if ( dwEscape > 5000 && !m_bHide && m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
				{
					PlayVRSound( EPST_Calling );
					m_dwTimer = Kernel::Inst()->GetAccumTimeDW(); 
					m_eSoundType = EPST_NULL;
				}
			}
			break;
		case EPAT_DISPPEAR:
			{
				const tagAvatarAtt lpAtt = pPlayer->GetAvatarAtt();
				const TCHAR* szTrackName = _T("disa"); 
				this->TransformPlayTrack( szTrackName, false, false, 0.0f );

				//消失音效特殊处理
				DWORD dwEscape = Kernel::Inst()->GetAccumTimeDW() - m_dwTimer;
				if ( dwEscape > 5000 && !m_bHide && m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
				{
					PlayVRSound( EPST_Disppear );
					m_dwTimer = Kernel::Inst()->GetAccumTimeDW(); 
					m_eSoundType = EPST_NULL;
				}

				if ( dwTimeLap > 2000 && EPAT_DISPPEAR==m_eActionType )
				{
					Hide(TRUE);
					SetState(EPAT_STAND,TRUE);
				}

			}
			break;
		case EPAT_STAND:
			{
				if ( pPlayer->IsDead() ) 
				{
					SetState(EPAT_DISPPEAR);
					m_dwEscapeTime = Kernel::Inst()->GetAccumTimeDW();
				}
				else
				{
					BOOL bNotFighting = TransformIsTrackEnd( m_strActionName.c_str() );
					if ( bNotFighting ) //战斗动画优先播放完
					{
						if( ( pPlayer->IsIdleState() ) && pFSM->IfCanPlayLowerAni() )
						{
							const TCHAR* szTrackName = _T("std"); 
							this->TransformPlayTrack( szTrackName, true, false, 0.0f );
							SetLocalSoundType(EPST_Leisure);
							//m_eSoundType = EPST_Leisure;
						}
						else if ( pPlayer->IsMoveState() )
							SetState(EPAT_MOVE);
					}
				}
			}
			break;
			//case EPAT_NULL:
		default: 
			{

			}
			break;
		}
	}

	
}

void VirtualRole::Hide( const bool bHide )
{

	if( bHide == m_bHide )
		return;

	m_bHide = bHide;
	if( P_VALID( m_pAvatarNode ) )
		m_pAvatarNode->Hide( TRUE );
 	if ( P_VALID(m_pEntityNode) )
 		m_pEntityNode->Hide( m_bHide );
	if ( !bHide )
		ShowVREffect();
	else
		CloseVREffect();
	Role *pR = RoleMgr::Inst()->FindRole(m_dwRoleID);
	RotateVRole(this->GetPos(), pR->GetPos());
}

const TCHAR* VirtualRole::GetVRMoveTrackName( float speed )
{
	return _T("walk");

	if(speed<300.0f)
		return _T("walk");
	else 
		return _T("run");
}

//同步虚拟角色和人物作战动作
VOID VirtualRole::SynCombatAction(tstring strActionName)
{
	if (strActionName.empty())
		return;
	m_strActionName = strActionName;
	SetState(EPAT_FIGHT);
	Player::Update();
}

//召唤时初始化元神模型
VOID VirtualRole::CallHolyManInit()
{
	float fPara = HolyManMgr::Inst()->GetModelScale(m_dwVRID);
	Vector3 v3( fPara, fPara, fPara );
	this->SetScale(v3);
	SynchPos();

}

//刷新声效控制
VOID VirtualRole::UpdateSound()
{
	//元神为激活 或者 不是自己元神的不用播音效
	if ( m_bHide || m_dwRoleID != RoleMgr::Inst()->GetLocalPlayerID()
		|| EPST_Disppear == m_eSoundType || EPST_Calling == m_eSoundType  )
		return;

	if ( m_eSoundType< EPST_TypeNum )
	{
	/*	if ( m_eSoundType > EPST_TimeEnd )
		{
			PlayVRSound( m_eSoundType );
		}
		else
		{
			for ( int i=0; i<=EPST_TimeEnd; ++i)
			{
				DWORD dwEscape = Kernel::Inst()->GetAccumTimeDW() - m_dwTimer[i];
				if ( dwEscape >30000 && m_eSoundType == EPLaySoundType(i) )
				{
					PlayVRSound( m_eSoundType );
					m_dwTimer[i] = Kernel::Inst()->GetAccumTimeDW(); 
				}
			}
		}*/
		DWORD dwEscape = Kernel::Inst()->GetAccumTimeDW() - m_dwTimer;
		if ( dwEscape >30000 && m_eSoundType != EPST_NULL )
		{
			PlayVRSound( m_eSoundType );
			m_dwTimer = Kernel::Inst()->GetAccumTimeDW(); 
		}
	}

	m_eSoundType = EPST_NULL;

}

//播放声效
VOID VirtualRole::PlayVRSound( EPLaySoundType eType)
{
	switch (eType)
	{
	case EPST_Fighting:
			PlayRandSound( 1, 2, 3 );
		break;
	case EPST_Winning:
			PlayRandSound( 2, 2, 4 );
		break;
	case EPST_Leisure:
			PlayRandSound( 3, 2, 3 );
		break;
	case EPST_BeAttack:
			PlayRandSound( 6, 2, 3 );
		break;
	case EPST_Disppear:
			PlayRandSound( 4, 1, 3 );
		break;
	case EPST_Calling:
			PlayRandSound( 5, 2, 4 );
		break;
	}
}

//按性别和声效数目选择随机播放声效
VOID VirtualRole::PlayRandSound( INT nSound, INT nMaleRandNum, INT nFemaleRandNum )
{
	if ( 0==nSound || nSound>EPST_TypeNum || 0==nMaleRandNum || 0==nFemaleRandNum )
		return;
	tstringstream tssName;
	INT nRand = 0;
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if ( P_VALID(pLP) )
	{
		if ( pLP->GetPlayerSex() == 0 ) //女
		{
			nRand = (m_pUtil->Rand()%nFemaleRandNum) + 1 ;
			tssName<<_T("female0");
		}
		else
		{
			nRand = (m_pUtil->Rand()%nMaleRandNum) + 1 ;
			tssName<<_T("man0");
		}
	}
	if ( 0 == nRand )
		return;
	tssName<<nSound<<_T("_")<<nRand;
	if (!tssName.str().empty())
	{
		Vector3 v3Pos = this->GetVRPos();
		//m_pAudioSys->Play3DSound(tssName.str().c_str(),50.0f,100.0f*20.0f,v3Pos,SOUND_LOUD);
		m_pAudioSys->Play2DSound(tssName.str().c_str());
	}
	
}

VOID VirtualRole::SetState(EPLayActionType eType, BOOL bForce /*= FALSE*/)
{
	//只有召唤能打断消失
	if ( EPAT_DISPPEAR == m_eActionType && EPAT_CALL != eType && FALSE == bForce )
		return;
	m_eActionType = eType;
}

// 加载简单模型（无装备属性等）---用于虚拟角色
bool VirtualRole::LoadVirtualAvatar( tstring strResName)
{
	SAFE_DELETE(m_pAvatarNode);
	m_pAvatarNode = new AvatarEquipNode; 
	m_pAvatarNode->LoadDefault();

	DetachSceneNodeFromSG(MapMgr::Inst()->GetSceneGraph() );
	tstringstream stream;
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	stream << _T("data\\animation\\npc\\") << strResName<< _T("\\std.fskel"); //默认
	if(pIFS->GetSize(stream.str().c_str())>0)
	{
		LoadTransformModel(stream.str().c_str(), strResName.c_str() );
		int i=1;
		tstring szSkinPath = GetTransformSkinPath( strResName.c_str(), i++ );			
		DWORD fileSize = pIFS->GetSize(szSkinPath.c_str());
		while( 0 != fileSize && GT_INVALID != fileSize )
		{
			this->LoadTransformSkin(szSkinPath.c_str());
			szSkinPath = GetTransformSkinPath( strResName.c_str(), i++);
			fileSize = pIFS->GetSize(szSkinPath.c_str());
		}	

	}
	AttachSceneNodeToSG(MapMgr::Inst()->GetSceneGraph());

	return TRUE;

}

VOID VirtualRole::LoadVirtualEffect( tstring strName )
{
	// 挂载特效
	//TCHAR szFile[255];
	//_stprintf( szFile, _T("data\\animation\\npc\\Effect\\%s.csv"), GetTypeID() );
	m_mapVREffect.clear();
	tstringstream tss;
	tss<<_T("data\\animation\\npc\\Effect\\")<<strName<<_T(".csv");
	tstring str = tss.str();
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	DWORD dwFileSize=pIFS->GetSize( str.c_str() );
	if(dwFileSize>0 && dwFileSize!=GT_INVALID)
	{
		CSVFile csvFile;
		if(csvFile.OpenFile( pIFS, str.c_str() ))
		{
			//只有一行
			csvFile.BeginReadLine();
			const vector<tstring>& line = csvFile.GetNextLine();
			int numToken=line.size();
			for(int i=0;i<numToken;i+=2)
			{
				if(i+1<numToken)
				{
					tstring strNode = line[i].c_str(); 
					tstring strEffect = line[i+1].c_str(); 
					m_mapVREffect.insert(make_pair(strNode, strEffect));
					//EffectMgr::Inst()->PlayVirtualEffect( m_dwRoleID, line[i+1].c_str(), line[i].c_str(), EffectMgr::EEST_RoleScale, NULL );
				}
			}
			csvFile.CloseFile();
		}
	}	
}

//显示模型特效
VOID VirtualRole::ShowVREffect()
{
	EffectMgr::Inst()->StopVirtualEffect(m_dwRoleID);
	std::map<tstring,tstring>::iterator it = m_mapVREffect.begin();
	for ( ; it!=m_mapVREffect.end(); ++it )
	{
		tstring strEffect  = it->second; 
		tstring strNode = it->first; 
		if ( strEffect.empty() || strNode.empty() )
			continue;
		EffectMgr::Inst()->PlayVirtualEffect( m_dwRoleID, strEffect.c_str(), strNode.c_str(), EffectMgr::EEST_RoleScale, NULL );
	}
	
}

//不显示模型特效
VOID VirtualRole::CloseVREffect()
{
	EffectMgr::Inst()->StopVirtualEffect(m_dwRoleID);
}

VOID VirtualRole::SetLocalSoundType( EPLaySoundType eType )
{
	if ( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
		m_eSoundType = eType;
	else
		m_eSoundType = EPST_NULL;
}