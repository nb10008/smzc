/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:05
	filename: 	e:\code\World\war_role.h
	file path:	e:\code\World
	file base:	war_role
	file ext:	h
	author:		zhangzhihua
	
	purpose:	战场角色
*********************************************************************/

#pragma once

class Unit;
CONST INT WAR_REWARD_ITEM_NUM = 3;
class WarRole
{
public:
	WarRole(DWORD dwAccountID, DWORD dwRoleID);
	~WarRole();

	static WarRole* Create(DWORD dwAccountID, DWORD dwRoleID);

	//---------------------------------------------------------------------------------
	// 初始化，更新和销毁
	//---------------------------------------------------------------------------------
	BOOL	Init();
	VOID	Update();
	VOID	Destroy();

	//---------------------------------------------------------------------------------
	// Set/Get
	//---------------------------------------------------------------------------------
	DWORD	GetAccountID();
	DWORD	GetRoleID();
	DWORD	GetRewardItemID(BYTE byIndex);
	VOID	SetRewardItemID(DWORD dwItemID1 = GT_INVALID, DWORD dwItemID2 = GT_INVALID, DWORD dwItemID3 = GT_INVALID);

	//---------------------------------------------------------------------------------
	// 死亡和击杀
	//---------------------------------------------------------------------------------
	VOID	OnDead(Unit* pSelf, Unit* pSrc);
	VOID	OnKill(Unit* pSelf, Unit* pSrc);

	//---------------------------------------------------------------------------------
	// 修改金钱 注意：买卖物品请用AddGlod，其它用ModGlod
	//---------------------------------------------------------------------------------
	VOID	AddGlod(INT nGold, BOOL bSendMsg = TRUE);
	INT		GetGold()						{ return m_nGolds; }

	//---------------------------------------------------------------------------------
	// 战场中玩家击杀玩家、怪、建筑数量
	//---------------------------------------------------------------------------------
	DWORD	GetNeutralMonsterKillCount();
	DWORD	GetMonsterKillCount();
	DWORD	GetBossKillCount();
	DWORD	GetRoleKillCount();
	DWORD	GetRoleDeadCount();
	DWORD   GetRareBossKillCount();

	//---------------------------------------------------------------------------------
	// 进入战场
	//---------------------------------------------------------------------------------
	VOID	OnEnterWar(Role* pRole);

	//---------------------------------------------------------------------------------
	// 离开战场
	//---------------------------------------------------------------------------------
	VOID	OnLeaveWar(Role* pRole);

private:
	DWORD				m_dwAccountID;			// 帐户ID
	DWORD				m_dwRoleID;				// 角色ID
	INT					m_nGolds;				// 金币	

	DWORD m_dwNeutralMonsterKillCount;	// 击杀中立怪数量
	DWORD m_dwMonsterKillCount;			// 击杀怪物计数
	DWORD m_dwBossKillCount;			// 击杀boss计数(首领)
	DWORD m_dwRareBossKillCount;		// 击杀boss计数(稀有)
	DWORD m_dwRoleKillCount;			// 击杀角色计数
	DWORD m_dwRoleDeadCount;			// 角色死亡计数

	DWORD				m_dwRewardItemID[WAR_REWARD_ITEM_NUM];//结算道具奖励

};



