#include "StdAfx.h"
#include "HolyManFrame.h"
#include "WorldFrame.h"
#include "ItemButton.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "HolyManMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "ItemMgr.h"
#include "Item.h"
#include "PocketFrame.h"
#include "ItemFrame.h"
#include "Container.h"
#include "DragBtn.h"
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\msg_yuanshen.h"
#include "ItemEvent.h"
#include "LevelUpData.h"
#include "CombatEvent.h"
#include "VirtualRole.h"
#include "GUIEffectMgr.h"
#include "SkillEvent.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "HolyEquipFrame.h"
//#include "EquipMaintainValueFrame.h"
#include "..\WorldDefine\msg_compose.h"

const Vector3 g_vHolyFrom( -1.73f, 135.0f, -162.82f );
const Vector3 g_vHolyLookAt( -4.09f, 135.0f, -62.85f );
const DWORD HolyMan_WakeUp		= 1100;  //元神觉醒快捷键

const TCHAR *szPic_HolyMan0		= _T("data\\ui\\soul\\ys_01.dds");
const TCHAR *szPic_HolyMan1		= _T("data\\ui\\soul\\ys_02.dds");
const TCHAR *szPic_HolyMan2		= _T("data\\ui\\soul\\ys_03.dds");
const TCHAR *szPic_HolyMan3		= _T("data\\ui\\soul\\ys_04.dds");
const TCHAR *szPic_HolyMan4		= _T("data\\ui\\soul\\ys_05.dds");
const TCHAR *szPic_HolyMan5		= _T("data\\ui\\soul\\ys_06.dds");
const TCHAR *szPic_HolyMan6		= _T("data\\ui\\soul\\ys_07.dds");
const TCHAR *szPic_HolyMan7		= _T("data\\ui\\soul\\ys_08.dds");
const TCHAR *szPic_HolyMan8		= _T("data\\ui\\soul\\ys_09.dds");
const TCHAR *szPic_HolyMan9		= _T("data\\ui\\soul\\ys_10.dds");

const FLOAT		g_fRebornTime	= 4.0f;

//---start test def ---
const DWORD HolyMan_KEY_UP_At			= 1104;
const DWORD HolyMan_KEY_BACK_At		= 1105;
const DWORD HolyMan_KEY_LEFT_At			= 1106;
const DWORD HolyMan_KEY_RIGHT_At		= 1107;
const DWORD HolyMan_KEY_High_A100	= 1108;
const DWORD HolyMan_KEY_High_M100	= 1109;
const DWORD HolyMan_KEY_High_A1000	= 1110;
const DWORD HolyMan_KEY_High_M1000	= 1111;
const DWORD HolyMan_KEY_Enter_O			= 1120;  //用于查看输出
static float	  g_fZValue	= 4.9f;	// z值
//---end test def ---

//元神模型预览屏 -- start of HolyManView
HolyManView::HolyManView(void)
:m_Trunk(this)
,m_fYaw(0)
,m_pNewCanvas(NULL)
,m_pHolyMenNode(NULL)
,m_pSG(NULL)
,m_pBnRotaLeft( NULL )
,m_pBnRotaRight( NULL )
,m_LookAt( g_vHolyLookAt )
,m_LookFrom( g_vHolyFrom )
{

	m_pCanvas = NULL ;
}

HolyManView::~HolyManView(void)
{}

BOOL HolyManView::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
	if( !GUIStatic::Init(pSystem, pFather, pXmlElement) )
		return FALSE;

#ifdef HolyMan_Test_Define
	m_pTestKeyMap->Register(HolyMan_KEY_LEFT_At,			g_StrTable[_T("KEY_LEFT_At")],			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_A);
	m_pTestKeyMap->Register(HolyMan_KEY_RIGHT_At,		g_StrTable[_T("KEY_RIGHT_At")],		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_D);
	m_pTestKeyMap->Register(HolyMan_KEY_UP_At,				g_StrTable[_T("KEY_UP_At")],				(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_W);
	m_pTestKeyMap->Register(HolyMan_KEY_BACK_At,			g_StrTable[_T("KEY_BACK_At")],			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_S);
	m_pTestKeyMap->Register(HolyMan_KEY_High_A100,		g_StrTable[_T("KEY_High_A100")],		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_K);
	m_pTestKeyMap->Register(HolyMan_KEY_High_M100,		g_StrTable[_T("KEY_High_M100")],		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_L);
	m_pTestKeyMap->Register(HolyMan_KEY_High_A1000,	g_StrTable[_T("KEY_High_A1000")],	(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_I);
	m_pTestKeyMap->Register(HolyMan_KEY_High_M1000,	g_StrTable[_T("KEY_High_M1000")],	(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_O);
	m_pTestKeyMap->Register(HolyMan_KEY_Enter_O,			g_StrTable[_T("KEY_Enter_O")],			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManView::TestOnKey),DIK_RETURN);
#endif

	// 因为需要深度缓冲，所以无法使用Render2D创建
	m_pNewCanvas = new tagGUIImage;
	if( !P_VALID(m_pNewCanvas) )
		return FALSE;

	m_pNewCanvas->rc = tagRect( 0, 0, m_ptSize.x, m_ptSize.y );
	m_pNewCanvas->ptSize = m_ptSize;

	// 渲染到纹理
	IRenderTexture* pRenderTexture = Device()->NewRenderTexture();
	bool bResult = pRenderTexture->Create(m_pNewCanvas->ptSize.x, m_pNewCanvas->ptSize.y, EPF_A8R8G8B8, true, false, 1, false);
	if( !bResult )	// 创建RenderTarget失败
	{
		SAFE_DEL(pRenderTexture);
		SAFE_DEL(m_pNewCanvas);
		return FALSE;
	}
	m_pNewCanvas->dwHandle = (DWORD)pRenderTexture;
	m_pNewCanvas->bRenderTarget = TRUE;

	m_pNewCanvas->ptOffset.x = m_ptView.x;
	m_pNewCanvas->ptOffset.y = m_ptView.y;
	m_pNewCanvas->bRefresh = TRUE;

	// 设置初始化摄像机位置

	m_Camera.SetLookAt( m_LookAt, m_LookFrom, MathConst::AxisY );
	// 设置透视投影
	m_Camera.SetPerspective( (float)m_ptSize.x, (float)m_ptSize.y, _DegToRad(45.0f), 1.0f, 20000.0f);
	// 建立空场景图
	m_pSG = new NullSceneGraph;
	// 绑定摄像机
	m_pSG->BindCamera( &m_Camera );

	return TRUE;
}

VOID HolyManView::Destroy()
{
	GUIStatic::Destroy();

	m_pBnRotaLeft = NULL;
	m_pBnRotaRight = NULL;

#ifdef HolyMan_Test_Define
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_LEFT_At")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_RIGHT_At")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_UP_At")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_BACK_At")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_High_A100")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_High_M100")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_High_A1000")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_High_M1000")], FALSE );
	m_pTestKeyMap->SetEnable( g_StrTable[_T("KEY_Enter_O")], FALSE );
#endif

	m_pSG->DetachDynamicNode( m_pHolyMenNode );
	SAFE_DEL( m_pHolyMenNode );
	SAFE_DEL( m_pSG );
	m_pRender->ReleaseRenderTarget(m_pNewCanvas->dwHandle);
	m_pRender->DestroyImage(m_pNewCanvas);
	m_pNewCanvas = 0;
}

VOID HolyManView::Update()
{
	if( !m_bInvisible )
	{
		// 处理旋转按钮 
		float fRotateAngle = 2.0f * Kernel::Inst()->GetDeltaTime();
		if( P_VALID( m_pBnRotaLeft ) && 1 == m_pBnRotaLeft->GetState() )
			RotateYaw( fRotateAngle );
		if( P_VALID( m_pBnRotaRight ) && 1 == m_pBnRotaRight->GetState() )
			RotateYaw( -fRotateAngle );
		 
#ifdef HolyMan_Test_Define
		//--按下鼠标右键并拖动
		if( m_pInput->IsMouseDown(1) && m_pInput->IsMouseMove() )
		{
			float fYaw = m_Camera.GetYaw() - (-m_pInput->GetRelativeX()*1.0f) * 0.6f * 0.006545f;
			m_Camera.SetYaw(fYaw);
			m_Camera.UpdateViewMatrix();
		}
#endif	
	}
	GUIStatic::Update();
}

