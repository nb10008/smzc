#include "StdAfx.h"
#include "PetShowWindow.h"
#include "PetProtoData.h"
#include "CreatureData.h"

const float g_fPetRotateSpeed = 1.2f;
const Vector3 g_vPetLookAt( 0.0f, 95.0f, 0.0f );
const Vector3 g_vPetFrom( 0.0f, 95.0f, -230.0f );

PetShowWindow::PetShowWindow(void) :
    m_pNewCanvas(0),
    m_pPetNode(0),
	m_pBnRotaLeft( NULL ),
	m_pBnRotaRight( NULL ),
    m_pSG(0),
    m_fModelYaw(0.0f)
{
}

PetShowWindow::~PetShowWindow(void)
{
}

BOOL PetShowWindow::Init( GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement )
{
    if( !GUIStatic::Init(pSystem, pFather, pXmlElement) )
        return FALSE;

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
    m_Camera.SetLookAt( g_vPetFrom, g_vPetLookAt, MathConst::AxisY );
    // 设置透视投影
    m_Camera.SetPerspective( (float)m_ptSize.x, (float)m_ptSize.y, _DegToRad(45.0f), 1.0f, 20000.0f);

    // 建立空场景图
    m_pSG = new NullSceneGraph;
    // 绑定摄像机
    m_pSG->BindCamera( &m_Camera );

    return TRUE;
}

VOID PetShowWindow::Destroy()
{
    GUIStatic::Destroy();

	m_pBnRotaLeft = NULL;
	m_pBnRotaRight = NULL;

    m_pSG->DetachDynamicNode( m_pPetNode );
    SAFE_DEL( m_pPetNode );
    SAFE_DEL( m_pSG );
    m_pRender->ReleaseRenderTarget(m_pNewCanvas->dwHandle);
    m_pRender->DestroyImage(m_pNewCanvas);
    m_pNewCanvas = 0;
}
//-----------------------------------------------------------------------------
// 添加子窗口
//-----------------------------------------------------------------------------
VOID PetShowWindow::AddChild(GUIWnd* pChild)
{
	GUIStatic::AddChild(pChild);

	pChild->SetCanvas(m_pNewCanvas);
	m_pNewCanvas->bRefresh = TRUE;

	// 保存旋转按钮指针
	if( pChild->GetClassName() == _T("CGUIButton") )
	{
		if( pChild->GetName() == _T("roletl") )
			m_pBnRotaLeft = (GUIButton*)pChild;
		else if( pChild->GetName() == _T("roletr") )
			m_pBnRotaRight = (GUIButton*)pChild;
	}
}
VOID PetShowWindow::Update()
{
	SetRefresh(TRUE);
	if( !m_bInvisible )
	{
		float fRotateAngle = g_fPetRotateSpeed * Kernel::Inst()->GetDeltaTime();
		if( P_VALID( m_pBnRotaLeft ) && 1 == m_pBnRotaLeft->GetState() )
			RotateYaw( fRotateAngle );
		if( P_VALID( m_pBnRotaRight ) && 1 == m_pBnRotaRight->GetState() )
			RotateYaw( -fRotateAngle );
			
	}

    GUIStatic::Update();
}

