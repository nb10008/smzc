#include "StdAfx.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_combat.h"

#include "fairy_contract.h"
#include "role.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "MsgInfoMgr.h"
#include "map_creator.h"
#include "creature.h"

// 这个宏，可能会变，所以不放进头文件里面去
#define FAIRY_CONTRACT_FULL_SCORE		150

#define DISCARD_TIMEOUT		( 0 - TICK_PER_SECOND * 600 )

#define BROADCAST_TYPEID	(100105)
#define FAIRY_CONTRACT_BUFF	90015


static const int s_nFairyContractItemCount = sizeof(ItemsOfFairyContract)/sizeof(ItemsOfFairyContract[0]);
//INT CContractItem::m_nContractTimeout = TICK_PER_SECOND * 3;
#define TIMEOUT_CLIENT

/************************************************************************/
/*                         妖精契约项                                   */
/************************************************************************/

INT	CContractItem::_mod_score(INT step)
{
	INT nLastScore = m_nScore;
	m_nScore += step;
	if( m_nScore < 0 )
		m_nScore = 0;
	else if( m_nScore > m_nFullScore )
		m_nScore = m_nFullScore;
	if( nLastScore != m_nScore )
	{
		RecalTimeoutValue();
	}
	return m_nScore;
}

BOOL	CContractItem::IsCompleted() const
{ 
	return m_nScore >= m_nFullScore; 
}

