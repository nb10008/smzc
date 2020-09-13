//----------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: map_creator.cpp
// author: aslan
// actor:
// data: 2008-6-27
// last:
// brief: 地图生成器，统筹管理所有的MapMgr，维护所有MapMgr的生存期，并提供地图静态属性，全局对象
//----------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_map.h"
#include "../ServerDefine/login_issue.h"

#include "map_creator.h"
#include "map.h"
#include "map_mgr.h"
#include "role.h"
#include "role_mgr.h"
#include "world_session.h"
#include "login_session.h"
#include "map_instance.h"
#include "map_instance_stable.h"
#include "map_restrict.h"
#include "game_guarder.h"
#include "map_instance_script.h"
#include "creature.h"
#include "TimeChecker.h"

#include "athletics_PvP_define.h"

#include "athletics_room.h"
#include "athletics_room_mgr.h"
#include "athletics_system.h" 

#include "guild_war_declare_mgr.h"

#include "../ServerDefine/mini_tool.h"
#include "../ServerDefine/msg_olinfo.h"
#include "vip_netbar.h"
#include "war_role_mgr.h"


#define POOL_SIZE 50*1024*1024		// 初始池大小为50M


MapCreator::MapCreator()
: m_MapEndEvent(TRUE, FALSE), m_nNumMapMgr(0), m_nWhichEvent(1)
{
	// 清空进过副本的所有角色的ID列表
	m_mapRoleIDList.Clear();
}

