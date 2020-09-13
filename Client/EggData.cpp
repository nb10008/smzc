#include "StdAfx.h"
#include "EggData.h"

EggData::EggData(void)
{
}

EggData::~EggData(void)
{
	// ÊÍ·ÅÄÚ´æ
	for (map<int,tagEggLayerData*>::iterator it = m_mapEggLayerInfo.begin();
		 it != m_mapEggLayerInfo.end();++it)
	{
		SAFE_DELETE(it->second);
	}
}
void EggData::LoadFromFile()
{
	list<tstring> listProto;
	tstring szTrainProto = _T("data\\system\\attdata\\gold_egg.xml");
	TObjRef<VarContainer> varTrainProto = CreateObj("EggData", "VarContainer");
	if (!varTrainProto->Load("VFS_System", szTrainProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load EggData file failed"));
		KillObj("EggData");
		return;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagEggLayerData* data = new tagEggLayerData();

		data->nLayer = varTrainProto->GetDword(_T("id"), begin->c_str(),0) -1;
		data->nNormalHammers = varTrainProto->GetDword(_T("general_consum"), begin->c_str(),0);
		data->nColourHammers = varTrainProto->GetDword(_T("special_consum"), begin->c_str(),0);
		TCHAR buffer[64];
		for (int i =1; i<=20;i++)
		{
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("egg_item%d"), i);
			data->list_Items.push_back(varTrainProto->GetDword(buffer, begin->c_str(),0));
		}
		m_mapEggLayerInfo.insert(make_pair(data->nLayer,data));

	}
	KillObj("EggData");
}
const tagEggLayerData* EggData::GetEggLayerInfo(const int& nLayer)
{
	map<int,tagEggLayerData*>::iterator it = m_mapEggLayerInfo.find(nLayer);
	if (it != m_mapEggLayerInfo.end())
	{
		return it->second;
	}
	return NULL;
}