DWORD	CContractItem::RandomItemID( void )
{
	int ndx = IUTIL->RandomInRange(0,s_nFairyContractItemCount-1);
	m_dwCurItemTypeID = ItemsOfFairyContract[ndx];
	return m_dwCurItemTypeID;
}
void				CContractItem::SetUserSelectedItem( DWORD dwItemID )
{
	//if( m_nTicks <= 0  )
	//	return;
	m_bSelected		= TRUE;
	m_nTicks			= m_nContractTimeout;
	m_dwUserSelected	= dwItemID;
}
void				CContractItem::RecalTimeoutValue(void)
{
	if( m_nScore <= m_nFullScore / 2 )
		m_nContractTimeout = TICK_PER_SECOND * 3;
	else
		m_nContractTimeout = TICK_PER_SECOND * 2;
}
void				CContractItem::_update_timeout(void)
{
	m_nTicks = m_nContractTimeout;

	//if( !P_VALID(m_pRole) )	// 这样做，可以防止，中途有客户端退出的情况
	m_pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
//#ifndef TIMEOUT_CLIENT
	// 如果客户端有两个3秒超时，没有给服务器发超时或者选择消息的 话，服务器夺回超时控制权
	if( m_nClientUndoTimeout > 2 )
	{
		_mod_score(-1);
		if( P_VALID( m_pRole ) )
		{
			tagNS_ArrestingFairy msg;
			msg.dwErrorCode			= E_Opt_Timeout;
			msg.bDone					= FALSE;
			msg.dwItemID				= RandomItemID();
			msg.nCurContractValue		= m_nScore;
			m_pRole->SendMessage( &msg, msg.dwSize );
		}
	}
//#endif
	if( !P_VALID( m_pRole ) )
		m_bDiscarded = TRUE;

	if( m_nTotalTimeout <= DISCARD_TIMEOUT )
	{
		m_bDiscarded = TRUE;
	}
}
struct tagFindItemType
{
	bool operator () (const tagItem* pItem,DWORD dwType)
	{
		if(!P_VALID(pItem) || !P_VALID(pItem->pProtoType))
			return false;
		return pItem->pProtoType->dwTypeID == dwType;
	}
};
BOOL				CContractItem::_update_select(BOOL Timeout)
{
	// 这段，不需要每次都给m_pRole复制，因为这时候客户端主动给服务器发了消息
	// 除非client发了消息，就宕机了，这种情况。。。
	if( !P_VALID(m_pRole) )
		m_pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);

	BOOL re = FALSE;
	if( P_VALID( m_pRole ) )
	{
		tagNS_ArrestingFairy msg;
		TList<tagItem*> _lst;
		if( P_VALID(m_dwUserSelected) && SearchPack( m_pRole->GetItemMgr().GetBag(),_lst,m_dwUserSelected, tagFindItemType() )  >= 1 )
		{
			if( m_dwUserSelected == m_dwCurItemTypeID ) // 成功
			{
				_mod_score(2 + m_nSkillLevel);
				msg.dwErrorCode				= E_Success;
			}
			else	// 失败
			{
				_mod_score(-2);
				msg.dwErrorCode				= E_Opt_Fail;
			}

			msg.bDone							= IsCompleted();
			if( !msg.bDone )
				msg.dwItemID					= RandomItemID();
			else
				msg.dwItemID					= GT_INVALID;
			msg.nCurContractValue		= m_nScore;
			
			RemoveFromContainer( m_pRole->GetItemMgr(), m_pRole->GetItemMgr().GetBag(), _lst,1,ELCID_Fairy_Costed );
			re = msg.bDone;
			m_pRole->SendMessage( &msg, msg.dwSize );
		}
		else
		{
			// -1是客户端发送的超时消息
			if( !P_VALID(m_dwUserSelected) )
			{
				_mod_score(-1);
				msg.dwErrorCode			= E_Opt_Timeout;
			}
			else
				msg.dwErrorCode			= E_Item_NotFound;
			msg.bDone					= FALSE;
			msg.dwItemID				= RandomItemID();
			msg.nCurContractValue		= m_nScore;
			m_pRole->SendMessage( &msg, msg.dwSize );
		}
	}	
	else // 角色无效了
		m_bDiscarded = TRUE ;

	return re;
}
BOOL			CContractItem::Update(void)
{
	if( m_bDiscarded )
		return FALSE;

	// 超时处理
	bool bTimeout = false;
	--m_nTotalTimeout ;
	if( m_nTicks-- <= 0  )	// 超时了
	{
		bTimeout = true;
	}
	if( !m_bSelected )
	{
		if( bTimeout )
		{
			++m_nClientUndoTimeout;
			_update_timeout();
		}
		return FALSE;
	}

	m_nClientUndoTimeout = 0;
	BOOL re = _update_select();
	m_bSelected = FALSE;

	m_nTicks = m_nContractTimeout;
	m_nTotalTimeout = 0;

	return re;
}
void CContractItem::_copy(const CContractItem & src,CContractItem&des)
{
	des.m_dwRoleID				= src.m_dwRoleID				;				
	des.m_dwCurItemTypeID		= src.m_dwCurItemTypeID		;
	des.m_nTicks				= src.m_nTicks					;					
	des.m_nScore				= src.m_nScore					;					
	des.m_dwUserSelected		= src.m_dwUserSelected		;	
	des.m_bSelected			= src.m_bSelected				;			
	des.m_bStart				= src.m_bStart					;					
	des.m_nSkillLevel			= src.m_nSkillLevel			;		
	des.m_nContractTimeout	= src.m_nContractTimeout		;
	des.m_pRole				= src.m_pRole					;        
	des.m_bDiscarded			= src.m_bDiscarded				;			
	des.m_nTotalTimeout		= src.m_nTotalTimeout			;	
	des.m_nClientUndoTimeout	= src.m_nClientUndoTimeout	;
}
CContractItem::CContractItem()
:	m_dwRoleID(0)	,	m_dwCurItemTypeID(0)	,	m_nTicks(0)			,
	m_nScore(0)	,	m_dwUserSelected(0)	,	m_bSelected(FALSE)	,
	m_bStart(FALSE),	m_nSkillLevel(0)		,	m_nContractTimeout(0)	,
	m_pRole(NULL)	,	m_bDiscarded(FALSE)	,	m_nTotalTimeout(0)		,
	m_nClientUndoTimeout(0)
{
}
CContractItem::CContractItem(const CContractItem & t)
{
	m_nClientUndoTimeout = 0;
	_copy(t,*this);
}
CContractItem & CContractItem::operator = (const CContractItem &t)
{
	_copy(t,*this);
	return *this;
}
void	CContractItem::Reset( DWORD dwRoleID )
{
	m_dwRoleID				= dwRoleID		;
	m_dwCurItemTypeID		= RandomItemID();
	m_nTicks				= 0		;					
	m_nScore				= 0		;					
	m_dwUserSelected		= 0		;	
	m_bSelected			= FALSE	;			
	m_bStart				= TRUE	;					
	m_nSkillLevel			= 0		;		
	m_pRole					= NULL	;        
	m_bDiscarded			= FALSE	;			
	m_nTotalTimeout		= 0		;	
	RecalTimeoutValue();
}
INT	CContractItem::GetContractContent(INT & nMaxV,DWORD &dwCurItemID,INT & num ,DWORD * pdwItemIDs,INT size)
{
	nMaxV				= m_nFullScore;
	dwCurItemID		= m_dwCurItemTypeID;
	num					= s_nFairyContractItemCount;
	if( size >= num && P_VALID(pdwItemIDs) )
	{
		pdwItemIDs[0]	= ItemsOfFairyContract[0];
		pdwItemIDs[1]	= ItemsOfFairyContract[1];
		pdwItemIDs[2]	= ItemsOfFairyContract[2];
		pdwItemIDs[3]	= ItemsOfFairyContract[3];
		pdwItemIDs[4]	= ItemsOfFairyContract[4];
	}
	return num;
}




