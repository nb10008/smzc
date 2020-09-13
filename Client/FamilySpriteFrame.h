#pragma once
#include "gameframe.h"
#include "..\WorldDefine\family_define.h"
/*
	家族妖精
*/
class SpriteRegisterFrame;
class SpriteTrainingFrame;
struct tagNS_GetFamilySpriteInfo;

struct GUISpriteAtt
{
	GUIStatic*		pNum;
	GUIStatic*		pBack;
	GUIProgress*	pPgValue;

	void RefreshUI(DWORD dwValue, DWORD dwMaxValue );
	void clear();
};

class FamilySpriteFrame :
	public GameFrame
{
public:
	FamilySpriteFrame(void);
	~FamilySpriteFrame(void);
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID ShowFrame(BOOL bShow = TRUE);
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID RefreshSpriteInfo();
private:
	DWORD OnNS_GetFamilySpriteInfo(tagNS_GetFamilySpriteInfo* pNetCmd,DWORD);
private:
	TSFPTrunk<FamilySpriteFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*										m_pWnd;
	GUIWnd*										m_pWndFather;

	
	GUIButton*									m_pBnRegister;
	GUIButton*									m_pBnTrain;
	GUIStatic*									m_pStLevel;
	GUIProgress*								m_pPgSpriteExp;

	GUISpriteAtt								m_sSpriteAtt[EFSA_Num];

	// 妖精登记
	SpriteRegisterFrame*					m_pSpriteRegisterFrame;
	//	妖精修炼
	SpriteTrainingFrame*					m_pSpriteTrainingFrame;
};
