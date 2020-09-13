#include "StdAfx.h"
#include "team.h"
#include "convenient_team.h"
#include "../worlddefine/msg_easy_team.h"
#include "timechecker.h"
#include "../vEngine/memory/safe_mem_pool.h"
#include "../ServerDefine/mini_tool.h"


static jm_tool::CSCreatedLock _queu_cs;

// 便捷组队内部专用内存池
static vEngine::SafeMemPool __mpool(2*1024*1024); // 不要太大，因为一个tagCTInfo也就是36bytes，一万个也不到500k。

static void FreeCTInfo(ConvenientTeam::tagCTInfo * pInfo);
static 	ConvenientTeam::tagCTInfo * MallocCTInfo(void);


/************************************************************************/
/* 这个模块可能会很影响服务器性能，因此从消息结构大小，到更新频繁度等都需要仔细斟酌 */
/* 而且最好有world控制台命令，来动态开关，这样在服务器压力大时，关闭它减轻压力 */
/************************************************************************/


static DWORD PushBackEasyTeamSimInfo(ServerMemPool* pMemPool,tagNS_GetEasyTeamInfo*& data,
											DWORD dwLeaderID,INT nLevel,		INT nRoleNum,
											INT nTeamMaxNum=MAX_TEAM_NUM,EEasyTeamType type = EETT_KillCreature,
											INT nMemLen=0,LPCTSTR strMemo=NULL,BYTE ActInst = 0);


VOID ConvenientTeam::OnTeamChanged(  ECTType eChangeType, INT nEventType,Team * pTeam )
{
	if( !m_bOpen || !m_bSendTeamChange )
		return ;
	if( !P_VALID(pTeam) )
		return;
	Role * pLeader = pTeam->GetMemPtr(0);
	if( !P_VALID(pLeader) )
		return;
	Map * pMap = pLeader->GetMap();
	if( !P_VALID(pMap) )
		return;

	if( nEventType == EETCT_DelTeam )
	{
		//CHECK_TIME_WITH_ALLPARAM_EX("OnTeamChanged ns_easyteam DelTeam",80,TRUE);
		tagNS_EasyTeamEvent msg;
		msg.byTeamEvent	= nEventType&0xFF;
		msg.byTeamInfo[0]		= 0;
		pMap->SendBigVisTileMsg(pLeader,&msg,msg.dwSize);
	}
	else
	{
		if( eChangeType == ECT_Creature )
		{
			//CHECK_TIME_WITH_ALLPARAM_EX( "OnTeamChanged ns_easyteam full" ,80,TRUE);
			DWORD dwSize = sizeof(tagNS_EasyTeamEvent) - 1 + sizeof(tagEasyTeamSimInfo);
			MCREATE_MSG(pMsg,dwSize,NS_EasyTeamEvent);
			pMsg->byTeamEvent = nEventType;
			tagEasyTeamSimInfo *pTeamInfo = (tagEasyTeamSimInfo*)pMsg->byTeamInfo;
			pTeamInfo->byContent[0]= 0;
			pTeamInfo->byMemoLen	= 0;
			pTeamInfo->byTeamType	= EETT_KillCreature;
			pTeamInfo->dwLeaderID	= pLeader->GetID();
			pTeamInfo->wRoles		= ((pTeam->GetMemNum() & 0xFF ) << 8) | (MAX_TEAM_NUM & 0xFF);
			pMap->SendBigVisTileMsg(pLeader,pMsg,dwSize);
			MDEL_MSG(pMsg);
		}
	}
}
/************************************************************************
step1: 提供杀怪简单方式的实现  9/26/2010
step2：提供全面，待
************************************************************************/
VOID ConvenientTeam::OnRoleVisTileChange( Unit * pUnit, Map * pMap,Map * pNewMap,INT nOldPos, INT nNewVisTileNdx )
{
	//if( !P_VALID(pUnit) )
	//	return;
	//if( !pUnit->IsRole() )
	//	return;
	//if( nOldPos == nNewVisTileNdx )
	//	return;

	//Role * pRole = (Role*)pUnit;
	//DWORD dwTeamID = pRole->GetTeamID();
	//if( dwTeamID != GT_INVALID ) // 有队伍，要通知原格子或原地图格子内的玩家队伍超出范围
	//{
	//}
}


