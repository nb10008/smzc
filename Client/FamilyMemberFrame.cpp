#include "StdAfx.h"
#include "..\WorldDefine\msg_family.h"
#include "..\WorldDefine\family_define.h"
#include "FamilyMemberFrame.h"
#include "FamilyFrame.h"
#include "IconStatic.h"
#include "FamilyMgr.h"
#include "MapLogicData.h"
#include "MapMgr.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "PlayerNameTab.h"
#include "TeamSys.h"
#include "TeamEvent.h"
#include "FilterData.h"
#include "FamilyEvent.h"
#include "CombatSysUtil.h"


const TCHAR *szBack_Black	= _T("data\\ui\\Main\\L_icon_back.bmp");		//黑框
const TCHAR *szBack_New		= _T("data\\ui\\compet\\compet_xinshou.bmp");	//新手
const TCHAR *szBack_Bow		= _T("data\\ui\\compet\\compet_xuanling.bmp");	//玄翎
const TCHAR *szBack_Bar		= _T("data\\ui\\compet\\compet_shenbing.bmp");	//神兵
const TCHAR *szBack_Doc		= _T("data\\ui\\compet\\compet_xianyin.bmp");	//仙音
const TCHAR *szBack_Sor		= _T("data\\ui\\compet\\compet_qiankun.bmp");	//乾坤

const TCHAR *szBack_Man		= _T("data\\ui\\family\\male.bmp");				//男
const TCHAR *szBack_Wem		= _T("data\\ui\\family\\female.bmp");			//女

static INT g_MaxRoleNameLength = 0;

FamilyMemberFrame::FamilyMemberFrame(void)
:m_trunk(this)
,m_pBtnTeam(NULL)
,m_pBtnInvite(NULL)
,m_pBtnReplace(NULL)
,m_pBtnDismiss(NULL)
,m_pBtnLeave(NULL)
,m_dwCurIndex(GT_INVALID)
{
	for (int i=0; i<MAX_FAMILY_MEMBER; ++i)
	{
		m_pPchMember[i] =	NULL;		
		m_pStcProfes[i]	=	NULL;			
		m_pStcLevel[i]	=	NULL;		
		m_pStcSex[i]	=	NULL;		
		m_pStcName[i]	=	NULL;		
		m_pStcPlace[i]	=	NULL;				
		m_pStcTips[i]	=	NULL;		
		m_pStcCurPick[i]=	NULL;

		m_pProHP[i]		=	NULL;			
		m_pProMP[i]		=	NULL;
	}

	for (int j=0; j<MAX_HEART_NUM; ++j )
	{
		m_pStcHeart[j]	=	NULL;
	}




}

FamilyMemberFrame::~FamilyMemberFrame(void)
{
}

BOOL FamilyMemberFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_dwCurIndex = GT_INVALID;

	//家族成员名字长度限制
	TObjRef<VarContainer> pVarContainer = CreateObj("CreateRole_Family", "VarContainer");
	pVarContainer->Load(NULL, _T("config\\CreateRole.xml"));
	g_MaxRoleNameLength = pVarContainer->GetDword(_T("Role_Name_Length"));
	KillObj("CreateRole_Family");


	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_InputRoleName"), (FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMemberFrame::OnMsgBox_InputRoleName));
	

	return TRUE;
}

BOOL FamilyMemberFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	if(P_VALID(GetObj("InviteRole_NameInput")))
		KillObj("InviteRole_NameInput");

	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_InputRoleName"),	(FRAMEEVENTPROC)m_trunk.sfp1(&FamilyMemberFrame::OnMsgBox_InputRoleName));
	
	return bRet;
}