/************************************************************************/
/*             妖精契约单                                               */
/************************************************************************/

void FairyContract::Start(void)
{
	m_bStarted = TRUE;
}

static VOID FairyGettedEvent(Role * pPicker,Creature * pLoster,tagItem * pItem)
{
	if( !P_VALID(pPicker) || !P_VALID(pLoster) || !P_VALID(pItem) )
		return;
	if( P_VALID(pItem->pProtoType) && MIsFaBao(pItem) )
	{
		tagEquip * pEquip = (tagEquip*)pItem;
		if( pEquip->equipSpec.byQuality >= EIQ_Orange )
		{
			DWORD dwContent = BROADCAST_TYPEID;
			//TCHAR szRoleName[X_SHORT_NAME] = {0};
			//g_roleMgr.GetRoleNameByID(i->first,szRoleName);
			//szRoleName[X_SHORT_NAME-1] = 0;
			DWORD dwTypeID = pLoster->GetTypeID();

			DWORD dwMsgID = g_MsgInfoMgr.BeginMsgEvent();
			g_MsgInfoMgr.AddMsgEvent(dwMsgID,EMUT_NewBroadcast,&dwContent);
			dwContent = pPicker->GetID();
			g_MsgInfoMgr.AddMsgEvent(dwMsgID,EMUT_RoleID,&dwContent);
			g_MsgInfoMgr.AddMsgEvent(dwMsgID,EMUT_NPCTypeID,&dwTypeID);
			g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgID);
		}
	}
}

void FairyContract::Update(void)
{
	if( !m_bStarted )
		return ;

	if( m_bCompleted )
		return;

	for( map<DWORD,CContractItem*>::iterator i = m_mappItem.begin(); i != m_mappItem.end(); )
	{
		if( !P_VALID(i->second) )
			continue;

		if( i->second->Update() ) // 如果更新完毕了
		{
			m_bCompleted = TRUE;
			m_dwWinner = i->first;
			Role * pRole = g_roleMgr.GetRolePtrByID(i->first);
			if( !P_VALID(pRole) )
			{
				m_bCompleted = FALSE;
				continue;
			}
			Map * pMap = pRole->GetMap();
			if( !P_VALID(pMap) )
			{
				m_bCompleted = FALSE;
				continue;
			}
			
			Creature * pCreature = pMap->FindCreature(m_dwFairyID);
			if( !P_VALID(pCreature) )	// 可能怪物死掉了，什么原因不清楚，这时候，应该break，让契约被删除
			{
				break;
			}

			break;
		}
		// 因为超时等原因，该玩家已经放弃契约
		if( i->second->IsDiscarded() )
		{
			map<DWORD,CContractItem*>::iterator lst = i;
			++i;
			DWORD dwRoleID = lst->first;
			Role * pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
			if( P_VALID(pRole) )
			{
				pRole->CancelBuff(FAIRY_CONTRACT_BUFF);
			}
			if( P_VALID(lst->second) )
			{
				delete lst->second;
			}
			m_mappItem.erase(lst);
			continue;
		}

		++i;
	}
	// 结束了，做清理工作
	if( m_bCompleted )
	{
		OnComplete();
	}
}

