//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: 公会战宣战管理器
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "guild_war_declare_mgr.h"
#include "../WorldDefine/msg_guild_war_declare.h"
#include "../ServerDefine/msg_guild_war_declare.h"
#include "db_session.h"
#include "role.h"

#define ONE_HOUR 3600 // 60*60,一个小时有多少秒


CGuildWarDeclare::CGuildWarDeclare( DWORD 			dwAttackGuildID,			// 宣战工会ID
									DWORD 			dwDefenceGuildID,			// 被宣战工会ID
									BOOL			bWarEnded,					// 宣战是不是已经结束过了
									tagDWORDTime	dwWarDeclareTime)			// 宣战时间
:m_dwAttackGuildID(dwAttackGuildID),
m_dwDefenceGuildID(dwDefenceGuildID),
m_bWarEnded(bWarEnded),
m_dwWarDeclareTime(dwWarDeclareTime)
{
}

// 给两个公会所有在线角色增加工会战buff
VOID CGuildWarDeclare::AddBuffToAllOnlineRole()
{
	Guild* pAttackGuild = g_guildMgr.GetGuild(m_dwAttackGuildID);
	if (P_VALID(pAttackGuild))
	{
		pAttackGuild->GetMemberMgr().AddBuffToAllOnLineMember(GUILD_WAR_BUFF_ID);
	}
	Guild* pDefenceGuild = g_guildMgr.GetGuild(m_dwDefenceGuildID);
	if (P_VALID(pDefenceGuild))
	{
		pDefenceGuild->GetMemberMgr().AddBuffToAllOnLineMember(GUILD_WAR_BUFF_ID);
	}
}

// 删除两个公会所有在线角色增加工会战buff
VOID CGuildWarDeclare::DeleteBuffOnAllOnlineRole(INT nDelType/*=0*/)
{
	if( 0 == nDelType || 1 == nDelType)
	{
		Guild* pAttackGuild = g_guildMgr.GetGuild(m_dwAttackGuildID);
		if (P_VALID(pAttackGuild))
		{
			pAttackGuild->GetMemberMgr().RemoveBuffFromAllOnLineMember(GUILD_WAR_BUFF_ID);
		}
	}
	if( 0 == nDelType || 2 == nDelType)
	{
		Guild* pDefenceGuild = g_guildMgr.GetGuild(m_dwDefenceGuildID);
		if (P_VALID(pDefenceGuild))
		{
			pDefenceGuild->GetMemberMgr().RemoveBuffFromAllOnLineMember(GUILD_WAR_BUFF_ID);
		}
	}
}

// 将宣战关系保存到数据库
VOID CGuildWarDeclare::SaveGuildWarDeclare2DB()
{
	tagNDBC_InsertGuildWarDeclare send;
	send.dwAttackGuildID = m_dwAttackGuildID;
	send.dwDefenceGuildID = m_dwDefenceGuildID;
	send.dwWarDeclareTime = m_dwWarDeclareTime;
	send.bWarEnded = m_bWarEnded;

	g_dbSession.Send(&send, send.dwSize);
}

// 检查战争结束时间是否到了
BOOL CGuildWarDeclare::IsGuildWarEnd()
{
	if (CalcTimeDiff(GetCurrentDWORDTime(), m_dwWarDeclareTime) >= ONE_HOUR)
	{
		return TRUE;
	}
	return FALSE;
}