BOOL FamilyMemberFrame::ReloadUI()
{
	m_pWndFather = m_pGUI->GetWnd(((FamilyFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("memberback"));

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&FamilyMemberFrame::EventHandler));

	TCHAR buffer[X_LONG_NAME];
	for (int i=0; i<MAX_FAMILY_MEMBER; ++i)
	{
		if ( i != 0 )
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("player%d"), i-1 );
		else//家长
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("leader") );

		m_pPchMember[i] = (GUIPatch*)m_pWnd->GetChild(buffer);		
		m_pStcProfes[i]	=	(IconStatic*)m_pPchMember[i]->GetChild(_T("jobback\\jobpic"));			
		m_pStcLevel[i]	=	(GUIStatic*)m_pPchMember[i]->GetChild(_T("level"));		
		m_pStcSex[i]	=	(IconStatic*)m_pPchMember[i]->GetChild(_T("sex"));		
		m_pStcName[i]	=	(GUIStatic*)m_pPchMember[i]->GetChild(_T("name"));		
		m_pStcPlace[i]	=	(GUIStatic*)m_pPchMember[i]->GetChild(_T("place"));				
		m_pStcTips[i]	=	(GUIStatic*)m_pPchMember[i]->GetChild(_T("farpic"));	
		m_pStcCurPick[i]=	(GUIStatic*)m_pPchMember[i]->GetChild(_T("choose"));	
		m_pProHP[i]		=	(GUIProgress*)m_pPchMember[i]->GetChild(_T("hpbar\\pro"));			
		m_pProMP[i]		=	(GUIProgress*)m_pPchMember[i]->GetChild(_T("mpbar\\pro"));		
	}

	for (int j=0; j<MAX_HEART_NUM; ++j)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("heart%d"), j );
		m_pStcHeart[j]	=	(GUIStatic*)m_pWnd->GetChild(buffer);	
		m_pStcHeart[j]->SetTopMost(TRUE);
	}

	m_pBtnTeam			=	(GUIButton*)m_pWnd->GetChild(_T("team"));	
	m_pBtnInvite		=	(GUIButton*)m_pWnd->GetChild(_T("invite"));	
	m_pBtnReplace		=	(GUIButton*)m_pWnd->GetChild(_T("remove"));	
	m_pBtnDismiss		=	(GUIButton*)m_pWnd->GetChild(_T("dismiss"));	
	m_pBtnLeave			=	(GUIButton*)m_pWnd->GetChild(_T("leave"));	

	
	
	return TRUE;

}

BOOL FamilyMemberFrame::EscCancel()
{
	return FALSE;
}

VOID FamilyMemberFrame::Update()
{

}

DWORD FamilyMemberFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnTeam)
			{
				SendTeamApply();
			}
			else if (pWnd == m_pBtnInvite)
			{
				SendInviteInput();	
			}
			else if (pWnd == m_pBtnReplace)
			{
				SendLeaderChange();
			} 
			else if (pWnd == m_pBtnDismiss)
			{
				SendDismisPlayer();
			} 
			else if (pWnd == m_pBtnLeave)
			{
				SendMemberLeave();
			} 
			
		}
		break;
	case EGUIE_DragStart:
		{
			for (int i=0; i<MAX_FAMILY_MEMBER; ++i)
			{
				if (	pWnd == m_pPchMember[i]
				||	pWnd == m_pStcProfes[i]
				||	pWnd == m_pStcLevel[i]
				||	pWnd == m_pStcSex[i]
				||	pWnd == m_pStcName[i]
				||	pWnd == m_pStcPlace[i] 
				||	pWnd == m_pStcTips[i] 
				||	pWnd == m_pProHP[i] 
				||	pWnd == m_pProMP[i] )
				{
					m_dwCurIndex = i;
					//显示选中条
					m_pStcCurPick[i]->SetInvisible(FALSE);
					for (int j=0; j<MAX_FAMILY_MEMBER; ++j)
					{
						if (j!=i)
						{
							m_pStcCurPick[j]->SetInvisible(TRUE);	
							if(!m_pStcHeart[j/2]->IsInvisible())
							{
								m_pStcHeart[j/2]->FlipToTop();
							}		
						}
					}
				}

			}
			 
		}
		break;
	case EGUIE_Drag:
		{

		}
		break;
	}
	return 0;
}

