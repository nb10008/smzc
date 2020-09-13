#include "stdafx.h"
#include "DresserFrame.h"
#include "RoleMgr.h"
#include "AvatarCfg.h"
#include "LocalPlayer.h"
#include "PlayerTryDressWnd.h"
#include "Container.h"
#include "ItemMgr.h"
#include "..\WorldDefine\msg_appearance_transform.h"

DresserFrame::DresserFrame( void )
: m_Trunk(this)
, m_pWnd(NULL)
, m_pCaption(NULL)
, m_dwNpcID(GT_INVALID)
,m_escapeTime(0)
,m_pLBtnHType(NULL)
,m_pRBtnHType(NULL)
,m_pStcHType(NULL)
,m_pLBtnHColor(NULL)
,m_pRBtnHColor(NULL)
,m_pStcHColor(NULL)
,m_pLBtnFace(NULL)
,m_pRBtnFace(NULL)
,m_pStcFace(NULL)
,m_pLBtnFDetail(NULL)
,m_pRBtnFDetail(NULL)
,m_pStcFDetail(NULL)
,m_pBtnOK(NULL)
,m_pBtnQuit(NULL)
,m_pBtnExit(NULL)
{

}

DresserFrame::~DresserFrame( void )
{

}

BOOL DresserFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	return TRUE;
}

BOOL DresserFrame::Destroy()
{
	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	return GameFrame::Destroy();;
}

VOID DresserFrame::Render3D()
{
	if(P_VALID(m_pPlayerView))
		m_pPlayerView->Render3D();
	GameFrame::Render3D();
}

BOOL DresserFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL DresserFrame::ReloadUI()
{
	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\dresser.xml");
	XmlElement element;
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "dresser_win\\dresser_pic\\back1", "ClassName", "PlayerTryDressWnd");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pCaption = (GUIPatch*)m_pWnd->GetChild(_T("dresser_pic"));
	m_pPlayerView = (PlayerTryDressWnd*)m_pCaption->GetChild(_T("back1"));

	m_pLBtnHType = (GUIButton*)m_pCaption->GetChild(_T("back2\\back0\\backbutt"));
	m_pRBtnHType = (GUIButton*)m_pCaption->GetChild(_T("back2\\back0\\nextbutt"));
	m_pStcHType = (GUIStatic*)m_pCaption->GetChild(_T("back2\\back0\\num"));

	m_pLBtnHColor = (GUIButton*)m_pCaption->GetChild(_T("back2\\back1\\backbutt"));
	m_pRBtnHColor = (GUIButton*)m_pCaption->GetChild(_T("back2\\back1\\nextbutt"));
	m_pStcHColor = (GUIStatic*)m_pCaption->GetChild(_T("back2\\back1\\num"));

	m_pLBtnFace = (GUIButton*)m_pCaption->GetChild(_T("back2\\back2\\backbutt"));
	m_pRBtnFace = (GUIButton*)m_pCaption->GetChild(_T("back2\\back2\\nextbutt"));
	m_pStcFace = (GUIStatic*)m_pCaption->GetChild(_T("back2\\back2\\num"));

	m_pLBtnFDetail = (GUIButton*)m_pCaption->GetChild(_T("back2\\back3\\backbutt"));
	m_pRBtnFDetail = (GUIButton*)m_pCaption->GetChild(_T("back2\\back3\\nextbutt"));
	m_pStcFDetail = (GUIStatic*)m_pCaption->GetChild(_T("back2\\back3\\num"));

	memset(&m_avatarAtt, 0x0, sizeof(m_avatarAtt));
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if( P_VALID(pLP) && P_VALID(m_pPlayerView) )
	{
		m_avatarAtt = pLP->GetAvatarAtt();
		TCHAR szTemp[X_SHORT_NAME] = {0};
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wHairMdlID+1);
		m_pStcHType->SetText(szTemp);
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wHairTexID+1);
		m_pStcHColor->SetText(szTemp);
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wFaceMdlID+1);
		m_pStcFace->SetText(szTemp);
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wFaceDetailTexID+1);
		m_pStcFDetail->SetText(szTemp);
		m_pPlayerView->UpdataAvatar(pLP);
	}

	m_pBtnOK = (GUIButton*)m_pCaption->GetChild(_T("surebutt"));
	m_pBtnQuit = (GUIButton*)m_pCaption->GetChild(_T("canclebutt"));
	m_pBtnExit = (GUIButton*)m_pCaption->GetChild(_T("closebutt"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&DresserFrame::EventHandler));
	return TRUE;
}

