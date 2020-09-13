#pragma once
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\specfunc_define.h"
#include "GameClientDefine.h"

/** \本地玩家物品更新
*/
struct tagUpdateLPItemEvent : public tagGameEvent
{
	EItemConType	eType;
	DWORD			dwTypeID;
	INT16			n16Index;

	tagUpdateLPItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = EICT_Null; n16Index = GT_INVALID;}
};

/** \右键点击容器中的物品
*/
struct tagClickItemEvent : public tagGameEvent
{
	EItemConType	eType;
	INT16			n16Index;
	DWORD			dwTypeID;
    INT64			n64ItemID;
	tagClickItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eType = EICT_Null; n16Index = GT_INVALID; dwTypeID = GT_INVALID;n64ItemID = GT_INVALID;}
};

/** \物品不同容器间移动
*/
struct tagItemPosChangeExEvent : public tagGameEvent
{
	EItemConType	eTypeSrc;
	EItemConType	eTypeDst;
	INT16			n16IndexSrc;
	DWORD			dwTypeIDSrc;

	tagItemPosChangeExEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{eTypeSrc = eTypeDst = EICT_Null; n16IndexSrc = GT_INVALID; dwTypeIDSrc = GT_INVALID;}
};

/** \特殊功能物品使用点选物品为对象，如鉴定卷
*/
struct tagOnTheRoleOfItemEvent : public tagGameEvent
{
	EItemConType		eTypeSrc;
	EItemConType		eTypeDst;
	INT16				n16IndexSrc;
	INT16				n16IndexDst;
	DWORD				dwTypeIDSrc;
	DWORD				dwTypeIDDst;
	EItemSpecFunc		eSpecFuncSrc;

	tagOnTheRoleOfItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		eTypeSrc = eTypeDst = EICT_Null;
		n16IndexSrc = n16IndexDst = GT_INVALID;
		dwTypeIDSrc = dwTypeIDDst = GT_INVALID;
		eSpecFuncSrc = EISF_Null;
	}
};

struct tagSpecItemEvent : public tagGameEvent
{
	INT64				n64Serial;
	EItemSpecFunc		eSpecFuncSrc;

	tagSpecItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		n64Serial    = GT_INVALID;
		eSpecFuncSrc = EISF_Null;
	}
};


/** \丢弃物品
*/
struct tagPutDownItemEvent : public tagGameEvent
{
	EDragType	eType;
	DWORD		dwParam;
	DWORD		dwTypeID;
	
	tagPutDownItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \购买物品
*/
struct tagBuyItemEvent : public tagGameEvent
{
	INT16			n16Pos;
	INT16			n16Num;
	DWORD			dwTypeID;

	tagBuyItemEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

/** \增加或删除物品log
*/
struct tagCombatMsgForItemChange : public tagGameEvent
{
	DWORD			dwTypeID;	// 物品原型ID
	INT16			n16Num;		// 物品数量
	INT16			nQuality;	// 物品品级
	

	tagCombatMsgForItemChange(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

/** \扩充行囊或仓库
*/
struct tagItemContainerExtend : public tagGameEvent
{
	bool bUseSilver;	// 是否消耗金钱，是：消耗金钱，否：消耗元宝
	bool bWalkWare;		// true 随身仓库，false 正常的，通过NPC访问仓库
	tagItemContainerExtend(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent( szEventName, pSenderFrame ){}
};

/** \装备丢弃向飞升界面发送的消息
*/
struct tagEquipDropEvent: public tagGameEvent
{
	INT64 n64ItemID;

	tagEquipDropEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame){}
};

/** \装备好友界面
*/
struct tagBeautyFriendsEvent: public tagGameEvent
{
	DWORD dwTargetID;
	DWORD dwSuitIndex;
	tagBeautyFriendsEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		:tagGameEvent(szEventName, pSenderFrame){}
};