/********************************************************************
	created:	2012/04/17
	created:	17:4:2012   11:37
	filename: 	e:\code\World\map_monster_gen.cpp
	file path:	e:\code\World
	file base:	map_monster_gen
	file ext:	cpp
	author:		zhangzhihua
	
	purpose:	怪物刷新管理器
*********************************************************************/
#include "StdAfx.h"
#include "map_monster_gen.h"
#include "map.h"
#include "../vEngine/globe/globe_define.h"
#include "creature.h"
#include "role.h"
#include "../ServerDefine/log_cmdid_define.h"

MapMonsterGenMgr::MapMonsterGenMgr()
{
}

MapMonsterGenMgr::~MapMonsterGenMgr()
{
	Destroy();
}

BOOL MapMonsterGenMgr::Init(const Map* pMap)
{
	if (!P_VALID(pMap))
	{
		return FALSE;
	}
	m_pMap = (Map*)pMap;
	m_pMapMonsterGenTB = NULL;
	m_eState = EMGS_NOTHING;
	DoStop();

	return TRUE;
}

VOID MapMonsterGenMgr::Destroy()
{
	m_mapMonsterGenRecord.ResetIterator();
	tagMGenInfo* pMGenInfo = NULL;
	while( m_mapMonsterGenRecord.PeekNext(pMGenInfo) )
	{
		SAFE_DEL(pMGenInfo);
	}

	m_mapMonsterGenRecord.Clear();
}

Map*  MapMonsterGenMgr::GetMap()
{
	if (!P_VALID(m_pMap))
	{
		ILOG->Write(_T("Map monster gen module error:Lack Map info!\r\n"));
	}

	return m_pMap;
}

tagMapMonsterGen* MapMonsterGenMgr::GetNextMonsterGenTable()
{
	DWORD dwID = GetID();
	DWORD dwWave = GetWave();
	if (!P_VALID(GetMap()))
	{
		return NULL;
	}

	do 
	{
		m_pMapMonsterGenTB = GetMap()->GetMapInfo()->mapMonsterGen.Peek(++dwID);
	}
	while (P_VALID(m_pMapMonsterGenTB) && dwWave == m_pMapMonsterGenTB->dwWave);

	if (!P_VALID(m_pMapMonsterGenTB))
	{
		return NULL;
	}

	DWORD dwRnd = IUTIL->Rand() % 10000;
	while (P_VALID(m_pMapMonsterGenTB) && dwRnd > m_pMapMonsterGenTB->dwWaveRate)
	{
		m_pMapMonsterGenTB = GetMap()->GetMapInfo()->mapMonsterGen.Peek(dwID + 1);
		break;
	}

	tagMGenInfo* pInfo = new tagMGenInfo;
	pInfo->pMapMonsterGenTB = m_pMapMonsterGenTB;
	m_mapMonsterGenRecord.Add(m_pMapMonsterGenTB->dwID, pInfo);

	return m_pMapMonsterGenTB;
}

BOOL MapMonsterGenMgr::SetState(EMonsterGenState eState)
{
	if ((eState < 0 && eState > EMGS_END) ||
		(EMGS_UPDATE == eState && (EMGS_START != GetState() && EMGS_CONTINUE != GetState())) ||
		(EMGS_WAITING == eState && EMGS_UPDATE != GetState()) ||
		(EMGS_CONTINUE == eState && EMGS_NOTHING != GetState()))
	{
		ILOG->Write(_T("MapMonsterGenMgr: MonsterGen State cannot change from %d to %d!\r\n"),
					GetState(), eState);
		return FALSE;
	}

	m_eState = eState;
	return TRUE;
}

EMonsterGenState MapMonsterGenMgr::GetState()
{
	return m_eState;
}

DWORD MapMonsterGenMgr::GetID()
{
	if (P_VALID(m_pMapMonsterGenTB))
	{
		return m_pMapMonsterGenTB->dwID;
	}

	return GT_INVALID;
}

DWORD MapMonsterGenMgr::GetWave()
{
	if (P_VALID(m_pMapMonsterGenTB))
	{
		return m_pMapMonsterGenTB->dwWave;
	}

	return GT_INVALID;
}