MapCreator::~MapCreator()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// 初始化所有地图基础信息和所有地图管理器
//-----------------------------------------------------------------------------
BOOL MapCreator::Init()
{
	INT nCpuNum = g_world.GetCpuNum();
	m_pSemIdleCpu = new Semaphore(nCpuNum, nCpuNum);	// 可以吗？

	m_pUtil			=	"Util";
	m_pThread		=	"Thread";
	m_dwInstanceIDGen	=	1000;

	RegisterAFMap();			// 注册地图工厂
	RegisterAFMapRestrict();	// 注册地图策略工厂

	// 初始化地图内存池
	NavKernel::Inst()->Init(POOL_SIZE, true);
	NavResMgr::Inst()->EnableIOAsync(false);//关闭多线程读盘

	// 读取地图逻辑属性xml表
	const TCHAR* szMapLogicProtoFile = _T("data/system/attdata/map_logic_proto.xml");
	CreateObj("MapLogicContainer", "VarContainer");
	TObjRef<VarContainer> pLogic = "MapLogicContainer";
	pLogic->Load(NULL, szMapLogicProtoFile, "id");

	// 读取地图配置文件，从配置文件中读取要加载的地图文件名
	CreateObj("MapContainer", "VarContainer");
	TObjRef<VarContainer> pVar = "MapContainer";
	pVar->Load("VirtualFileSys", _T("server_config/world/map_list.ini"));

	// 加载副本的加权总和
	ZeroMemory(m_nInstanceNumCur, sizeof(m_nInstanceNumCur));
	m_nInstanceCoefNumLimit = pVar->GetInt(_T("coef_num"),	_T("instance"), 1000);
	m_nInstanceCoefNumCur = 0;

	// 加载各个地图属性
	INT nMapNum = pVar->GetInt(_T("num"), _T("map"));
	if( nMapNum <= 0 ) return FALSE;

	TCHAR szTemp[X_SHORT_NAME] = {'\0'};
	for(INT n = 0; n < nMapNum; ++n)
	{
		_stprintf(szTemp, _T("map%d"), n+1);
		LPCTSTR szMapFileName = (LPCTSTR)pVar->GetString(szTemp, _T("map"));
		if( FALSE == P_VALID(szMapFileName) ) return FALSE;

		if( FALSE == LoadMapInfo(szMapFileName, pLogic) )
		{
			ASSERT(0);
			return FALSE;
		}
	}

	// 读取出生地图
	LPCTSTR szBornMapName = (LPCTSTR)pVar->GetString(_T("name"), _T("born_map"));
	m_dwBornMapID = m_pUtil->Crc32(szBornMapName);

	const tagMapInfo* pBornMapInfo = GetMapInfo(m_dwBornMapID);
	ASSERT( P_VALID(pBornMapInfo) );

	// 读取出生地图导航点
	m_nBornPosNum = pVar->GetInt(_T("waypoint_num"), _T("born_map"));
	ASSERT( m_nBornPosNum > 0 );

	m_pvBornPos = new Vector3[m_nBornPosNum];

	TCHAR szBornWPName[X_SHORT_NAME];
	for(INT n = 0; n < m_nBornPosNum; n++)
	{
		_stprintf(szTemp, _T("waypoint_%d"), (n+1));
		IFASTCODE->MemCpy(szBornWPName, pVar->GetString(szTemp, _T("born_map"), _T("")), sizeof(szBornWPName));
		DWORD dwBornWPID = m_pUtil->Crc32(szBornWPName);

		const tagMapWayPointInfoList* pWPList = pBornMapInfo->mapWayPointList.Peek(dwBornWPID);
		if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
		{
			tagWayPointInfo info = pWPList->list.Front();
			m_pvBornPos[n] = info.vPos;
		}
		else
		{
			IMSG(_T("Can't find the reborn waypoint of the map %s\r\n"), pBornMapInfo->szMapFileName);
		}
	}


	// 读取牢狱地图
	LPCTSTR szPrisonMapName = (LPCTSTR)pVar->GetString(_T("name"), _T("prison_map"));
	m_dwPrisonMapID = m_pUtil->Crc32(szPrisonMapName);

	const tagMapInfo* pPrisonMapInfo = GetMapInfo(m_dwPrisonMapID);
	if( P_VALID(pPrisonMapInfo) )
	{

		TCHAR szPrisonWPName[X_SHORT_NAME];
		IFASTCODE->MemCpy(szPrisonWPName, pVar->GetString(_T("inprisonpoint"), _T("prison_map"), _T("")), sizeof(szPrisonWPName));
		DWORD dwPrisonWPID = m_pUtil->Crc32(szPrisonWPName);

		const tagMapWayPointInfoList* pWPList = pPrisonMapInfo->mapWayPointList.Peek(dwPrisonWPID);
		if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
		{
			tagWayPointInfo info = pWPList->list.Front();
			m_PutInPrisonPoint = info.vPos;
		}
		else
		{
			IMSG(_T("Can't find the putin prison waypoint of the map %s\r\n"), pPrisonMapInfo->szMapFileName);
		}

		IFASTCODE->MemCpy(szPrisonWPName, pVar->GetString(_T("outprisonpoint"), _T("prison_map"), _T("")), sizeof(szPrisonWPName));
		dwPrisonWPID = m_pUtil->Crc32(szPrisonWPName);

		pWPList = pPrisonMapInfo->mapWayPointList.Peek(dwPrisonWPID);
		if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
		{
			tagWayPointInfo info = pWPList->list.Front();
			m_PutOutPrisonPoint = info.vPos;
		}
		else
		{
			IMSG(_T("Can't find the putout prison waypoint of the map %s\r\n"), pPrisonMapInfo->szMapFileName);
		}

	}
	else
	{
		ASSERT(0);
		IMSG(_T("Can't find the prison map %s\r\n"), szPrisonMapName);
	}

	// 新手村的副本数量
	m_nMaxStableInstanceNum = pVar->GetInt(_T("num"), _T("initial_map"));


	KillObj("MapContainer");
	KillObj("MapLogicContainer");

	// 遍历所有的MapInfo，生成各个MapMgr
	tagMapInfo* pInfo  = NULL;
	m_mapMapInfo.ResetIterator();
	while( m_mapMapInfo.PeekNext(pInfo) )
	{
		MapMgr* pMapMgr = CreateMapMgr(pInfo);
		if( !P_VALID(pMapMgr) )
		{
			ASSERT(0);
			return FALSE;
		}

		m_mapMapMgr.Add(pInfo->dwID, pMapMgr);

		// 调用脚本
		Map* pMap = pMapMgr->GetSingleMap();
		if (P_VALID(pMap))
		{
			pMap->OnInit();
		}		
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// 载入地图基本信息
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapInfo(LPCTSTR szFileMapName, TObjRef<VarContainer>& pLogic)
{
	ASSERT( P_VALID(szFileMapName) );
	if( FALSE == P_VALID(szFileMapName) ) return FALSE;

	// 首先读取MB文件，获取需要的地图属性
	TCHAR szFullFileMapName[MAX_PATH] = {_T('\0')};
	_sntprintf_s(szFullFileMapName, MAX_PATH, MAX_PATH, _T("data\\map\\%s\\%s.mb"), szFileMapName, szFileMapName);

	CreateObj("MapFile", "VirtualFileSys");
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	HANDLE dwHandle = (HANDLE)pVFS->Open(szFullFileMapName);
	if( FALSE == GT_VALID(dwHandle) ) return FALSE;

	tagMapHeader header;
	pVFS->Read(&header, sizeof(header), (DWORD)dwHandle);

	tagMapInfo* pInfo = new tagMapInfo;
	if( FALSE == pInfo )
	{
		pVFS->Close((DWORD)dwHandle);
		KillObj("MapFile");
		return FALSE;
	}

	_sntprintf_s(pInfo->szMapFileName, X_SHORT_NAME, X_SHORT_NAME, _T("%s"), szFileMapName);

	// 加载地图导航点
	LoadMapWayPoint(&header, (DWORD)dwHandle, pInfo);
	// 加载地图触发器
	LoadMapTrigger(&header, (DWORD)dwHandle, pInfo);
	// 加载地图区域
	LoadMapArea(&header, (DWORD)dwHandle, pInfo);
	// 加载地图生物
	LoadMapCreature(&header, (DWORD)dwHandle, pInfo);
	// 加载地图刷怪点
	LoadMapSpawnPoint(&header, (DWORD)dwHandle, pInfo);
	// 加载地图场景特效
	LoadMapTriggerEffect(&header, (DWORD)dwHandle, pInfo);
	// 加载地图中的门
	LoadMapDoor(&header, (DWORD)dwHandle, pInfo);
	// 加载怪物刷新表 [4/19/2012 zhangzhihua]
	LoadMapMonsterGen(pInfo);


	// 付值pInfo的各项逻辑属性
	pInfo->dwID					=	m_pUtil->Crc32(szFileMapName);
	pInfo->nWidth				=	pLogic->GetInt(_T("width"), szFileMapName);
	pInfo->nHeight				=	pLogic->GetInt(_T("height"), szFileMapName);
	pInfo->nVisDist				=	pLogic->GetInt(_T("visdist"), szFileMapName, VIS_DIST);
	pInfo->eType				=	(EMapType)pLogic->GetDword(_T("type"), szFileMapName);
	pInfo->eNoarmalMapType		=	(ENormalMapType)pLogic->GetDword(_T("normal_map_type"), szFileMapName);
	pInfo->eInstanceRank		=	(EMapInstanceRank)pLogic->GetDword(_T("instance_rank"), szFileMapName, EMIR_NoLimit);
	pInfo->dwMaxNumLimit		=	pLogic->GetDword(_T("instance_num_limit"), szFileMapName, DEFAULT_MAX_INSTANCE_NUM);
	pInfo->dwWeatherID			=	GT_INVALID;
	_tcsncpy(pInfo->szMapFileName, szFileMapName, MCalTCharArrayNum(pInfo->szMapFileName) - 1);
	// Jason v2.1.2 飞升场景
	pInfo->bSoaringLimit		= pLogic->GetDword(_T("soar_limit"),szFileMapName,0);
	pInfo->bNoJump				= pLogic->GetDword(_T("nojump"),szFileMapName,0);
	// 显示地图名字和对应的ID (脚本里用到)
	IMSG(_T("map %s --> %u\r\n"), pInfo->szMapFileName, pInfo->dwID);

	// 复活点
	TCHAR szRebornWPName[X_SHORT_NAME];
	_tcsncpy(szRebornWPName, pLogic->GetString(_T("reborn_waypoint"), szFileMapName), MCalTCharArrayNum(szRebornWPName) - 1);

	DWORD dwRebornWPID = m_pUtil->Crc32(szRebornWPName);
	const tagMapWayPointInfoList* pWPList = pInfo->mapWayPointList.Peek(dwRebornWPID);
	if( P_VALID(pWPList) && pWPList->list.Size() > 0 )
	{
		tagWayPointInfo info = pWPList->list.Front();
		pInfo->vRebornPos = info.vPos;
	}
	else
	{
		IMSG(_T("Can't find the reborn waypoint of the map %s\r\n"), pInfo->szMapFileName);
	}

	m_mapMapInfo.Add(pInfo->dwID, pInfo);

	// 存储所有普通地图ID（非安全区）
	if(pInfo->eType == EMT_Normal && pInfo->eNoarmalMapType == ENMT_Normal)
	{
		if (pInfo->dwID != TObjRef<Util>()->Crc32(_T("j01")) 
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("j03"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("j04"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("s51"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("s52"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d01"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d02"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d03"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d04"))
			&&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d05"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d06"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d07"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d08"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d09"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d10"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("d11"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("s55_1"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("s55_2"))
			 &&pInfo->dwID != TObjRef<Util>()->Crc32(_T("s55_3")))
		{
			m_listNormalMapID.PushBack(pInfo->dwID);
		}
	}

	// 如果是副本，则现在就要生成一个副本的导航图模板
	if( EMT_Normal != pInfo->eType )
	{
		NavMap* pNav = LoadInstanceTemplate(pInfo);
		if( !P_VALID(pNav) )
		{
			ASSERT(0);
			return FALSE;
		}
		else
		{
			m_mapInstanceTemplate.Add(pInfo->dwID, pNav);
		}
	}

	pVFS->Close((DWORD)dwHandle);
	KillObj("MapFile");

	return TRUE;
}

//----------------------------------------------------------------------------
// 加载地图导航点
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapWayPoint(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumWayPoint <= 0 )
	{
		return TRUE;
	}

	// 根据导航点的偏移量来定位文件
	pVFS->Seek(handle, pHeader->dwWayPointOffset, GT_SEEK_SET);

	tagMapWayPoint temp;
	for(INT n = 0; n < pHeader->nNumWayPoint; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);
		DWORD dwWayPointID = m_pUtil->Crc32(temp.szName);

		tagMapWayPointInfoList* pList = pInfo->mapWayPointList.Peek(dwWayPointID);

		if( !P_VALID(pList) )
		{
			pList = new tagMapWayPointInfoList;
			pList->dwID = dwWayPointID;
			pInfo->mapWayPointList.Add(dwWayPointID, pList);
		}

		// 生成一个路径点
		tagWayPointInfo point;
		point.vPos = temp.vPos;
		point.vRange = temp.vRange;
		
		pList->list.PushBack(point);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// 加载地图触发器
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapTrigger(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumTrigger <= 0 )
	{
		return TRUE;
	}

	pVFS->Seek(handle, pHeader->dwTriggerOffset, GT_SEEK_SET);

	DWORD dwObjID = GT_INVALID;
	INT nRegionPointNum = 0;
	TCHAR szMapName[X_LONG_NAME] = {0};
	TCHAR szWayPointName[X_SHORT_NAME] = {0};
	vector<POINT> region;

	for(INT n = 0; n < pHeader->nNumTrigger; n++)
	{
		// 首先读取id并检查
		pVFS->Read(&dwObjID, sizeof(dwObjID), handle);

		tagMapTriggerInfo* pTrigger = pInfo->mapTrigger.Peek(dwObjID);
		ASSERT( !P_VALID(pTrigger) );

		pTrigger = new tagMapTriggerInfo;

		// 设置id和类型
		pTrigger->dwObjID = dwObjID;
		pVFS->Read(&pTrigger->eType, sizeof(pTrigger->eType), handle);

		// 读取区域点
		pVFS->Read(&nRegionPointNum, sizeof(nRegionPointNum), handle);
		ASSERT(nRegionPointNum > 0);

		region.clear();
		region.resize(nRegionPointNum);
		Vector3 vPos;
		POINT pt;
		for(INT m = 0; m < nRegionPointNum; m++)
		{
			pVFS->Read(&vPos, sizeof(vPos), handle);
			pt.x = LONG(vPos.x / FLOAT(TILE_SCALE));
			pt.y = LONG(vPos.z / FLOAT(TILE_SCALE));
			region[m] = pt;		// 格子坐标
		}
		pTrigger->hRgn = CreatePolygonRgn((POINT*)&region[0], nRegionPointNum, ALTERNATE);
		ASSERT(NULL != pTrigger->hRgn);

		// 读取包围盒
		pVFS->Read(&pTrigger->box.max, sizeof(pTrigger->box.max), handle);
		pVFS->Read(&pTrigger->box.min, sizeof(pTrigger->box.min), handle);
		// 读取高度
		pVFS->Read(&pTrigger->fHeight, sizeof(pTrigger->fHeight), handle);
		
		// 读取地图名称
		pVFS->Read(szMapName, sizeof(szMapName), handle);

		// 读取导航点名称
		pVFS->Read(szWayPointName, sizeof(szWayPointName), handle);

		// 转换成ID
		pTrigger->dwMapID = m_pUtil->Crc32(szMapName);
		pTrigger->dwWayPointID = m_pUtil->Crc32(szWayPointName);

		// 跳转一个位置（越过srciptName）
		pVFS->Seek(handle, sizeof(TCHAR) * X_SHORT_NAME, GT_SEEK_CUR);

		// 读取值
		pVFS->Read(&pTrigger->dwParam, sizeof(pTrigger->dwParam), handle);

		// 跳转到下一个位置（越过bFlag, bLock）
		pVFS->Seek(handle, sizeof(bool)+sizeof(bool), GT_SEEK_CUR);

		// 读取任务序列号
		pVFS->Read(&pTrigger->dwQuestSerial, sizeof(pTrigger->dwQuestSerial), handle);

		// 跳转到下一个位置（越过byReserved）
		pVFS->Seek(handle, sizeof(BYTE)*124, GT_SEEK_CUR);

		// 添加到列表中
		pInfo->mapTrigger.Add(pTrigger->dwObjID, pTrigger);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// 读取地图区域信息
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapArea(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumMapRect <= 0 )
	{
		return TRUE;
	}
	
	// 读取地图区域属性
	TCHAR szFullFileMapName[MAX_PATH] = {_T('\0')};
	_sntprintf_s(szFullFileMapName, MAX_PATH, MAX_PATH, _T("data\\map\\%s\\maparea.xml"), pInfo->szMapFileName);
	CreateObj("MapAreaContainer", "VarContainer");
	TObjRef<VarContainer> pMapArea = "MapAreaContainer";
	pMapArea->Load(NULL, szFullFileMapName, "id");

	pVFS->Seek(handle, pHeader->dwMapRectOffset, GT_SEEK_SET);

	DWORD dwObjID = GT_INVALID;
	EMapArea eType = EMA_Null;
	INT nRegionPointNum = 0;
	vector<POINT> region;

	for(INT n = 0; n < pHeader->nNumMapRect; n++)
	{
		pVFS->Read(&dwObjID, sizeof(dwObjID), handle);
		pVFS->Read(&eType, sizeof(eType), handle);

		// 显示区域服务器不需要加载，跳转到下一个
		if( eType == EMA_Null || eType == EMA_Display )
		{
			tstringstream tssID;
			tssID << dwObjID;
			if (!(BOOL)pMapArea->GetDword(_T("tax"), tssID.str().c_str(), 0))
			{
				pVFS->Read(&nRegionPointNum, sizeof(nRegionPointNum), handle);
				ASSERT( nRegionPointNum > 0 );

				INT nOffset = sizeof(Vector3)*nRegionPointNum + sizeof(AABBox) + sizeof(FLOAT)
					+ sizeof(bool) + sizeof(bool) + sizeof(BYTE)*128;

				pVFS->Seek(handle, nOffset, GT_SEEK_CUR);
				continue;
			}
		}

		tagMapAreaInfo* pArea = new tagMapAreaInfo;

		// 设置ID和类型
		pArea->dwObjID = dwObjID;
		pArea->eType = eType;

		// 载入区域点
		pVFS->Read(&nRegionPointNum, sizeof(nRegionPointNum), handle);
		ASSERT(nRegionPointNum > 0);
		
		region.clear();
		region.resize(nRegionPointNum);
		Vector3 vPos;
		POINT pt;
		for(INT m = 0; m < nRegionPointNum; m++)
		{
			pVFS->Read(&vPos, sizeof(vPos), handle);
			pt.x = LONG(vPos.x / FLOAT(TILE_SCALE));
			pt.y = LONG(vPos.z / FLOAT(TILE_SCALE));
			region[m] = pt;
		}
		pArea->hRgn = CreatePolygonRgn((POINT*)&region[0], nRegionPointNum, ALTERNATE);
		ASSERT(NULL != pArea->hRgn);

		// 载入包裹盒
		pVFS->Read(&pArea->box.max, sizeof(pArea->box.max), handle);
		pVFS->Read(&pArea->box.min, sizeof(pArea->box.min), handle);
		// 载入高度
		pVFS->Read(&pArea->fHeight, sizeof(pArea->fHeight), handle);

		// 跳转到下一个点
		pVFS->Seek(handle, sizeof(bool)+sizeof(bool)+sizeof(BYTE)*128, GT_SEEK_CUR);

		// 根据类型加入到不同的列表
		switch (eType)
		{
		case EMA_Safe:
			pInfo->mapSafeArea.Add(dwObjID, pArea);
			break;
		case EMA_PVP:
			pInfo->mapPVPArea.Add(dwObjID, pArea);
			break;
		case EMA_Stall:
			pInfo->mapStallArea.Add(dwObjID, pArea);
			break;
		case EMA_Prison:
			pInfo->mapPrisonArea.Add(dwObjID, pArea);
			break;
		case EMA_Script:
			pInfo->mapScriptArea.Add(dwObjID, pArea);
			break;
		case EMA_Display:
			pInfo->mapTaxArea.Add(dwObjID, pArea);
			break;
		default:
			SAFE_DEL(pArea);
			break;
		}
	}

	KillObj("MapAreaContainer");

	return TRUE;
}

//----------------------------------------------------------------------------
// 读取地图生物
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapCreature(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumNPC <= 0 )
	{
		return TRUE;
	}

	// 根据地图生物的偏移量来定位文件
	pVFS->Seek(handle, pHeader->dwNpcOffset, GT_SEEK_SET);

	tagMapNPC temp;
	for(INT n = 0; n < pHeader->nNumNPC; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapCreatureInfo* pCreature = pInfo->mapCreatureInfo.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pCreature) );

		pCreature = new tagMapCreatureInfo;
		pCreature->dwObjID = temp.dwObjID;
		pCreature->dwTypeID = temp.dwTypeID;
		pCreature->vPos = temp.vPos;
		pCreature->fYaw = 270 - temp.fYaw;
		pCreature->bCollide = temp.bCollide;
		pCreature->patrolList = pInfo->mapWayPointList.Peek(m_pUtil->Crc32(temp.szName));

		// 加入到列表中
		pInfo->mapCreatureInfo.Add(pCreature->dwObjID, pCreature);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// 读取刷怪点信息
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapSpawnPoint(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumSpawnPoint <= 0 )
	{
		return TRUE;
	}

	// 根据地图生物的偏移量来定位文件
	pVFS->Seek(handle, pHeader->dwSpawnPointOffset, GT_SEEK_SET);

	tagMapSpawnPoint temp;
	for(INT n = 0; n < pHeader->nNumSpawnPoint; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapSpawnPointInfo* pSpawnPoint = pInfo->mapSpawnPoint.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pSpawnPoint) );

		pSpawnPoint = new tagMapSpawnPointInfo;
		pSpawnPoint->dwObjID = temp.dwObjID;
		pSpawnPoint->dwSpawnPointID = temp.dwGroupID;
		pSpawnPoint->nLevelInc = temp.nLevelInc;
		pSpawnPoint->bCollide = temp.bCollide;
		pSpawnPoint->vPos = temp.vPos;
		
		// 加入到列表中
		pInfo->mapSpawnPoint.Add(pSpawnPoint->dwObjID, pSpawnPoint);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// 读取地图特效信息
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapTriggerEffect(tagMapHeader* pHeader, DWORD handle, tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumMapTriggerEffect <= 0 )
	{
		return TRUE;
	}

	// 根据地图生物的偏移量来定位文件
	pVFS->Seek(handle, pHeader->dwMapTriggerEffectOffset, GT_SEEK_SET);

	tagMapTriggerEffect temp;
	for(INT n = 0; n < pHeader->nNumMapTriggerEffect; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapTriggerEffect* pTriggerEffect = pInfo->mapTriggerEffect.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pTriggerEffect) );

		pTriggerEffect = new tagMapTriggerEffect;
		pTriggerEffect->dwObjID = temp.dwObjID;

		// 加入到列表中
		pInfo->mapTriggerEffect.Add(pTriggerEffect->dwObjID, pTriggerEffect);
	}

	return TRUE;
}