VOID HolyManView::Render()
{
	if( m_bInvisible )
		return GUIStatic::Render();

	if( !P_VALID( m_pHolyMenNode ) )
		return;

	// 渲染文字及子控件
	m_pRender->PushRenderTarget(m_pNewCanvas);
	m_pNewCanvas->rc = m_rcText + m_ptView;
	m_pRender->Text(m_pNewCanvas, m_strText, m_pFont, m_dwTextColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
	GUIWnd::Render();
	m_pRender->PopRenderTarget(m_pNewCanvas);

	// 渲染到父窗口
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pNewCanvas->rc.SetLTWH(0, 0, m_ptSize.x, m_ptSize.y);
	m_pRender->Draw(m_pCanvas, m_pNewCanvas, 0xFFFFFFFF, m_eAlphaWrite);
}

VOID HolyManView::RefreshViewCoord()
{
	GUIStatic::RefreshViewCoord();
	m_pNewCanvas->ptOffset.x = m_ptView.x;
	m_pNewCanvas->ptOffset.y = m_ptView.y;
}

void HolyManView::RotateYaw( const float fAngle )
{
	if( !P_VALID( m_pHolyMenNode ) )
		return;

	m_fYaw += fAngle;

	Transform tran;
	tran.LoadIdentity();
	tran.Rotate( m_fYaw, 0.0f, 0.0f );
	m_pHolyMenNode->SetWorldMat( tran.GetMatrix() );
}

VOID HolyManView::SetInvisible( BOOL b )
{
	if( m_bInvisible == b )	// 防止重复创建删除
		return;

	GUIStatic::SetInvisible(b);

}

VOID HolyManView::AddChild(GUIWnd* pChild)
{
	GUIStatic::AddChild(pChild);

	pChild->SetCanvas(m_pNewCanvas);
	m_pNewCanvas->bRefresh = TRUE;

	// 保存旋转按钮指针
	if( pChild->GetClassName() == _T("CGUIButton") )
	{	
		if( pChild->GetName() == _T("lastbutt") )
			m_pBnRotaLeft = (GUIButton*)pChild;
		else if( pChild->GetName() == _T("nextbutt") )
			m_pBnRotaRight = (GUIButton*)pChild;
	}
}

void HolyManView::Render3D()
{
	if( m_bInvisible )
		return GUIStatic::Render();
	if( !P_VALID( m_pHolyMenNode ) )
		return;

	UpdateCamera();
	IDevice* pDev = Device();
	IRenderTexture* pRT = (IRenderTexture*)m_pNewCanvas->dwHandle;
	if (P_VALID(pDev)&&P_VALID(pRT))
	{
		float fTimeDelta = Kernel::Inst()->GetDeltaTime();
		m_pNewCanvas->bRefresh = TRUE;
		m_bRefresh = TRUE;

		pRT->Begin();
		pDev->Clear( true, true, false, 0xff000000, 1.0f, 0 );
		StateBlock block;
		block.SaveState(ERS_ColorWriteEnable);
		pDev->SetRenderState(ERS_ColorWriteEnable, D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED);
		// 关闭后期处理
		const bool bEnablePostProcess = Kernel::Inst()->GetRenderSys()->IsEnablePostProcess();
		Kernel::Inst()->GetRenderSys()->EnablePostProcess( false );
		m_pSG->Render( fTimeDelta );
		Kernel::Inst()->GetRenderSys()->EnablePostProcess( bEnablePostProcess );

		block.Apply();
		pRT->End();
	}

}

tstring HolyManView::GetTransformSkinPath(const TCHAR* pModelName, int index )
{
	tstringstream stream;
	stream << _T("data\\animation\\npc\\") << pModelName << _T("\\") << pModelName
		<< _T("_obj") << index << _T(".fskin");
	return stream.str();
}

void HolyManView::SetHolyManShow( )
{
	// 卸载节点
	m_pSG->DetachDynamicNode(m_pHolyMenNode);
	SAFE_DEL(m_pHolyMenNode);
	// 重建节点
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if ( !P_VALID(pLP) )
		return;
	AvatarEquipNode* pSN = (AvatarEquipNode*)pLP->GetSceneNode();
	if ( !P_VALID(pSN) )
		return;
	const tagHolySoulInfo info= HolyManMgr::Inst()->GetHolyManAtt();
	DWORD dwModelID = (info.SoulAttEx).dwSoulID;
	const tagAvatarAtt lpAtt = pSN->GetAtt();
	tstring resPath =  HolyManMgr::Inst()->GetHolyManSkelPath(dwModelID,lpAtt.bySex);
	tstring resName =  HolyManMgr::Inst()->GetHolyManSkelName(dwModelID,lpAtt.bySex);
	if ( resPath.empty() || resName.empty() )
		return;
	m_pHolyMenNode = LoadEntity( resPath.c_str() );	 //测试
	if(!P_VALID(m_pHolyMenNode)) 
		return;
	int i=1;
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	tstring szSkinPath = GetTransformSkinPath( resName.c_str(), i++ );			
	DWORD fileSize = pIFS->GetSize(szSkinPath.c_str());
	while( 0 != fileSize && GT_INVALID != fileSize )
	{
		SGSkinNode *pNewSkin = new SGSkinNode;
		pNewSkin->LoadRes( szSkinPath.c_str() );
		m_pHolyMenNode->AttachChild( pNewSkin );

		szSkinPath = GetTransformSkinPath( resName.c_str(), i++);
		fileSize = pIFS->GetSize(szSkinPath.c_str());
	}	
	m_pHolyMenNode->UpdateBoundingBox();
	// 挂载节点
	m_pSG->AttachDynamicNode(m_pHolyMenNode);
	// 更新摄像机
	UpdateCamera();
}

//-----------------------------------------------------------------------------
//  test--------消息处理
//-----------------------------------------------------------------------------
DWORD HolyManView::TestOnKey( DWORD dwID, BOOL bDown )
{

#ifdef HolyMan_Test_Define
	if (m_bInvisible)
		return 0;
	m_LookAt = m_Camera.GetLookat();
	//look at
	if(dwID==HolyMan_KEY_UP_At)
	{
		Vector3 newPos=m_Camera.GetLookat()+m_Camera.GetForward()*10.0f;
		m_LookAt = newPos;
	}
	else if(dwID==HolyMan_KEY_BACK_At)
	{
		Vector3 newPos=m_Camera.GetLookat()-m_Camera.GetForward()*10.0f;
		m_LookAt = newPos;
	}
	else if(dwID==HolyMan_KEY_LEFT_At)
	{
		Vector3 newPos=m_Camera.GetLookat()+m_Camera.GetSide()*10.0f;
		m_LookAt = newPos;
	}
	else if(dwID==HolyMan_KEY_RIGHT_At)
	{
		Vector3 newPos=m_Camera.GetLookat()-m_Camera.GetSide()*10.0f;
		m_LookAt = newPos;
	}
	//high +- 100 or 1000
	else if (dwID==HolyMan_KEY_High_A100 /*&& bDown*/)
	{
		m_LookFrom.y +=5;
		m_LookAt.y +=5;
	}
	else if (dwID==HolyMan_KEY_High_M100 /*&& bDown*/)
	{
		m_LookFrom.y -=5;
		m_LookAt.y -=5;
	}
	else if (dwID==HolyMan_KEY_High_A1000 /*&& bDown*/)
	{
		m_LookFrom.y +=10;
		m_LookAt.y +=10;
	}
	else if (dwID==HolyMan_KEY_High_M1000 /*&& bDown*/)
	{
		m_LookFrom.y -=10;
		m_LookAt.y -=10;
	}
	else if ( dwID==HolyMan_KEY_Enter_O )
	{
		g_fZValue += 0.5f;
	}
	//进入UpdateViewMatrix查看CameraBase::SetLookAt(m_vFrom,m_vLookat,m_vUp)的值
	m_Camera.SetLookAtPos(m_LookAt);
	m_Camera.UpdateViewMatrix();
#endif

	return TRUE;

}

void HolyManView::UpdateCamera()
{
	if( !P_VALID(m_pHolyMenNode ) || m_bInvisible)
		return;
	//重新设置摄像机
	m_Camera.SetLookAtPos(m_LookAt);
	m_Camera.UpdateViewMatrix();
	//m_Camera.SetLookAt( m_LookFrom, m_LookAt, MathConst::AxisY );
}

EntityNode* HolyManView::LoadEntity( LPCTSTR szFileName )
{
	EntityNode* pEntityNode = new EntityNode;

	pEntityNode->LoadRes( szFileName );

	pEntityNode->Hide( FALSE );

	return pEntityNode;
}

void HolyManView::LoadSkin( LPCTSTR szFileName, EntityNode* pEntityNode )
{
	ASSERT(pEntityNode!=NULL);
	if (pEntityNode != NULL)
	{
		SGSkinNode *pNewSkin = new SGSkinNode;
		pNewSkin->LoadRes( szFileName );
		pEntityNode->AttachChild( pNewSkin );
	}
}

HolyManFrame::HolyManFrame(void)
:m_Trunk(this)
,m_pWnd(NULL)
,m_pWndCaption(NULL)
,m_pBtnClose(NULL)
,m_pPthAtt(NULL)
,m_pPthLv(NULL)
,m_pPBAtt(NULL)
,m_pPBLv(NULL)
,m_pPBEquip(NULL)
,m_pHolyManView(NULL)
,m_pBtnLeft(NULL)
,m_pBtnRight(NULL)
,m_pStcHolyLv_1(NULL)
,m_pStcHolyLv_2(NULL)
,m_pStcHolyLv_3(NULL)
,m_pStcHolyLvData(NULL)
,m_pProHolyExp(NULL)
,m_pProHolyValue(NULL)
,m_pProHolyPrivity(NULL)
,m_pStcDamage(NULL)
,m_pStcHitRate(NULL)
,m_pStcDeath(NULL)
,m_pStcDeathValue(NULL)
,m_pStcOutAttack(NULL)
,m_pStcInAttack(NULL)
,m_pStcExact(NULL)
,m_pStcExtraDamage(NULL)
,m_pStcIgnoreTough(NULL)
,m_pStcMorale(NULL)
,m_pStcHolyLv1_1(NULL)
,m_pStcHolyLv1_2(NULL)
,m_pStcHolyLv1_3(NULL)
,m_pStcHolyLvData1(NULL)
,m_pProHolyExp1(NULL)
,m_pProHolyValue1(NULL)
,m_pProHolyPrivity1(NULL)
,m_pEBAffuseExp(NULL)
,m_pBtnAffuseMax(NULL)
,m_pBtnAffuseOK(NULL)
,m_pBtnAffuseCancel(NULL)
,m_pIBRebornItem(NULL)
,m_pIBRebornStuff(NULL)
,m_pBtnRebornOK(NULL)
,m_pBtnRebCancel(NULL)
,m_pBtnAddStuff(NULL)
,m_pBtnMinuStuff(NULL)
,m_pProReborn(NULL)
,m_PStcRebornRate(NULL)
,m_pStcSucceed(NULL)
,m_pRebornItem(NULL)
,m_pRebornStuff(NULL)
,m_pResHolyMenValue(NULL)
,m_pWndHolyState(NULL)
,m_pStcNormalState(NULL)
,m_pBtnHolyState(NULL)
,m_pHolyEquip(NULL)
,m_dwTimeEclipse(GT_INVALID)
,m_dwCurTime(0)
,m_dwCurTimeState(0)
,m_n64AffuseHolyExp(0)
,m_bIfProgress(FALSE)
,m_nRebornStuffNum(0)
{
	for ( int i=0; i<EHEFI_End; ++i )
	{
		m_pIBHolyEquip[i] = NULL;
	}

	for ( int j=0; j<STATE_VALUE_RATE; ++j )
	{
		m_pStcStateRate[j] = NULL;
	}

}

HolyManFrame::~HolyManFrame(void)
{
}

DWORD HolyManFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd==m_pBtnClose )
			{
				ShowWnd(FALSE); 
			}
			else if ( pWnd== m_pBtnAffuseMax )
			{
				LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
				if (P_VALID(pLP))
				{
					INT64 n64Exp = pLP->GetRoleLevelExp();
					tstringstream tssExp;
					tssExp<<n64Exp;
					m_pEBAffuseExp->SetText( tssExp.str().c_str() );
				}
			} 
			else if ( pWnd== m_pBtnAffuseOK )
			{
				SendHolyAffuseMsg();
			}
			else if ( pWnd== m_pBtnAffuseCancel )
			{
				m_pEBAffuseExp->SetText(_T(""));
			}
			else if ( pWnd== m_pIBRebornItem )
			{
				LockItem(FALSE);
				m_pRebornItem = NULL;
				m_pIBRebornItem->RefreshItem();
			}
			else if ( pWnd== m_pIBRebornStuff )
			{
				LockItem(FALSE);
				m_pRebornStuff = NULL;
				RefreshUI_Reborn();
			}
			else if ( pWnd== m_pBtnRebornOK )
			{
				if ( !m_bIfProgress && P_VALID(m_pRebornItem) && P_VALID(m_pRebornStuff) )
				{
					m_pBtnRebornOK->SetEnable(FALSE);
					m_pProReborn->SetValue(0,TRUE);
					m_pProReborn->SetValue(g_fRebornTime);
					m_bIfProgress = TRUE;
					LockItem(TRUE);
				}
			}
			else if ( pWnd== m_pBtnRebCancel )
			{
				m_bIfProgress = FALSE;
				m_pBtnRebornOK->SetEnable(TRUE);
				m_pProReborn->SetValue(0,TRUE);
				LockItem(FALSE);
				RefreshUI_Reborn();
			}
			else if ( pWnd== m_pBtnAddStuff )
			{
				if ( P_VALID(m_pRebornStuff) )
				{
					DWORD dwTypeID			=	m_pRebornStuff->GetItemTypeID();
					ItemContainer* pPocket	=	ItemMgr::Inst()->GetPocket();
					INT nCount						=	pPocket->GetItemQuantity(dwTypeID);
					if ( nCount > HolyManMgr::Inst()->GetFullRateMinNum() )
						nCount = HolyManMgr::Inst()->GetFullRateMinNum();
					if ( m_nRebornStuffNum < nCount )
					{
						++m_nRebornStuffNum;
						RefreshUI_Reborn();
					}
				}
			}
			else if ( pWnd== m_pBtnMinuStuff )
			{
				if ( m_nRebornStuffNum-1 >0 )
				{
					--m_nRebornStuffNum;
					RefreshUI_Reborn();
				}
			}
			else
			{
				for ( int i=0; i<EHEFI_End; ++i )
				{
					if( pWnd==m_pIBHolyEquip[i]  )
					{
						HolyManMgr::Inst()->SendMsg_UnHolyEquip( EHolyEquipPos(i) );
						break;
					}					
				}
			}

		}
		break;
	case EGUIE_Drag:
		{
			if ( pWnd== m_pIBRebornItem )
			{
				LockItem(FALSE);
				m_pRebornItem = NULL;
				m_pIBRebornItem->RefreshItem();
			}
			else if ( pWnd== m_pIBRebornStuff )
			{
				LockItem(FALSE);
				m_pRebornStuff = NULL;
				RefreshUI_Reborn();
			}
			else
			{
				for ( int i=0; i<EHEFI_End; ++i )
				{
					if( pWnd==m_pIBHolyEquip[i]  )
					{
// 						ItemContainer* pHolyEquip = ItemMgr::Inst()->GetHolyEquip();
// 						if( P_VALID(pHolyEquip) )
// 						{
// 							Item* pItem = pHolyEquip->GetValue( (INT16) i );
// 							DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
// 							if ( P_VALID(pItem) && P_VALID(pDrag) )
// 							{
// 								pEvent->dwParam1 = EDT_HolyEquip;
// 								pEvent->dwParam2 = (EHolyEquipPos) i ;
// 								pEvent->dwParam3 = pItem->GetItemTypeID();
// 								pDrag->OnEvent(pEvent);	 
// 							}
// 						}
						break;
					}					
				}
			}
		}
		break;
	case EGUISCB_Drop:
		{
			if ( pWnd==m_pIBRebornItem || pWnd==m_pIBRebornStuff )
			{
				if(EDT_Item == pEvent->dwParam1)//背包中的物品 
				{
					Item* pItem = ItemMgr::Inst()->GetPocketItem((INT16)LOWORD(pEvent->dwParam2));
					if ( P_VALID(pItem) )
					{
						INT16 n16Pos = pItem->GetPos();
						AddItem( n16Pos);	
					}
				}
			}
			else
			{
				for ( int i=0; i<EHEFI_End; ++i )
				{
					if( pWnd==m_pIBHolyEquip[i]  )
					{
						if(EDT_Item == pEvent->dwParam1)//背包中的物品 
						{
							Item* pItem = ItemMgr::Inst()->GetPocketItem((INT16)LOWORD(pEvent->dwParam2));
							if ( P_VALID(pItem) )
							{
								HolyManMgr::Inst()->SendMsg_HolyEquip( (EHolyEquipPos) i, pItem->GetPos() );
							}
						}
						break;
					}					
				}
			}
			
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pEvent->dwParam1 != 1 )
				return 0;

			if ( pWnd == m_pPBAtt )
			{
				LockItem(FALSE);
				m_bIfProgress = FALSE;
				m_pPthAtt->SetInvisible(FALSE);
				m_pPthLv->SetInvisible(TRUE);
				m_pHolyEquip->ShowWnd(FALSE);
				m_pPBLv->SetState(EGUIBS_PopUp);
				m_pPBEquip->SetState(EGUIBS_PopUp);
				RefreshUI_Att();
			}
			else if ( pWnd == m_pPBLv )
			{
				m_pPthAtt->SetInvisible(TRUE);
				m_pPthLv->SetInvisible(FALSE);
				m_pHolyEquip->ShowWnd(FALSE);
				m_pPBAtt->SetState(EGUIBS_PopUp);
				m_pPBEquip->SetState(EGUIBS_PopUp);

				LockItem(FALSE);
				m_bIfProgress = FALSE;
				m_pRebornItem = NULL;
				m_pRebornStuff = NULL;
				RefreshUI_Lv();
			}
			else if ( pWnd == m_pPBEquip )
			{
				LockItem(FALSE);
				m_bIfProgress = FALSE;
				m_pPthAtt->SetInvisible(TRUE);
				m_pPthLv->SetInvisible(TRUE);
				m_pHolyEquip->ShowWnd(TRUE);
				m_pPBAtt->SetState(EGUIBS_PopUp);
				m_pPBLv->SetState(EGUIBS_PopUp);
			}
			 
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd==m_pIBRebornItem )
			{
				if ( P_VALID(m_pRebornItem) )
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pRebornItem).c_str());
			}
			else 	if( pWnd==m_pIBRebornStuff )
			{
				if ( P_VALID(m_pRebornStuff) )
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pRebornStuff).c_str());
			}
			else 
			{
				for ( int i=0; i<EHEFI_End; ++i )
				{
					if( pWnd==m_pIBHolyEquip[i]  )
					{
// 						ItemContainer* pHolyEquip = ItemMgr::Inst()->GetHolyEquip();
// 						if( P_VALID(pHolyEquip) )
// 						{
// 							Item* pItem = pHolyEquip->GetValue( (INT16) i );
// 							if ( P_VALID(pItem) )
// 								ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
// 							else
// 							{
// 								tstringstream tssPos;
// 								tssPos<<_T("HolyMan_Tips4")<< i ;
// 								ShowTip(pWnd, g_StrTable[tssPos.str().c_str()] );
// 							}
// 						}
						break;
					}					
				}
			}
			
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(NULL, _T(""));
		}
		break;
	default:
		break;
	}
	return 1;
}

