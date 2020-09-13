#pragma once
#include "gameframe.h"
#include "ReputeMgr.h"
class Item;

namespace
{
	const INT GODEVIL_SKILL_NUM = 11;
	const INT GODEVIL_SKILL_LEVEL_INDEX[GODEVIL_SKILL_LEVEL] = {0,1,3,5,8};
	const DWORD TALENTBTN_COLOR[2] = {0xFF9A9A9A,0xFFFFFFFF};
	const tstring strIconDefault = _T("data\\ui\\Skill\\L-skill-tiao-01.bmp");
	const INT SPIRIT_CRYSTAL_MAX_ATTRIBUTES = 8;
}

struct tagEquipPatch
{
	GUIStatic*		pStName;
	GUIStatic*		pStDesc;
};

class TalentButton;
class ItemButton;
class IconStatic;
class Item;
struct tagItemPosChangeExEvent;
struct tagNS_SoulCrystalGrow;
struct tagNS_SoulCrystalRenew;

class GodEvilReputeFrame :
	public GameFrame
{

public:
	GodEvilReputeFrame(void);
	~GodEvilReputeFrame(void);
	
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID OpenGEReputeFrame(BYTE byType);
	
	//--------------------军事分界------------------------------------------朝鲜省
	//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
	//--------------------38线----------------------------------------------
	//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
	//--------------------军事分界------------------------------------------韩国省

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID	RefreshGEInfo();

	BOOL	Reload_GESkillUI(GUIWnd* pWnd);
	DWORD	EventHandler_Skill(tagGUIEvent* pEvent);
	VOID	RefreshGESKillInfo();
	VOID	SwapGodToEvil();
	FLOAT	CalcReputeProgress();

	VOID	ShowSkillOPWnd(DWORD dwSkillID);
	VOID	SetRenewItem(Item* pItem);
	VOID	CloseSKillOPWnd();
	DWORD	OnEventRefreshGEInfo(tagGameEvent* pGameEvent);
	DWORD	OnEventCloseGESkillOPWnd(tagGameEvent* pGameEvent);
	DWORD	OnEventMove2GESkillRenew(tagItemPosChangeExEvent* pGameEvent);
	


	//--------------------军事分界------------------------------------------朝鲜省
	//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
	//--------------------38线----------------------------------------------
	//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
	//--------------------军事分界------------------------------------------韩国省
	DWORD EventHandler_Equip(tagGUIEvent* pEvent);
	BOOL Reload_GEEquipUI(GUIWnd* pWnd);
	VOID RefreshEquipUI(Item* pEquip);
	VOID RefreshRateInfo();
	VOID LockItem( bool bOperable );
	VOID RefreshShenHunNum();

	VOID ResetAblity();
	VOID LevelUp();
	VOID ClearUI();

	DWORD OnMsgBox_HunJingReset_Check(tagMsgBoxEvent* pGameEvent);
	DWORD OnMsgBox_HunJingLevelUp_Check(tagMsgBoxEvent* pGameEvent);

	DWORD OnNS_SoulCrystalGrow(tagNS_SoulCrystalGrow* pCmd, DWORD);
	DWORD OnNS_SoulCrystalRenew(tagNS_SoulCrystalRenew* pCmd, DWORD);
private:
	TSFPTrunk<GodEvilReputeFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;

	vector<GUIStatic*>					m_lstSwapWnd;
	EReputeType							m_eType;
	DWORD								m_dwCurChooseSkillID;
	DWORD								m_lastUpdateTime;

	GUIWnd*								m_pWnd;

	GUIWnd*								m_pWndSpiritSKill;
	GUIWnd*								m_pWndSpiritEquip;
	GUIPushButton*						m_pPbnSpiritSKill;
	GUIPushButton*						m_pPbnSpiritEquip;

	GUIStatic*							m_pStcGESoul;
	GUIButton*							m_pBtnSysClose;

	TalentButton*						m_pLstBtnSkill[GODEVIL_SKILL_NUM];
	DWORD								m_dwLstSkill[GODEVIL_SKILL_NUM];
	GUIStatic*							m_pLstStcSkillInfo[GODEVIL_SKILL_NUM];

	GUIProgress*						m_pPgrRepute;
	GUIStatic*							m_pLstStcReputeLevelReq[GODEVIL_SKILL_LEVEL - 1];


	GUIWnd*								m_pWnd_SkillOP;
	IconStatic*							m_pStcOP_SKill;
	GUIPatch*							m_pPchOP_SKillName;
	GUIStaticEx*						m_pStcOP_SkillDesc;
	GUIStatic*							m_pStcOP_SkillState;
	GUIStatic*							m_pStcOP_SkillLimitTime;

	ItemButton*							m_pBtnOP_Item;
	GUIStatic*							m_pStcOP_ActiveSpirit;
	GUIStatic*							m_pStcOP_ActiveSilver;

	GUIStatic*							m_pStcOP_RenewSpirit;
	GUIStatic*							m_pStcOP_RenewSilver;

	GUIButton*							m_pBtnOP_Active;
	GUIButton*							m_pBtnOP_Renew;

	GUIButton*							m_pBtnOP_SysClose;

	//--------------------军事分界------------------------------------------朝鲜省
	//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
	//--------------------38线----------------------------------------------
	//地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷地雷
	//--------------------军事分界------------------------------------------韩国省
	ItemButton*						m_pItemSpiritCrystal;//魂晶升级界面神魂晶
	ItemButton*						m_pItemSpirit;			//魂石精华
	tagEquipPatch					m_pStAttrabutes[SPIRIT_CRYSTAL_MAX_ATTRIBUTES];	//神魂晶属性
	
	GUIStatic*							m_pNewAttProbability;	//开启新能力概率
	GUIStatic*							m_pUpdateAttProbability;//能力等级提升概率
	GUIButton*						m_pBtnLevelUp;				//提升等级 按钮
	GUIButton*						m_pBtnResetAtt;				//重置能力按钮
	GUIPushButton*				m_pBtnNoConfirmations;	//没有二次确认

	GUIStatic*							m_pStName;		
	GUIStatic*							m_pStZiZhi;	
	GUIStatic*							m_pStTotal;	

	GUIStatic*							m_pNextLevelNeedsItem;	//下一等级需要
	GUIStatic*							m_pNextLevelNeedsGold;	//下一等级需要
	GUIStatic*							m_pNextLevelNeedsSilver;	//下一等级需要

	GUIStatic*							m_pStHunNumName;
	GUIStatic*							m_pStShenHunNum;	

	GUIStatic*							m_pStHunType;	

	Item*								m_pEquip;		//装备
	Item*								m_pItem;		//魂石精华

	
};
