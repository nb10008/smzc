//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: pet_soul.cpp
// author: xlguo
// actor:
// data: 2009-07-13
// last:
// brief: 宠物灵魂
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "pet_soul.h"

#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_pet.h"
#include "../ServerDefine/pet_skill_define.h"
#include "../ServerDefine/msg_log.h"

#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/msg_pet_skill.h"
#include "../WorldDefine/filter.h"

#include "pet_define.h"
#include "role_mgr.h"
#include "db_session.h"
#include "pet_skill.h"
#include "role.h"
#include "pet_tracker.h"
#include "pet.h"
#include "pet_pocket.h"

//----------------------------------------------------------------------------------------------------
// 构造函数
//----------------------------------------------------------------------------------------------------
PetSoul::PetSoul()
	:m_PetAtt(this), m_EquipBar(this)
{
	nCount			= GT_INVALID;
	m_dwWorkingSkillTypeID = GT_INVALID;
	m_pMaster		= NULL;
	m_pBody			= NULL;
	m_pPassenger	= NULL;
}

//----------------------------------------------------------------------------------------------------
// 析构函数
//----------------------------------------------------------------------------------------------------
PetSoul::~PetSoul()
{
	m_pPassenger	= NULL;
	m_pMaster		= NULL;	
	PetSkill* pPetSkill = NULL;
	PetSkillMap::TMapIterator itor = m_mapPetSkill.Begin();
	while(m_mapPetSkill.PeekNext(itor, pPetSkill))
		PetSkill::DeletePetSkill(pPetSkill);
	m_mapPetSkill.Clear();
	if(P_VALID(m_pBody))
	{
		Pet::Delete(m_pBody);
		m_pBody			= NULL;
	}
}

//----------------------------------------------------------------------------------------------------
// 数据库创建宠物灵魂
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::CreateDBSoul( DWORD dwTypeID,LPCTSTR tszName,Role* pMaster, INT nQualiry, INT64 n64EggSerial, BOOL bLive)
{
	tagNDBC_CreatePetSoul send;

	if (!PetAtt::InitCreateAtt(&send.createData, dwTypeID, pMaster->GetID(), tszName, nQualiry, bLive))
	{
		return FALSE;
	}
	send.n64Serial = n64EggSerial;
	g_dbSession.Send(&send, send.dwSize);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// tbd:删除宠物灵魂
//----------------------------------------------------------------------------------------------------
VOID PetSoul::DeleteSoul( PetSoul* pSoul, BOOL bFromDB /*= FALSE*/ )
{
	if (bFromDB)
	{
		// 从数据库删除
		tagNDBC_DelPetSoul send;
		send.nPetSoulNum = 1;
		send.dwPetID[0] = pSoul->GetID();
		g_dbSession.Send(&send, send.dwSize);
	}

	SAFE_DEL(pSoul);
}

//----------------------------------------------------------------------------------------------------
// 获取技能
//----------------------------------------------------------------------------------------------------
PetSkill* PetSoul::GetSkill( DWORD dwPetSkillID )
{
	return m_mapPetSkill.Peek(dwPetSkillID);
}

//----------------------------------------------------------------------------------------------------
// 技能书学习技能
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::LearnBookSkill( INT64 n64ItemID )
{
	// 宠物状态是否满足
	if (!IsCalled())
	{
		return E_Pets_Pet_NotCalled;
	}

	if (IsMounted() || IsWorking())
	{
		return E_Pets_PetSkill_Learn_StateLimit;
	}

	// 技能书是否满足
	tagItem *pItem = m_pMaster->GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_PetSkillBook)
	{
		return E_Pets_PetSkill_Learn_ItemInvalid;
	}

	// 技能原型
	DWORD dwPetSkillTypeID	= pItem->pProtoType->nSpecFuncVal1;
	DWORD dwPetSkillID		= MTransPetSkillID(dwPetSkillTypeID);
	DWORD dwPetSkillLvl		= MTransPetSkillLevel(dwPetSkillTypeID);
	const tagPetSkillProto* pSkillProto = g_attRes.GetPetSkillProto(dwPetSkillTypeID);
	if (!P_VALID(pSkillProto))
	{
		return E_Pets_PetSkill_InvalidSkillID;
	}

	// 技能书等级
	if (pSkillProto->nPetLvlLim > GetProto()->nRoleLvlLim)
	{
		return E_Pets_PetSkill_Learn_PetLvlLimit;
	}

	if (GetPetAtt().GetStep() < pSkillProto->nLearn_step|| GetPetAtt().GetGrade() < pSkillProto->nLearn_grade)
	{
		return E_Pets_PetSkill_Learn_PetLvlLimit;
	}

	// 技能等级是否满足
	PetSkill* pSkill = GetSkill(dwPetSkillID);
	if (P_VALID(pSkill))
	{
		DWORD dwExistPetSkillID		= MTransPetSkillID(pSkill->GetSkillTypeID());
		DWORD dwExistPetSkillLvl	= MTransPetSkillLevel(pSkill->GetSkillTypeID());

		if (dwExistPetSkillLvl >= dwPetSkillLvl)
		{
			return E_Pets_PetSkill_Learn_HasHighLvlSkill;
		}
	}

	// 潜能值不够
	if (GetPetAtt().GetAttVal(EPA_Potential) < pSkillProto->nLearn_PontentialCost)
	{
		return E_Pets_PetSkill_Learn_PotentialNotEnough;
	}

	// 学习宠物技能
	DWORD dwErrorCode = LearnSkill(dwPetSkillTypeID);
	if (E_Pets_Success == dwErrorCode)
	{
		// 扣除物品
		dwErrorCode = m_pMaster->GetItemMgr().ItemUsedFromBag(n64ItemID, (INT16)1, (DWORD)ELCLD_PET_ITEM_PET_LEARNSKILL);

		// 扣除潜力值
		GetPetAtt().ModAttVal(EPA_Potential, -pSkillProto->nLearn_PontentialCost);
	}

	return dwErrorCode;
}

//----------------------------------------------------------------------------------------------------
// 升级学习技能
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::LearnNormalSkill( INT nLevel )
{
	if (nLevel < 0 || nLevel > 90)
	{
		ASSERT(0);
		return GT_INVALID;
	}

	// 宠物状态是否满足
	if (IsMounted() || IsWorking())
	{
		return E_Pets_PetSkill_Learn_StateLimit;
	}

	BOOL bHasNew = FALSE;
	const std::list<DWORD> &normalSkills = g_attRes.GetPetNormalSkillList(nLevel);
	
	for (	std::list<DWORD>::const_iterator itr = normalSkills.begin(); 
			itr != normalSkills.end(); 
			++itr)
	{
		DWORD dwSkillTypeID = *itr;

		const tagPetSkillProto* pSkillProto = g_attRes.GetPetSkillProto(dwSkillTypeID);

		if (!P_VALID(pSkillProto) 
			||!pSkillProto->bLearn_condition 
			|| GetPetAtt().GetGrade() != pSkillProto->nLearn_grade
			|| GetPetAtt().GetStep() != pSkillProto->nLearn_step
			|| pSkillProto->nPetLvlLim != GetProto()->nRoleLvlLim 
			|| !IUTIL->Probability(pSkillProto->nLearn_prob/100))
			continue;

		// 学习宠物技能
		if(E_Success == LearnSkill(dwSkillTypeID))
			bHasNew = TRUE;
		
	}
		
	return bHasNew ? E_Success : GT_INVALID;
}

