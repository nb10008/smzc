//-----------------------------------------------------------------------------
//!\file title_mgr.h
//!\author xlguo
//!
//!\date 2009-01-18
//! last 2009-02-02
//!
//!\brief 称号消息处理过程
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

// project
#include "..\ServerDefine\title_define.h"
#include "..\WorldDefine\msg_role_title.h"
#include "att_res.h"

// stl
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>

using std::bitset;
using std::vector;
using std::list;
using std::find;

// Jason 2010-3-30 v1.5.1
INT GetMaxTitleNum(void);
#define MAX_TITLE_NUM_EX	GetMaxTitleNum()
bool TITLEID_VALID_EX( INT id );
VOID SetMaxTitleNum(INT v);
// 下面这个不能替代强大的bitset，目前只能部分实现类似bitset的功能，不支持赋值等操作
//template <class T, class Con = vector<T> >
class TBitSet : public vector<bool>
{
	int _size;
	bool _validate(int ndx)
	{
		return ndx >= 0 && ndx < _size ;
	}
public:
	TBitSet(INT ncon = MAX_TITLE_NUM)
		:vector<bool>(ncon),_size(ncon)
	{
		reset();
	}
	void arrage(int size)
	{
		resize(size);
		reset();
		_size = size;
	}
	void reset(void)
	{
		for(vector<bool>::iterator i = begin(); i != end() ; ++i)
			*i = false;
	}
	void reset(int ndx)
	{
		if( _validate(ndx) )
			(*this)[ndx] = false;
	}
	void set(int ndx)
	{
		if( _validate(ndx) )
			(*this)[ndx] = true;
	}
	bool test(int ndx)
	{
		if( _validate(ndx) )
			return (*this)[ndx];
		return false;
	}
	int count()
	{
		int r = 0;
		for(vector<bool>::iterator i = begin(); i != end() ; ++i)
			if( *i )
				++r;
		return r;
	}
};



class TitleMgr
{
	typedef list<UINT16>					ListUINT16;
	typedef vector<UINT16>					VecUINT16;
	// Jason 2010-3-30
	//typedef bitset<MAX_TITLE_NUM>			BitSet;
	//typedef vector<bool> BitSet;
	typedef TBitSet BitSet;

public:
	~TitleMgr(){	Destroy();	}

	// 1初始化选项
	void InitOpts(Role* pRole, UINT16 u16ActTitleID, BOOL bVisibility);

	// 2初始化称号数据
	void InitTitles(const BYTE* &pData, const INT32 nNum);

	// 销毁数据
	void Destroy();

	// 更新，若为时限制称号，则更新剩余时间
	void Update();

	void Resize(int size)
	{
		m_bitsetObtainedMark.arrage(size);
		m_bitsetDBInserted.arrage(size);
		m_bitsetNeedSaveDB.arrage(size);
		m_ArrPtrCondition.resize(size);
	}

public:
	// 存储到数据库
	void SaveTitlesToDB(IN LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	// 发信号
	bool SigEvent(EnumTitleEvent titleEvent, DWORD dwArg1, DWORD dwArg2);

	// 设置称号可见性
	DWORD SetVisibility(bool bVisibility)	{	m_bVisibility = bVisibility;	return E_Title_Success;	}

	// 返回可见性
	bool Visibility() const { return TRUE == m_bVisibility; }

	// 激活称号
	DWORD ActiviteTitle(const UINT16 u16TitleID);
	
	// 当前使用称号
	UINT16 GetActiviteTitle() const {	return m_u16ActiveID;	}

	// 取得所有已获得称号数据
	DWORD GetObtainedTitleIDs(UINT16* &pData, UINT16 &u16Num);

	// 已获得称号数目
	DWORD GetObtainedTitlesNum() {	return m_bitsetObtainedMark.count();}

	//获得剩余时间map
	VOID GetRemainTimeMap( TMap<UINT16,DOUBLE> &mapTimeRemain){ mapTimeRemain = m_mapTimeRemain;}

	// 交还称号数据
	DWORD ReturnTitlesData(LPVOID pData) {	SAFE_DEL(pData);	return E_Title_Success;	}

	//删除称号
	BOOL SigRemoveEvent( EnumTitleEvent titleEvent, DWORD dwArg1 = GT_INVALID);

	// 检查称号是否已经获取
	BOOL IsTitleObtained(UINT16 u16TitleID) { return m_bitsetObtainedMark.test(u16TitleID); }

private:
	// 称号插入到数据库
	VOID InsertTitle2DB( UINT16 u16TitleID, BOOL bNew );

	// 称号从数据库删除
	VOID RemoveTitleFromDB( UINT16 u16TitleID);

	// 清除关系
	void CleanRelation(const UINT16 u16TitleID);

	// 通知客户端新获得的称号
	void NotifyClient(VecUINT16& vecNewObtTitles);
	// 通知客户端 玩家丢失了称号
	void NotifyClientDelete(VecUINT16& vecDelTitleIDs);
private:
	UINT16								m_u16ActiveID;						// 当前称号
	BOOL								m_bVisibility;						// 可见性

	ListUINT16							m_EventMap[ETE_MAX_EVENTNUM];		// 事件映射
	// Jason 2010-3-30
	//Condition*							m_ArrPtrCondition[MAX_TITLE_NUM];	// 条件指针数组
	vector<Condition*>						m_ArrPtrCondition;

	BitSet								m_bitsetObtainedMark;				// 称号是否已获得
	BitSet								m_bitsetDBInserted;					// 已插入数据库
	BitSet								m_bitsetNeedSaveDB;					// 需要保存到数据库
	//TMap<UINT16,DWORD>					m_mapTimeRemain;					// ID对应的剩余时间，用于记录限时称号
	TMap<UINT16,DOUBLE>					m_mapTimeRemain;					// ID对应的剩余时间，用于记录限时称号

																			// 以ms为单位

	BOOL								m_bDayHasSent;						//剩余一天的消息已发出
	BOOL								m_bHourHasSent;						//剩余一小时的消息已发出

	Role*								m_pRole;							// 角色指针

};

