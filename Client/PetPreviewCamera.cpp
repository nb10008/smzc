#include "StdAfx.h"
#include "PetPreviewCamera.h"
#include "PetProtoData.h"
#include "..\WorldDefine\msg_pet.h"
#include "CreatureData.h"

//------------仅供测试用------------
#include "QuestEvent.h"
//----------------------------------


// <! class 1:   PetCameraWindow------------------------------
PetCameraWindow::PetCameraWindow(void)
:m_fCamX(0)
,m_fCamY(0)
,m_fCamZ(-1)
{
	PetShowWindow::PetShowWindow();
}

VOID PetCameraWindow::SetCamera(ESetAxis ESAxis,float fValue)
{
	switch (ESAxis)
	{
	case E_Axis_X:
		m_fCamX = fValue;
		break;
	case E_Axis_Y:
		m_fCamY = fValue;
		break;
	case E_Axis_Z:
		m_fCamZ = fValue;
		break;
	default:
		break;
	}
}

void PetCameraWindow::UpdateCamera()
{
	if( !P_VALID(m_pPetNode ) )
		return;

	//重新设置摄像机
	Vector3 vec3LookFrom( 0, 0, -1 );
	Vector3 vec3LookAt( 0, 0, 0 );

	vec3LookFrom.x = m_fCamX;
	vec3LookFrom.y = m_fCamY;
	vec3LookFrom.z = m_fCamZ;

	vec3LookAt.x = m_fCamX;
	vec3LookAt.y = m_fCamY;
	vec3LookAt.z = 0;

	m_Camera.SetLookAt( vec3LookFrom, vec3LookAt, MathConst::AxisY );
}

//  <! class 1:   PetPreviewCamera---------------------------------------------
PetPreviewCamera::PetPreviewCamera(void)
:m_Trunk(this)
,m_dwPetID(GT_INVALID)
{
	m_pPteWindow = NULL;
	m_pEBCameraX = NULL;
	m_pEBCameraY = NULL;
	m_pEBCameraZ = NULL;
}

VOID PetPreviewCamera::Update()
{
	GameFrame::Update();
}

BOOL PetPreviewCamera::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL PetPreviewCamera::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL ret    =   GameFrame::Init(pMgr, pFather, dwParam);

	// 注册游戏事件
	m_pFrameMgr->RegisterEventHandle(_T("PetCameraUI"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PetPreviewCamera::RevPetCameraUI));
	
	return ret;
}

BOOL PetPreviewCamera::Destroy()
{
	BOOL ret    =   GameFrame::Destroy();

	// 注销游戏事件
	m_pFrameMgr->UnRegisterEventHandler(_T("PetCameraUI"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PetPreviewCamera::RevPetCameraUI));

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	return ret;
}

BOOL PetPreviewCamera::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\common.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "comm_win\\comm_pic\\petshow_back", "ClassName", "PetCameraWindow");
	// 创建UI
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pPteWindow	= (PetCameraWindow*)m_pWnd->GetChild(_T("comm_pic\\petshow_back"));
	m_pEBCameraX	= (GUIEditBox*)m_pWnd->GetChild(_T("comm_pic\\putinpic1\\putin"));
	m_pEBCameraY	= (GUIEditBox*)m_pWnd->GetChild(_T("comm_pic\\putinpic2\\putin"));
	m_pEBCameraZ	= (GUIEditBox*)m_pWnd->GetChild(_T("comm_pic\\putinpic3\\putin"));
	m_pBtnExit		= (GUIButton*)m_pWnd->GetChild(_T("comm_pic\\butt"));

	m_pWnd->SetInvisible(TRUE);

	// 注册UI事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&PetPreviewCamera::EventHandler));

	return TRUE;
}

VOID PetPreviewCamera::Render3D()
{
	if (P_VALID(m_pPteWindow))
	{
		m_pPteWindow->Render3D();
	}
	GameFrame::Render3D();
}

DWORD PetPreviewCamera::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnExit )
			{
				if (P_VALID(m_pWnd))
					m_pWnd->SetInvisible( TRUE );
			}
		}
		break;
	case EGUIE_EditBoxEnter:
		{
			if( pWnd == m_pEBCameraX )
			{
				 SetAxis(E_Axis_X);
			}
			if( pWnd == m_pEBCameraY )
			{
				 SetAxis(E_Axis_Y);
			}
			if( pWnd == m_pEBCameraZ )
			{
				 SetAxis(E_Axis_Z);
			}
		}
		break;
	}

	return 0;
}

