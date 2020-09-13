/********************************************************************
	created:	2010/12/07
	created:	7:12:2010   14:44
	filename: 	e:\F-Project\src\World\fairy_birth.h
	file path:	e:\F-Project\src\World
	file base:	fairy_birth
	file ext:	h
	author:		Jason
	
	purpose:	妖精繁殖申请相关管理器
*********************************************************************/

#ifndef fairy_birth_h__
#define fairy_birth_h__


struct tagFairyInviteBirthData
{
	DWORD	dwInvitorID;		// 邀请人ID
	DWORD	dwInviteeID;		// 被邀请人ID
	UINT64	u64InvitorFairyID;	// 邀请人妖精
	UINT64	u64InviteeFairyID;	// 被邀请人妖精
	DWORD	dwTimeout;
	BOOL Isvalid()
	{
		return P_VALID(dwInvitorID) && P_VALID(dwInviteeID) && u64InvitorFairyID > 0 && u64InviteeFairyID > 0;
	}
	BOOL IsTimeout()
	{
		return dwTimeout < 1 ;
	}
	VOID ResetTimeout()
	{
		dwTimeout = TICK_PER_SECOND * 30;
	}
};

class FairyMatingMgr
{
	std::map<DWORD,tagFairyInviteBirthData *>	m_FairyInviteBirthData;	// 邀请配对信息
	std::map<DWORD,DWORD>	m_inviteeInvitor;	// 被邀请人 配对信息

	static tagFairyInviteBirthData * _make();
	static void	_destroy(tagFairyInviteBirthData * pData);

	int		m_nUpdateCounter;

	FairyMatingMgr();
	~FairyMatingMgr();

	VOID ClearInnerData();
	bool _mod(DWORD dwInvitorID,tagFairyInviteBirthData * pData,bool _2=false);
	bool _remove(DWORD dwInvitorID,bool _2=false);
	tagFairyInviteBirthData * _find(DWORD dwInvitorID,bool _2=false) const;
public:
	static FairyMatingMgr* Create();
	static VOID Destroy();

	// 下面这4个方法应该在地图线程调用
	tagFairyInviteBirthData * Find(DWORD dwInvitorID) const;
	VOID Mod(DWORD dwInvitorID,tagFairyInviteBirthData * pData);
	VOID Remove(DWORD dwInvitorID);
	tagFairyInviteBirthData * Add(DWORD dwInvitorID);
	// 超时更新，通知。。。
	// 必须在主控线程中调用
	VOID	Update();
};
class Role;
struct tagFabao;
// 获取妖精配对邀请信息，只要该函数返回TRUE，那些指针参数肯定是有效的，否则会返回FALSE，但不会清理掉垃圾数据，由调用者酌情自行删除
extern BOOL GetFabaoMatingOtherInfo(Role* pRole,Role *& pOther,tagFabao *& pMyFabao , tagFabao *& pHisFabao ,tagFairyInviteBirthData *& pData ,FairyMatingMgr*& pMgr );
// 获取某个角色的待生育妖精
extern BOOL GetFabaoMatingFromRole( Role * pRole, INT nStatus, tagFabao *& pMyFabao );


#endif // fairy_birth_h__




