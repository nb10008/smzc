#pragma once
#include "gameframe.h"

struct tagNS_GetOfflineExp;

class OfflineExpFrame : public GameFrame
{
	enum EHarvestExpWndType
	{
		EHET_Init,
		EHET_Free,
		EHET_All
	};
public:
	OfflineExpFrame(void);
	~OfflineExpFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();

	void SetData( DWORD dwHour, DWORD dwMinute, INT64 dwFreeExp, INT64 dwTotalExp, DWORD dwYuanbao )
	{
		m_dwOffHour = dwHour;
		m_dwOffMinute = dwMinute;
		m_dwFreeExp = dwFreeExp;
		m_dwTotalExp = dwTotalExp;
		m_dwYuanBao = dwYuanbao;
	}

	void ShowWnd( BOOL bShow )
	{
		if( P_VALID(m_pWnd) )
		{
			m_pWnd->SetInvisible( !bShow );
			m_pWnd->SetRefresh( TRUE );
		}

		if( bShow )
			RefreshContent();
	}

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	
	void RefreshContent();
	
	void SendHarvestExpMessage();
	
	DWORD OnNS_GetOfflineExp( tagNS_GetOfflineExp* pMsg, DWORD dwParam );
	
private:
	TSFPTrunk<OfflineExpFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	EHarvestExpWndType			m_eWndType;				//当前显示窗口的类型
	bool						m_bWaitServerReturn;	//正在等服务器的消息

	GUIWnd*						m_pWnd;
	GUIButton*					m_pBtnGet;				//获得按钮
	GUIButton*					m_pBtnReturn;			//返回按钮 以及 提取全部经验
	//GUIButton*				m_pBtnClose;			//关闭按钮
	GUIStatic*					m_pStcTitle;			//窗口title
	GUIStaticEx*				m_pStcContent;			//窗口内容

	DWORD						m_dwOffHour;			//离线小时
	DWORD						m_dwOffMinute;			//离线分钟
	INT64						m_dwTotalExp;			//总共经验
	INT64						m_dwFreeExp;			//免费经验
	DWORD						m_dwYuanBao;			//额外经验需要的元宝
};