DWORD DresserFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnExit || pWnd == m_pBtnQuit )
				m_pFrameMgr->AddToDestroyList(this);
			else if( pWnd == m_pBtnOK )
			{
				ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
				INT nNum = pContainer->GetItemQuantity(APPEARANCE_TRANSFORM_ITEM);
				if( nNum <= 0 )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoChangAvatarItem")]);
					break;
				}
				tagNC_AppearanceTransform m;
				m.dwNPCID = m_dwNpcID;
				m.wHairTexID = m_avatarAtt.wHairTexID;
				m.wHairMdlID = m_avatarAtt.wHairMdlID;
				m.wFaceMdlID = m_avatarAtt.wFaceMdlID;
				m.wFaceDetailTexID = m_avatarAtt.wFaceDetailTexID;
				m_pSession->Send(&m);
				m_pFrameMgr->AddToDestroyList(this);
			}
			else if(pWnd == m_pLBtnHType)					//发型左按钮
			{
				int id = m_avatarAtt.wHairMdlID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumHairTypeEx()-1;
				else if(id>=AvatarCfg::Inst()->GetNumHairTypeEx())
					id = 0;
				m_avatarAtt.wHairMdlID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wHairMdlID+1);
				if(P_VALID(m_pStcHType))
					m_pStcHType->SetText(szTemp);
				m_pStcHType->SetRefresh(true);
			}
			else if(pWnd == m_pRBtnHType)					//发型右按钮
			{
				int id = m_avatarAtt.wHairMdlID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumHairTypeEx())
					id = 0;
				m_avatarAtt.wHairMdlID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wHairMdlID+1);
				if(P_VALID(m_pStcHType))
					m_pStcHType->SetText(szTemp);
				m_pStcHType->SetRefresh(true);
			}
			else if(pWnd == m_pLBtnHColor)					//发色左按钮
			{
				int id = m_avatarAtt.wHairTexID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumHairColor()-1;
				else if(id>=AvatarCfg::Inst()->GetNumHairColor())
					id = 0;
				m_avatarAtt.wHairTexID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wHairTexID+1);
				if(P_VALID(m_pStcHColor))
					m_pStcHColor->SetText(szTemp);
				m_pStcHColor->SetRefresh(true);
			}
			else if(pWnd == m_pRBtnHColor)					//发色右按钮
			{
				int id = m_avatarAtt.wHairTexID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumHairColor())
					id = 0;
				m_avatarAtt.wHairTexID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wHairTexID+1);
				if(P_VALID(m_pStcHColor))
					m_pStcHColor->SetText(szTemp);
				m_pStcHColor->SetRefresh(true);
			}
			else if(pWnd == m_pLBtnFace)					//面部左按钮
			{
				int id = m_avatarAtt.wFaceMdlID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumFaceEx()-1;
				else if(id>=AvatarCfg::Inst()->GetNumFaceEx())
					id = 0;
				m_avatarAtt.wFaceMdlID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wFaceMdlID+1);
				if(P_VALID(m_pStcFace))
					m_pStcFace->SetText(szTemp);
				m_pStcFace->SetRefresh(true);
			}
			else if(pWnd == m_pRBtnFace)					//面部右按钮
			{
				int id = m_avatarAtt.wFaceMdlID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumFaceEx())
					id = 0;
				m_avatarAtt.wFaceMdlID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wFaceMdlID+1);
				if(P_VALID(m_pStcFace))
					m_pStcFace->SetText(szTemp);
				m_pStcFace->SetRefresh(true);
			}
			else if(pWnd == m_pLBtnFDetail)					//面部细节左按钮
			{
				int id = m_avatarAtt.wFaceDetailTexID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumFaceDetail()-1;
				else if(id>=AvatarCfg::Inst()->GetNumFaceDetail())
					id = 0;
				m_avatarAtt.wFaceDetailTexID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wFaceDetailTexID+1);
				if(P_VALID(m_pStcFDetail))
					m_pStcFDetail->SetText(szTemp);
				m_pStcFDetail->SetRefresh(true);
			}
			else if(pWnd == m_pRBtnFDetail)					//面部细节右按钮
			{
				int id = m_avatarAtt.wFaceDetailTexID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumFaceDetail())
					id = 0;
				m_avatarAtt.wFaceDetailTexID = id;
				m_pPlayerView->UpdataAvatarAtt(m_avatarAtt);

				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_avatarAtt.wFaceDetailTexID+1);
				if(P_VALID(m_pStcFDetail))
					m_pStcFDetail->SetText(szTemp);
				m_pStcFDetail->SetRefresh(true);
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

VOID DresserFrame::Update()
{
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
			return;
		}
		if (RoleMgr::Inst()->IsOutValidDist( m_dwNpcID ) )
		{
			m_pMgr->AddToDestroyList(this);
		}
	}
}
