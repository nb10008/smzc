#pragma once

#include "CurrencyMgr.h"
#include "ReputeMgr.h"
#include "ProgressEx.h"
#include "..\WorldDefine\reputation.h"
#include "..\WorldDefine\msg_famehall.h"

class FameHallFrame;
struct tagNS_SynTotalStrength;
struct tagNS_GetStrengthDetail;
struct tagNS_SynPvPStatics;
struct tagUpdateLPAttEvent;

/*!
	\class ReputeFrame
	\brief 声望系统界面
	\author hyu
	\base GameFrame
	\remark
*/
class ReputeFrame : public GameFrame
{
public:
	ReputeFrame(void);
	~ReputeFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	
	//刷新声望显示
	VOID UpdateRepute();

private:
	DWORD OnNS_SynTotalStrength(tagNS_SynTotalStrength* pMsg, DWORD);
    DWORD OnNS_GetStrengthDetail(tagNS_GetStrengthDetail* pMsg, DWORD);
	DWORD OnNS_SynPvPStatics(tagNS_SynPvPStatics* pMsg, DWORD);

	DWORD OnGameEvent(tagGameEvent* pGameEvent);

	//DWORD OnFamousUIOpen(tagGameEvent* pMsg);
	//DWORD OnFamousUIClose(tagGameEvent* pMsg);

    DWORD OnReputeChange(tagReputeChange* pMsg);
	DWORD OnAttibuteChange(tagUpdateLPAttEvent* pMsg);
    DWORD OnBecomeEvent(tagBecomeEvent* pMsg);
    DWORD OnRoleClanContributeChange(tagRoleClanContributeChange* pMsg);

	DWORD EventHandler(tagGUIEvent* pEvent);

	TSFPTrunk<ReputeFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	//GUI控件
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;

	//GUIButton*					m_pBtEnterFameHall;
    GUIButton*                  m_pBtRanking;

    GUIStatic*                  m_pStcGuildExploit;     //!< 帮派功勋
    //GUIStatic*                  m_pStcGuildExploitTxt;  //!< 帮派功勋文字
    GUIStatic*                  m_pStcGuildContribute;  //!< 帮派贡献
    //GUIStatic*                  m_pStcExperiace;        //!< 帮派阅历

    GUIStatic*                  m_pStcStrengthValue;    //!< 实力总值
	GUIStatic*					m_pStcLevel;			// 等级实力值
	GUIStatic*					m_pStcEquip;			// 装备实力值
	GUIStatic*					m_pStcSprite;			// 妖精实力值
	//GUIStatic*					m_pStcTitle;			// 称号实力值
	//GUIStatic*					m_pStcFriend;			// 好友实力值
    //GUIStatic*                  m_pStcStrengthBase;     //!< 固有实力
    //GUIStatic*                  m_pStcStrengthEx;       //!< 过阶实力
    //GUIButton*                  m_pBtStrengthDetail;    //!< 实力按钮

    GUIStatic*                  m_pStcWuXun;            //!< 武勋
	//GUIStatic*					m_pStcWuXunNextLevel;	//!< 下级武勋值
    //GUIStatic*                  m_pStcWuXunTxt;         //!< 武勋txt
    GUIStatic*                  m_pStcWuJi;             //!< 武技
	//GUIStatic*                  m_pStcWuJiMax;          //!< 本级最大武技
    GUIStatic*                  m_pStcTotal;	        //!< 3v3 总场数
    GUIStatic*                  m_pStcWinCount;         //!< 3v3 胜率

	GUIStatic*					m_pFlowerNum;	//鲜花数
	GUIStatic*					m_pEggNum;		//鸡蛋数

	//struct tagReputeGUICollect;
	//tagReputeGUICollect* m_pReputeGUICollect;

	/*! 其它 */
	//bool						m_bisFamousOpen;	/*!< 名人堂窗体状态 */
	//FameHallFrame*				m_pFameHallFrame;	/*!< 名人堂窗体frame */

    bool                        m_bneedRefreshRepute;
    DWORD                       m_timeFirst;
};
