#include "StdAfx.h"
#include "BuyTalentData.h"

BuyTalentData::BuyTalentData(void)
{
}

BuyTalentData::~BuyTalentData(void)
{
}

BuyTalentData	g_BuyTalentData;

BuyTalentData* BuyTalentData::Inst()
{	
	return &g_BuyTalentData;
}

void BuyTalentData::LoadFromFile()
{
	const TCHAR* szPath = _T("data\\system\\attdata\\TalentAvail_buy.xml");

	list<tstring> costList;
	list<tstring>::iterator iter;

	//∂¡»°æ≠—È
	TObjRef<VarContainer> varBuy = CreateObj( "BuyTalentProto", "VarContainer" );
	if( !varBuy->Load( "VFS_System", szPath, "id", &costList ) )
		IMSG( _T("Load file role_att_level_up.xml failed\r\n") );
	
	
	for( iter=costList.begin(); iter!=costList.end(); ++iter )
	{
		INT nLevel,nCost;
		nLevel	= varBuy->GetInt( _T("id"),		(*iter).c_str(), 0 );
		nCost	= varBuy->GetInt( _T("cost"),	(*iter).c_str(), 0 );
		m_mapCost.insert( make_pair( nLevel, nCost ) );
	}

	KillObj("BuyTalentProto");
}