void FairyContract::OnComplete(void)
{
	DWORD dwRoleID = 0;
	Role * pRole = NULL,*pWinner = NULL;
	tagNS_ArrestingFairy msg;
	msg.bDone = TRUE;
	msg.dwErrorCode = E_UseItem_ItemNotExist;
	msg.dwItemID = GT_INVALID;
	msg.nCurContractValue = 0;
	for( map<DWORD,CContractItem*>::iterator i = m_mappItem.begin(); i != m_mappItem.end(); ++i )
	{
		dwRoleID = i->first;
		pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if( m_dwWinner == dwRoleID)
			pWinner = pRole;

		if( P_VALID(pRole) )
		{
			pRole->CancelBuff(FAIRY_CONTRACT_BUFF);
			if( pRole != pWinner )
				pRole->SendMessage(&msg,msg.dwSize);
		}
		if( P_VALID(i->second) )
			delete i->second;
	}
	m_mappItem.clear();
	// 移除npc（妖精灵魂）
	if( P_VALID(m_pOwner) )
	{
		// 增加对副本的支持
		//DWORD dwMapID = m_pOwner->GetMapID();
		Map * pMap = NULL;//g_mapCreator.GetMap(dwMapID,0);
		if( P_VALID(pWinner) )
			pMap = pWinner->GetMap();
		else
			pMap = g_mapCreator.GetMap(m_pOwner->GetMapID(),m_pOwner->MapInstanceID());
		if( P_VALID(pMap) )
		{
			if( P_VALID(pWinner) )
			{
				Creature * pCreature = pMap->FindCreature(m_dwFairyID);
				if( P_VALID(pCreature) )
				{
					INT nMaxHp = g_attRes.GetAttMax(ERA_MaxHP) + 1; // 满血也可以被杀了，hh
					// 以防万一，ch，没有填击杀所属，通过这个也可以设置所属了
					if( pCreature->GetTaggedOwner() != m_dwWinner )
						pCreature->SetTagged(m_dwWinner);
					pCreature->SetTaggedOnly(TRUE);
					pCreature->SetDeadLootItemFunc(FairyGettedEvent);
					pCreature->ChangeHP(-nMaxHp,pWinner);
					pCreature->SetDeadLootItemFunc();
				}
			}
			else
				pMap->DeleteCreature(m_dwFairyID);
		}
	}
	m_dwWinner = GT_INVALID;
}

void FairyContract::OnDestroy(void)
{
	for( map<DWORD,CContractItem*>::iterator i = m_mappItem.begin(); i != m_mappItem.end(); ++i )
	{
		if( P_VALID(i->second) )
			delete i->second;
	}
	m_mappItem.clear();
	if( P_VALID(m_pOwner) )
	{
		DWORD dwMapID = m_pOwner->GetMapID();
		Map * pMap = g_mapCreator.GetMap(dwMapID,0);
		if( P_VALID(pMap) )
		{
			pMap->DeleteCreature(m_dwFairyID);
		}
	}
}
void FairyContract::Terminate(BOOL bDelCreature)
{
	DWORD dwRoleID = 0;
	Role * pRole = NULL;
	m_dwWinner = GT_INVALID;
	for( map<DWORD,CContractItem*>::iterator i = m_mappItem.begin(); i != m_mappItem.end(); ++i )
	{
		dwRoleID = i->first;
		pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if( P_VALID(pRole) )
		{
			pRole->CancelBuff(FAIRY_CONTRACT_BUFF);
		}
		if( P_VALID(i->second) )
			delete i->second;
	}
	m_mappItem.clear();
	// 移除npc（妖精灵魂）
	if( P_VALID(m_pOwner) && bDelCreature )
	{
		DWORD dwMapID = m_pOwner->GetMapID();
		Map * pMap = g_mapCreator.GetMap(dwMapID,0);
		if( P_VALID(pMap) )
		{
			pMap->DeleteCreature(m_dwFairyID);
		}
	}
}
FairyContract::FairyContract(MapFairyContract *p,DWORD dwFairyID)
: m_dwFairyID(dwFairyID),m_bCompleted(FALSE),m_pOwner(p),m_bStarted(FALSE),m_dwWinner(GT_INVALID)
{
}
FairyContract::~FairyContract()
{
	OnDestroy();
}