BOOL HolyManFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	// 注册游戏事件处理函数
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateHolyManEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnUpdateHolyManEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_Move2Equip"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnChangeItemEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPSkillEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnActiveHolySkillEvent));

	m_pFrameMgr->RegisterEventHandle(_T("tagSpecItemEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnUseSpecItemEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagClickItemEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::ClickItemEvent));
	m_pFrameMgr->RegisterEventHandle(_T("EquipDrop"),							(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnItemDropCheck));
	//m_pFrameMgr->RegisterEventHandle(_T("RefreshMaintainValueUI"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::RevRefreshFrame));

	m_pCmdMgr->Register("NS_ItemRemove",(NETMSGPROC)m_Trunk.sfp2(&HolyManFrame::NetItemRemove), _T("NS_ItemRemove"));

	m_pCmdMgr->Register("NS_GetHolyAffuse",	(NETMSGPROC)m_Trunk.sfp2(&HolyManFrame::OnNS_GetHolyAffuse),	_T("NS_GetHolyAffuse") );
	m_pCmdMgr->Register("NS_GetHolyReborn",	(NETMSGPROC)m_Trunk.sfp2(&HolyManFrame::OnNS_GetHolyReborn),	_T("NS_GetHolyReborn") );
	
	m_pKeyMap->Register(HolyMan_WakeUp,	g_StrTable[_T("KEY_HOLY_WAKE")],	(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&HolyManFrame::OnKey),DIK_Z);

	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	m_pResHolyMenValue = IDraw2D::Inst()->NewTexture(_T("data\\ui\\common\\L_jindu-glass-e-j.bmp"),pIFS);

	return bRet;
}

BOOL HolyManFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	if( P_VALID(m_pWndHolyState) )
	{
		m_pGUI->AddToDestroyList(m_pWndHolyState);
		m_pWndHolyState = NULL;
	}
	m_pHolyEquip = NULL;
	// 注销游戏事件处理函数
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateHolyManEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnUpdateHolyManEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Equip"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnChangeItemEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPSkillEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnActiveHolySkillEvent));

	m_pFrameMgr->UnRegisterEventHandler(_T("tagSpecItemEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnUseSpecItemEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagClickItemEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::ClickItemEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("EquipDrop"),								(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::OnItemDropCheck));
	//m_pFrameMgr->UnRegisterEventHandler(_T("RefreshMaintainValueUI"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyManFrame::RevRefreshFrame));

	m_pCmdMgr->UnRegister("NS_ItemRemove",(NETMSGPROC)m_Trunk.sfp2(&HolyManFrame::NetItemRemove));

	m_pCmdMgr->UnRegister("NS_GetHolyAffuse",		(NETMSGPROC)m_Trunk.sfp2(&HolyManFrame::OnNS_GetHolyAffuse));
	m_pCmdMgr->UnRegister("NS_GetHolyReborn",		(NETMSGPROC)m_Trunk.sfp2(&HolyManFrame::OnNS_GetHolyReborn));

	m_pKeyMap->SetEnable( g_StrTable[_T("KEY_HOLY_WAKE")], FALSE );

	IDraw2D::Inst()->ReleaseTexture(m_pResHolyMenValue);

	return bRet;
}

BOOL HolyManFrame::ReloadUI()
{
	//-----------元神界面------------
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\soul.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\att\\soulpic", "ClassName", "HolyManView");
	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\upgrade\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\upgrade\\IMicon", "ClassName", "ItemButton");
	for ( int i=0; i<EHEFI_End; ++i )
	{
		char cbuff[X_LONG_NAME];
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"soulwin\\soulback\\att\\icon%d",i );
		m_pGUI->ChangeXml(&element, /*tss.str().c_str()*/cbuff, "ClassName", "ItemButton");
	}

	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\equip\\itemback\\item", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\equip\\starback\\itemback\\item", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\equip\\holeback\\itemback\\item", "ClassName", "ItemButton");

	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\equip\\studback\\itemback0\\item", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "soulwin\\soulback\\equip\\studback\\itemback1\\item", "ClassName", "ItemButton");

	for ( int i=0; i<MAX_EQUIPHOLE_NUM; ++i )
	{
		char cbuff[X_LONG_NAME];
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"soulwin\\soulback\\equip\\holeback\\holeback\\hole%d",i );
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "StaticEx");

		char cbuff1[X_LONG_NAME];
		_snprintf(cbuff1, sizeof(cbuff1)/sizeof(char),"soulwin\\soulback\\equip\\studback\\holeback\\hole%d",i );
		m_pGUI->ChangeXml(&element, cbuff1, "ClassName", "StaticEx");

		//char cbuff2[X_LONG_NAME];
		//_snprintf(cbuff2, sizeof(cbuff2)/sizeof(char),"soulwin\\soulback\\equip\\studback\\holeback\\hole%d\\choose",i );
		//m_pGUI->ChangeXml(&element, cbuff2, "ClassName", "GUIPushButton");
	}

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element); 

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&HolyManFrame::EventHandler));

	m_pWndCaption		= m_pWnd->GetChild(_T("soulback"));
	m_pBtnClose			=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\close") );
	m_pPthAtt				=	 (GUIPatch*)m_pWnd->GetChild( _T("soulback\\att") );
	m_pPthLv					=	 (GUIPatch*)m_pWnd->GetChild( _T("soulback\\upgrade") );
	m_pPBAtt					=	 (GUIPushButton*)m_pWnd->GetChild( _T("soulback\\frame\\att") );
	m_pPBLv					=	 (GUIPushButton*)m_pWnd->GetChild( _T("soulback\\frame\\upgrade") );

	//元神属性分页
	m_pHolyManView		=	 (HolyManView*)m_pWnd->GetChild( _T("soulback\\att\\soulpic") );
	m_pBtnLeft				=	 (GUIButton*)m_pHolyManView->GetChild( _T("lastbutt") );
	m_pBtnRight				=	 (GUIButton*)m_pHolyManView->GetChild( _T("nextbutt") );

	m_pStcHolyLv_1		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\junior") );
	m_pStcHolyLv_2		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\intermediate") );
	m_pStcHolyLv_3		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\senior") );
	m_pStcHolyLvData	=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\level") );

	m_pProHolyExp		=	 (GUIProgress*)m_pWnd->GetChild( _T("soulback\\att\\expbar") );
	m_pProHolyValue		=	 (GUIProgress*)m_pWnd->GetChild( _T("soulback\\att\\soulbar") );
	m_pProHolyPrivity	=	 (GUIProgress*)m_pWnd->GetChild( _T("soulback\\att\\privitybar") );

	m_pStcDamage		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num0") );
	m_pStcHitRate			=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num1") );
	m_pStcDeath			=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num2") );
	m_pStcDeathValue	=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num3") );
	m_pStcOutAttack		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num4") );
	m_pStcInAttack			=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num5") );
	m_pStcExact				=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num6") );
	m_pStcExtraDamage	=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num7") );
	m_pStcIgnoreTough	=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num8") );
	m_pStcMorale			=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\att\\wordback\\num9") );

	TCHAR buffer[X_LONG_NAME];
	for (int j=0; j<EHEFI_End; ++j)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("soulback\\att\\icon%d"), j );
		m_pIBHolyEquip[j]	=	(ItemButton*)m_pWnd->GetChild(buffer);		
	}

	//元神升级分页	
	m_pStcHolyLv1_1		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\upgrade\\junior") );
	m_pStcHolyLv1_2		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\upgrade\\intermediate") );
	m_pStcHolyLv1_3		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\upgrade\\senior") );
	m_pStcHolyLvData1	=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\upgrade\\level") );

	m_pProHolyExp1		=	 (GUIProgress*)m_pWnd->GetChild( _T("soulback\\upgrade\\expbar") );
	m_pProHolyValue1	=	 (GUIProgress*)m_pWnd->GetChild( _T("soulback\\upgrade\\soulbar") );
	m_pProHolyPrivity1	=	 (GUIProgress*)m_pWnd->GetChild( _T("soulback\\upgrade\\privitybar") );

	m_pEBAffuseExp		=	 (GUIEditBox*)m_pWnd->GetChild( _T("soulback\\upgrade\\expnum") );
	m_pBtnAffuseMax	=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\max") );
	m_pBtnAffuseOK		=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\surebutt0") );
	m_pBtnAffuseCancel=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\cancle0") );

	m_pIBRebornItem		=	 (ItemButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\icon") );
	m_pIBRebornStuff		=	 (ItemButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\IMicon") );
	m_pBtnRebornOK		=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\surebutt1") );
	m_pBtnRebCancel		=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\cancle1") );
	m_pBtnAddStuff		=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\plus") );
	m_pBtnMinuStuff		=	 (GUIButton*)m_pWnd->GetChild( _T("soulback\\upgrade\\minus") );
	m_pProReborn			=	 (GUIProgress*)m_pWnd->GetChild( _T("soulback\\upgrade\\progressbar\\pro") );
	m_PStcRebornRate	=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\upgrade\\word\\num") );
	m_pStcSucceed		=	 (GUIStatic*)m_pWnd->GetChild( _T("soulback\\upgrade\\success") );

	m_pPBEquip				=	 (GUIPushButton*)m_pWnd->GetChild( _T("soulback\\frame\\equip") );
	m_pHolyEquip			= (HolyEquipFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("HolyEquipFrame"), _T("HolyEquipFrame"), 0);
	if(!P_VALID(m_pHolyEquip))
		IMSG(_T("Create HolyEquipFrame failed"));

	m_pWnd->SetInvisible(TRUE);

	//-----------元神状态界面------------
	XmlElement ele;
	tstring strPath1 = g_strLocalPath + _T("\\ui\\soul_state.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath1.c_str());
	m_pWndHolyState = m_pGUI->CreateWnd(_T("\\desktop"), &ele); 

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndHolyState->GetFullName().c_str(), m_Trunk.sfp1(&HolyManFrame::StateEventHandler));
 
	m_pStcNormalState	=	 (GUIStatic*)m_pWndHolyState->GetChild( _T("normalpic") );
	m_pBtnHolyState		=	 (GUIButton*)m_pWndHolyState->GetChild( _T("soulbut") );
	TCHAR buffer1[X_LONG_NAME];
	for ( int k=0; k<STATE_VALUE_RATE; ++k )
	{
		_sntprintf(buffer1, sizeof(buffer1)/sizeof(TCHAR), _T("state\\pic%d\\full"), k );
		m_pStcStateRate[k]	=	(GUIStatic*)m_pWndHolyState->GetChild(buffer1);		
	}
	m_pWndHolyState->SetInvisible(TRUE);

	return TRUE;
}

VOID HolyManFrame::Update()
{
	GameFrame::Update();

	m_dwTimeEclipse += Kernel::Inst()->GetDeltaTimeDW();
	if( m_dwTimeEclipse > 1000 )//每隔1秒检测
	{
		m_dwTimeEclipse = 0;

		++m_dwCurTime;
		if ( m_dwCurTime>2 ) //约2秒
		{
			m_dwCurTime = 0;	
			if (P_VALID(m_pBtnRebornOK))
				m_pBtnRebornOK->SetEnable(TRUE);
			if (P_VALID(m_pBtnAffuseOK))
				m_pBtnAffuseOK->SetEnable(TRUE);
			if (P_VALID(m_pStcSucceed))
				m_pStcSucceed->SetInvisible(TRUE);
		}

		++m_dwCurTimeState;
		if ( m_dwCurTimeState>10 ) //约10秒
		{
			m_dwCurTimeState = 0;	
			if (P_VALID(m_pBtnHolyState))
				m_pBtnHolyState->SetEnable(TRUE);
		}

		if (m_bIfProgress)
		{
			if (P_VALID(m_pPthLv)&&(!m_pPthLv->IsInvisible()))
			{
				float fCurVal = m_pProReborn->GetCurrentValue();
				if ( fCurVal >= g_fRebornTime )
				{
					SendHolyRebornMsg();
					m_bIfProgress = FALSE;
				} 
			}
		}

	}

}

BOOL HolyManFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;
 
	ShowWnd(FALSE);

	return TRUE;
}