//----------------------------------------------------------------------------
// 读取地图门信息
//----------------------------------------------------------------------------
BOOL MapCreator::LoadMapDoor(tagMapHeader *pHeader, DWORD handle, tagMapInfo *pInfo)
{
	ASSERT( P_VALID(pHeader) && GT_VALID(handle) && P_VALID(pInfo) );
	TObjRef<VirtualFileSys> pVFS = "MapFile";
	ASSERT( P_VALID(pVFS) );

	if( pHeader->nNumMapDoor <= 0 )
	{
		return TRUE;
	}

	// 根据地图生物的偏移量来定位文件
	pVFS->Seek(handle, pHeader->dwMapDoorOffset, GT_SEEK_SET);

	tagMapDoor temp;
	for(INT n = 0; n < pHeader->nNumMapDoor; n++)
	{
		pVFS->Read(&temp, sizeof(temp), handle);

		tagMapDoor* pMapDoor = pInfo->mapDoor.Peek(temp.dwObjID);
		ASSERT( !P_VALID(pMapDoor) );

		pMapDoor = new tagMapDoor;
		pMapDoor->dwObjID			= temp.dwObjID;
		pMapDoor->dwTypeID			= temp.dwTypeID;
		pMapDoor->vPos				= temp.vPos;
		pMapDoor->fYaw				= temp.fYaw;
		pMapDoor->bInitState		= temp.bInitState;
		pMapDoor->bBlockingTileNPC	= temp.bBlockingTileNPC;
		memcpy(pMapDoor->fScale, temp.fScale, sizeof(temp.fScale));

		// 加入到列表中
		pInfo->mapDoor.Add(pMapDoor->dwObjID, pMapDoor);
	}

	return TRUE;
}

BOOL MapCreator::LoadMapMonsterGen(tagMapInfo* pInfo)
{
	// 读取地图刷怪信息
	TCHAR szFullFileMapName[MAX_PATH] = {_T('\0')};
	_sntprintf_s(szFullFileMapName, MAX_PATH, MAX_PATH, _T("data\\map\\%s\\monstergen.xml"), pInfo->szMapFileName);

	// 读入文件
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;
	CreateObj("MapMonsterGenContainer", "VarContainer");
	TObjRef<VarContainer> pVar = "MapMonsterGenContainer";
	pVar->Clear();
	if(!pVar->Load(NULL, szFullFileMapName, "id", &ListField))
	{
		// 地图没有刷怪信息 [4/18/2012 zhangzhihua]
		KillObj("MapMonsterGenContainer");
		return FALSE;
	}

	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		tagMapMonsterGen* pResNode = new tagMapMonsterGen;

		// 初始化
		pResNode->dwID					= (DWORD)pVar->GetDword(_T("id"), it->c_str(), 0);
		pResNode->dwWave				= (DWORD)pVar->GetDword(_T("wave"), it->c_str(), 0);
		pResNode->dwWaveRate			= (DWORD)pVar->GetDword(_T("waverate"), it->c_str(), 0);
		pResNode->eStartTimeType		= (EMapGenTimeRecordType)pVar->GetDword(_T("starttimetype"), it->c_str(), EMGTRT_AfterGen);
		pResNode->dwWaveTimeInterval	= (DWORD)pVar->GetDword(_T("wavetimeinterval"), it->c_str(), 0);
		pResNode->dwRewardMoney			= (DWORD)pVar->GetDword(_T("rewardmoney"), it->c_str(), 0);
		pResNode->dwEachTimeInterval	= (DWORD)pVar->GetDword(_T("eachtimeinterval"), it->c_str(), 0);
		pResNode->wNum					= (DWORD)pVar->GetDword(_T("num"), it->c_str(), 0);
		for (INT i = 0; i < MAX_MONSTER_GEN_NUM; i++)
		{
			TCHAR szTemp[X_LONG_STRING];
			wsprintf(szTemp, _T("monsterid%d"), i);
			pResNode->MonsterGenNode[i].dwMonsterID	= (DWORD)pVar->GetDword(szTemp, it->c_str(), 0);
			wsprintf(szTemp, _T("rate%d"), i);
			pResNode->MonsterGenNode[i].dwRate		= (DWORD)pVar->GetDword(szTemp, it->c_str(), 0);
			for (INT j = 0; j < MAX_MONSTER_GEN_LINE; j++)
			{
				wsprintf(szTemp, _T("monsterid%drate%d"), i, j);
				pResNode->MonsterGenNode[i].dwMonsterLine[j][EMonsterGen_Rate]	= (DWORD)pVar->GetDword(szTemp, it->c_str(), 0);

				wsprintf(szTemp, _T("monsterid%dline%d"), i, j);
				pResNode->MonsterGenNode[i].dwMonsterLine[j][EMonsterGen_Line]	= (DWORD)pVar->GetDword(szTemp, it->c_str(), 0);
			}
		}

		// 放入map中
		pInfo->mapMonsterGen.Add(pResNode->dwID, pResNode);
	}

	KillObj("MapMonsterGenContainer");
	return TRUE;
}

