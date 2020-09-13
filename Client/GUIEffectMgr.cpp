#include "StdAfx.h"
#include "GUIEffectMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "EffectMgr.h"
#include "LoadingFrame.h"

GUIEffectMgr::GUIEffectMgr(void)
{
	
}

GUIEffectMgr::~GUIEffectMgr(void)
{
}


GUIEffectMgr g_guiEffectMgr;
GUIEffectMgr* GUIEffectMgr::Inst()
{
	return &g_guiEffectMgr;
}

VOID GUIEffectMgr::Init()
{
	m_pDeskTop = TObjRef<GUISystem>()->GetDesktop();
	m_pRender = TObjRef<GUISystem>()->GetRender();

	m_pCanvasImg = new tagGUIImage;
	m_pCanvasImg->ptSize.x = m_pDeskTop->GetSize().x ;
	m_pCanvasImg->ptSize.y = m_pDeskTop->GetSize().y ;
	m_pCanvasImg->rc = tagRect( 0, 0, m_pCanvasImg->ptSize.x, m_pCanvasImg->ptSize.y );

	IRenderTexture* pRenderTexture = Device()->NewRenderTexture();
	bool bResult = pRenderTexture->Create( m_pCanvasImg->ptSize.x, m_pCanvasImg->ptSize.y, EPF_A8R8G8B8, true, false, 1, false );
	if( !bResult )	// 创建RenderTarget失败
	{
		SAFE_DEL(pRenderTexture);
		SAFE_DEL(m_pCanvasImg);
		return;
	}
	m_pCanvasImg->dwHandle = (DWORD)pRenderTexture;
	m_pCanvasImg->bRenderTarget = TRUE;

	m_pCamera = new CameraBase;
	m_pSG = new NullSceneGraph;

	// 设置初始化摄像机位置
	m_pCamera->SetLookAt( Vector3( 0.0f, 0.0f, -190.0f ), Vector3( 0.0f, 0.0f, 0.0f ), MathConst::AxisY );
	
	// 设置透视投影
	m_pCamera->SetPerspective( (float)m_pCanvasImg->ptSize.x, (float)m_pCanvasImg->ptSize.y, _DegToRad(45.0f), 1.0f, 20000.0f );

	m_pSG->BindCamera( m_pCamera );

	m_CameraData.LoadFromFile();
}

VOID GUIEffectMgr::Destroy()
{
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		DeleteEffect( itr->second );
	}
	m_mapEffect.clear();

	SAFE_DEL( m_pSG );
	SAFE_DEL( m_pCamera );
	if( P_VALID(m_pCanvasImg) )
	{
		m_pRender->ReleaseRenderTarget( m_pCanvasImg->dwHandle);
		m_pRender->DestroyImage(m_pCanvasImg);
		m_pCanvasImg = 0;
	}
}

VOID GUIEffectMgr::Update()
{
	const float fTimeDelta = Kernel::Inst()->GetDeltaTime();

	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); )
	{
		tagBaseEffectData* pData = itr->second;
		if( !P_VALID(pData->pSGNode) )
			continue;

		if( P_VALID(pData->pWnd) && pData->pWnd->IsInvisible() )
			pData->pSGNode->Close();

		// 更新特效
		pData->pSGNode->UserUpdate( fTimeDelta, m_pSG );

		// 删除已经关闭的特效
		if( pData->pSGNode->IsClosed() )
		{
			DeleteEffect( pData );
			itr = m_mapEffect.erase( itr );
			continue;
		}
		itr++;
	}
}

