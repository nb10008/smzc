#pragma once
#include "gameframe.h"

class ItemButton;
class Item;
struct tagItemPosChangeExEvent;
struct tagNS_FairyBorn;

class SpriteBornFrame :
	public GameFrame
{
public:
	SpriteBornFrame(void);
	~SpriteBornFrame(void);


	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID  Update();
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }


private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID SendNetMessage();
	DWORD OnEventMove2Born(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnNS_FairyBorn(tagNS_FairyBorn* pNetCmd,DWORD);

	VOID PutInSprite(INT64 n64ID);
	VOID ClearInfo();
	VOID RefreshListBox(int nValue);
private:
	TSFPTrunk<SpriteBornFrame>		m_Trunk;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<GUISystem>						m_pGUI;

	GUIWnd*												m_pWnd;
	GUIListBox*											m_pLstProduct;	// 产出列表
	ItemButton*											m_pIcItem;
	//IconStatic*											m_pIcItemBack;
	GUIStatic*											m_pStGold;
	GUIStatic*											m_pStSilver;
	GUIButton*											m_pBnStart;
	GUIButton*											m_pBnClose;
	GUIButton*											m_pBnExit;

	DWORD												m_dwNPCID;				// NPC的ID
	DWORD												m_escapeTime;
	INT64													m_N64ItemID;
};