VOID HolyManFrame::Render3D()
{
	
	Update3DHolyValue();

	BOOL bShowHolyMenView = TRUE;
	if (bShowHolyMenView)
	{
		m_pHolyManView->Render3D();
	}

    GameFrame::Render3D();

}

VOID HolyManFrame::Update3DHolyValue()
{
	BOOL bShowHolyMenValue = HolyManMgr::Inst()->IsHolyAwake();
	if( !bShowHolyMenValue )
		return ;
	static int nWidth = 75;
	static int nHeight = 8;
	ClientCamera* pCamera = GetWorldFrm()->GetCamera();
	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();

	if( !P_VALID(pCamera) || !P_VALID(pLP) ) 
		return;
	
	const Vector3& lookFrom = pCamera->GetPos();
	
	IDraw2D::Inst()->BeginDraw(IDraw2D::EBlend_Alpha, true);
	Vector3 worldPos,srcPos;

	DWORD dwLpId = RoleMgr::Inst()->GetLocalPlayerID();
	VirtualRole*pVp =  RoleMgr::Inst()->GetVirtualRole(dwLpId);
	if ( !P_VALID(pVp) || pVp->IsHide() )
		return;
	pVp->GetHeadPos(worldPos);

	srcPos = pCamera->WorldToScreenF(worldPos);
	srcPos.z *= 0.5f;

	if(srcPos.z<=0 || srcPos.z>=0.5f)
		return;
	//srcPos.z = 1.0f;

	POINT pt = pCamera->WorldToScreen(worldPos);
	pt.y -= 15;


	const tagHolySoulInfo HolySoulInfo = HolyManMgr::Inst()->GetHolyManAtt();
	if ( 0 == HolySoulInfo.SoulAttEx.dwSoulID )
		return;
	tagSoulAttEx  soulAttEx = HolySoulInfo.SoulAttEx;
	const INT nValue		 =  HolyManMgr::Inst()->GetHolyManValue();
	const INT nMaxValue =  soulAttEx.nMaxSoulValue;
	INT nPerValue	 = 0;
	if ( nMaxValue!=0 && nValue<=nMaxValue )
		nPerValue = (INT)(((float)nValue)/((float)nMaxValue)*((float)nWidth));

	RECT destRC,srcRC;

	srcRC.left = srcRC.top = 0;
	srcRC.right = srcRC.left+nWidth;
	srcRC.bottom = nHeight;

	destRC.left = pt.x - nWidth/2;
	destRC.right = destRC.left + nPerValue;
	destRC.top = pt.y-nHeight/2;
	destRC.bottom = destRC.top+nHeight/2;

#ifdef HolyMan_Test_Define
	IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResHolyMenValue, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, g_fZValue);