VOID FamilyMemberFrame::ShowWnd(BOOL bShow)
{
	if (!P_VALID(m_pWnd))
		return ;

	ClearUI();
	m_dwCurIndex = GT_INVALID;
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( bShow && P_VALID(pLp) )
	{
		DWORD dwFID = pLp->GetRoleFamilyID();
		if (dwFID!=GT_INVALID)
			FamilyMgr::Inst()->SendFamilyInfoReq(dwFID);

		m_pBtnTeam->SetEnable(FALSE);
		m_pBtnInvite->SetEnable(FALSE);
		m_pBtnReplace->SetEnable(FALSE);
		m_pBtnDismiss->SetEnable(FALSE);
		m_pBtnLeave->SetEnable(FALSE);
		
	}

	m_pWnd->SetInvisible(!bShow);
	m_pWnd->FlipToTop();
}

VOID FamilyMemberFrame::RefreshUI()
{
	if ( !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
		return;

	//清空界面先
	ClearUI();
	const std::map<DWORD, DWORD> mapFamily = FamilyMgr::Inst()->GetFamilyMemberIDList();
	
	std::map<DWORD, DWORD>::const_iterator cit = mapFamily.begin();
	
	for ( ; cit!=mapFamily.end(); ++cit)
	{
		DWORD	i = cit->first,		dwRoleID = cit->second;
		const tagRoleFamilyInfo *pInfo = FamilyMgr::Inst()->GetMemberByID(dwRoleID);
		if (!P_VALID(pInfo) || i<0 || i>=MAX_FAMILY_MEMBER)
			continue;

		if(FamilyMgr::Inst()->JudgeIsSameFamily(pInfo->dwLoverID))
		{
			m_pStcHeart[i/2]->SetInvisible(FALSE);
			m_pStcHeart[i/2]->FlipToTop();
		}
		//职业
		EClassType eType = pInfo->eClassType;
		if (eType<EV_Base||eType>EV_TianYi)
			eType = EV_Base;
		tstring strClass = GetClassTypePic(eType);
		m_pStcProfes[i]->SetPic(strClass.c_str());
		tstringstream tssClass;
		tssClass<<_T("ClassName_")<<((int)eType);
		m_pStcProfes[i]->SetText(g_StrTable[tssClass.str().c_str()]);	
		//等级
		tstringstream tssLv;
		tssLv<<_T("lv ")<<pInfo->nLevel;
		m_pStcLevel[i]->SetText(tssLv.str().c_str());
		//性别
		if ( 0 == (INT)pInfo->bySex )// 0:girl
			m_pStcSex[i]->SetPic(szBack_Wem);
		else
			m_pStcSex[i]->SetPic(szBack_Man);
		//姓名
		tstring strRoleName	= PlayerNameTab::Inst()->FindNameByID(pInfo->dwRoleID);
		if (!strRoleName.empty())
			m_pStcName[i]->SetText(strRoleName.c_str());	
		//场景		
		const tagMapLogicData* pLogicData = MapLogicData::Inst()->FindMapLogicData( pInfo->dwMapID );
		DWORD dwCurMapID = MapMgr::Inst()->GetCurMapID();
		DWORD dwColor = 0xFF888888;
		tstring strPlace = g_StrTable[_T("FamilyMember_NotOnline")];
		if (P_VALID(pLogicData))
		{
			if ( dwCurMapID == pInfo->dwMapID )//同场景
				dwColor = 0xFF00FF00;
			else if ( ENMT_Normal == pLogicData->eNormalType )	//野外
			//else if ( EMT_Normal == pLogicData->eMapType )	//野外
				dwColor = 0xFFFF0000;
			else
				dwColor = 0xFF0000FF;
			strPlace = pLogicData->szShowName;
		}
		m_pStcPlace[i]->SetTextColor(dwColor);
		m_pStcPlace[i]->SetText(strPlace.c_str());
		//血魔量
		if ( pInfo->nHp==GT_INVALID || pInfo->nMp==GT_INVALID )
		{
			m_pStcTips[i]->SetInvisible(FALSE);
			m_pProHP[i]->SetInvisible(TRUE);
			m_pProMP[i]->SetInvisible(TRUE);
		}
		else
		{
			m_pStcTips[i]->SetInvisible(TRUE);
			m_pProHP[i]->SetInvisible(FALSE);
			m_pProHP[i]->SetMaxValue((FLOAT)pInfo->nHpMax);
			m_pProHP[i]->SetValue((FLOAT)pInfo->nHp,TRUE);
			m_pProMP[i]->SetInvisible(FALSE);
			m_pProMP[i]->SetMaxValue((FLOAT)pInfo->nMpMax);
			m_pProMP[i]->SetValue((FLOAT)pInfo->nMp,TRUE);
		}
		m_pPchMember[i]->SetInvisible(FALSE);
	}

	//刷新按钮
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( P_VALID(pLp) && pLp->IsInFamily() )
	{
		if (pLp->IsRoleFamilyLeader())
		{
			m_pBtnTeam->SetEnable(TRUE);
			m_pBtnInvite->SetEnable(TRUE);
			m_pBtnReplace->SetEnable(TRUE);
			m_pBtnDismiss->SetEnable(TRUE);
			m_pBtnLeave->SetEnable(FALSE);
		}
		else
		{
			m_pBtnTeam->SetEnable(TRUE);
			m_pBtnInvite->SetEnable(TRUE);
			m_pBtnReplace->SetEnable(FALSE);
			m_pBtnDismiss->SetEnable(FALSE);
			m_pBtnLeave->SetEnable(TRUE);
		}

		//向主页面发送刷新消息
		tstring strFamilyName = FamilyMgr::Inst()->GetFamilyName(pLp->GetRoleFamilyID());
		if (!strFamilyName.empty())
		{
			tagFamilyNameEvent evt( _T("FamilyEvent_GetName"), this );
			evt.strFamilyName = strFamilyName;
			m_pFrameMgr->SendEvent( &evt );
		}

	}
	
}

VOID FamilyMemberFrame::ClearUI()
{
	for ( int i=0; i<MAX_FAMILY_MEMBER; ++i )
	{
		m_pStcProfes[i]->SetPic(szBack_Black);	
		m_pStcProfes[i]->SetText(_T(""));
		m_pStcLevel[i]->SetText(_T(""));
		m_pStcSex[i]->SetPic(szBack_Black);	
		m_pStcSex[i]->SetText(_T(""));			 	
		m_pStcName[i]->SetText(_T(""));			 
		m_pStcPlace[i]->SetText(_T(""));		 
		m_pStcTips[i]->FlipToTop();

		m_pProHP[i]->SetValue(0,TRUE);			 
		m_pProMP[i]->SetValue(0,TRUE);					 

		m_pStcCurPick[i]->SetInvisible(TRUE);	
		m_dwCurIndex = GT_INVALID;

		m_pPchMember[i]->SetInvisible(TRUE);	
	}

	for ( int j=0; j<MAX_HEART_NUM; ++j )
	{
		m_pStcHeart[j]->SetInvisible(TRUE);	
	}

}

const TCHAR* FamilyMemberFrame::GetClassTypePic(EClassType eType)
{
	switch (eType)
	{
	case EV_Blade:
	case EV_Fuhu:
	case EV_XuanJia:
		return szBack_Bar;
	case EV_Bow:
	case EV_HuanShi:
	case EV_ZhanXin:
		return szBack_Bow;
	case EV_Strength:
	case EV_LeiMang:
	case EV_GuaXiang:
		return szBack_Sor;
	case EV_Musician:
	case EV_ShenShi:
	case EV_TianYi:
		return szBack_Doc;
	default: // EV_Base新手
		return szBack_New;
		break;
	}
}

DWORD FamilyMemberFrame::OnMsgBox_InputRoleName( tagMsgInputBoxEvent* pGameEvent )
{
	if( MBF_OK == pGameEvent->eResult && MBIT_Name == pGameEvent->eInputType )
	{
		tstring tstrInput = pGameEvent->strInputName;

		//判断名字是否非法
		DWORD dwErrorCode = FilterData::Inst()->IsValidName(tstrInput.c_str(), g_MaxRoleNameLength);
		if(E_Success == dwErrorCode)
		{
			tagFamilyInviteEvent evt(_T("FamilyEvent_Invite"), this);
			evt.dwRoleID	= PlayerNameTab::Inst()->FindIDByName(tstrInput.c_str());
			evt.strRoleName	= tstrInput;
			m_pMgr->SendEvent(&evt);
		}
		else
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NameInput_Invalid")]);
	}
		
	return 0;

}

VOID  FamilyMemberFrame::SendTeamApply()
{
	//队伍中
	if (TeamSys::Inst()->IsInTeam())
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Family_InTeam")]);
		return;
	}
	// 竞技场、据点战禁止组队
	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pPlayer))
	{
		if(pPlayer->IsInState(ES_PvPAttack) || pPlayer->IsInState(ES_PvPDefence))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L8")]);
			return;
		}

		if(pPlayer->IsInState(ES_CSAttack) || pPlayer->IsInState(ES_CSDefence))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PvP_Err_L15")]);
			return;
		}
	}

	//在线的玩家才发送
	const std::map<DWORD, DWORD> mapFamily = FamilyMgr::Inst()->GetFamilyMemberIDList();
	std::map<DWORD, DWORD>::const_iterator cit = mapFamily.begin();
	for ( ; cit!=mapFamily.end(); ++cit)
	{
		DWORD dwRoleID = cit->second;
		const tagRoleFamilyInfo *pInfo = FamilyMgr::Inst()->GetMemberByID(dwRoleID);
		if ( !P_VALID(pInfo) )
			continue;
		const tagMapLogicData* pLogicData = MapLogicData::Inst()->FindMapLogicData( pInfo->dwMapID );
		if (P_VALID(pLogicData)) //查到场景表明在线
		{
			tagApplyJoinTeamEvent evt( _T("tagApplyJoinTeamEvent"), this );
			evt.dwRoleID = pInfo->dwRoleID;
			m_pMgr->SendEvent( &evt );
		}
	}
	
}

