#include "stdafx.h"
#include "ActivityMgr.h"
#include "ActivityBroadcastHandler.h"
#include "ServerTime.h"
#include "../WorldDefine/msg_activity.h"
#include "FestivalActivityPromptFrame.h"
#include "RoleMgr.h"
#include "Role.h"
#include "LocalPlayer.h"
#include "CombatSysUtil.h"



namespace SortObj 
{
	bool CompareTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute)
	{
		if (((SrcHour * 60 + SrcMinute) - (DestHour * 60 + DestMinute)) > 0)
			return true;
		return false;
	}

	bool IsActive(const tagActivityProtoData &activity)
	{
		if( (activity.week == ServerTime::Inst()->GetCurrentWeekDay())
			&& (CompareTime(activity.startHour, activity.startMinute, ServerTime::Inst()->GetCurrentHour(), ServerTime::Inst()->GetCurrentMinute()) == false)
			&& (CompareTime(activity.endHour, activity.endMinute, ServerTime::Inst()->GetCurrentHour(), ServerTime::Inst()->GetCurrentMinute()) == false) )
		{
			return true;
		}

		return false;
	}

	class SortByTime
	{
	public:
		bool operator()(const tagActivityProtoData &op1, const tagActivityProtoData &op2)
		{

			if (op1.week < op2.week)
			{
				return true;
			}
			else if (op1.week > op2.week)
			{
				return false;
			}
			else
			{
				if (op1.startHour < op2.startHour)
				{
					return true;
				}
				else if (op1.startHour > op2.startHour)
				{
					return false;
				}
				else
				{
					return op1.startMinute < op2.startMinute;
				}
			}

		}
	};

	class SortByMonthAndDay
	{
	public:
		bool operator()(const tagActivityProtoData &op1, const tagActivityProtoData &op2)
		{

			if (op1.startmonth < op2.startmonth)
			{
				return true;
			}
			else if (op1.startmonth == op2.startmonth)
			{
				return op1.startday > op2.startday;
			}
			else
			{
				return false;
			}

		}
	};

	class SortByQuestID
	{
	public:
		bool operator()(const tagActivityProtoData &op1, const tagActivityProtoData &op2)
		{
			return op1.id < op2.id;
		}
	};

	class SortByLevel
	{
	public:
		bool operator()(const tagActivityProtoData &op1, const tagActivityProtoData &op2)
		{
			if( op1.minLevel < op2.minLevel )
				return true;
			else if( op1.minLevel > op2.minLevel )
				return false;
			else 
				return op1.maxLevel < op2.maxLevel;
		}
	};

	class SortByNpcID
	{
	public:
		bool operator()(const tagActivityProtoData &op1, const tagActivityProtoData &op2)
		{
			return op1.acceptNpcID < op2.acceptNpcID;
		}
	};

	class SortDaily
	{
	public:
		bool operator()(const tagDailyQuestProtoData &op1, const tagDailyQuestProtoData &op2)
		{
			if( op1.minLevel < op2.minLevel )
				return true;
			else if( op1.minLevel > op2.minLevel )
				return false;
			else 
				return op1.maxLevel < op2.maxLevel;
		}
	};

}


ActivityMgr::ActivityMgr()
: m_trunk(this), 
m_pNetCmdMgr(NULL),
m_pGameFrameMgr(NULL),
m_pSession(NULL),
m_pFsectivalActivityFrame(NULL)
{
}

ActivityMgr::~ActivityMgr()
{

}