#else
	IDraw2D::Inst()->Draw(&destRC, &srcRC, m_pResHolyMenValue, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, srcPos.z);
#endif

	IDraw2D::Inst()->EndDraw();
}

VOID HolyManFrame::ShowWnd(BOOL bShow)
{
	if ( !P_VALID(m_pWnd) )
		return;

	BOOL bActive = HolyManMgr::Inst()->IsActiveHolyMan();
	if ( !bActive && bShow )
	{
		HolyManMgr::Inst()->ShowErrorTips(8);
		return;
	}

	m_pWnd->SetInvisible(!bShow);
	if (bShow)
	{
		m_pPthAtt->SetInvisible(FALSE);
		m_pPthLv->SetInvisible(TRUE);
		m_pPBAtt->SetState( EGUIBS_PushDown );
		m_pHolyManView->SetHolyManShow( );
		
		HolyManMgr::Inst()->UpdateSoulAttValue();
		
		//关掉相关的互斥页面
		tagGameEvent event(_T("Close_MaintainValue"), this);
		m_pMgr->SendEvent(&event);
	}
	else
	{
		m_bIfProgress = FALSE;
		m_pBtnRebornOK->SetEnable(TRUE);
		m_pProReborn->SetValue(0,TRUE);
		LockItem(FALSE);
		m_pRebornItem = NULL;
		m_pRebornStuff = NULL;
		
		if ( P_VALID(m_pHolyEquip) ) //关闭的时候一起关
			m_pHolyEquip->ShowWnd(bShow);
	}

}

BOOL HolyManFrame::IsShowing()
{ 
	if ( P_VALID(m_pWnd) && !m_pWnd->IsInvisible() )
		return TRUE;
	return FALSE;
}

LPCTSTR HolyManFrame::Int2String(tstring& strOut,INT nNum1,INT nNum2/*=GT_INVALID*/)
{
	tstringstream tss;
	tss<<nNum1;
	if (nNum2 != GT_INVALID)
		tss<<_T("/")<<nNum2;
	strOut = tss.str();
	return strOut.c_str();
}

BOOL HolyManFrame::AddItem(INT16 nPos)
{
	if ( !P_VALID(m_pPthLv) || m_pPthLv->IsInvisible() || m_bIfProgress )
		return FALSE;

	Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos); 
	if ( !P_VALID(pItem) )
		return FALSE;

	//老君仙露
	if ( pItem->GetItemTypeID() == REBORN_STUFF_BINDING 
		|| pItem->GetItemTypeID() == REBORN_STUFF_UNBINDING )
	{
		m_pRebornStuff = pItem;

		DWORD dwTypeID			= m_pRebornStuff->GetItemTypeID();
		ItemContainer* pPocket	=  ItemMgr::Inst()->GetPocket();
		INT		 nCount				= pPocket->GetItemQuantity(dwTypeID);
		INT nFullRateMinNum = HolyManMgr::Inst()->GetFullRateMinNum();
		if ( nCount < nFullRateMinNum )
			m_nRebornStuffNum = nCount;
		else
			m_nRebornStuffNum = nFullRateMinNum;

		RefreshUI_Reborn();
	}
	//幻元玉魄
	else if (	pItem->GetItemTypeID() == REBORN_ITEM_TYPEID_LV1
		|| pItem->GetItemTypeID() == REBORN_ITEM_TYPEID_LV2
		|| pItem->GetItemTypeID() == REBORN_ITEM_TYPEID_LV3 )
	{
		m_pRebornItem = pItem;
		RefreshUI_Reborn();
	}

	return TRUE;

}


VOID HolyManFrame::LockItem(bool bOperable)
{
	if( P_VALID(m_pRebornItem) )
		m_pRebornItem->SetOperable(!bOperable);

	if( P_VALID(m_pRebornStuff) )
		m_pRebornStuff->SetOperable(!bOperable);

	m_pIBRebornItem->SetUnClickable(bOperable);
	m_pIBRebornStuff->SetUnClickable(bOperable);

	m_pBtnAddStuff->SetEnable(!bOperable);
	m_pBtnMinuStuff->SetEnable(!bOperable);

}

BOOL	HolyManFrame::ClearUI_Att()
{
	if ( !P_VALID(m_pWnd) || m_pPthAtt->IsInvisible() )
		return TRUE;

	for ( int i=0; i<EHEFI_End; ++i )
	{
		m_pIBHolyEquip[i]->RefreshItem();
	}

	m_pIBHolyEquip[0]->SetPicFileName(szPic_HolyMan0);
	m_pIBHolyEquip[1]->SetPicFileName(szPic_HolyMan1);
	m_pIBHolyEquip[2]->SetPicFileName(szPic_HolyMan2);
	m_pIBHolyEquip[3]->SetPicFileName(szPic_HolyMan3);
	m_pIBHolyEquip[4]->SetPicFileName(szPic_HolyMan4);
	m_pIBHolyEquip[5]->SetPicFileName(szPic_HolyMan5);
	m_pIBHolyEquip[6]->SetPicFileName(szPic_HolyMan6);
	m_pIBHolyEquip[7]->SetPicFileName(szPic_HolyMan7);
	m_pIBHolyEquip[8]->SetPicFileName(szPic_HolyMan8);
	m_pIBHolyEquip[9]->SetPicFileName(szPic_HolyMan9);

	m_pStcHolyLv_1->SetInvisible(TRUE);
	m_pStcHolyLv_2->SetInvisible(TRUE);
	m_pStcHolyLv_3->SetInvisible(TRUE);
	m_pStcHolyLvData->SetText(_T(""));

	m_pProHolyExp->SetValue(0,TRUE);
	m_pProHolyExp->SetText(_T(""));
	m_pProHolyValue->SetValue(0,TRUE);
	m_pProHolyValue->SetText(_T(""));
	m_pProHolyPrivity->SetValue(0,TRUE);
	m_pProHolyPrivity->SetText(_T(""));

	m_pStcDamage->SetText(_T(""));
	m_pStcHitRate->SetText(_T(""));
	m_pStcDeath->SetText(_T(""));
	m_pStcDeathValue->SetText(_T(""));
	m_pStcOutAttack->SetText(_T(""));
	m_pStcInAttack->SetText(_T(""));
	m_pStcExact->SetText(_T(""));
	m_pStcExtraDamage->SetText(_T(""));
	m_pStcIgnoreTough->SetText(_T(""));
	m_pStcMorale->SetText(_T(""));

	return FALSE;
}

BOOL	HolyManFrame::ClearUI_Lv()
{
	if ( !P_VALID(m_pWnd) || m_pPthLv->IsInvisible() )
		return TRUE;
 
	m_pStcHolyLv1_1->SetInvisible(TRUE);
	m_pStcHolyLv1_2->SetInvisible(TRUE);
	m_pStcHolyLv1_3->SetInvisible(TRUE);
	m_pStcHolyLvData1->SetText(_T(""));

	m_pProHolyExp1->SetValue(0,TRUE);
	m_pProHolyExp1->SetText(_T(""));
	m_pProHolyValue1->SetValue(0,TRUE);
	m_pProHolyValue1->SetText(_T(""));
	m_pProHolyPrivity1->SetValue(0,TRUE);
	m_pProHolyPrivity1->SetText(_T(""));

	//m_pEBAffuseExp->SetText(_T(""));
	m_pIBRebornItem->RefreshItem();
	m_pIBRebornStuff->RefreshItem();
	m_pProReborn->SetValue(0,TRUE);

	return FALSE;
}