//----------------------------------------------------------------------------
// 预先生成副本地图的NavMap，当生成副本时直接复制，成功时，会加入m_mapInstanceTemplate
//----------------------------------------------------------------------------
NavMap* MapCreator::LoadInstanceTemplate(tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pInfo) );
	ASSERT( EMT_Normal != pInfo->eType );

	// 生成一个VFS
	IFS* pVfs = Kernel::Inst()->DefaultFS();
	NavMap* pNav = new NavMap;
	if( !P_VALID(pNav) )
	{
		return NULL;
	}

	pNav->LoadFromFile(pInfo->szMapFileName, pVfs, pVfs, NavMap::ELO_Collider | NavMap::ELO_NPCNavMap | NavMap::ELO_NPCNavGraph);
	Vector3 zoneSize(FLOAT_MAX, FLOAT_MAX, FLOAT_MAX);
	pNav->SetViewZoneSize(zoneSize);
	Vector3 zoneCenter(0, 0, 0);
	pNav->InvalidViewZone(zoneCenter);

	return pNav;
}

//----------------------------------------------------------------------------------
// 销毁
//----------------------------------------------------------------------------------
VOID MapCreator::Destroy()
{
	// 删除所有的MapMgr
	m_mapMapMgr.ResetIterator();
	MapMgr* pMapMgr = NULL;

	while( m_mapMapMgr.PeekNext(pMapMgr) )
	{
		SAFE_DEL(pMapMgr);
	}

	m_mapMapMgr.Clear();

	// 销毁进过副本的所有角色的ID列表
	DestroyRoleIDList();

	// 删除所有的副本模板
	m_mapInstanceTemplate.ResetIterator();
	NavMap* pNav = NULL;

	while( m_mapInstanceTemplate.PeekNext(pNav) )
	{
		SAFE_DEL(pNav);
	}

	m_mapInstanceTemplate.Clear();

	// 删除所有的地图基本信息
	m_mapMapInfo.ResetIterator();
	tagMapInfo* pInfo = NULL;

	while( m_mapMapInfo.PeekNext(pInfo) )
	{
		// 清空刷怪点
		tagMapSpawnPointInfo* pSpawnPoint = NULL;
		pInfo->mapSpawnPoint.ResetIterator();
		while( pInfo->mapSpawnPoint.PeekNext(pSpawnPoint) )
		{
			SAFE_DEL(pSpawnPoint);
		}
		pInfo->mapSpawnPoint.Clear();

		// 清空地图怪物
		tagMapCreatureInfo* pMapCreatureInfo = NULL;
		pInfo->mapCreatureInfo.ResetIterator();
		while( pInfo->mapCreatureInfo.PeekNext(pMapCreatureInfo) )
		{
			SAFE_DEL(pMapCreatureInfo);
		}
		pInfo->mapCreatureInfo.Clear();

		// 清空区域
		tagMapAreaInfo* pArea = NULL;

		pInfo->mapSafeArea.ResetIterator();
		while( pInfo->mapSafeArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapSafeArea.Clear();

		pInfo->mapPVPArea.ResetIterator();
		while( pInfo->mapPVPArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapPVPArea.Clear();

		pInfo->mapStallArea.ResetIterator();
		while( pInfo->mapStallArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapStallArea.Clear();

		pInfo->mapPrisonArea.ResetIterator();
		while( pInfo->mapPrisonArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapPrisonArea.Clear();

		pInfo->mapScriptArea.ResetIterator();
		while( pInfo->mapScriptArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapScriptArea.Clear();

		pInfo->mapTaxArea.ResetIterator();
		while( pInfo->mapTaxArea.PeekNext(pArea) )
		{
			SAFE_DEL(pArea);
		}
		pInfo->mapTaxArea.Clear();


		// 清空触发器
		tagMapTriggerInfo* pTrigger = NULL;
		pInfo->mapTrigger.ResetIterator();
		while( pInfo->mapTrigger.PeekNext(pTrigger) )
		{
			SAFE_DEL(pTrigger);
		}
		pInfo->mapTrigger.Clear();

		// 清空导航点
		tagMapWayPointInfoList* pList = NULL;
		pInfo->mapWayPointList.ResetIterator();
		while( pInfo->mapWayPointList.PeekNext(pList) )
		{
			SAFE_DEL(pList);
		}
		pInfo->mapWayPointList.Clear();

		// 清空场景特效
		tagMapTriggerEffect* pEffect = NULL;
		pInfo->mapTriggerEffect.ResetIterator();
		while (pInfo->mapTriggerEffect.PeekNext(pEffect))
		{
			SAFE_DEL(pEffect);
		}
		pInfo->mapTriggerEffect.Clear();

		// 清空门
		tagMapDoor* pDoor = NULL;
		pInfo->mapDoor.ResetIterator();
		while (pInfo->mapDoor.PeekNext(pDoor))
		{
			SAFE_DEL(pDoor);
		}
		pInfo->mapDoor.Clear();
		pInfo->mapMonsterGen.Clear();
		
		// 清空地图静态属性结构
		SAFE_DEL(pInfo);
	}

	m_mapMapInfo.Clear();

	m_listNormalMapID.Clear();

	// 删除出生点内存
	SAFE_DEL_ARRAY(m_pvBornPos);

	// 反注册所有的注册类
	UnRegisterAll();

	SAFE_DEL(m_pSemIdleCpu);

	m_mapOnlineInfo.ResetIterator();
	tagMapOnlineInfo* pMapOnlineInfo = NULL;
	while( m_mapOnlineInfo.PeekNext(pMapOnlineInfo) )
	{
		if( P_VALID( pMapOnlineInfo))
			delete pMapOnlineInfo;
	}
	m_mapOnlineInfo.Clear();
}

//--------------------------------------------------------------------------------
// 注册地图派生工厂
//--------------------------------------------------------------------------------
VOID MapCreator::RegisterAFMap()
{
	m_AFMap.Register(TWrap<Map>(),					EMT_Normal);
	m_AFMap.Register(TWrap<MapInstanceNormal>(),	EMT_Instance);
	m_AFMap.Register(TWrap<MapInstanceStable>(),	EMT_System);
	m_AFMap.Register(TWrap<MapInstanceScript>(),	EMT_ScriptCreate);
}

//--------------------------------------------------------------------------------
// 注册地图策略派生工厂
//--------------------------------------------------------------------------------
VOID MapCreator::RegisterAFMapRestrict()
{
	m_AFRestrict.Register(TWrap<MapRestrictNormal>(),		EMT_Normal);
	m_AFRestrict.Register(TWrap<MapRestrictInstance>(),		EMT_Instance);
	m_AFRestrict.Register(TWrap<MapRestrictStable>(),		EMT_System);
	m_AFRestrict.Register(TWrap<MapRestrictScript>(),		EMT_ScriptCreate);
	//m_AFRestrict.Register(TWrap<MapRestrictPVP>,		EMT_PVP);
}

//--------------------------------------------------------------------------------
// 反注册接口
//--------------------------------------------------------------------------------
VOID MapCreator::UnRegisterAll()
{
	m_AFMap.Unregister(TWrap<Map>(),				EMT_Normal);
	m_AFMap.Unregister(TWrap<MapInstanceNormal>(),	EMT_Instance);
	m_AFMap.Unregister(TWrap<MapInstanceStable>(),	EMT_System);
	m_AFMap.Unregister(TWrap<MapInstanceScript>(),	EMT_ScriptCreate);

	m_AFRestrict.Unregister(TWrap<MapRestrictNormal>(),		EMT_Normal);
	m_AFRestrict.Unregister(TWrap<MapRestrictInstance>(),	EMT_Instance);
	m_AFRestrict.Unregister(TWrap<MapRestrictStable>(),		EMT_System);
	m_AFRestrict.Unregister(TWrap<MapRestrictScript>(),		EMT_ScriptCreate);
	//m_AFRestrict.Unregister(TWrap<MapRestrictPVP>,			EMT_PVP);

}

//--------------------------------------------------------------------------------
// 生成普通地图
//--------------------------------------------------------------------------------
Map* MapCreator::CreateAFMap(const tagMapInfo* pInfo)
{
	if( !P_VALID(pInfo) || EMT_Normal != pInfo->eType ) return NULL;

	return m_AFMap.Create(pInfo->eType);
}

//--------------------------------------------------------------------------------
// 生成副本地图
//--------------------------------------------------------------------------------
MapInstance* MapCreator::CreateAFMapInstance(const tagMapInfo* pInfo)
{
	if( !P_VALID(pInfo) || EMT_Normal == pInfo->eType ) return NULL;

	return static_cast<MapInstance*>(m_AFMap.Create(pInfo->eType));
}

//---------------------------------------------------------------------------------
// 释放地图
//---------------------------------------------------------------------------------
VOID MapCreator::DestroyAFMap(Map* pMap)
{
	if( !P_VALID(pMap) ) return;

	const tagMapInfo* pInfo = pMap->GetMapInfo();
	if( !P_VALID(pInfo) ) return;

	m_AFMap.Destroy(pMap, pInfo->eType);
}

//--------------------------------------------------------------------------------
// 生成地图策略
//--------------------------------------------------------------------------------
MapRestrict* MapCreator::CreateAFMapRestrict(const tagMapInfo* pInfo)
{
	if( !P_VALID(pInfo) ) return NULL;

	return m_AFRestrict.Create(pInfo->eType);
}

//--------------------------------------------------------------------------------
// 释放地图策略
//--------------------------------------------------------------------------------
VOID MapCreator::DestroyAFMapRestrict(MapRestrict* pRestrict)
{
	if( !P_VALID(pRestrict) ) return;

	const tagMapInfo* pInfo = pRestrict->GetMapInfo();
	if( !P_VALID(pInfo) ) return;

	m_AFRestrict.Destroy(pRestrict, pInfo->eType);
}

//--------------------------------------------------------------------------------
// 停止所有地图管理器线程
//--------------------------------------------------------------------------------
VOID MapCreator::StopAllMapMgr()
{
	MapMgr::StopThread();
	StartAllMapMgr();	// 唤醒所有线程，让他们自行退出，因为上面已经设置了结束标志

	m_mapMapMgr.ResetIterator();
	MapMgr* pMapMgr = NULL;

	while( m_mapMapMgr.PeekNext(pMapMgr) )
	{
		tstring& strThreadName = pMapMgr->GetThreadName();
		m_pThread->WaitForThreadDestroy(strThreadName.c_str(), INFINITE);
	}
}

//------------------------------------------------------------------------------
// 生成一个地图对应的MapMgr，启动MapMgr线程
//------------------------------------------------------------------------------
MapMgr* MapCreator::CreateMapMgr(tagMapInfo* pInfo)
{
	ASSERT( P_VALID(pInfo) );

	MapMgr* pMapMgr = new MapMgr;
	if( !P_VALID(pMapMgr) )
		return NULL;

	if( FALSE == pMapMgr->Init(pInfo) )
	{
		SAFE_DEL(pMapMgr);
		return NULL;
	}

	return pMapMgr;
}

//----------------------------------------------------------------------------------
// 根据地图ID和副本ID确定一个唯一地图
//----------------------------------------------------------------------------------
Map* MapCreator::GetMap(DWORD dwMapID, DWORD dwInstance)
{
	MapMgr* pMapMgr = GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr) ) return NULL;

	Map* pMap = NULL;

	if( pMapMgr->IsNormalMap() )	pMap = pMapMgr->GetSingleMap();
	else							pMap = pMapMgr->GetInstance(dwInstance);

	return pMap;
}

//------------------------------------------------------------------------------------------
// 地图生成器的更新
//------------------------------------------------------------------------------------------
VOID MapCreator::Update()
{
	CHECK_TIME();

	// 开启所有地图线程
	StartAllMapMgr();

	// 等待所有地图线程本次更新完毕
	WaitAllMapMgrEnd();

	// 更新完毕之后的后续操作（串行操作）
	UpdateAllDelayedStuff();

	// Jason 2010-7-17 v1.0.0 攻防预警信息更新，一定要在做后做
	DealAllAttackDefenseInfo();
}

//-------------------------------------------------------------------------------------------
// 每个Tick所有地图线程暂停后的操作，一定要满足顺序，这三个调用的顺序不能颠倒
//-------------------------------------------------------------------------------------------
VOID MapCreator::UpdateAllDelayedStuff()
{
	//CHECK_SPEC_BEGIN(NULL,100)
	// 保存并退出所有已经不在线的玩家
	LogoutAllRemovedRolePerTick();

	// 处理所有要返回选人界面的玩家
	DealAllReturnCharacterRole();

	// 处理所有要切换地图的玩家
	DealAllChangeMapRole();

	// 处理所有要切换到确切地图的玩家
	DealAllChangeMapInstRole();

	// 删除所有本Tick要删除的副本
	DealAllDestroiedInstance();

	//CHECK_SPEC_END
}

//--------------------------------------------------------------------------------------------
// 保存并退出所有已经不在线的玩家
//--------------------------------------------------------------------------------------------
VOID MapCreator::LogoutAllRemovedRolePerTick()
{
	//CHECK_SPEC_BEGIN("LogoutAllRemovedRolePerTick",100)
	DWORD dwRoleID = m_listLogOutRole.PopFront();

	while( P_VALID(dwRoleID) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if( !P_VALID(pRole) )
		{
			dwRoleID = m_listLogOutRole.PopFront();
			continue;
		}
		pRole->CheckAllEquip();

		// 去掉玩家和交易目标的交易状态
			pRole->ClenupExchState();
			pRole->ClenupPetExchState();
		// 如果角色身上有公会战buff，要删掉
		g_guildWarDeclareMgr.OnRoleLogout(pRole);

		// 宝箱强制掉落(夺宝奇兵)
		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if (P_VALID(pRoleScript))
		{
			pRoleScript->ForceLootChase(pRole);
		}

		Map* pSrcMap = pRole->GetMap();
		if (pSrcMap->IsWar())
		{
			WarRoleMgr* pWarRoleMgr = pSrcMap->GetWarRoleMgr();
			if (P_VALID(pWarRoleMgr))
			{
				pWarRoleMgr->OnRoleLeaveWar(pRole);
			}
		}

		// 找到该玩家登出时所在的地图
		Map* pMap = pRole->GetMap();
		if( P_VALID(pMap) )
		{
			pMap->RoleLogOut(pRole->GetID());
		}

		//去掉稀有领域特效buff
		pRole->RemoveAreaBuff();//modify by xp

		// 保存玩家
		pRole->SaveToDB(TRUE);

		// 清掉Session
		PlayerSession* pSession = pRole->GetSession();
		if( P_VALID(pSession) )
		{
			pSession->SyncOnlineNum(pRole, TRUE);

			// 给登陆服务器发消息通知防沉迷停止计时（越南版本防沉迷）
			SendFatigueTimer(pRole, 0);

			// Jason 2010-1-15 v1.3.2 离线登记
			pSession->RegisterUserOffline(pRole);

			// IMSG(_T("player logout in mapcreator, sessionid=%u, internalid=%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex());
			g_worldSession.RemoveSession(pSession->GetSessionID());
		}
			
		g_roleMgr.DeleteRole(pRole->GetID());
		g_loginSession.SendPlayerLogout(pSession->GetSessionID());
		g_worldSession.PlayerLogout();
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);

		// 得到下一个玩家
		dwRoleID = m_listLogOutRole.PopFront();
	}
	//CHECK_SPEC_END
}

//---------------------------------------------------------------------------------------------
// 处理所有本tick要切换地图的玩家
//---------------------------------------------------------------------------------------------
VOID MapCreator::DealAllChangeMapRole()
{
	//CHECK_TIME_WITH_ALLPARAM_EX("DealAllChangeMapRole",100,TRUE)
	tagChangeMapInfo info = m_listChangeMapRole.PopFront();

	while( P_VALID(info.dwRoleID) )
	{
		BOOL bEnterInstMsg = FALSE;
		INT nAddWearyValue = 0;
		Role* pRole = g_roleMgr.GetRolePtrByID(info.dwRoleID);
		if( !P_VALID(pRole) )
		{
			info = m_listChangeMapRole.PopFront();
			continue;
		}

		MapMgr* pDestMapMgr = g_mapCreator.GetMapMgr(info.dwDestMapID);
		if( !P_VALID(pDestMapMgr) )
		{
			info = m_listChangeMapRole.PopFront();
			continue;
		}

		Map* pDestMap = pDestMapMgr->CanEnter(pRole, info.dwMisc);
		if( !P_VALID(pDestMap) )
		{
			info = m_listChangeMapRole.PopFront();
			continue;
		}

		// 不是普通地图，应该是副本，检查副本是否有疲劳限制
		if (pDestMap->GetMapType() != EMT_Normal)
		{
			EWearyCheckResult eWearyCheckResult = CheckRoleWearyValue(info.dwRoleID, pDestMap->GetMapID(), pDestMap->GetInstanceID(), &nAddWearyValue);
			if (eWearyCheckResult == EWCR_Can_Not_Enter)	// 不能进入副本
			{
				info = m_listChangeMapRole.PopFront();
				continue;
			}
			else if (eWearyCheckResult == EWCR_Can_Enter)	// 可以进入副本
			{
				bEnterInstMsg = TRUE;
			}
		}

		// 宝箱强制掉落(夺宝奇兵)
		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if (P_VALID(pRoleScript))
		{
			pRoleScript->ForceLootChase(pRole);
		}

		// 从原地图移走
		Map* pSrcMap = pRole->GetMap();
		BOOL bChgMap = (info.dwDestMapID != pSrcMap->GetMapID());
		if( P_VALID(pSrcMap) ) pSrcMap->RoleLeaveMap(pRole->GetID(), bChgMap);

		// 加入到新地图
		pRole->SetMapID(info.dwDestMapID);
		pRole->GetMoveData().Reset(info.vPos.x, info.vPos.y, info.vPos.z, pRole->GetMoveData().m_vFace.x, pRole->GetMoveData().m_vFace.y, pRole->GetMoveData().m_vFace.z);

		pRole->CheckAllEquip(info.dwDestMapID);

		pDestMap->AddRole(pRole);

		if (pSrcMap->IsWar() && !pDestMap->IsWar())
		{
			WarRoleMgr* pWarRoleMgr = pSrcMap->GetWarRoleMgr();
			if (P_VALID(pWarRoleMgr))
			{
				pWarRoleMgr->OnRoleLeaveWar(pRole);
			}
		}
		else if (!pSrcMap->IsWar() && pDestMap->IsWar())
		{
			WarRoleMgr* pWarRoleMgr = pDestMap->GetWarRoleMgr();
			if (P_VALID(pWarRoleMgr))
			{
				pWarRoleMgr->OnRoleEnterWar(pRole);
			}
		}

		if (bEnterInstMsg)
		{
			// 发送进入副本消息
			tagNS_EnterInstance send;
			send.dwErrorCode = E_Success;
			send.dwTimeLimit = (pDestMap->GetMapType() == EMT_Instance)?((static_cast<MapInstanceNormal*>(pDestMap))->CalTimeLimit()):GT_INVALID;
			send.nInstanceWearyValue = nAddWearyValue;
			send.nRoleWearyValue = pRole->GetAttValue(ERA_Weary);
			send.dwMapID = info.dwDestMapID;
			pRole->SendMessage(&send, send.dwSize);

			// 金牌网吧，特殊副本buff
			if( g_VipNetBarMgr.IsOpen() )
				g_VipNetBarMgr.AddInstBuff(pRole);
		}
		// 取下一个玩家
		info = m_listChangeMapRole.PopFront();
	}
}

//---------------------------------------------------------------------------------------------
// 处理所有本tick要切换地图的玩家
//---------------------------------------------------------------------------------------------
VOID MapCreator::DealAllChangeMapInstRole()
{
	//CHECK_TIME_WITH_ALLPARAM_EX("DealAllChangeMapInstRole",100,TRUE)
	tagChangeMapInstInfo info = m_listChangeMapInstRole.PopFront();

	while( P_VALID(info.dwRoleID) )
	{
		BOOL bEnterInstMsg = FALSE;
		INT nAddWearyValue = 0;
		Role* pRole = g_roleMgr.GetRolePtrByID(info.dwRoleID);
		if( !P_VALID(pRole) )
		{
			info = m_listChangeMapInstRole.PopFront();
			continue;
		}		

		Map* pDestMap = g_mapCreator.GetMap(info.dwDestMapID, info.dwDestMapInstID);
		if( !P_VALID(pDestMap) )
		{
			info = m_listChangeMapInstRole.PopFront();
			continue;
		}

		// 检查副本是否有疲劳限制
		if (pDestMap->GetMapType() != EMT_Normal)
		{
			EWearyCheckResult eWearyCheckResult =   CheckRoleWearyValue(info.dwRoleID, pDestMap->GetMapID(), pDestMap->GetInstanceID(), &nAddWearyValue);
			if (eWearyCheckResult == EWCR_Can_Not_Enter)	// 不能进入副本
			{
				info = m_listChangeMapInstRole.PopFront();
				continue;
			}
			else if (eWearyCheckResult == EWCR_Can_Enter)	// 可以进入副本
			{
				bEnterInstMsg = TRUE;
			}
		}

		// 宝箱强制掉落(夺宝奇兵)
		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if (P_VALID(pRoleScript))
		{
			pRoleScript->ForceLootChase(pRole);
		}

		// 从原地图移走
		Map* pSrcMap = pRole->GetMap();
		BOOL bChgMap = (info.dwDestMapID != pSrcMap->GetMapID());
		if( P_VALID(pSrcMap) ) pSrcMap->RoleLeaveMap(pRole->GetID(), bChgMap);

		// 加入到新地图
		pRole->SetMapID(info.dwDestMapID);
		pRole->GetMoveData().Reset(info.vPos.x, info.vPos.y, info.vPos.z, pRole->GetMoveData().m_vFace.x, pRole->GetMoveData().m_vFace.y, pRole->GetMoveData().m_vFace.z);

		pDestMap->AddRole(pRole);
		
		if (bEnterInstMsg)
		{
			// 发送进入副本消息
			tagNS_EnterInstance send;
			send.dwErrorCode = E_Success;
			send.dwTimeLimit = (pDestMap->GetMapType() == EMT_Instance)?((static_cast<MapInstanceNormal*>(pDestMap))->CalTimeLimit()):GT_INVALID;
			send.nInstanceWearyValue = nAddWearyValue;
			send.nRoleWearyValue = pRole->GetAttValue(ERA_Weary);
			send.dwMapID = info.dwDestMapID;
			pRole->SendMessage(&send, send.dwSize);

			// 金牌网吧，特殊副本buff
			if( g_VipNetBarMgr.IsOpen() )
				g_VipNetBarMgr.AddInstBuff(pRole);
		}

		// 取下一个玩家
		info = m_listChangeMapInstRole.PopFront();
	}
}


//---------------------------------------------------------------------------------------------
// 处理所有本tick要回到选人界面的玩家
//---------------------------------------------------------------------------------------------
VOID MapCreator::DealAllReturnCharacterRole()
{
	//CHECK_TIME_WITH_ALLPARAM_EX("DealAllReturnCharacterRole",100,TRUE)
	DWORD dwRoleID = m_listReturnCharacterRole.PopFront();

	while( P_VALID(dwRoleID) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);

		if( P_VALID(pRole) )
		{
			pRole->CheckAllEquip();

			// 如果角色身上有公会战buff，要删掉
			g_guildWarDeclareMgr.OnRoleLogout(pRole);
			// 去掉玩家和交易目标的交易状态
			pRole->ClenupExchState();
			pRole->ClenupPetExchState();

			// 宝箱强制掉落(夺宝奇兵)
			const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pRoleScript))
			{
				pRoleScript->ForceLootChase(pRole);
			}

			Map* pSrcMap = pRole->GetMap();
			if (pSrcMap->IsWar())
			{
				WarRoleMgr* pWarRoleMgr = pSrcMap->GetWarRoleMgr();
				if (P_VALID(pWarRoleMgr))
				{
					pWarRoleMgr->OnRoleLeaveWar(pRole);
				}
			}

			pRole->GetMap()->RoleLeaveMap(pRole->GetID(), FALSE, TRUE);

			// 给登陆服务器发消息通知防沉迷停止计时（越南版本防沉迷）
			SendFatigueTimer(pRole, 0);

			// 找到Session，重置各种设置
			PlayerSession* pSession = pRole->GetSession();

			// 分类统计各个媒体的在线人数
			if (P_VALID(pSession))
				g_worldSession.OnRoleLogout(pSession->GetDistributionID());

			// 放入到选人界面中
			g_worldSession.AddGlobalSession(pSession);

			//去掉稀有领域特效buff
			pRole->RemoveAreaBuff();//modify by xp

			//去掉结拜buff
			pRole->CancelBrotherProfit();
			// 保存角色，并删除角色
			pRole->SaveToDB(TRUE);
			// 防止玩家退到选人界面通过选人界面刷钱
			pSession->Refresh();
			g_roleMgr.DeleteRole(pRole->GetID());
		}

		// 得到一个新的玩家
		dwRoleID = m_listReturnCharacterRole.PopFront();
	}
}

