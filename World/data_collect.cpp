#include "stdafx.h"
#include "data_collect.h"
#include "db_session.h"
#include "../ServerDefine/msg_log.h"

class DataCollection g_DataCollection;

VOID DataCollection::Init(const DWORD* pData)
{
	if (!m_bOpen)
	{
		return;
	}

	memcpy(m_data, pData, sizeof(DWORD)*DataCollection_Max_Data_Num );
}

VOID DataCollection::Update()
{
	if (!m_bOpen)
	{
		return;
	}

	if (m_nSaveCount < DataCollection_Save_DB_Internal)
	{
		++m_nSaveCount;
		return;
	}

	m_nSaveCount = 0;

	tagNDBC_LogWorldData log;
	memcpy(log.dwData, m_data, sizeof(DWORD)*DataCollection_Max_Data_Num);
	g_dbSession.Send(&log, log.dwSize);
}