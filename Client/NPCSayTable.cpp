#include "stdafx.h"
#include "NPCSayTable.h"

NPCSayTable::NPCSayTable( void )
{

}

NPCSayTable::~NPCSayTable( void )
{

}

NPCSayTable* NPCSayTable::Inst()
{
	static NPCSayTable inst;
	return &inst;
}

void NPCSayTable::LoadFromFile()
{
	tstring strPath = g_strLocalPath + _T("\\NPCtalk.xml");

	list<tstring> sayList;
	TObjRef<VarContainer> con = CreateObj( "NPCSayTable", "VarContainer" );
	if(!con->Load("VFS_System", strPath.c_str(), "id", &sayList))
	{
		IMSG(_T("Load file NPCtalk.xml failed\r\n"));
		return;
	}
	list<tstring>::iterator end = sayList.end();
	for (list<tstring>::iterator iter = sayList.begin(); iter != end; ++iter)
	{
		tagNPCSay npcSay;
		npcSay.dwNPCID = con->GetDword(_T("id"), iter->c_str(), 0);
		npcSay.strSay= con->GetString(_T("value"), iter->c_str(), _T(""));
		npcSay.nRate = con->GetInt(_T("random"), iter->c_str(), 0);
		m_mapSayProto.insert(make_pair(npcSay.dwNPCID, npcSay));
	}
	KillObj("NPCSayTable");
}