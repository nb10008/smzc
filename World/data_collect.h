//--------------------------------------------------------------------------
// 类名：游戏数据收集类
// 说明：由脚本控制记录一些统计性的日志数据
//--------------------------------------------------------------------------
#pragma once
#include "../ServerDefine/data_collect.h"

class DataCollection
{
public:
	DataCollection() {m_nSaveCount = 0; m_bOpen = true;}

	VOID Init(const DWORD* pData);
	VOID Update();

	VOID SetOpen(bool bStat) { m_bOpen = bStat; }

	DWORD GetData(BYTE byIndex) 
	{ 
		if (!m_bOpen)
		{
			return 0;
		}
	
		if(DataCollection_Index_Valid(byIndex)) return m_data[byIndex];
	}

	VOID SetData(BYTE byIndex, DWORD dwdata) 
	{ 
		if (!m_bOpen)
		{
			return;
		}

		if(DataCollection_Index_Valid(byIndex)) m_data[byIndex] = dwdata; 
	}
private:
	bool m_bOpen;
	DWORD m_data[DataCollection_Max_Data_Num];
	INT m_nSaveCount;
};

extern DataCollection g_DataCollection;