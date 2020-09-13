#include "StdAfx.h"
#include "BeAttackFrame.h"
#include "StallMgr.h"
#include "Role.h"
#include "Player.h"
#include "RoleMgr.h"
#include "MapMgr.h"

BeAttackFrame::BeAttackFrame(void) : m_pTex(NULL),m_pTexBack(NULL)
,m_bRender(FALSE)
,m_fTime(0.0f)
,m_nAlpha(0xFF)
,m_dwColor(0xFFFFFFFF)
{
}

BeAttackFrame::~BeAttackFrame(void)
{
}

BOOL BeAttackFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr,pFather,dwParam) )
		return FALSE;

	Prepare();
	return TRUE;
}

BOOL BeAttackFrame::Destroy()
{
	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);

	if(P_VALID(m_pTexBack))
		IDraw2D::Inst()->ReleaseTexture(m_pTexBack);

	return GameFrame::Destroy();
}

VOID BeAttackFrame::Update()
{
	if( !m_bRender )
		return;
	m_fTime = Cool3D::Kernel::Inst()->GetDeltaTime();
	INT nStep = 0;
	if (m_bIsEffectBack)
	{
		nStep= INT( 0xFF / ( ( 1.0f / m_fTime ) * 0.2f) );
	}
	else
	{
		nStep= INT( 0xFF / ( ( 1.0f / m_fTime ) * 2.0f ) );
	}
	
	if( m_nAlpha >= 0 && m_nAlpha <= 0xFF )
	{
		m_nAlpha -= nStep;
		if( m_nAlpha < 0 )
		{
			m_nAlpha = 0xFF;
			m_bRender = FALSE;
		}
	}
	m_dwColor = ( m_nAlpha << 24 ) | ( m_dwColor & 0x00FFFFFF );
}

VOID BeAttackFrame::Render3D()
{
	if( P_VALID(m_pTex) && m_bRender )
	{
		IDraw2D::Inst()->BeginDraw(IDraw2D::EBlend_Alpha, true);
		if (m_bIsEffectBack)
		{
			IDraw2D::Inst()->Draw( &m_rectDest, &m_rectSrc, m_pTexBack, m_dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write, 0.1f );
		}
		else{
			IDraw2D::Inst()->Draw( &m_rectDest, &m_rectSrc, m_pTex, m_dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write, 0.1f );
		}
		
		IDraw2D::Inst()->EndDraw();	
	}
}

VOID BeAttackFrame::Prepare()
{
	tstring strFlag = _T("data\\ui\\pk\\L_batk_1.bmp");
	tstring strFlagBack = _T("data\\ui\\pk\\L_batk_2.bmp");
	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);

	if(P_VALID(m_pTexBack))
		IDraw2D::Inst()->ReleaseTexture(m_pTexBack);

	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	m_pTex = IDraw2D::Inst()->NewTexture(strFlag.c_str(), pIFS);
	m_pTexBack = IDraw2D::Inst()->NewTexture(strFlagBack.c_str(), pIFS);

	if (!P_VALID(m_pTex))
		return;

	IDeviceTex *pRealTex = m_pTex->GetDeviceTex();
	if (!P_VALID(pRealTex))
		return;
	m_rectSrc.top = m_rectSrc.left = 0;
	m_rectSrc.bottom = m_rectSrc.top + pRealTex->Height();
	m_rectSrc.right = m_rectSrc.left + pRealTex->Width();
	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	m_rectDest.left = 0;
	m_rectDest.top = 0;
	m_rectDest.right = pDesktop->GetSize().x;
	m_rectDest.bottom = pDesktop->GetSize().y;
}

VOID BeAttackFrame::SetRenderFlag( BOOL bFlag ,bool bIsEffectBack)
{
	m_bIsEffectBack = bIsEffectBack;
	m_bRender = bFlag;
	if(!m_bRender)
	{
		m_nAlpha = 0xFF;
		m_dwColor = 0xFFFFFFFF;
		m_fTime = 0.0f;
	}
}