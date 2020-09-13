/********************************************************************
	created:	2012/04/17
	created:	17:4:2012   11:36
	filename: 	e:\code\World\map_monster_gen.h
	file path:	e:\code\World
	file base:	map_monster_gen
	file ext:	h
	author:		zhangzhihua
	
	purpose:	怪物刷新管理器
*********************************************************************/
#pragma once
#include "../WorldDefine/time.h"
#include "world.h"

class Creature;
class Unit;

enum EMonsterGenState
{
	EMGS_UPDATE = 0,		// 刷新
	EMGS_START,				// 开始
	EMGS_WAITING,			// 等待
	EMGS_STOP,				// 停止
	EMGS_CONTINUE,			// 继续
	EMGS_NOTHING,			// 什么也不做

	EMGS_END,
};

class MapMonsterGenMgr
{
public:
	MapMonsterGenMgr();
	~MapMonsterGenMgr();

public:
	BOOL					Init(const Map* pMap);
	VOID					Destroy();
	Map*					GetMap();
	tagMapMonsterGen*		GetNextMonsterGenTable();

	BOOL					SetState(EMonsterGenState eState);
	EMonsterGenState		GetState();

	DWORD					GetID();
	DWORD					GetWave();

	VOID					DoUpdate();				// 刷怪
	VOID					DoStart();				// 开始刷怪
	VOID					DoStop();				// 结束刷怪
	VOID					DoWating();				// 挂起等待
	VOID					DoContinue();			// 继续挂起的刷怪

	VOID					Update();
	VOID					OnCreatureDie(Creature* pCreature, Unit* pKiller);// 怪物死亡

private:
	struct tagMGenInfo
	{
		WORD				wDeadMonster;			// 杀死怪物数量
		tagMapMonsterGen*	pMapMonsterGenTB;		// 怪物刷新表中的对应行数据
		TList<Creature*>	listCreature;
		tagMGenInfo()
		{
			wDeadMonster = 0;
			pMapMonsterGenTB = NULL;
			listCreature.Clear();
		}
	};
	typedef TMap<DWORD, tagMGenInfo*>			MonsterGenRecordMap;			// 刷怪记录

	DWORD					m_dwNextWaveGenTime;	// 下拨怪物刷新时间
	DWORD					m_dwNextMonsterTime;	// 下一个怪的刷新时间
	WORD					m_wGenNum;				// 本波怪物当前已经刷新的数量
	tagMapMonsterGen*		m_pMapMonsterGenTB;		// 怪物刷新表中的对应行数据

	DWORD					m_dwCurTime;			// 当前时间
	DWORD					m_dwWaitingTime;		// 挂起等待时间点

	EMonsterGenState		m_eState;				// 状态控制
	Map*					m_pMap;
	MonsterGenRecordMap		m_mapMonsterGenRecord;	// 刷怪记录
};















