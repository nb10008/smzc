#pragma once

#include "ServerTime.h"
struct tagRefreshGodStateEvent;
struct tagRefreshPersonalGodStateEvent;
struct tagRefreshFlagStateEvent;
class IconStatic;

class GodStateFrame : public GameFrame
{
public:
	GodStateFrame();
	~GodStateFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	VOID Show(bool bShow);
private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<GodStateFrame> m_Trunk;
	
	GUIWnd *m_pWndExpand;
	GUIPatch *m_pAttackNum;
	GUIPatch *m_pAttackGuildName;
	GUIPatch *m_pDefenceNum;
	GUIPatch *m_pDefenceGuildName;
	GUIButton *m_pBtnFold;	// 收起
	GUIButton *m_pBtnDetail;// 规则
	GUIStatic *m_pStcKillTime;	// 击杀次数
	GUIStatic *m_pStcLiveTime;	// 活跃时间
	GUIStatic *m_pStcTime;		// 倒计时
	IconStatic *m_pIconFlagSky;		// 天旗
	IconStatic *m_pIconFlagDevil;	// 魔期
	IconStatic *m_pIconFlagHuman;	// 人旗
	GUIStatic *m_pStcExp;			// 所得金钱
	GUIStatic *m_pStcGold;			// 所得金
	GUIStatic *m_pStcSilver;		// 所得银
	GUIStatic *m_pStcGuildExploit;		// 工会功勋
	GUIStatic *m_pStcGuildContribute;	// 工会贡献

	DWORD EventHandlerExpand(tagGUIEvent *pGUIEvent);

	DWORD OnEventRefreshGodState(tagRefreshGodStateEvent *pEvent);
	DWORD OnEventRefreshPersonalGodState(tagRefreshPersonalGodStateEvent *pEvent);
	DWORD OnEventRefreshFlagState(tagRefreshFlagStateEvent *pEvent);

	DWORD m_dwAttackGuildID;
	DWORD m_dwDefenceGuildID;

	tagDWORDTime m_ActivityTime;	// 活动开始时间

	GUIWnd *m_pWndFold;
	GUIPatch *m_pAttackNumFold;
	GUIPatch *m_pAttackGuildNameFold;
	GUIPatch *m_pDefenceNumFold;
	GUIPatch *m_pDefenceGuildNameFold;
	IconStatic *m_pIconFlagSkyFold;		// 天旗
	IconStatic *m_pIconFlagDevilFold;	// 魔期
	IconStatic *m_pIconFlagHumanFold;	// 人旗
	GUIStatic *m_pStcTimeFold;		// 倒计时
	GUIButton *m_pBtnExpend;	// 收起
};