//---------------------------------------------------------------------------------------------
// 删除所有本tick要删除的副本
//---------------------------------------------------------------------------------------------
VOID MapCreator::DealAllDestroiedInstance()
{
	//CHECK_TIME_WITH_ALLPARAM_EX("DealAllDestroiedInstance",100,TRUE)
	tagInstanceDestroyInfo info = m_listDestroyInstance.PopFront();

	while( P_VALID(info.dwMapID) )
	{
		// 找到MapMgr
		MapMgr* pMapMgr = GetMapMgr(info.dwMapID);
		if( !P_VALID(pMapMgr) )
		{
			info = m_listDestroyInstance.PopFront();
			continue;
		}

		// 找到副本
		MapInstance* pInstance = pMapMgr->GetInstance(info.dwInstanceID);
		if( !P_VALID(pInstance) )
		{
			info = m_listDestroyInstance.PopFront();
			continue;
		}

		// 删除副本ID对应的角色ID列表
		ListRoleID* pListRoleID = NULL;
		pListRoleID = m_mapRoleIDList.Peek(info.dwInstanceID);
		if (P_VALID(pListRoleID))
		{
			if (P_VALID(pListRoleID))
			{
				pListRoleID->Clear();
				SAFE_DEL(pListRoleID);
			}
			m_mapRoleIDList.Erase(info.dwInstanceID);
		}

		// 删除副本
		pMapMgr->DestroyInstance(pInstance);

		// 删除下一个
		info = m_listDestroyInstance.PopFront();
	}
}

