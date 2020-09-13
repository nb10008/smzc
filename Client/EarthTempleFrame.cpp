#include "StdAfx.h"
#include "EarthTempleFrame.h"
#include "MapMgr.h"
#include "MapLogicData.h"
#include "ZoneMapData.h"
#include "..\WorldDefine\action.h"
#include "..\WorldDefine\specfunc_define.h"
#include "ItemEvent.h"
#include "CombatSysUtil.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "SkillProEvent.h"
#include "..\WorldDefine\msg_combat.h"
#include "..\WorldDefine\skill_define.h"
#include "SkillMgr.h"

EarthTempleFrame::EarthTempleFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_pBn_Ok_Set(NULL),
m_pBn_Cancel_Set(NULL),
m_pBn_Close_Set(NULL),
m_pCb_Earth_Box(NULL),
m_pTree(NULL),
dwSelectMapID(NULL),
CurrItemID(NULL),
m_bIsSpelling(false),
m_dwTimeCounter(NULL),
m_pBnFrist(NULL),
m_pBnSecond(NULL),
m_bDirectOpen(FALSE)
{
	m_PosVector3 = Vector3(0,0,0);
	m_strTempMark.nMask = 0;
	m_strTempMark.nType = 0;
}

EarthTempleFrame::~EarthTempleFrame(void)
{
}

BOOL EarthTempleFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam );

	m_pFrameMgr->RegisterEventHandle( _T("tagSpecItemEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&EarthTempleFrame::OnOpenEarthTemple));
	m_pCmdMgr->Register("NS_EarthTemple",	(NETMSGPROC)m_Trunk.sfp2(&EarthTempleFrame::OnNS_EarthTemple),	_T("NS_EarthTemple"));

	m_pCmdMgr->Register("NS_Skill",						(NETMSGPROC)m_Trunk.sfp2(&EarthTempleFrame::OnNS_Skill),				 _T("NS_Skill"));

	InitMapInfo();

	FillMapListBox();

	if (!m_vecEarthMapShow.empty())
	{
		tagZoneMapShow &data = m_vecEarthMapShow[0];
		m_pCb_Earth_Box->GetEditBox()->SetText(data.strShowName.c_str());
		ReBuidMarksTree( data.dwMapID );

		dwSelectMapID = data.dwMapID;
	}

	return bRet;
}

BOOL EarthTempleFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pWnd->SetInvisible( TRUE );

	m_pFrameMgr->UnRegisterEventHandler(_T("tagSpecItemEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&EarthTempleFrame::OnOpenEarthTemple));
	m_pCmdMgr->UnRegister("NS_EarthTemple",			(NETMSGPROC)m_Trunk.sfp2(&EarthTempleFrame::OnNS_EarthTemple));

	m_pCmdMgr->UnRegister("NS_Skill",					(NETMSGPROC)m_Trunk.sfp2(&EarthTempleFrame::OnNS_Skill));

	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	return bRet;
}

BOOL EarthTempleFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\ground.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "groundwin\\gr_pic\\directionpic\\direction", "ClassName", "AppliedTree");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	if( !P_VALID( m_pWnd )  )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pBn_Ok_Set	=	(GUIButton*)m_pWnd->GetChild( _T("gr_pic\\sendbutt") );
	m_pBn_Cancel_Set=	(GUIButton*)m_pWnd->GetChild( _T("gr_pic\\offbutt") );
	m_pBn_Close_Set	=	(GUIButton*)m_pWnd->GetChild( _T("gr_pic\\closebutt") );
	m_pCb_Earth_Box	=	(GUIComboBox*)m_pWnd->GetChild( _T("gr_pic\\ground") );
	m_pTree			=	(AppliedTree*)m_pWnd->GetChild( _T("gr_pic\\directionpic\\direction") );
	m_pBnFrist		=	(GUIPushButton*)m_pWnd->GetChild( _T("gr_pic\\targetbox\\target0") );
	m_pBnSecond		=	(GUIPushButton*)m_pWnd->GetChild( _T("gr_pic\\targetbox\\target1") );

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&EarthTempleFrame::OnWnd_Temple));
	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

BOOL EarthTempleFrame::EscCancel()
{
	if (m_pWnd->IsInvisible())
		return FALSE;
	m_pWnd->SetInvisible( TRUE );
	return TRUE;
}