// 公会战争结束了
VOID CGuildWarDeclare::GuildWarEnd()
{
	tagNS_GuildWarEnd send;
	send.dwAttackGuildID = m_dwAttackGuildID;
	send.dwDefenceGuildID = m_dwDefenceGuildID;

	// 给宣战帮派发送战争结束消息
	Guild* pAttackGuild = g_guildMgr.GetGuild(m_dwAttackGuildID);
	if (P_VALID(pAttackGuild))
	{
		pAttackGuild->SendGuildMsg(&send, send.dwSize);
	}

	// 给被宣战帮派发送战争结束消息
	Guild* pDefenceGuild = g_guildMgr.GetGuild(m_dwDefenceGuildID);
	if (P_VALID(pDefenceGuild))
	{
		pDefenceGuild->SendGuildMsg(&send, send.dwSize);
	}

	// 标识已经结束过了
	m_bWarEnded = TRUE;

	// 存入数据库
	tagNDBC_SetGuildWarDeclareEnded send2DB;
	send2DB.dwAttackGuildID = m_dwAttackGuildID;
	send2DB.dwDefenceGuildID = m_dwDefenceGuildID;
	send2DB.dwWarDeclareTime = m_dwWarDeclareTime;
	g_dbSession.Send(&send2DB, send2DB.dwSize);
}

// 公会战宣战管理器类
CGuildWarDeclareMgr::CGuildWarDeclareMgr()
{

}

CGuildWarDeclareMgr::~CGuildWarDeclareMgr()
{
	// 释放强制脱离师门申请列表
	DestoryGuildWarDeclareMap();
}

VOID CGuildWarDeclareMgr::Init()
{
	// 载入数据库中的工会宣战关系
	LoadAllGuildWarDeclareFromDB();
}

VOID CGuildWarDeclareMgr::Update()
{
	CHECK_TIME()
	MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
	CGuildWarDeclare* pGuildWarDeclare = NULL;
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if (!pGuildWarDeclare->IsGuildWarEnded() && pGuildWarDeclare->IsGuildWarEnd())
				{
					INT nType = 0;
					if( GetGuildWarNum(pGuildWarDeclare->GetAttackGuildID()) > 1)
					{
						nType = 2;
					}

					if( GetGuildWarNum(pGuildWarDeclare->GetDefenceGuildID()) > 1)
					{
						if( 2 == nType)
						{
							nType = 3;
						}
						else
						{
							nType = 1;
						}
					}

					// 结束了，删除buff
					pGuildWarDeclare->DeleteBuffOnAllOnlineRole(nType);

					// 公会战争结束了
					pGuildWarDeclare->GuildWarEnd();
				}
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}
}

// 将宣战关系添加到map里面
VOID CGuildWarDeclareMgr::AddGuildWarDeclare2Map(DWORD dwAttackGuildID,			// 宣战工会ID
												 DWORD dwDefenceGuildID)		// 被宣战工会ID
{
	// 将宣战关系添加到map里面
	AddGuildWarDeclare2Map(dwAttackGuildID, dwDefenceGuildID, GetCurrentDWORDTime());
}

// 初始化宣战关系map
VOID CGuildWarDeclareMgr::InitGuildWarDeclareMap(DWORD dwGuildWarDeclareCnt, tagGuildWarDeclare* pGuildWarDeclare)
{
	if (dwGuildWarDeclareCnt == 0 || !P_VALID(pGuildWarDeclare))
	{
		return;
	}

	for (DWORD dwCnt = 0; dwCnt < dwGuildWarDeclareCnt; dwCnt++)
	{
		// 将宣战关系添加到map里面
		AddGuildWarDeclare2Map(pGuildWarDeclare[dwCnt].dwAttackGuildID, pGuildWarDeclare[dwCnt].dwDefenceGuildID, pGuildWarDeclare[dwCnt].dwWarDeclareTime, pGuildWarDeclare[dwCnt].bWarEnded, FALSE);
	}
}

