#pragma once

class StateFrame;
class EquipmentFrame;
class ReputeFrame;
class RoleTitleFrame;
class InformationFrame;
class TrumpFrame;
class ShengLingFrame;
struct tagNS_ItemPosChangeEx;
struct tagNS_GetRoleInitState_Item;
class CharacterFrame :
	public GameFrame
{
public:
	CharacterFrame(void);
	~CharacterFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	tstring GetGUIFullName()
	{
		if(P_VALID(m_pWndCaption))
			return m_pWndCaption->GetFullName();
		else 
			return _T("");
	};

    tstring GetStateWndFullName()
    {
        if (P_VALID(m_pWnd))
            return m_pWnd->GetFullName();
        else
            return _T("");
    }

protected:

	VOID ShowWnd(BOOL bVal);

	//--处理GUI消息
	DWORD EventHandler(tagGUIEvent* pEvent);
	//--处理游戏事件
	DWORD OnOpenCharacter(tagGameEvent* pGameEvent);
	DWORD OnCloseCharacter(tagGameEvent* pGameEvent);

    DWORD OnOpenEquipState(tagGameEvent* pEvent);
    DWORD OnOpenInfomation(tagGameEvent* pEvent);
    DWORD OnOpenReputation(tagGameEvent* pEvent);
    DWORD OnOpenTitle(tagGameEvent* pEvent);
	DWORD OnOpenTrump(tagGameEvent* pEvent);
	DWORD OnOpenHoly(tagGameEvent* pEvent);
	DWORD NetItemPosChangeEx(tagNS_ItemPosChangeEx* pNetCmd, DWORD);
	DWORD NetInitLocalPlayerItem(tagNS_GetRoleInitState_Item* pNetCmd, DWORD);
private:
	TSFPTrunk<CharacterFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	StateFrame*					m_pState;
	EquipmentFrame*				m_pEquipment;
	ReputeFrame*				m_pRepute;
	RoleTitleFrame*				m_pRoleTitle;
	InformationFrame*			m_pInformation;
	TrumpFrame*					m_pTrump;
	ShengLingFrame*				m_pShengLing;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIWnd*						m_pWndState;
	GUIWnd*						m_pWndCareer;
	GUIWnd*						m_pWndRepute;
	GUIWnd*						m_pWndTitle;
	GUIWnd*						m_pWndRoleTitle;
	GUIWnd*						m_pWndTrump;
	GUIWnd*						m_pWndShengLing;
	GUIPushButton*				m_pPbnState;
	GUIPushButton*				m_pPbnCareer;
	GUIPushButton*				m_pPbnRepute;
	GUIPushButton*				m_pPbnRoleTitle;
	GUIPushButton*				m_pPbnTrump;
	GUIPushButton*				m_pPbnShengLing;

	GUIButton*					m_pBtnClose;
};
