#pragma once
#include "gameframe.h"

struct tagRolePickEvent;
struct tagScenePickEvent;
struct tagGroundItemPickEvent;
struct tagMouseMoveEvent;
struct tagRoleRClickEvent;
class Role;

/** \class CombatFrame_EventCreater
	\brief 战斗相关游戏事件生成器
*/

// 6秒规则
const INT SelectTargetClearTime = 6000;

class CombatFrame_EventCreater :
	public GameFrame
{
public:
	CombatFrame_EventCreater(void);
	virtual ~CombatFrame_EventCreater(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();

	//--
	bool IsSelectEnemy(){ return m_bSelectEnemy;}
	DWORD GetCurTargetID(){ return m_curTargetRoleID;}

private:
	DWORD OnKey(DWORD dwID, BOOL bDown);
	DWORD OnRolePickEvent(tagRolePickEvent* pEvent);
	DWORD OnRoleRClickEvent(tagRoleRClickEvent* pEvent );
	DWORD OnScenePickEvent(tagScenePickEvent* pEvent);
	DWORD OnGroundItemPickEvent(tagGroundItemPickEvent* pEvent);
	DWORD OnMouseMoveEvent(tagMouseMoveEvent* pEvent);
	bool  CanSwitchTargetRole(Role* pRole,const Frustum* pFrustum);

	// TAB按键相关
	VOID  ClearAttackingRoleMap();
	// 自动清空攻击者列表
	VOID  UpdateAttackerMap();
public:
	// 添加新的攻击者再攻击者map中，添加成功返回true
	BOOL  AddNewAttacker(DWORD dwSrcRoleID,DWORD dwTarRoleID);
	BOOL  DeleteNewAttacker(DWORD dwSrcRoleID);
	DWORD GetNextTarget();

private:
	TSFPTrunk<CombatFrame_EventCreater>	m_trunk;
	TObjRef<GameInputMap>	m_pKeyMap;

	DWORD					m_curTargetRoleID;
	bool					m_bSelectEnemy;

	BOOL					m_bFrontKeyDown;
	BOOL					m_bBackKeyDown;
	BOOL					m_bLeftKeyDown;
	BOOL					m_bRightKeyDown;

	map<DWORD,Role*>		m_mapAttackingRoleMap;  //!< 存储目前正在攻击本地玩家的NPC或者RL
	DWORD					m_dwAutoClearTimeMark;

	DWORD					m_dwLastTabTime;
};