BOOL ConvenientTeam::GetEasyTeamInfo(tagNS_GetEasyTeamInfo*& pMsg,Unit * pUnit,ServerMemPool* pMemPool)
{
	if( !m_bOpen )
		return FALSE;

	std::set<DWORD> teams;
	if( !P_VALID(pUnit) )
		return FALSE;
	Map * pMap = pUnit->GetMap();
	if( !P_VALID(pMap) )
		return FALSE;

	set<DWORD> zuduiteams;
	set<DWORD>::iterator ifinded;
	INT nZhuNum = GetZuduiInfo(pMsg,pUnit,pMemPool,zuduiteams);

	//CHECK_SPEC_BEGIN_EX( "GetVisTileTeams" ,80,TRUE)
	pMap->GetVisTileTeams(pUnit->GetVisTileIndex(),teams);
	//CHECK_SPEC_END
	const Team * pTeam = NULL;
	Role * ply = NULL;
	BOOL re = FALSE;
	//CHECK_SPEC_BEGIN_EX( "PushBackEasyTeamSimInfo all" ,80,TRUE)
	for( std::set<DWORD>::iterator i = teams.begin(); i != teams.end(); ++i )
	{
		pTeam = g_groupMgr.GetTeamPtr(*i);
		if( P_VALID(pTeam) )
		{
			ply = pTeam->GetMemPtr(0);
			if( P_VALID(ply) )
			{
				ifinded = zuduiteams.find(ply->GetID());
				if( ifinded != zuduiteams.end() )
					continue;
				if( PushBackEasyTeamSimInfo(pMemPool,pMsg,ply->GetID(),ply->GetLevel(),pTeam->GetMemNum()) > 0 )
				{
					re = TRUE;
					if( pMsg->dwSize >= m_nPckLimit )
						break;
				}
			}
		}
	}
	if( nZhuNum == 0 && teams.size() == 0 )
	{
		static tagNS_GetEasyTeamInfo tem;
		if( P_VALID(pMemPool) )
			pMsg = (tagNS_GetEasyTeamInfo * )pMemPool->Alloc(sizeof(tagNS_GetEasyTeamInfo));
		else
			pMsg = new tagNS_GetEasyTeamInfo;
		pMsg->dwID = tem.dwID;
		pMsg->dwSize = tem.dwSize;
		pMsg->wTeamNums = 0;
		re = TRUE;
	}
	
	if( nZhuNum > 0 && re == FALSE )
		re = TRUE;

	//CHECK_SPEC_END
	return re;
}


static DWORD GetEasyTeamMsgLen(tagNS_GetEasyTeamInfo*pMsg)
{
	if( !P_VALID(pMsg) )
		return 0;
	//CHECK_TIME_WITH_ALLPARAM( "GetEasyTeamMsgLen" ,80)
	DWORD size = sizeof(tagNS_GetEasyTeamInfo) ;
	for( INT i = 0; i < pMsg->wTeamNums; ++i )
	{
		if( i > 0 )
			size += sizeof(tagEasyTeamSimInfo);
		if( pMsg->Teams[i].byMemoLen > 0 )
			size += pMsg->Teams[i].byMemoLen - 1;
	}

	return size;
}

// 移动到下一个对的位置
static tagEasyTeamSimInfo * MoveNextTeamInfo(tagNS_GetEasyTeamInfo * pMsg,INT nTeamNdx)
{
	tagEasyTeamSimInfo * pStep = pMsg->Teams;
	INT pos = NULL;
	for( INT i = 0; i < nTeamNdx; ++i )
	{
		pos = sizeof(tagEasyTeamSimInfo);
		if( pStep->byMemoLen > 0 )
			pos += pStep->byMemoLen - 1;
		pStep = (tagEasyTeamSimInfo*)(((BYTE*)pStep) + pos);
	}
	return pStep;
}

