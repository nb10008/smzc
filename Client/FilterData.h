#pragma once
#include ".\BaseData.h"
#include "..\WorldDefine\filter.h"
class FilterData
	: public BaseData
{
public:
	FilterData(void);
	~FilterData(void);

	static FilterData* Inst();

	/** 载入过滤词文件
	*/
	virtual void LoadFromFile();

	/** 名字是否全法
	*/
	DWORD IsValidName(LPCTSTR name, INT nMax=7, INT nMin=1);

	/** 聊天过滤
	*/
	DWORD WordsFilteror(tstring& strText, INT nMax=200);

	/** 功率系统图片
	*/
	DWORD SysPicFilteror(tstring& strText, INT nMax=200);

	/** 检查过滤词是否包含在 目标字符串中
	*/
	tstring IsContainFilter( tstring& strText );

private:
	vector<tstring>					m_vecNameFilter;
	vector<tstring>					m_vecChatFilter;
	vector<tstring>					m_vecPicFilter;
};