VOID MapMonsterGenMgr::DoUpdate()
{
	m_dwCurTime = timeGetTime();
	if (!P_VALID(GetMap()))
	{
		return;
	}

	while (m_dwCurTime > m_dwNextWaveGenTime && m_dwCurTime > m_dwNextMonsterTime)
	{
		// 获得刷怪表
		tagMapMonsterGen * pMonsterGen = m_pMapMonsterGenTB;
		if (!P_VALID(pMonsterGen))
		{
			// 没有取到刷怪表，默认结束刷怪
			DoStop();
			return;
		}

		if (0 == pMonsterGen->wNum)
		{
			GetNextMonsterGenTable();
			continue;
		}

		// 随机怪物ID
		DWORD dwRnd = IUTIL->Rand() % 10000;
		DWORD dwMid = 0;
		while (dwMid < (MAX_MONSTER_GEN_NUM - 1))
		{
			if (dwRnd < pMonsterGen->MonsterGenNode[dwMid].dwRate)
			{
				break;
			}

			dwMid++;
		}

		// 随机导航ID
		dwRnd = IUTIL->Rand() % 10000;
		DWORD dwLid = 0;
		while (dwLid < (MAX_MONSTER_GEN_LINE - 1/* 减一防止dwLid越界*/))
		{
			if (dwRnd < pMonsterGen->MonsterGenNode[dwMid].dwMonsterLine[dwLid][EMonsterGen_Rate])
			{
				break;
			}

			dwLid++;
		}
		tagMapWayPointInfoList* pWayPII =
			GetMap()->GetMapInfo()->mapWayPointList.Peek(pMonsterGen->MonsterGenNode[dwMid].dwMonsterLine[dwLid][EMonsterGen_Line]);

		if (!P_VALID(pWayPII))
		{
			ILOG->Write(_T("Map monster gen err: get way point,check the monstergen.xml!\r\n"));
			DoStop();
			return;
		}
		tagWayPointInfo& WayPI = pWayPII->list.Front();

		// 生成怪物
		// 随机怪物朝向
		Vector3 vFaceTo;
		FLOAT fYaw = FLOAT(IUTIL->Rand() % 360);
		vFaceTo.x = cosf(fYaw * 3.1415927f / 180.0f);
		vFaceTo.z = sinf(fYaw * 3.1415927f / 180.0f);
		vFaceTo.y = 0.0f;

		Creature *pCreature = GetMap()->CreateCreature(pMonsterGen->MonsterGenNode[dwMid].dwMonsterID, WayPI.vPos, vFaceTo);
		if (!P_VALID(pCreature))
		{
			break;
		}
		GetMap()->SetCreaturePatrolList(pCreature, pMonsterGen->MonsterGenNode[dwMid].dwMonsterLine[dwLid][EMonsterGen_Line]);
		tagMGenInfo* pInfo = m_mapMonsterGenRecord.Peek(pMonsterGen->dwID);
		pInfo->listCreature.PushBack(pCreature);

		// 本波怪物当前已经刷新的数量+1
		if (pMonsterGen->wNum == ++m_wGenNum)
		{
			if (EMGTRT_AfterGen == pMonsterGen->eStartTimeType)
			{
				m_dwNextWaveGenTime = m_dwCurTime + pMonsterGen->dwWaveTimeInterval;
			}
			else if (EMGTRT_AllDead == pMonsterGen->eStartTimeType)
			{
				m_dwNextWaveGenTime = 0xffffffff;
			}
			GetNextMonsterGenTable();	//开始下一波刷怪
			m_wGenNum = 0;
		}
		if (m_wGenNum != 0)
		{
			m_dwNextMonsterTime = m_dwCurTime + pMonsterGen->dwEachTimeInterval;
		}
	}
}

VOID MapMonsterGenMgr::DoStart()
{
	if (FALSE == SetState(EMGS_UPDATE))
	{
		return;
	}

	GetNextMonsterGenTable();
}

