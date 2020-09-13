#pragma once

const INT MAX_BROADCAST_NUM = 3;
class BigBroadCastFrame : public GameFrame
{
public:
	BigBroadCastFrame();
	~BigBroadCastFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID PushInfo(LPCTSTR szContent);

private:
	TObjRef<GUISystem> m_pGUI;

	GUIWnd *m_pWnd;

	GUIStaticEx *m_pStcBroadcast[MAX_BROADCAST_NUM];

	typedef std::vector<tstring> MsgCache;
	MsgCache m_msgCache;
	DWORD m_dwTime[MAX_BROADCAST_NUM];						// 时间标志位
	bool m_bBroadcasting[MAX_BROADCAST_NUM];				// 播放标志位
};