//-------------------------------------------------------------------------------------------------
// 外部指定某个副本删除
//-------------------------------------------------------------------------------------------------
VOID MapCreator::SetInstanceDelete(DWORD dwMapID, DWORD dwInstanceID)
{
	MapMgr* pMapMgr = GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr) ) return;

	MapInstance* pInstance = pMapMgr->GetInstance(dwInstanceID);
	if( !P_VALID(pInstance) ) return;

	pInstance->SetDelete();
}

//-------------------------------------------------------------------------------------------------
// 找到某个地图接管玩家
//-------------------------------------------------------------------------------------------------
BOOL MapCreator::TakeOverRoleWhenOnline(Role* pRole)
{
	if( !P_VALID(pRole) ) return FALSE;

	DWORD dwMapID = pRole->GetMapID();		// 玩家离线时的地图ID
	Vector3 vPos = pRole->GetCurPos();		// 玩家离线时的坐标

	MapMgr* pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
	if( !P_VALID(pMapMgr) )
	{
		ILOG->Write(_T("Can not find a map when player online, roleid=%u, mapid=%u\n"), pRole->GetID(), dwMapID);
		return FALSE;
	}

	Map* pMap = pMapMgr->CanTakeOverWhenOnline(pRole, dwMapID, vPos);

	while( !P_VALID(pMap) )
	{
		pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
		if (P_VALID(pMapMgr))
		{
			pMap = pMapMgr->CanTakeOverWhenOnline(pRole, dwMapID, vPos);

			if( !P_VALID(pMap) && dwMapID == pMapMgr->GetMapInfo()->dwID )	// 不能接管，但出口地图还是它自己，说明有错误
			{
				ILOG->Write(_T("find a duplicated map when player online, roleid=%u, mapid=%u\n"), pRole->GetID(), dwMapID);
				break;
			}
		}
		else
		{
			break;
		}
	}

	// 给登陆服务器发消息通知防沉迷开始计时（越南版本防沉迷）
	SendFatigueTimer(pRole, 1);

	if( P_VALID(pMap) )
	{
		// 找到了一个目标地图
		pRole->SetMapID(dwMapID);
		pRole->GetMoveData().Reset(vPos.x, vPos.y, vPos.z, pRole->GetFaceTo().x, pRole->GetFaceTo().y, pRole->GetFaceTo().z);

		pMap->AddRole(pRole);

		return TRUE;
	}
	else
	{
		// 没有找到目标地图，则传回出生点
		dwMapID = g_mapCreator.GetBornMapID();
		vPos = g_mapCreator.RandGetOneBornPos();

		pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
		pMap = pMapMgr->CanTakeOverWhenOnline(pRole, dwMapID, vPos);

		if( !P_VALID(pMap) )
		{
			ILOG->Write(_T("Critical Error, role can not enter the born map!!!, roleid=%u\n"), pRole->GetID());
			return FALSE;
		}

		pRole->SetMapID(dwMapID);
		pRole->GetMoveData().Reset(vPos.x, vPos.y, vPos.z, pRole->GetFaceTo().x, pRole->GetFaceTo().y, pRole->GetFaceTo().z);

		pMap->AddRole(pRole);

		return TRUE;
	}
}