VOID	HolyManFrame::RefreshUI_Att()
{
// 	if ( ClearUI_Att() )			
// 		return;
// 	
// 	//御览框
// 	m_pHolyManView->SetHolyManShow( );
// 	
// 	//装备栏
// 	for ( int i=0; i<EHEFI_End; ++i )
// 	{
// 		Item* pEquip = ItemMgr::Inst()->GetConItem( EICT_HolyEquip, i );
// 		if ( !P_VALID(pEquip) )
// 			continue;
// 		m_pIBHolyEquip[i]->RefreshItem( pEquip->GetItemTypeID(), 0, pEquip->GetItemQuality() );
// 	}
// 
// 	const tagHolySoulInfo HolySoulInfo = HolyManMgr::Inst()->GetHolyManAtt();
// 	if ( 0 == HolySoulInfo.SoulAttEx.dwSoulID ) //表明没有没有更新
// 		return;
// 
// 	tagSoulAttEx  soulAttEx = HolySoulInfo.SoulAttEx;
// 	//经验值进度条
// 	INT  nMaxExp = INT((soulAttEx.n64MaxExp)>>16 ), nCurExp = INT((soulAttEx.n64LevelExp)>>16) ;
// 	if ( nCurExp == 0 || nMaxExp ==0 )
// 	{
// 		nCurExp	= INT ( soulAttEx.n64LevelExp );
// 		nMaxExp	= INT ( soulAttEx.n64MaxExp );
// 	}
// 	if ( nMaxExp <= nCurExp  )  
// 		nMaxExp = nCurExp;
// 	m_pProHolyExp->SetMaxValue((float)nMaxExp);
// 	m_pProHolyExp->SetValue((float)nCurExp,TRUE);
// 	tstringstream tssExp;
// 	tssExp<< soulAttEx.n64LevelExp<<_T("/")<<soulAttEx.n64MaxExp; 
// 	m_pProHolyExp->SetText( tssExp.str().c_str() );
// 	tstring strOut;
// 	//元神值进度条
// 	m_pProHolyValue->SetMaxValue(float(soulAttEx.nMaxSoulValue));
// 	m_pProHolyValue->SetValue( float(soulAttEx.nCurSoulValue) ,TRUE);
// 	m_pProHolyValue->SetText( Int2String(strOut,soulAttEx.nCurSoulValue, soulAttEx.nMaxSoulValue) );
// 	//默契值进度条
// 	m_pProHolyPrivity->SetMaxValue(float(soulAttEx.nMaxCovalue));
// 	m_pProHolyPrivity->SetValue(float(soulAttEx.nCurCovalue),TRUE);
// 	m_pProHolyPrivity->SetText( Int2String(strOut,soulAttEx.nCurCovalue, soulAttEx.nMaxCovalue)  );
// 
// 	tagSoulAtt  soulAtt = HolySoulInfo.SoulAtt;
// 	//元神级别（高中低）
// 	if ( 20000 == soulAtt.nSoulAtt[ESAT_RebornLv] )
// 		m_pStcHolyLv_2->SetInvisible(FALSE);
// 	else if (  30000 == soulAtt.nSoulAtt[ESAT_RebornLv] )
// 		m_pStcHolyLv_3->SetInvisible(FALSE);
// 	else // if ( 1 == soulAtt.nSoulAtt[ESAT_RebornLv]  )
// 		m_pStcHolyLv_1->SetInvisible(FALSE);
// 
// 	m_pStcHolyLvData->SetText( Int2String( strOut,soulAtt.nSoulAtt[ESAT_SoulLevel] ) );
// 	m_pStcDamage->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Demage]) );
// 	m_pStcHitRate->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Hit]) );
// 	m_pStcDeath->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Crit]) );
// 	m_pStcDeathValue->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_CritAmount]) );
// 	m_pStcOutAttack->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Exattack]) );
// 	m_pStcInAttack->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Inattack]) );
// 	m_pStcExact->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Accrate]) );
// 	m_pStcExtraDamage->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Exdamage]) );
// 	m_pStcIgnoreTough->SetText( Int2String(strOut,soulAtt.nSoulAtt[ESAT_Toughness]) );
// 	m_pStcMorale->SetText( Int2String( strOut,INT(soulAtt.nSoulAtt[ESAT_Morale]) ) );

}

VOID	HolyManFrame::RefreshUI_Lv()
{
	if ( ClearUI_Lv() )
		return;

	const tagHolySoulInfo HolySoulInfo = HolyManMgr::Inst()->GetHolyManAtt();
	if ( 0 == HolySoulInfo.SoulAttEx.dwSoulID ) //表明没有没有更新
		return;

	tagSoulAttEx  soulAttEx = HolySoulInfo.SoulAttEx;
	//经验值进度条
	INT  nMaxExp = INT((soulAttEx.n64MaxExp)>>16 ), nCurExp = INT((soulAttEx.n64LevelExp)>>16) ;
	if ( nCurExp == 0 || nMaxExp ==0 )
	{
		nCurExp	= INT ( soulAttEx.n64LevelExp );
		nMaxExp	= INT ( soulAttEx.n64MaxExp );
	}
	if ( nMaxExp <= nCurExp  )  
		nMaxExp = nCurExp;
	m_pProHolyExp1->SetMaxValue((float)nMaxExp);
	m_pProHolyExp1->SetValue((float)nCurExp,TRUE);
	tstringstream tssExp;
	tssExp<< soulAttEx.n64LevelExp <<_T("/")<<soulAttEx.n64MaxExp; 
	m_pProHolyExp1->SetText( tssExp.str().c_str() );
	tstring strOut;
	//元神值进度条
	m_pProHolyValue1->SetMaxValue(float(soulAttEx.nMaxSoulValue));
	m_pProHolyValue1->SetValue( float(soulAttEx.nCurSoulValue) ,TRUE);
	m_pProHolyValue1->SetText( Int2String(strOut,soulAttEx.nCurSoulValue, soulAttEx.nMaxSoulValue) );
	//默契值进度条
	m_pProHolyPrivity1->SetMaxValue(float(soulAttEx.nMaxCovalue));
	m_pProHolyPrivity1->SetValue(float(soulAttEx.nCurCovalue),TRUE);
	m_pProHolyPrivity1->SetText( Int2String(strOut,soulAttEx.nCurCovalue, soulAttEx.nMaxCovalue)  );

	tagSoulAtt  soulAtt = HolySoulInfo.SoulAtt;
	//元神级别（高中低）
// 	if ( 20000 == soulAtt.nSoulAtt[ESAT_RebornLv] )
// 		m_pStcHolyLv1_2->SetInvisible(FALSE);
// 	else if (  30000 == soulAtt.nSoulAtt[ESAT_RebornLv] )
// 		m_pStcHolyLv1_3->SetInvisible(FALSE);
// 	else // if ( 1 == soulAtt.nSoulAtt[ESAT_RebornLv]  )
// 		m_pStcHolyLv1_1->SetInvisible(FALSE);

	//m_pStcHolyLvData1->SetText( Int2String(strOut, soulAtt.nSoulAtt[ESAT_SoulLevel] ) );

	RefreshUI_Reborn();

}

BOOL	HolyManFrame::ClearUI_State()
{
	if ( !P_VALID(m_pWndHolyState) || m_pWndHolyState->IsInvisible() )
		return TRUE;

	for ( int j=0; j<STATE_VALUE_RATE; ++j )
	{
		m_pStcStateRate[j]->SetInvisible(TRUE);
	}

	m_pStcNormalState->SetInvisible(FALSE);
	m_pBtnHolyState->SetInvisible(TRUE);

	return FALSE;
}

VOID	HolyManFrame::Update2DHolyValue()
{
	if ( !P_VALID(m_pWnd)  )
		return ;
	const tagHolySoulInfo HolySoulInfo = HolyManMgr::Inst()->GetHolyManAtt();
	if ( 0 == HolySoulInfo.SoulAttEx.dwSoulID ) //表明没有没有更新
		return;

	tagSoulAttEx  soulAttEx = HolySoulInfo.SoulAttEx;
	tstring strOut;
	if ( !m_pPthAtt->IsInvisible() )
	{
		//元神值进度条1
		m_pProHolyValue->SetMaxValue(float(soulAttEx.nMaxSoulValue));
		m_pProHolyValue->SetValue( float(soulAttEx.nCurSoulValue) ,TRUE);
		m_pProHolyValue->SetText( Int2String(strOut,soulAttEx.nCurSoulValue, soulAttEx.nMaxSoulValue) );
		m_pProHolyValue->SetRefresh(TRUE);
	}
	else if ( !m_pPthLv->IsInvisible() )
	{
		//元神值进度条2
		m_pProHolyValue1->SetMaxValue(float(soulAttEx.nMaxSoulValue));
		m_pProHolyValue1->SetValue( float(soulAttEx.nCurSoulValue) ,TRUE);
		m_pProHolyValue1->SetText( Int2String(strOut,soulAttEx.nCurSoulValue, soulAttEx.nMaxSoulValue) );
		m_pProHolyValue1->SetRefresh(TRUE);
	}
}

VOID	HolyManFrame::UpdateCovalue()
{
	if ( !P_VALID(m_pWnd)  )
		return ;
	const tagHolySoulInfo HolySoulInfo = HolyManMgr::Inst()->GetHolyManAtt();
	if ( 0 == HolySoulInfo.SoulAttEx.dwSoulID ) //表明没有没有更新
		return;

	tagSoulAttEx  soulAttEx = HolySoulInfo.SoulAttEx;
	tstring strOut;
	if ( !m_pPthAtt->IsInvisible() )
	{
		//元神默契值条1
		m_pProHolyPrivity->SetMaxValue(float(soulAttEx.nMaxCovalue));
		m_pProHolyPrivity->SetValue(float(soulAttEx.nCurCovalue),TRUE);
		m_pProHolyPrivity->SetText( Int2String(strOut,soulAttEx.nCurCovalue, soulAttEx.nMaxCovalue)  );
		m_pProHolyPrivity->SetRefresh(TRUE);
	}
	else if ( !m_pPthLv->IsInvisible() )
	{
		//元神默契值条2
		m_pProHolyPrivity1->SetMaxValue(float(soulAttEx.nMaxCovalue));
		m_pProHolyPrivity1->SetValue(float(soulAttEx.nCurCovalue),TRUE);
		m_pProHolyPrivity1->SetText( Int2String(strOut,soulAttEx.nCurCovalue, soulAttEx.nMaxCovalue)  );
		m_pProHolyPrivity1->SetRefresh(TRUE);
	}
}

