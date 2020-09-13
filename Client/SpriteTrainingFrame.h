#pragma once
#include "gameframe.h"
#include "../WorldDefine/family_define.h"
/*
	Ñý¾«ÐÞÁ¶
*/

class ItemButton;
class Item;
struct tagNS_FamilySpriteTrain;
struct tagNS_GetFamilySpriteTrain;
struct tagItemPosChangeExEvent;


enum ESpriteShowState
{
	ESSS_Base = 1,
	ESSS_Attack,
	ESSS_State,
};

class SpriteTrainingFrame :
	public GameFrame
{
public:
	SpriteTrainingFrame(void);
	~SpriteTrainingFrame(void);
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID ShowFrame(BOOL bShow = TRUE);
	DWORD EventHandler(tagGUIEvent* pEvent);
	BOOL IsInvisable(){return m_pWnd->IsInvisible(); }

	VOID RefreshUI();
	VOID SubmitTrain(DWORD dwTrainID);

	VOID CommitTrain();
private:
	DWORD OnNS_FamilySpriteTrain(tagNS_FamilySpriteTrain* pNetCmd,DWORD);
	DWORD OnNS_GetFamilySpriteTrain(tagNS_GetFamilySpriteTrain* pNetCmd,DWORD);
	DWORD OnMsgBox_SpriteTrain_Check(tagMsgBoxEvent* pGameEvent);
	DWORD OnClose_Family_AllUi(tagGameEvent* pGameEvent);
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
private:
	TSFPTrunk<SpriteTrainingFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*										m_pWnd;
	GUIButton*									m_pBnSubmit;
	GUIButton*									m_pBnClose;
	GUIButton*									m_pBnCloseX;

	GUIPushButton*							m_pPbBase;
	GUIPushButton*							m_pPbAttack;
	GUIPushButton*							m_pPbState;

	GUIStatic*									m_pStTodayNum;
	GUIPushButton*							m_pPbHideDone;
	GUIListBox*									m_pLbShow;

	ESpriteShowState						m_eState;
	vector<DWORD>						m_vecCurrShowList;
	DWORD										m_dwCurrTrainID;

	//////////////////////////////////////////////////////////////////////////
	//	ÐÞÁ¶Ñý¾«
	//////////////////////////////////////////////////////////////////////////
	GUIWnd*										m_pWndTrain;
	GUIButton*									m_pBnTrain;
	GUIButton*									m_pBnTrainClose;
	ItemButton*									m_pIbNeedItem;
	ItemButton*									m_pIbRewordItem;

	GUIPatch*									m_pStName;
	GUIStatic*									m_pStAtt;
	GUIPatch*									m_pStAtt2;
	GUIStatic*									m_pStItem;
	GUIStatic*									m_pStSpriteExp;
	GUIStatic*									m_pStIExp;
	GUIStatic*									m_pStIResultNum;
	GUIProgress*								m_pPgProgress;

	INT64											m_n64TrainItemID;
	Item*											m_pTItem;

	bool												m_bStart;
	vector<tagFamilySpriteRequirementSave> m_vecFamilySpriteAtt;
};