BOOL EarthTempleFrame::OnWnd_Temple(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_Ok_Set )
		{
			if (!CanGoto())
				break;

			if (m_PosVector3 == Vector3(0,0,0))
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EarthTempleCanNotGoto2")]);
				break;
			}

			//--播放吟唱动作
			LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
			if (!P_VALID(pLp))
				break;

			if (!pLp->IsIdleState())
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EarthTempleCanNotUse")]);
				break;
			}

			tstring ss =_T("ze11");
			pLp->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,ss.c_str(),true,true);

			//--发送游戏事件，显示吟唱进度条
			tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
			event.bShow=true;
			event.bSkill=false;
			event.dwTypeID=3070051;//土地庙ID
			event.dwTime=6000;
			TObjRef<GameFrameMgr>()->SendEvent(&event);

			m_bIsSpelling = true;

			m_pWnd->SetInvisible( TRUE );
		}
		if( pWnd == m_pBn_Cancel_Set )
		{
			m_pWnd->SetInvisible( TRUE );
		}
		if( pWnd == m_pBn_Close_Set )
		{
			m_pWnd->SetInvisible( TRUE );
		}

		break;
	case EGUIE_PushButtonDown:
		if (pWnd == m_pBnFrist)
		{
			if( m_pBnFrist->GetState() == EGUIBS_PushDown )
			{
				if (0 == m_strFristUnit.dwMapID)
				{
					m_PosVector3 = Vector3(0,0,0);
					break;
				}
				dwSelectMapID	= m_strFristUnit.dwMapID;
				m_PosVector3	= m_strFristUnit.position;
				m_szTempName	= m_strFristUnit.name;

				m_pCb_Earth_Box->GetEditBox()->SetText(MapLogicData::Inst()->FindMapShowName(dwSelectMapID));
				ReBuidMarksTree(dwSelectMapID);

			}
		}
		if (pWnd == m_pBnSecond)
		{
			if( m_pBnSecond->GetState() == EGUIBS_PushDown )
			{
				if (0 == m_strSecondUnit.dwMapID)
				{
					m_PosVector3 = Vector3(0,0,0);
					break;
				}
				dwSelectMapID	= m_strSecondUnit.dwMapID;
				m_PosVector3	= m_strSecondUnit.position;
				m_szTempName	= m_strSecondUnit.name;

				m_pCb_Earth_Box->GetEditBox()->SetText(MapLogicData::Inst()->FindMapShowName(dwSelectMapID));
				ReBuidMarksTree(dwSelectMapID);
			}
		}
		break;
	case EGUIE_ItemClick:
		if ( pWnd == m_pTree )
		{
			map<DWORD, tagMarkIndex>::iterator itr = m_mapMarkIndex.find( pEvent->dwParam1 );
			if( itr != m_mapMarkIndex.end() )
			{
				const tagZoneMapData* pData = ZoneMapData::Inst()->FindZoneMapData( dwSelectMapID );
				if( P_VALID( pData ) )
				{
					ASSERT(itr->second.nType<(int)pData->vecMarkTypes.size());
					const tagMapMarkType& markType = pData->vecMarkTypes[itr->second.nType];
					ASSERT(itr->second.nMask<(int)markType.vecMarks.size());
					const tagMapMarkData& markData = markType.vecMarks[itr->second.nMask];
					m_PosVector3 = markData.vPos;

					m_strTempMark.nType = itr->second.nType;
					m_strTempMark.nMask = itr->second.nMask;

					m_szTempName = m_pTree->GetItem(pEvent->dwParam1)->strShow;

					m_pBnFrist->SetState(EGUIBS_PopUp);
					m_pBnSecond->SetState(EGUIBS_PopUp);
				}
			}
			else
			{
				m_PosVector3 = Vector3(0,0,0);
			}
		}
		if ( pWnd == m_pCb_Earth_Box->GetListBox() )
		{
			if(pEvent->dwParam1 > m_vecEarthMapShow.size())
				break;

			tagZoneMapShow &data = m_vecEarthMapShow[pEvent->dwParam1];
		
			m_pCb_Earth_Box->GetEditBox()->SetText(data.strShowName.c_str());
			dwSelectMapID = data.dwMapID;
			ReBuidMarksTree(data.dwMapID);

			m_PosVector3 = Vector3(0,0,0);

			m_pBnFrist->SetState(EGUIBS_PopUp);
			m_pBnSecond->SetState(EGUIBS_PopUp);
		}
		break;
	default:
		break;
	}


	return TRUE;
}

