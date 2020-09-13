#include "StdAfx.h"
#include "../WorldDefine/ItemDefine.h"
#include "role.h"

#include "fairy_birth.h"
#include "../ServerDefine/mini_tool.h"


// 妖精申请配对内部专用内存池
static vEngine::SafeMemPool __mpool(1*1024*1024); // 

static FairyMatingMgr* __pThis = NULL;
static jm_tool::CSCreatedLock	__cs;
static jm_tool::CSCreatedLock	__cs_map;

tagFairyInviteBirthData * FairyMatingMgr::_make()
{
	DWORD dwSize = sizeof(tagFairyInviteBirthData);
	LPVOID pData = __mpool.Alloc(dwSize);
	if( !P_VALID(pData) )
		return NULL;
	memset(pData,0,dwSize);
	return reinterpret_cast<tagFairyInviteBirthData*>(pData);
}

void	FairyMatingMgr::_destroy(tagFairyInviteBirthData * pData)
{
	if( P_VALID(pData) )
		__mpool.Free(pData);
}

FairyMatingMgr::FairyMatingMgr()
	:m_nUpdateCounter(0)
{
}
FairyMatingMgr::~FairyMatingMgr()
{
	ClearInnerData();
}

FairyMatingMgr* FairyMatingMgr::Create()
{
	if( __pThis )
		return __pThis;
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(__cs);
	if( __pThis )
		return __pThis;
	__pThis = new FairyMatingMgr;
	return __pThis;
}
VOID FairyMatingMgr::Destroy()
{
	if( !__pThis )
		return ;
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(__cs);
	if( __pThis )
		delete __pThis;
	__pThis = NULL;
}

