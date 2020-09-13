#pragma once
#include "Loong_CommonDef.h"


class CServerMerge
{
public:
	CServerMerge(void);
	virtual ~CServerMerge(void);
public:

	virtual BOOL Init(tagConfigDetail	stConfigOne[EM_DB_TYPE_END],
					  tagConfigDetail	stConfigTwo[EM_DB_TYPE_END],
					  EDBMergeType		eMergeType,
					  BOOL				bDataClean,
					  tstring			strSrc2WorldName,
					  INT				nSurviveDay,
					  const std::multimap<ERoleCompensationType, tagCompensation*> mmCompensation) = 0;

	virtual BOOL ServerMerge() = 0;
};
