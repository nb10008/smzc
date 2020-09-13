#pragma once

#pragma pack(push, 1)
#include "msg_common_errorcode.h"

#define EXTERNAL_LINKS_NUM	6				//外部链接最多个数

struct tagExternalLink
{
	TCHAR linkName[8];
	TCHAR linkValue[X_LONG_NAME];

	tagExternalLink()
	{
		memset( linkName, 0, 8 );
		memset( linkValue, 0, X_LONG_NAME );
	}
};

CMD_START(NS_GetExternalLinks)
	DWORD				dwElementNum;
	tagExternalLink		links[1];
CMD_END

#pragma pack(pop)