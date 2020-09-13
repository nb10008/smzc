#pragma once

#include "IconStatic.h"
struct tagCityWarApplyInfoRefreshEvent;
struct tagGetSomeGuildNameEvent;
struct tagNS_SignupForCitywar;


class CityWarApplyFrame : public GameFrame
{
public:
	enum EApplyType
	{
		EAT_ApplyManage,	// 申请城市管理
		EAT_ApplyAttack,	// 申请城战
		EAT_ApplyDefence	// 申请防御
	};


	CityWarApplyFrame();
	~CityWarApplyFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	
private:
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnEventApplyInfoRefresh(tagCityWarApplyInfoRefreshEvent *pEvent);
	DWORD OnEventGetSomeGuildName(tagGetSomeGuildNameEvent *pEvent);
	DWORD OnMsgBoxEventApplyManage(tagMsgBoxEvent *pEvent);
	DWORD OnMsgBoxEventConfirmDefence(tagMsgBoxEvent *pEvent);

	DWORD OnNetSignupForCitywar(tagNS_SignupForCitywar *pNetCmd);


	VOID UpdateUI();
private:
	TObjRef<NetSession> m_pSession;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TSFPTrunk<CityWarApplyFrame> m_Trunk;


	GUIWnd *m_pWnd;
	GUIStatic *m_pStcCityName;			// 城市名称
	IconStatic *m_pIstCityIcon;			// 主城图标
	GUIButton *m_pBtnCityInfo;			// 城市信息
	GUIStatic *m_pStcOwnGuild;			// 统治权所属帮派
	GUIStatic *m_pStcNextWarTime;		// 下次城战时间
	GUIStatic *m_pStcApplyCostLowLimit;	// 进攻报名费下限
	GUIStatic *m_pStcApplycostAverage;	// 报名费平均费用

	GUIListBox *m_pLstAttackGuildList;	// 进攻报名帮派列表
	GUIListBox *m_pLstDefenseGuildList;	// 防御报名帮派列表

	GUIButton *m_pBtnApplyManage;		// 申请管理城市
	GUIButton *m_pBtnConfirmAssistDefense;	// 确认协助防御帮派
	GUIButton *m_pBtnAttackApply;		// 进攻报名
	GUIButton *m_pBtnDefenseApply;		// 防御报名
	GUIButton *m_pBtnCancel;			// 取消
	GUIButton *m_pBtnClose;				// 关闭 
	int m_nCurRowAttack;
	int m_nCurRowDefense;
	
	int m_nGuildNameGetFlag;	
	DWORD m_dwCityID;
	DWORD m_dwOwnGuildID;
	std::vector<DWORD> m_vecAttackIDs;
	std::vector<DWORD> m_vecDefenseIDs;
	std::vector<DWORD> m_vecConfirmIDs;
	EApplyType m_eApplyType;

	DWORD m_dwAvgMoney;
	DWORD m_dwTime;
};

