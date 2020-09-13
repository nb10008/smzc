#include "StdAfx.h"
#include "GUICameraData.h"

GUICameraData::GUICameraData(void)
{
}

GUICameraData::~GUICameraData(void)
{
}

BOOL GUICameraData::LoadFromFile()
{
	// 读取Traindata对应的名字和描述
	list<tstring> listName;
	tstring szTrainName = g_strLocalPath + _T("\\attdata\\uieffect.xml");
	TObjRef<VarContainer> varTrainName = CreateObj("CameraData", "VarContainer");
	if (!varTrainName->Load("VFS_System", szTrainName.c_str(), "id", &listName))
	{
		IMSG(_T("Load CameraData file failed"));
		KillObj("CameraData");
		return FALSE;
	}
	list<tstring>::iterator iend = listName.end();
	for (list<tstring>::iterator begin = listName.begin(); begin != iend; ++begin)
	{
		tagGUICameraData data;
		data.id = varTrainName->GetInt(_T("id"), begin->c_str(),0);

		data.V3CameraFrom.x = varTrainName->GetFloat(_T("CamFromX"), begin->c_str(),0.0);
		data.V3CameraFrom.y = varTrainName->GetFloat(_T("CamFromY"), begin->c_str(),0.0);
		data.V3CameraFrom.z = varTrainName->GetFloat(_T("CamFromZ"), begin->c_str(),-190.0);

		data.V3CameraLookAt.x = varTrainName->GetFloat(_T("CamAtX"), begin->c_str(),0.0);
		data.V3CameraLookAt.y = varTrainName->GetFloat(_T("CamAtY"), begin->c_str(),0.0);
		data.V3CameraLookAt.z = varTrainName->GetFloat(_T("CamAtZ"), begin->c_str(),0.0);

		data.V3EffectPos.x = varTrainName->GetFloat(_T("PosX"), begin->c_str(),0.0);
		data.V3EffectPos.y = varTrainName->GetFloat(_T("PosY"), begin->c_str(),0.0);
		data.V3EffectPos.z = varTrainName->GetFloat(_T("PosZ"), begin->c_str(),0.0);

		data.V3EffectScale.x = varTrainName->GetFloat(_T("ScaleX"), begin->c_str(),1.0);
		data.V3EffectScale.y = varTrainName->GetFloat(_T("ScaleY"), begin->c_str(),1.0);
		data.V3EffectScale.z = varTrainName->GetFloat(_T("ScaleZ"), begin->c_str(),1.0);

		data.strEffectName = varTrainName->GetString(_T("txname"), begin->c_str(),_T(""));
		data.eMode	= varTrainName->GetInt(_T("Blend_mode"), begin->c_str(),1);

		DWORD dwCRC = TObjRef<Util>()->Crc32(data.strEffectName.c_str());
		m_mapCameraData.insert(make_pair(dwCRC,data));
	}
	KillObj("CameraData");

	return TRUE;
}
const tagGUICameraData* GUICameraData::GetCameraData(DWORD id) const
{
	CameraDataMap::const_iterator iter = m_mapCameraData.find(id);
	if (iter != m_mapCameraData.end())
		return &(iter->second);
	return NULL;
}