//----------------------------------------------------------------------------------------------------
// 学习一个技能
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::AddSkill( PetSkill* pSkill )
{
	DWORD dwPetSkillTypeID	= pSkill->GetSkillTypeID();
	DWORD dwPetSkillID		= MTransPetSkillID(dwPetSkillTypeID);
 	DWORD dwPetSkillLvl		= MTransPetSkillLevel(dwPetSkillTypeID);

	// 添加映射
	if (GT_VALID(m_mapPetSkill.Peek(dwPetSkillID)))
	{
		return GT_INVALID;
	}
	m_mapPetSkill.Add(dwPetSkillID, pSkill);

	if (P_VALID(GetMaster()) && IsCalled())
	{
		PassiveSkill *pPassiveSkill = dynamic_cast<PassiveSkill *>(pSkill);
		if (P_VALID(pPassiveSkill))
		{
			pPassiveSkill->Active();
		}
	}

	EnhanceSkill *pEnhanceSkill = dynamic_cast<EnhanceSkill *>(pSkill);
	if (P_VALID(pEnhanceSkill))
	{
		pEnhanceSkill->Open();
	}

	return E_Pets_Success;
}

DWORD PetSoul::AddSkillOnLoad( PetSkill* pSkill )
{
	DWORD dwPetSkillTypeID		= pSkill->GetSkillTypeID();
	DWORD dwPetSkillID	= MTransPetSkillID(dwPetSkillTypeID);
	DWORD dwPetSkillLvl		= MTransPetSkillLevel(dwPetSkillTypeID);

	// 添加映射
	if (GT_VALID(m_mapPetSkill.Peek(dwPetSkillID)))
	{
		return GT_INVALID;
	}
	m_mapPetSkill.Add(dwPetSkillID, pSkill);

	EnhanceSkill *pEnhanceSkill = dynamic_cast<EnhanceSkill *>(pSkill);
	if (P_VALID(pEnhanceSkill))
	{
		pEnhanceSkill->Open();
	}

	return E_Pets_Success;
}


//----------------------------------------------------------------------------------------------------
// 遗忘技能
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::ForgetSkill( DWORD dwPetSkillID )
{
	if(IsMounted() || IsWorking())
	{
		ASSERT(0);
		return GT_INVALID;
	}

	PetSkill* pSkill = m_mapPetSkill.Peek(dwPetSkillID);
	if (!P_VALID(pSkill))
	{
		return GT_INVALID;
	}

	DWORD dwPetSkillTypeID = pSkill->GetSkillTypeID();
	DWORD dwRtv = RemoveSkill(pSkill);
	if (E_Pets_Success != dwRtv)
	{
		ASSERT(0);
		return dwRtv;
	}

	PetSkill::DeletePetSkill(pSkill);

	if(P_VALID(GetMaster()))
	{
		tagNS_RemovePetSkill sendNS;
		sendNS.dwPetID			= GetID();
		sendNS.dwSkillID		= dwPetSkillTypeID;
		GetMaster()->SendMessage(&sendNS, sendNS.dwSize);
	}
	tagNDBC_DelPetSkill send;
	send.dwPetID			= GetID();
	send.dwPetSkillTypeID	= dwPetSkillTypeID;
	g_dbSession.Send(&send, send.dwSize);
	
	return E_Pets_Success;
}

//----------------------------------------------------------------------------------------------------
// 更新所有技能
//----------------------------------------------------------------------------------------------------
VOID PetSoul::UpdateAllSkill()
{
	PetSkill* pPetSkill = NULL;
	PetSkillMap::TMapIterator itr = m_mapPetSkill.Begin();
	//for all PetSkill 
	while (	m_mapPetSkill.PeekNext(itr, pPetSkill))
	{
		//Update
		pPetSkill->Update();
	}
}

//----------------------------------------------------------------------------------------------------
// 将所有技能设置为不可工作状态
//----------------------------------------------------------------------------------------------------
VOID PetSoul::DisableAllSkill()
{
	PetSkill* pPetSkill = NULL;
	PetSkillMap::TMapIterator itr = m_mapPetSkill.Begin();
	//for all PetSkill 
	while (	m_mapPetSkill.PeekNext(itr, pPetSkill))
	{
		//Update
		pPetSkill->SetWorking(FALSE);
	}
}

//----------------------------------------------------------------------------------------------------
// 将所有被禁用技能设置为可工作状态
//----------------------------------------------------------------------------------------------------
VOID PetSoul::EnableAllSkill()
{
	PetSkill* pPetSkill = NULL;
	PetSkillMap::TMapIterator itr = m_mapPetSkill.Begin();
	//for all PetSkill 
	while (	m_mapPetSkill.PeekNext(itr, pPetSkill))
	{
		//Update
		pPetSkill->SetWorking(TRUE);
	}
}

//----------------------------------------------------------------------------------------------------
// 获得id
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::GetID()const
{
	return	m_PetAtt.GetID();
}

//----------------------------------------------------------------------------------------------------
// 获得原型id
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::GetProtoID() const
{
	return m_PetAtt.GetProtoID();
}

//----------------------------------------------------------------------------------------------------
// 更新
//----------------------------------------------------------------------------------------------------
VOID PetSoul::Update()
{
	const tagPetProto* pTmpProto = GetProto();
	if (!P_VALID(pTmpProto))
	{
		return;
	}

	// 检查是否有状态矛盾
	if ( pTmpProto->dwLifeTime != GT_INVALID && !m_PetAtt.GetLiveState())
	{
		if( IsPreparing() )
			m_PetAtt.UnSetPetState(EPS_Preparing);
		
		if( IsMounted() )
		{
			m_PetAtt.UnSetPetState(EPS_Mounting);
			
			m_PetAtt.ModAttVal(EPA_MountNum, -1);

			GetMaster()->SetMount(FALSE, GetPetAtt().GetAttVal(EPA_MountSpeed), GetPetAtt().GetProto()/* + GetMaster()->GetAttValue(ERA_Speed_Mount)*/);

			if (P_VALID(m_pPassenger))
			{
				m_pPassenger->GetPetPocket()->StopRideAfter(m_pMaster);
			}
		}
	}

	// 更新生存状态，检查是否生存期已满
	tagDWORDTime dwBornData = m_PetAtt.GetBornDate();
	DWORD dwLifetime = pTmpProto->dwLifeTime + m_PetAtt.GetAddedLife();
	tagDWORDTime dwDeadLine = IncreaseTime(dwBornData, dwLifetime); 
	tagDWORDTime dwNow = GetCurrentDWORDTime();
	if ( pTmpProto->dwLifeTime != GT_INVALID) // 是否是时限宠物
	{
		if ( m_PetAtt.GetLiveState() && CompareDwordTime(dwDeadLine, dwNow) == GT_INVALID )
		{
			// 将灵兽变为休息状态
			Role* pMaster = GetMaster();
			if ( !P_VALID(pMaster) )
			{
				return;
			}

			PetPocket* pPocket = pMaster->GetPetPocket();
			if ( !P_VALID(pPocket) )
			{
				return;
			}

			if (  P_VALID(GetProto()) )
			{
				if ( 0 != GetProto()->nMountSpeed)
				{
					//手动取消乘骑的状态
					pMaster->StopMount();
				}
			}
			// 生存期已尽
			m_PetAtt.SetPetLive(FALSE);
			m_PetAtt.SetAddedLife(0);
	
			// 禁用灵兽的所有技能
			DisableAllSkill();

			

			pPocket->RestPet(GetID());

			// 通知客户端
			tagNS_PetLiveEnd send;
			send.dwPetID = GetID();
			pMaster->SendMessage(&send, send.dwSize);

		}
		else if ( FALSE == m_PetAtt.GetLiveState() && CompareDwordTime(dwDeadLine, dwNow) != GT_INVALID )
		{
			// 生存期被延长
			m_PetAtt.SetPetLive(TRUE);

			// 启用灵兽的所有技能
			//EnableAllSkill();

			// 通知客户端
			Role* pMaster = GetMaster();
			if ( !P_VALID(pMaster) )
			{
				return;
			}
	
			tagNS_PetLifeExtend send;
			send.dwPetID = GetID();
			send.dwLife = CalcTimeDiff(dwDeadLine, dwNow);
			pMaster->SendMessage(&send, send.dwSize);
		}
	}
	// 生存状态更新完毕

	if (P_VALID(GetMaster()) )
	{
		EMoveState ems = GetMaster()->GetMoveData().GetCurMoveState();
		switch(ems)
		{
		case EMS_Swim:
			if (IsMounted() && !m_PetAtt.GetProto()->bFlyable)
			{
				GetMaster()->StopMount();
			}
			if (IsPetInMap())
			{
				BodyLeaveMap();
			}
			break;		
		case EMS_SwimStand:
		case EMS_Walk:
			if(IsMounted() && !m_PetAtt.GetProto()->bFlyable)
			{
				if(GetMaster()->GetMoveData().IsWillSwim())
					GetMaster()->StopMount();
			}
			if (!IsMounted() && !IsPetInMap())
			{
				BodyEnterMap();
			}
			break;
	case EMS_Stand:			
			if (!IsMounted() && !IsPetInMap())
			{
				BodyEnterMap();
			}
			break;
		}
	}
	//update all skills
	UpdateAllSkill();

	//update equip
	m_EquipBar.Update();

	
	//灵力回复1%/2秒
	if (!IsInState(EPS_Called) && --nCount < 0)
	{
		nCount = SPIRIT_RESUME_COUNT;
		INT nMaxSpirit = GetPetAtt().GetAttVal(EPA_SpiritMax);
		if (GetPetAtt().GetAttVal(EPA_Spirit) < nMaxSpirit)
		{
			GetPetAtt().ModAttVal(EPA_Spirit, nMaxSpirit / 100, TRUE);
		}
	}
}