// 清除数据库中过期的宣战关系
VOID CGuildWarDeclareMgr::ClearExpireGuildDeclareInDB()
{
	tagNDBC_ClearExpireGuildWarDeclare send;
	g_dbSession.Send(&send, send.dwSize);

	// 清除内存中过期的宣战关系
	tagDWORDTime dwCurTime = DecreaseTime(GetCurrentDWORDTime(), ONE_HOUR);
	
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator eraseIterator;
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if (pGuildWarDeclare->GetWarDeclareTime() < dwCurTime)
				{
					eraseIterator = iterator;
					iterator++;
					// 过期了，删除之
					m_mapGuildWarDeclareList.erase(eraseIterator);
					SAFE_DEL(pGuildWarDeclare);
				}
				else
				{
					iterator++;
				}
				
			}
			if (dwCnt < 0)
				break;
		}
	}

	// 通知所有公会恢复剩余宣战次数
	tagNS_ResetRemainDeclareTimes send2Client;
	g_guildMgr.SendMsgToAllGuild(&send2Client, send2Client.dwSize);
}

// 同步与角色所属帮派处于战争状态的帮派
VOID CGuildWarDeclareMgr::SyncWarStateGuild(Role* pRole)
{
	if (!P_VALID(pRole))
	{
		return;
	}

	DWORD dwGuildID = pRole->GetGuildID();
	INT nGuildCnt = 0;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	// 取帮派个数和帮派列表
	TList<DWORD> listGuildID;
	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if (pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
					pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
					(pGuildWarDeclare->GetAttackGuildID() == dwGuildID ||
					pGuildWarDeclare->GetDefenceGuildID() == dwGuildID))
				{
					nGuildCnt++;
					DWORD dwEnemyGuildID = (pGuildWarDeclare->GetAttackGuildID() == dwGuildID)?
						pGuildWarDeclare->GetDefenceGuildID():pGuildWarDeclare->GetAttackGuildID();
					listGuildID.PushBack(dwEnemyGuildID);
				}
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}

	// 如果没有帮派，则返回
	if (nGuildCnt <= 0)
	{
		tagNS_SyncWarStateGuild syncWarStateGuild;
		syncWarStateGuild.nGuildCount = 0;
		syncWarStateGuild.nRemainWarDeclareTimes = (MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY <= GetTodayDeclareWarTimes(dwGuildID))?
			0:MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY - GetTodayDeclareWarTimes(dwGuildID);
		pRole->SendMessage(&syncWarStateGuild, syncWarStateGuild.dwSize);
		return;
	}

	// 创建消息
	DWORD dwSize = sizeof(tagNS_SyncWarStateGuild) + (nGuildCnt-1)*sizeof(DWORD);
	MCREATE_MSG(pSend, dwSize, NS_SyncWarStateGuild);
	if (!P_VALID(pSend))
	{
		return;
	}

	listGuildID.ResetIterator();
	pSend->nGuildCount = nGuildCnt;
	for (INT nCnt = 0; nCnt < nGuildCnt; nCnt++)
	{
		if (listGuildID.PeekNext(pSend->dwGuildID[nCnt]) == FALSE)
		{
			break;
		}
	}

	// 给角色添加公会战buff
	const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(GUILD_WAR_BUFF_ID);
	if ( P_VALID(pBuffProto))
	{
		pRole->TryAddBuff(pRole, pBuffProto, NULL, NULL, NULL);
	}

	pSend->nRemainWarDeclareTimes = (MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY <= GetTodayDeclareWarTimes(dwGuildID))?
		0:MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY - GetTodayDeclareWarTimes(dwGuildID);

	// 发送消息
	pRole->SendMessage(pSend, pSend->dwSize);
}

// 取指定帮派当天对外宣战次数
BYTE CGuildWarDeclareMgr::GetTodayDeclareWarTimes(DWORD dwGuildID)
{
	if (dwGuildID == 0)
		return 0;

	INT nGuildCnt = 0;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.find(dwGuildID);
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;

			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				GetCurrentDWORDTime().day == pGuildWarDeclare->GetWarDeclareTime().day &&
				pGuildWarDeclare->GetAttackGuildID() == dwGuildID)
			{
				nGuildCnt++;		
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}
	return nGuildCnt;
}

