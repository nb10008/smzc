#pragma once
#include "gameframe.h"

class FaceBookFrame :
	public GameFrame
{
public:
	FaceBookFrame(void);
	~FaceBookFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();

	BOOL EventHandler(tagGUIEvent* pEvent);
	tstring	GetURL_Taiwan();
	tstring	GetURL_HongKong();
private:
	TSFPTrunk<FaceBookFrame>		m_Trunk;
	TObjRef<GUISystem>          m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	GUIWnd*                     m_pWnd;
	GUIButton*					m_pBnClose;			
	GUIButton*					m_pBnFaceBook;			

};
