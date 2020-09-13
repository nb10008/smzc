//-----------------------------------------------------------------------------
//!\file	
//!\brief	
//!
//!\date	
//! last	
//!\author	
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------


#pragma once
//#include "type_define.h"
#include "world.h"

class CRoleInfoReader
{
public:
	CRoleInfoReader(void);
	~CRoleInfoReader(void);

	BOOL Load( LPCTSTR szFileName, TRoleMap &mapRoleMap );

private:
	TObjRef<Util>				m_pUtil;

};
