#pragma once
#include "..\WorldDefine\msg_frame.h"
#include "TrainFrame.h"

#define MAX_DailyExp 2000
/** \class SysMenuFrame
    \brief 右下角系统菜单界面（包括扩展菜单）
*/
struct tagNS_QuerySoaringSwitch;
class FlashPushButton;
class ActiveProgress;
struct tagUpdateLPAttEvent;
struct tagFamilyFrameEvent;

class SysMenuFrame : public GameFrame
{
public:
	SysMenuFrame();
	~SysMenuFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual void Update();
	enum EMenuExItem
	{
		//EMI_Help,
		EMI_Sociality,
		EMI_Faction,
		EMI_Pet,
		EMI_ZoneMap,
		EMI_WorldMap,
		EMI_Encrypt,
		EMI_SysOptionMenu,
		EMI_Num,
		EMI_Help,
	};
private:
	/** \游戏事件响应函数
	*/
	DWORD OnClose_Role_UIEvent(tagGameEvent* );
	DWORD OnClose_Package_UIEvent(tagGameEvent* );
	DWORD OnClose_Dower_UIEvent(tagGameEvent* );
	DWORD OnOpen_Task_UIEvent(tagGameEvent* );
	DWORD OnClose_Task_UIEvent(tagGameEvent* );
	DWORD OnClose_Mall_UIEvent(tagGameEvent* );
	DWORD OnOpen_Mall_UIEvent(tagGameEvent* );
	DWORD OnOpen_Role_UIEvent(tagGameEvent*);
	DWORD OnOpen_Dower_UIEvent(tagGameEvent*);
	DWORD OnOpen_Guild_UIEvent(tagGameEvent*);
	DWORD OnClose_Guild_UIEvent(tagGameEvent*);
	DWORD OnClose_Pet_UIEvent(tagGameEvent*);
	DWORD OnClose_Sociality_UIEvent(tagGameEvent*);
	DWORD OnClose_Train_UIEvent(tagGameEvent* );
	DWORD OnClose_Soar_UIEvent(tagGameEvent*);
	DWORD OnForceOpen_Package_UIEvent(tagGameEvent*);
	DWORD OnOpen_Area_UIEvent(tagGameEvent* );
	DWORD OnClose_Area_UIEvent(tagGameEvent* );
	DWORD OnOpen_Pulse_UIEvent(tagGameEvent* );
	DWORD OnClose_Pulse_UIEvent(tagGameEvent* );

	DWORD OnGameEventLevelUp(tagGameEvent*);
	DWORD TryOpenSoarQuest(tagMsgBoxEvent* pGameEvent);

	DWORD FlashTrainButton(tagGameEvent*);

	DWORD OnOpen_Holy(tagGameEvent* pEvent);

	/** \热键响应函数
	*/
	DWORD OnOpenClose_ShopHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_RoleHotKey( DWORD dwID, BOOL bDown );
    DWORD OnOpenClose_RoleInfoKey( DWORD dwID, BOOL bDown );
    DWORD OnOpenClose_RoleEquipKey( DWORD dwID, BOOL bDown );
    DWORD OnOpenClose_RoleTitleKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_PackageHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_DowerHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_TaskHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_ZoneMapHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_WorldMapHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_SocialityHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_HelpHotKey(DWORD dwID, BOOL bDown);
    DWORD OnOpenClose_PetHotKey(DWORD dwID, BOOL bDown);
	DWORD OnOpenClose_Guild(DWORD dwID, BOOL bDown);
	DWORD OnOpenClose_TrainHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_AreaHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_SoarHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_FamilyHotKey( DWORD dwID, BOOL bDown );
	DWORD OnOpenClose_RoleHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_PackageHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_DowerHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_TaskHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_GuildChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_PetHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_SocialityHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_HelpHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_ZoneMapHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_WorldMapHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_ShopHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_TrainHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_SoarHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_AreaHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnOpenClose_FamilyHotKeyChange(DWORD dwID, DWORD key1, DWORD key2);


