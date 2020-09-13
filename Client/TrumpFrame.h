#pragma once
#include "../WorldDefine/ItemDefine.h"

class Equipment;
class IconStatic;
class ItemButton;
struct tagNS_FabaoPractice;
struct tagNS_FabaoInlay;
struct tagNS_FabaoWuxingReversal;
struct tagNS_FabaoWuxingMerging;
struct tagFairyMood;
struct tagFairyExcited;
struct tagNS_FabaoMoodChanged;
struct tagNS_FairyStuntBook;
struct tagNS_FairyNirvana;
struct tagNS_FabaoFeeding;
struct tagItemPosChangeExEvent;
struct tagNS_SyncFaBaoStoreExpInfo;
struct tagNS_ExpBall2ExpPilular;
struct tagNS_ExpBallBind2Role;
struct tagNS_InquireSpirteBornState;

class TrumpFrame :
	public GameFrame
{
public:
	TrumpFrame(void);
	~TrumpFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	VOID UpdateUI();
	VOID LockItem(bool bOperable);
	VOID ClearExpSaveInfo();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID ClearText();
	DWORD NetTrumpRefine( tagNS_FabaoPractice *pNetCmd, DWORD );
	DWORD NetTrumpMixBead( tagNS_FabaoInlay *pNetCmd, DWORD );
	DWORD NS_FabaoWuxingReversal(tagNS_FabaoWuxingReversal *pNetCmd,DWORD);
	DWORD NS_FabaoWuxingMerging(tagNS_FabaoWuxingMerging *pNetCmd,DWORD);
	DWORD NetFairyMoodChange(tagNS_FabaoMoodChanged *pNetCmd,DWORD);
	DWORD NetLearnFairyBook( tagNS_FairyStuntBook *pNetCmd, DWORD );
	DWORD NetFairyNirvana( tagNS_FairyNirvana *pNetCmd, DWORD );
	DWORD NetFairyFeeding( tagNS_FabaoFeeding *pNetCmd, DWORD );
	DWORD NetFairySaveExp( tagNS_SyncFaBaoStoreExpInfo *pNetCmd, DWORD );
	DWORD NetFairyExpChange2Ball( tagNS_ExpBall2ExpPilular *pNetCmd, DWORD );
	DWORD NoNS_ExpBallBind2Role( tagNS_ExpBallBind2Role *pNetCmd, DWORD);
	DWORD NoNS_InquireSpirteBornState( tagNS_InquireSpirteBornState *pNetCmd, DWORD);

	DWORD CloseAllTrumpChildFrame(tagGameEvent* pEvent);
	INT GetLingCaoNum();
	VOID UpdateMoodInfo(INT nMood);
	DWORD OnUpdateFairyMood(tagFairyMood* pEvent);
	DWORD OnFairyExcited(tagFairyExcited* pEvent);
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	VOID PutItem(INT16 nPos);
	VOID ChangeExpVector();
	VOID RefreshSpriteBornUI();
private:
	TSFPTrunk<TrumpFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		    m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;

	GUIStatic *m_pStcLingCao[EWX_End];		// 五灵槽的名称
	ItemButton *m_pIconLingCao[EWX_End];	// 五灵槽
	ItemButton *m_pIconTrump;				// 妖精图片

	GUIStatic *m_pStcTrumpName;				// 妖精的名字
	GUIStatic *m_pStcTrumpType;				// 妖精的类型
	GUIStatic *m_pStcTrumpQuality;			// 妖精的资质
	GUIProgress *m_pProFairyQuality;		// 妖精的资质
	GUIStatic *m_pStcFairyExp;				// 妖精的经验
	GUIProgress *m_pProFairyExp;			// 妖精的经验
	GUIStatic *m_pStcTrumpLevel;			// 妖精的等级 
	GUIStatic *m_pStcEquipLevel;			// 妖精的装备等级

	GUIProgress *m_pProFairyMood;			// 妖精的心情
	GUIStatic *m_pStcFairyMood;				// 妖精的心情
	IconStatic *m_pIconAutoFeed;			// 妖精的喂养图标
	IconStatic *m_pIconMoodPic;				// 妖精心情图标
	GUIButton *m_pBtnAutoFeed;				// 自动喂养
	GUIStatic *m_pStcEffect;				// 亢奋效果
	GUIStatic *m_pStcEffectInfo;			// 亢奋效果

	GUIStatic *m_pStcRoleAtt[3];			// 增加的属性
	GUIStatic *m_pStcRoleAttValue[3];		// 增加的属性值
	GUIStatic *m_pStcRoleAttValueRate[3];	// 
	GUIStatic *m_pStcFairyMoodInfo;
	GUIStatic *m_pStcFairyExpInfo;
	GUIStatic *m_pStcFairyExpRate;

	GUIPushButton *m_pPbnFairySoul;
	GUIPushButton *m_pPbnFairyExp;
	GUIPushButton *m_pPbnFairyBorn;
	GUIPatch *m_pStcFairySoulBack;
	GUIPatch *m_pStcFairyExpBack;
	GUIPatch *m_pStcFairyBornBack;
	GUIProgress *m_pProFairySaveExp;
	GUIStaticEx *m_pStcFairySaveExp;
	GUIStaticEx *m_pStcFairyMaxSaveExp;
	GUIButton *m_pBtnSaveExp;
	IconStatic *m_pIconExpBall;
	IconStatic *m_pIconExpVector;
	GUIStaticEx *m_pStcFairyTransExp;
	GUIStaticEx *m_pStcFairyCurSaveExp;
	GUIButton *m_pBtnPre;
	GUIButton *m_pBtnNext;
	GUIButton *m_pBtnTransferExp;
	GUIProgress *m_pProTime;
	GUIStaticEx *m_pStcTrumpWuXing[EWX_End];				// 灵魂奖励
	GUIStaticEx *m_pStcTrumpWuXing1[EWX_End];			// 灵魂奖励
	GUIStaticEx *m_pStcTrumpWuXing2[EWX_End];			// 灵魂奖励
	GUIStaticEx *m_pStcTrumpWuXingAward[EWX_End];	// 灵魂觉醒奖励

	GUIButton *m_pBtnRefineTrump;
	GUIButton *m_pBtnTrumpMixBead;
	GUIButton *m_pBtnTrumpReborn;

	Equipment *m_pTrump;
	DWORD m_dwFairyMoodBuffer;

	INT64 m_n64ExpBall;
	DWORD m_dwCurSaveExp;
	size_t m_nIndex;
	BOOL m_bSaveExp;
	vector<DWORD> m_vecExpBallID;
	bool m_bProcessing;

	GUIStatic* m_pStMatingTime;
	GUIStatic* m_pStSpriteStateNormal;		// 普通阶段
	GUIStatic* m_pStSpriteStatePregnancy;	// 怀孕阶段
	GUIStatic* m_pStSpriteStateBorn;			// 生育阶段
	GUIStatic* m_pStSpriteStateRest;			// 休息阶段

	GUIStatic* m_pStHour;
	GUIStatic* m_pStMIn;
};