VOID EarthTempleFrame::InitMapInfo()
{
	m_vecEarthMapShow.clear();
	const map<DWORD, tagMapLogicData>& mapMapLogicData = MapLogicData::Inst()->GetDataMap();
	for( map<DWORD, tagMapLogicData>::const_iterator citr = mapMapLogicData.begin(); citr != mapMapLogicData.end(); citr++ )
	{
		// 只显示普通地图
		if( EMT_Normal != citr->second.eMapType )
			continue;

		// 屏蔽训练场
		if( ENMT_TrainingField == citr->second.eNormalType)
			continue;

		// 游戏中的地图都是以m开头的（去掉测试场景）
		if( citr->second.szName.empty() || _T('s') != citr->second.szName.c_str()[0] )
			continue;

		tagZoneMapShow zm;
		zm.dwMapID = citr->first;
		zm.strShowName = citr->second.szShowName;
		m_vecEarthMapShow.push_back( zm );
	}
}

VOID EarthTempleFrame::FillMapListBox()
{
	m_pCb_Earth_Box->GetListBox()->Clear();

	for( size_t i = 0; i < m_vecEarthMapShow.size(); i++ )
	{
		tagZoneMapShow &data = m_vecEarthMapShow[i];
		m_pCb_Earth_Box->GetListBox()->SetText( i, 0, data.strShowName.c_str() );
	}

	tagPoint ptSizeListBox = m_pCb_Earth_Box->GetListBox()->GetSize();
	ptSizeListBox.y = m_pCb_Earth_Box->GetListBox()->GetRowHeight() * m_vecEarthMapShow.size();
	ptSizeListBox.x = m_pCb_Earth_Box->GetEditBox()->GetSize().x;

	m_pCb_Earth_Box->GetListBox()->SetResizable( TRUE );
	m_pCb_Earth_Box->GetListBox()->Resize( ptSizeListBox );
	m_pCb_Earth_Box->GetListBox()->SetResizable( FALSE );
	m_pCb_Earth_Box->GetListBox()->SetRefresh(TRUE);

}

VOID EarthTempleFrame::ReBuidMarksTree( const DWORD dwMapID )
{
	if( !P_VALID(m_pTree ) )
		return;

	m_pTree->RemoveItem( m_pUtil->Crc32( _T("\\Root") ) );
	m_mapMarkIndex.clear();

	const DWORD dwRootItemID = m_pTree->InsterItem( NULL, _T("Root"), _T("rt") );
	ASSERT( P_VALID(dwRootItemID) );

	const tagZoneMapData* pData = ZoneMapData::Inst()->FindZoneMapData( dwMapID );
	if( !P_VALID( pData ) )
		return;

	// 填充树形控件
	const vector<tagMapMarkType>& vecTypes = pData->vecMarkTypes;
	for( size_t nType = 0; nType < vecTypes.size(); nType++ )
	{
		// 第一层：标记类型
		const tagMapMarkType& markType = vecTypes[nType];
		tstringstream streamType;
		streamType<<_T("Type_")<<nType;
		const DWORD dwTypeID = m_pTree->InsterItem( _T("\\Root"), streamType.str().c_str(), markType.strType.c_str() );
		ASSERT( GT_INVALID != dwTypeID );

		// 第二层：标记
		for( size_t nMark = 0; nMark < markType.vecMarks.size(); nMark++ )
		{
			const tagMapMarkData& markData = markType.vecMarks[nMark];
			tstringstream streamFather;
			streamFather<<_T("\\Root")<<_T("\\")<<streamType.str().c_str();
			tstringstream streamMark;
			streamMark<<_T("Mark_")<<nMark;
			const DWORD dwMarskID = m_pTree->InsterItem( streamFather.str().c_str(), streamMark.str().c_str(), markData.strName.c_str() );
			ASSERT( GT_INVALID != dwMarskID );
			m_mapMarkIndex[dwMarskID] = tagMarkIndex( nType, nMark );
		}
	}

	// 不展开标记
	tagGUITreeItem* pRootItem = m_pTree->GetItem( dwRootItemID );
	ASSERT( P_VALID( pRootItem ) );
	for( std::list<tagGUITreeItem*>::iterator itr = pRootItem->listChildren.begin(); itr != pRootItem->listChildren.end(); itr++ )
	{
		/*if( itr == pRootItem->listChildren.begin() )
			continue;*/

		tagGUITreeItem* pItem = (*itr);
		pItem->bExpended = false;
	}
	m_pTree->RefreshScrollBar();
}

