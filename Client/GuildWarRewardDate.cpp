#include "StdAfx.h"
#include "GuildWarRewardDate.h"

GuildWarRewardDate::GuildWarRewardDate(void)
{
}

GuildWarRewardDate::~GuildWarRewardDate(void)
{
	m_mapGuildWarReward.clear();
}


BOOL	GuildWarRewardDate::LoadFormFile()
{
	list<tstring> listProto;
	m_mapGuildWarReward.clear();
	tstring szTrainProto = _T("data\\system\\attdata\\war_award.xml");
	TObjRef<VarContainer> varTrainProto = CreateObj("GuildWarRewardProto", "VarContainer");
	if (!varTrainProto->Load("VFS_System", szTrainProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load GuildWarRewardProto file failed"));
		KillObj("GuildWarRewardProto");
		return FALSE;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		GuildRewardData data;

		data.ePos = (EGuildPos)varTrainProto->GetInt(_T("identity"), begin->c_str(),0);
		data.nOccupyNum = varTrainProto->GetInt(_T("num"), begin->c_str(),0);
		data.dwItem =  varTrainProto->GetDword(_T("item_id"), begin->c_str(),0);
		data.dwBuff =  varTrainProto->GetDword(_T("buff_id"), begin->c_str(),0);
		
		m_mapGuildWarReward.insert(make_pair(make_pair(data.ePos,data.nOccupyNum),data));

	}
	KillObj("GuildWarRewardProto");

	return TRUE;
}

const GuildRewardData* GuildWarRewardDate::FindData(const EGuildPos& pos,const INT& num) const
{
	RewardMap::const_iterator iter = m_mapGuildWarReward.find(make_pair(pos,num));
	if (iter != m_mapGuildWarReward.end())
	{
		return &(iter->second);
	}

	return NULL;
}