static DWORD PushBackEasyTeamSimInfo(ServerMemPool* pMemPool		,	tagNS_GetEasyTeamInfo*& data,
									 DWORD dwLeaderID				,	INT nLevel,		INT nRoleNum,
									 INT nTeamMaxNum				,	EEasyTeamType type ,
									 INT nMemLen					,	LPCTSTR strMemo, BYTE byActInst )
{
	BOOL bUsePool = TRUE;
	if( !P_VALID(pMemPool) )
		bUsePool = FALSE;

	DWORD dwSize = 0;
	DWORD dwRecSize = 0;

	static DWORD dwPadding = sizeof(tagEasyTeamSimInfo);

	if( data == NULL )
	{
		// 链的头部，不用movenext
		dwSize = sizeof(tagNS_GetEasyTeamInfo) + (nMemLen > 0 ? (nMemLen*sizeof(TCHAR)-1) : 0);
		if( bUsePool )
			data = (tagNS_GetEasyTeamInfo*)pMemPool->Alloc(dwSize+dwPadding);
		else
			data = (tagNS_GetEasyTeamInfo*)new BYTE[dwSize+dwPadding];
		data->wTeamNums = 0;
		data->Teams[data->wTeamNums].byMemoLen	= nMemLen*sizeof(TCHAR);
		if( nMemLen > 0 )
			_tcsncpy((TCHAR*)data->Teams[data->wTeamNums].byContent, strMemo,nMemLen);
		else
			data->Teams[data->wTeamNums].byContent[0] = 0;
		data->Teams[data->wTeamNums].byTeamType		= type;
		data->Teams[data->wTeamNums].dwLeaderID		= dwLeaderID;
		data->Teams[data->wTeamNums].byLeaderLevel	= nLevel;
		data->Teams[data->wTeamNums].byInsAndActi	= byActInst;
		data->Teams[data->wTeamNums].wRoles		= (( nRoleNum & 0xff ) << 8) | (nTeamMaxNum & 0xff);
		data->wTeamNums++;
		data->dwSize	= dwSize;
		static tagNS_GetEasyTeamInfo tem;
		data->dwID		= tem.dwID;
		return dwSize;
	}
	else
	{
		dwSize = data->dwSize;//GetEasyTeamMsgLen(data);
		dwRecSize = sizeof(tagEasyTeamSimInfo);
		if( nMemLen > 0 && !P_VALID(strMemo) )
			return 0;

		if( nMemLen > 0 )
			dwRecSize += nMemLen*sizeof(TCHAR) - 1;
		LPVOID pData = NULL;
		if( bUsePool )
			pData = pMemPool->Alloc(dwSize+dwRecSize+dwPadding);
		else
			pData = new BYTE[dwSize+dwRecSize+dwPadding];
		if( !P_VALID(pData) )
			return 0;
		memcpy(pData,data,dwSize);

		if( bUsePool )
			pMemPool->Free(data);
		else
			delete[] (BYTE*)data;

		data = (tagNS_GetEasyTeamInfo*)pData;
		tagEasyTeamSimInfo * pTeamInfo = (tagEasyTeamSimInfo *)((LPBYTE)pData+dwSize);//MoveNextTeamInfo(data,data->wTeamNums);
		pTeamInfo->byMemoLen	= nMemLen*sizeof(TCHAR);
		if( nMemLen > 0 )
			_tcsncpy((TCHAR*)pTeamInfo->byContent, strMemo,nMemLen);
		else
			pTeamInfo->byContent[0] = 0;
		pTeamInfo->byTeamType		= type;
		pTeamInfo->dwLeaderID		= dwLeaderID;
		pTeamInfo->byLeaderLevel	= nLevel;
		pTeamInfo->byInsAndActi	= byActInst;
		pTeamInfo->wRoles		= (( nRoleNum & 0xff ) << 8) | (nTeamMaxNum & 0xff);
		data->wTeamNums++;
		data->dwSize	= dwSize+dwRecSize;
	}


	return dwSize+dwRecSize;
}

static VOID FreeData(map<INT,ConvenientTeam::tagCTInfo*>& con)
{
	for( map<INT,ConvenientTeam::tagCTInfo*>::iterator i = con.begin(); i != con.end(); ++i )
	{
		ConvenientTeam::tagCTInfo * p = i->second;
		if( P_VALID(p) )
			FreeCTInfo(p);
	}
}

static ConvenientTeam * pConvenientTeam = NULL;

ConvenientTeam::ConvenientTeam()
:m_bOpen(FALSE),m_bSendTeamChange(FALSE),m_nPlyNumLimit(GT_INVALID),m_nPckLimit(2*1024)
{
}
ConvenientTeam::~ConvenientTeam()
{
	FreeData(m_ZdCTInfos);
	FreeData(m_QzCTInfos);
}
// 这些，就一份，不用内存池了
ConvenientTeam& ConvenientTeam::GetInstance()
{
	if( !pConvenientTeam )
	{
		pConvenientTeam = new ConvenientTeam;
	}
	return *pConvenientTeam;
}

VOID ConvenientTeam::Destroy()
{
	if( pConvenientTeam )
	{
		pConvenientTeam->m_bOpen = FALSE;
		delete pConvenientTeam;
	}
	pConvenientTeam = NULL;
}

BOOL ConvenientTeam::GetFreePlayers( tagNS_GetSinglePlayers*& pMsg, Unit * pUnit, ServerMemPool* pMemPool )
{
	if( !m_bOpen )
		return FALSE;

	BOOL bUsePool = TRUE;
	if( !P_VALID(pMemPool) )
		bUsePool = FALSE;
	
	std::set<DWORD> plys;
	if( !P_VALID(pUnit) )
		return FALSE;
	Map * pMap = pUnit->GetMap();
	if( !P_VALID(pMap) )
		return FALSE;

	//CHECK_SPEC_BEGIN_EX( "GetFreePlayers" ,80,TRUE)
	pMap->GetNoTeamPlayers(pUnit->GetVisTileIndex(),plys);
	//CHECK_SPEC_END
	
	DWORD dwPlys = plys.size();
	DWORD dwSize = sizeof(tagNS_GetSinglePlayers) + (dwPlys > 0 ? (sizeof(DWORD)*(dwPlys - 1)) : 0);
	if( bUsePool )
		pMsg = (tagNS_GetSinglePlayers*)pMemPool->Alloc(dwSize);
	else
		pMsg = (tagNS_GetSinglePlayers*)new BYTE[dwSize];

	static tagNS_GetSinglePlayers tem;
	pMsg->dwID = tem.dwID;

	pMsg->dwSize = dwSize;
	pMsg->wNums = dwPlys;
	DWORD * pID = pMsg->dwRoleID;
	INT n = 0;
	for( std::set<DWORD>::iterator i = plys.begin(); i != plys.end(); ++i )
	{
		pID[n++] = *i;
	}

	return TRUE;
}



