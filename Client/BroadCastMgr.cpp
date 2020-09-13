#include "stdafx.h"
#include "BroadCastMgr.h"
#include "QuestScript.h"
#include "GameScriptMgr.h"
#include "ChatFrame.h"
#include "BigBroadCastFrame.h"
#include "PlayerNameTab.h"
#include "..\WorldDefine\msg_script.h"
#include "..\WorldDefine\ScriptMsgInfo.h"
#include "GuildEvent.h"
#include "GuildMgr.h"
#include "MapEvent.h"

namespace
{
	tstring& replace_all(tstring& str,const tstring& old_value,const tstring& new_value)
	{  
		while( true )
		{
			tstring::size_type pos(0);
			if( (pos=str.find(old_value)) != tstring::npos )
				str.replace(pos,old_value.length(),new_value);
			else
				break;
		}  
		return str;
	}
}

BroadCastMgr::BroadCastMgr( void ):m_Trunk(this)
, m_bAllRoleFinded(true)
, m_bAllGuildFinded(true)
, m_dwLastParseTime(GT_INVALID)
, m_dwCurTime(GT_INVALID)
{
	
}

BroadCastMgr::~BroadCastMgr( void )
{
	
}


BroadCastMgr* BroadCastMgr::Inst()
{
	static BroadCastMgr broadcastMgr;
	return &broadcastMgr;
}

void BroadCastMgr::Init()
{
	m_pUtil			=  TObjRef<Util>();
	m_pCmdMgr		= TObjRef<NetCmdMgr>();
	m_pSession		= TObjRef<NetSession>();
	m_pFrameMgr		= TObjRef<GameFrameMgr>();

	//--注册游戏事件
	m_pFrameMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnRoleGetNameEvent));
	m_pFrameMgr->RegisterEventHandle(_T("GuildEvent_NameGet"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnGuildGetNameEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagEnterPromptEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnEnterPromptEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagPushSpecInfoEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnPushSpecInfo));
	

	m_bAllRoleFinded = true;
	m_bAllGuildFinded = true;

    LoadBroadCastProto();
}

void BroadCastMgr::Destroy()
{
	//--注销游戏事件
	m_pFrameMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnRoleGetNameEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("GuildEvent_NameGet"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnGuildGetNameEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagEnterPromptEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnEnterPromptEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagPushSpecInfoEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&BroadCastMgr::OnPushSpecInfo));

	m_msgCache.clear();
	m_roleCache.clear();
	m_guildCache.clear();
}

void BroadCastMgr::Update()
{
	if (m_msgCache.empty())
		return;

	if (m_bAllRoleFinded && m_bAllGuildFinded)
	{
		MsgCache::iterator msgIter = m_msgCache.begin();
		tstring &strMsg = msgIter->strContent;
		m_dwLastParseTime = timeGetTime();	// 记录解析发生的时间
		bool bRet = ParseBroadCast(strMsg);
		if (bRet)
		{
			if (msgIter->eType == EMCT_Broadcast)
			{
				PushBroadCast(strMsg.c_str());
			}
			else if (msgIter->eType == EMCT_Chat)
			{
				PushChatInfo(strMsg.c_str(), msgIter->eChannel);
			}
			else if (msgIter->eType == EMCT_BigBroadcast)
			{
				PushBigBroadCast(strMsg.c_str());
			}
			else if (msgIter->eType == EMCT_BigNewBroadcast)
			{
				PushBigBroadCast(strMsg.c_str());
			}
			else if (msgIter->eType == EMCT_SmallRunHorse)
			{
				ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
				if( P_VALID(pFrame) )
				{
					pFrame->PushScrollText(strMsg.c_str());
				}
			}
			else if (msgIter->eType == EMCT_BigRunHorse)
			{
				ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
				if( P_VALID(pFrame) )
				{
					//	如果当前大跑马灯存在具有优先级的信息
					if (pFrame->BigRunHorseCurrentPriority())
					{
						if (msgIter->npriority < pFrame->BigRunHorseCurrentPriority())		// 如果新信息优先级较高，直接开始播放
						{
							pFrame->PushBigScrollTextImmediately(strMsg.c_str(),msgIter->npriority,40,msgIter->color);
							PushChatInfo(strMsg.c_str(), ESCC_Affiche);
						} 
						else if (msgIter->npriority == pFrame->BigRunHorseCurrentPriority())
						{
							pFrame->BigScrollTextCancelAfterThisTime();
							pFrame->PushBigScrollText(strMsg.c_str(),msgIter->npriority,40,msgIter->color);
							PushChatInfo(strMsg.c_str(), ESCC_Affiche);
						}
						else
						{
							pFrame->PushBigScrollText(strMsg.c_str(),msgIter->npriority,40,msgIter->color);
							PushChatInfo(strMsg.c_str(), ESCC_Affiche);
						}
					}
					else
					{
						pFrame->PushBigScrollText(strMsg.c_str(),msgIter->npriority,40,msgIter->color);
						PushChatInfo(strMsg.c_str(), ESCC_Affiche);
					}
				}
			}
			else if (msgIter->eType == EMCT_ChatNew)
			{
				this->PushChatInfo(strMsg.c_str(), ESCC_Affiche);
			}
			m_msgCache.erase(msgIter);
		}
	}
	m_dwCurTime = timeGetTime();
	// 如果在指定超时时间都没发生过解析，清空一条广播
	if (m_dwCurTime - m_dwLastParseTime >= BROADCAST_OVERTIME_SET)
	{
		if (!m_msgCache.empty())
		{
			MsgCache::iterator msgIter = m_msgCache.begin();
			m_msgCache.erase(msgIter);
		}
		// 重置为true，给下一条广播获得处理的机会
		m_bAllRoleFinded = true;
		m_bAllGuildFinded = true;
	}

}

