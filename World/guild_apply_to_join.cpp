#include "stdafx.h"
#include "guild_mgr.h"
#include "guild_member.h"
#include "role.h"
#include "guild_apply_to_join.h"
#define __USE_JM_TOOL_TEMP
#include "../ServerDefine/mini_tool.h"
#include "../worlddefine/msg_guild.h"
#include <queue>
#include "guild.h"
#include "role_mgr.h"
#include "hash_map"
#include <iostream>
//#include <fstream>
#include <algorithm>
#include "guild_war_declare_mgr.h"


/**
 * 效率不佳啊，应该缓存关键数据
 * \param p 
 * \return 
 */
bool operator < (const tagGuildPriority & t,const tagGuildPriority & p)
{
	bool re = false;
	Guild * pGuild = g_guildMgr.GetGuild(t.dwGuildID);
	Guild * pOtherGuild = g_guildMgr.GetGuild(p.dwGuildID);
	
	if( !P_VALID(pGuild) || !P_VALID(pOtherGuild) )
		return false;

	int nMemberNum = pGuild->GetMemberMgr().GetNumber() - pOtherGuild->GetMemberMgr().GetNumber();
	if( nMemberNum > 0 )
		return false;
	else if( nMemberNum < 0 )
		return true;

	if( pGuild->GetGuildAtt().byLevel > pOtherGuild->GetGuildAtt().byLevel )
		return false;
	else if( pGuild->GetGuildAtt().byLevel < pOtherGuild->GetGuildAtt().byLevel )
		return true;
	
	tagRoleInfo * pRoleInfo = g_roleMgr.GetRoleInfo(pGuild->GetGuildAtt().dwLeaderRoleID);
	tagRoleInfo * pOtherRoleInfo = g_roleMgr.GetRoleInfo( pOtherGuild->GetGuildAtt().dwLeaderRoleID );

	if( !P_VALID(pRoleInfo) && P_VALID(pOtherRoleInfo) )
		return true;	
	if( P_VALID(pRoleInfo) && !P_VALID(pOtherRoleInfo) )
		return false;

	if( !P_VALID(pRoleInfo) || !P_VALID(pOtherRoleInfo) )
		return false;

	if( pRoleInfo->byLevel > pOtherRoleInfo->byLevel )
		return false;
	else if( pRoleInfo->byLevel < pOtherRoleInfo->byLevel )
		return true;

	return pGuild->GetGuildAtt().dwCreateTime < pOtherGuild->GetGuildAtt().dwCreateTime;
};
bool operator == (const tagGuildPriority & t,const tagGuildPriority & p)
{
	return t.dwGuildID == p.dwGuildID;
}

struct MyGuildSingletonAdaptor;