/************************************************************************/
/*                      instance & activity                             */
/************************************************************************/


#ifdef _UNICODE
#define Constructor()	wstring::basic_string()
#define Destructor()	wstring::~basic_string()
#else
#define Constructor()	string::basic_string()
#define Destructor()	string::~basic_string()
#endif

static 	ConvenientTeam::tagCTInfo * MallocCTInfo(void)
{
	ConvenientTeam::tagCTInfo * pInfo = (ConvenientTeam::tagCTInfo*)__mpool.Alloc(sizeof(ConvenientTeam::tagCTInfo));
	pInfo->strContent.Constructor();
	return pInfo;
}
static void FreeCTInfo(ConvenientTeam::tagCTInfo * pInfo)
{
	if( P_VALID(pInfo) )
	{
		pInfo->strContent.Destructor();
		__mpool.Free(pInfo);
	}
}
#ifdef Constructor
#undef Constructor
#endif
#ifdef Destructor
#undef Destructor
#endif


// 发布新的组队/求组信息
DWORD ConvenientTeam::IssueCTInfo(tagNC_PutOutInfo * pMsg, Unit * ply)
{
	if( !m_bOpen )
		return GT_INVALID;
	if( !P_VALID(pMsg) || !P_VALID(ply) )
		return GT_INVALID;

	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(_queu_cs);

	if( m_nPlyNumLimit > 0 && m_PlayerInfoID.size() > m_nPlyNumLimit )
		return GT_INVALID;

	TPlayerInfoID::iterator i = m_PlayerInfoID.find(ply->GetID());
	if( i != m_PlayerInfoID.end() )
		return GT_INVALID;

	INT ndx = 0;

	tagCTInfo * pInfo = MallocCTInfo();
	if( !P_VALID(pInfo) ) // 预留的内存不够了，说明这个玩意太占内存，干脆就不允许加了
		return GT_INVALID;

	pInfo->byScene				=	pMsg->byScene;
	pInfo->byTeamType			=	pMsg->byTeamType;
	pInfo->byTeamTypeDetail	=	pMsg->byTeamTypeDetail;
	pInfo->byTeamNum			=	pMsg->byTeamNum;
	pInfo->byMaxLevel			=	pMsg->byMaxLevel;
	pInfo->byMinLevel			=	pMsg->byMinLevel;
	pInfo->dwRoleID			=	ply->GetID();
	if( pMsg->btLen > 0 )
	{
		TCHAR buf[X_SHORT_NAME];
		INT num = min(X_SHORT_NAME,pMsg->btLen /*/ sizeof(TCHAR)*/);
		jm_tscopy_s(buf,num,(LPCTSTR)pMsg->byContent);
		if( num < X_SHORT_NAME )
			buf[num] = 0;
		else
			buf[X_SHORT_NAME-1] = 0;
		pInfo->strContent			=	buf;
	}
	
	ndx = InsertCTInfo(pInfo,pMsg->bTeam);

	m_PlayerInfoID[ply->GetID()] = ndx;

	return E_Success;
}

static INT _index = 0;
static INT _index2 = 0;
static INT MaxCTInfoNdx(VOID)
{
	return max(_index,_index2);
}

INT	ConvenientTeam::InsertCTInfo(tagCTInfo * pInfo,bool bIsZudui)
{
	INT ndx = 0;
	if( bIsZudui )
	{
		ndx = ++_index;
		m_ZdCTInfos[ndx] = pInfo;
	}
	else
	{
		ndx = ++_index2;
		m_QzCTInfos[ndx] = pInfo;
		ndx = 0 - ndx;
	}
	if( ndx != 0 )
	{
		TSceneCTInfos::iterator i = m_SceneCTInfos.find(pInfo->byScene);
		if( i != m_SceneCTInfos.end() )
		{
			m_SceneCTInfos[pInfo->byScene].push_back(ndx);
		}
		else
		{
			vector<INT> t ;
			t.push_back(ndx);
			m_SceneCTInfos.insert(std::make_pair(pInfo->byScene, t));
		}
	}
	return ndx;
}


