#pragma once
class PlayerTryDressWnd;

class DresserFrame :
	public GameFrame
{
public:
	DresserFrame(void);
	~DresserFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	virtual VOID Render3D();
	VOID SetNPCID(DWORD id) { m_dwNpcID = id; }

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<DresserFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd						*m_pWnd;				//主界面
	GUIPatch					*m_pCaption;			//标题
	PlayerTryDressWnd			*m_pPlayerView;

	GUIButton*					m_pLBtnHType;		//发型左按钮
	GUIButton*					m_pRBtnHType;		//发型右按钮
	GUIStatic*					m_pStcHType;		//发型号显示static

	GUIButton*					m_pLBtnHColor;		//发色左按钮
	GUIButton*					m_pRBtnHColor;		//发色右按钮
	GUIStatic*					m_pStcHColor;		//发色号显示static

	GUIButton*					m_pLBtnFace;		//面部左按钮
	GUIButton*					m_pRBtnFace;		//面部右按钮
	GUIStatic*					m_pStcFace;			//面部号显示static

	GUIButton*					m_pLBtnFDetail;		//面部细节左按钮
	GUIButton*					m_pRBtnFDetail;		//面部细节右按钮
	GUIStatic*					m_pStcFDetail;		//面部细节号显示static

	GUIButton*					m_pBtnOK;
	GUIButton*					m_pBtnQuit;
	GUIButton*					m_pBtnExit;

	DWORD						m_dwNpcID;
	DWORD						m_escapeTime;

	tagAvatarAtt				m_avatarAtt;		//角色属性
};