VOID  FamilyMemberFrame::SendInviteInput()
{
	if(P_VALID(GetObj("InviteRole_NameInput")))
		KillObj("InviteRole_NameInput");

	CreateObj("InviteRole_NameInput", "MsgInputBox");
	TObjRef<MsgInputBox>("InviteRole_NameInput")->Init(
		_T(""),g_StrTable[_T("Family_InviteRoleName")],
		_T("MsgBox_InputRoleName"),GT_INVALID, MBIT_Name, NULL, FALSE);
}

VOID  FamilyMemberFrame::SendLeaderChange()
{
	tagFamilyEvent evt( _T("FamilyEvent_Event"), this );
	evt.eEventType	= EFET_ChangeLeader;
	evt.dwRoleIndex = m_dwCurIndex;
	m_pMgr->SendEvent( &evt );
}

VOID  FamilyMemberFrame::SendDismisPlayer()
{
	tagFamilyEvent evt( _T("FamilyEvent_Event"), this );
	evt.eEventType	= EFET_MemberDismis;
	evt.dwRoleIndex = m_dwCurIndex;
	m_pMgr->SendEvent( &evt );
}

VOID  FamilyMemberFrame::SendMemberLeave()
{
	tagFamilyEvent evt( _T("FamilyEvent_Event"), this );
	evt.eEventType	= EFET_LeaveFamily;
	evt.dwRoleIndex = GT_INVALID;
	m_pMgr->SendEvent( &evt );
}

BOOL FamilyMemberFrame::IsShowing()
{
	BOOL bHide = TRUE;
	if (P_VALID(m_pWnd))
		bHide = m_pWnd->IsInvisible();

	return (!bHide);
}