/**
 * \ingroup LoongWorld
 *	该类只是简单的实现了，服务器重启时刻帮派排序功能，没有动态实现，不过，还可以使用stl的堆函数来实现动态的，时间复杂度 Nlog(N)
 *
 * \par requirements
 *
 * \version 1.0
 * first version
 *
 * \date 2010-02-11
 *
 * \author Jason
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class JMApplyToJoinGuildToolkit : public ApplyToJoinGuildToolkit
{
public:

	enum{ E_Bill_Valve = 100, E_GuildRanking_Valve = 100 };
private:
	struct TDAarry{
		DWORD vec[E_Bill_Valve];
		TDAarry()
		{
			memset(vec,0,sizeof(vec));
		}
		void push_back(DWORD t)
		{
			for(int i = 0 ; i < E_Bill_Valve; ++i )
			{
				if( 0 == vec[i] )
				{
					vec[i] = t;
					break;
				}
			}
		}
		TDAarry(const TDAarry & t)
		{
			memcpy(vec,t.vec,sizeof(vec));
		}
		TDAarry& operator=(const TDAarry & t)
		{
			memcpy(vec,t.vec,sizeof(vec));
			return *this;
		}
		INT find(DWORD t)
		{
			for(int i = 0 ; i < E_Bill_Valve; ++i )
			{
				if( t == vec[i] )
				{
					return i;
				}
			}
			return GT_INVALID;
		}
		void set(int index,DWORD t)
		{
			if( index >=0 && index < E_Bill_Valve )
				vec[index] = t;
		}
		INT size()
		{
			int r = 0;
			for(int i = 0 ; i < E_Bill_Valve; ++i )
			{
				if( 0 != vec[i] )
					++r;
			}
			return r;
		}
		DWORD operator[](int ndx)
		{
			if( ndx >=0 && ndx < E_Bill_Valve )
				return vec[ndx];
			return GT_INVALID;
		}
	};
	typedef TDAarry _TBillCon;
	friend struct MyGuildSingletonAdaptor;
	std::map<DWORD,_TBillCon> m_mapApplierBill;		// 申请入帮派清单，最大限度是100个

	std::map<DWORD, DWORD>	m_mapRoleGuilds;

	//std::priority_queue<tagGuildPriority> m_GuildQueue;
	INT	m_nRankNum;
	BYTE * m_buf;
	size_t m_nBufSize;
	stdext::hash_map<DWORD,size_t> _appliers_and_guild_leader;
	std::vector<DWORD> _clear_info;

	std::vector<tagGuildPriority> m_guildinfos;

	INT m_nRankingSize;

	jm_tool::CSCreatedLock _cs;

	BOOL m_bRankingChanged;

	BOOL is_changed(void)
	{
		return m_bRankingChanged;
	}
	void reset_guildranking(void);
protected:
	JMApplyToJoinGuildToolkit()
		:m_nRankNum(0),m_buf(NULL),m_nBufSize(0),m_nRankingSize(0),m_bRankingChanged(FALSE)
	{
		;
	}
	virtual ~JMApplyToJoinGuildToolkit()
	{
		if(m_buf)
			g_MemPoolSafe.Free(m_buf);
	}
private:
	tagGuildSmallInfo m_GuildRanking[E_GuildRanking_Valve] ;
public:

	virtual VOID AddGuild(DWORD dwGuildID)
	{
		if( !P_VALID(dwGuildID) )
			return;
		if( !P_VALID( g_guildMgr.GetGuild(dwGuildID) ) )
			return;

		jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
		//m_GuildQueue.push(dwGuildID);
		m_guildinfos.push_back(dwGuildID);
		++m_nRankingSize;
		m_bRankingChanged = TRUE;
	}
	virtual VOID DelGuild(DWORD dwGuildID)
	{
		if( !P_VALID(dwGuildID) )
			return;
// 		if( !P_VALID( g_guildMgr.GetGuild(dwGuildID) ) )
// 			return;

		jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
		std::vector<tagGuildPriority>::iterator i = std::find(m_guildinfos.begin(),m_guildinfos.end(),tagGuildPriority(dwGuildID));
		if( i != m_guildinfos.end() )
		{
			m_guildinfos.erase(i);
			--m_nRankingSize;
			m_bRankingChanged = TRUE;
		}
	}

	virtual DWORD ApplyToJoinGuild(Role * pRole,DWORD dwGuildID)
	{
		if( P_VALID(pRole) && P_VALID(dwGuildID) )
		{
			tagNS_WantToJoinGuild msg;
			DWORD dwGuildID0 = pRole->GetGuildID();
			if( !P_VALID( dwGuildID0 ) )
			{
				Guild* pGuild = g_guildMgr.GetGuild(dwGuildID);
				if( P_VALID(pGuild) )
				{
					if( pGuild->IsInGuildState(EGDSS_Warfare) )
						return E_Guild_State_Limit;

					DWORD dwRoleID = pRole->GetID();
					GuildMember & guildMem = pGuild->GetMemberMgr();
					if( guildMem.IsExist(dwRoleID) )
					{
						msg.dwErrorCode = E_Guild_Join_AlreadyIn;
						pRole->SendMessage(&msg,msg.dwSize);
						return 0;
					}
					INT nMaxRoleNum = MGuildMaxMember(pGuild->GetGuildAtt().byLevel);
					if( guildMem.GetNumber() >= nMaxRoleNum )
					{
						msg.dwErrorCode = E_GuildApply_MemberTooMuch;
						pRole->SendMessage(&msg,msg.dwSize);
						return 0;
					}

					jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
					std::map<DWORD,_TBillCon>::iterator i = m_mapApplierBill.find(dwGuildID);
					if( i == m_mapApplierBill.end() )
					{
						// 申请过别的帮派
						if( m_mapRoleGuilds.find(dwRoleID) != m_mapRoleGuilds.end() )
						{
							msg.dwErrorCode = E_GuildApply_YouHaveAlreadyApplied;
							pRole->SendMessage(&msg,msg.dwSize);
							return GT_INVALID;
						}
						_TBillCon tem ;
						tem.push_back( dwRoleID );
						m_mapApplierBill[dwGuildID] = tem;
						m_mapRoleGuilds[dwRoleID] = dwGuildID;
					}
					else
					{
						// 申请过别的帮派
						if( m_mapRoleGuilds.find(dwRoleID) != m_mapRoleGuilds.end() )
						{
							msg.dwErrorCode = E_GuildApply_YouHaveAlreadyApplied;
							pRole->SendMessage(&msg,msg.dwSize);
							return GT_INVALID;
						}
						int ndx = i->second.find(dwRoleID);

						if( ndx != GT_INVALID )
						{
							msg.dwErrorCode = E_GuildApply_YouHaveAlreadyApplied;
						}
						else
						{
							if( i->second.size() < E_Bill_Valve )
							{
								i->second.push_back(dwRoleID);
								msg.dwErrorCode = E_Success;

								_appliers_and_guild_leader[pGuild->GetGuildAtt().dwLeaderRoleID] = i->second.size();
								m_mapRoleGuilds[dwRoleID] = dwGuildID;
							}
							else
								msg.dwErrorCode = E_GuildApply_ApplyTooMuch;
						}
					}
				}
				else
				{
					msg . dwErrorCode = E_GuildApply_NotExist;
				}
			}
			else
			{
				msg.dwErrorCode = E_GuildApply_YouAlredyHaveGuild;
			}
			pRole->SendMessage(&msg,msg.dwSize);
		}
		return 0;
	}
	virtual DWORD ResponseApplierToJoinGuild(Role * pGuildOwner,DWORD dwSrcRoleID,DWORD dwGuildID)
	{
		//jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
		return 0;
	}
	virtual DWORD GetApplyBill(Role * pRole,DWORD dwGuildID)
	{
		if( P_VALID(pRole) )
		{
			tagNS_GuildApplierBill msg;
			msg.dwErrorCode = E_Success;
			msg.n16BillNum = 0;
			DWORD dwGuildOwnerID = pRole->GetID();
			if( P_VALID(dwGuildID) )
			{
				Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
				if( P_VALID(pGuild) )
				{
					if( pRole->GetID() == pGuild->GetGuildAtt().dwLeaderRoleID )
					{
						jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
						std::map<DWORD,_TBillCon>::iterator i = m_mapApplierBill.find(dwGuildID);
						if( i != m_mapApplierBill.end() )
						{
							_TBillCon& vec = i->second;
							msg.n16BillNum = vec.size();
							if( msg.n16BillNum > 0 )
							{
								if( msg.n16BillNum > E_Bill_Valve )
									msg.n16BillNum = E_Bill_Valve;

								size_t size = sizeof(tagNS_GuildApplierBill) + ( msg.n16BillNum - 1 ) * sizeof(tagGuildApplierBillInfo) ;
								msg.dwSize = size;
								BYTE * buff = (BYTE*)g_MemPoolSafe.Alloc(size);
								tagGuildApplierBillInfo * p = (tagGuildApplierBillInfo*)( buff + sizeof(tagNS_GuildApplierBill) - sizeof(tagGuildApplierBillInfo) );
								memcpy(buff,&msg,sizeof(msg));

								tagNS_GuildApplierBill * pMsg = (tagNS_GuildApplierBill*)buff;

								tagDWORDTime now = g_world.GetWorldTime();

								for(int j = 0; j < msg.n16BillNum; ++j)
								{
									p->dwRoleID = vec[j];
									tagRoleInfo * roleinfo = g_roleMgr.GetRoleInfo(p->dwRoleID);
									if( P_VALID(roleinfo) )
									{
										p->byLevel = roleinfo->byLevel;
										p->byOnline = roleinfo->bOnline;
										p->bySex	= roleinfo->bySex;
										p->byVaction= roleinfo->byClass;
										if( p->byOnline ) 
											p->dwOfflineTime = 0;
										else
											p->dwOfflineTime = CalcTimeDiff(now,roleinfo->dwTimeLastLogout);
										INT len = _tcslen( roleinfo->szRoleName );
										if( len > X_SHORT_NAME - 1 )
											len = X_SHORT_NAME - 1;
										_tcsncpy(p->strRoleName,roleinfo->szRoleName,len);
										p->strRoleName[len] = 0;
										++p;
									}
									else
										pMsg->n16BillNum--;
								}

								pRole->SendMessage(buff,size);
								g_MemPoolSafe.Free(buff);

								return E_Success;
							}
						}
					}
					else
						msg.dwErrorCode = E_Guild_Power_NotEnough;
				}
				else
					msg.dwErrorCode = E_GuildApply_NotExist;
			}
			pRole->SendMessage(&msg,msg.dwSize);
			return E_Success;
		}
		return GT_INVALID;
	}
	virtual DWORD GetGuildTenet( Role * pRole,DWORD dwGuildID )
	{
		if( P_VALID(pRole) )
		{
			tagNS_GetGuildPurpose msg;
			msg.dwGuildID = dwGuildID;
			memset(msg.szContent,0,sizeof(msg.szContent));
			memset(msg.szGuildName,0,sizeof(msg.szGuildName));

			Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
			if( P_VALID(pGuild) )
			{
				INT len = pGuild->GetGuildAtt().strName.length();
				if( len > X_SHORT_NAME - 1 )
					len = X_SHORT_NAME - 1;
				_tcsncpy(msg.szGuildName,pGuild->GetGuildAtt().strName.c_str(),len);
				msg.szGuildName[len] = 0;
				len = pGuild->GetGuildAtt().strTenet.length();
				if ( len > X_LONG_NAME - 1 )
					len = X_LONG_NAME - 1;

				_tcsncpy(msg.szContent,pGuild->GetGuildAtt().strTenet.c_str(),len);
				msg.szContent[len] = 0;
			}
			else
				msg.dwErrorCode = E_GuildApply_NotExist;

			pRole->SendMessage(&msg,msg.dwSize);
			return E_Success;
		}
		return GT_INVALID;
	}

	virtual DWORD GetGuildRanking(Role * pRole)
	{
		if( P_VALID(pRole) )
		{
			//jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);

			//reset_guildranking();

			if( m_buf && m_nBufSize > 0 )
			{
				//static int iF = 0;
				//if( 0 == iF )
				//{
				//	iF = 1;
				//	tagNS_GetAllGuildSmallInfo * p = (tagNS_GetAllGuildSmallInfo *)m_buf;
				//	tagGuildSmallInfo * pZ = p->guilds;
				//	std::wofstream  fs("e:\\out.dat");
				//	for( int i = 0; i < p->nGuildNum; ++i )
				//	{
				//		fs.write(pZ[i].szGuildName,_tcslen(pZ[i].szGuildName));
				//		fs.write(_T("\r\n"), 2);
				//	}
				//	fs.close();
				//}
				pRole->SendMessage(m_buf,m_nBufSize);
				return E_Success;
			}
		}
		return GT_INVALID;
	}

	virtual VOID GuildRanking()
	{
		jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
		m_bRankingChanged = TRUE;
		reset_guildranking();
	
	}

	virtual VOID Update()
	{
		static LONG volatile counter = 0;
		static tagDWORDTime lastUpdateTime = 0;
		if( InterlockedIncrement(&counter) >= TICK_PER_SECOND * 60 )
		{
			InterlockedExchange((LPLONG VOLATILE)&counter,0);
			tagDWORDTime now = g_world.GetWorldTime();
			if( lastUpdateTime.year == now.year && lastUpdateTime.month == now.month && lastUpdateTime.day == now.day )
				return;
			// 给帮主压一个清空消息
			if( now.hour == 0 )
			{
				lastUpdateTime = now;
				jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
				_clear_info.clear();
				m_mapRoleGuilds.clear();
				for( std::map<DWORD,_TBillCon>::iterator i = m_mapApplierBill.begin(); i != m_mapApplierBill.end() ; ++i )
				{
					Guild * pGuild = g_guildMgr.GetGuild(i->first);
					if( P_VALID(pGuild) )
					{
						tagGuild const & guildInfo = pGuild->GetGuildAtt();
						tagRoleInfo * owner = g_roleMgr.GetRoleInfo(guildInfo.dwLeaderRoleID);
						if( P_VALID(owner) )
						{
							if( owner->bOnline )
							{
								Role * pOwner = g_roleMgr.GetRolePtrByID(owner->dwRoleID);
								tagNS_NotifyGuildOwnerApplierCounter msg;
								msg.dwApplierCounter = GT_INVALID;
								pOwner->SendMessage(&msg,msg.dwSize);
							}
							else
							{
								_clear_info.push_back(owner->dwRoleID);
							}
						}
					}
				}
				m_mapApplierBill.clear();
				// 角色等级可能改变，这时候就默认排名一次吧，等待运行效果再说吧
			}
		}
	}

	virtual DWORD ConstractGuildMaster( Role * pRole,DWORD dwGuildID )
	{
		if( P_VALID(pRole) )
		{
			Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
			tagNS_ContactGuildLeader msg;
			msg.dwErrorCode = E_Success;
			if ( P_VALID(pGuild) )
			{
				DWORD dwOwnerID = pGuild->GetGuildAtt().dwLeaderRoleID;
				tagRoleInfo * ownerinfo = g_roleMgr.GetRoleInfo(dwOwnerID);
				if( P_VALID(ownerinfo) )
				{
					msg.dwGuildLeaderID = dwOwnerID;
					INT len = _tcslen( ownerinfo->szRoleName );
					if( len > X_SHORT_NAME - 1 )
						len = X_SHORT_NAME - 1;

					_tcsncpy(msg.szLeaderName,ownerinfo->szRoleName,len);
					msg.szLeaderName[len] = 0;

					if ( !ownerinfo->bOnline )
						msg.dwErrorCode = E_GuildApply_LeaderOffline;
					
				}
				else
					msg.dwErrorCode = E_GuildApply_LackOfLeader;
			}
			else
				msg.dwErrorCode = E_GuildApply_NotExist;

			pRole->SendMessage(&msg,msg.dwSize);

			return E_Success;
		}
		return GT_INVALID;
	}

	virtual DWORD OnGuildMasterNotice( Role * pRole )
	{
		if( P_VALID(pRole) )
		{
			DWORD dwGuildID = pRole->GetGuildID();
			if ( P_VALID(dwGuildID) )
			{
				Guild * pGuild = g_guildMgr.GetGuild(dwGuildID) ;
				if( P_VALID(pGuild) )
				{
					if( pGuild->GetGuildAtt().dwLeaderRoleID == pRole->GetID() )
					{
						jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
						std::map<DWORD,_TBillCon>::iterator i = m_mapApplierBill.find(dwGuildID);
						if( i != m_mapApplierBill.end() )
						{
							size_t size = i->second.size();
							if( size > 0 )
							{
								// DONE:通知帮主消息
								tagNS_NotifyGuildOwnerApplierCounter msg;
								msg.dwApplierCounter = size;
								pRole->SendMessage(&msg,msg.dwSize);
							}
						}
						else
						{
							vector<DWORD>::iterator it = std::find(_clear_info.begin(),_clear_info.end(),pRole->GetID());
							if( it != _clear_info.end() )
							{
								tagNS_NotifyGuildOwnerApplierCounter msg;
								msg.dwApplierCounter = GT_INVALID;
								pRole->SendMessage(&msg,msg.dwSize);
								_clear_info.erase(it);
							}
						}
						return E_Success;
					}
				}
			}
		}
		return GT_INVALID;
	}

	virtual DWORD ApproveToJoinGuild( Role * pMaster , DWORD dwApplier , BOOL agreement )
	{
		if( P_VALID(pMaster) )
		{
			DWORD dwGuildID = pMaster->GetGuildID();
			if ( P_VALID(dwGuildID) )
			{
				Guild * pGuild = g_guildMgr.GetGuild(dwGuildID) ;
				if( P_VALID(pGuild) )
				{
					if( pGuild->GetGuildAtt().dwLeaderRoleID == pMaster->GetID() )
					{
						jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
						map<DWORD,_TBillCon>::iterator i = m_mapApplierBill.find(dwGuildID);
						// 删除一天内这个人的帮派申请记录(只有一个)，一天就让他申请一次，因此下面的限制不用清，一天就只能申请一次
						//map<DWORD,DWORD>::iterator itRGmap = m_mapRoleGuilds.find(dwApplier);
						//if( itRGmap != m_mapRoleGuilds.end() )
						//	m_mapRoleGuilds.erase(itRGmap);

						if( i != m_mapApplierBill.end() )
						{
							_TBillCon & verCon = i->second;
							INT ndx = verCon.find(dwApplier);
							if( ndx == GT_INVALID )
							{
								return GT_INVALID;
							}
							verCon.set(ndx,0);

							m_mapRoleGuilds.erase(dwApplier);

							Role * pApplier = g_roleMgr.GetRolePtrByID(dwApplier);
							tagRoleInfo * pApplierInfo = g_roleMgr.GetRoleInfo(dwApplier);

							if( agreement )
							{
								if( P_VALID(pApplierInfo) )
								{
									if( 0 != pApplierInfo->dwGuildID && GT_INVALID != pApplierInfo->dwGuildID)
									{
										tagNS_ApproveToJoinGuild msg;
										msg .dwErrorCode = E_GuildApply_ApplierInOtherGuild;
										pMaster->SendMessage(&msg,msg.dwSize);
										return GT_INVALID;
									}
								}
								else
									return GT_INVALID;

								//pGuild->GetMemberMgr().AddMember(dwApplier,EGMP_BangZhong);
								// Jason 2010-2-21 借鉴guild_hanlder.cpp line193
								DWORD dwErrorCode = pGuild->ApproveToJoinGuild(pMaster->GetID(), dwApplier);
								/*
								DWORD dwErrorCode = pGuild->InviteJoin(pMaster->GetID(), dwApplier);
								if( E_Role_Not_Online != dwErrorCode && E_Guild_Join_BeInvited != dwErrorCode && E_Success != dwErrorCode && E_Guild_Join_NotBeInvited != dwErrorCode )
									return GT_INVALID;

								if( E_Role_Not_Online == dwErrorCode || E_Guild_Join_NotBeInvited == dwErrorCode )
								{
									pGuild->GetMemberMgr().AddMember(dwApplier,EGMP_BangZhong);
									if( P_VALID(pApplier) )
										pApplier->SetGuildID(dwGuildID);
								}
								*/
								if( dwErrorCode != E_Success )
									return dwErrorCode;

								// 帮派广播
								tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(dwApplier);
								if (P_VALID(pRoleInfo))
								{
									INT32 nRoleNameCnt = _tcsclen(pRoleInfo->szRoleName);
									if( nRoleNameCnt > X_SHORT_NAME - 1 )
										nRoleNameCnt = X_SHORT_NAME - 1;
									INT32 nMsgSz = sizeof(tagNS_GuildJoinBroad) + nRoleNameCnt * sizeof(TCHAR);

									MCREATE_MSG(pSend, nMsgSz, NS_GuildJoinBroad);
									pSend->dwRoleID		= dwApplier;
									pSend->dwRoleNameID	= dwApplier;

									//memcpy(pSend->szRoleName, pRoleInfo->szRoleName, (nRoleNameCnt) * sizeof(TCHAR));
									_tcsncpy(pSend->szRoleName, pRoleInfo->szRoleName, nRoleNameCnt);
									pSend->szRoleName[nRoleNameCnt] = 0;

									pGuild->SendGuildMsg(pSend, pSend->dwSize);
									MDEL_MSG(pSend);
								}

								// 同步到周围玩家
								if( P_VALID(pApplier) )
								{
									tagGuildMember* pMember = pGuild->GetMember(dwApplier);
									Map*			pMap	= pApplier->GetMap();
									if (P_VALID(pMember) && P_VALID(pMap))
									{
										tagNS_RemoteRoleGuildInfoChange send;
										send.dwGuildID		= dwGuildID;
										send.dwRoleID		= dwApplier;
										send.n8GuildPos		= pMember->eGuildPos;

										pMap->SendBigVisTileMsg(pApplier, &send, send.dwSize);
									}
								}

								m_bRankingChanged = TRUE;
							}
							if( P_VALID(pApplier) )
							{
								tagNS_ApproveToJoinGuildTrans msg;
								if( agreement )
								{
									// 角色加入帮派，检查是否有公会宣战关系
									g_guildWarDeclareMgr.OnRoleJoinGuild(pApplier);

									msg.dwErrorCode = 0;
								}
								else
								{
									msg.dwErrorCode = 1;
								}
								msg.dwGuildMasterID = pMaster->GetID();
								tagRoleInfo * pRoleInfo = g_roleMgr.GetRoleInfo(pMaster->GetID());
								if( !P_VALID(pRoleInfo) )
									return GT_INVALID;

								INT len = _tcslen( pRoleInfo->szRoleName );
								if( len > X_SHORT_NAME - 1 )
									len = X_SHORT_NAME - 1;
								_tcsncpy( msg.szMasterName,pRoleInfo->szRoleName,len );
								msg.szMasterName[len] = 0;

								len = pGuild->GetGuildAtt().strName.length();
								if( len > X_SHORT_NAME - 1 )
									len = X_SHORT_NAME - 1;
								_tcsncpy(msg.szGuildName,pGuild->GetGuildAtt().strName.c_str(),len);
								msg.szGuildName[len] = 0;

								pApplier->SendMessage(&msg,msg.dwSize);

								std::map<DWORD,_TBillCon>::iterator itFinded = m_mapApplierBill.find(dwGuildID);
								if( itFinded != m_mapApplierBill.end() )
								{
									INT ndx0 = itFinded->second.find(dwApplier);
									if( ndx0 != GT_INVALID )
										itFinded->second.set(ndx0,0);
								}

								//pGuild->SendAllMembers2Client(pApplier);
							}
						}
					}
				}
			}
		}
		return GT_INVALID;
	}

	virtual DWORD SearchGuild(Role * pRole, INT findType,const tstring & strFindWhat)
	{
		if( P_VALID(pRole) )
		{
			tagNS_SearchGuild msg;
			msg.nGuildNum = 0;
			msg.dwErrorCode = E_Guild_NotExist;
			tagRoleInfo * roleInfo = NULL;
			DWORD dwGuildOwnerID = GT_INVALID;
			DWORD dwGuildID = GT_INVALID;

			if( findType ) //帮主名字查找
			{
				dwGuildOwnerID = g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(strFindWhat.c_str()));
				Role * pInnerRole = g_roleMgr.GetRolePtrByID(dwGuildOwnerID);
				if( P_VALID(pInnerRole) )
				{
					dwGuildID = pInnerRole->GetGuildID();
				}
			}
			else // 帮派名字
			{
				dwGuildID = IUTIL->Crc32(_tcslwr( const_cast<TCHAR*>( strFindWhat.c_str() ) ));
				Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
				if( P_VALID(pGuild) )
				{
                    if ( pGuild->GetGuildAtt().byLevel == 0 )
                    {
                          dwGuildOwnerID  = pGuild->GetGuildAtt().dwFounderNameID ;     //是这样的, 把空帮派的帮主id储存在名字id上
						  Role* pGuildOwner = g_roleMgr.GetRolePtrByID(dwGuildOwnerID);
						  if ( P_VALID( pGuildOwner))
						  {
							  dwGuildID  = pGuildOwner->GetGuildID();
						  }
						  if ( 0 == dwGuildOwnerID)
						  {
							  dwGuildOwnerID = -1;
							  dwGuildID      = -1; 
						  }
                    }
					else
					{
                          dwGuildOwnerID = pGuild->GetGuildAtt().dwLeaderRoleID;
					}
					
				}
			}
			roleInfo = g_roleMgr.GetRoleInfo(dwGuildOwnerID);

			if( P_VALID(roleInfo) )
			{
				//if( roleInfo->bOnline )
				{
					//Role * pDst = g_roleMgr.GetRolePtrByID(dwGuildOwnerID);
					//if( P_VALID(pDst) )
					{
						//DWORD dwGuildID = pDst->GetGuildID();
						if( P_VALID(dwGuildID) )
						{
							Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
							if( P_VALID(pGuild) )
							{
								msg.nGuildNum = 1;
								msg.guilds[0].dwGuildID = dwGuildID;
								msg.guilds[0].dwLeaderID = pGuild->GetGuildAtt().dwLeaderRoleID;
								msg.guilds[0].byLevel	= pGuild->GetGuildAtt().byLevel;
								msg.guilds[0].nGuildMembersNum = pGuild->GetGuildAtt().n16MemberNum;
								INT len = pGuild->GetGuildAtt().strName.length();
								if( len > X_SHORT_NAME - 1 )
									len = X_SHORT_NAME - 1;

								_tcsncpy(msg.guilds[0].szGuildName,pGuild->GetGuildAtt().strName.c_str(),len);
								msg.guilds[0].szGuildName[len] = 0;

								len = _tcslen(roleInfo->szRoleName);
								if( len >= 8 )
									len = 8;
								_tcsncpy(msg.guilds[0].szLeaderName,roleInfo->szRoleName,len);
								msg.guilds[0].szLeaderName[len] = 0;
								msg.dwErrorCode = E_Success;
							}
						}
					}
				}
			}
			else
				msg.dwErrorCode = E_Guild_NotExist;

			pRole->SendMessage(&msg,msg.dwSize);

			return E_Success;
		}
		return GT_INVALID;
	}
	virtual DWORD ChangeGuildMember( DWORD dwGuildID, DWORD dwRoleID );
};

