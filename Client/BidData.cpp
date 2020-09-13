#include "StdAfx.h"
#include "BidData.h"

BidData::BidData(void)
{
}

BidData::~BidData(void)
{
}

void BidData::LoadFormFile()
{
	list<tstring> listProto;
	tstring szTrainProto = _T("data\\system\\attdata\\bid_proto.xml");
	TObjRef<VarContainer> varTrainProto = CreateObj("GodBidData", "VarContainer");
	if (!varTrainProto->Load("VFS_System", szTrainProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load GodBidData file failed"));
		KillObj("GodBidData");
		return;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagBidDataLocal data;

		data.dwID = varTrainProto->GetDword(_T("id"), begin->c_str(),0);
		data.dwItemID = varTrainProto->GetDword(_T("item_id"), begin->c_str(),0);
		data.dwMinBidPrice = varTrainProto->GetDword(_T("bid_min"), begin->c_str(),0);
		data.dwStartPrice =  varTrainProto->GetDword(_T("bid_end"), begin->c_str(),0);
		data.eType = (EBidType)varTrainProto->GetInt(_T("bid_form"), begin->c_str(),0);
		data.nNum =varTrainProto->GetInt(_T("bid_quantity"), begin->c_str(),0);

		m_mapBidMap.insert(make_pair(data.dwID,data));

	}
	KillObj("GodBidData");
}
const tagBidDataLocal* BidData::GetBidData(const DWORD& id) const
{
	BidMap::const_iterator iter = m_mapBidMap.find(id);
	if (iter != m_mapBidMap.end())
	{
		return &(iter->second);
	}
	return NULL;
}