void GUIEffectMgr::Render3D()
{
	if( m_mapEffect.empty() || m_bLoading )
		return;

	IDevice* pDev = Device();
	IRenderTexture* pRT = (IRenderTexture*)m_pCanvasImg->dwHandle;

	float fTimeDelta = Kernel::Inst()->GetDeltaTime();
	m_pCanvasImg->bRefresh = TRUE;

	bool bNeedClearPixBuffer = true;
 	EffecMap::iterator iter = m_mapEffect.begin();
 	for( ; iter != m_mapEffect.end(); ++iter )
 	{
		tagBaseEffectData* pEffectData = iter->second; 
 		if( !P_VALID(pEffectData->pSGNode) )
 			continue;
		
		m_pSG->AttachDynamicNode( pEffectData->pSGNode );

		// 与窗口无关的是全屏特效
		if( !P_VALID(pEffectData->pWnd) )
		{
			if( P_VALID(pRT) )
			{
				pRT->Begin();
				if(bNeedClearPixBuffer)
				pDev->Clear( true, true, false, 0x00000000, 1.0f, 0 );
				bNeedClearPixBuffer = false;
				StateBlock block;
				block.SaveState( ERS_ColorWriteEnable );
				block.SaveState(ERS_SrcBlend);
				block.SaveState(ERS_DstBlend);
				
				pDev->SetAlphaBlend( EBlend_One, EBlend_Zero );
				pDev->SetRenderState( ERS_ColorWriteEnable, D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_ALPHA );

				// 关闭后期处理
				const bool bEnablePostProcess = Kernel::Inst()->GetRenderSys()->IsEnablePostProcess();
                bool bEnableDos = Kernel::Inst()->GetRenderSys()->IsEnableDof();
				Kernel::Inst()->GetRenderSys()->EnablePostProcess( false );
                Kernel::Inst()->GetRenderSys()->EnableDof( false );
				m_pSG->Render( fTimeDelta );
				Kernel::Inst()->GetRenderSys()->EnablePostProcess( bEnablePostProcess );
                Kernel::Inst()->GetRenderSys()->EnableDof( bEnableDos );

// 				IRenderTexture* pPPP = (IRenderTexture*)pRT;
// 				pPPP->WriteToFile(_T("data\\ui\\help\\q_bantou111.tga"));

				block.Apply();
				pRT->End();
			}
		}
		// 与窗口相关的是界面特效
		else
		{
			IRenderTexture* pRT1 = (IRenderTexture*)pEffectData->dwHandle;
			if( P_VALID(pRT1) )
			{
				pRT1->Begin();
				pDev->Clear( true, true, false, 0x00000000, 1.0f, 0 );
				StateBlock block;
				block.SaveState( ERS_ColorWriteEnable );
				block.SaveState(ERS_SrcBlend);
				block.SaveState(ERS_DstBlend);
				pDev->SetAlphaBlend( EBlend_One, EBlend_Zero );
				block.SaveState(ERS_SrcBlend);
				block.SaveState(ERS_DstBlend);
				pDev->SetAlphaBlend( EBlend_One, EBlend_Zero );
				pDev->SetRenderState( ERS_ColorWriteEnable, D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_ALPHA );

				// 关闭后期处理
				const bool bEnablePostProcess = Kernel::Inst()->GetRenderSys()->IsEnablePostProcess();
                bool bEnableDos = Kernel::Inst()->GetRenderSys()->IsEnableDof();
				Kernel::Inst()->GetRenderSys()->EnablePostProcess( false );
                Kernel::Inst()->GetRenderSys()->EnableDof( false );
				m_pSG->Render( fTimeDelta );
				Kernel::Inst()->GetRenderSys()->EnablePostProcess( bEnablePostProcess );
                Kernel::Inst()->GetRenderSys()->EnableDof( bEnableDos );

				block.Apply();
				pRT1->End();
			}
		}
 		
		m_pSG->DetachDynamicNode( pEffectData->pSGNode );
 	}
}

