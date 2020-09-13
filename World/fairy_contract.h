//-----------------------------------------------------------------------------
// Copyright (c) 2004 Dacheng Entertainment All rights reserved.
// filename	:	ScriptMsgInfo.h
// author		:	Jason
// actor			:		
// data			:	2010/08/26
// last			:	
// brief			:	妖精契约
//-----------------------------------------------------------------------------

#ifndef fairy_contract_h__
#define fairy_contract_h__


/**
 * \ingroup World
 *	契约单项
 *
 * \par requirements
 *
 * \version 1.0
 * first version
 *
 * \date 2010-08-27
 *
 * \author Jason
 * 
 */
class Role;
class CContractItem
{
	DWORD		m_dwRoleID;				// 契约相关角色id
	DWORD		m_dwCurItemTypeID;		// 当前契约所需物品
	INT			m_nTicks;					// 当前物品，消耗的tick，每次切换物品时，这个要重置
	INT			m_nScore;					// 得分情况
	DWORD		m_dwUserSelected;			// 用户选择的
	BOOL		m_bSelected;				// 在用户选择时，置TRUE，在update后，置FALSE
	BOOL		m_bStart;					// 玩家参与契约后，第一次update时，根据这个来发消息给客户端
	INT			m_nSkillLevel;				// 签订契约的技能等级
	INT			m_nContractTimeout;		// (TICK_PER_SECOND * 3)或者(TICK_PER_SECOND * 2)
	Role		*	m_pRole;
	BOOL		m_bDiscarded;				// 是否废弃
	INT			m_nTotalTimeout;			// 总计时器，在10分钟后，没有反应，就宣布改玩家契约被丢弃
	INT			m_nClientUndoTimeout;		// 多久没有收到客户端倒计时了
	INT			m_nFullScore;				// 总分
public:
	CContractItem();
	CContractItem(const CContractItem & );
	CContractItem & operator = (const CContractItem &);

	DWORD			GetRoleID( void ) const{return m_dwRoleID;}
	void			SetRoleID( DWORD dwRoleID ){m_dwRoleID = dwRoleID;}

	DWORD			GetCurItemID( void ) const { return m_dwCurItemTypeID; }
	
	INT				GetScore(void) const { return m_nScore; }

	void			SetUserSelectedItem( DWORD dwItemID ) ; //{m_dwUserSelected = dwItemID;重置tick时间}

	__inline BOOL	IsDiscarded() { return m_bDiscarded; }
	__inline void	SetDiscarded(BOOL b) { m_bDiscarded = b;}

	// 当用户选择了放弃后，因为client没有发消息给服务器
	// 当他再次打开时，管理器调用这个方法，给他原来的数据库清零
	void			Reset( DWORD dwRoleID );

	//void			Start(void);
	// 获取契约内容
	INT			GetContractContent(INT & nMaxV,DWORD &dwCurItemID,INT & num ,DWORD * pdwItemIDs,INT size);

	void			SetSkillLevel( INT level ){ m_nSkillLevel = level; }

	INT				GetTotalScore(void) const {return m_nFullScore;}
	void			SetTotalScore(INT n) {m_nFullScore = n;}

	
	/**
	 * 每tick更新
	 * \param void 
	 * \return 当用户选择了契约物品后，update会判断选择或者超时，做加减分处理，并通知相关用户；当一个用户完成契约后，返回TRUE（管理器的update就返回，并全副广播），否则返回FALSE。
	 */
	BOOL			Update(void) ;	// 每个tick需要更新的
private:
	BOOL			IsCompleted() const ;
	// 从ItemsOfFairyContract(itemdefine.h中定义)随机一个物品，填充m_dwCurItemTypeID，并返回之
	DWORD			RandomItemID( void );	 
	static void		_copy(const CContractItem & src,CContractItem&des);
	void			RecalTimeoutValue(void);
	void			_update_timeout(void);
	BOOL			_update_select(BOOL Timeout=FALSE);
	INT			_mod_score(INT );
};

class MapFairyContract;
/**
* \ingroup World
* 契约单，与每个妖精精魄，签订契约的清单
* \version 1.0
* first version
*
* \date 2010-08-30
*
* \author Jason
*
*/
class FairyContract
{
	MapFairyContract * m_pOwner;
	DWORD		m_dwFairyID;	// 妖精灵魂id
	BOOL		m_bCompleted;
	BOOL		m_bStarted;
	map<DWORD,CContractItem*>	m_mappItem;	// <角色id，单项>
	DWORD		m_dwWinner;	// 赢家
public:
	CContractItem * GetItem( DWORD dwRoleID )
	{
		map<DWORD,CContractItem*>::iterator i = m_mappItem.find(dwRoleID);
		if( i != m_mappItem.end() )
			return i->second;
		return NULL;
	}
	void AddItem( DWORD dwRoleID, CContractItem * pItem )
	{
		m_mappItem[dwRoleID] = pItem ;
	}

	void Start(void);

	void Update(void);

	BOOL IsStarted(void) {return m_bStarted;}

	BOOL IsCompleted() const {return m_bCompleted;}

	// 因为npc死亡等原因，而中断所有契约人
	void Terminate(BOOL bDelCreature = FALSE);

	DWORD	GetWinner(void){ return m_dwWinner; }

	FairyContract(MapFairyContract *p,DWORD dwFairyID);

	~FairyContract();

private:
	// 完成契约时调用
	void OnComplete(void);
	// 销毁契约时用，这个我是在析构时调用
	void OnDestroy(void);
};


/**
* \ingroup World
* 每个地图相关的妖精契约
* \version 1.0
* first version
*
* \date 2010-08-30
*
* \author Jason
*	销毁契约时，调用了deletecreature，由于这个函数需要在处理复活时才真正删除creature，因此，需要一个机制来在析构map时，再次清理复活列表中残存的creature
*/
class MapFairyContract
{
	DWORD	m_dwMapID;	// 地图id
	DWORD	m_dwInstanceID;	// 副本id
	map<DWORD,FairyContract* > m_mapFairyContract; // <妖精灵魂id，契约单>
public:
	MapFairyContract(DWORD dwMapID);

	~MapFairyContract();
	
	DWORD GetMapID(void) const {return m_dwMapID;}
	// 增加副本id接口
	DWORD MapInstanceID(void)const {return m_dwInstanceID;}
	void MapInstanceID(DWORD dwInstID){m_dwInstanceID = dwInstID;}

	// 得到某个妖精的契约单
	FairyContract * GetContract( DWORD dwFairyID );
	// 增加一个契约单，在玩家使用调查（契约open）技能时，调用
	FairyContract* AddContract( DWORD dwFairyID );

	// 由于契约存在时限（通过时限npc控制），该函数可以在npc消失时，结束契约活动
	void Terminate(DWORD dwFairyID,BOOL bDelCreature = FALSE);
	// 这个方法，只是在需要时，清除所有契约，不过，目前只想在析构中调用
	void Clear(void);

	// 每个tick更新，判断妖精契约完成情况，并适时通知玩家
	void Update(void);

	BOOL IsOver(void);	// 是否全部结束了

	CContractItem * GetContractItem( DWORD dwRoleID );
};


#endif // fairy_contract_h__





