#include "StdAfx.h"
#include "TrainProtoData.h"


BOOL TrainProtoData::LoadFromFile()
{
	//	读取trial_proto
	list<tstring> listProto;
	tstring szTrainProto = _T("data\\system\\attdata\\trial_proto.xml");
	TObjRef<VarContainer> varTrainProto = CreateObj("TrainProto", "VarContainer");
	if (!varTrainProto->Load("VFS_System", szTrainProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load Train proto file failed"));
		KillObj("TrainProto");
		return FALSE;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagTrainProtoData data;

		data.dwTrainID = varTrainProto->GetInt(_T("id"), begin->c_str());
		data.eType = ETS_Close;
		data.nCapability = varTrainProto->GetInt(_T("capability"),begin->c_str(),0);
		data.dwItem1ID = varTrainProto->GetInt(_T("Item1ID"),begin->c_str(),0);
		data.nItem1Num = varTrainProto->GetInt(_T("Item1Num"),begin->c_str(),0);
		data.byItem1Quality = varTrainProto->GetInt(_T("Item1Quality"),begin->c_str(),0);
		data.dwItem2ID = varTrainProto->GetInt(_T("Item2ID"),begin->c_str(),0);
		data.nItem2Num = varTrainProto->GetInt(_T("Item2Num"),begin->c_str(),0);
		data.byItem2Quality = varTrainProto->GetInt(_T("Item2Quality"),begin->c_str(),0);
		data.nLevelOneType = varTrainProto->GetInt(_T("type1"),begin->c_str(),0);
		data.nLevelTwoType = varTrainProto->GetInt(_T("type2"),begin->c_str(),0);
		data.nMaxNum = varTrainProto->GetInt(_T("maxnum"),begin->c_str(),0);
		data.dwMoney = varTrainProto->GetDword(_T("money"),begin->c_str(),0);
		data.dwExp = varTrainProto->GetDword(_T("exp"),begin->c_str(),0);
		data.bVisible = varTrainProto->GetInt(_T("visible"),begin->c_str(),0);
		data.nCurrentNum = varTrainProto->GetInt(_T("curnum"),begin->c_str(),0);
		m_mapTrainProtoData.insert(make_pair(data.dwTrainID,data));

	}
	KillObj("TrainProto");

	// 读取Traindata对应的名字和描述
	list<tstring> listName;
	tstring szTrainName = g_strLocalPath + _T("\\attdata\\trial_name.xml");
	TObjRef<VarContainer> varTrainName = CreateObj("TrainName", "VarContainer");
	if (!varTrainName->Load("VFS_System", szTrainName.c_str(), "id", &listName))
	{
		IMSG(_T("Load Train name file failed"));
		KillObj("TrainName");
		return FALSE;
	}
	list<tstring>::iterator iend = listName.end();
	for (list<tstring>::iterator begin = listName.begin(); begin != iend; ++begin)
	{
		UINT16 id = varTrainName->GetInt(_T("id"), begin->c_str(), 0);
		tstring name			  = varTrainName->GetString(_T("name"), begin->c_str(), _T(""));
		tstring GoalDescription   =	varTrainName->GetString(_T("Goal_Desc"), begin->c_str(), _T(""));
		tstring FinishDescription = varTrainName->GetString(_T("Finishi_Desc"), begin->c_str(), _T(""));
		tstring StartDescription  =	varTrainName->GetString(_T("Start_Desc"), begin->c_str(), _T(""));
		tstring Desc1  =	varTrainName->GetString(_T("type1name"), begin->c_str(), _T(""));
		tstring Desc2  =	varTrainName->GetString(_T("type2name"), begin->c_str(), _T(""));

		TrainProtoMap::iterator it = m_mapTrainProtoData.find(id);
		if (it != m_mapTrainProtoData.end())
		{
			it->second.Name = name;
			it->second.GoalDescription = GoalDescription;
			it->second.FinishDescription = FinishDescription;
			it->second.StartDescription = StartDescription;
			it->second.DescLevelOne = Desc1;
			it->second.DescLevelTwo = Desc2;
		}
	}
	KillObj("TrainName");

	return TRUE;
}

const TrainProtoData::TrainProtoMap& TrainProtoData::GetData() const
{
	return m_mapTrainProtoData;
}

const tagTrainProtoData * TrainProtoData::GetTrainProto(DWORD id) const
{
	TrainProtoMap::const_iterator iter = m_mapTrainProtoData.find(id);
	if (iter != m_mapTrainProtoData.end())
		return &(iter->second);
	return NULL;
}


