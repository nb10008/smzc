#include "StdAfx.h"
#include "OverSeaData.h"

OverSeaData::OverSeaData(void):m_Trunk(this)
{
}

OverSeaData::~OverSeaData(void)
{
}

OverSeaData OverSeaData::s_gOverSeaData;

OverSeaData* OverSeaData::Inst()
{
	return &s_gOverSeaData;
}

void OverSeaData::Init()
{
	m_pCmdMgr	= TObjRef<NetCmdMgr>();
	m_pSession	= TObjRef<NetSession>();
	m_pFrameMgr	= TObjRef<GameFrameMgr>();
	LoadFormFile();
	m_bOpenFunc = true;
}

void OverSeaData::LoadFormFile()
{
	m_listFormualID.clear();
	list<tstring> listProto;
	tstring szTrainProto = _T("data\\system\\attdata\\overseas_hide.xml");
	TObjRef<VarContainer> varTrainProto = CreateObj("overseas_hide", "VarContainer");
	if (!varTrainProto->Load("VFS_System", szTrainProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load overseas_hide file failed"));
		KillObj("overseas_hide");
		return;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		if (OverSea_Formual == varTrainProto->GetDword(_T("type"), begin->c_str(),0))
		{
			m_listFormualID.push_back(varTrainProto->GetDword(_T("typeid"), begin->c_str(),0));
		}

		if (OverSea_Quest == varTrainProto->GetDword(_T("type"), begin->c_str(),0))
		{
			m_listQuestID.push_back(varTrainProto->GetDword(_T("typeid"), begin->c_str(),0));
		}

		if (OverSea_NPC == varTrainProto->GetDword(_T("type"), begin->c_str(),0))
		{
			m_listNPCID.push_back(varTrainProto->GetDword(_T("typeid"), begin->c_str(),0));
		}
	}
	KillObj("overseas_hide");
}

bool OverSeaData::IsFormualHide(const DWORD& id)
{
	if(!m_bOpenFunc)	return false;

	for (list<DWORD>::iterator it = m_listFormualID.begin();
		it != m_listFormualID.end(); ++it)
	{
		if ((*it) == id)
			return true;
	}
	return false;
}

bool OverSeaData::IsQuestHide(const DWORD& id)
{
	if(!m_bOpenFunc)	return false;

	for (list<DWORD>::iterator it = m_listQuestID.begin();
		it != m_listQuestID.end(); ++it)
	{
		if ((*it) == id)
			return true;
	}
	return false;
}

bool OverSeaData::IsNPCHide(const DWORD& id)
{
	if(!m_bOpenFunc)	return false;

	for (list<DWORD>::iterator it = m_listNPCID.begin();
		it != m_listNPCID.end(); ++it)
	{
		if ((*it) == id)
			return true;
	}
	return false;
}