#pragma once
#include "servermerge.h"
#include "Loong_CommonDef.h"
#include "Loong_MergeLogicCtrl.h"
#include "Loong_ThreadTask.h"

//前导声明
class CLoongMergeLogicCtrl;
struct tagThreadPara;

class CLoongServerMerge :public CServerMerge
{
public:
	CLoongServerMerge(void);
	virtual ~CLoongServerMerge(void);

//接口实现
public:
	/*
	/	初始化
	*/
	BOOL Init(	tagConfigDetail	stConfigOne[EM_DB_TYPE_END],
				tagConfigDetail stConfigTwo[EM_DB_TYPE_END],
				EDBMergeType	eMergeType,
				BOOL			bDataClean,
				tstring			strSrc2WorldName,
				INT				nSurviveDay,
				const std::multimap<ERoleCompensationType, tagCompensation*> mmCompensation);
	/*
	/	开始合并
	*/
	BOOL ServerMerge();

public:
	//合并逻辑
	CLoongMergeLogicCtrl m_clsMergeLogicCtrl;

	//开动线程池合并的事件
	HANDLE				m_hPoolMergeEvent;

	ThreadTaskPool		m_clsThreadTaskP;			// 线程任务

private:
	//初始化合并逻辑
	BOOL LoongMergeLogicCtrlInit(CLoongMergeLogicCtrl &clsMergeLogicCtrl);

	//得到线程池状态
	EThreadPoolState GetThreadPoolState();

	//玩家补偿
	BOOL LoongRoleCompensation();

	//换名补偿
	BOOL LoongRoleCompensationChangeRoleName(const tagRoleCompensation &st);

	//等待线程
	EThreadPoolState LoongWaitThreadTask();
private:  

	BOOL				m_bDataClean;				// 数据清理

	EDBMergeType		m_eMergeType;				// db合并类型

	INT					m_nItemDelEquipDelSurviveDay; //保留多少天物品删除备份和装备删除备份

	tstring				m_strSrc2WorldName;			// 源2世界名

	DataBase			m_LoongDB[EM_DB_TYPE_END];	// Beton数据库

	std::list<HANDLE>	m_listSimpleThreadPool;		// 简单线程池	

	std::multimap<ERoleCompensationType, tagCompensation*> m_mmCompensation; // 玩家补偿
};

UINT MyThreadPool(LPVOID pParam);
