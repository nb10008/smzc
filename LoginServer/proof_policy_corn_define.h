//-----------------------------------------------------------------------------
//!\file proof_policy_corn_define.h
//!\author Lamoair
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief 韩国登入器 验证策略的定义 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.

#pragma once
#pragma pack(push, 1)

struct tagDecryptData{

	string	NationCode;		    //国家代码
	string	PortalSiteCode;			
	string	UserID;			   
	string	SecurityCode;				
	string	UserAge;			
    string	GameStartTime;
	tagDecryptData()
	{
			
	}

};

#define  MAXSTR_SIZE   256       //加密字符串的最大长度
#define  SECKEY_STRCOUNT 6       //加密字符解密后得到6个字符
#define  SECKEY_FLAG   22        //加密字符标志

#pragma pack(pop)