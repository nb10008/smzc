/********************************************************************
	created:	2010/01/18
	created:	18:1:2010   18:33
	filename: 	e:\loong\trunk\LoongWorld\player_regression.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	player_regression
	file ext:	h
	author:		Jason
	
	purpose:	
*********************************************************************/

#ifndef player_regression_h__
#define player_regression_h__

#include "player_regression_define.h"
#define __USE_JM_TOOL_TEMP
#include "../ServerDefine/mini_tool.h"
#undef  __USE_JM_TOOL_TEMP
#include "../vEngine/util/util.h"

using namespace jm_tool;



class PlayerRegression
{
	typedef std::map<DWORD,tagPlayRegressionRule> TRegMap;
	TRegMap m_mapRules;
	tagPlayRegressionRule m_curRule;
	Util m_util;
	CSCreatedLock m_cs;
	LONG volatile  m_lOpen;
	LONG volatile  m_lCounter;
	//DWORD	m_dwNPC_ID;
	//DWORD	m_dwMapID;
public:
	VOID ClearRules();
	INT		Size()
	{
		return m_mapRules.size();
	}
	// 非线程安全函数，必须在att_res.init中调用，其他地方慎用
	VOID AddRule(VarContainer * pVar,LPCTSTR field);
	BOOL Initial();	// 
	BOOL IsOpen() const //玩家回归活动是否开启了，可能不止一条哟
	{
		return m_lOpen;
	}
	VOID Open(BOOL b){::InterlockedExchange((LPLONG)&m_lOpen,b);}
	BOOL IniRole(Role * pRole); // 判断角色是否可以领取回归奖励，如果可以，就发送领取消息。在角色初始化ok后，调用
	BOOL IniPlayer(PlayerSession * pRole); // 判断角色是否可以领取回归奖励，如果可以，就发送领取消息。在角色初始化ok后，调用
	BOOL IniMap(Map * pMap); //刷npc
	VOID UpdateMap(Map * pMap); // 更新，当节日活动开始时刷出怪；活动结束时，删除怪

	BOOL CanRecRegression(Role*pRole);
	BOOL CanRecRegression(tagDWORDTime timeLogout,BYTE byBackFlag,UINT16 days,BYTE byLevel);

	DWORD RoleGotoNpc(Role * pRole);
	DWORD RewardPlayerBack(Role * pRole);

	PlayerRegression();
	~PlayerRegression();
};

typedef JMSingleton<PlayerRegression> TPlayerRegression;

#endif // player_regression_h__

