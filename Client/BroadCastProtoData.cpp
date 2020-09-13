#include "StdAfx.h"
#include "BroadCastProtoData.h"

BOOL BroadCastProtoData::LoadFromFile()
{
	// 读取广播信息
	list<tstring> listProto;
	tstring szBroadcastProto = _T("data\\local\\zhCN\\broadcast_list.xml");
	TObjRef<VarContainer> varBroadcastProto = CreateObj("BroadcastProto", "VarContainer");
	if (!varBroadcastProto->Load("VFS_System", szBroadcastProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load Broadcast proto file failed"));
		KillObj("BroadcastProto");
		return FALSE;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagBroadCastProtoData data;

		data.dwBoardcastID  = varBroadcastProto->GetInt(_T("id"), begin->c_str());
		data.eBoardcastType = static_cast<ENewBroadcastType>(varBroadcastProto->GetInt(_T("type"), begin->c_str()));
		data.eChannelType   = static_cast<ENewChannelType>(varBroadcastProto->GetInt(_T("channel"), begin->c_str()));
		data.Content		= varBroadcastProto->GetString(_T("value"), begin->c_str(), _T(""));
		m_mapBroadCastProtoData.insert(make_pair(data.dwBoardcastID,data));

	}
	KillObj("BroadcastProto");

	// 读取频道信息
	list<tstring> listChannel;
	tstring szChannel = _T("data\\system\\attdata\\ChannelType.xml");
	TObjRef<VarContainer> varChannelName = CreateObj("BroadcastChannel", "VarContainer");
	if (!varChannelName->Load("VFS_System", szChannel.c_str(), "id", &listChannel))
	{
		IMSG(_T("Load BroadcastChannel file failed"));
		KillObj("BroadcastChannel");
		return FALSE;
	}

	list<tstring>::iterator iend = listChannel.end();
	for (list<tstring>::iterator begin = listChannel.begin(); begin != iend; ++begin)
	{

		tagChannelProtoData channelData;

		channelData.eChannelType	= static_cast<ENewChannelType>(varChannelName->GetInt(_T("id"), begin->c_str()));
		channelData.ChannelName		= varChannelName->GetString(_T("name"), begin->c_str(), _T(""));
		tstring color				= varChannelName->GetString(_T("color"), begin->c_str());
		channelData.dwColour		= _tcstoul(color.c_str(), NULL, 16);
		channelData.npriority       = varChannelName->GetInt(_T("priority"), begin->c_str());



		m_mapChannelProtoMap.insert(make_pair(channelData.eChannelType,channelData));
	}
	KillObj("BroadcastChannel");

	return TRUE;

}

const BroadCastProtoData::BroadCastProtoMap& BroadCastProtoData::GetBroadCastProtoMap() const
{
	return m_mapBroadCastProtoData;
}

const tagBroadCastProtoData * BroadCastProtoData::GetBroadCastProto(DWORD id) const
{
	BroadCastProtoMap::const_iterator iter = m_mapBroadCastProtoData.find(id);
	if (iter != m_mapBroadCastProtoData.end())
		return &(iter->second);

	return NULL;
}

const tagChannelProtoData * BroadCastProtoData::GetChannelProto(ENewChannelType channel) const
{
	ChannelProtoMap::const_iterator iter = m_mapChannelProtoMap.find(channel);
	if (iter != m_mapChannelProtoMap.end())
		return &(iter->second);

	return NULL;
}