const Vector3 MapCreator::GetReBornPoint( DWORD dwRebornMapID )
{
	Vector3 vRt;
	vRt.x = vRt.y = vRt.z = GT_INVALID;
	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(dwRebornMapID);
	if(P_VALID(pMapInfo))
	{
		vRt = pMapInfo->vRebornPos;
	}
	else
	{
		// 出生地图没找到
		ASSERT(0);
	}

	return vRt;
}

//-------------------------------------------------------------------------------------------------
// 是否可以创建副本
//-------------------------------------------------------------------------------------------------
BOOL MapCreator::CanCreateInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr)
{
	if( EMT_Normal == pInfo->eType ) return FALSE;
	if( pInfo->eInstanceRank < EMIR_NoLimit || pInfo->eInstanceRank >= EMIR_End ) return FALSE;

	if( EMIR_NoLimit == pInfo->eInstanceRank ) return TRUE;

	if(pMapMgr->GetCurInstanceNum() >= pInfo->dwMaxNumLimit )	return FALSE;

	// 计算一下加权总和是否大于上限
	if( m_nInstanceCoefNumCur + INSTANCE_COEF[pInfo->eInstanceRank] > m_nInstanceCoefNumLimit ) return FALSE;

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
// 增加一个副本
//-------------------------------------------------------------------------------------------------
VOID MapCreator::AddInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr)
{
	if( EMT_Normal == pInfo->eType ) return;
	if( pInfo->eInstanceRank < EMIR_NoLimit || pInfo->eInstanceRank >= EMIR_End ) return;

	pMapMgr->ChangeInstanceNum(1);
	m_nInstanceNumCur[pInfo->eInstanceRank] += 1;
	m_nInstanceCoefNumCur += INSTANCE_COEF[pInfo->eInstanceRank];
}

//-------------------------------------------------------------------------------------------------
// 移除一个部分
//-------------------------------------------------------------------------------------------------
VOID MapCreator::RemoveInstance(const tagMapInfo* pInfo, MapMgr* pMapMgr)
{
	if( EMT_Normal == pInfo->eType ) return;
	if( pInfo->eInstanceRank < EMIR_NoLimit || pInfo->eInstanceRank >= EMIR_End ) return;

	pMapMgr->ChangeInstanceNum(-1);
	m_nInstanceNumCur[pInfo->eInstanceRank] -= 1;
	m_nInstanceCoefNumCur -= INSTANCE_COEF[pInfo->eInstanceRank];
}

VOID MapCreator::DestroyRoleIDList()
{
	m_mapRoleIDList.ResetIterator();
	ListRoleID* pListRoleID = NULL;
	while (m_mapRoleIDList.PeekNext(pListRoleID))
	{
		if (P_VALID(pListRoleID))
		{
			pListRoleID->Clear();
			SAFE_DEL(pListRoleID);
		}
	}
	m_mapRoleIDList.Clear();
}

// 进入副本前，对角色进行疲劳度检查；
EWearyCheckResult MapCreator::CheckRoleWearyValue(DWORD dwRoleID, DWORD dwInstanceMapID, DWORD dwInstanceID, INT* pnAddWearyValue)
{
	Role * pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))	return EWCR_Can_Not_Enter;

	// 检查副本是否有疲劳限制
	const tagInstance* pInstanceProto = g_attRes.GetInstanceProto(dwInstanceMapID);
	if (!P_VALID(pInstanceProto))	return EWCR_NoWearyCheck;

	INT	nWearyValue, nWearyAdded = 0;
	INT nWearyAddValue = pInstanceProto->nWearyValue;
	// Jason 2010-11-23 v1.2.2增加buff对进入副本的疲劳值影响
// 	if (g_VipNetBarMgr.IsOpen())
// 	{
		INT nInstWeary = pRole->GetAttModValue(ERA_InstanceWearyEffect);
		if (nInstWeary != 0)
		{
			nWearyAddValue += nInstWeary;
		}
		else
		{
			INT nInstWearyPct = pRole->GetAttModValuePct(ERA_InstanceWearyEffect);
			if (nInstWearyPct != 0)
				nWearyAddValue += (INT)(( (FLOAT)nWearyAddValue * nInstWearyPct ) / 10000);
		}
// 	}

	if (nWearyAddValue <= 0)
	{
		if (P_VALID(pnAddWearyValue))
		{
			*pnAddWearyValue = 0;
		}
		return EWCR_Can_Enter;
	}
	// 检查角色是否有进入过这个副本，如果进入过不再进行疲劳值积累,直接进入副本地图
	ListRoleID* pListRoleID = m_mapRoleIDList.Peek(dwInstanceID);
	if (P_VALID(pListRoleID))
	{
		if (pListRoleID->IsExist(dwRoleID))
		{
			if (P_VALID(pnAddWearyValue))
			{
				*pnAddWearyValue = 0;
			}
			return EWCR_Can_Enter;
		}
	}
	else
	{
		if (P_VALID(pnAddWearyValue))
		{
			*pnAddWearyValue = 0;
		}
		return EWCR_Can_Enter;
	}
	
	// 角色没有进入过这个副本，对角色的疲劳值进行预增值；如果预增值后疲劳值大于100，提示错误信息
	nWearyValue = pRole->GetAttValue(ERA_Weary);
	nWearyAdded = nWearyValue + nWearyAddValue;
	if (nWearyAdded > pRole->GetWearyUpperLimit())
	{
		// 提示“您今日过于疲劳，无法再进入该副本！可服用药品回复疲劳度，或尝试选择积累疲劳少的副本进行游戏！”
		tagNS_EnterInstance send;
		send.dwTimeLimit = GT_INVALID;
		send.dwErrorCode = E_Instance_Too_Weary;
		pRole->SendMessage(&send, send.dwSize);
		return EWCR_Can_Not_Enter;
	}

	// 否则，先将角色ID加入进过副本的所有角色的ID列表、增加疲劳值并继续将角色传入副本地图
	pListRoleID->PushBack(dwRoleID);
	pRole->SetAttValue(ERA_Weary, nWearyAdded);

	if (P_VALID(pnAddWearyValue))
	{
		*pnAddWearyValue = nWearyAddValue;
	}

	return EWCR_Can_Enter;
}

// 创建一个进入过这个副本的角色ID的列表
VOID MapCreator::AddRoleIDList2Map(DWORD dwInstanceID)
{
	ListRoleID* pListRoleID = new ListRoleID;
	if (!P_VALID(pListRoleID))
	{
		return;
	}
	pListRoleID->Clear();
	pListRoleID->ResetIterator();
	m_mapRoleIDList.Add(dwInstanceID, pListRoleID);
}

// 给登陆服务器发消息通知防沉迷开始或停止计时（越南版本防沉迷）
VOID MapCreator::SendFatigueTimer(Role* pRole, BYTE byEnterMap)
{
	PlayerSession* pSession = pRole->GetSession();
	if (P_VALID(pSession))
	{
		tagNWL_FatigueTimer send;
		send.byEnterMap = byEnterMap;
		send.dwAccountID = pSession->GetSessionID();
		g_loginSession.Send(&send, send.dwSize);
	}
}

DWORD MapCreator::GetRandNormalMapID()
{
	DWORD dwMapID = 0;

	m_listNormalMapID.RandPeek(dwMapID);
	
	return dwMapID;
}

VOID MapCreator::DealAllAttackDefenseInfo(VOID)
{
	CHECK_SPEC_BEGIN(NULL,100)
	MapMgr * pMgr = NULL;
	for( MapMgrMap::TMapIterator i = m_mapMapMgr.Begin();m_mapMapMgr.PeekNext(i,pMgr); )
	{
		if( P_VALID(pMgr) )
			pMgr->DealAttackDefenseRelationship();
	}
	CHECK_SPEC_END
}