/************************************************************************/
/*                  妖精契约，地图相关                                  */
/************************************************************************/

void MapFairyContract::Update(void)
{
	FairyContract * pFC = NULL;
	for( map<DWORD,FairyContract* >::iterator i = m_mapFairyContract.begin(); i != m_mapFairyContract.end(); )
	{
		pFC = i->second;
		if( P_VALID(pFC) )
		{
			pFC->Update();
			if( pFC->IsCompleted() )
			{
				map<DWORD,FairyContract* >::iterator lst = i;
				++i;
				delete pFC;
				m_mapFairyContract.erase(lst);
				continue;
			}
		}
		++i;
	}
}
void MapFairyContract::Terminate(DWORD dwFairyID,BOOL bDelCreature)
{
	map<DWORD,FairyContract* >::iterator i = m_mapFairyContract.find(dwFairyID);
	if( i != m_mapFairyContract.end() )
	{
		if( P_VALID(i->second) )
			i->second->Terminate(bDelCreature);
	}
}
void MapFairyContract::Clear(void)
{
	for( map<DWORD,FairyContract* >::iterator i = m_mapFairyContract.begin(); i != m_mapFairyContract.end(); ++i )
	{
		if( P_VALID(i->second) )
		{
			i->second->Terminate(TRUE);
			delete i->second;
		}
	}
	m_mapFairyContract.clear();
}

FairyContract* MapFairyContract::AddContract( DWORD dwFairyID )
{
	FairyContract * p = GetContract(dwFairyID);
	if( !P_VALID(p) )
	{
		p = new FairyContract(this,dwFairyID);
		m_mapFairyContract[dwFairyID] = p;
		p->Start();
	}
	return p;
}
MapFairyContract::~MapFairyContract()
{
	Clear();
}

MapFairyContract::MapFairyContract(DWORD dwMapID)
: m_dwMapID(dwMapID)
{
}
FairyContract * MapFairyContract::GetContract( DWORD dwFairyID )
{
	map<DWORD,FairyContract* >::iterator i = m_mapFairyContract.find(dwFairyID);
	if( i != m_mapFairyContract.end() )
		return i->second;
	return NULL;
}

BOOL MapFairyContract::IsOver(void)
{
	for( map<DWORD,FairyContract* >::iterator i = m_mapFairyContract.begin(); i != m_mapFairyContract.end(); ++i )
	{
		if( P_VALID(i->second) )
		{
			if(i->second->IsCompleted() == FALSE)
				return FALSE;
		}
	}
	return TRUE;
}

CContractItem * MapFairyContract::GetContractItem( DWORD dwRoleID )
{
	CContractItem * re = NULL;
	for( map<DWORD,FairyContract* >::iterator i = m_mapFairyContract.begin(); i != m_mapFairyContract.end(); ++i )
	{
		if( !P_VALID(i->second) )
			continue;
		re = i->second->GetItem(dwRoleID);
		if( P_VALID(re) )
			break;
	}
	return re;
}

#undef FAIRY_CONTRACT_FULL_SCORE
#undef DISCARD_TIMEOUT
#undef BROADCAST_TYPEID
#undef FAIRY_CONTRACT_BUFF
#ifdef TIMEOUT_CLIENT
#undef TIMEOUT_CLIENT
#endif