DWORD JMApplyToJoinGuildToolkit::ChangeGuildMember( DWORD dwGuildID, DWORD dwRoleID )
{
	Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
	tagRoleInfo * pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if( P_VALID(pGuild) && P_VALID(pRoleInfo) )
	{
		m_bRankingChanged = TRUE;
		//reset_guildranking();
		return E_Success;
	}
	return GT_INVALID;
}

void JMApplyToJoinGuildToolkit::reset_guildranking(void)
{
	if( is_changed() )
	{
		//jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(_cs);
		//if( m_nRankNum == 0 || m_nRankingSize != flag )
		{
			size_t size = m_guildinfos.size();
			m_nRankingSize = size;
			m_nRankNum = 0;
			if( P_VALID(m_buf) )
			{
				g_MemPoolSafe.Free(m_buf);
				m_buf = NULL;
			}

			std::priority_queue<tagGuildPriority> m_GuildQueue(std::less<tagGuildPriority>(),m_guildinfos);

			memset(m_GuildRanking,0,sizeof(m_GuildRanking));

			for( int i = 0 ; i < size && i < E_GuildRanking_Valve; ++i )
			{
				const tagGuildPriority& tem = m_GuildQueue.top();
				Guild * pGuild = g_guildMgr.GetGuild(tem.dwGuildID);
				if( !P_VALID(pGuild) )
					continue;

				m_GuildRanking[i].dwGuildID = tem.dwGuildID;
				m_GuildRanking[i].dwLeaderID = pGuild->GetGuildAtt().dwLeaderRoleID;
				m_GuildRanking[i].byLevel	= pGuild->GetGuildAtt().byLevel;
				m_GuildRanking[i].nGuildMembersNum = pGuild->GetMemberMgr().GetNumber();//pGuild->GetGuildAtt().n16MemberNum;
				INT len = pGuild->GetGuildAtt().strName.length();
				if( len > X_SHORT_NAME - 1 )
					len = X_SHORT_NAME - 1;

				_tcsncpy(m_GuildRanking[i].szGuildName,pGuild->GetGuildAtt().strName.c_str(),len);
				m_GuildRanking[i].szGuildName[len] = 0;

				tagRoleInfo * pRoleInfo = g_roleMgr.GetRoleInfo(m_GuildRanking[i].dwLeaderID);
				if( P_VALID(pRoleInfo) )
				{
					len = _tcslen(pRoleInfo->szRoleName);
					if( len > 8 )
						len = 8;
					_tcsncpy(m_GuildRanking[i].szLeaderName,pRoleInfo->szRoleName,len);
					m_GuildRanking[i].szLeaderName[len] = 0;
				}
				else
					m_GuildRanking[i].szLeaderName[0] = 0;

				++m_nRankNum;
				m_GuildQueue.pop();
			}

		}
		if( m_nRankNum > 0 && m_buf == NULL )
		{
			size_t size = sizeof( tagNS_GetAllGuildSmallInfo ) + (m_nRankNum - 1) * sizeof( tagGuildSmallInfo ) ;
			m_buf = (BYTE*)g_MemPoolSafe.Alloc(size);
			m_nBufSize = size;
			if( P_VALID(m_buf) )
			{
				tagNS_GetAllGuildSmallInfo tem ;
				tem.dwSize = size;
				tem.dwErrorCode = E_Success;
				tem.nGuildNum = m_nRankNum;
				memcpy(m_buf,&tem,sizeof(tem));
				BYTE * p = m_buf + sizeof( tagNS_GetAllGuildSmallInfo ) - sizeof( tagGuildSmallInfo );
				memcpy(p,m_GuildRanking,m_nRankNum * sizeof(tagGuildSmallInfo));
			}
			else
				m_buf = NULL;
		}
	}
}


