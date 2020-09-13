#pragma once
struct tagNS_CSRoleRecord;
struct tagOpenCityWarResult;

class CityWarResultFrame: public GameFrame
{
public:
	CityWarResultFrame();
	~CityWarResultFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

protected:
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnEventOpenCityWarResult(tagOpenCityWarResult* pEvent);
	DWORD OnNetCityWarRoleRecord(tagNS_CSRoleRecord *pEvent, DWORD);

	void QueryCityWarRoleRecord();
private:
	TSFPTrunk<CityWarResultFrame> m_Trunk;
	TObjRef<GameFrameMgr> m_pFrmMgr;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetSession>	m_pSession;
	TObjRef<NetCmdMgr> m_pCmdMgr;

	GUIWnd*					m_pWnd;
	GUIButton*				m_pBtnClose;
	GUIButton*				m_pBtnCancel;
	GUIStatic*				m_pStaticKill;
	GUIStatic*				m_pStaticDestroyDoor;
	GUIStatic*				m_pStaticRepairDoor;
	GUIStatic*				m_pStaticAcquireWare;
	GUIStatic*				m_pStaticAcquireColumn;
	GUIStatic*				m_pStaticAcquireDragon;
	GUIStatic*				m_pStaticGuildReward;
	GUIStatic*				m_pStaticGuildContribution;
	GUIStatic*				m_pStaticMoneyReward;
	GUIListBox*				m_pLstRanking;
};