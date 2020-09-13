#pragma once
//--------------------------------------------------------------------------------------------
// Copyright (c) 2010 DACHENG Entertainment All rights reserved.
// filename: BroadCastProtoData.h
// author: Su.Zhang
// actor:
// data: 2010-3-31
// last:
// brief: 广播数据结构定义
//--------------------------------------------------------------------------------------------

enum ENewBroadcastType 
{
	ENBT_BigNewBroadcast,   // 大字屏显
	ENBT_SmallRunHorse, 	// 小字跑马灯
	ENBT_BigRunHorse,		// 大字跑马灯
	ENBT_ChatNew		    // 聊天栏公告

};
enum ENewChannelType 
{
	ENCT_CityWar = 1,       // 城战频道
	ENCT_Guild,				// 帮派频道
	ENCT_Affiche,			// 公告频道
	ENCT_Special,			// 特殊频道
	ENCT_Activity			// 活动频道
};

// 广播静态数据结构
struct tagBroadCastProtoData
{
	DWORD					dwBoardcastID;
	ENewBroadcastType		eBoardcastType;
	ENewChannelType			eChannelType;
	tstring					Content;

};

struct tagChannelProtoData
{
	ENewChannelType			eChannelType;
	tstring					ChannelName;
	DWORD					dwColour;
	INT						npriority;
};


class BroadCastProtoData
{
public:
	typedef std::map<DWORD, tagBroadCastProtoData> BroadCastProtoMap;
	typedef std::map<ENewChannelType, tagChannelProtoData> ChannelProtoMap;
public:

	BOOL LoadFromFile();
	const BroadCastProtoMap& GetBroadCastProtoMap() const;
	const tagBroadCastProtoData *GetBroadCastProto(DWORD id) const;
	const tagChannelProtoData *GetChannelProto(ENewChannelType channel) const;
private:
	BroadCastProtoMap m_mapBroadCastProtoData;	
	ChannelProtoMap   m_mapChannelProtoMap;
};

