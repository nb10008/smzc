#pragma once
#include "gameframe.h"

struct tagStall;

class BeAttackFrame :
	public GameFrame
{
public:
	BeAttackFrame(void);
	~BeAttackFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual VOID Render3D();
	VOID SetRenderFlag( BOOL bFlag,bool bIsEffectBack = false );
private:
	VOID Prepare();
private:
	TObjRef<GUISystem>	m_pGUI;
	ResTexture			*m_pTex;
	ResTexture			*m_pTexBack;
	RECT				m_rectSrc;
	RECT				m_rectDest;
	float				m_fTime;
	BOOL				m_bRender;
	INT					m_nAlpha;
	DWORD				m_dwColor;
	bool				m_bIsEffectBack;
};
