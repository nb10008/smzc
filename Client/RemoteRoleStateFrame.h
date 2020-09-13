#pragma once
#include "..\WorldDefine\container_define.h"
class PlayerViewWindow;
class ItemButton;
class Equipment;
class BlinkStatic;
struct tagNS_GetRemoteRoleEquipInfo;
class RemoteRoleStateFrame : public GameFrame
{
public:
	RemoteRoleStateFrame(void);
	~RemoteRoleStateFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();

	void SetDisplayRemoteRoleID(DWORD dwRoleID) { m_dwRemoteRoleID = dwRoleID; }
	DWORD GetDisplayRemoteRoleID()const { return m_dwRemoteRoleID; }

	//设置向服务器发送请求装备时间
	void SetRequireTime(DWORD dwTime) { m_dwTime = dwTime; }
	DWORD GetRequireTime()const { return m_dwTime; }

	//更新当前要查看的角色avatar
	void UpdateEquipAvatar();

	//更新当前要查看的角色装备
	void UpdateAllEquip();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	//游戏事件
	DWORD OnDeleRemoteRoleEquipInfo(tagGameEvent* pGameEvent);

	//网络消息
	DWORD OnNS_GetRemoteRoleEquipInfo(tagNS_GetRemoteRoleEquipInfo* pMsg, DWORD dwParam);

	//获取装备栏位
	EEquipPos GetEquipPos(INT nIndex);

	//获取装备
	Equipment* GetCurRemoteRoleEquip( EEquipPos pos=EEP_Null );

	//更新单个装备
	void UpdateEquipment( EEquipPos ePos );

	void UpdataBlick();

private:
	TSFPTrunk<RemoteRoleStateFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetCmdMgr>					m_pCmdMgr;
	TObjRef<NetSession>					m_pSession;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	GUIStatic*					m_pStcEquipBack[EEP_End];
	ItemButton*					m_pBtnEquip[EEP_End];
	BlinkStatic*					m_pBlkEquip[EEP_End];
	PlayerViewWindow*			m_pWndEquipView;

	BlinkStatic*					m_pBlkEquipFb;

	// vip
	GUIStatic*					m_pStcSilverVIP;
	GUIStatic*					m_pStcGoldVIP;
	GUIStatic*					m_pStcPlatinumVIP;

	BOOL						m_bIsFashionDisplay;	/*!< 是否是时装模式 */
	DWORD						m_dwRemoteRoleID;
	DWORD						m_dwTime;

	map<INT16, Equipment>		m_mapRemoteRoleEquip;
	DWORD					m_dwTimer;
};