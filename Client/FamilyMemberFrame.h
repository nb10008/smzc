#pragma once
#include "gameframe.h"
#include "..\WorldDefine\family_define.h"

#define  MAX_HEART_NUM     6
class IconStatic;
class FamilyMemberFrame :
	public GameFrame
{
public:
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID ShowWnd(BOOL bShow = TRUE);
	VOID RefreshUI();
	BOOL IsShowing(); 
	FamilyMemberFrame(void);
	~FamilyMemberFrame(void);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID  ClearUI();
	const TCHAR* GetClassTypePic(EClassType eType);
	
	DWORD OnMsgBox_InputRoleName( tagMsgInputBoxEvent* pGameEvent );
	
	VOID  SendTeamApply();
	VOID  SendInviteInput();
	VOID  SendLeaderChange();
	VOID  SendDismisPlayer();
	VOID  SendMemberLeave();

private:
	TSFPTrunk<FamilyMemberFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<GUISystem>						m_pGUI;

	GUIWnd*									m_pWnd;
	GUIWnd*									m_pWndFather;

	GUIPatch*								m_pPchMember[MAX_FAMILY_MEMBER];		// 条	 目			
	IconStatic*								m_pStcProfes[MAX_FAMILY_MEMBER];		// 职	 业
	GUIStatic*								m_pStcLevel[MAX_FAMILY_MEMBER];			// 等	 级
	IconStatic*								m_pStcSex[MAX_FAMILY_MEMBER];			// 性	 别		
	GUIStatic*								m_pStcName[MAX_FAMILY_MEMBER];			// 姓	 名
	GUIStatic*								m_pStcPlace[MAX_FAMILY_MEMBER];			// 场	 景
	GUIStatic*								m_pStcTips[MAX_FAMILY_MEMBER];			// 位置提示
	GUIStatic*								m_pStcCurPick[MAX_FAMILY_MEMBER];		// 选 中 条

	GUIProgress*							m_pProHP[MAX_FAMILY_MEMBER];			// H	P
	GUIProgress*							m_pProMP[MAX_FAMILY_MEMBER];			// M	P

	GUIStatic*								m_pStcHeart[MAX_HEART_NUM];				// 心型图标

	GUIButton*								m_pBtnTeam;			//组队
	GUIButton*								m_pBtnInvite;		//邀请
	GUIButton*								m_pBtnReplace;		//移交
	GUIButton*								m_pBtnDismiss;		//开除
	GUIButton*								m_pBtnLeave;		//脱离


	DWORD									m_dwCurIndex;		//当前选中玩家索引

};