VOID	HolyManFrame::RefreshUI_State()
{
	if ( ClearUI_State() )
		return;

	const tagHolySoulInfo HolySoulInfo = HolyManMgr::Inst()->GetHolyManAtt();
	if ( 0 == HolySoulInfo.SoulAttEx.dwSoulID ) //表明没有没有更新
		return;

	tagSoulAttEx  soulAttEx = HolySoulInfo.SoulAttEx;

	//元神值进度条
	INT nCompareNum = INT(STATE_VALUE_COMPARE);
	INT nRate = 0;
	if ( soulAttEx.nCurSoulValue >= STATE_VALUE_COMPARE )
	{
		m_pBtnHolyState->SetInvisible(FALSE);
		m_pStcNormalState->SetInvisible(TRUE);
	}
	 
	if ( STATE_VALUE_RATE*nCompareNum <= soulAttEx.nCurSoulValue )
	{
		nRate = STATE_VALUE_RATE;
	
		tagPoint posWnd = m_pBtnHolyState->GetView();
		tagPoint sizeWnd = m_pBtnHolyState->GetSize();
		LONG top, left, right, bottom;
		top		= posWnd.x - sizeWnd.x/2;
		left		= posWnd.y-sizeWnd.y;
		right		= top + 2*sizeWnd.x;
		bottom	= left + 2*sizeWnd.y;
		tagRect rect(top,left,right,bottom);
		Vector3 v3Pos(0.0f,-50.0f,0.0f) , v3Scale(2.0f,1.0f,2.0f);
		GUIEffectMgr::Inst()->PlayGuiEffect( _T("txjm03"),m_pBtnHolyState, rect, v3Pos, v3Scale );
	}
	else 
	{
		if ( 3*nCompareNum <= soulAttEx.nCurSoulValue )
			nRate = 3;
		else if ( 2*nCompareNum <= soulAttEx.nCurSoulValue )
			nRate = 2;
		else if ( 1*nCompareNum <= soulAttEx.nCurSoulValue)
		   nRate = 1;
		else
			nRate = 0;

		GUIEffectMgr::Inst()->StopGuiEffect( m_pBtnHolyState );
		//for ( int j=nRate; j<STATE_VALUE_RATE; ++j )
		//	GUIEffectMgr::Inst()->StopGuiEffect( m_pStcStateRate[j] );
	}

	if (nRate>=1&&nRate<=STATE_VALUE_RATE)
	{
		for ( int i=0; i<nRate; ++i )
		{
			m_pStcStateRate[i]->SetInvisible(FALSE);
			tagRect rect(0,0,0,0);
			Vector3 v3Pos(0.0f,0.0f,0.0f) , v3Scale(1.0f,1.0f,1.0f);
			/*if ( STATE_VALUE_RATE == nRate )
				GUIEffectMgr::Inst()->PlayGuiEffect( _T("txjm02"),m_pStcStateRate[i], rect, v3Pos, v3Scale );
			else
				GUIEffectMgr::Inst()->PlayGuiEffect( _T("txjm01"),m_pStcStateRate[i], rect, v3Pos, v3Scale );*/
		}
	}

}

VOID  HolyManFrame::RefreshUI_Reborn()
{
	HolyManMgr::Inst()->UpdateItem(&m_pRebornItem);
	HolyManMgr::Inst()->UpdateItem(&m_pRebornStuff);

	if ( !P_VALID(m_pRebornItem) )
		m_pIBRebornItem->RefreshItem( );
	else
		m_pIBRebornItem->RefreshItem( m_pRebornItem->GetItemTypeID(), 0 , m_pRebornItem->GetItemQuality() );

	if ( !P_VALID(m_pRebornStuff) )
	{
		m_pIBRebornStuff->RefreshItem( );
		m_nRebornStuffNum = 0;
	}
	else
	{
		m_pIBRebornStuff->RefreshItem( m_pRebornStuff->GetItemTypeID(), m_nRebornStuffNum , m_pRebornStuff->GetItemQuality() );
	}

	if ( P_VALID(m_pRebornStuff) && P_VALID(m_pRebornItem) )
	{
		INT nRate = HolyManMgr::Inst()->GetRateByItemNum(m_nRebornStuffNum);
		tstringstream tssRate;
		tssRate<<nRate<<_T("%");
		m_PStcRebornRate->SetText( tssRate.str().c_str() );
	}
	else
	{
		m_PStcRebornRate->SetText( _T("0%") );
	}

	m_pPthLv->SetRefresh(TRUE);
}

DWORD HolyManFrame::StateEventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd==m_pBtnHolyState )
			{
				//RoleMgr::Inst()->HolyManCtrl( RoleMgr::Inst()->GetLocalPlayerID(), EPAT_CALL,  HolyManMgr::HolyMan_BuffID );
				SendCallHolyManMsg();
			}
		}
		break;
	case EGUIE_Drag:
		{

		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for ( int i=0; i<STATE_VALUE_RATE; ++i )
			{
				if (  pWnd == m_pStcStateRate[i]
				|| pWnd == m_pStcNormalState 
					|| pWnd == m_pBtnHolyState )
				{
					ShowTip(pWnd, g_StrTable[ _T("HolyMan_Tips54") ] );
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(NULL, _T(""));
		}
		break;
	default:
		break;
	}
	return 1;
}

VOID	HolyManFrame::SendCallHolyManMsg()
{
	if ( !P_VALID(m_pBtnHolyState) || !m_pBtnHolyState->IsEnable() )
		return;

	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if ( pLP->GetRoleState()&ERS_PrisonArea
	  || pLP->GetRoleState()&ERS_Mount
	  || pLP->GetRoleState()&ERS_Mount2
	  || pLP->GetRoleState()&ERS_WaterWalk
	  || pLP->GetRoleState()&ERS_Stall 
	  || pLP->GetRoleState()&ERS_Transform  )
	{
		HolyManMgr::Inst()->ShowErrorTips(32);
		return;
	}

	if (pLP->IsPlayerHolyAwake()) //已处于 觉醒状态 则关掉
	{
		tagNC_CancelBuff cmd;
		cmd.dwBuffTypeID = pLP->GetHolyBuffID();
		m_pSession->Send( &cmd );
		return;
	}
	m_pBtnHolyState->SetEnable(FALSE);

	m_dwCurTimeState = 0;	 
	const tagHolySoulInfo HolySoulInfo = HolyManMgr::Inst()->GetHolyManAtt();
	if ( 0 == HolySoulInfo.SoulAttEx.dwSoulID ) //表明没有没有更新
		return;
	
	tagSoulAtt   SoulAtt = HolySoulInfo.SoulAtt;
	INT nRebornLv = 0;//SoulAtt.nSoulAtt[ESAT_RebornLv];
	
	tagUseSkillEvent event(_T("tagUseSkillEvent"), this);
	event.dwSkillID	 = DWORD((HolyManMgr::HolyMan_SkillID1)*100+1);
	//switch (nRebornLv)
	//{
	//case 20000:
	//	event.dwSkillID	 = DWORD((HolyManMgr::HolyMan_SkillID2)*100+nRebornLv/10000);
	//	break;
	//case 30000:
	//	event.dwSkillID	 = DWORD((HolyManMgr::HolyMan_SkillID3)*100+nRebornLv/10000);
	//	break;
	//default: //case 1:
	//	event.dwSkillID	 = DWORD((HolyManMgr::HolyMan_SkillID1)*100+nRebornLv/10000);
	//}
	event.dwTargetRoleID	= RoleMgr::Inst()->GetLocalPlayerID();
	m_pFrameMgr->SendEvent(&event);
 
	return;
}

VOID HolyManFrame::SendHolyAffuseMsg()
{
	BOOL IsCombat = HolyManMgr::Inst()->IsCombatState(HolyManMgr::EHCSCT_AFFUSE);
	if ( IsCombat )		return;
	tstring strExp = m_pEBAffuseExp->GetText();
	if ( strExp.empty() )
		return;
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(pLP))
	{
		INT64 n64Exp = 0, n64MaxExp = pLP->GetRoleLevelExp();
		tstringstream tssExp;
		tssExp<<strExp;
		tssExp>>n64Exp;
		if ( n64Exp >= 0 && n64Exp <= n64MaxExp)
		{
			m_dwCurTime = 0;

			if (P_VALID(m_pBtnAffuseOK))
				m_pBtnAffuseOK->SetEnable(FALSE);
			if (P_VALID(m_pEBAffuseExp))
				m_pEBAffuseExp->SetText(_T(""));

			m_n64AffuseHolyExp = n64Exp;

			if (n64Exp>0)
			{
				tagNC_GetHolyAffuse msg;
				msg.n64AffuseExp = n64Exp;
				m_pSession->Send(&msg);
			}
		}
		else 
		{
			HolyManMgr::Inst()->ShowErrorTips(22);
		}
	}
}

VOID HolyManFrame::SendHolyRebornMsg()
{
	if ( !P_VALID(m_pPthLv) || m_pPthLv->IsInvisible() || !CanReborn() )
		return;

	m_dwCurTime = 0;

	if (P_VALID(m_pBtnRebornOK))
		m_pBtnRebornOK->SetEnable(FALSE);

	tagNC_GetHolyReborn msg;
	msg.n64ItemID = m_pRebornItem->GetItemId();
	msg.n64StuffID = m_pRebornStuff->GetItemId();
	msg.nNumStuff = m_nRebornStuffNum;
	m_pSession->Send(&msg);

	HolyManMgr::Inst()->SetHolyInitState(TRUE);

}