VOID PetShowWindow::Render()
{
    if( m_bInvisible )
        return GUIStatic::Render();

    if( !P_VALID( m_pPetNode ) )
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

VOID PetShowWindow::RefreshViewCoord()
{
    GUIStatic::RefreshViewCoord();
    m_pNewCanvas->ptOffset.x = m_ptView.x;
    m_pNewCanvas->ptOffset.y = m_ptView.y;
}

VOID PetShowWindow::SetInvisible( BOOL b )
{

    if( m_bInvisible == b )	// 防止重复创建删除
        return;

    GUIStatic::SetInvisible(b);

    if( b )
    {
        SAFE_DEL( m_pPetNode );                                          // 删除PetNode
        m_pRender->ReleaseRenderTarget(m_pNewCanvas->dwHandle);          // 删除画布,释放render target
        m_pNewCanvas->dwHandle = GT_INVALID;

    }
    else
    {
        tagRect rc;
        rc = m_ptSize;
        m_pNewCanvas->dwHandle = m_pRender->CreateRenderTarget(rc);      // 创建render target
        if( !GT_VALID(m_pNewCanvas->dwHandle) )
        {
            // 创建RenderTarget失败
            GUIStatic::SetInvisible( TRUE );
        }
        if( !P_VALID( m_pPetNode ) )                                   // 创建AvatarNode
            m_pPetNode = new EntityNode();
    }
}

void PetShowWindow::Render3D()
{
    if( m_bInvisible )
        return GUIStatic::Render();

    if( !P_VALID( m_pPetNode ) )
        return;

    //UpdateCamera();

    IDevice* pDev = Device();
    IRenderTexture* pRT = (IRenderTexture*)m_pNewCanvas->dwHandle;
    float fTimeDelta = Kernel::Inst()->GetDeltaTime();

    m_pNewCanvas->bRefresh = TRUE;
    m_bRefresh = TRUE;

    pRT->Begin();
    pDev->Clear( true, true, false, 0x00000000, 1.0f, 0 );
    StateBlock block;
    block.SaveState(ERS_ColorWriteEnable);
    pDev->SetRenderState(ERS_ColorWriteEnable, D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED);

    // 关闭后期处理
    const bool bEnablePostProcess = Kernel::Inst()->GetRenderSys()->IsEnablePostProcess();
    bool bEnableDos = Kernel::Inst()->GetRenderSys()->IsEnableDof();
    Kernel::Inst()->GetRenderSys()->EnablePostProcess( false );
    Kernel::Inst()->GetRenderSys()->EnableDof( false );
    m_pSG->Render( fTimeDelta );
    Kernel::Inst()->GetRenderSys()->EnablePostProcess( bEnablePostProcess );
    Kernel::Inst()->GetRenderSys()->EnableDof( bEnableDos );

    block.Apply();
    pRT->End();
}

void PetShowWindow::RotateYaw( FLOAT fAngle )
{
    if( !P_VALID( m_pPetNode ) )
        return;

    m_fModelYaw += fAngle;

    Transform tran;
    tran.LoadIdentity();
    tran.Rotate( m_fModelYaw, 0.0f, 0.0f );
    m_pPetNode->SetWorldMat( tran.GetMatrix() );
}

void PetShowWindow::SetPetShow( DWORD dwPetTypeID, EPetModelType eModelType /*= EPMT_Follow*/ )
{
    m_dwCurrPetTypeId   = dwPetTypeID;

    // 卸载节点
    m_pSG->DetachDynamicNode(m_pPetNode);
    SAFE_DEL(m_pPetNode);

    // 获得宠物原型
    const tagPetProto* petProto = PetProtoData::Inst()->FindPetProto(dwPetTypeID);
    if (!P_VALID(petProto))
        return;

	if ( petProto->type3 == 6 )  
	{	
		eModelType = EPMT_Follow;
	} 
	//乘骑
	else
	{	
		eModelType = EPMT_Mount;
	}

    // 重建节点
    m_pPetNode = LoadEntity(petProto->strModelPath[eModelType].c_str());
    if (EAT_Skeleton == petProto->eAniType[eModelType])
    {
        for (list<tstring>::const_iterator it = petProto->lstSkinPath[eModelType].begin();
            it != petProto->lstSkinPath[eModelType].end(); ++it)
        {
            LoadSkin(it->c_str(), m_pPetNode);
        }
        m_pPetNode->UpdateBoundingBox();
    }

    // 挂载节点
    m_pSG->AttachDynamicNode(m_pPetNode);

 //   // 更新摄像机
	//if( eModelType == EPMT_Follow )
	//	m_Camera.SetLookAt( g_vPetFrom, g_vPetLookAt, MathConst::AxisY );
	//else
	this->UpdateCamera();
	
}
    
void PetShowWindow::UpdateCamera()
{
    if( !P_VALID(m_pPetNode ) )
        return;
	
//<! 注释原因：改为读表获取摄像机位置，有策划控制。
	//AABBox boxPet;
	//// 获得宠物原型
	//const tagPetProto* petProto = PetProtoData::Inst()->FindPetProto(m_dwCurrPetTypeId);
	//float fScale = 1.0f;
	//if (P_VALID(petProto))
	//{
	//	 
	//	fScale = petProto->scalePercent;
	//	boxPet.Merge( petProto->boundingBox );
	//}
	//
 //   boxPet.Transform( m_pPetNode->GetWorldMat() );

	//// 计算m_pPetNode的包围盒
	//AABBox boxPettest;
	//const int nNumChild = m_pPetNode->GetNumChildren();
	//for( int i = 0; i < nNumChild; i++ )
	//{
	//	const SceneNode *pChild = m_pPetNode->GetChild( i );
	//	if(pChild->IS_STATIC_CLASS(SGSkinNode))
	//	{
	//		const ResSkin *pRes=((const SGSkinNode*)pChild)->GetResSkin();
	//		boxPettest.Merge(*(pRes->GetAABBox()));
	//	}
	//}
	//
 //   Vector3 vLookAt = boxPet.GetCenter();
 //   Vector3 vFrom = vLookAt;
 //   //vFrom.z -= 220.0f;
 //   vFrom.z -= boxPet.max.z - boxPet.min.z + boxPet.max.y - boxPet.min.y ;
	////vLookAt.y += 35.0f;
 //   // 查看模型是否是N25
 //   const tagPetProto* pProto = PetProtoData::Inst()->FindPetProto(m_dwCurrPetTypeId);
 //   if (P_VALID(pProto) && pProto->strModelName[EPMT_Follow] == _T("N25"))
 //   {
 //       vFrom.z -= 90.0f;
 //   }
 //   m_Camera.SetLookAt( vFrom, vLookAt, MathConst::AxisY );

	const tagPetProto* petProto = PetProtoData::Inst()->FindPetProto(m_dwCurrPetTypeId);
	Vector3 vec3LookFrom( 0, 0, -1 );
	Vector3 vec3LookAt( 0, 0, 0 );
	if (P_VALID(petProto))
	{
		EPetModelType eModelType = EPMT_Follow;
		if ( petProto->type3 == 6 )		//宠物
			eModelType = EPMT_Follow;
		else							//坐骑
			eModelType = EPMT_Mount;
		tagCreatureMdlData* m_pMdlData = (tagCreatureMdlData*)CreatureData::Inst()->FindMdlData( petProto->strModelName[eModelType].c_str() );
		if (P_VALID(m_pMdlData))
		{
			vec3LookFrom.x = m_pMdlData->vec3Camera.x;
			vec3LookFrom.y = m_pMdlData->vec3Camera.y;
			vec3LookFrom.z = m_pMdlData->vec3Camera.z;

			vec3LookAt.x = m_pMdlData->vec3Camera.x;
			vec3LookAt.y = m_pMdlData->vec3Camera.y;
			vec3LookAt.z = 0;
		}
	}
	m_Camera.SetLookAt( vec3LookFrom, vec3LookAt, MathConst::AxisY );

}

EntityNode* PetShowWindow::LoadEntity( LPCTSTR szFileName )
{
    EntityNode* pEntityNode = new EntityNode;

    pEntityNode->LoadRes( szFileName );

    pEntityNode->Hide( FALSE );

    return pEntityNode;
}

void PetShowWindow::LoadSkin( LPCTSTR szFileName, EntityNode* pEntityNode )
{
    ASSERT(pEntityNode!=NULL);
    if (pEntityNode != NULL)
    {
        SGSkinNode *pNewSkin = new SGSkinNode;
        pNewSkin->LoadRes( szFileName );
        pEntityNode->AttachChild( pNewSkin );
    }
}

VOID PetShowWindow::SetModelYaw(float fNum)
{
	m_fModelYaw=fNum;
}