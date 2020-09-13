#include "stdafx.h"
#include "TimeChecker.h"

TObjRef<Log> TimeChecker::s_logger;

void TimeChecker::Init()
{
	static bool initialized = false;
	if(!initialized)
	{
		initialized = true;
		vEngine::g_pInterfaceMgr->Create("TimeCheckerLog", "Log");
		s_logger = "TimeCheckerLog";
		if( P_VALID(s_logger) )
			s_logger->Create();
	}
}