void GUIEffectMgr::Render()
{
	LoadingFrame* pFrame = (LoadingFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Loading"));
	if (P_VALID(pFrame))
		return;

	if( m_mapEffect.empty() || m_bLoading )
		return;

	IDevice* pDev = Device();

//   block.SaveState(ERS_AlphaBlendEnable);	
//   block.SaveState(ERS_AlphaTestEnable);
//   block.SaveState(ERS_AlphaTestFunc);
//   block.SaveState(ERS_AlphaRef);

// 	pDev->EnableAlphaBlend(false);
// 	pDev->EnableAlphaTest(true);
// 	pDev->SetAlphaTestFunc(ECMP_Greater);
// 	pDev->SetAlphaRef(0);
	
 	EffecMap::iterator iter = m_mapEffect.begin();
	for( ; iter != m_mapEffect.end(); ++iter )
	{
		tagBaseEffectData* pEffectData = iter->second;
	 	if( !P_VALID(pEffectData) )
			continue;

		StateBlock block;
		block.SaveState(ERS_SrcBlend);
		block.SaveState(ERS_DstBlend);
		if ( EEBM_One_One ==  pEffectData->eBlendMode)
			pDev->SetAlphaBlend( EBlend_One, EBlend_One );
		else if (EEBM_One_One ==  pEffectData->eBlendMode)
			pDev->SetAlphaBlend( EBlend_SrcAlpha, EBlend_InvSrcAlpha );
		
		if( P_VALID(pEffectData->pWnd) )
		{
			if( !WndIsHide(pEffectData->pWnd))
			{
				const tagPoint& ptView = iter->second->pWnd->GetView();
				const tagPoint& ptSize = iter->second->pWnd->GetSize();
				tagRect destRect( ptView.x, ptView.y, ptView.x + ptSize.x, ptView.y + ptSize.y );

				LONG dx = ((IRenderTexture*)pEffectData->dwHandle)->Width() / 2 - ptSize.x / 2;
				LONG dy = ((IRenderTexture*)pEffectData->dwHandle)->Height() / 2 - ptSize.y / 2;
				tagRect rectSrc;
				rectSrc.SetLTWH( dx, dy, ptSize.x, ptSize.y );

				//优先用输入位置
				tagRect rectTmp(pEffectData->destRect);
				if (	rectTmp.left - rectTmp.right !=0
					&&	rectTmp.top - rectTmp.bottom !=0 )
					IDraw2D::Inst()->Draw((const RECT*)&rectTmp, (const RECT*)&rectSrc, (IRenderTexture*)pEffectData->dwHandle, 0xFFFFFFFF, ETFilter_POINT );
				else
					IDraw2D::Inst()->Draw((const RECT*)&destRect, (const RECT*)&rectSrc, (IRenderTexture*)pEffectData->dwHandle, 0xFFFFFFFF, ETFilter_POINT );
			}
		}
		else
		{
			LONG dx = m_pCanvasImg->ptSize.x / 2 - (pEffectData->destRect.right - pEffectData->destRect.left) / 2;
			LONG dy = m_pCanvasImg->ptSize.y / 2 - (pEffectData->destRect.bottom - pEffectData->destRect.top) / 2;
			tagRect rectSrc;
			rectSrc.SetLTWH( dx, dy, pEffectData->destRect.right - pEffectData->destRect.left, pEffectData->destRect.bottom - pEffectData->destRect.top );
			IDraw2D::Inst()->Draw((const RECT*)&pEffectData->destRect, (const RECT*)&rectSrc, (IRenderTexture*)m_pCanvasImg->dwHandle, 0xFFFFFFFF, ETFilter_POINT);
		}
		block.Apply();
	}

	
}

DWORD GUIEffectMgr::PlayGuiEffect( const TCHAR* szEffectName, GUIWnd* pWnd, tagRect& rect, const Vector3& vPos, const Vector3& vScale, const TCHAR* szEffectFolder ,bool bReSetCamera /*= false*/)
{
	// 重新设置摄像机位置，防止摄像机偏移
	if(bReSetCamera)
	m_pCamera->SetLookAt( Vector3( 0.0f, 0.0f, -190.0f ), Vector3( 0.0f, 0.0f, 0.0f ), MathConst::AxisY );

	if( !P_VALID(pWnd) )
		return GT_INVALID;

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szEffectFolder );
	if( NULL == pNode )
		return GT_INVALID;

	m_pCamera->SetLookAt( Vector3( 0.0f, 0.0f, -600.0f ), Vector3( 0.0f, 0.0f, 0.0f ), MathConst::AxisY );

	tagBaseEffectData* pData = new tagBaseEffectData(EET_Common);
	pData->pWnd = pWnd;
	pData->pSGNode = pNode;
	pData->destRect.Set( rect.left, rect.top, rect.right, rect.bottom );
	tagPoint ptSize = pWnd->GetSize();
	if ( (rect.right-rect.left!=0) && (rect.bottom-rect.top!=0) ) //有输入位置，则用输入的，否则用空控件的
		ptSize.Set( abs(rect.right-rect.left), abs(rect.bottom-rect.top) );


	const tagPoint& ptTempSize = pWnd->GetSize();
	IRenderTexture* pRenderTexture = Device()->NewRenderTexture();
	bool bResult = pRenderTexture->Create( ptTempSize.x, ptTempSize.y, EPF_A8R8G8B8, true, false, 1, false );
	if( !bResult )
	{
		m_pRender->ReleaseRenderTarget( (DWORD)pRenderTexture );
		SAFE_DEL( pNode );
		SAFE_DEL( pData ); 
		return GT_INVALID;
	}

	pData->dwHandle = (DWORD)pRenderTexture;

	Transform trans;
	trans.Scale( vScale.x, vScale.y, vScale.z );
	trans.Translate( vPos );
	pNode->SetWorldMat( trans.GetMatrix() );
	pNode->Play();
	
	const DWORD dwID = AddEffect( pData );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		m_pRender->ReleaseRenderTarget( pData->dwHandle );
		SAFE_DEL( pData );
		return GT_INVALID;
	}
	// sound
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		tstring ss = szEffectName;
		tstring tt = _T("_sound");
		ss = ss + tt;
		EffectMgr::Inst()->PlayRoleEffect( plp->GetID(),ss.c_str(),NULL,EffectMgr::EEST_Y,_T("ui"));
	}
	return dwID;
}