	// 打开各界面
	DWORD OnNS_OpenFramePage( tagNS_OpenFramePage *pNetCmd, DWORD );
	DWORD OnNS_QuerySoaringSwitch(tagNS_QuerySoaringSwitch *pNetCmd, DWORD );

	// 打开每日信仰界面
	DWORD OnOpenDialyExpFrame(tagGameEvent* pEvent);					//点击按钮显示界面
	DWORD OnOpenDialyExpFrameByTime(tagUpdateLPAttEvent* pEvent);		//十秒后自动关闭的
	DWORD OnFamilyEvent_Frame(tagFamilyFrameEvent* pEvent);

private:
	TSFPTrunk<SysMenuFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameInputMap>		m_pKeyMap;
	TObjRef<Util>				m_pUtil;
	TObjRef<GUIRender>			m_pRender;

	GUIWnd*                     m_pWnd;                   //系统菜单窗口
	GUIWnd*						m_pPanel;                 //底版
	GUIPushButton*				m_pBn_Role;               //角色按钮
	GUIPushButton*				m_pBn_Package;            //行囊按钮
	GUIPushButton*				m_pBn_Dower;              //天资按钮
	GUIPushButton*				m_pBn_Task;               //任务按钮
	GUIPushButton*				m_pBn_Mall;               //商城按钮
	GUIPushButton*				m_pBn_Guild;			  //帮派按钮
    GUIPushButton*              m_pBn_Pet;                //灵兽按钮
	GUIPushButton*              m_pBn_Sociality;          //人际按钮
	GUIButton*		            m_pBn_Expand;             //菜单按钮
	FlashPushButton*			m_pBn_Train;			  //试炼按钮
	GUIPushButton*				m_pBn_PvP;				  //竞技场按钮
	GUIPushButton*              m_pBn_Soar;				  //飞升按钮
	GUIButton*					m_pBn_Hide;				  //隐藏按钮
	GUIPushButton*				m_pBn_God;               //领域按钮
	GUIPushButton*				m_pBn_Family;            //家族按钮
	GUIPushButton*				m_pBn_Pulse;            //血脉按钮
	
	// TIPS描述，不包含热键
	tstring						m_strTip_Role;               //角色
	tstring						m_strTip_Package;            //行囊
	tstring						m_strTip_Dower;              //天资
	tstring						m_strTip_Task;               //任务
	tstring						m_strTip_Mall;               //商城
	tstring						m_strTip_Guild;				 //帮派
	tstring						m_strTip_Pet;                //灵兽
	tstring						m_strTip_Sociality;          //人际
	tstring						m_strTip_Train;				 //试炼
	tstring						m_strTip_Soar;				 //飞升
	tstring						m_strTip_Hide;				 //隐藏
	tstring						m_strTip_God;				 //领域
	tstring						m_strTip_Family;			//家族
	tstring						m_strTip_Pulse;			//血脉

	bool						m_bSoarOpen;

	GUIWnd*						m_pHideWnd;
	GUIPushButton*				m_pBtnMall;
	GUIButton*					m_pBtnHide;

	GUIWnd*						m_pWndGodDailyExp;
	ActiveProgress*				m_pProcess;
	GUIStatic*					m_pStcText;
	INT							m_nShowTime;	//值为GT_INVALID表示不自动关闭
	DWORD                       m_escapeTime;

	BOOL GUIEventHandlerHideWnd(tagGUIEvent* pEvent);

	BOOL GUIEventHandlerMenu(tagGUIEvent* pEvent);        //系统菜单事件响应回调函数

	GUIWnd*                     m_pWndMenuEx;             //系统扩展菜单窗口
	GUIListBox*                 m_pListMenuEx;            //系统扩展菜单条目
	tstring                     m_strMenuExItemText[EMI_Num];

	BOOL GUIEventHandlerMenuEx(tagGUIEvent* pEvent);      //系统扩展菜单事件响应回调函数
	VOID SafeDestroyUI();
	VOID UpdataMenuSize();
};