ConvenientTeam::tagCTInfo * ConvenientTeam::GetCTInfo(INT ndx)
{
	tagCTInfo * p = NULL;
	TCTInfo * pCon = NULL;
	if( ndx > 0 )
	{
		pCon = &m_ZdCTInfos;
	}
	else if( ndx < 0 )
	{
		pCon = &m_QzCTInfos;
		ndx = 0 - ndx;
	}
	TCTInfo::iterator it = pCon->find(ndx);
	if( it != pCon->end() )
		p = it->second;
	return p;
}


VOID	ConvenientTeam::DeleteCTInfo(INT CTInfoNdx ,INT nSceneNdx)
{
	// vector support random-access iterator
	if( nSceneNdx >= 0 )
	{
		m_SceneCTInfos.erase(nSceneNdx);
	}
	if( CTInfoNdx == 0 )
		return;

	TCTInfo * pCon = NULL;
	if( CTInfoNdx > 0 )
		pCon = &m_ZdCTInfos;
	else
	{
		pCon = &m_QzCTInfos;
		CTInfoNdx = -CTInfoNdx;
	}
	if( CTInfoNdx >MaxCTInfoNdx())
		return;

	TCTInfo::iterator i = pCon->find(CTInfoNdx);
	if( i != pCon->end() )
	{
		tagCTInfo * p = i->second;
		pCon->erase(i);
		// inner this function("FreeCTInfo") ,it will check the pointer
		if( P_VALID(p) )
		{
			INT nScene = p->byScene;
			TSceneCTInfos::iterator it = m_SceneCTInfos.find(nScene);
			if( it != m_SceneCTInfos.end() )
			{
				vector<INT>::iterator iFind = find(it->second.begin(),it->second.end(),CTInfoNdx);
				if( iFind != it->second.end() )
				{
					it->second.erase(iFind);
					if(it->second.empty())
						m_SceneCTInfos.erase(it);
				}
			}
			m_PlayerInfoID.erase(p->dwRoleID);

			FreeCTInfo(p);
		}
	}

	if( m_ZdCTInfos.empty() && m_QzCTInfos.empty() )
		m_SceneCTInfos.clear();
}


static BOOL FillPersonInfo(tagNS_GetPersonalInfo *& pMsg,Role* ply,const ConvenientTeam::tagCTInfo * pInfo,ServerMemPool* pMemPool = NULL)
{
	if( !P_VALID(pInfo) )
		return FALSE;

	BOOL bMP = TRUE;
	if( !P_VALID(pMemPool) )
		bMP = FALSE;

	DWORD dwCTSize = sizeof(tagPersonalMode);
	DWORD dwTotal = 0;
	if( pInfo->strContent.length() > 0 )
		dwCTSize += -1 + sizeof(TCHAR) * pInfo->strContent.length(); 
	if( P_VALID(pMsg) )
	{
		dwTotal = pMsg->dwSize + dwCTSize;
		LPVOID pData = NULL;
		if( bMP )
			pData = pMemPool->Alloc(dwTotal);
		else
			pData = new BYTE[dwTotal];
		if( !P_VALID(pData) )
			return FALSE;

		tagNS_GetPersonalInfo * pTemMsg = (tagNS_GetPersonalInfo*)pData;
		memcpy(pTemMsg,pMsg,pMsg->dwSize);
		pTemMsg->dwSize = dwTotal;
		pTemMsg->wNums++;
		tagPersonalMode * pAdding		= (tagPersonalMode*)((BYTE*)pTemMsg+pMsg->dwSize);

		pAdding->byType				= pInfo->byTeamType;
		pAdding->byLevel				= ply->GetLevel();
		pAdding->dwRoleID				= ply->GetID();
		pAdding->byInsAndActi			= pInfo->byTeamTypeDetail;
		pAdding->byLen					= pInfo->strContent.length();
		if( pAdding->byLen > 0 )
			jm_tscopy_s((TCHAR*)pAdding->byContent,pAdding->byLen,pInfo->strContent.c_str());
		
		if( bMP )
			pMemPool->Free(pMsg);
		else
			delete[] (BYTE*)pMsg;

		pMsg = pTemMsg;
	}
	else
	{
		tagNS_GetPersonalInfo tem;
		dwTotal = sizeof(tagNS_GetPersonalInfo);
		if( dwCTSize > sizeof(tagPersonalMode) )
			dwTotal += dwCTSize - sizeof(tagPersonalMode);
		LPVOID  pData = NULL;
		if( bMP )
			pData = pMemPool->Alloc(dwTotal);
		else
			pData = new BYTE[dwTotal];
		if( !P_VALID(pData) )
			return FALSE;
		pMsg = (tagNS_GetPersonalInfo*)pData;
		memcpy(pMsg,&tem,tem.dwSize);
		pMsg->dwSize = dwTotal;
		pMsg->wNums = 1;
		tagPersonalMode * pAdding		= (tagPersonalMode*)pMsg->Person;

		pAdding->byType				= pInfo->byTeamType;
		pAdding->byLevel				= ply->GetLevel();
		pAdding->dwRoleID				= ply->GetID();
		pAdding->byInsAndActi			= pInfo->byTeamTypeDetail;
		pAdding->byLen					= pInfo->strContent.length();
		if( pAdding->byLen > 0 )
			jm_tscopy_s((TCHAR*)pAdding->byContent,pAdding->byLen,pInfo->strContent.c_str());
	}

	return TRUE;
}