DWORD GUIEffectMgr::PlayGuiEffect( const TCHAR* szEffectName, GUIWnd* pWnd,EEffectType eType/*= EET_Common*/,const TCHAR* szEffectFolder /*= _T("ui")*/)
{
	const tagGUICameraData* pCamData = m_CameraData.GetCameraData(TObjRef<Util>()->Crc32(szEffectName));
	if (!P_VALID(pCamData))
		return GT_INVALID;

	m_pCamera->SetLookAt( pCamData->V3CameraFrom,pCamData->V3CameraLookAt, MathConst::AxisY );

	if( !P_VALID(pWnd) )
		return GT_INVALID;

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szEffectFolder );
	if( NULL == pNode )
		return GT_INVALID;

	tagBaseEffectData* pData = new tagBaseEffectData(eType);
	pData->pWnd = pWnd;
	pData->pSGNode = pNode;
	pData->eBlendMode = static_cast<EEffectBlendMode>(pCamData->eMode);
	pData->strEffectName = szEffectName;

	const tagPoint& ptSize = pWnd->GetSize();
	IRenderTexture* pRenderTexture = Device()->NewRenderTexture();
	bool bResult = pRenderTexture->Create( ptSize.x, ptSize.y, EPF_A8R8G8B8, true, false, 1, false );
	if( !bResult )
	{
		m_pRender->ReleaseRenderTarget( (DWORD)pRenderTexture );
		SAFE_DEL( pNode );
		SAFE_DEL( pData ); 
		return GT_INVALID;
	}

	pData->dwHandle = (DWORD)pRenderTexture;

	Transform trans;
	trans.Scale( pCamData->V3EffectScale.x, pCamData->V3EffectScale.y, pCamData->V3EffectScale.z );
	trans.Translate( pCamData->V3EffectPos );
	pNode->SetWorldMat( trans.GetMatrix() );
	pNode->Play();

	const DWORD dwID = AddEffect( pData );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		m_pRender->ReleaseRenderTarget( pData->dwHandle );
		SAFE_DEL( pData );
		return GT_INVALID;
	}

	// sound
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		tstring ss = szEffectName;
		tstring tt = _T("_sound");
		ss = ss + tt;
		EffectMgr::Inst()->PlayUiSoundEffect(ss.c_str(),eType);
	}
	

	return dwID;
}