void BroadCastMgr::BroadCast( LPCTSTR szContent, BOOL bHasRoleOrGuild )
{
	if( bHasRoleOrGuild == TRUE )
	{
		tagMsgCache cache;
		cache.eType = EMCT_Broadcast;
		cache.strContent = szContent;
		m_msgCache.push_back( cache );
	}	
	else
	{
		this->PushBroadCast(szContent);
	}
}

void BroadCastMgr::ToChatChannel(LPCTSTR szContent, ESendChatChannel eChannel, BOOL bHasRoleOrGuid)
{
	if (bHasRoleOrGuid == TRUE)
	{
		tagMsgCache cache;
		cache.eType = EMCT_Chat;
		cache.eChannel = eChannel;
		cache.strContent = szContent;
		m_msgCache.push_back( cache );
	}
	else
	{
		this->PushChatInfo(szContent, eChannel);
	}
}

void BroadCastMgr::BigBroadCast(LPCTSTR szContent, BOOL bHasRoleOrGuild)
{
	if (bHasRoleOrGuild == TRUE)
	{
		tagMsgCache cache;
		cache.eType = EMCT_BigBroadcast;
		cache.strContent = szContent;
		m_msgCache.push_back(cache);
	}
	else
	{
		this->PushBigBroadCast(szContent);
	}

}
void BroadCastMgr::NewBroadCast(LPCTSTR szContent, BOOL bHasRoleOrGuild,DWORD msgID)
{
	const tagBroadCastProtoData* data = m_BroadcastData.GetBroadCastProto(msgID);
	if(!P_VALID(data))
		return;

	/*tstring tempStr = szContent;
	tstring &strMsg = tempStr;*/
	//bool bRet = ParseBroadCast(strMsg);

	//if (bRet)
	//{
		switch (data->eBoardcastType)
		{
		case ENBT_BigNewBroadcast:
			{
				tagMsgCache cache;
				cache.eType = EMCT_BigNewBroadcast;
				cache.strContent = szContent;
				cache.bHasRoleOrGuild = bHasRoleOrGuild;
				m_msgCache.push_back( cache );
				//BigBroadCast(szContent,bHasRoleOrGuild);
			}
			break;
		case ENBT_SmallRunHorse:
			{
				/*ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
				if( P_VALID(pFrame) )
				{
					pFrame->PushScrollText(szContent);
				}*/
				tagMsgCache cache;
				cache.eType = EMCT_SmallRunHorse;
				cache.strContent = szContent;
				m_msgCache.push_back( cache );
			}
			break;
		case ENBT_BigRunHorse:
			{

				const tagChannelProtoData * channel = m_BroadcastData.GetChannelProto(data->eChannelType);
				if(!P_VALID(channel)) 
					break;
				tstring strTemp = _T("");
				strTemp.append(channel->ChannelName);
				strTemp.append(szContent);

				tagMsgCache cache;
				cache.eType = EMCT_BigRunHorse;
				cache.strContent = strTemp;
				cache.npriority = channel->npriority;
				cache.color     = channel->dwColour;
				m_msgCache.push_back( cache );

				//ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
				//if( P_VALID(pFrame) )
				//{
				//	//	如果当前大跑马灯存在具有优先级的信息
				//	if (pFrame->BigRunHorseCurrentPriority())
				//	{
				//		if (channel->npriority < pFrame->BigRunHorseCurrentPriority())		// 如果新信息优先级较高，直接开始播放
				//		{
				//			pFrame->PushBigScrollTextImmediately(strTemp.c_str(),channel->npriority,40,channel->dwColour);
				//			this->PushChatInfo(szContent, ESCC_Affiche);
				//		} 
				//		else if (channel->npriority == pFrame->BigRunHorseCurrentPriority())
				//		{
				//			pFrame->BigScrollTextCancelAfterThisTime();
				//			pFrame->PushBigScrollText(strTemp.c_str(),channel->npriority,40,channel->dwColour);
				//			this->PushChatInfo(szContent, ESCC_Affiche);
				//		}
				//		else
				//		{
				//			pFrame->PushBigScrollText(strTemp.c_str(),channel->npriority,40,channel->dwColour);
				//			this->PushChatInfo(szContent, ESCC_Affiche);
				//		}
				//	}
				//	else
				//	{
				//		pFrame->PushBigScrollText(strTemp.c_str(),channel->npriority,40,channel->dwColour);
				//		this->PushChatInfo(szContent, ESCC_Affiche);
				//	}

				//}
			}
			break;
		case ENBT_ChatNew:
			{
				tagMsgCache cache;
				cache.eType = EMCT_ChatNew;
				cache.strContent = szContent;
				m_msgCache.push_back( cache );
				//this->PushChatInfo(szContent, ESCC_Affiche);
			}
			break;
		default:
			break;
		}
	//}



}
bool BroadCastMgr::ParseBroadCast( tstring& strContext )
{
	
	UINT			nIndex = 0;
	bool			bRet = true;
	DWORD			dwRoleID = 0;
	DWORD			dwGuildID = 0;
	UINT			paramIndex = 0;

	while( nIndex < strContext.size() )
	{
		if( strContext[nIndex]==_T('<') && strContext[nIndex+1]==_T('R') )
		{
			tstring::size_type flagEndPos = strContext.find_first_of(_T('>'), nIndex);
			if( flagEndPos == tstring::npos || flagEndPos - nIndex <= 1 )
			{
				bRet = false;
				break;			//语法错误
			}

			//取出RoleID
			tstring str = strContext.substr( nIndex+3, flagEndPos-(nIndex+3));
			dwRoleID = _tcstoul( str.c_str() , NULL, 10);
			if( IS_PLAYER(dwRoleID) )
			{

				tstring strName = PlayerNameTab::Inst()->FindNameByID(dwRoleID);
				if( !strName.empty() )
				{
					strContext.replace( nIndex, flagEndPos-nIndex+1, strName.c_str() );
					nIndex += strName.length();
				}
				else
				{
					bRet = false;
					m_roleCache.push_back(make_pair(false,dwRoleID));
					m_bAllRoleFinded = false;
					nIndex += (flagEndPos-nIndex+1);
				}
			}
			else // 防止死循环
			{
				strContext.replace( nIndex, flagEndPos-nIndex+1, _T("") );
			}
		}
		else if (strContext[nIndex]==_T('<') && strContext[nIndex+1]==_T('G'))
		{
			tstring::size_type flagEndPos = strContext.find_first_of(_T('>'), nIndex);
			if (flagEndPos == tstring::npos || flagEndPos - nIndex <= 1)
			{
				bRet = false;
				break;		// 语法错误
			}
			// 取出GuildID
			tstring str = strContext.substr(nIndex+3, flagEndPos-(nIndex+3));
			dwGuildID = _tcstoul(str.c_str(), NULL, 10);
			tstring strGuildName = GuildMgr::Inst()->GetGuildName(dwGuildID);

			if (!strGuildName.empty())
			{
				strContext.replace(nIndex, flagEndPos - nIndex + 1, strGuildName.c_str());
				nIndex += strGuildName.length();
			}
			else
			{
				bRet = false;
				m_guildCache.push_back(make_pair(false, dwGuildID));
				m_bAllGuildFinded = false;
				nIndex += (flagEndPos-nIndex+1);
			}
		}

		else
			++nIndex;
	}

	return bRet;
}

