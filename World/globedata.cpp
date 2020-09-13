/********************************************************************
	created:	2012/05/31
	created:	31:5:2012   18:01
	filename: 	e:\code\World\globedata.cpp
	file ext:	cpp
	author:		zhangzhihua
	
	purpose:	游戏全局变量管理器
*********************************************************************/
#include "StdAfx.h"
#include "globedata.h"

GlobeData::GlobeData()
{
	memset(m_nValue, 0, sizeof(m_nValue));
}

GlobeData::~GlobeData()
{

}

VOID GlobeData::Init()
{

}

INT GlobeData::Get(EGlobeData eKey)
{
	if (EGD_End == eKey)
	{
		return 0;
	}

	return m_nValue[eKey];
}