BOOL ConvenientTeam::GetPersonalInfo( tagNS_GetPersonalInfo *& pMsg, Unit * ply,ServerMemPool* pMemPool )
{
	if( !m_bOpen )
		return FALSE;
	if( !P_VALID(ply) )
		return FALSE;

	BOOL re = FALSE;
	tagCTInfo temPlyInfo;
	INT nSelfInfoNdx = 0;
	{
		jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(_queu_cs);
		TPlayerInfoID::iterator ifind = m_PlayerInfoID.find(ply->GetID());
		if( ifind != m_PlayerInfoID.end() )
		{
			nSelfInfoNdx = ifind->second;
			if( nSelfInfoNdx > 0 )
				nSelfInfoNdx = 0;
			else
			{
				tagCTInfo * pFind = GetCTInfo(nSelfInfoNdx);
				if( P_VALID(pFind) )
				{
					temPlyInfo.byMaxLevel		= pFind->byMaxLevel;
					temPlyInfo.byMinLevel		= pFind->byMinLevel;
					temPlyInfo.byScene		= pFind->byScene;
					temPlyInfo.byTeamNum		= pFind->byTeamNum;
					temPlyInfo.byTeamType		= pFind->byTeamType;
					temPlyInfo.byTeamTypeDetail		= pFind->byTeamTypeDetail;
					temPlyInfo.dwRoleID		= pFind->dwRoleID;
					temPlyInfo.strContent		= pFind->strContent;
				}
				else
					nSelfInfoNdx = 0;
			}
		}
	}

	DWORD dwMapID = ply->GetMapID();
	TMapScene::iterator i = m_MapIDScene.find(dwMapID);
	if( nSelfInfoNdx == 0 && i == m_MapIDScene.end() )
		return FALSE;
	if(i == m_MapIDScene.end())
	{
		if( nSelfInfoNdx < 0 )
		{
			if( FillPersonInfo(pMsg,(Role*)ply,&temPlyInfo,pMemPool) )
				re = TRUE;
		}
		return re;
	}

	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g1(_queu_cs);
	INT size = m_QzCTInfos.size();
	if( size <= 0 )
	{
		if( P_VALID(pMemPool) )
		{
			tagNS_GetPersonalInfo tem;
			pMsg = (tagNS_GetPersonalInfo*)pMemPool->Alloc(sizeof(tagNS_GetPersonalInfo));
			memcpy(pMsg,&tem,tem.dwSize);
			pMsg->wNums = 0;
			return TRUE;
		}
		return FALSE;
	}

	tagCTInfo * pInfo = NULL;
	INT nTeamNum = 0;
	INT nLevelMin = 0, nLevelMax = 0;

	//CHECK_TIME_WITH_ALLPARAM_EX("easyteam GetPersonalInfo",80,TRUE);

	vector<int> _vecCTInfoToDel;

	vector<INT> & scenes = i->second;
	for( vector<INT>::iterator it = scenes.begin(); it != scenes.end(); ++it )
	{
		TSceneCTInfos::iterator iNner = m_SceneCTInfos.find(*it);
		if( iNner == m_SceneCTInfos.end() )
			continue;

		vector<INT> & vCTInfos = iNner->second;
		for( vector<INT>::iterator a = vCTInfos.begin(); a != vCTInfos.end(); ++a )
		{
			if( *a > 0 )
				continue;

			pInfo = GetCTInfo(*a);
			Role * player = NULL;

			if( P_VALID(pInfo) )
			{
				nTeamNum	= pInfo->byTeamNum;
				nLevelMin	= pInfo->byMinLevel;
				nLevelMax	= pInfo->byMaxLevel;
				INT id = ply->GetID();
				if(pInfo->dwRoleID != id && ply->GetLevel() < nLevelMin || ply->GetLevel() > nLevelMax)
					continue;
				player = g_roleMgr.GetRolePtrByID(pInfo->dwRoleID);
				if( !P_VALID(player) )
					continue;
				if( P_VALID(player->GetTeamID()) )
				{
					const Team * pTeam = g_groupMgr.GetTeamPtr(player->GetTeamID());
					if( pInfo->dwRoleID != id && !P_VALID(pTeam) )
						continue;
					if( pInfo->dwRoleID != id && P_VALID(pTeam) && pTeam->GetMemNum() >= nTeamNum )
					{
						//DeleteCTInfo(*a);
						_vecCTInfoToDel.push_back(*a);
						continue;
					}
				}
			}
			else
				continue;

			if( nSelfInfoNdx == *a )
				nSelfInfoNdx = 0;

			if( FillPersonInfo(pMsg,player,pInfo,pMemPool) )
			{
				re = TRUE;
				if( pMsg->dwSize >= m_nPckLimit )
					return re;
			}
			else
				break;
		}
	}
	if( _vecCTInfoToDel.size() > 0 )
	{
		for( vector<int>::iterator iToDel = _vecCTInfoToDel.begin(); iToDel != _vecCTInfoToDel.end(); ++iToDel )
		{
			DeleteCTInfo(*iToDel);
		}
	}
	if( nSelfInfoNdx < 0 )
	{
		if( FillPersonInfo(pMsg,(Role*)ply,&temPlyInfo,pMemPool) )
			re = TRUE;
	}
	return re;
}


