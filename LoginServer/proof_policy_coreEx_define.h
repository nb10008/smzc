//-----------------------------------------------------------------------------
//!\file proof_policy_cornEx_define.h
//!\author zxzhang
//!
//!\date 2011-9-5
//! last 2011-9-5
//!
//!\brief 韩国登入验证策略的定义 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.

#pragma once
#pragma pack(push, 1)

#define X_KR_DIVIDE_SIGN '|' //发送棒子消息的分隔符
#define X_KR_GAME_TYPE 1	 //棒子的游戏类型是1
#define X_KR_PROOF_SIGN "1"  //验证信息

struct tagKoreaBaseNetCmd
{
	char	czChar1;
	char	czChar2;
	UINT	nPacketSize;
	UINT16	nPacketProctolSize;
	tagKoreaBaseNetCmd()
	{
		memset(this,0,sizeof(*this));
		czChar1 = 'D';
		czChar2 = 'F';
	}
};


struct tagNHC_KoreaUserInfoProof : public tagKoreaBaseNetCmd
{
	UINT   nUsn;
	CHAR   czHashValue[1];
	UINT   nGameIDX;
};

struct tagNHS_KoreaUserInfoProof : public tagKoreaBaseNetCmd
{
	UINT  nUsn;
	UINT  nResultVal;
};


#pragma pack(pop)