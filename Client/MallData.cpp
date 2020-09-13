#include "stdafx.h"
#include "MallData.h"

const INT nTitleLength = 8;

MallData::MallData( void )
{
	m_dwSize = 0;
}

MallData::~MallData( void )
{

}

MallData g_MallData;
MallData* MallData::Inst()
{
	return & g_MallData;
}

void MallData::LoadFromFile()
{
	tstring strPath = g_strLocalPath + _T("\\attdata\\mall_title.xml");
	tstring strPath2 = g_strLocalPath + _T("\\attdata\\mall_title_nd.xml");
	const TCHAR* szMallName = strPath.c_str();

	list<tstring> mallNameList;
	list<tstring> mallName2List;
	list<tstring>::iterator iter;

	//读取商城类型
	TObjRef<VarContainer> varMall = CreateObj( "MallProto", "VarContainer" );
	if(!varMall->Load("VFS_System", szMallName, "id", &mallNameList))
		IMSG(_T("Load file mall_title.xml failed"));

	for(iter=mallNameList.begin(); iter!=mallNameList.end(); ++iter)
	{	
		tagMallTitleData data;
		data.nIndex			= varMall->GetInt(_T("id"), (*iter).c_str(), 0) - 1;
		data.strTitleName	= varMall->GetString(_T("name1"), (*iter).c_str(), _T(""));

		m_mapMallData.insert(make_pair(data.nIndex, data));

		++m_dwSize;
	}

	KillObj("MallProto");

	//读取二级分类
	TObjRef<VarContainer> varMall2 = CreateObj( "MallProto2", "VarContainer" );
	if(!varMall2->Load("VFS_System", strPath2.c_str(), "id", &mallName2List))
		IMSG(_T("Load file mall_title_nd.xml failed"));

	for(iter=mallName2List.begin(); iter!=mallName2List.end(); ++iter)
	{	
		tagMallTitle2Data data;
		data.nIndex			= varMall2->GetInt(_T("id"), (*iter).c_str(), 0);
		data.strTitle2Name	= varMall2->GetString(_T("name1"), (*iter).c_str(), _T(""));

		m_mapMall2Data.insert(make_pair(data.nIndex, data));

		DWORD dwTitleID		= varMall2->GetInt(_T("mall_title_id"), (*iter).c_str(), 0);

		if ( dwTitleID <= 6 ) 
		{
			m_mmapMall1to2Data.insert(make_pair(dwTitleID, data));
		}
		
	}

	// 加入其它分类
	tagMallTitle2Data tData_Mark;
	tData_Mark.nIndex		= 0;
	tData_Mark.strTitle2Name	= _T("");
	m_mmapMall1to2Data.insert(make_pair(7, tData_Mark));

	// 加入其它分类
	tagMallTitle2Data tData_volume;
	tData_volume.nIndex		= 0;
	tData_volume.strTitle2Name	= _T("");
	m_mmapMall1to2Data.insert(make_pair(8, tData_volume));

	KillObj("MallProto2");
}