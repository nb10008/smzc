#include "stdafx.h"
#include "ActivityProtoData.h"


BOOL ActivityProtoData::LoadFromFile()
{
	// 读取activity_proto
	list<tstring> listProto;
	tstring szActivityProto = _T("data\\system\\attdata\\activity_proto.xml");
	TObjRef<VarContainer> varActivityProto = CreateObj("ActivityProto", "VarContainer");
	if (!varActivityProto->Load("VFS_System", szActivityProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load Activity proto file failed"));
		KillObj("ActivityProto");
		return FALSE;
	}
	
	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagActivityProtoData data;

		data.id = varActivityProto->GetInt(_T("id"), begin->c_str());
		//data.type = static_cast<EActivityType>(varActivityProto->GetInt(_T("type"), begin->c_str()));
		//data.acceptTimeLimit = static_cast<BOOL>(varActivityProto->GetInt(_T("accepttimelimit"), begin->c_str()));
		data.week = static_cast<EWeek>(varActivityProto->GetInt(_T("weekday"), begin->c_str(), 0));
	
		//add douyubo	节日活动
		data.prompt = varActivityProto->GetInt( _T("prompt"), begin->c_str(), 0 );
		data.startyear=varActivityProto->GetInt( _T("startyear"), begin->c_str(), 0 );
		data.endyear=varActivityProto->GetInt( _T("endyear"), begin->c_str(), 0 );

		data.startmonth = varActivityProto->GetInt( _T("startmonth"), begin->c_str(), 0 );
		data.endmonth = varActivityProto->GetInt( _T("endmonth"), begin->c_str(), 0 );
		data.startday = varActivityProto->GetInt( _T("startday"), begin->c_str(), 0 );
		data.endday = varActivityProto->GetInt( _T("endday"), begin->c_str(), 0 );
		
		data.startHour = varActivityProto->GetInt(_T("starthour"), begin->c_str(), 0);
		data.startMinute = varActivityProto->GetInt(_T("startminute"), begin->c_str(), 0);
		data.endHour = varActivityProto->GetInt(_T("endhour"), begin->c_str(), 0);
		data.endMinute = varActivityProto->GetInt(_T("endminute"), begin->c_str(), 0);
		data.acceptNpcID = varActivityProto->GetDword(_T("AcceptNpcID"), begin->c_str(), 0xffff);
		data.minLevel = varActivityProto->GetInt(_T("MinLevel"), begin->c_str(), 0);
		data.maxLevel = varActivityProto->GetInt(_T("MaxLevel"), begin->c_str(), 150);
		data.acceptTimes = varActivityProto->GetInt(_T("accepttimes"), begin->c_str(), 1);
		data.mode = static_cast<EActivityMode>(varActivityProto->GetInt(_T("activitymode"), begin->c_str(), 0));
		data.activityType = static_cast<ENewActivityType>(varActivityProto->GetInt(_T("activitytype"), begin->c_str(), 0));
		m_mapActivityProtoData.insert(make_pair(data.id, data));
	}
	KillObj("ActivityProto");

	// 读取activitydata对应的名字和描述
	list<tstring> listName;
	tstring szActivityName = g_strLocalPath + _T("\\attdata\\activity_name.xml");
	TObjRef<VarContainer> varActivityName = CreateObj("ActivityName", "VarContainer");
	if (!varActivityName->Load("VFS_System", szActivityName.c_str(), "id", &listName))
	{
		IMSG(_T("Load Activity name file failed"));
		KillObj("ActivityName");
		return FALSE;
	}
	list<tstring>::iterator iend = listName.end();
	for (list<tstring>::iterator begin = listName.begin(); begin != iend; ++begin)
	{
		UINT16 id = varActivityName->GetInt(_T("id"), begin->c_str(), 0);
		tstring name = varActivityName->GetString(_T("name"), begin->c_str(), _T(""));
		tstring desc = varActivityName->GetString(_T("desc"), begin->c_str(), _T(""));
		bool visible= static_cast<BOOL>(varActivityName->GetInt(_T("visible"), begin->c_str()));
		ActivityProtoMap::iterator it = m_mapActivityProtoData.find(id);
		if (it != m_mapActivityProtoData.end())
		{
			it->second.name = name;
			it->second.desc = desc;
			it->second.visible = visible;
		}
	}
	KillObj("ActivityName");

	// 读取DailyQusetdata
	list<tstring> listDaily;
	tstring szDailyProto =  _T("data\\system\\attdata\\DailyQuest_proto.xml");
	TObjRef<VarContainer> varDailyProto = CreateObj("DailyProto", "VarContainer");
	if (!varDailyProto->Load("VFS_System", szDailyProto.c_str(), "id", &listDaily))
	{
		IMSG(_T("Load DailyProto file failed"));
		KillObj("DailyProto");
		return FALSE;
	}
	list<tstring>::iterator iend2 = listDaily.end();
	for (list<tstring>::iterator begin = listDaily.begin(); begin != iend2; ++begin)
	{
		tagDailyQuestProtoData data;
		data.id		=  varDailyProto->GetInt(_T("id"), begin->c_str(), 0);
		data.preID  =  varDailyProto->GetInt(_T("PrevQuestID"), begin->c_str(), 0);
		data.minLevel = varDailyProto->GetInt(_T("MinLevel"), begin->c_str(), 0);
		data.maxLevel = varDailyProto->GetInt(_T("MaxLevel"), begin->c_str(), 0);
		data.acceptTimes = varDailyProto->GetInt(_T("TimesOneDay"), begin->c_str(), 0);
		data.acceptNpcID = varDailyProto->GetInt(_T("Npcid"), begin->c_str(), 0);
		data.doneTimes  = 0;
		m_mapDaily.insert(make_pair(data.id,data));
	}
	KillObj("DailyProto");

	list<tstring> listDailyName;
	tstring szDailyName = g_strLocalPath + _T("\\attdata\\DailyQuest_name.xml");
	TObjRef<VarContainer> varDailyName = CreateObj("DailyName", "VarContainer");
	if (!varDailyName->Load("VFS_System", szDailyName.c_str(), "id", &listDailyName))
	{
		IMSG(_T("Load DailyName file failed"));
		KillObj("DailyName");
		return FALSE;
	}
	list<tstring>::iterator iends = listDailyName.end();
	for (list<tstring>::iterator begin = listDailyName.begin(); begin != iends; ++begin)
	{
		UINT16 id = varDailyName->GetInt(_T("id"), begin->c_str(), 0);
		tstring name = varDailyName->GetString(_T("QuestName"), begin->c_str(), _T(""));
		tstring desc = varDailyName->GetString(_T("Introduction"), begin->c_str(), _T(""));
		DailyProtoMap::iterator it = m_mapDaily.find(id);
		if (it != m_mapDaily.end())
		{
			it->second.name = name;
			it->second.desc = desc;
		}
	}
	KillObj("DailyName");

	return TRUE;
}

const ActivityProtoData::ActivityProtoMap& ActivityProtoData::GetData() const
{
	return m_mapActivityProtoData;
}
const ActivityProtoData::DailyProtoMap& ActivityProtoData::GetDailyData() const
{
	return m_mapDaily;
}

const tagActivityProtoData * ActivityProtoData::GetQuestProto(UINT16 id) const
{
	ActivityProtoMap::const_iterator iter = m_mapActivityProtoData.find(id);
	if (iter != m_mapActivityProtoData.end())
		return &(iter->second);
	return NULL;
}

const tagDailyQuestProtoData * ActivityProtoData::GetDailyProto(UINT16 id) const
{
	DailyProtoMap::const_iterator iter = m_mapDaily.find(id);
	if (iter != m_mapDaily.end())
		return &(iter->second);
	return NULL;
}
VOID ActivityProtoData::ClearDailyQuestDoneTimes()
{
	for (DailyProtoMap::iterator iter = m_mapDaily.begin();
		iter != m_mapDaily.end();++iter)
	{
		iter->second.doneTimes = 0;
	}
}