bool ActivityMgr::Init()
{
	m_pNetCmdMgr->Register("NS_ActivityTaskState", (NETMSGPROC)(m_trunk.sfp2(&ActivityMgr::OnNS_ActivityTaskState)), _T("NS_ActivityTaskState"));
	
	m_pNetCmdMgr->Register("NS_RoleInitOk", (NETMSGPROC)m_trunk.sfp2(&ActivityMgr::OnNS_RoleInitOk), _T("NS_RoleInitOk") );

	m_pNetCmdMgr->Register("NS_GetRoleInitState_DailyCompleteQuest",		(NETMSGPROC)m_trunk.sfp2(&ActivityMgr::NS_GetCompleteQuest),	_T("已经完成的任务"));
	m_pNetCmdMgr->Register("NS_GetSingleQuestDoneInfo",		(NETMSGPROC)m_trunk.sfp2(&ActivityMgr::NS_GetSingleQuestDoneInfo),	_T("NS_GetSingleQuestDoneInfo"));

	m_pNetCmdMgr->Register("NS_XunLianChangRemainTime",		(NETMSGPROC)m_trunk.sfp2(&ActivityMgr::NS_XunLianChangRemainTime),	_T("NS_XunLianChangRemainTime"));

	m_pGameFrameMgr->RegisterEventHandle( _T("ClearDailyQuestDoneTimes"), (FRAMEEVENTPROC)m_trunk.sfp1(&ActivityMgr::ClearDailyQuestDoneTimes));

	if (!LoadActivityProto())
		return false;
	
	ActivityBroadcastHandler::Inst()->Init();

	tagNC_GetRoleInitState m;
	m.eType = ERIT_DailyCompleteQuest;
	m_pSession->Send(&m);

	return true;	
}

VOID ActivityMgr::Update()
{
	if( P_VALID(m_pFsectivalActivityFrame) )
	{
		if( m_pFsectivalActivityFrame->CanDestroy() )
		{
			m_pGameFrameMgr->AddToDestroyList( m_pFsectivalActivityFrame );
			m_pFsectivalActivityFrame = NULL;
		}
	}
}

void ActivityMgr::Destroy()
{
	m_pNetCmdMgr->UnRegister("NS_ActivityTaskState", (NETMSGPROC)(m_trunk.sfp2(&ActivityMgr::OnNS_ActivityTaskState)));
	m_pNetCmdMgr->UnRegister("NS_RoleInitOk", (NETMSGPROC)(m_trunk.sfp2(&ActivityMgr::OnNS_RoleInitOk)) );
	m_pNetCmdMgr->UnRegister("NS_GetRoleInitState_DailyCompleteQuest",	(NETMSGPROC)m_trunk.sfp2(&ActivityMgr::NS_GetCompleteQuest));
	m_pNetCmdMgr->UnRegister("NS_GetSingleQuestDoneInfo",			(NETMSGPROC)m_trunk.sfp2(&ActivityMgr::NS_GetSingleQuestDoneInfo));
	m_pNetCmdMgr->UnRegister("NS_XunLianChangRemainTime",			(NETMSGPROC)m_trunk.sfp2(&ActivityMgr::NS_XunLianChangRemainTime));

	m_pGameFrameMgr->UnRegisterEventHandler( _T("ClearDailyQuestDoneTimes"), (FRAMEEVENTPROC)m_trunk.sfp1(&ActivityMgr::ClearDailyQuestDoneTimes));

	ActivityBroadcastHandler::Inst()->Destroy();
	m_activities.clear();
	m_dailys.clear();
}

ActivityMgr* ActivityMgr::Inst()
{
	static ActivityMgr inst;
	return &inst;
}

bool ActivityMgr::LoadActivityProto()
{
	if (!(m_data.LoadFromFile()))
		return false;
	InitActivityTable();

	return true;
}

void ActivityMgr::InitActivityTable()
{
	const ActivityProtoData::ActivityProtoMap &data = m_data.GetData();
	ActivityProtoData::ActivityProtoMap::const_iterator begin = data.begin();
	ActivityProtoData::ActivityProtoMap::const_iterator end = data.end();
	for (; begin != end; ++begin)
	{
		if (begin->second.mode == EActivityMode_Day)
		{
			for (int i = 0; i < 7; ++i)
			{
				tagActivityProtoData temp = begin->second;
				temp.week = static_cast<EWeek>(i);
				m_activities.push_back(temp);
			}
		}
		else
			m_activities.push_back(begin->second);
	}

	const ActivityProtoData::DailyProtoMap &datas = m_data.GetDailyData();
	ActivityProtoData::DailyProtoMap::const_iterator begins = datas.begin();
	ActivityProtoData::DailyProtoMap::const_iterator ends = datas.end();
	for (; begins != ends ;++begins)
	{
		m_dailys.push_back(begins->second);
	}

}