////-----------------------------------------------------------------------------------------------
//// 创建副本
////---------------------------------------------------------------------------------------------
//BOOL MapCreator::CreateInstance(DWORD dwCreateID, DWORD dwTeamID, MapMgr *pDestMapMgr, DWORD dwDestMapID, EInstanceHardMode eInstanceHardMode)
//{
//	Map *pInstance = new MapInstanceNormal(m_dwInstanceIDGen, dwCreateID, dwTeamID, eInstanceHardMode);
//	if( !P_VALID(pInstance) )
//	{
//		return FALSE;
//	}
//
//	tagMapInfo* pDestMapInfo = m_mapMapInfo.Peek(dwDestMapID);
//	if(FALSE == pInstance->Init(pDestMapInfo))
//	{
//		SAFE_DEL(pInstance);
//		return FALSE;
//	}
//
//	pDestMapMgr->AddInstance(m_dwInstanceIDGen, pInstance);
//
//	return TRUE;
//}
//
////---------------------------------------------------------------------------------------------
//// 进入副本
////---------------------------------------------------------------------------------------------
//BOOL MapCreator::EnterInstance(Role *pRole, MapMgr* pMapMgr, tagChangeMapInfo& info)
//{
//	DWORD dwErrorCode = E_Success;
//
//	const tagInstance *pInstance = g_attRes.GetInstanceProto(info.dwDestMapID);
//	if(!P_VALID(pInstance))
//	{
//		IMSG(_T("Can't find Instance Proto %u\r\n"), info.dwDestMapID);
//
//		return FALSE;
//	}
//
//	// 小队进入
//	if(pRole->GetTeamID() != GT_INVALID)
//	{
//		const CTeam *pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
//		if(!P_VALID(pTeam))
//			return FALSE;
//
//		DWORD dwInstanceID = pTeam->GetOwnInstanceID();
//
//		// 创建副本
//		if(dwInstanceID == GT_INVALID)
//		{
//			dwErrorCode = CanCreateInstance(pRole, info, pInstance);
//
//			if(dwErrorCode == E_Success)
//			{
//				if(FALSE == CreateInstance(pRole->GetID(), pRole->GetTeamID(), pMapMgr, info.dwDestMapID, (EInstanceHardMode)info.dwMisc))
//					return FALSE;
//
//				// 设置小队副本ID
//				pTeam->SetOwnInstanceID(m_dwInstanceIDGen);
//				pTeam->SetOwnInstanceMapID(info.dwDestMapID);
//
//				// 询问玩家是否通知队友
//				if(pInstance->bNoticeTeamate == true)
//				{
//					tagNS_InstanceNofity	send;
//					pRole->GetSession()->SendMessage(&send, send.dwSize);
//				}
//
//				// 增加副本ID索引
//				++m_dwInstanceIDGen;
//			}
//		}
//		else	// 进入已有副本
//		{
//			Map *pMap = pMapMgr->GetRoleInstancePtr(pRole);
//			if(!P_VALID(pMap))
//				return FALSE;
//
//			dwErrorCode = pMap->CanEnter(pRole, info.dwDestMapID);
//		}
//	}
//	else // 单人进入
//	{
//		dwErrorCode = CanCreateInstance(pRole, info, pInstance);
//
//		if(dwErrorCode == E_Success)
//		{
//			if(FALSE == CreateInstance(pRole->GetID(), pRole->GetTeamID(), pMapMgr, info.dwDestMapID, (EInstanceHardMode)info.dwMisc))
//				return FALSE;
//
//			// 设置单人副本ID
//			pRole->SetMyOwnInstanceID(m_dwInstanceIDGen);
//
//			// 增加副本ID索引
//			++m_dwInstanceIDGen;
//		}
//	}
//
//	Map		*pMap = pMapMgr->GetRoleInstancePtr(pRole);
//	DWORD	dwTimeLimit = GT_INVALID;
//	INT		nRoleNum = 0;
//
//	if(E_Success == dwErrorCode && !P_VALID(pMap))
//	{
//		ASSERT(0);
//		return FALSE;
//	}
//
//	if(P_VALID(pMap))
//	{
//		dwTimeLimit = pMap->CalTimeLimit();
//	}
//	
//	// 返回客户端进入副本是否成功
//	tagNS_EnterInstance		send;
//	send.dwTimeLimit = dwTimeLimit;
//	send.dwErrorCode = dwErrorCode;
//	pRole->GetSession()->SendMessage(&send, send.dwSize);
//
//	if(dwErrorCode == E_Success)
//		return TRUE;
//	else
//		return FALSE;
//}
//
////---------------------------------------------------------------------------------------------
//// 是否能创建副本
////---------------------------------------------------------------------------------------------
//DWORD MapCreator::CanCreateInstance(Role *pRole, tagChangeMapInfo& info, const tagInstance* pInstance)
//{
//	// 是否可选副本难度
//	if(pInstance->bSelectHard == false)
//	{
//		// 不可选
//		if((EInstanceHardMode)info.dwMisc != EIHM_Normal)
//			return GT_INVALID;
//	}
//	else // 可选
//	{
//		// 是否可选通常难度
//		if((EInstanceHardMode)info.dwMisc == EIHM_Normal && pInstance->bSelectNormal != true)
//			return GT_INVALID;
//
//		// 是否可选精英难度
//		if((EInstanceHardMode)info.dwMisc == EIHM_Elite && pInstance->bSelectElite != true)
//			return GT_INVALID;
//
//		// 是否可选魔王难度
//		if((EInstanceHardMode)info.dwMisc == EIHM_Devil && pInstance->bSelectDevil != true)
//			return GT_INVALID;
//	}
//
//	// 副本等级下限
//	if(pInstance->nLevelDownLimit > pRole->GetLevel())
//		return E_Instance_Level_Down_Limit;
//
//	// 副本等级下限
//	if(pInstance->nLevelUpLimit < pRole->GetLevel())
//		return E_Instance_Level_Up_Limit;
//
//	// 人数下限
//	if(pInstance->nNumDownLimit > 1)
//	{
//		const CTeam *pTeam = g_groupMgr.GetTeamPtr(pRole->GetTeamID());
//		if(!P_VALID(pTeam))
//			return GT_INVALID;
//		else if(pTeam->GetMemNum() < pInstance->nNumDownLimit) 
//			return E_Instance_Role_Lack;
//	}
//
//	// 是否是同一副本
//	if(pRole->GetMapID() == info.dwDestMapID)
//		return GT_INVALID;
//
//	return E_Success;
//}

MapCreator g_mapCreator;

VOID	MapCreator::ReportMapDynInfo( BOOL bSaveToDB )
{
	BOOL bChanged = FALSE;
	BOOL bShouldSend = FALSE;
	INT nOld = 0;
	MapMgrMap::TMapIterator it = m_mapMapMgr.Begin();
	MapMgr* pMagMgr = NULL;
	bool bInsert = false;
	tstring strKey;
	// 因为mapcreator是全局的，这个方法也是在threadupdate里面调用，因此，我这样做；如果是多线程，请不要这样实现
	static TCHAR szBuff[100] = {0};
	for( ; m_mapMapMgr.PeekNext(it,pMagMgr) ; )
	{
		bInsert = false;
		if( !P_VALID(pMagMgr))
			continue;
		Map* pMap =	pMagMgr->GetSingleMap();

		tagMapOnlineInfo * pOnlineInfo = m_mapOnlineInfo.Peek(pMagMgr->GetMapID());
		if( !P_VALID(pOnlineInfo) )
		{
			pOnlineInfo = new tagMapOnlineInfo;
			pOnlineInfo->bIsInst		= !pMagMgr->IsNormalMap();
			pOnlineInfo->dwMapID	= pMagMgr->GetMapID();
			pOnlineInfo->strMapName = pMagMgr->GetMapInfo()->szMapFileName;
			bInsert = true;
		}
		nOld = pOnlineInfo->nOnlineNum;
		if( pOnlineInfo->bIsInst )
		{
			pOnlineInfo->nOnlineNum	= pMagMgr->GetInstancesRoleNum();
			pOnlineInfo->nOpenNum	= pMagMgr->GetCurInstanceNum();
		}
		else
		{
			pOnlineInfo->nOnlineNum	= P_VALID(pMap) ? pMap->GetRoleNum() : 0 ;
			pOnlineInfo->nOpenNum	= 1;
		}
		if( bInsert || nOld != pOnlineInfo->nOnlineNum )
		{
			bChanged = TRUE;
			bShouldSend = TRUE;
		}
		else
			bChanged = FALSE;

		pOnlineInfo->bChanged		= bChanged;

		if( bInsert )
			m_mapOnlineInfo.Add(pOnlineInfo->dwMapID,pOnlineInfo);
		else
			m_mapOnlineInfo.ChangeValue(pOnlineInfo->dwMapID,pOnlineInfo);

		strKey = pOnlineInfo->strMapName;
		_stprintf(szBuff,_T("(%d)"),pOnlineInfo->nOpenNum);
		strKey += szBuff;
		g_world.WatchKeyValue(pOnlineInfo->strMapName.c_str(),strKey.c_str(),pOnlineInfo->nOnlineNum);
	}
	if( bSaveToDB && bShouldSend )
	{
		// 变成字段，存入数据库
		// NDBC_WorldMapOlInfo,		tagWorldMapOnlineInfo
		INT nSize = m_mapOnlineInfo.Size();
		if( nSize > 0 )
		{
			jm_tool::TArray<tagMapOnlineInfo> temArray( nSize );
			TMap<DWORD,tagMapOnlineInfo*>::TMapIterator it = m_mapOnlineInfo.Begin();
			tagMapOnlineInfo* pOlInfo = NULL;
			INT nSendNum = 0;
			for( ; m_mapOnlineInfo.PeekNext(it,pOlInfo) ;  )
			{
				if( P_VALID( pOlInfo ) && pOlInfo->bChanged )
				{
					temArray[nSendNum++].copy(pOlInfo);
				}
			} // for 
			if( nSendNum > 0 )
			{
				INT nSendSize = sizeof( tagNDBC_WorldMapOlInfo ) + nSendNum * sizeof(tagWorldMapOnlineInfo) - 1;
				MCREATE_MSG(pOlMsg,nSendSize,NDBC_WorldMapOlInfo);
				pOlMsg->nNum		= nSendNum;
				tagWorldMapOnlineInfo * pData = (tagWorldMapOnlineInfo*)pOlMsg->data;
				for( INT i = 0; i < nSendNum; ++i )
				{
					pData->dwMapID			= temArray[i].dwMapID;
					pData->bIsInst					= temArray[i].bIsInst;
					pData->nOnlineNum		= temArray[i].nOnlineNum;
					pData->nOpenNum			= temArray[i].nOpenNum;
					++pData;
				}
				g_dbSession.Send(pOlMsg,pOlMsg->dwSize);
				MDEL_MSG(pOlMsg);
			}
		} // if size
	} // if save 
}