VOID ConvenientTeam::DeleteCTInfoOfPlayer(Unit * ply,BOOL bTeamOrNot)
{
	if( !m_bOpen )
		return ;
	if( !P_VALID(ply) )
		return;
	DWORD dwRoleID = ply->GetID();
	if( !ply->IsRole() )
		return;
	//Role * pRole = (Role*)ply;

	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(_queu_cs);
	TPlayerInfoID::iterator i = m_PlayerInfoID.find(dwRoleID);
	if( i == m_PlayerInfoID.end() )
		return;

	INT nCTInfoNdx = i->second;
	DeleteCTInfo(nCTInfoNdx);
	m_PlayerInfoID.erase(dwRoleID);
}


BOOL ConvenientTeam::GetZuduiInfo(tagNS_GetEasyTeamInfo*& pMsg,Unit * pUnit,ServerMemPool* pMemPool,set<DWORD> & team_leaders)
{
	DWORD dwMapID = pUnit->GetMapID();
	tagCTInfo temPlyInfo;
	INT nSelfInfoNdx = 0;
	{
		jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(_queu_cs);
		TPlayerInfoID::iterator ifind = m_PlayerInfoID.find(pUnit->GetID());
		if( ifind != m_PlayerInfoID.end() )
		{
			nSelfInfoNdx = ifind->second;
			if( nSelfInfoNdx < 0 )
				nSelfInfoNdx = 0;
			else
			{
				tagCTInfo * pFind = GetCTInfo(nSelfInfoNdx);
				if( P_VALID(pFind) )
				{
					temPlyInfo.byMaxLevel		= pFind->byMaxLevel;
					temPlyInfo.byMinLevel		= pFind->byMinLevel;
					temPlyInfo.byScene		= pFind->byScene;
					temPlyInfo.byTeamNum		= pFind->byTeamNum;
					temPlyInfo.byTeamType		= pFind->byTeamType;
					temPlyInfo.byTeamTypeDetail		= pFind->byTeamTypeDetail;
					temPlyInfo.dwRoleID		= pFind->dwRoleID;
					temPlyInfo.strContent		= pFind->strContent;
				}
				else
					nSelfInfoNdx = 0;
			}
		}
	}

	BOOL re = FALSE;
	TMapScene::iterator i = m_MapIDScene.find(dwMapID);
	if( nSelfInfoNdx == 0 && i == m_MapIDScene.end() )
		return FALSE;
	if( i == m_MapIDScene.end() )
	{
		if( nSelfInfoNdx > 0 )
		{
			INT mlen = 0;
			INT mem = 0;
			if( P_VALID(((Role*)pUnit)->GetTeamID()) )
			{
				const Team * pTeam = g_groupMgr.GetTeamPtr(((Role*)pUnit)->GetTeamID());
				if( P_VALID(pTeam) )
				{
					mem = pTeam->GetMemNum();
				}
			}
			if( !temPlyInfo.strContent.empty() )
				mlen = temPlyInfo.strContent.length();
			const TCHAR * pM = NULL;
			if( mlen > 0 )
				pM = temPlyInfo.strContent.c_str();
			if( PushBackEasyTeamSimInfo(pMemPool,pMsg,temPlyInfo.dwRoleID,pUnit->GetLevel(),mem,temPlyInfo.byTeamNum,(EEasyTeamType)temPlyInfo.byTeamType,mlen,pM,temPlyInfo.byTeamTypeDetail) > 0 )
			{
				team_leaders.insert(temPlyInfo.dwRoleID);
				re = TRUE;
			}
		}
		return re;
	}

	//CHECK_TIME_WITH_ALLPARAM_EX("easyteam GetZuduiInfo",80,TRUE);

	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(_queu_cs);
	vector<INT> & scenes = i->second;
	INT nTeamNum = 0;
	INT nLevelMin = 0, nLevelMax = 0;
	vector<INT> _vecToDelCTInfo;

	for( vector<INT>::iterator it = scenes.begin(); it != scenes.end(); ++it )
	{
		TSceneCTInfos::iterator iNner = m_SceneCTInfos.find(*it);
		if( iNner == m_SceneCTInfos.end() )
			continue;

		vector<INT> & vCTInfos = iNner->second;
		for( vector<INT>::iterator a = vCTInfos.begin(); a != vCTInfos.end(); ++a )
		{
			if(*a < 0)
				continue;
			tagCTInfo * pInfo = GetCTInfo(*a);
			if( !P_VALID(pInfo) )
				continue;
			Role * ply = g_roleMgr.GetRolePtrByID(pInfo->dwRoleID);
			if( !P_VALID(ply) )
				continue;

			if( (pUnit->GetID() != ply->GetID()) && pUnit->GetLevel() < pInfo->byMinLevel || pUnit->GetLevel() > pInfo->byMaxLevel )
				continue;

			INT mem = 0;
			if( P_VALID(ply->GetTeamID()) )
			{
				const Team * pTeam = g_groupMgr.GetTeamPtr(ply->GetTeamID());
				if( P_VALID(pTeam) )
				{
					mem = pTeam->GetMemNum();
					INT nTeamMemLimit = min(MAX_TEAM_NUM,pInfo->byTeamNum);
					if( mem >= nTeamMemLimit/*pInfo->*/ ) // 组队人数限制
					{
						//DeleteCTInfo(pInfo->byScene);
						_vecToDelCTInfo.push_back(pInfo->byScene);
						continue;
					}
				}
			}

			INT mlen = 0;
			if( !pInfo->strContent.empty() )
				mlen = pInfo->strContent.length();

			const TCHAR * pM = NULL;
			if( mlen > 0 )
				pM = pInfo->strContent.c_str();

			if( nSelfInfoNdx == *a )
				nSelfInfoNdx = 0;

			if( PushBackEasyTeamSimInfo(pMemPool,pMsg,pInfo->dwRoleID,ply->GetLevel(),mem,pInfo->byTeamNum,(EEasyTeamType)pInfo->byTeamType,mlen,pM,pInfo->byTeamTypeDetail) > 0 )
			{
				team_leaders.insert(pInfo->dwRoleID);
				++re;
				if( pMsg->dwSize >= m_nPckLimit )
					return re;
			}
		}
	}
	if( _vecToDelCTInfo.size() > 0 )
	{
		for( vector<int>::iterator iToDel = _vecToDelCTInfo.begin(); iToDel != _vecToDelCTInfo.end(); ++iToDel )
		{
			DeleteCTInfo(*iToDel);
		}
	}
	if( nSelfInfoNdx > 0 )
	{
		INT mlen = 0;
		INT mem = 0;
		if( P_VALID(((Role*)pUnit)->GetTeamID()) )
		{
			const Team * pTeam = g_groupMgr.GetTeamPtr(((Role*)pUnit)->GetTeamID());
			if( P_VALID(pTeam) )
			{
				mem = pTeam->GetMemNum();
			}
		}
		if( !temPlyInfo.strContent.empty() )
			mlen = temPlyInfo.strContent.length();
		const TCHAR * pM = NULL;
		if( mlen > 0 )
			pM = temPlyInfo.strContent.c_str();
		if( PushBackEasyTeamSimInfo(pMemPool,pMsg,temPlyInfo.dwRoleID,pUnit->GetLevel(),mem,temPlyInfo.byTeamNum,(EEasyTeamType)temPlyInfo.byTeamType,mlen,pM,temPlyInfo.byTeamTypeDetail) > 0 )
		{
			team_leaders.insert(temPlyInfo.dwRoleID);
			re = TRUE;
		}
	}

	return re;
}


VOID ConvenientTeam::Init(VarContainer * pVar,LPCTSTR field)
{
	if( P_VALID(pVar) && field )
	{
		DWORD dwMapID = 0;
		
		INT nScene = pVar->GetInt(_T("id"),field);
		TCHAR strMap[50] = {0};

		for( int i = 0; i < 10 ; ++i )
		{
			_stprintf(strMap,_T("map%02d_id"),i+1);
			LPCTSTR sMap	= pVar->GetString(strMap,field,NULL);
			if( P_VALID(sMap) )
			{
				dwMapID = IUTIL->Crc32(sMap);

				TMapScene::iterator ifind = m_MapIDScene.find(dwMapID);
				if( ifind != m_MapIDScene.end() )
				{
					vector<INT> &t = ifind->second;
					t.push_back(nScene);
				}
				else
				{
					vector<INT> t;
					t.push_back(nScene);
					m_MapIDScene[dwMapID] = t;
				}
			}
		}
	}
}


VOID ConvenientTeam::LimitTeamInfoNum(INT num)
{
	m_nPlyNumLimit = num;
}