tagFairyInviteBirthData * FairyMatingMgr::Find(DWORD dwInvitorID) const
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(__cs_map);
	return _find(dwInvitorID);
}
tagFairyInviteBirthData * FairyMatingMgr::_find(DWORD dwInvitorID,bool _2) const
{
	std::map<DWORD,tagFairyInviteBirthData *>::const_iterator it = m_FairyInviteBirthData.find(dwInvitorID);
	if( it != m_FairyInviteBirthData.end() )
		return it->second;
	if( _2 )
		return NULL;
		
	std::map<DWORD,DWORD>::const_iterator it1 = m_inviteeInvitor.find(dwInvitorID);
	if( it1 != m_inviteeInvitor.end() )
	{
		dwInvitorID = it1->second;
		return _find(dwInvitorID,true);
	}
	return NULL;
}
VOID FairyMatingMgr::Mod(DWORD dwInvitorID,tagFairyInviteBirthData * pData)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(__cs_map);
	_mod(dwInvitorID,pData);
}
bool FairyMatingMgr::_mod(DWORD dwInvitorID,tagFairyInviteBirthData * pData,bool _2)
{
	std::map<DWORD,tagFairyInviteBirthData *>::iterator it = m_FairyInviteBirthData.find(dwInvitorID);
	if( it != m_FairyInviteBirthData.end() )
	{
		//memcpy(it->second,pData,sizeof(tagFairyInviteBirthData));
		it->second->u64InvitorFairyID 	= pData->u64InvitorFairyID;
		it->second->u64InviteeFairyID 	= pData->u64InviteeFairyID;
		it->second->dwTimeout 		= pData->dwTimeout;
		it->second->dwInvitorID 		= pData->dwInvitorID;
		it->second->dwInviteeID 		= pData->dwInviteeID;
		if( P_VALID(pData->dwInviteeID) )
		{
			m_inviteeInvitor[pData->dwInviteeID] = dwInvitorID;
		}
		return true;
	}
	if( _2 )
		return false;
		
	std::map<DWORD,DWORD>::iterator it1 = m_inviteeInvitor.find(dwInvitorID);
	if( it1 != m_inviteeInvitor.end() )
	{
		dwInvitorID = it1->second;
		return _mod(dwInvitorID,pData,true);
	}
	return false;
}
VOID FairyMatingMgr::Remove(DWORD dwInvitorID)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(__cs_map);
	_remove(dwInvitorID);
}
bool FairyMatingMgr::_remove(DWORD dwInvitorID,bool _2)
{
	std::map<DWORD,tagFairyInviteBirthData *>::iterator it = m_FairyInviteBirthData.find(dwInvitorID);
	if( it != m_FairyInviteBirthData.end() )
	{
		tagFairyInviteBirthData * p = it->second;
		m_FairyInviteBirthData.erase(it);
		_destroy(p);
		return true;
	}
	if( _2 )
		return false;
		
	std::map<DWORD,DWORD>::iterator it1 = m_inviteeInvitor.find(dwInvitorID);
	if( it1 != m_inviteeInvitor.end() )
	{
		dwInvitorID = it1->second;
		m_inviteeInvitor.erase(it1);
		return _remove(dwInvitorID,true);
	}
	return false;
}
tagFairyInviteBirthData * FairyMatingMgr::Add(DWORD dwInvitorID)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> g(__cs_map);
	std::map<DWORD,tagFairyInviteBirthData *>::iterator it = m_FairyInviteBirthData.find(dwInvitorID);
	if( it == m_FairyInviteBirthData.end() )
	{
		tagFairyInviteBirthData * p = _make();
		if( P_VALID(p) )
		{
			p->dwInvitorID = dwInvitorID;
			p->ResetTimeout() ;
			m_FairyInviteBirthData[dwInvitorID] = p;
			return p;
		}
		return NULL;
	}
	return it->second;
}
VOID	FairyMatingMgr::Update()
{
#if 0
	tagFairyInviteBirthData * p = NULL;
	std::map<DWORD,tagFairyInviteBirthData *>::iterator it = m_FairyInviteBirthData.begin();
	bool bFirst = true;
	for( ; it != m_FairyInviteBirthData.end();  )
	{
		if( bFirst )
		{
			bFirst = false;
			it += m_nUpdateCounter;
		}
		p = it->second;
		if( P_VALID(p) )
		{
			// 下面的地方需要斟酌，当全部合法时，就不要更新了
			if( !p->Isvalid() && p->IsTimeout() )
			{
				Role * pInvitor = g_roleMgr.GetRolePtrByID(p->dwInvitorID);
				Role * pInvitee = g_roleMgr.GetRolePtrByID(p->dwInviteeID);
				tagNS_FairyMatingCancelAll msg;
				if( P_VALID(pInvitor) )
				{
					pInvitor->SendMessage(&msg,msg.dwSize);
				}
				if( P_VALID(pInvitee) )
				{
					pInvitee->SendMessage(&msg,msg.dwSize);
				}
				std::map<DWORD,tagFairyInviteBirthData *>::iterator it0 = it;
				++it;
				m_FairyInviteBirthData.erase(it0);
				continue;
			}
		}
		++it;
	}
#endif
}