DWORD PetPreviewCamera::RevPetCameraUI( tagPetCameraUI* pGameEvent)
{
	if (m_pWnd->IsInvisible())
		return 0;

	m_dwPetID = pGameEvent->dwPetID;
	m_pPteWindow->SetPetShow(m_dwPetID);
	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD PetPreviewCamera::SetAxis(ESetAxis eAxis)
{
	float fAxisValue = 0;
	GUIEditBox*	 pTempEB = NULL;
	switch (eAxis)
	{
	case E_Axis_X:
		pTempEB = m_pEBCameraX; 
		break;
	case E_Axis_Y:
		pTempEB = m_pEBCameraY;
		break;
	case E_Axis_Z:
		pTempEB = m_pEBCameraZ;
		break;
	default:
		break;
	}
	if (P_VALID(pTempEB))
	{
		tstring str = pTempEB->GetText();
		if (!str.empty())
		{
			tstringstream tss;
			tss<<str;
			tss>>fAxisValue;
		}
		if (m_dwPetID!=GT_INVALID)
		{
			m_pPteWindow->SetCamera( eAxis, fAxisValue );
			m_pPteWindow->SetPetShow(m_dwPetID);
		}
	}
	return 0;

}

//  <! class 1:   PetPreviewMgr-----------------------------------------
PetPreviewMgr::PetPreviewMgr(void)
:m_Trunk(this)
,m_pPetCamera(NULL)
{
}

PetPreviewMgr* PetPreviewMgr::Inst()
{
	static PetPreviewMgr inst;
	return &inst;
}

void PetPreviewMgr::Init()
{
	//网络事件
	m_pCmdMgr->Register("NS_PetCamera", (NETMSGPROC)m_Trunk.sfp2(&PetPreviewMgr::OnOpenFrame),  _T("NS_PetCamera"));
	// 测试游戏事件
	m_pGameFrameMgr->RegisterEventHandle(_T("tagStartNPCTalkEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PetPreviewMgr::Test_OnOpenFrame));

}

void PetPreviewMgr::Destroy()
{
	//网络事件
	m_pCmdMgr->UnRegister("NS_PetCamera", (NETMSGPROC)m_Trunk.sfp2(&PetPreviewMgr::OnOpenFrame));
	// 测试游戏事件
	m_pGameFrameMgr->UnRegisterEventHandler(_T("tagStartNPCTalkEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PetPreviewMgr::Test_OnOpenFrame));
	m_pPetCamera = NULL;
}

DWORD PetPreviewMgr::OnOpenFrame(tagNS_PetCamera* pEvt, DWORD)
{
	m_pPetCamera = (PetPreviewCamera*) m_pGameFrameMgr->GetFrame(_T("PetPreviewCamera"));
	if (!P_VALID(m_pPetCamera))
		m_pPetCamera = (PetPreviewCamera*) m_pGameFrameMgr->CreateFrame(_T("World"), _T("PetPreviewCamera"), _T("PetPreviewCamera"),0);

	if (P_VALID(m_pPetCamera))
		m_pPetCamera->Show(TRUE);

	return 0;
}

//------------仅供测试用------------
DWORD PetPreviewMgr::Test_OnOpenFrame(tagStartNPCTalkEvent* pEvt)
{
	if (0)
	{
		const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pEvt->typeID);
		if( EFNPCT_Null == pCreatureProto->eFunctionType )
		{
			m_pPetCamera = (PetPreviewCamera*) m_pGameFrameMgr->GetFrame(_T("PetPreviewCamera"));
			if (!P_VALID(m_pPetCamera))
				m_pPetCamera = (PetPreviewCamera*) m_pGameFrameMgr->CreateFrame(_T("World"), _T("PetPreviewCamera"), _T("PetPreviewCamera"),0);

			if (P_VALID(m_pPetCamera))
				m_pPetCamera->Show(TRUE);
		}
	}
	return 0;
}
//----------------------------------