BOOL HolyManFrame::CanReborn()
{
	BOOL bCan = TRUE;
	BOOL IsCombat = HolyManMgr::Inst()->IsCombatState(HolyManMgr::EHCSCT_REBORN);
	BOOL bHolyAwake = HolyManMgr::Inst()->IsHolyAwake();
	if ( IsCombat )	
	{
		HolyManMgr::Inst()->ShowErrorTips(52);
		bCan = FALSE;
	}
	else if (bHolyAwake)
	{
		HolyManMgr::Inst()->ShowErrorTips(236);
		bCan = FALSE;
	}
	else if ( !P_VALID(m_pRebornItem) ||  !P_VALID(m_pRebornStuff) )
	{
		HolyManMgr::Inst()->ShowErrorTips(233);
		bCan = FALSE;
	}
	else if ( 0 == m_nRebornStuffNum )
	{
		HolyManMgr::Inst()->ShowErrorTips(233);
		bCan = FALSE;
	}
	else if ( REBORN_MAX_LEVEL <= HolyManMgr::Inst()->GetRebornLevel() )
	{
		HolyManMgr::Inst()->ShowErrorTips(234);
		bCan = FALSE;
	}
	else if ( HOLYSOUL_MAX_LEVEL > HolyManMgr::Inst()->GetHolyManLevel() )
	{
		HolyManMgr::Inst()->ShowErrorTips(232);
		bCan = FALSE;
	}

	if ( !bCan )
	{
		LockItem(FALSE);
		m_bIfProgress = FALSE;
		m_pBtnRebornOK->SetEnable(TRUE);
		m_pProReborn->SetValue(0,TRUE);
	}

	return bCan;
}

DWORD HolyManFrame::OnUpdateHolyManEvent(tagUpdateHolyManEvent* pEvent)
{
	//刷新元神值
	if ( pEvent->eAttType == EHEAT_SoulValue )
	{
		HolyManMgr::Inst()->CheckAwakeState();
		Update2DHolyValue();
		RefreshUI_State();
	}
	else if ( pEvent->eAttType == EHEAT_Covalue )
	{
		UpdateCovalue();
	}
	//全刷新
	else 
	{
		RefreshUI_Att();
		RefreshUI_Lv();
		RefreshUI_State();
	}
	
	return 0;
}

//--处理物品换位事件
DWORD HolyManFrame::OnChangeItemEvent(tagItemPosChangeExEvent* pGameEvent)
{
	if ( !P_VALID(m_pWnd) || EICT_Bag != pGameEvent->eTypeSrc )
		return 0;

	//只有背包中的物品拖到装备栏才做穿装处理
	if( EICT_HolyEquip == pGameEvent->eTypeDst && !m_pPthAtt->IsInvisible() )
	{
		DWORD dwTypeID = pGameEvent->dwTypeIDSrc;
		EHolyEquipType ePosType = HolyManMgr::Inst()->GetHolyManEquipPos(dwTypeID);
		if ( EHET_NULL != ePosType )
		{
			EHolyEquipPos ePos = EHolyEquipPos (HolyManMgr::Inst()->TypeToIndex(ePosType));
			HolyManMgr::Inst()->SendMsg_HolyEquip( ePos, pGameEvent->n16IndexSrc );
		}	
	}
	else if ( !m_pPthLv->IsInvisible() )
	{
		AddItem( pGameEvent->n16IndexSrc);
		RefreshUI_Reborn();
	}

	return 0;
}

DWORD HolyManFrame::OnActiveHolySkillEvent(tagUpdateLPSkillEvent* pEvent)
{
	if ( HolyManMgr::Inst()->IsHolyAwakeSkillID( pEvent->dwID  )  
		&& pEvent->bAdd == TRUE )
	{
		if (P_VALID(m_pWndHolyState))
		{
			m_pWndHolyState->SetInvisible(FALSE);
			RefreshUI_State();
		}
	}
	return 0;
}

DWORD HolyManFrame::OnNS_GetHolyAffuse( tagNS_GetHolyAffuse *pNetCmd, DWORD )
{
	if ( pNetCmd->dwErrCode == E_Success ) 
	{
		HolyManMgr::Inst()->ShowErrorTips(30);
		if ( HolyManMgr::Inst()->HasUpdate() == FALSE)
			HolyManMgr::Inst()->AddSoulExp(m_n64AffuseHolyExp);
		else
			HolyManMgr::Inst()->NoUpdate();

		HolyManMgr::Inst()->UpdateSoulAttValue();
	}
	else
	{
		HolyManMgr::Inst()->ShowErrorTips(pNetCmd->dwErrCode);
	}

	m_n64AffuseHolyExp = 0;

	return 0;

}

DWORD HolyManFrame::OnNS_GetHolyReborn( tagNS_GetHolyReborn *pNetCmd, DWORD )
{
	m_bIfProgress = FALSE;
	m_pBtnRebornOK->SetEnable(TRUE);
	m_pProReborn->SetValue(0,TRUE);
	m_pProReborn->SetRefresh(TRUE);
	LockItem(FALSE);

	if ( pNetCmd->dwErrCode != E_Success && pNetCmd->dwErrCode != E_HolySoulReborn_Fail  ) 
	{
		HolyManMgr::Inst()->ShowErrorTips(pNetCmd->dwErrCode);
	}
	else
	{
		if ( pNetCmd->dwErrCode == E_Success )
		{
			m_pStcSucceed->SetInvisible(FALSE);
			m_pStcSucceed->FlipToTop();
			HolyManMgr::Inst()->SetHolyInitState(FALSE);
		}
		else  // E_HolySoulReborn_Fail
			HolyManMgr::Inst()->ShowErrorTips(pNetCmd->dwErrCode);

		m_pRebornItem = NULL;
		m_pRebornStuff = NULL;
		m_nRebornStuffNum = 0;
	}
	
	RefreshUI_Reborn();
	
	return 0;
}

//-----------------------------------------------------------------------------
//  按键消息处理
//-----------------------------------------------------------------------------
DWORD HolyManFrame::OnKey( DWORD dwID, BOOL bDown )
{
	if( dwID==HolyMan_WakeUp && bDown )
	{
		SendCallHolyManMsg();
	}

	return 0;
}

DWORD HolyManFrame::OnUseSpecItemEvent(tagSpecItemEvent* pGameEvent)
{
	if ( !P_VALID(m_pPthLv) || m_pPthLv->IsInvisible() || pGameEvent->eSpecFuncSrc != EISF_ProtectSign )
		return 0;
	Item* pItem = ItemMgr::Inst()->GetPocketItemByID(pGameEvent->n64Serial);
	if (P_VALID(pItem))
	{
		INT16 n16Pos = pItem->GetPos();
		AddItem( n16Pos);
	}
	return 0;
}

DWORD HolyManFrame::ClickItemEvent(tagClickItemEvent* pGameEvent)
{
	if ( !P_VALID(m_pPthLv) || m_pPthLv->IsInvisible() )
		return 0;
	Item* pItem = ItemMgr::Inst()->GetPocketItemByID(pGameEvent->n64ItemID);
	if (P_VALID(pItem))
	{
		INT16 n16Pos = pItem->GetPos();
		AddItem( n16Pos);
	}
	return 0;
}

DWORD HolyManFrame::OnItemDropCheck(tagEquipDropEvent* pGameEvent)
{
	if ( !P_VALID(m_pPthLv) || m_pPthLv->IsInvisible() )
		return 0;

	if ( P_VALID(m_pRebornItem) && m_pRebornItem->GetItemId() == pGameEvent->n64ItemID )
	{
		LockItem(FALSE);
		m_pRebornItem = NULL;
		RefreshUI_Reborn();
	}
	else if ( P_VALID(m_pRebornStuff) && m_pRebornStuff->GetItemId() == pGameEvent->n64ItemID )
	{
		LockItem(FALSE);
		m_pRebornStuff = NULL;
		RefreshUI_Reborn();
	}
	
	return 0;
}

// DWORD HolyManFrame::RevRefreshFrame( tagRefreshMaintainValueUIEvent* pGameEvent)
// {
// 	if ( !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
// 		return 0;
// 
// 	if ( EDT_Item == pGameEvent->eBagType )
// 	{
// 		DWORD dwTmpID1 = GT_INVALID;
// 		DWORD dwTmpID2 = GT_INVALID;
// 		Item* pItem1 = ItemMgr::Inst()->GetPocketItem( pGameEvent->dwPos );
// 		if ( P_VALID(pItem1) )
// 			dwTmpID1 = pItem1->GetItemTypeID();
// 		Item* pItem2 = ItemMgr::Inst()->GetPocketItem( pGameEvent->dwDst );
// 		if ( P_VALID(pItem2) )
// 			dwTmpID2= pItem2->GetItemTypeID();
// 
// 		if (P_VALID(m_pRebornStuff) 
// 			&& ( m_pRebornStuff->GetItemTypeID()==dwTmpID1 || m_pRebornStuff->GetItemTypeID()==dwTmpID2 ) )
// 		{
// 			LockItem(FALSE);
// 			m_bIfProgress = FALSE;
// 			m_pBtnRebornOK->SetEnable(TRUE);
// 			m_pProReborn->SetValue(0,TRUE);
// 			m_pRebornStuff = NULL;
// 			RefreshUI_Reborn();
// 		}
// 		else if (P_VALID(m_pRebornItem) 
// 			&& (m_pRebornItem->GetPos()==pGameEvent->dwPos||m_pRebornItem->GetPos()==pGameEvent->dwDst) )
// 		{
// 			LockItem(FALSE);
// 			m_pRebornItem = NULL;
// 			RefreshUI_Reborn();
// 		}
// 	}
// 
// 	return 0;
// }

DWORD HolyManFrame::NetItemRemove(tagNS_ItemRemove* pMsg, DWORD pPrama)
{
	if(pMsg->eConType != EICT_Bag)
		return 0;

	RefreshUI_Reborn();

	return 0;
}

