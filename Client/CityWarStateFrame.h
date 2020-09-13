#pragma once

#define NUM_OF_COLUMN 5
#define NUM_OF_DOOR 3

class GlintStatic;
class IconStatic;
struct tagNS_CityStruggleInfo;
struct tagNS_CityStruggleInfoChange;
struct tagLeaveCityWarEvent;
class CityWarStateFrame: GameFrame
{

public:
	CityWarStateFrame();
	~CityWarStateFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

protected:
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD RuleEventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnLeaveCityWarEvent(tagLeaveCityWarEvent* pGameEvent);

	DWORD OnNetCityStruggleInfo(tagNS_CityStruggleInfo*, DWORD);
	DWORD OnNetCityStruggleInfoChange(tagNS_CityStruggleInfoChange*, DWORD);

private:
	void SetDefenceDragon(BOOL bSealed, BOOL bSealing, INT countDownTime);
	void SetAttackDragon(BOOL bSealed, BOOL bSealing, INT countDownTime);
	void SetDefenceCount(INT iCount);
	void SetAttackCount(INT iCount);
	void SetColumnState(INT iIndex, BOOL bSealed);
	void SetWare(INT iCount);
	void SetDoor(INT iDoorIndex, BOOL bDestroyed, INT repairTime);
private:
	TSFPTrunk<CityWarStateFrame> m_Trunk;
	TObjRef<GameFrameMgr> m_pFrmMgr;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetSession>	m_pSession;
	TObjRef<NetCmdMgr> m_pCmdMgr;

	GUIWnd*					m_pWnd;
	GUIButton*				m_pBtnWarResult;
	GUIButton*				m_pBtnWarRule;
	GUIButton*				m_pBtnFolder;
	GUIStatic*				m_pStaticDefenceCount;
	GUIStatic*				m_pStaticAttackCount;
	GlintStatic*			m_pGSDefenceDragon;
	GlintStatic*			m_pGSAttackDragon;
	GlintStatic*			m_pGSDefenceTime;
	GlintStatic*			m_pGSAttackTime;
	IconStatic*				m_pISColumn[NUM_OF_COLUMN];
	GUIStatic*				m_pStaticNumOfWare;
	GUIStatic*				m_pStaticDoor[NUM_OF_DOOR];

	GUIWnd*					m_pRuleDescription;
	GUIButton*				m_pBtnRuleDescOK;
	GUIButton*				m_pBtnRuleDescCancel;

	DWORD					m_dwDefenceDragonTime;
	DWORD					m_dwAttackDragonTime;
	DWORD					m_dwDoorRepireTime[NUM_OF_DOOR];
	tstring					m_strColumPic[NUM_OF_COLUMN][3];
	tstring					m_strDragonPic[3];
};