DWORD EarthTempleFrame::OnOpenEarthTemple(tagSpecItemEvent* pEvent)
{
	if (IsInSendingState())
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EarthTempleUsing")]);
		return 0;
	}
	if (EISF_EarthTemple == pEvent->eSpecFuncSrc )
	{
		m_pWnd->SetInvisible( FALSE );

		CurrItemID = pEvent->n64Serial;

		m_bDirectOpen = FALSE;
	}
	return 0;
}

bool EarthTempleFrame::CanGoto()
{
	if ( 3017298127 == dwSelectMapID )//龙城
	{
		if ( 4 == m_strTempMark.nType && 7 == m_strTempMark.nMask )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EarthTempleCanNotGoto")]);
			return false;
		}
		if ( 4 == m_strTempMark.nType && 8 == m_strTempMark.nMask )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EarthTempleCanNotGoto")]);
			return false;
		}
	}

	if ( 0 == CurrItemID && !m_bDirectOpen)
	{
		return false;
	}
	return true;
}
VOID EarthTempleFrame::Update()
{
	if (m_bIsSpelling)
	{
		if (!PlayerStateCheck())//是否有打断
		{
			StopSendMsg();
			m_dwTimeCounter = 0;
		}
		if (m_dwTimeCounter > 6000)//是否可以发送消息
		{
			StopSendMsg();
			SendMsg();
			m_dwTimeCounter = 0;
		}

		m_dwTimeCounter += Kernel::Inst()->GetDeltaTimeDW();
	}
	
}

VOID EarthTempleFrame::SendMsg()
{
	m_bIsSpelling = false;

	tagNC_EarthTemple cmd;
	cmd.n64ItemID = CurrItemID;
	cmd.dwMapID	= dwSelectMapID;
	cmd.destPos = m_PosVector3;
	cmd.bDirectOpen = m_bDirectOpen;
	m_pSession->Send(&cmd);

	ReBuidMarksTree( dwSelectMapID );

	SortUnits();

	m_PosVector3 = Vector3(0,0,0);
	m_strTempMark.nType = 0;
	m_strTempMark.nMask = 0;
	CurrItemID = 0;
	
	m_pBnFrist->SetState(EGUIBS_PopUp);
	m_pBnSecond->SetState(EGUIBS_PopUp);
}

VOID EarthTempleFrame::StopSendMsg()
{
	m_bIsSpelling = false;

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return;
	//--停止吟唱动作
	pLp->GetAniPlayer()->Sync2Lower();

	//--发送游戏事件，隐藏吟唱进度条
	tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
	event.bShow=false;
	event.dwTypeID=1360117;
	event.dwTime=0;
	TObjRef<GameFrameMgr>()->SendEvent(&event);
}

bool EarthTempleFrame::PlayerStateCheck()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(pLp))
		return false;

	if (!pLp->IsIdleState())
		return false;
	
	return true;
}

BOOL EarthTempleFrame::IsInSendingState()
{
	return m_bIsSpelling;
}

VOID EarthTempleFrame::SortUnits()
{
	if (	m_strFristUnit.dwMapID == dwSelectMapID
		&&	m_strFristUnit.position== m_PosVector3)
	{
		return;
	}
	m_strSecondUnit.dwMapID = m_strFristUnit.dwMapID;
	m_strSecondUnit.name	= m_strFristUnit.name;
	m_strSecondUnit.position= m_strFristUnit.position;
	m_pBnSecond->SetText(m_strSecondUnit.name.c_str());

	m_strFristUnit.dwMapID = dwSelectMapID;
	m_strFristUnit.position= m_PosVector3;
	m_strFristUnit.name	   = m_szTempName;
	m_pBnFrist->SetText(m_strFristUnit.name.c_str());
}

DWORD EarthTempleFrame::OnNS_EarthTemple(tagNS_EarthTemple* pMsg,DWORD pParam)
{
	if (pMsg->dwErrorCode != E_Success)
	{
		switch (pMsg->dwErrorCode)
		{
		default:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EarthTempleBunengyong")]);
			break;
		}
	}
	return 0;
}

DWORD EarthTempleFrame::OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam)
{
	if(pMsg->dwErrorCode!=E_Success)
		return 0; 

	const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pMsg->dwSkillID/100);
	if(P_VALID(pData))
	{
		if(ESSF_FreeOpenFace == pData->pProto->eSpecFunc && ESSOF_FLY == pData->pProto->nSpecFuncVal1)
		{
			m_bDirectOpen = TRUE;
			m_pWnd->SetInvisible( FALSE );
		}
	}

	return 0;
}