// 取指定帮派对同一个指定帮派当天的宣战次数
BYTE CGuildWarDeclareMgr::GetTodaySameGuildBeWarTimes( DWORD dwAttackGuildID,			// 宣战帮派ID
													   DWORD dwDefenceGuildID)			// 被宣战帮派ID
{
	if (dwAttackGuildID == 0 || dwDefenceGuildID == 0)
		return 0;

	INT nGuildCnt = 0;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.find(dwAttackGuildID);
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				GetCurrentDWORDTime().day == pGuildWarDeclare->GetWarDeclareTime().day &&
				pGuildWarDeclare->GetAttackGuildID() == dwAttackGuildID &&
				pGuildWarDeclare->GetDefenceGuildID() == dwDefenceGuildID)
			{
				nGuildCnt++;		
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}

	return nGuildCnt;
}

// 载入数据库中的公会宣战关系
VOID CGuildWarDeclareMgr::LoadAllGuildWarDeclareFromDB()
{
	tagNDBC_LoadGuildWarDeclare send;

	g_dbSession.Send(&send, send.dwSize);
}

// 释放公会宣战关系map
VOID CGuildWarDeclareMgr::DestoryGuildWarDeclareMap()
{
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			SAFE_DEL(pGuildWarDeclare);
			iterator++;
			if (dwCnt < 0)
				break;
		}
		m_mapGuildWarDeclareList.clear();
	}

}

// 将宣战关系添加到map里面
VOID CGuildWarDeclareMgr::AddGuildWarDeclare2Map(DWORD 			dwAttackGuildID,			// 宣战工会ID
												 DWORD 			dwDefenceGuildID,			// 被宣战工会ID
												 tagDWORDTime	dwWarDeclareTime,			// 宣战时间
												 BOOL			bWarEnded/* = FALSE*/,		// 宣战是不是已经结束过了
												 BOOL			bNeedSave2DB /*= TRUE*/) 	// 是否要存数据库
{
	if (dwAttackGuildID == 0 || dwDefenceGuildID == 0)
		return;

	CGuildWarDeclare* pGuildWarDeclare = new CGuildWarDeclare(dwAttackGuildID, dwDefenceGuildID, bWarEnded, dwWarDeclareTime);
	if (P_VALID(pGuildWarDeclare))
	{
		m_mapGuildWarDeclareList.insert(std::make_pair(dwAttackGuildID, pGuildWarDeclare));
		if (bNeedSave2DB)
		{
			// 将数据存入数据库
			pGuildWarDeclare->SaveGuildWarDeclare2DB();
		}

		// 宣战，添加buff
		pGuildWarDeclare->AddBuffToAllOnlineRole();
	}
}

// 当有公会解散时触发
VOID CGuildWarDeclareMgr::OnGuildDismiss(DWORD dwGuildID)
{
	if (dwGuildID == 0)
		return;

	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
				pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
				(pGuildWarDeclare->GetAttackGuildID() == dwGuildID ||
				pGuildWarDeclare->GetDefenceGuildID() == dwGuildID))
			{
				// 结束了，删除buff
				pGuildWarDeclare->DeleteBuffOnAllOnlineRole();

				// 公会战争结束了
				pGuildWarDeclare->GuildWarEnd();
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}
}

// 当有角色退出帮派时候触发
VOID CGuildWarDeclareMgr::OnRoleLeaveGuild(Role* pRole)
{
	if (!P_VALID(pRole))
	{
		return;
	}

	// 如果身上有公会战争的buff，删除它
	if (pRole->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID))
	{
		pRole->RemoveBuff(GUILD_WAR_BUFF_TYPE_ID, TRUE);
	}

	// 同步公会战争状态
	tagNS_SyncWarStateGuild send;
	send.nGuildCount = 0;
	send.nRemainWarDeclareTimes = 0;
	pRole->SendMessage(&send, send.dwSize);
}

