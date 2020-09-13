#pragma once

#include "..\WorldDefine\msg_rankings.h"
#include "SocialEvent.h"

struct tagNS_GetRankingInfo;
struct tagNS_GetOfflineEquipInfo;
struct tagNS_GetOfflineReputation;

class StrengthRankingFrame : public GameFrame
{
public:
	StrengthRankingFrame();
	~StrengthRankingFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	VOID SendGetRankInfo(int nPage, ERankingType eType);
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnNetGetRankInfo(tagNS_GetRankingInfo *pNetCmd);
	DWORD OnNetGetPetRankInfo(tagNS_GetPetRankingInfo *pNetCmd);
	DWORD OnNetGetOfflineEquipInfo(tagNS_GetOfflineEquipInfo *pNetCmd);
	DWORD OnNetGetOfflineReputation(tagNS_GetOfflineReputation *pNetCmd);
	VOID UpdateUI();

private:
	TSFPTrunk<StrengthRankingFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<NetSession> m_pSession;
	TObjRef<NetCmdMgr> m_pCmdMgr;

	GUIWnd *m_pWnd;


	GUIPushButton *m_pPbnStrength;	// 实力
	GUIPushButton *m_pPbnLevel;		// 等级
	GUIPushButton *m_pBtnEquip;		// 装备
	GUIPushButton *m_pPbnFame;		// 声望
	GUIPushButton *m_pPbnPet;		// 宝宝
	GUIPushButton *m_pPbnFlower;	// 美女榜~
	GUIPushButton *m_pPbnEgg;		// 人妖榜

	GUIStatic *m_pStcCommon;
	GUIListBox *m_pLstRanking;
	GUIButton *m_pBtnPre;		// 前一页
	GUIButton *m_pBtnNext;		// 下一页
	GUIStatic *m_pStcCurNum;	// 当前页数
	GUIStatic *m_pStcLocalRank;	// 自己排名 
	GUIButton *m_pBtnClose1;	// 右上角关闭按钮
	GUIButton *m_pBtnClose2;	// 右下角关闭按钮
	GUIButton *m_pBtnInfo;		// 规则说明

	int m_nCurPageNum;			// 当前页数
	ERankingType m_eCurType;	// 当前类型

	DWORD m_dwRank[RANKINGS_PAGE_LEN];		// 记录当前排名分页的ROLEID
	DWORD m_dwLastRow;						// 上次选择的行
};