DWORD GUIEffectMgr::PlayScreenEffect( const TCHAR* szEffectName, const tagPoint& ptView, const tagPoint& ptSize, 
									  const Vector3& vPos, const Vector3& vScale, const TCHAR* szEffectFolder,bool bReSetCamera /*= false */)
{
	// 重新设置摄像机位置，防止摄像机偏移
	if(bReSetCamera)
		m_pCamera->SetLookAt( Vector3( 0.0f, 0.0f, -190.0f ), Vector3( 0.0f, 0.0f, 0.0f ), MathConst::AxisY );

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szEffectFolder );
	if( NULL == pNode )
		return GT_INVALID;

	m_pCamera->SetLookAt( Vector3( 0.0f, 0.0f, -190.0f ), Vector3( 0.0f, 0.0f, 0.0f ), MathConst::AxisY );

	tagBaseEffectData* pData = new tagBaseEffectData(EET_Common);
	pData->pSGNode = pNode;
	pData->destRect.Set( ptView.x, ptView.y, ptView.x + ptSize.x, ptView.y + ptSize.y );

	Transform trans;
	trans.Scale( vScale.x, vScale.y, vScale.z );
	trans.Translate( vPos );
	pNode->SetWorldMat( trans.GetMatrix() );
	pNode->Play();

	const DWORD dwID = AddEffect( pData );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
		return GT_INVALID;
	}
	// sound
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		tstring ss = szEffectName;
		tstring tt = _T("_sound");
		ss = ss + tt;
		EffectMgr::Inst()->PlayRoleEffect( plp->GetID(),ss.c_str(),NULL,EffectMgr::EEST_Y,_T("ui"));
	}
	return dwID;
}

DWORD GUIEffectMgr::PlayScreenEffect( const TCHAR* szEffectName,const tagPoint& ptView, const tagPoint& ptSize,EEffectType eType/*= EET_Common*/,const TCHAR* szEffectFolder /*= _T("ui")*/)
{
	
	const tagGUICameraData* pCamData = m_CameraData.GetCameraData(TObjRef<Util>()->Crc32(szEffectName));
	if (!P_VALID(pCamData))
		return GT_INVALID;

	if (P_VALID(m_pCamera))
		m_pCamera->SetLookAt( pCamData->V3CameraFrom,pCamData->V3CameraLookAt, MathConst::AxisY );

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szEffectFolder );
	if( NULL == pNode )
		return GT_INVALID;

	tagBaseEffectData* pData = new tagBaseEffectData(eType);
	pData->pSGNode = pNode;
	pData->destRect.Set( ptView.x, ptView.y, ptView.x + ptSize.x, ptView.y + ptSize.y );
	pData->eBlendMode = static_cast<EEffectBlendMode>(pCamData->eMode);
	pData->strEffectName = szEffectName;

	Transform trans;
	trans.Scale( pCamData->V3EffectScale.x, pCamData->V3EffectScale.y, pCamData->V3EffectScale.z  );
	trans.Translate( pCamData->V3EffectPos  );
	pNode->SetWorldMat( trans.GetMatrix() );
	pNode->Play();

	const DWORD dwID = AddEffect( pData );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
		return GT_INVALID;
	}

	// sound
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		tstring ss = szEffectName;
		tstring tt = _T("_sound");
		ss = ss + tt;
		EffectMgr::Inst()->PlayUiSoundEffect(ss.c_str(),eType);
	}

	tstring sss = szEffectName;
	tstring ttt = _T("_a");
	sss = sss + ttt;
	PlayScreenEffect_a(sss.c_str(),ptView,ptSize,eType,szEffectFolder);

	return dwID;
}