// 获取妖精配对邀请信息，只要该函数返回TRUE，那些指针参数肯定是有效的，否则会返回FALSE，但不会清理掉垃圾数据，由调用者酌情自行删除
BOOL GetFabaoMatingOtherInfo(Role* pRole,Role *& pOther,tagFabao *& pMyFabao , tagFabao *& pHisFabao ,tagFairyInviteBirthData *& pData ,FairyMatingMgr*& pMgr )
{
	pOther		= NULL;
	pMyFabao	= NULL;
	pHisFabao	= NULL;
	pData		= NULL;
	pMgr		= NULL;
	if( !P_VALID(pRole) )
		return FALSE;

	FairyMatingMgr* pMatingMgr = FairyMatingMgr::Create();
	pMgr = pMatingMgr;
	DWORD dwRoleID = pRole->GetID();
	if( P_VALID(pMatingMgr) )
	{
		pData = pMatingMgr->Find(dwRoleID);
		if( P_VALID(pData) )
		{
			tagItem * pMyItem = NULL, * pHisItem = NULL;
			UINT64 u64HisFabaoID = 0;
			if( pData->dwInviteeID == dwRoleID )
			{
				pMyItem = pRole->GetItemMgr().GetItemFromAllContainer(pData->u64InviteeFairyID);
				u64HisFabaoID = pData->u64InvitorFairyID;
				pOther = g_roleMgr.GetRolePtrByID(pData->dwInvitorID);
			}
			else if( pData->dwInvitorID == dwRoleID )
			{
				pMyItem = pRole->GetItemMgr().GetItemFromAllContainer(pData->u64InvitorFairyID);
				u64HisFabaoID = pData->u64InviteeFairyID;
				pOther = g_roleMgr.GetRolePtrByID(pData->dwInviteeID);
			}
			else
			{
				//pMatingMgr->Remove(dwRoleID);
				return FALSE;
			}
			if( !P_VALID(pOther) )
			{
				//pMatingMgr->Remove(dwRoleID);
				return FALSE;
			}

			pHisItem = pOther->GetItemMgr().GetItemFromAllContainer(u64HisFabaoID);

			if( !P_VALID(pMyItem) || !P_VALID(pMyItem->pProtoType) || !MIsFaBao(pMyItem) ||
				!P_VALID(pHisItem) || !P_VALID(pHisItem->pProtoType) || !MIsFaBao(pHisItem) )
			{
				//pMatingMgr->Remove(dwRoleID);
				return FALSE;
			}
			pMyFabao	= (tagFabao*)pMyItem;
			pHisFabao	= (tagFabao*)pHisItem;

			return TRUE;
		}
	}
	if( !P_VALID(pOther) )
	{
		return FALSE;
	}

	return FALSE;
}
static BOOL GetFabaoMatingFromContainer(ItemContainer& con,INT nStatus, tagFabao *& pMyFabao);
BOOL GetFabaoMatingFromRole( Role * pRole, INT nStatus, tagFabao *& pMyFabao )
{
	pMyFabao = NULL;
	if( !P_VALID(pRole) )
		return FALSE;

	//tagFabao * pFabao = NULL;
	//tagEquip * pEquip = pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	//if( MIsFaBao(pEquip) )
	//{
	//	pFabao = (tagFabao*)pEquip;
	//	if( (INT)pFabao->eFabaoStatus == nStatus )
	//	{
	//		pMyFabao = pFabao;
	//		return TRUE;
	//	}
	//}
	if( GetFabaoMatingFromContainer(pRole->GetItemMgr().GetBag(),nStatus,pMyFabao) )
		return TRUE;
	//if( GetFabaoMatingFromContainer(pRole->GetItemMgr().GetRoleWare(),nStatus,pMyFabao) )
	//	return TRUE;
	return FALSE;
}

static BOOL GetFabaoMatingFromContainer(ItemContainer& con,INT nStatus, tagFabao *& pMyFabao)
{
	tagItem * pItem = NULL;
	tagFabao * pFabao = NULL;
	INT nSize = con.GetCurSpaceSize();
	for( INT16 i = 0; i < nSize ; ++i )
	{
		pItem = con.GetItem(i);
		if( MIsFaBao(pItem) )
		{
			pFabao = (tagFabao*)pItem;
			if( (INT)pFabao->eFabaoStatus == nStatus )
			{
				pMyFabao = pFabao;
				return TRUE;
			}
		}
	}
	return FALSE;
}

VOID FairyMatingMgr::ClearInnerData()
{
	tagFairyInviteBirthData * p = NULL;
	for( std::map<DWORD,tagFairyInviteBirthData *>::iterator it = m_FairyInviteBirthData.begin(); it != m_FairyInviteBirthData.end(); ++it )
	{
		p = it->second;
		if( P_VALID(p) )
			_destroy(p);
	}
}
