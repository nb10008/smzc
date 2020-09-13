#include "StdAfx.h"
#include "bloodbrother.h"
#include "..\WorldDefine\brother_define.h"

BloodbrotherMgr::BloodbrotherMgr(void)
{
}

BloodbrotherMgr::~BloodbrotherMgr(void)
{
}

bool BloodbrotherMgr::CanMakeBrother()
{
	return (0 == m_data.dwTeatherID);
}

void BloodbrotherMgr::InitData(DWORD dwID,tagDWORDTime time)
{
	m_data.init(dwID,time);
}