DWORD GUIEffectMgr::PlayScreenEffect_a( const TCHAR* szEffectName,const tagPoint& ptView, const tagPoint& ptSize,EEffectType eType/*= EET_Common*/,const TCHAR* szEffectFolder /*= _T("ui")*/)
{

	const tagGUICameraData* pCamData = m_CameraData.GetCameraData(TObjRef<Util>()->Crc32(szEffectName));
	if (!P_VALID(pCamData))
		return GT_INVALID;

	m_pCamera->SetLookAt( pCamData->V3CameraFrom,pCamData->V3CameraLookAt, MathConst::AxisY );

	SGAttachableEffectNode* pNode = LoadSFXFile( szEffectName, szEffectFolder );
	if( NULL == pNode )
		return GT_INVALID;

	tagBaseEffectData* pData = new tagBaseEffectData(eType);
	pData->pSGNode = pNode;
	pData->destRect.Set( ptView.x, ptView.y, ptView.x + ptSize.x, ptView.y + ptSize.y );
	pData->eBlendMode = static_cast<EEffectBlendMode>(pCamData->eMode);

	Transform trans;
	trans.Scale( pCamData->V3EffectScale.x, pCamData->V3EffectScale.y, pCamData->V3EffectScale.z  );
	trans.Translate( pCamData->V3EffectPos  );
	pNode->SetWorldMat( trans.GetMatrix() );
	pNode->Play();

	const DWORD dwID = AddEffect( pData );
	if( GT_INVALID == dwID )
	{
		SAFE_DEL( pData->pSGNode );
		SAFE_DEL( pData );
		return GT_INVALID;
	}

	return dwID;
}

VOID GUIEffectMgr::StopEffect( const DWORD dwEffectID )
{
	EffecMap::iterator itr = m_mapEffect.find( dwEffectID );
	if( itr != m_mapEffect.end() )
	{
		tagBaseEffectData* pData = itr->second;
		pData->pSGNode->Close();
	}
}

VOID GUIEffectMgr::StopGuiEffect( GUIWnd* pWnd , const TCHAR* szEffectName )
{
	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		tagBaseEffectData* pEffectData = itr->second;
		if ( !P_VALID(pEffectData)  )	
			continue;

		GUIWnd *pEffectWnd = pEffectData->pWnd;
		if ( !P_VALID(pEffectWnd) || !P_VALID(pWnd) )
			continue;

		tstring strEffectWndFullName = pEffectWnd->GetFullName();
		tstring strWndFullName = pWnd->GetFullName();
		if ( strWndFullName.empty() || strEffectWndFullName.empty() )
			continue;

		int nRet1 = 0; 		
		if ( szEffectName != NULL)
		{
			tstring strName = pEffectData->strEffectName;
			tstring szName2 = szEffectName;
			if ( strName.empty() || szName2.empty() )
				continue;
			nRet1 = strName.compare(szName2);				
		}

		int nRet2 = strWndFullName.compare(strEffectWndFullName);	
		if ( 0 == nRet1 && 0 == nRet2 )  //表明该控件已加载过该特效
		{
			SGAttachableEffectNode* pSG =  pEffectData->pSGNode;
			if (P_VALID(pSG))
				pSG->Close();
			return;
		}
	}
}


DWORD GUIEffectMgr::AddEffect( tagBaseEffectData* pData )
{
	if( !P_VALID(m_pSG) )
		return GT_INVALID;

	for( EffecMap::iterator itr = m_mapEffect.begin(); itr != m_mapEffect.end(); itr++ )
	{
		tagBaseEffectData* pEffectData = itr->second;
		if ( !P_VALID(pEffectData) || !P_VALID(pData) )	
			continue;

		GUIWnd *pEffectWnd = pEffectData->pWnd;
		GUIWnd *pWnd = pData->pWnd;
		if ( !P_VALID(pEffectWnd) || !P_VALID(pWnd) )
			continue;

		tstring strEffectWndFullName = pEffectWnd->GetFullName();
		tstring strWndFullName = pWnd->GetFullName();
		if ( strWndFullName.empty() || strEffectWndFullName.empty() )
			continue;

		tstring strEffectName = pEffectData->strEffectName;
		tstring strName = pData->strEffectName;
		if ( strEffectName.empty() || strName.empty() )
			continue;

		int nRet1 = strWndFullName.compare(strEffectWndFullName);	 
		int nRet2 = strEffectName.compare(strName);							
		if ( 0 == nRet1 && 0 == nRet2 )  //表明该控件已加载过该特效
		{
			SGAttachableEffectNode* pSG =  pEffectData->pSGNode;
			if ( P_VALID(pSG) && pSG->IsClosed() )
				pSG->Play();
			return GT_INVALID;
		}
	}

	m_pSG->AttachDynamicNode( pData->pSGNode );

	m_mapEffect[m_dwCurEffectID] = pData;
	return m_dwCurEffectID++;
}

