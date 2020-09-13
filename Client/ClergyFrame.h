#pragma once
#include "..\WorldDefine\clergy_define.h"

/**\神职晋升Frame
*/
class IconStatic;
class LocalPlayer;
struct tagNS_GetAttribute;
struct tagNS_PromotionClergy;

const DWORD dwGreenColor = 0xff67ff4b;	//绿色
const DWORD dwRedColor = 0xffff4b4b;	//红色
const DWORD dwWhiteColor = 0xffffffff;	//白色
const DWORD dwObtainColor = 0xff4be1d5;	//担任中的颜色

class ClergyFrame : GameFrame
{
public:
	ClergyFrame();
	~ClergyFrame();
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID RefrashInfo(DWORD dwID);	//通过神职ID刷新界面
	VOID ClearInfo();				//清空信息
	VOID Show();
	VOID Hide();
	VOID LockButt();				//锁定按钮
	VOID UnLockButt();				//解锁按钮
	VOID SetIsObtain();				//设置是否满足

	DWORD OnGameEvent(tagGameEvent* pGameEvent);


private:
	TSFPTrunk<ClergyFrame> m_Trunk;
	TObjRef<GUISystem>		   m_pGUI;
	TObjRef<GameFrameMgr>	   m_pFrameMgr;
	TObjRef<NetSession>        m_pNetSession;
	TObjRef<NetCmdMgr>         m_pNetCmdMgr;

	GUIWnd                  *m_pWnd;
	GUIWnd                  *m_pWndCaption;
	GUIStaticEx             *m_pStcGodHeadLimit;				//神格限制
	GUIStaticEx             *m_pStcSexLimit;					//性别限制
	GUIStaticEx             *m_pStcAttLimit;					//属性限制
	GUIStaticEx             *m_pStcLevelLimit;					//级别限制
	GUIStaticEx             *m_pStcClassLimit;					//职业限制
	GUIStaticEx             *m_pStcAttValue;					//属性数值
	IconStatic              *m_pIStcGodIcon;					//图标
	GUIButton               *m_pBtnUp;							//上一个
	GUIButton               *m_pBtnDown;						//下一个
	GUIStatic				*m_pStcClergyText;					//神职文本(上位神=)
	GUIStatic				*m_pStcClergyName;					//神职名称
	GUIStatic				*m_pStcHasReached;					//是否满足条件
	GUIStatic				*m_pStcDesc;						//描述
	GUIButton				*m_pBtnClose;						//关闭
	GUIButton				*m_pBtnPromotion;					//晋升按钮
	IconStatic				*m_pIStcFieldIcon[MAX_FieldNum];	//领域图标

	DWORD					m_dwClergyID;						//神职ID

	const tagClergyName*	pClergyInfo;

	DWORD                   m_dwMapNum;					//神职个数
	bool					m_bIsReach;					//是否达成
	DWORD                   m_dwRoleAtt[ERA_End];		//人物属性暂存
	LocalPlayer*			m_pLocalPlayer;				//本地玩家指针
	DWORD					m_escapeTime;

	BOOL					m_bIsTransform;				//是否变身状态


private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnNS_GetAttribute(tagNS_GetAttribute* pCmd, DWORD);
	DWORD OnNS_PromotionClergy(tagNS_PromotionClergy* pCmd, DWORD);

	DWORD OnEventOpenClergyFrame(tagGameEvent* pEvent);

public:
	tstring GetClergyIconPath(DWORD dwClergyID);
};