struct MyGuildSingletonAdaptor
{
	typedef JMApplyToJoinGuildToolkit * pointer;
	static pointer construct(void * p = NULL)
	{
		return new JMApplyToJoinGuildToolkit;
	}
	static void destroy(pointer p)
	{
		if( P_VALID(p) )
			delete p;
	}
};


typedef jm_tool::JMSingleton<JMApplyToJoinGuildToolkit,MyGuildSingletonAdaptor,jm_tool::CSCreatedLock> TApplyJoinToGuildToolkit;

template<class T,class Alloc>
class CSingletonGuard
{
	typedef	T * TPointer;
	TPointer m_Ptr;
public:
	CSingletonGuard(TPointer p)
		: m_Ptr(p)
	{
		;
	}
	~CSingletonGuard()
	{
		if( P_VALID(m_Ptr) )
		{
			TPointer p = m_Ptr;
			m_Ptr = NULL;
			Alloc::destroy(p);
		}
	}
	VOID Attach(TPointer p)
	{
		m_Ptr = p;
	}
};


static CSingletonGuard<JMApplyToJoinGuildToolkit,MyGuildSingletonAdaptor> my_little_pointer(NULL);

ApplyToJoinGuildToolkit * ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit()
{
	JMApplyToJoinGuildToolkit * p = TApplyJoinToGuildToolkit::Instance();
	my_little_pointer.Attach(p);
	return p;
}


#undef __USE_JM_TOOL_TEMP