DWORD BroadCastMgr::OnRoleGetNameEvent( tagRoleGetNameEvent* pGameEvent )
{
	if (m_roleCache.empty())
		return 0;

	RoleCache::iterator roleIter;
	bool bAllFinded = true;
	for( roleIter=m_roleCache.begin(); roleIter!=m_roleCache.end(); ++roleIter )
	{
		if( !(roleIter->first) )
		{
			if( roleIter->second == pGameEvent->dwRoleID && pGameEvent->strRoleName != _T(""))
				roleIter->first = true;
			else
				bAllFinded = false;
		}
	}

	m_bAllRoleFinded = bAllFinded;
	if (m_bAllRoleFinded)
	{
		m_roleCache.clear();
	}

	return 0;
}

void BroadCastMgr::PushBroadCast( LPCTSTR szContent )
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		pFrame->PushScrollText(szContent);
		pFrame->PushInfo(szContent, ESCC_Affiche);
	}
}


void BroadCastMgr::PushChatInfo(LPCTSTR szContent, ESendChatChannel eChannel)
{
	ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
	if( P_VALID(pFrame) )
	{
		pFrame->PushInfo(szContent, eChannel);
	}
}

void BroadCastMgr::PushBigBroadCast(LPCTSTR szContent)
{
	BigBroadCastFrame *pFrame = (BigBroadCastFrame*)m_pFrameMgr->GetFrame(_T("BigBroadcast"));
	if (P_VALID(pFrame))
		pFrame->PushInfo(szContent);
}

