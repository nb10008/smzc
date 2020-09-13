#pragma once
#include "gameframe.h"
#include "..\WorldDefine\clergy_define.h"

struct tagNS_CampaignResult;

class CampaignResultFrame :
	public GameFrame
{
public:
	CampaignResultFrame(void);
	~CampaignResultFrame(void);
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	void AcquireInfo(ECampaignGodType eType);
	void SetInvisiable(bool b){m_pWnd->SetInvisible(b);}
private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	void RefreshList(ECampaignGodType eType,int num);
	DWORD OnNS_CampaignResult(tagNS_CampaignResult* pNetCmd,DWORD);
	void SortGodCampaignInfo(tagGodCampaignInfo& info);
private:
	TSFPTrunk<CampaignResultFrame>		m_Trunk;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*			m_pWnd;

	GUIPushButton*	m_pPbnWar;
	GUIPushButton*	m_pPbnSun;
	GUIPushButton*	m_pPbnMana;
	GUIPushButton*	m_pPbnPeace;

	GUIListBox*		m_pListInfo;

	GUIButton*			m_pBnClose;
	GUIButton*			m_pBnX;
	tagCampaignResultInfo m_ResultInfo;

};