// 当有角色加入公会时候触发
VOID CGuildWarDeclareMgr::OnRoleJoinGuild(Role* pRole)
{
	if (!P_VALID(pRole))
	{
		return;
	}

	BOOL bGuildInWar = FALSE;
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
				pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
				(pGuildWarDeclare->GetAttackGuildID() == pRole->GetGuildID() ||
				pGuildWarDeclare->GetDefenceGuildID() == pRole->GetGuildID()))
			{
				// 如果身上没有有公会战争的buff，添加一个
				if (pRole->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID) == FALSE)
				{
					const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(GUILD_WAR_BUFF_ID);
					if (P_VALID(pBuffProto))
					{
						pRole->TryAddBuff(pRole, pBuffProto, NULL, NULL, NULL);
					}
				}
				bGuildInWar = TRUE;
				break;
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}
	if (bGuildInWar)
	{
		// 同步公会战争状态
		SyncWarStateGuild(pRole);
	}
}

// 两个公会是否有敌对关系
BOOL CGuildWarDeclareMgr::IsInGuildWarState(Role* pRole1, Role* pRole2)
{
	if (!P_VALID(pRole1) || !P_VALID(pRole2))
	{
		return FALSE;
	}
	if (!pRole1->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID) || !pRole2->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID))
	{
		return FALSE;
	}
	DWORD dwGuildID1 = pRole1->GetGuildID();
	DWORD dwGuildID2 = pRole2->GetGuildID();

	return IsInGuildWarState(dwGuildID1, dwGuildID2);
}

BOOL CGuildWarDeclareMgr::IsInGuildWarState(DWORD dwGuildID1, DWORD dwGuildID2)
{
	INT dwCnt = m_mapGuildWarDeclareList.size();

	if (dwCnt > 0)
	{
		MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
		CGuildWarDeclare* pGuildWarDeclare = NULL;
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare) &&
				pGuildWarDeclare->IsGuildWarEnd() == FALSE &&
				pGuildWarDeclare->IsGuildWarEnded() == FALSE &&
				((pGuildWarDeclare->GetAttackGuildID() == dwGuildID1 &&
				pGuildWarDeclare->GetDefenceGuildID() == dwGuildID2) ||
				(pGuildWarDeclare->GetAttackGuildID() == dwGuildID2 &&
				pGuildWarDeclare->GetDefenceGuildID() == dwGuildID1)))
			{
				return TRUE;
			}
			iterator++;
			if (dwCnt < 0)
				break;
		}
	}
	return FALSE;
}

// 当角色下线时触发
VOID CGuildWarDeclareMgr::OnRoleLogout(Role* pRole)
{
	// 如果角色身上有公会战buff，要删掉
	if (pRole->IsHaveBuff(GUILD_WAR_BUFF_TYPE_ID))
	{
		pRole->RemoveBuff(GUILD_WAR_BUFF_TYPE_ID, TRUE);
	}
}


INT CGuildWarDeclareMgr::GetGuildWarNum( DWORD dwGuildID )
{
	MultiMapIterator iterator = m_mapGuildWarDeclareList.begin();
	CGuildWarDeclare* pGuildWarDeclare = NULL;
	INT dwCnt = m_mapGuildWarDeclareList.size();
	INT nNum = 0;

	if (dwCnt > 0)
	{
		while (iterator != m_mapGuildWarDeclareList.end())
		{
			dwCnt--;
			pGuildWarDeclare = iterator->second;
			if (P_VALID(pGuildWarDeclare))
			{
				if( dwGuildID == pGuildWarDeclare->GetAttackGuildID() )
				{
					nNum++;
				}

				if( dwGuildID ==  pGuildWarDeclare->GetDefenceGuildID() )
				{
					nNum++;
				}
			}
			iterator++;

			if (dwCnt < 0)
				break;
		}
	}

	return nNum;
}


CGuildWarDeclareMgr g_guildWarDeclareMgr;