DWORD BroadCastMgr::OnGuildGetNameEvent(tagGuildNameGetEvent *pGameEvent)
{
	if (m_guildCache.empty())
		return 0;

	bool bAllFinded = true;

	for (GuildCache::iterator iter = m_guildCache.begin(); iter != m_guildCache.end(); ++iter)
	{
		if (!(iter->first))
		{
			if (iter->second == pGameEvent->dwGuildID && pGameEvent->strName != _T(""))
				iter->first = true;
			else
				bAllFinded = false;
		}
	}
	m_bAllGuildFinded = bAllFinded;
	if (m_bAllGuildFinded)
	{
		m_guildCache.clear();
	}

	return 0;
}


bool BroadCastMgr::LoadBroadCastProto()
{
	if (!(m_BroadcastData.LoadFromFile()))
		return false;

	return true;
}

DWORD BroadCastMgr::GetChannelColor(ENewChannelType escc)
{
	switch(escc)
	{
	case ENCT_CityWar:
		return 0xFFfea5a0;//_T("<color=(254,165,160)>");
		break;
	case ENCT_Guild:
		return 0xFF8080ff;//_T("<color=(128,128,255)>");
		break;
	case ENCT_Affiche:
		return 0xFFffbb47;//_T("<color=(255,187,71)>");
		break;
	case ENCT_Special:
		return 0xFF80ffff;//_T("<color=(128,255,255)>");
		break;
	case ENCT_Activity:
		return 0xFF80ff80;//_T("<color=(128,255,128)>");
		break;
	}
	return 0xFFfea5a0;//_T("<color=(224,247,255)>");
}

DWORD BroadCastMgr::OnEnterPromptEvent(tagEnterPromptEvent* pGameEvent)
{
	PushChatInfo(pGameEvent->strPromptDesc.c_str(),ESCC_Current);
	return 0;
}
DWORD BroadCastMgr::OnPushSpecInfo(tagPushSpecInfoEvent* pGameEvent)
{
	PushChatInfo(pGameEvent->strDesc.c_str(),pGameEvent->eChannelType);
	return 0;
}