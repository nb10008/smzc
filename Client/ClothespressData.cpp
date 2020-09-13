#include "StdAfx.h"
#include "ClothespressData.h"

ClothespressData::ClothespressData(void)
{
}

ClothespressData::~ClothespressData(void)
{
}

ClothespressData* ClothespressData::Inst()
{
	static ClothespressData inst;
	return &inst;
}

void ClothespressData::LoadFromFile()
{
	const TCHAR* szSuitProto = _T("data\\system\\attdata\\wardrobe_proto.xml");
	tstring strPath = g_strLocalPath + _T("\\attdata\\wardrobe_name.xml");
	LPCTSTR szSuitName = strPath.c_str(); 

	list<tstring> protoList;
	list<tstring> nameList;
	list<tstring>::iterator iter;

	TObjRef<VarContainer> varProto = CreateObj( "ClothespressData", "VarContainer" );
	if(!varProto->Load("VFS_System", szSuitProto, "id", &protoList))
		IMSG(_T("Load file wardrobe_proto.xml failed\r\n"));

	for(iter=protoList.begin(); iter!=protoList.end(); ++iter)
	{
		tagClothespressData FashionData;
		FashionData.nSuitId	= varProto->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		TCHAR szTemp[X_SHORT_NAME] = {0};
		for( int i = 0; i < 5; i++ )
		{
			_sntprintf(szTemp ,X_SHORT_NAME, _T("wardrobe_field%d"), i + 1 );
			FashionData.dwSuitTypeID[i] = varProto->GetDword(szTemp, (*iter).c_str(), GT_INVALID);
		}

		for( int i = 0; i < 3; i++ )
		{
			_sntprintf(szTemp ,X_SHORT_NAME, _T("buff_id%d"), i + 1 );
			FashionData.dwBufferID[i] = varProto->GetDword(szTemp, (*iter).c_str(), GT_INVALID);
		}

		INT nType = varProto->GetInt(_T("wardrobe_type"), (*iter).c_str(), GT_INVALID);
		if( nType == 1 )
		{
			INT nSex =  varProto->GetInt(_T("wardrobe_sex"), (*iter).c_str(), GT_INVALID);
			if( nSex == 1 ) // ÄÐ×°
				m_vecMaleFashion.push_back(FashionData);
			else if( nSex == 2 ) // Å®×°
				m_vecFemaleFashion.push_back(FashionData);
		}
		else if( nType == 2 )
			m_DiyFashion = FashionData;
		m_mapFashion.insert(make_pair(FashionData.nSuitId, FashionData));
	}

	KillObj("ClothespressData");

	TObjRef<VarContainer> varName = CreateObj( "SuitName", "VarContainer" );
	if(!varName->Load("VFS_System", szSuitName, "id", &nameList))
		IMSG(_T("Load file wardrobe_name.xml failed\r\n"));

	for(iter=nameList.begin(); iter!=nameList.end(); ++iter)
	{
		DWORD dwID = varName->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tstring strSuitName = varName->GetString( _T("wardrobe_name"),	(*iter).c_str(), _T("") );
		m_mapSuitName.insert(make_pair(dwID, strSuitName));
	}

	KillObj("SuitName");
}

tstring ClothespressData::GetSuitName(INT nSuitID)
{
	map<DWORD, tstring>::iterator iter = m_mapSuitName.find(nSuitID);
	if(iter != m_mapSuitName.end())
		return iter->second;
	return _T("");
}

tagClothespressData* ClothespressData::GetFashionInfo(DWORD dwIndex)
{
	map<DWORD, tagClothespressData>::iterator iter = m_mapFashion.find(dwIndex);
	if(iter != m_mapFashion.end())
		return &iter->second;
	return NULL;
}