VOID MapMonsterGenMgr::DoStop()
{
	if (FALSE == SetState(EMGS_NOTHING))
	{
		return;
	}

	m_dwNextWaveGenTime	= 0;
	m_dwNextMonsterTime	= 0;
	m_wGenNum			= 0;
	m_pMapMonsterGenTB = NULL;
	Destroy();
}

VOID MapMonsterGenMgr::DoWating()
{
	if (FALSE == SetState(EMGS_NOTHING))
	{
		return;
	}

	m_dwWaitingTime = m_dwCurTime;
}

VOID MapMonsterGenMgr::DoContinue()
{
	if (FALSE == SetState(EMGS_UPDATE))
	{
		return;
	}

	DWORD dwDiff = m_dwCurTime - m_dwWaitingTime;
	m_dwNextMonsterTime += dwDiff;
	m_dwNextWaveGenTime += dwDiff;
}

VOID MapMonsterGenMgr::Update()
{
	if (!P_VALID(GetMap()))
	{
		return;
	}

	if (GetMap()->GetMapInfo()->mapMonsterGen.Size() <= 0)
	{
		return;
	}

	switch (m_eState)
	{
	case EMGS_UPDATE:
		{
			// 刷怪
			DoUpdate();
		}
		break;
	case EMGS_START:
		{
			// 开始刷怪
			DoStart();
		}
		break;
	case EMGS_WAITING:
		{
			DoWating();
		}
		break;
	case EMGS_STOP:
		{
			DoStop();
		}
		break;
	case EMGS_CONTINUE:
		{
			DoContinue();
		}
		break;
	case EMGS_NOTHING:
		{
			// 啥也不做，我是打酱油的
		}
		break;	
	}
}

VOID MapMonsterGenMgr::OnCreatureDie(Creature* pCreature, Unit* pKiller)
{
	if (!P_VALID(GetMap()))
	{
		return;
	}

	MonsterGenRecordMap::TMapIterator it = m_mapMonsterGenRecord.Begin();
	tagMGenInfo *pMGenInfo;
	while (m_mapMonsterGenRecord.PeekNext(it, pMGenInfo))
	{
		if (!P_VALID(pMGenInfo))
		{
			continue;
		}

		TList<Creature*>::TListIterator listIt = pMGenInfo->listCreature.Begin();
		Creature * pCreatureNode =  NULL;
		while (pMGenInfo->listCreature.PeekNext(listIt, pCreatureNode))
		{
			if (!P_VALID(pCreatureNode))
			{
				continue;
			}

			if (pCreatureNode != pCreature)
			{
				continue;
			}

			pMGenInfo->wDeadMonster++;
			pMGenInfo->listCreature.Erase(pCreatureNode);
			if (pMGenInfo->pMapMonsterGenTB->wNum <= pMGenInfo->wDeadMonster)
			{
				// 本波怪物已经全部杀完：1. 获得奖励 2. 开始计时 3. 清除刷怪表
				Map::RoleMap::TMapIterator itRole = GetMap()->GetRoleMap().Begin();
				Role* pRole = NULL;
				while (GetMap()->GetRoleMap().PeekNext(itRole, pRole))
				{
					if (!P_VALID(pRole))
					{
						continue;
					}

					pRole->GetCurMgr().IncBagSilver(pMGenInfo->pMapMonsterGenTB->dwRewardMoney, ELCID_Loot);
				}

				if (pMGenInfo->pMapMonsterGenTB->dwWave + 1 == m_pMapMonsterGenTB->dwWave &&
					EMGTRT_AllDead == pMGenInfo->pMapMonsterGenTB->eStartTimeType)
				{
					m_dwNextWaveGenTime = m_dwCurTime + pMGenInfo->pMapMonsterGenTB->dwWaveTimeInterval;
					//m_dwNextMonsterTime = m_dwCurTime + pMGenInfo->pMapMonsterGenTB->dwEachTimeInterval;
				}

				m_mapMonsterGenRecord.Erase(pMGenInfo->pMapMonsterGenTB->dwID);
				SAFE_DEL(pMGenInfo);
				return;
			}
		}
	}
}