DWORD ActivityMgr::OnNS_ActivityTaskState(tagNS_ActivityTaskState *pCmd, DWORD)
{
	return 0;	
}
void ActivityMgr::SortDaily()
{
	SortObj::SortDaily sortobj;
	std::sort(m_dailys.begin(), m_dailys.end(), sortobj);
}

void ActivityMgr::SortActivities(ActivityMgr::ESortType type)
{
	switch (type)
	{
	case EST_SortByTime:
		{
			SortObj::SortByTime sortobj;
			std::sort(m_activities.begin(), m_activities.end(), sortobj);
		} break;
	case EST_SortByName:
		{
			SortObj::SortByQuestID sortobj;
			std::sort(m_activities.begin(), m_activities.end(), sortobj);
		} break;
	case EST_SortByLevel:
		{
			SortObj::SortByLevel sortobj;
			std::sort(m_activities.begin(), m_activities.end(), sortobj);
		} break;
	case EST_SortByNpc:
		{
			SortObj::SortByNpcID sortobj;
			std::sort(m_activities.begin(), m_activities.end(), sortobj);
		} break;
	case EST_SortByAcceptTime:
		{
			
		} break;
	case EST_SortByMonthAndDay:
		{
			SortObj::SortByMonthAndDay sortobj;
			std::sort(m_activities.begin(), m_activities.end(), sortobj);
		} break;
	default:break;
	}
}

const ActivityMgr::ActivityVector& ActivityMgr::GetActivityVector()
{
	return m_activities;
}
const ActivityMgr::DailyVector& ActivityMgr::GetDailyVector()
{
	return m_dailys;
}

UINT16 ActivityMgr::GetActivityID( DWORD index ) const
{
	if (index < m_activities.size())
		return m_activities[index].id;
	return -1;
}


//处理网络事件，服务器已经把所有的初始化消息都发完的时候，发送角色初始化完毕的消息
DWORD ActivityMgr::OnNS_RoleInitOk( tagNS_RoleInitOk* pNetCmd, DWORD dwParam )
{
	m_pFsectivalActivityFrame = (FestivalActivityPromptFrame*)TObjRef<GameFrameMgr>()->CreateFrame(_T("World"), _T("FestivalActivityPromptFrame"), _T("FestivalActivityPromptFrame"), 0);
	return 0;
}

DWORD ActivityMgr::NS_GetCompleteQuest(tagNS_GetRoleInitState_DailyCompleteQuest* pMsg, DWORD pParam)
{
	if (pMsg->nNum == 0)
	{
		TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("ClearDailyQuestDoneTimes"), NULL ) );
	}
	for(INT i = 0; i<pMsg->nNum ; i++)
	{
		ActivityProtoData::DailyProtoMap::const_iterator it = m_data.GetDailyData().find(pMsg->completeQuest[i].u16QuestID);
		if (it != m_data.GetDailyData().end())
		{
			it->second.doneTimes = pMsg->completeQuest[i].nTimes;
		}
	}
	return 0;

}
DWORD ActivityMgr::NS_GetSingleQuestDoneInfo(tagNS_GetSingleQuestDoneInfo* pMsg, DWORD pParam)
{
	ActivityProtoData::DailyProtoMap::const_iterator it = m_data.GetDailyData().find(pMsg->u16QuestID);
	if (it != m_data.GetDailyData().end())
	{
		it->second.doneTimes = pMsg->nTimes;
	}
	return 0;
}

DWORD ActivityMgr::ClearDailyQuestDoneTimes(tagGameEvent* pGameEvent)
{
	m_data.ClearDailyQuestDoneTimes();

	return 0;
}
DWORD ActivityMgr::NS_XunLianChangRemainTime(tagNS_XunLianChangRemainTime* pMsg, DWORD pParam)
{
	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLP))
	{
		tagRoleBuff* buff = pLP->FindBuff(pMsg->dwBuffID);
		if (P_VALID(buff))
		{
			tagBuffProtoClient* buffproto = buff->pBuffProto;
			if (!P_VALID(buffproto))
				return 0;

			tstring szName = buffproto->szName;
			INT times = pMsg->dwTime/60000;

			TCHAR szText[512];
			_stprintf(szText, g_StrTable[_T("XunLianChang_RemainTime")], szName.c_str(),times);
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(szText);
		}
	}
	return 0;
}