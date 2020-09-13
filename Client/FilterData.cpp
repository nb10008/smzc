#include "stdafx.h"
#include "FilterData.h"
#include "..\WorldDefine\msg_common_errorcode.h"

using namespace Filter;

FilterData::FilterData( void )
{

}

FilterData::~FilterData( void )
{

}

FilterData g_FilterData;
FilterData* FilterData::Inst()
{
	return &g_FilterData;
}

void FilterData::LoadFromFile()
{
	tstring strPath1 = g_strLocalPath + _T("\\attdata\\name_filter.xml");
	const TCHAR* szNameFilter = strPath1.c_str();
	tstring strPath2 = g_strLocalPath + _T("\\attdata\\chat_filter.xml");
	const TCHAR* szChatFilter = strPath2.c_str();

	list<tstring> nameList;
	list<tstring> chatList;
	list<tstring>::iterator iter;

	//加载姓名过滤词表
	TObjRef<VarContainer> varNameFilter = CreateObj( "NameFilter", "VarContainer" );
	if(!varNameFilter->Load("VFS_System", szNameFilter, "id", &nameList))
		IMSG(_T("Load file name_filter.xml failed\r\n"));

	for( iter=nameList.begin(); iter!=nameList.end(); ++iter )
	{
		tstring strNameFilter;
		strNameFilter = varNameFilter->GetString(_T("name"), (*iter).c_str(), _T(""));
		m_vecNameFilter.push_back(strNameFilter);
	}

	KillObj("NameFilter");

	//加载聊天过滤词表
	TObjRef<VarContainer> varChatFilter = CreateObj( "ChatFilter", "VarContainer" );
	if(!varChatFilter->Load("VFS_System", szChatFilter, "id", &chatList))
		IMSG(_T("Load file chat_filter.xml failed\r\n"));

	for( iter=chatList.begin(); iter!=chatList.end(); ++iter )
	{
		tstring strChatFilter;
		strChatFilter = varChatFilter->GetString(_T("name"), (*iter).c_str(), _T(""));
		m_vecChatFilter.push_back(strChatFilter);
	}

	//初始化系统图片表
	m_vecPicFilter.push_back(_T("//gg"));
	m_vecPicFilter.push_back(_T("//xt"));

	KillObj("ChatFilter");
}

DWORD FilterData::IsValidName(LPCTSTR name, INT nMax/*=7*/, INT nMin/*=1*/)
{
	DWORD dwError;
	dwError = CheckName(name, nMax, nMin, &m_vecNameFilter);
	if( dwError!=E_Success )
		return dwError;

	return E_Success;
}

DWORD FilterData::WordsFilteror( tstring& strText, INT nMax/*=200*/ )
{
	DWORD dwError;
	BOOL bFindFlag = FALSE;
	dwError = WordsFilterForChat( strText, bFindFlag, &m_vecChatFilter );
	if( dwError!=E_Success )
		return dwError;

	if(strText.size() > (DWORD)nMax)
		return E_Filter_Text_TooLong;

	return E_Success;
}

DWORD FilterData::SysPicFilteror(tstring& strText, INT nMax/* =200 */)
{
	DWORD dwError;
	BOOL bFindFlag = FALSE;
	dwError = WordsFilterForChat(strText, bFindFlag, &m_vecPicFilter);
	if(dwError != E_Success)
		return dwError;

	if(strText.size() > (DWORD)nMax)
		return E_Filter_Text_TooLong;

	return E_Success;
}
							

tstring FilterData::IsContainFilter( tstring& strText )
{
	size_t textLen = strText.size();	//待检测的字符串的长度
	if( textLen == 0 )
		return _T("");

	tstring strTemp;
	strTemp.assign(strText);
	transform(strText.begin(), strText.end(), strText.begin(), tolower);
	size_t index = 0;
	size_t fLen = m_vecChatFilter.size();		//表的大小

	for( size_t i=0; i<fLen; ++i )
	{
		index = 0;
		size_t length = m_vecChatFilter.at(i).size();
		if(textLen < length)
			continue;

		tstring strFind = m_vecChatFilter.at(i);
		transform(strFind.begin(), strFind.end(), strFind.begin(), tolower);
		index = strText.find(strFind, index);
		while( index != string::npos )
		{
			return strFind;
		}
	}

	return _T("");
}