VOID GUIEffectMgr::RemoveEffect( const DWORD dwEffectID )
{
	EffecMap::iterator itr = m_mapEffect.find( dwEffectID );
	if( itr != m_mapEffect.end() )
	{
		DeleteEffect( itr->second );
		m_mapEffect.erase( itr );
	}
}

VOID GUIEffectMgr::DeleteEffect( tagBaseEffectData* pData )
{
	if( P_VALID( pData->pSGNode ) )
	{
		SceneNode* pFather = (SceneNode*)pData->pSGNode->GetParent();
		if( NULL != pFather  )
			pFather->DetachChild( pData->pSGNode );
		else
			m_pSG->DetachDynamicNode( pData->pSGNode );
		
		m_pRender->ReleaseRenderTarget( pData->dwHandle );
		SAFE_DEL( pData->pSGNode );
	}
	pData->pWnd = NULL;
	SAFE_DEL( pData );
}

SGAttachableEffectNode* GUIEffectMgr::LoadSFXFile( const TCHAR* szEffectName, const TCHAR* szFolderName/* = _T("ui")*/ )
{
	if( !P_VALID(szEffectName) || _T('\0') == szEffectName[0] )
		return NULL;

	// 如果传了文件夹名称就构造路径，否则直接使用szEffectName作为路径
	tstring strEffectPath;
	if( NULL != szFolderName )
	{
		strEffectPath = _T("data\\system\\effect\\");
		strEffectPath += szFolderName;
		strEffectPath += _T("\\");
		strEffectPath += szEffectName;
		strEffectPath += _T(".sfx");
	}
	else
	{
		strEffectPath = szEffectName;
	}

	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	DWORD dwFileSize = pIFS->GetSize( strEffectPath.c_str() );
	if( 0 == dwFileSize || GT_INVALID == dwFileSize )
	{
		TRACE( _T("SFX file have not found.file=%s\r\n"), strEffectPath.c_str() );
		return NULL;
	}

	SGAttachableEffectNode *pEffect=new SGAttachableEffectNode;
	pEffect->SetUserWorldMat( true );
	pEffect->LoadRes( strEffectPath.c_str() );
	pEffect->SetUserUpdate( true );
	pEffect->SetUserPlay( true );

	return pEffect;
}

bool GUIEffectMgr::WndIsHide(GUIWnd* pWnd)
{
	GUIWnd* pTempWnd = pWnd;
	while (P_VALID(pTempWnd) && (_tcscmp(pTempWnd->GetName().c_str(),_T("desktop"))!= 0))
	{
		if (pTempWnd->IsInvisible())
			return true;

		pTempWnd = pTempWnd->GetFather();
	}
	return false;
}

VOID GUIEffectMgr::SetUICamera( const Vector3& vPos, const Vector3& vLookAt)
{
	m_pCamera->SetLookAt( vPos, vLookAt, MathConst::AxisY );
}

VOID GUIEffectMgr::CloseAllEffect()
{
	EffecMap::iterator iter = m_mapEffect.begin();
	for( ; iter != m_mapEffect.end(); ++iter )
	{
		tagBaseEffectData* pData = iter->second;
		pData->pSGNode->Close();
	}
}

VOID GUIEffectMgr::StopEffectByType( EEffectType eType )
{
	EffecMap::iterator iter = m_mapEffect.begin();
	for( ; iter != m_mapEffect.end(); ++iter )
	{
		if(eType != iter->second->eType)
			continue;

		tagBaseEffectData* pData = iter->second;
		if (P_VALID(pData))
		{
			pData->pSGNode->Close();
		}
	}

	EffectMgr::Inst()->StopUiSoundEffect(eType);
}

VOID GUIEffectMgr::RemoveOneEffectByType(EEffectType eType)
{
	EffecMap::iterator iter = m_mapEffect.begin();
	for( ; iter != m_mapEffect.end(); ++iter )
	{
		if(eType != iter->second->eType)
			continue;

		DeleteEffect( iter->second );
		m_mapEffect.erase( iter );
		return;
	}

	EffectMgr::Inst()->StopUiSoundEffect(eType);
}