//----------------------------------------------------------------------------------------------------
// 与躯体整合
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::IntegrateInPet(Pet* pPet)
{
	if (!P_VALID(pPet) || P_VALID(m_pBody))
	{
		return FALSE;
	}

	//Set Body by Pet
	m_pBody = pPet;

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 离开躯体
//----------------------------------------------------------------------------------------------------
VOID PetSoul::DetachFromPet()
{
	if (!P_VALID(m_pBody))
	{
		ASSERT(0);
		return;
	}

	//Set Body to NULL
	//m_pBody = NULL;
}

//----------------------------------------------------------------------------------------------------
// 初始化
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::Init( const tagDBPet* &pSoulData, BOOL bCreate )
{
	// 加载宠物属性
	if (!m_PetAtt.InitAtt(&pSoulData->petAtt))
	{
		return FALSE;
	}

	// 加载生存状态
	/*m_bLive = m_PetAtt.GetLiveState();
	m_dwBornDate = m_PetAtt.GetBornDate();
	m_byAddedLife = m_PetAtt.GetAddedLife();*/

	PVOID pData = (PVOID)pSoulData->byData;

	if (bCreate)
	{
		OnLevelUp();
	}
	else
	{
		// 加载宠物技能
		InitPetSkill(pData, pData, pSoulData->nPetSkillNum);

		// 加载宠物装备
		InitPetEquip(pData, pData, pSoulData->nPetEquipNum);

		if( 0 == pSoulData->nPetSkillNum && 0 == pSoulData->nPetEquipNum )
		{
			BYTE* pDataTemp = (BYTE*)pData;
			pDataTemp += sizeof(BYTE);
			pSoulData = (tagDBPet*)pDataTemp;
		}
		else
		{
			pSoulData = (tagDBPet*)pData;
		}

	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 通过数据库数据创建灵魂
//----------------------------------------------------------------------------------------------------
PetSoul* PetSoul::CreateSoulByDBData( const BYTE* &pData, BOOL bCreate )
{
	MTRANS_POINTER(pSoulData, pData, const tagDBPet);
	PetSoul* pNewSoul = new PetSoul;

	if (!pNewSoul->Init(pSoulData, bCreate))
	{
		ASSERT(0);
		SAFE_DEL(pNewSoul);
		return NULL;
	}
	pData = (BYTE*)pSoulData;

	return pNewSoul;
}

//----------------------------------------------------------------------------------------------------
// 宠物换装
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::Equip( INT64 n64ItemID, INT8 n8DstPos, BOOL bSend /*= FALSE*/ )
{
	tagItem* p2Equip = m_EquipBar.GetFromRoleBag(n64ItemID);
	if (!P_VALID(p2Equip))
	{
		ASSERT(0);
		return E_Pets_PetEquip_PetEquipNotInBag;
	}

	if (EISF_PetEquipment != p2Equip->pProtoType->eSpecFunc)
	{
		ASSERT(0);
		return E_Pets_PetEquip_IsNotPetEquip;
	}

	const tagPetEquipProto* pProto = g_attRes.GetPetEquipProto(p2Equip->pProtoType->nSpecFuncVal1);
	if (!P_VALID(pProto))
	{
		ASSERT(0);
		return E_Pets_PetEquip_UnknownEquipProto;
	}

	//检查等级
	if (m_PetAtt.GetStep() < pProto->nStep || m_PetAtt.GetGrade() < pProto->nGrade)
	{
		return E_Pets_PetEquip_PetLevelNotEnough;
	}

	return m_EquipBar.Equip(n64ItemID, n8DstPos, bSend);
}

//----------------------------------------------------------------------------------------------------
// 宠物卸装
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::UnEquip( INT64 n64ItemID, INT16 n16DstPos, BOOL bSend /*= FALSE*/ )
{
	//tbd: 例行检查
	tagItem* p2Equip = m_EquipBar.GetFromPet(n64ItemID);
	if (!P_VALID(p2Equip))
	{
		ASSERT(0);
		return E_Pets_PetEquip_PetEquipNotInBag;
	}

	const tagPetEquipProto* pProto = g_attRes.GetPetEquipProto(p2Equip->pProtoType->nSpecFuncVal1);
	if (!P_VALID(pProto))
	{
		ASSERT(0);
		return E_Pets_PetEquip_UnknownEquipProto;
	}

	return m_EquipBar.UnEquip(n64ItemID, n16DstPos, bSend);
}

//----------------------------------------------------------------------------------------------------
// 初始化宠物技能
//----------------------------------------------------------------------------------------------------
VOID PetSoul::InitPetSkill(IN PVOID pData, OUT PVOID& pDataOut, IN INT nNum)
{
	tagDBPetSkill* pDBPetSkill		= (tagDBPetSkill*)pData;
	tagDBPetSkill* pDBPetSkillEnd	= (tagDBPetSkill*)pData + nNum;

	while(pDBPetSkill < pDBPetSkillEnd)
	{
		PetSkill* pPetSkill = PetSkill::CreateDBPetSkill(pDBPetSkill, this);
		if (P_VALID(pPetSkill))
		{
			if (E_Pets_Success != AddSkillOnLoad(pPetSkill))
			{
				ASSERT(0);
				ILOG->Write(_T("pet skill<typeid:%d> init failed! Please check db!\n"), pPetSkill->GetSkillTypeID());
				PetSkill::DeletePetSkill(pPetSkill);
			}
		}
		else
		{
			ASSERT(0);
		}
		++pDBPetSkill;
	}

	pDataOut = (PVOID)pDBPetSkillEnd;
}

//----------------------------------------------------------------------------------------------------
// 初始化宠物装备
//----------------------------------------------------------------------------------------------------
VOID PetSoul::InitPetEquip(IN PVOID pData, OUT PVOID& pDataOut, IN INT nNum)
{
	const tagItem* pItem	= NULL;
	const tagItem* pItemEnd = NULL;

	BYTE * p = (BYTE*)pData;
	//while (pItem < pItemEnd)
	for(INT i = 0; i < nNum; ++i)
	{
		INT32 * pLen = (INT32*)p;
		p += sizeof(INT32);
		if( *pLen < sizeof(tagItem) )
		{
			p += *pLen;
			continue;
		}
		pItem = (const tagItem*)p;
		tagItem* pTmpItem = new tagItem;
		ASSERT(!MIsEquipment(pItem->dwTypeID));
		IFASTCODE->MemCpy(pTmpItem, pItem, sizeof(tagItem));

		const tagItemProto* pItemProto = g_attRes.GetItemProto(pTmpItem->dwTypeID);
		if (!P_VALID(pItemProto))
		{
			SAFE_DEL(pTmpItem);
			continue;
		}
		pTmpItem->pProtoType = pItemProto;

		pTmpItem->pScript = g_ScriptMgr.GetItemScript( pTmpItem->dwTypeID);

		if(m_EquipBar.IsOnePlaceFree(pTmpItem->n16Index))
		{
			m_EquipBar.AddEquip(pTmpItem, pTmpItem->n16Index, TRUE);
		}
		else
		{
			ASSERT(0);
			ILOG->Write(_T("pet equip<serial:%d, typeid,%d> init failed! Please check db!\n"), pTmpItem->GetKey(), pTmpItem->dwTypeID);
			::Destroy(pTmpItem);
		}
		
		//++pItem;
		p += *pLen;
	}

	pDataOut = (PVOID)p;
}

//----------------------------------------------------------------------------------------------------
// 填充客户端用宠物属性结构
//----------------------------------------------------------------------------------------------------
VOID PetSoul::FillClientPetAtt( tagPetInitAttr* pInitAtt )
{
	pInitAtt->dwID				= GetID();
	pInitAtt->dwTypeID			= GetProtoID();

	pInitAtt->nQuality			= GetPetAtt().GetAttVal(EPA_Quality);
	pInitAtt->nAptitude			= GetPetAtt().GetAttVal(EPA_Aptitude);
	pInitAtt->nSpirit			= GetPetAtt().GetAttVal(EPA_Spirit);
	pInitAtt->nSpiritMax		= GetPetAtt().GetAttVal(EPA_SpiritMax);
	INT v = GetPetAtt().GetAttVal(EPA_WuXingEnergy);
	pInitAtt->nWuXingEnergy		= 0;//v < 0 ? 0 : v; 
	pInitAtt->nLevel			= GetPetAtt().GetVLevel();
	pInitAtt->nExpCurrent		= GetPetAtt().GetCurExp();
	pInitAtt->nExpLevelUp		= GetPetAtt().GetExpLvlUp();
	pInitAtt->nPotential		= GetPetAtt().GetAttVal(EPA_Potential);
	pInitAtt->nMountSpeed		= GetPetAtt().GetAttVal(EPA_MountSpeed);
	pInitAtt->nBagGrid			= GetPetAtt().GetAttVal(EPA_BagGrid);
	pInitAtt->byPetState		= GetPetAtt().GetState();
	pInitAtt->nMountNum			= GetPetAtt().GetAttVal(EPA_MountNum);
	pInitAtt->nMountNumMax		= GetPetAtt().GetAttVal(EPA_MountNumMax);
	pInitAtt->bLocked			= GetPetAtt().IsLocked() == TRUE;
	pInitAtt->bLive				= GetPetAtt().GetLiveState();
	
	// 计算剩余生存时间
	tagDWORDTime dwNow = GetCurrentDWORDTime();
	tagDWORDTime dwBornTime = GetPetAtt().GetBornDate();
	const tagPetProto* pProto = GetPetAtt().GetProto();
	if (!P_VALID(pProto))
	{
		return;
	}

	DWORD dwLife = pProto->dwLifeTime + (GetPetAtt().GetAddedLife());
	tagDWORDTime dwDeadline = IncreaseTime(dwBornTime, dwLife);

	pInitAtt->dwLastLiveTime = CalcTimeDiff(dwDeadline, dwNow);

	// 序列化装备
	pInitAtt->nPetEquipNum		= 0;
	tagPetEquipMsgInfo* pEquipMsgInfo = (tagPetEquipMsgInfo*)pInitAtt->byData;
	for(INT16 nIndex = 0; nIndex < SIZE_PET_EQUIP_BAR; ++nIndex)
	{
		tagItem* pItem = m_EquipBar.GetItem(nIndex);
		if (!P_VALID(pItem)) 
			continue;
		m_EquipBar.FillClientEquipData(pEquipMsgInfo, pItem);
		++pEquipMsgInfo;
		++pInitAtt->nPetEquipNum;
	}

	// 序列化技能
	pInitAtt->nPetSkillNum		= m_mapPetSkill.Size();
	tagPetSkillMsgInfo* pSkillMsgInfo	= (tagPetSkillMsgInfo*)pEquipMsgInfo;
	PetSkillMap::TMapIterator itr = m_mapPetSkill.Begin();
	PetSkill* pSkill = NULL;
	while (m_mapPetSkill.PeekNext(itr, pSkill))
	{
		if (!P_VALID(pSkill))
			continue;
		pSkill->FillClientInfo(pSkillMsgInfo);
		++pSkillMsgInfo;		
	}
}

//----------------------------------------------------------------------------------------------------
// 存数据库
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::SaveToDB( IN LPVOID pData, OUT LPVOID &pOutPointer )
{
	MTRANS_POINTER(pDBPet, pData, tagDBPet);

	// --------------------------------------------------------------------------------------------\
	// 在保存数据库之前
	// 计算宠物实力 = 阶*250 + 等*25 +（最大潜力-当前潜力）/20 + 装备品级总和*150
	// --> 阶*500 + 等*50+（最大潜力-当前潜力）/10+装备品级总和*300
	DWORD dwEquipValue = 0;
	tagItem	*pTemp = NULL;
	for(INT16 i=0; i< GetEquipBar()->GetCurSpaceSize(); ++i)
	{
		pTemp = GetEquipBar()->GetItem(i);
		if(P_VALID(pTemp))
		{
			dwEquipValue += pTemp->pProtoType->byQuality;
		}
	}
	dwEquipValue *= 300;

	// -（最大潜力-当前潜力)/(20-->10)， 最大潜力=资质*10
	DWORD dwPotentialValue = ( GetPetAtt().GetAttVal(EPA_Aptitude)*10 - GetPetAtt().GetAttVal(EPA_Potential) )/10;

	DWORD dwPetValue = GetPetAtt().GetStep()*500 + GetPetAtt().GetGrade()*50 + dwEquipValue + dwPotentialValue;

	m_PetAtt.SetPetValue(dwPetValue);
	pDBPet->petAtt.dwPetValue = dwPetValue;
	//---------------------------------------------------------------------------------------------/

	// 宠物属性
	if(!GetPetAtt().SaveToDB(&pDBPet->petAtt))
		return FALSE;

	// 宠物技能
	PetSkillMap::TMapIterator itr = m_mapPetSkill.Begin();
	PetSkill* pSkill = NULL;
	pDBPet->nPetSkillNum = 0;
	tagDBPetSkill* pSkills = (tagDBPetSkill*)pDBPet->byData;
	while (m_mapPetSkill.PeekNext(itr, pSkill))
	{
		if(!P_VALID(pSkill))
			continue;
		pSkill->SaveToDB(&pSkills[pDBPet->nPetSkillNum]);
		++pDBPet->nPetSkillNum;
	}

	pOutPointer = (PVOID)(&pSkills[pDBPet->nPetSkillNum]);

// 	// 宠物装备
// 	pDBPet->nPetEquipNum = 0;
// 	PVOID pEquips = (PVOID)(&pSkills[pDBPet->nPetSkillNum]);
// 	m_EquipBar.SaveToDB(pEquips, pEquips, pDBPet->nPetEquipNum);
// 	pOutPointer = (PVOID)pEquips;

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 装备换位置
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::EquipSwapPos( INT64 n64ItemID, INT8 n8DstPos, BOOL bSend /*= FALSE*/ )
{
	INT16 n16DstPos = (INT16)n8DstPos;

	return m_EquipBar.SwapEquipPos(n64ItemID, n8DstPos, bSend);
}

//----------------------------------------------------------------------------------------------------
// 填充客户端用宠物属性结构
//----------------------------------------------------------------------------------------------------
VOID PetSoul::GetEquipInfo( INT64 n64ItemID, tagPetEquipMsgInfo* pMsgInfo )
{
	tagItem* pItem = m_EquipBar.GetItem(n64ItemID);
	m_EquipBar.FillClientEquipData(pMsgInfo, pItem);
}

//----------------------------------------------------------------------------------------------------
// 灌注经验
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::PourExp( INT64 n64ItemID )
{
	if (!IsCalled())
		return E_Pets_StateLimit;

	Role* pMaster = GetMaster();
	if (!P_VALID(pMaster))
		return E_Pets_Soul_MasterNotFound;

	if (GetPetAtt().GetGrade() == MAX_PET_GRADE && 
		GetPetAtt().GetCurExp() >= GetPetAtt().GetExpLvlUp())
		return E_Pets_Soul_GradeNotFit;

	PetSoul*	pSoul = GetPetAtt().GetPetSoul();
	DWORD  dwResult = 0;
	if ( P_VALID(pSoul))
	{
		if( pSoul->IsWorking() )
		{
		    return  dwResult = E_Pets_PetSkill_Learn_StateLimit;
		}
	}

	// 普通灌注
	if (!GT_VALID(n64ItemID))
	{
		DWORD dwRtv = E_Pets_Success;

		const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(pMaster->GetLevel());
		INT64 nMoneyNeed	= GetPetAtt().CalPourMoney();
		INT64 nCurMoney		= GetMaster()->GetCurMgr().GetBagSilver();
		INT nNeedExp		= INT(pEffect->nExpLevelUp * 0.02);
		INT64 nCurExp			= pMaster->GetCurLevelExp();
		if(nCurExp < nNeedExp)
		{
			dwRtv = E_Pets_Soul_RoleExpNotEnough;
		}
		else if (nCurMoney < nMoneyNeed)
		{
			dwRtv = E_Pets_Soul_RoleMoneyNotEnough;
		}
		else
		{
   
			pMaster->ExpChange(INT64(-nNeedExp));
			pMaster->GetCurMgr().DecBagSilver(nMoneyNeed, ELCLD_RolePourExpPet);

			GetPetAtt().ExpChange(nNeedExp, TRUE);
			dwRtv = E_Pets_Success;
		}

		return dwRtv;
		
	}
	// 物品灌注
	else
	{
		tagItem* pItem = GetMaster()->GetItemMgr().GetBagItem(n64ItemID);
		if (!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_PetExpPour)
		{
			return E_Pets_ItemNotValid;
		}
		
		GetPetAtt().ExpChange(pItem->pProtoType->nSpecFuncVal1, TRUE);

		GetMaster()->GetItemMgr().ItemUsedFromBag(n64ItemID, 1, ELCLD_RolePourExpPet);

		return E_Pets_Success;
	}
}

//----------------------------------------------------------------------------------------------------
// 灌注经验
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::OnLevelUp()
{
	// 学习技能
	return LearnNormalSkill(GetPetAtt().GetVLevel());

}

//----------------------------------------------------------------------------------------------------
// 处理宠物技能命令
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::HandleSkillCmd( DWORD dwPetSkillTypeID, PVOID pSkillData, DWORD dwSkillDataSize )
{
	PVOID pSendData = g_MemPoolSafe.Alloc(MAX_PET_SKILL_DATA);
	DWORD dwSendSize = 0;

	DWORD dwErrCode = E_Pets_Success;

	DWORD dwPetSkillID = MTransPetSkillID(dwPetSkillTypeID);
	PetSkill* pPetSkill = GetSkill(dwPetSkillID);
	if (!P_VALID(pPetSkill))
	{
		g_MemPoolSafe.Free(pSendData);
		return E_Pets_PetSkill_Skill_NotExist;
	}

	while(true)
	{		
		BYTE byCastCond		= pPetSkill->GetCastCondition();
		BYTE byStateFlag	= GetPetAtt().GetStateFlag();
		if(	IsMounted()
			|| (byStateFlag & byCastCond) != byStateFlag 
			|| IsWorking() && GetWorkingSkillTypeID() != pPetSkill->GetSkillTypeID()
			|| m_pMaster->IsInRoleStateAny(ERS_Stall | ERS_StallSet)
			)
		{
			dwErrCode = E_Pets_PetSkill_Use_StateLimit;
			break;
		}

		ActiveSkill *pActiveSkill = dynamic_cast<ActiveSkill *>(pPetSkill);
		if (!P_VALID(pActiveSkill))
		{
			dwErrCode = E_Pets_PetSkill_Use_NotActiveSkill;
			break;
		}

		ZeroMemory(pSendData, MAX_PET_SKILL_DATA);
		dwErrCode = pActiveSkill->PreHandleCmd(pSkillData, dwSkillDataSize, pSendData, dwSendSize);
		
		break;
	}
	pPetSkill->SendPetSkillMsg(pSendData, dwSendSize, dwErrCode);
	g_MemPoolSafe.Free(pSendData);
	return dwErrCode;
}

//----------------------------------------------------------------------------------------------------
// 整合到玩家
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::IntegrateInRole( Role* pRole )
{
	if (!P_VALID(pRole))
	{
		ASSERT(0);
		return FALSE;
	}
	SetMaster(pRole);

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 从玩家身上分离
//----------------------------------------------------------------------------------------------------
VOID PetSoul::DetachFromRole()
{

	SetMaster(NULL);
}

//----------------------------------------------------------------------------------------------------
// 实体进入地图
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::BodyEnterMap()
{
	if (!P_VALID(m_pMaster) || !P_VALID(m_pMaster->GetMap()))
	{
		ASSERT(0);
		return GT_INVALID;
	}

	if(!P_VALID(m_pBody))	return GT_INVALID;

	if(IsPetInMap())		return GT_INVALID;

	DWORD	dwMapID = m_pMaster->GetMapID();
	Vector3	vNewPos = m_pMaster->GetMoveData().m_vPos;
	Vector3	vNewFace = m_pMaster->GetMoveData().m_vFace;

	m_pBody->SetMapID(dwMapID);
	m_pBody->GetMoveData().Reset(vNewPos.x, vNewPos.y, vNewPos.z, vNewFace.x, vNewFace.y, vNewFace.z);

	m_pMaster->GetMap()->AddCreature(m_pBody);

	return E_Success;
}

//----------------------------------------------------------------------------------------------------
// 宠物实体离开地图
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::BodyLeaveMap()
{
	if (!P_VALID(GetBody()))
	{
		ASSERT(0);
		return GT_INVALID;
	}
	
	if(!IsPetInMap())		return GT_INVALID;

	if (P_VALID(GetBody()->GetMap()))
	{
		m_pMaster->GetMap()->RemovePet(m_pBody);
	}
	
	return E_Success;
}

//----------------------------------------------------------------------------------------------------
// 是否召唤
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::IsCalled()
{
	return IsInState(EPS_Called) && P_VALID(m_pBody);
}

//----------------------------------------------------------------------------------------------------
// 学习技能
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::LearnSkill( DWORD dwPetSkillTypeID )
{
	DWORD dwSkillID		= MTransPetSkillID(dwPetSkillTypeID);
	INT	nSkillLvl		= MTransPetSkillLevel(dwPetSkillTypeID);

	const tagPetSkillProto* pProto = g_attRes.GetPetSkillProto(dwPetSkillTypeID);
	if (!P_VALID(pProto))
	{
		ASSERT(0);
		ILOG->Write(_T("learn pet skill no pet proto<skill_type_id: %u>,<pet_id: %u>\r\n"),dwPetSkillTypeID, GetID());
		return E_Pets_UnkownErr;
	}

	DWORD dwSkillType3 = GetPetAtt().GetProto()->nType3;
	if(pProto->nType3 != 0 && pProto->nType3 != dwSkillType3)
		return E_Pets_PetSkill_Learn_Type3NotFit;

	PetSkill* pPreSkill = GetSkill(dwSkillID);
	if( nSkillLvl != 1 && !P_VALID(pPreSkill) 
		|| P_VALID(pPreSkill) && pPreSkill->GetProto()->nSkillLevel + 1 != nSkillLvl)
		return E_Pets_PetSkill_Learn_NoPreSkill;

	// 遗忘低级技能
	ForgetSkill(dwSkillID);

	// 宠物是否有空闲位置
	if (m_mapPetSkill.Size() >= MAX_PET_SKILL_NUM)
	{
		ILOG->Write(_T("learn pet skill not enough space<skill_type_id: %u>,<pet_id: %u>\r\n"),dwPetSkillTypeID, GetID());
		return E_Pets_PetSkill_Learn_MaxLeanNum;
	}

	// 创建技能对象
	PetSkill* pPetSkill = PetSkill::CreatePetSkill(dwPetSkillTypeID, this);
	if (!P_VALID(pPetSkill))
	{
		ILOG->Write(_T("learn pet skill create err<skill_type_id: %u>,<pet_id: %u>\r\n"),dwPetSkillTypeID, GetID());
		return E_Pets_UnkownErr;
	}

	// 添加技能
	DWORD dwRtv = AddSkill(pPetSkill);
	if (E_Pets_Success != dwRtv)
	{
		ILOG->Write(_T("learn pet skill addskill err<skill_type_id: %u>,<pet_id: %u>\r\n"),dwPetSkillTypeID, GetID());
		PetSkill::DeletePetSkill(pPetSkill);
	}
	else
	{
		if (P_VALID(GetMaster()))
		{
			tagNS_AddPetSkill sendNS;
			sendNS.dwPetID	= GetID();
			sendNS.dwSkillID	= dwPetSkillTypeID;
			GetMaster()->SendMessage(&sendNS, sendNS.dwSize);
		}

		tagNDBC_InsertPetSkill send;
		send.dwPetID	= GetID();
		pPetSkill->SaveToDB(&send.petSkill);
		g_dbSession.Send(&send, send.dwSize);
	}

	return dwRtv;
}

//----------------------------------------------------------------------------------------------------
// 移除一个技能
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::RemoveSkill( PetSkill* pSkill )
{
	// 停止工作
	if(pSkill->CanSetWorking(FALSE))
		pSkill->SetWorking(FALSE);
	
	// 失效
	PassiveSkill *pPassiveSkill = dynamic_cast<PassiveSkill *>(pSkill);
	if (P_VALID(pPassiveSkill))
	{
		pPassiveSkill->DeActive();
	}

	EnhanceSkill *pEnhanceSkill = dynamic_cast<EnhanceSkill *>(pSkill);
	if (P_VALID(pEnhanceSkill))
	{
		pEnhanceSkill->Close();
	}


	DWORD dwPetSkillTypeID	= pSkill->GetSkillTypeID();
	DWORD dwPetSkillID		= MTransPetSkillID(dwPetSkillTypeID);
	DWORD dwPetSkillLvl		= MTransPetSkillLevel(dwPetSkillTypeID);

	// 移除映射
	if (!GT_VALID(m_mapPetSkill.Peek(dwPetSkillID)))
	{
		return GT_INVALID;
	}
	m_mapPetSkill.Erase(dwPetSkillID);

	return E_Pets_Success;
}

//----------------------------------------------------------------------------------------------------
// 设置是否是工作状态
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::SetWorking( BOOL bWorking, DWORD dwPetSkillTypeID )
{
	if (bWorking)
	{
		if (GT_VALID(m_dwWorkingSkillTypeID))
		{
			return FALSE;
		}
		else
		{
			GetPetAtt().SetPetState(EPS_Working, TRUE);
			m_dwWorkingSkillTypeID = dwPetSkillTypeID;
			return TRUE;
		}
	}
	else
	{
		if (dwPetSkillTypeID != m_dwWorkingSkillTypeID)
		{
			return FALSE;
		}
		else 
		{
			GetPetAtt().UnSetPetState(EPS_Working, TRUE);
			m_dwWorkingSkillTypeID = GT_INVALID;
			return TRUE;
		}
	}
}

//----------------------------------------------------------------------------------------------------
// 能否设置骑乘
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::CanSetMount( BOOL bSet )
{
	if (!P_VALID(GetMaster()))
	{
		return FALSE;
	}

	if (IsWorking())
	{
		return FALSE;
	}

	if (bSet)
	{
		if (IsMounted() || GetMaster()->IsInRoleState(ERS_Mount))
		{
			return FALSE;
		}
		if (GetPetAtt().GetAttVal(EPA_MountNum) >= GetPetAtt().GetAttVal(EPA_MountNumMax))
		{
			return FALSE;
		}
	}
	else
	{
		if (!IsMounted() || !GetMaster()->IsInRoleState(ERS_Mount))
		{
			return FALSE;
		}
		if (GetPetAtt().GetAttVal(EPA_MountNum) <= 0 || GetPetAtt().GetAttVal(EPA_MountNumMax) <= 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 设置骑乘
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::SetMount( BOOL bSet )
{
	if (!P_VALID(GetMaster()))
	{
		ASSERT(0);
		return FALSE;
	}

	// 判断宠物是否活着
	if (!GetLiveState())
	{
		return FALSE;
	}

	if (bSet)
	{
		// 若召唤，则从地图中移除宠物
		if (IsCalled() && IsPetInMap())
		{
			BodyLeaveMap();
		}

		// 设置宠物状态
		GetPetAtt().SetPetState(EPS_Mounting);

		GetPetAtt().ModAttVal(EPA_MountNum, 1);

		GetMaster()->SetMount(TRUE, GetPetAtt().GetAttVal(EPA_MountSpeed), GetPetAtt().GetProto()/* + GetMaster()->GetAttValue(ERA_Speed_Mount)*/);

		// 发送骑乘消息
		if (P_VALID(GetMaster()) && P_VALID(GetMaster()->GetMap()))
		{
			tagNS_Mount send;
			send.dwRoleID		= GetMaster()->GetID();
			send.dwPetID		= GetID();
			send.dwPetTypeID	= GetProtoID();
			GetMaster()->GetMap()->SendBigVisTileMsg(GetMaster(), &send, send.dwSize);
		}
		
		return TRUE;
	}
	else
	{
		// 若召唤，则从地图中移除宠物
		if (IsCalled() && !IsPetInMap())
		{
			BodyEnterMap();
		}

		// 设置宠物状态
		GetPetAtt().UnSetPetState(EPS_Mounting);
		
		GetPetAtt().ModAttVal(EPA_MountNum, -1);

		GetMaster()->SetMount(FALSE, GetPetAtt().GetAttVal(EPA_MountSpeed), GetPetAtt().GetProto()/* + GetMaster()->GetAttValue(ERA_Speed_Mount)*/);

		if (P_VALID(m_pPassenger))
		{
			m_pPassenger->GetPetPocket()->StopRideAfter(m_pMaster);
		}

		return TRUE;
	}
}

//----------------------------------------------------------------------------------------------------
// 能否设置为工作
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::CanSetWroking( BOOL bSet, DWORD dwPetSkillTypeID )
{
	if (bSet)
	{
		if (GT_VALID(m_dwWorkingSkillTypeID))
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		if (dwPetSkillTypeID != m_dwWorkingSkillTypeID)
		{
			return FALSE;
		}
		else 
		{
			return TRUE;
		}
	}
}

//----------------------------------------------------------------------------------------------------
// 是否正在工作
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::IsWorking()
{
	return GT_VALID(m_dwWorkingSkillTypeID) && IsInState(EPS_Working);
}

//----------------------------------------------------------------------------------------------------
// 是否在预备驾驭
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::IsPreparing()
{
	return IsInState(EPS_Preparing);
}

//----------------------------------------------------------------------------------------------------
// 是否可设置为预备驾驭
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::CanSetPreparing( BOOL bSet )
{
	// Whether Can Prepare Pet?
	if (!GetLiveState())
	{
		ASSERT(0);
		return E_Pets_Soul_LiveEnd;
	}

	if(bSet)
	{
		return !IsPreparing();
	}
	else
	{
		return IsPreparing();
	}
}

//----------------------------------------------------------------------------------------------------
// 设置预备驾驭
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::SetPreparing( BOOL bSet )
{
	if (bSet)
	{
		GetPetAtt().SetPetState(EPS_Preparing);
	}
	else
	{
		GetPetAtt().UnSetPetState(EPS_Preparing);
	}
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 是否在某种状态
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::IsInState( EPetState eState )
{
	return GetPetAtt().IsPetInState(eState);
}

//----------------------------------------------------------------------------------------------------
// 能否被设置是否召唤
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::CanSetCalled( BOOL bSet )
{
	if (bSet)
	{
		return !IsCalled();
	}
	else
	{
		return IsCalled();
	}
}

//----------------------------------------------------------------------------------------------------
// 设置是否召唤
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::SetCalled( BOOL bSet, BOOL bSync /*= TRUE*/ )
{
	if (bSet)
	{
		//Create Pet by PetID
		MTRANS_ELSE_RET(pNewBody, Pet::Create(GetID(), this), Pet, FALSE);
		pNewBody->SetCollide(TRUE);

		//IntegrateSoul
		pNewBody->IntegrateSoul(this);

		SetBody(pNewBody);

		//Set Pet State
		GetPetAtt().SetPetState(EPS_Called, bSync);

		if (P_VALID(GetMaster()->GetMap()) && !IsMounted() && !IsPetInMap())
		{
			BodyEnterMap();
		}

		// 所有被动技能生效
		PetSkill* pSkill = NULL;
		PetSkillMap::TMapIterator itr = m_mapPetSkill.Begin();
		while (m_mapPetSkill.PeekNext(itr, pSkill))
		{
			PassiveSkill *pPassiveSkill = dynamic_cast<PassiveSkill *>(pSkill);
			if (P_VALID(pPassiveSkill))
			{
				pPassiveSkill->Active();
			}
		}

		// 放特效
		Map* pMap = pNewBody->GetMap();
		if (P_VALID(pMap))
		{
			tagNS_CallPet callPet;
			callPet.dwPetID = GetID();
			pMap->SendBigVisTileMsg(pNewBody, &callPet, callPet.dwSize);
		}
	}
	else
	{
		// 所有被动技能失效
		PetSkill* pSkill = NULL;
		PetSkillMap::TMapIterator itr = m_mapPetSkill.Begin();
		while (m_mapPetSkill.PeekNext(itr, pSkill))
		{
			PassiveSkill *pPassiveSkill = dynamic_cast<PassiveSkill *>(pSkill);
			if (P_VALID(pPassiveSkill))
			{
				pPassiveSkill->DeActive();
			}
		}

		if (IsPetInMap())
		{
			BodyLeaveMap();
		}
		
		//DetachSoul Soul from Pet
		GetBody()->DetachSoul();

		//delete Pet
		Pet::Delete(GetBody());

		SetBody(NULL);

		GetPetAtt().UnSetPetState(EPS_Called, bSync);
	}

	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 是否正在被骑乘
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::IsMounted()
{
	return IsInState(EPS_Mounting);
}

//----------------------------------------------------------------------------------------------------
// 取得原型
//----------------------------------------------------------------------------------------------------
const tagPetProto* PetSoul::GetProto()
{
	return GetPetAtt().GetProto();
}

//----------------------------------------------------------------------------------------------------
// 提升资质
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::Enhance( INT nAptitudeAdd )
{
	INT nAptitudeOld = GetPetAtt().GetAttVal(EPA_Aptitude);
	GetPetAtt().ModAttVal(EPA_Aptitude, nAptitudeAdd, TRUE);
	nAptitudeAdd = GetPetAtt().GetAttVal(EPA_Aptitude) - nAptitudeOld;
	GetPetAtt().ModAttVal(EPA_Potential, 10 * nAptitudeAdd, TRUE);

	SendSaveAtt2DB();

	return E_Pets_Success;
}

//----------------------------------------------------------------------------------------------------
// 升阶
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::UpStep( DWORD dwPetSkillTypeID )
{
	if (GT_VALID(dwPetSkillTypeID))
	{
		LearnSkill(dwPetSkillTypeID);
	}
	
	return GetPetAtt().UpStep();
}

//----------------------------------------------------------------------------------------------------
// 能否添加一个副骑乘
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::CanAddPassenger( Role* pPassenger )
{
	if (!P_VALID(pPassenger))
		return FALSE;
	if(P_VALID(m_pPassenger))
		return FALSE;
	if(GetPetAtt().GetAttVal(EPA_MountNum) >= GetPetAtt().GetAttVal(EPA_MountNumMax))
		return FALSE;
	if (GetMaster()->GetMapID() != pPassenger->GetMapID())
		return FALSE;
	if (pPassenger->IsInRoleStateAny(ERS_PrisonArea))
		return FALSE;
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 是否能移除一个副骑乘
//----------------------------------------------------------------------------------------------------
BOOL PetSoul::CanRemovePassenger( Role* pPassenger )
{
	if (!P_VALID(m_pPassenger))
		return FALSE;
	if(m_pPassenger != pPassenger)
		return FALSE;
	if(GetPetAtt().GetAttVal(EPA_MountNum) <= 0)
		return FALSE;
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
// 添加一个副骑乘
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::AddPassenger( Role* pPassenger )
{
	ASSERT(CanAddPassenger(pPassenger));
	m_pPassenger = pPassenger;
	//pPassenger->ModAttModValue(ERA_Speed_XZ, GetPetAtt().GetAttVal(EPA_MountSpeed) + GetMaster()->GetAttValue(ERA_Speed_Mount));
	pPassenger->SetPassenger(GetMaster());
	GetPetAtt().ModAttVal(EPA_MountNum, 1, TRUE);
	return E_Pets_Success;
}

//----------------------------------------------------------------------------------------------------
// 从宠物身上移除一个副骑乘
//----------------------------------------------------------------------------------------------------
DWORD PetSoul::RemovePassenger( Role* pPassenger )
{
	ASSERT(CanRemovePassenger(pPassenger));
	//pPassenger->ModAttModValue(ERA_Speed_XZ, -GetPetAtt().GetAttVal(EPA_MountSpeed) - GetMaster()->GetAttValue(ERA_Speed_Mount));
	pPassenger->SetPassenger(NULL);
	GetPetAtt().ModAttVal(EPA_MountNum, -1, TRUE);
	m_pPassenger = NULL;
	pPassenger->RestorOwnerSpeed();
	return E_Pets_Success;
}

BOOL PetSoul::IsPetInMap()
{
	MTRANS_ELSE_RET(pMap, m_pMaster->GetMap(), Map, FALSE);
	return P_VALID(pMap->FindPet(GetPetAtt().GetID()));
}

VOID PetSoul::SetLocked( BOOL bSet )
{
	if (m_PetAtt.IsLocked() == bSet)
	{
		return;
	}
	m_PetAtt.SetLocked(bSet);		
	// 同步到数据库
	SendSaveAtt2DB();
}

BOOL PetSoul::HasEquip()
{
	return m_EquipBar.HasEquip();
}

INT PetSoul::ExportSpecSkill( INT nPetAtt, std::list<PetSkill*> &listModSkill )
{
	INT nPetSkillType = GetModSkillType(nPetAtt);
	std::list<PetSkill*> listAllSkill;
	m_mapPetSkill.ExportAllValue(listAllSkill);
	std::list<PetSkill*>::iterator itr = listAllSkill.begin();
	while(itr != listAllSkill.end())
	{
		const tagPetSkillProto* pProto = (*itr)->GetProto();
		if (P_VALID(pProto) && pProto->eType == (EPetskillType3)nPetSkillType)
		{
			listModSkill.push_back(*itr);
		}
		++itr;
	}
	return listModSkill.size();
}

INT PetSoul::GetModSkillType( INT nPetAtt )
{
	switch(nPetAtt)
	{
	case EPA_DeliveryConsume:	//快递耗时	使用快递技能时，灵兽工作的时间缩放	默认0，装备加成-？~0：ms，对应技能表“工作时间”（降低5秒，填-5000）
		return EPT_Deliver;
		break;
	case EPA_SellConsume:		//贩卖耗时	使用贩卖技能时，灵兽工作的时间缩放	默认0，装备加成-？~0：ms，对应技能表“工作时间”
		return EPT_Sale;
		break;
	case EPA_StorageConsume:	//存放耗时	使用存放技能时，灵兽工作的时间缩放	默认0，装备加成-？~0：ms，对应技能表“工作时间”
		return EPT_StoreHouse;
		break;
	case EPA_GatherConsume:		//采集耗时	使用采集技能时，灵兽工作时间缩放	默认0，装备加成-？~0：ms，对应技能表“工作时间”
		return EPT_Gather;
		break;
	case EPA_TrainResume:		//训练恢复	灵兽的自动训练技能的恢复时间缩放	默认0，装备加成-？~0：ms，对应技能表“恢复时间”
		return EPT_Experience;
		break;
	case EPA_SpiritRate:		//灵力消耗	(0-1 0-10000)使用技能时的灵力消耗缩放比	默认100%，装备加成-100%~0%，对应技能表energy_cost
		return EPT_Null;
		break;
	case EPA_ExpRate:			//经验加成	(0-1 0-10000)获得经验时的缩放比	默认100%，装备加成0%~400%
		return EPT_Null;
		break;
	case EPA_PickUpResume:		//拾取恢复	降低灵兽拾取的恢复时间	默认0，装备加成-20~-1
		return EPT_PickUp;
		break;
	case EPA_MedicineSaving:	//吃药节省	(0-1 0-10000)宠物每次喂药时可能不消耗该药品	默认0%，装备加成1%~5%
		return EPT_MedicineFeed;
		break;
	case EPA_StrengthEffect:	//强身效果	(0-1 0-10000)提升强身类技能的效果	默认100%，装备加成0%~400%
		return EPT_Strengthen;
		break;
	// Jason 2010-4-8 v1.5.1 取消五行灵力，不再使用这个属性了。
	//case EPA_WuXingConsume:		//五行力消耗	(0-1 0-10000)当使用天赋技能时，降低该技能对五行力的需求	默认100%，装备加成-50%~0%，对应技能表“五行力消耗”
	//	return EPT_WuXing;
	//	break;
	case EPA_MountSpeed:		//骑乘速度	(0-1 0-10000)灵兽被驾御时的移动速度	默认白30%，黄40%，绿50%，蓝60%，橙70% 装备加成0%~100%	受阶影响
		return EPT_Null;
		break;
	case EPA_MountNumMax:		//骑乘人数
		return EPT_MountAdd;
		break;
	default:
		return EPT_Null;
		break;
	}

}

DWORD PetSoul::Feed( INT nSpritMod )
{
	GetPetAtt().ModAttVal(EPA_Spirit, nSpritMod);

	return E_Pets_Success;
}

VOID PetSoul::SendSaveAtt2DB()
{
	tagNDBC_UpdatePetSoul send;
	GetPetAtt().SaveToDB(&send.petAtt);
	send.nPetSoulNum = 1;
	g_dbSession.Send(&send, send.dwSize);
}

DWORD PetSoul::StopSkillWorking( DWORD dwPetSkillID )
{
	MTRANS_ELSE_RET(pSkill, GetSkill(dwPetSkillID), PetSkill, GT_INVALID);
	pSkill->SetWorkCounting(GT_INVALID);

	return E_Pets_Success;

}

VOID PetSoul::LogPet( DWORD dwMasterID, DWORD dwPetID, DWORD dwCmdID )
{
	tagNDBC_LogPet send;
	send.sLogPet.dwMasterID = dwMasterID;
	send.sLogPet.dwPetID = dwPetID;
	send.sLogPet.dwCmdID = dwCmdID;
	g_dbSession.Send(&send, send.dwSize);
}

DWORD PetSoul::Rename( LPCTSTR tszNewName )
{
	DWORD dwErrCode = Filter::CheckName(tszNewName,X_PET_NAME, 1, g_attRes.GetNameFilterWords());
	if (dwErrCode != E_Success) return dwErrCode;

	GetPetAtt().SetName(tszNewName);

	tagNS_PetDispInfoChange send;

    Pet* pBody = GetBody();
	if ( P_VALID(pBody) )
	{
         Map* pMap  = pBody->GetMap();
		 if ( P_VALID(pMap) )
		 {
			 GetPetAtt().GetName(send.DispData.szName);
			 send.dwPetID = GetID();
			 send.DispData.dwTypeID = GetProtoID();

			 pMap->SendBigVisTileMsg(pBody, &send, send.dwSize);
			 return E_Pets_Success;
		 }
	}
    Role*  pMaster = GetMaster();
	if ( P_VALID(pMaster) )
	{
		GetPetAtt().GetName(send.DispData.szName);
		send.dwPetID = GetID();
		send.DispData.dwTypeID = GetProtoID();

		pMaster->SendMessage(&send,send.dwSize);
	}


    return E_Pets_Success;
	 
	//MTRANS_ELSE_RET(pBody, GetBody(), Pet, E_Pets_Success);
	//MTRANS_ELSE_RET(pMap, pBody->GetMap(), Map, E_Pets_UnkownErr);

	

	

	
}