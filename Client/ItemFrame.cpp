#include "stdafx.h"
#include "ItemFrame.h"
#include "PocketFrame.h"
#include "StorageFrame.h"
#include "ThroneFrame.h"
#include "ItemProduceFrame.h"
#include "ShopShelfFrame.h"
#include "TradeFrame.h"
#include "ItemButton.h"
#include "ButtonEx.h"
#include "ItemEvent.h"
#include "CombatEvent.h"
#include "GuildEvent.h"
#include "QuestEvent.h"
#include "Item.h"
#include "CursorMgr.h"
#include "ItemMgr.h"
#include "NetSession.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\msg_loot.h"
#include "..\WorldDefine\msg_talent.h"
#include "..\WorldDefine\msg_func_npc.h"
#include "..\WorldDefine\msg_exchange.h"
#include "..\WorldDefine\msg_spec_item.h"
#include "GameClientDefine.h"
#include "QuestMgr.h"
#include "QuestScript.h"
#include "CombatSysUtil.h"
#include "SocialEvent.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "PetEvent.h"
#include "SkillMgr.h"
#include "PetManager.h"
#include "GuildMgr.h"
#include "NPCTalkFrame.h"
#include "InsuranceFrame.h"
#include "FishMgr.h"
#include "RenameFrame.h"
#include "MallMgr.h"
#include "ItemTrackPlayerFrame.h"
#include "FilterData.h"
#include "..\WorldDefine\easybuy.h"
#include "ItemProtoData.h"
#include "ReputeMgr.h"
#include "FashionComposeFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\msg_digbao.h"
#include "SoarEquipFrame.h"
#include "GodStrengthenFrame.h"
#include "EquipStrengthenFrame.h"
#include "..\WorldDefine\RoleDefine.h"
#include ".\FairyAutoFeedFrame.h"
#include ".\FairyTransferFrame.h"
#include ".\FairyUpgradeFrame.h"
#include ".\TrumpRefineFrame.h"
#include ".\TrumpMixBeadFrame.h"
#include ".\TrumpRebornFrame.h"
#include "..\WorldDefine\msg_eggbroker.h"
#include "EggBrokerFrame.h"
#include "FashionDesignerFrame.h"
#include "TreasureChestFrame.h"
#include "PlayerTradeFrame.h"
#include "StallShelfFrame.h"
#include "HolyManMgr.h"
#include "HolyEquipFrame.h"
#include "HolyManFrame.h"
#include "ItemProtoData.h"//add by xp
#include "Container.h"

ItemFrame::ItemFrame():
m_Trunk(this), 
m_n64OperSrcItem(GT_INVALID)
{
	m_pPocket = NULL;
	m_pOperSrcItem = NULL;
    m_n64OperSrcItem = GT_INVALID;
	m_pOperDstItem = NULL;
	m_eCurStrategy = EIST_ItemPocket;
	m_bCanUseBag = TRUE;
}

ItemFrame::~ItemFrame()
{

}

BOOL ItemFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam );

	m_pMgr->RegisterEventHandle(_T("Open_Package"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnOpenPackage));
	m_pMgr->RegisterEventHandle(_T("Close_Package"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnClosePackage));
	m_pMgr->RegisterEventHandle(_T("Open_Ware"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnOpenWare));
	m_pMgr->RegisterEventHandle(_T("Close_Ware"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnCloseWare));
	m_pMgr->RegisterEventHandle(_T("Close_Ware_Now"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnCloseWareNow));
	m_pMgr->RegisterEventHandle(_T("tagClickItemEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnTryToUseItem));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_TargetItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnTheRoleOfItem));
	m_pMgr->RegisterEventHandle(_T("PutDownItem"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnRolePutDownItem));
	m_pMgr->RegisterEventHandle(_T("MsgBox_PutDownCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnPutDownItemCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_UseItemCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnUseSpecItemCheck));
	m_pMgr->RegisterEventHandle(_T("MsgBox_AddPotTips"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnAddPotValue));
	m_pMgr->RegisterEventHandle(_T("MsgBoxEvent_EquipSignatureInput"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnEquipSignatureInput) );
	m_pMgr->RegisterEventHandle(_T("EnterWar"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnEnterWar));
	m_pMgr->RegisterEventHandle(_T("EnterNormalMap"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnEnterNormalMap));

	return bRet;
}

BOOL ItemFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pMgr->UnRegisterEventHandler(_T("Open_Package"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnOpenPackage));
	m_pMgr->UnRegisterEventHandler(_T("Close_Package"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnClosePackage));
	m_pMgr->UnRegisterEventHandler(_T("Open_Ware"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnOpenWare));
	m_pMgr->UnRegisterEventHandler(_T("Close_Ware"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnCloseWare));
	m_pMgr->UnRegisterEventHandler(_T("Close_Ware_Now"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnCloseWareNow));
	m_pMgr->UnRegisterEventHandler(_T("tagClickItemEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnTryToUseItem));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_TargetItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnTheRoleOfItem));
	m_pMgr->UnRegisterEventHandler(_T("PutDownItem"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnRolePutDownItem));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_PutDownCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnPutDownItemCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_UseItemCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnUseSpecItemCheck));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_AddPotTips"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnAddPotValue));
	m_pMgr->UnRegisterEventHandler(_T("EnterWar"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnEnterWar));
	m_pMgr->UnRegisterEventHandler(_T("EnterNormalMap"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnEnterNormalMap));

	m_pMgr->UnRegisterEventHandler(_T("MsgBoxEvent_EquipSignatureInput"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ItemFrame::OnEquipSignatureInput) );

	if(P_VALID(GetObj("UseItemCheck")))
		KillObj("UseItemCheck");

	return bRet;
}

BOOL ItemFrame::ReloadUI()
{
	// 背包界面
	m_pPocket = (PocketFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Pocket"), _T("PocketFrame"), 0);
	return TRUE;
}

DWORD ItemFrame::OnTryToUseItem( tagClickItemEvent* pGameEvent )
{
	
	switch (m_eCurStrategy)
	{
	case EIST_ItemPocket:
		{
			if(EICT_Bag == pGameEvent->eType || EICT_Quest == pGameEvent->eType || EICT_WarBag)
			{
				//物品使用限制检测
				if( !CheckItemLimit(ItemMgr::Inst()->GetConItemByID(pGameEvent->eType, pGameEvent->n64ItemID)))
					break;
				if(MIsEquipment(pGameEvent->dwTypeID))
				{
					//这里知道是装备了，所以才进行类型转换
					Equipment* nCurrentEqup = static_cast<Equipment*>(ItemMgr::Inst()->GetConItemByID(pGameEvent->eType, pGameEvent->n64ItemID));
					//装备使用限制检测
					if (!CheckEquipmentLimit(nCurrentEqup))
						break;

					tagItemPosChangeExEvent event(_T("ItemEvent_Move2Equip"),this);
// 					if (HolyManMgr::Inst()->IsHolyManEquip(pGameEvent->dwTypeID)) //元神装备
// 						event.eTypeDst = EICT_HolyEquip;
// 					else //一般装备
					event.eTypeDst = EICT_Equip;
					event.eTypeSrc = pGameEvent->eType;
					event.n16IndexSrc = pGameEvent->n16Index;
					event.dwTypeIDSrc = pGameEvent->dwTypeID;
					m_pMgr->SendEvent(&event);
				}
				else
				{
					// 物品使用
					Item* pItem = ItemMgr::Inst()->GetConItemByID(pGameEvent->eType, pGameEvent->n64ItemID);
					if(P_VALID(pItem))
					{
						//--检查玩家状态
						const DWORD RoleStateMask=ERS_Stall;
						if(RoleMgr::Inst()->GetLocalPlayer()->GetRoleState()&RoleStateMask)
							return 0;
						
						//检查物品的状态
						CheckItemState( pItem );

						// 优先判断是否是特殊功用物品
						EItemSpecFunc eType = pItem->GetItemSpecFunc();
						if(eType != EISF_Null)
						{
							OnUseSpecItem(pItem);
						}
						// 判断物品是否可以使用
						else if(pItem->IsUseable() && CanUseItemHolyAwake(pItem))
						{						
							OnUseItem(pItem);
						}
						else
						{
							tagUseFairyFoodEvent e(_T("tagUseFairyFoodEvent"), this);
							e.dwItemTypeID = pItem->GetItemTypeID();
							m_pFrameMgr->SendEvent(&e);
						}
					}
				}
			}
		}
		break;
	case EIST_ItemStorage:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if(EICT_Bag == pGameEvent->eType)
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Ware"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_RoleWare;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
			else if(EICT_RoleWare == pGameEvent->eType)
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Bag"),this);
				event.eTypeSrc = EICT_RoleWare;
				event.eTypeDst = EICT_Bag;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
    case EIST_GuildStorage:
        {
            // 判断是否有权限
            if(!GuildMgr::Inst()->isWareOpretable())
            {
                CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildPrivacyUseErr")]);
                return 0;
            }

            // 判断是否绑定
            Item* pItem = ItemMgr::Inst()->GetConItem(EICT_Bag, pGameEvent->n16Index);
            if (P_VALID(pItem))
            {
                if (pItem->IsItemBinded())
                {
                    CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildWareBinding")]);
                    return 0;
                }
            }

            // 不是背包发送的直接返回
            if (pGameEvent->pSender == NULL)
                return 0;

            if (EICT_Bag == pGameEvent->eType)
            {
                tagItemPosChangeExEvent event(_T("ItemEvent_Move2Guild"),this);
                event.eTypeSrc = EICT_Bag;
                event.eTypeDst = EICT_GuildWare;
                event.n16IndexSrc = pGameEvent->n16Index;
                event.dwTypeIDSrc = pGameEvent->dwTypeID;
                m_pMgr->SendEvent(&event);
            }
        }
        break;
	case EIST_ItemShopVend:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if(EICT_Bag == pGameEvent->eType)
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Shop"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_Shop;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_ItemPlayerTrade:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if(EICT_Bag == pGameEvent->eType)
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Trade"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_ItemStall:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if(EICT_Bag == pGameEvent->eType)
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Stall"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_ItemSmithCompose:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType || EICT_WarBag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Compose"),this);
				event.eTypeSrc = pGameEvent->eType;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_HolyCompose:
		{
			if (pGameEvent->pSender != NULL && EICT_Bag == pGameEvent->eType)
			{	 
				m_pOperSrcItem = ItemMgr::Inst()->GetConItemByID(pGameEvent->eType, pGameEvent->n64ItemID);
				if(!P_VALID(m_pOperSrcItem))
					return 0;

				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Compose"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);	
			}
		}
		break;
	case EIST_ItemInsurance:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Insurance"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_GuildUpgrade:
		{
			// 不是背包发送的直接返回
			if ( NULL != pGameEvent->pSender && EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2GuildUpgrade"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
    case EIST_FashionSpin:
        {
            // 时装精纺
            FashionComposeFrame* pFrame = (FashionComposeFrame*)m_pFrameMgr->GetFrame(_T("FashionComposeFrame"));
            if (P_VALID(pFrame))
            {
                Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16Index);
                if (P_VALID(pItem))
                {
                    pFrame->SetItem(pItem);
                }
            }
        }
        break;
	case EIST_TrumpRefine:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2TrumpRefine"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_MixBead:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2MixBead"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_TrumpReborn:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2Reborn"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_EquipStrengthen:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2EquipStrengthen"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_GodStrengthen:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2GodStrengthen"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FairyAutoFeed:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2FairyAutoFeed"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FairyDamageTransfer:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2FairyTransfer"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FairyDamageUpgrade:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2FairyDamageUpgrade"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FairyTransferExp:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2FairyTransferExp"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FairyBorn:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2FairyBorn"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FairyPartnerShip:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2PartnerShip"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_RandomProduce:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2FashionDesigner"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FamilySpriteRegster:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2SpriteRegister"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_FamilySpriteTrain:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2SpriteTrain"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	case EIST_GESkillRenew:
		{
			// 不是背包发送的直接返回
			if (pGameEvent->pSender == NULL)
				return 0;

			if( EICT_Bag == pGameEvent->eType )
			{
				tagItemPosChangeExEvent event(_T("ItemEvent_Move2GESkillRenew"),this);
				event.eTypeSrc = EICT_Bag;
				event.eTypeDst = EICT_End;
				event.n16IndexSrc = pGameEvent->n16Index;
				event.dwTypeIDSrc = pGameEvent->dwTypeID;
				m_pMgr->SendEvent(&event);
			}
		}
		break;
	}
	return 0;
}

VOID ItemFrame::OnUseItem(Item* pItem)
{
	tagUseItemEvent e(_T("tagUseItemEvent"), this);
	e.dwItemTypeID = pItem->GetItemTypeID();
	e.n64ItemID = pItem->GetItemId();
	m_pFrameMgr->SendEvent(&e);
}

VOID ItemFrame::OnUseSpecItem(Item* pItem)
{
	switch (pItem->GetItemSpecFunc())
	{
	case EISF_AcceptQuest:
		{
			tagUseQuestEvent e(_T("QuestEvent_UseQuest"), this);
			e.itemID = pItem->GetItemTypeID();
			e.questID = pItem->GetProto()->nSpecFuncVal1;
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_IdetifyEquip:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"), this);
			e.eCursor = ECAN_Identify;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_HoleGem:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_Gem;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_Brand:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_Brand;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent(&e);

		}
		break;
	case EISF_LongHunShi:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_Longfu;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_Chisel:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_Chisel;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_NewChisel:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_Chisel;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_RockGod:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_RockGod;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_ItemLock:
		{
			tagActiveCursorEvent e( _T("Event_CursorActive"), this );
			e.eCursor = ECAN_ItemLock;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_ItemUnLock:
		{
			tagActiveCursorEvent e( _T("Event_CursorActive"), this );
			e.eCursor = ECAN_ItemLock;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_EquipSignature:
		{
			tagActiveCursorEvent e( _T("Event_CursorActive"), this );
			e.eCursor = ECAN_EquipSignature;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_Grind:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_Grind;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_EquipAddPotValue:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_AddPot;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_BeGoldStone:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_BeGoldStone;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_Chest:// 宝箱
		{
			TreasureChestFrame* pFrame = (TreasureChestFrame*)m_pFrameMgr->GetFrame(_T("TreasureChest"));
			if (P_VALID(pFrame) && pFrame->CanUseAnother())
			{
				tagNC_TreasureChest msg;
				if(pItem->GetItemSpecFunc() == EISF_Chest)
				{
					msg.n64ChestID	= pItem->GetItemId();
					m_pSession->Send(&msg);				
				}

			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TreasureChest_cantuse")]);
			}
			
		}
		break;
	//case EISF_ChestKey:// 宝箱钥匙
	//	{
	//		tagActiveCursorEvent e(_T("Event_CursorActive"),this);
	//		e.eCursor = ECAN_Key;
	//		e.dwParam = pItem->GetPos();
	//		m_pFrameMgr->SendEvent( &e );
	//	}
	//	break;
	case EISF_RemoveAttPt:
		{
			//ShowUseItemCheckDlg( pItem );
		}
		break;
	case EISF_RemoveTalentPt:
		{
			//ShowUseItemCheckDlg( pItem );
		}
		break;
	case EISF_FriendGift:
		{
			tagUseGiftEvent e(_T("SocialEvent_UseGift"), this);
			e.giftGlobalID	= pItem->GetItemId();
			e.giftTypeID	= pItem->GetItemTypeID();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
    case EISF_PetEgg:
        {
            ShowUseItemCheckDlg( pItem );
        }
        break;
    case EISF_PetFood:
        {

			if ( 2 == pItem->GetProto()->nSpecFuncVal1 )
			{
				tagSpecItemEvent e(_T("tagSpecItemEvent"), NULL);
				e.n64Serial = pItem->GetItemId();
				e.eSpecFuncSrc = pItem->GetItemSpecFunc();
				m_pFrameMgr->SendEvent(&e);
				m_pMgr->SendEvent( &tagGameEvent( _T("Open_SpecFrame"), this ) );
				break;
			}
            tagUsePetFoodEvent e(_T("tagUsePetFoodEvent"), this);
            e.n64ItemID     = pItem->GetItemId();
            m_pFrameMgr->SendEvent(&e);
        }
        break;
    case EISF_PetSkillBook:
        {
            ShowUseItemCheckDlg(pItem);
        }
        break;
    case EISF_PetEquipment:
        {
            tagUsePetEquip e(_T("tagUsePetEquip"), this);
            e.n64ItemID     =   pItem->GetItemId();
            m_pFrameMgr->SendEvent(&e);
        }
        break;
    case EISF_PetLvlupStep:
        {
            PetManager::Inst()->OnUseLvlupStep(pItem);
        }
        break;
    case EISF_PetExpPour:
        {
            PetManager::Inst()->OnUseExpPour(pItem);
        }
        break;
    case EISF_PetEnhance:
        {
            PetManager::Inst()->OnUseEnhance(pItem);
        }
        break;
    case EISF_ColorProbability:
        {
        }
        break;
    case EISF_Dye:
        {
            tagActiveCursorEvent e(_T("Event_CursorActive"),this);
            e.eCursor = ECAN_Dye;
            e.dwParam = pItem->GetPos();
            m_pFrameMgr->SendEvent(&e);
        }
        break;
	case EISF_LearnSkill:// 技能书
		{
			// 判断技能是否可学，可学则走物品使用正常流程，不可使用则显示错误信息
			DWORD dwRet = SkillMgr::Inst()->IsSkillCanLearn((DWORD)pItem->GetProto()->nSpecFuncVal1);
			if(E_Success == dwRet)
			{
				ShowUseItemCheckDlg(pItem);
			}
			else
			{
				SkillMgr::Inst()->ShowNetError(dwRet);
			}
		}
		break;
	case EISF_GuildSkillBook:
		{
			tagUseGuildSkillBookEvent event(_T("GuildEvent_UseGuildSkillBook"), this);
			event.pItem = pItem;
			m_pFrameMgr->SendEvent(&event);
		}
        break;
    case EISF_PetLock:
        {
            PetManager::Inst()->SetPetLock(pItem->GetItemId(), true);
        }
        break;
    case EISF_PetUnLock:
        {
            PetManager::Inst()->SetPetLock(pItem->GetItemId(), false);
        }
        break;
	case EISF_PortableWare:									//随身仓库
		{
			tagNC_WareOpen e;
			e.n64ItemID = pItem->GetItemId();
			m_pSession->Send( &e );
		}
		break;
	case EISF_Fishing:
		{
			FishMgr::Inst()->SetFishFoodType( pItem->GetItemTypeID() );
			FishMgr::Inst()->SetItemN64ID( pItem->GetItemId() );
			OnUseItem( pItem );
		}
		break;
    case EISF_TransportTool:                                //可记录传送符
        {
            NPCTalkFrame* pNpcTalkFrame = (NPCTalkFrame*)m_pFrameMgr->GetFrame(_T("NPCTalk"));
            if (!P_VALID(pNpcTalkFrame))
            {
                pNpcTalkFrame = (NPCTalkFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("NPCTalk"), _T("NPCTalkFrame"), 0 );
            }

            pNpcTalkFrame->ClearOptions();
            TCHAR szTemp[256] = {0};
            LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pLp))
            {
                _sntprintf(szTemp, sizeof(szTemp) / sizeof(TCHAR), g_StrTable[_T("TransportItemDesc")]
                    , pLp->GetTransportPointName().c_str(), pLp->GetTransportPoint().x, pLp->GetTransportPoint().z);
            }
            pNpcTalkFrame->SetTransportItemEnable(true);
            pNpcTalkFrame->SetContent(szTemp);
            pNpcTalkFrame->AddTalkOption(g_StrTable[_T("TransportSet")], 0, 0, NPCTalkFrame::EOT_SetTransportPoint);
            pNpcTalkFrame->AddTalkOption(g_StrTable[_T("TransportGo")], 1, 0, NPCTalkFrame::EOT_SetTransportPoint);
            pNpcTalkFrame->AddTalkOption(g_StrTable[_T("TransportCanel")], 2, 0, NPCTalkFrame::EOT_SetTransportPoint);
            pNpcTalkFrame->Show();
            pNpcTalkFrame->SetTransItem(pItem);
        }
		break;
	case EISF_RenameItem:
		{
			ShowUseItemCheckDlg( pItem );
		}
        break;
    case EISF_PetPocketExtItem:
        {
           // ShowUseItemCheckDlg( pItem );
        }
        break;
	case EISF_LoadScript:
		{
			ShowUseItemCheckDlg( pItem );
		}
		break;
	case EISF_PlayerTrack:
		{
			ItemTrackPlayerFrame* pFrame = (ItemTrackPlayerFrame*)m_pFrameMgr->GetFrame( _T("ItemTrackPlayerFrame") );
			if( !P_VALID(pFrame) )
			{
				m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("ItemTrackPlayerFrame"), _T("ItemTrackPlayerFrame"), 0 );
				pFrame = (ItemTrackPlayerFrame*)m_pFrameMgr->GetFrame( _T("ItemTrackPlayerFrame") );
				if( !P_VALID(pFrame) )
					break;
			}

			if( !pFrame->SetTrackItem( pItem ) )
			{
				if( GetObj( "TrackItemCannotUse_MsgBox" ) )
					KillObj( "TrackItemCannotUse_MsgBox" );

				CreateObj( "TrackItemCannotUse_MsgBox","MsgBox" );
				TObjRef<MsgBox>("TrackItemCannotUse_MsgBox")->Init( _T(""), g_StrTable[_T("PlayerTrack_UseItemError")], NULL, MBF_OK, FALSE );
			}
		}
		break;
	case EISF_GuildChangeName:
		{
			ShowUseItemCheckDlg( pItem );
		}
		break;
	case EISF_EarthTemple:
		{
			tagSpecItemEvent e(_T("tagSpecItemEvent"), NULL);
			e.n64Serial = pItem->GetItemId();
			e.eSpecFuncSrc = pItem->GetItemSpecFunc();
			m_pFrameMgr->SendEvent(&e);
		}
		break;
	case EISF_UpgradeEquipMaxPotValue:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_UpgradeMaxPot;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_ZhanDouFu:
		{
			tagSpecItemEvent e(_T("tagSpecItemEvent"), NULL);
			e.n64Serial = pItem->GetItemId();
			e.eSpecFuncSrc = pItem->GetItemSpecFunc();
			m_pFrameMgr->SendEvent(&e);
		}
		break;
	case EISF_XiHun:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_XiHun;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_FuHun:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_FuHun;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_GuiYuan:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_GuiYuan;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_FairyPowder:
		{
			FLOAT fCDCount = 0.0f;
			FLOAT fRestoreRatio = 0.0f;
			ItemMgr::Inst()->GetItemCurCDTime(pItem->GetItemTypeID(), fCDCount, fRestoreRatio);
			if(fCDCount > 0)
				break;

			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_GuiYuan;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_GodStrengthen:
		{
			GodStrengthenFrame* pFrame = (GodStrengthenFrame*)m_pFrameMgr->GetFrame( _T("GodStrengthen") );
			if( !P_VALID(pFrame) )
				pFrame = (GodStrengthenFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("GodStrengthen"), _T("GodStrengthenFrame"), 0 );
			if(P_VALID(pFrame))
				SetStrategy(EIST_GodStrengthen);
		}
		break;
	case EISF_VipCard:
		{
			LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
			if (P_VALID(pLp))
			{
				if ( 0 == pLp->GetAttribute(ERA_VipLevel))
				{
					ShowUseItemCheckDlg( pItem );
				}
				else if (pLp->GetAttribute(ERA_VipLevel) > pItem->GetProto()->nSpecFuncVal1)
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("VIPcard_NotNeedToChangeCard")]);
				else if(pLp->GetAttribute(ERA_VipLevel) == pItem->GetProto()->nSpecFuncVal1)
				{
					TCHAR szTmp[X_LONG_NAME];

					tagDWORDTime time = ServerTime::Inst()->CalCurrentServerDwordTime();
					DWORD timeDiff = CalcTimeDiff(time,pLp->GetVipDays());
					INT LastDays = pLp->GetVipDaysCanLast() - timeDiff/3600/24;

					if(P_VALID(GetObj("UseItemCheck")))
						KillObj("UseItemCheck");
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("VIPcard_ChangeCardTheSame")], LastDays);
					CreateObj("UseItemCheck", "MsgBox");
					TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_UseItemCheck"));
					m_pOperSrcItem = pItem;
					m_n64OperSrcItem = pItem->GetItemId();
					m_pOperDstItem = NULL;
				}
				else
				{
					TCHAR szTmp[X_LONG_NAME];

					tagDWORDTime time = ServerTime::Inst()->CalCurrentServerDwordTime();
					DWORD timeDiff = CalcTimeDiff(time,pLp->GetVipDays());
					INT LastDays = pLp->GetVipDaysCanLast() - timeDiff/3600/24;

					if(P_VALID(GetObj("UseItemCheck")))
						KillObj("UseItemCheck");
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("VIPcard_ChangeCardHighLevel")], LastDays);
					CreateObj("UseItemCheck", "MsgBox");
					TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_UseItemCheck"));
					m_pOperSrcItem = pItem;
					m_n64OperSrcItem = pItem->GetItemId();
					m_pOperDstItem = NULL;
				}
			}
			
		}
		break;
	case EISF_TreasureSeizer:
		{
			ShowUseItemCheckDlg( pItem );
		}
		break;
	case EISF_FairyStuntBook:
		{
			Equipment *pFairy = ItemMgr::Inst()->GetCurEquip(EEP_Face);
			if( !P_VALID(pFairy) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoFairy")]);
				return;
			}
			const tagFabaoSpec *pFairySpe = pFairy->GetFaBaoSpe();
			if(!P_VALID(pFairySpe))
				return;
			if(pFairySpe->n16Stage < 60)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyNonage")]);
				return;
			}

			const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(pItem->GetProto()->nSpecFuncVal1);
			if(P_VALID(pSkill))
			{
				if(P_VALID(GetObj("UseItemCheck")))
					KillObj("UseItemCheck");
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("LearnFairySkill")], pSkill->szName);
				CreateObj("UseItemCheck", "MsgBox");
				TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTemp, _T("MsgBox_UseItemCheck"));
				m_pOperSrcItem = pItem;
				m_n64OperSrcItem = pItem->GetItemId();
				m_pOperDstItem = NULL;
			}
		}
		break;
	case EISF_FairyNirVana:
		{
			Equipment *pFairy = ItemMgr::Inst()->GetCurEquip(EEP_Face);
			if( !P_VALID(pFairy) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoFairyNoNirvana")]);
				return;
			}
			const tagFabaoSpec *pFairySpe = pFairy->GetFaBaoSpe();
			if(!P_VALID(pFairySpe))
				return;
			if(pFairySpe->n16Stage < 61)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyNirvanaLevelTooLow")]);
				return;
			}

			if(P_VALID(GetObj("UseItemCheck")))
				KillObj("UseItemCheck");
			INT nLevel = 0;
			if(pItem->GetProto()->nSpecFuncVal1 == 0)
				nLevel = 60;
			else
				nLevel = ((pFairySpe->n16Stage - 1)/10)*10;
			TCHAR szTemp[X_LONG_NAME] = {0};
			_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("FairyNirvanaWarning")], pFairy->GetDisplay()->szName, nLevel);
			CreateObj("UseItemCheck", "MsgBox");
			TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTemp, _T("MsgBox_UseItemCheck"));
			m_pOperSrcItem = pItem;
			m_n64OperSrcItem = pItem->GetItemId();
			m_pOperDstItem = NULL;
		}
		break;
	case EISF_ExpBall:
		{
			if(0 ==  pItem->GetProto()->nSpecFuncVal1 )
				ShowUseItemCheckDlg( pItem );
		}
		break;
	case EISF_ExpPilular:
		{
			tagNC_UseExpPilular m;
			m.n64ExpPilularSerialID = pItem->GetItemId();
			m_pSession->Send(&m);
		}
		break;
 	case EISF_EggHammer:
 		{
 			EggBrokerFrame *pFrame = (EggBrokerFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("EggBroker") );
 			if( P_VALID(pFrame) )
 			{
 				if (pFrame->IsInVisable())
 				{
 					tagNC_EggBegin m;
 					m_pSession->Send(&m);
 				}
 			}
 		}
 		break;
	case EISF_FairyDecPregnancy:
	case EISF_FairyIncBreedingTimes:
	case EISF_FairyDecRestTime:
		{
			tagUseFairyFoodEvent e(_T("tagUseFairyFoodEvent"), this);
			e.dwItemTypeID = pItem->GetItemTypeID();
			m_pFrameMgr->SendEvent(&e);
		}
		break;
	case EISF_GlobalLimitedItem:
		{
			tagNC_UseLimitedItem send;
			send.n64ItemID = pItem->GetItemId();
			m_pSession->Send(&send);
		}
		break;
	case EISF_FairyRaiseIntelligenceMax:
	case EISF_FairyDoRaiseIntelligence:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"),this);
			e.eCursor = ECAN_Sprite;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_DiamondEquipGrowIM:
		{
			tagActiveCursorEvent e( _T("Event_CursorActive"), this );
			e.eCursor = ECAN_Chisel;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent( &e );
		}
		break;
	case EISF_ProtectSign:
		{
			tagSpecItemEvent e(_T("tagSpecItemEvent"), NULL);
			e.n64Serial = pItem->GetItemId();
			e.eSpecFuncSrc = pItem->GetItemSpecFunc();
			m_pFrameMgr->SendEvent(&e);
		}
		break;
	case EISF_PerfectGrow:
	case EISF_XiStarIM:
		{
			tagActiveCursorEvent e(_T("Event_CursorActive"), this);
			e.eCursor = ECAN_EquipPerfectGrow;
			e.dwParam = pItem->GetPos();
			m_pFrameMgr->SendEvent(&e);

		}
		break;
	case EISF_HolyStrength:
		{
			tagNC_HolyStrength cmd;
			cmd.n64ItemID = pItem->GetItemId();
			m_pSession->Send(&cmd);

// 			tagActiveCursorEvent e(_T("Event_CursorActive"), this);
// 			e.eCursor = ECAN_HolyStrength;
// 			e.dwParam = pItem->GetPos();
// 			m_pFrameMgr->SendEvent(&e);
		}
		break;
	case EISF_HolyEquipmentNumber:
		{
			tagNC_HolyEquipmentNumber cmd;
			cmd.n64ItemID=pItem->GetItemId();
			m_pSession->Send(&cmd);
		}
		break;
// 	case EISF_EquipReduceIM:	//这个不要了
// 		{
// 			tagActiveCursorEvent e(_T("Event_CursorActive"), this);
// 			e.eCursor = ECAN_ReduceStar;
// 			e.dwParam = pItem->GetPos();
// 			m_pFrameMgr->SendEvent(&e);
// 		}

	}
	
}

DWORD ItemFrame::OnOpenPackage( tagGameEvent* pGameEvent )
{
	if(!m_bCanUseBag)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BagCanNotUsed")]);
		return 0;
	}

	if(RoleMgr::Inst()->GetLocalPlayer()->GetRoleStateEx(ERSE_BagPsdPass))
	{
		//MallMgr::Inst()->OpenEasyBuyFrame( _T("World"), _T("PocketEasyBuyFrame"), "\\desktop\\bagback", EBUT_BagUI, "OuterLeftTop","pocketui" );
		m_pPocket->ShowWnd(TRUE);
	}
	else
	{
		if(P_VALID(GetObj("OpenBagPsd")))
			KillObj("OpenBagPsd");

		CreateObj("OpenBagPsd", "MsgInputBox");
		TObjRef<MsgInputBox>("OpenBagPsd")->Init(
			_T(""),g_StrTable[_T("PocketEncrypt_TextCurrentPassword")],
			_T("MsgBox_OpenBagPsd"),GT_INVALID, MBIT_Password);

	}
	return 0;
}

DWORD ItemFrame::OnClosePackage( tagGameEvent* pGameEvent )
{
	m_pPocket->ShowWnd(FALSE);
	return 0;
}

DWORD ItemFrame::OnTheRoleOfItem( tagOnTheRoleOfItemEvent* pGameEvent )
{
	switch (pGameEvent->eSpecFuncSrc)
	{
	case EISF_IdetifyEquip:
		{
			//--只有装备才可以被鉴定
			if(MIsEquipment(pGameEvent->dwTypeIDDst))
			{
				//--鉴定卷和所要鉴定的装备都应该在背包中
				if(pGameEvent->eTypeSrc == EICT_Bag &&
					pGameEvent->eTypeDst == EICT_Bag)
				{
					Item* pItemSrc = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeSrc, pGameEvent->n16IndexSrc);
					Item* pItemDst = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);
					//--确定找到两物品
					ASSERT(P_VALID(pItemSrc) && P_VALID(pItemDst));
					//--两个物品的TypeID相符
					ASSERT(pItemSrc->GetItemTypeID() == pGameEvent->dwTypeIDSrc && pItemDst->GetItemTypeID() == pGameEvent->dwTypeIDDst);
					
					if(EISF_IdetifyEquip == pItemSrc->GetItemSpecFunc()
						&& ((Equipment*)pItemDst)->IsNotIdetified())
					{
						ShowUseItemCheckDlg(pItemSrc, pItemDst);
					}
				}
			}
		}
		break;
	case EISF_Grind:
		{
			// 先需要判断是否是装备，因为只有武器才有崭新，武器属于装备
			if(MIsEquipment(pGameEvent->dwTypeIDDst))
			{
				//--磨石应该在背包中,要打磨的装备在背包或装备栏中
				if(pGameEvent->eTypeSrc == EICT_Bag &&
					(pGameEvent->eTypeDst == EICT_Bag || pGameEvent->eTypeDst == EICT_Equip))
				{
					Item* pItemSrc = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeSrc, pGameEvent->n16IndexSrc);
					Item* pItemDst = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);
					//--确定找到两物品
					ASSERT(P_VALID(pItemSrc) && P_VALID(pItemDst));
					//--两个物品的TypeID相符
					ASSERT(pItemSrc->GetItemTypeID() == pGameEvent->dwTypeIDSrc && pItemDst->GetItemTypeID() == pGameEvent->dwTypeIDDst);
					
					if(EISF_Grind == pItemSrc->GetItemSpecFunc()
						&& pItemDst->GetProto()->eType == EIT_Weapon)
					{
						ShowUseItemCheckDlg(pItemSrc, pItemDst);
					}
				}
			}
		}
		break;
    case EISF_Dye:
        {
            // 获得相关物品
            Item* pItemSrc = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeSrc, pGameEvent->n16IndexSrc);
            Equipment* pFashionDst = static_cast<Equipment*>(ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst));
            ASSERT(P_VALID(pItemSrc) && P_VALID(pFashionDst));

            // 判断是否是时装
            if (pFashionDst->GetProto()->eType != EIT_Fashion)
            {
                CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DyeFashionErr1")]);
            }
            else
            {
                // 如果此时装已有同样的颜色则不需要染
                if (pItemSrc->GetProto()->nSpecFuncVal1 == pFashionDst->GetEquip()->n8ColorID)
                {
                    CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DyeFashionErr2")]);
                }
                else
                {
                    // 判断等级
                    if (pItemSrc->GetItemLevel() < pFashionDst->GetItemLevel() / 10)
                    {
                        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DyeFashionErr3")]);
                    }
                    else
                    {
                        ShowUseItemCheckDlg(pItemSrc, pFashionDst);
                    }
                }
            }
        }
        break;
	case EISF_EquipAddPotValue:
		{
			// 先需要判断是否是装备,只有装备才可以提升潜力
			if(MIsEquipment(pGameEvent->dwTypeIDDst))
			{
				//--道具和装备是否都在包里
				if(pGameEvent->eTypeSrc == EICT_Bag &&
					pGameEvent->eTypeDst == EICT_Bag)
				{
					Item* pItemSrc = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeSrc, pGameEvent->n16IndexSrc);
					Item* pItemDst = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);
					//--确定找到两物品
					ASSERT(P_VALID(pItemSrc) && P_VALID(pItemDst));
					//--两个物品的TypeID相符
					ASSERT(pItemSrc->GetItemTypeID() == pGameEvent->dwTypeIDSrc && pItemDst->GetItemTypeID() == pGameEvent->dwTypeIDDst);

					Equipment *pEquip = (Equipment*)pItemDst;
					if(P_VALID(pEquip))
					{
						INT32 nCurPot = pEquip->GetEquipPotVal();	// 当前可用潜力值
						INT32 nMaxPot = pEquip->GetEquipPotMax();
						if( nCurPot >= nMaxPot )
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipPotIsMax")]);
							return 0;
						}
						// 潜力值可提升次数为0
						if( 0 == pEquip->GetEquipPotAdvanceTime() )
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipNoAddPotTime")]);
							return 0;
						}

						INT32 nAddPot = pItemSrc->GetProto()->nSpecFuncVal2;	// 可提升的潜力值
						if( nCurPot + nAddPot > nMaxPot )
						{
							INT32 nCanAddPot = nMaxPot - nCurPot;
							if( nCanAddPot > 0 )
							{
								if(P_VALID(GetObj("AddPotTips")))
									KillObj("AddPotTips");

								TCHAR szTemp[X_LONG_NAME] = {0};
								_sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("AddPotValue")], nAddPot, nCanAddPot );
								CreateObj("AddPotTips", "MsgBox");
								TObjRef<MsgBox>("AddPotTips")->Init( _T(""), szTemp, _T("MsgBox_AddPotTips"), MsgBoxFlag(MBF_OK | MBF_Cancel), TRUE );
								m_pOperSrcItem = pItemSrc;
								m_n64OperSrcItem = pItemSrc->GetItemId();
								m_pOperDstItem = pItemDst;
							}
						}
						else
						{
							if( EISF_EquipAddPotValue == pItemSrc->GetItemSpecFunc()
								&& ( pItemDst->GetProto()->eType == EIT_Weapon || pItemDst->GetProto()->eType == EIT_ClothArmor
									 || pItemDst->GetProto()->eType == EIT_Decoration || pItemDst->GetProto()->eType == EIT_Armor ) )
							{
								ShowUseItemCheckDlg(pItemSrc, pItemDst);
							}
						}
					}
				}
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemCanNotAddPot")]);
			}
		}
		break;
	case EISF_BeGoldStone:
		{
			//--道具和装备是否都在包里
			if(pGameEvent->eTypeSrc == EICT_Bag &&
				pGameEvent->eTypeDst == EICT_Bag)
			{
				Item* pItemSrc = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeSrc, pGameEvent->n16IndexSrc);
				Item* pItemDst = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);
				//--确定找到两物品
				ASSERT(P_VALID(pItemSrc) && P_VALID(pItemDst));
				//--两个物品的TypeID相符
				ASSERT(pItemSrc->GetItemTypeID() == pGameEvent->dwTypeIDSrc && pItemDst->GetItemTypeID() == pGameEvent->dwTypeIDDst);
				DWORD dwGoldStoneType = pItemSrc->GetProto()->nSpecFuncVal1;
				//if( ItemMgr::Inst()->IsInGoldStoneTable( pItemDst->GetItemTypeID(), dwGoldStoneType ) )
				//{
				if( EISF_BeGoldStone == pItemSrc->GetItemSpecFunc() )
				{
					ShowUseItemCheckDlg(pItemSrc, pItemDst);
				}
				//}
				//else
				//{
				//	if( dwGoldStoneType == 1 )
				//	{
				//		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotUseGoldStone")]);
				//	}
				//	else if( dwGoldStoneType == 2 )
				//	{
				//		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotUseMoreGoldStone")]);
				//	}
				//}
			}
		}
		break;
	case EISF_EquipSignature:
		{
			ProcessEquipSingnature( pGameEvent );
		}
		break;
	case EISF_UpgradeEquipMaxPotValue:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;

			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}

			if( pItem->GetProto()->nSpecFuncVal1 != GT_INVALID && pEquip->GetEquipPos() != pItem->GetProto()->nSpecFuncVal1 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoUsefulOnThisKindOfEquip")]);
				return 0;
			}

			ShowUseItemCheckDlg(pItem, pEquip);
		}
		break;
	case EISF_XiHun:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;

			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}
			const tagEquipSpec *pEquipSpec = pEquip->GetEquip();
			const tagItemProto *pEquipPro = pEquip->GetProto();
			const tagItemProto *pItemProto = pItem->GetProto();
			if( !P_VALID(pEquipSpec) || !P_VALID(pEquipPro) )
				return 0;

			if( pEquipPro->eType == EIT_FaBao || pEquipPro->eType == EIT_Fashion || pEquip->IsCanXiHun() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ActionNotSuitable")]);
				return 0;
			}

			if( pEquipSpec->byConsolidateLevel == 0 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipStrengLevelIsZero")]);
				return 0;
			}

			if( pEquipSpec->byConsolidateLevel > pItemProto->nSpecFuncVal1 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("XiHunStoneLevelTooLow")]);
				return 0;
			}

			ShowUseItemCheckDlg(pItem, pEquip);
		}
		break;
	case EISF_FuHun:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;
			const tagItemProto *pItemProto = pItem->GetProto();
			if(!P_VALID(pItemProto))
				return 0;
			if( pItemProto->nSpecFuncVal1 == 0 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FuHunStoneNoSoul")]);
				return 0;
			}

			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}
			const tagEquipSpec *pEquipSpec = pEquip->GetEquip();
			const tagItemProto *pEquipPro = pEquip->GetProto();
			if( !P_VALID(pEquipSpec) || !P_VALID(pEquipPro) )
				return 0;

			if( pEquipPro->eType == EIT_FaBao || pEquipPro->eType == EIT_Fashion )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ActionNotSuitable")]);
				return 0;
			}

			if( pEquipSpec->byGodStrengthenLevel == 0 && pItemProto->nSpecFuncVal1 >= 21 && pItemProto->nSpecFuncVal1 <= 30 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotFuHunToNoShenZhu")]);
				return 0;
			}

			if( pItemProto->nSpecFuncVal1 - 20 > pEquipSpec->byGodStrengthenLevel )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGodStrengthenLevelIsTooLow")]);
				return 0;
			}

			if( pEquipSpec->byConsolidateLevel >= pItemProto->nSpecFuncVal1 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FuHunStoneLevelTooLow")]);
				return 0;
			}

			INT nQlty = pEquip->GetItemQuality();
			const tagEquipQltyEffect* pAtt = ItemProtoData::Inst()->FindEquipQltyEffect(nQlty);
			if( (P_VALID(pAtt) && pItemProto->nSpecFuncVal1 > pAtt->nMaxStrengthenLevel + pEquipSpec->byGodStrengthenLevel) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipMaxStrLevelTooLow")]);
				return FALSE;
			}

			TCHAR szTmp[X_LONG_NAME];
			if(P_VALID(GetObj("UseItemCheck")))
				KillObj("UseItemCheck");
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("FuHun_Check")], pEquip->GetItemName().c_str(), pItem->GetItemName().c_str());
			CreateObj("UseItemCheck", "MsgBox");
			TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_UseItemCheck"));
			m_pOperSrcItem = pItem;
			m_n64OperSrcItem = pItem->GetItemId();
			m_pOperDstItem = pEquip;
		}
		break;
	case EISF_GuiYuan:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;
			
			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}
			const tagEquipSpec *pEquipSpec = pEquip->GetEquip();
			if( !P_VALID(pEquipSpec) )
				return 0;

			if( pEquipSpec->byBrandLevel == 0 || pEquipSpec->byBrandLevel == GT_INVALID )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotGuiYuan")]);
				return 0;
			}

			TCHAR szTmp[X_LONG_NAME];
			if(P_VALID(GetObj("UseItemCheck")))
				KillObj("UseItemCheck");

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuiYuan_Check")], pEquip->GetItemName().c_str(), pItem->GetItemName().c_str());
			CreateObj("UseItemCheck", "MsgBox");
			TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_UseItemCheck"));
			m_pOperSrcItem = pItem;
			m_n64OperSrcItem = pItem->GetItemId();
			m_pOperDstItem = pEquip;
		}
		break;
	case EISF_FairyPowder:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;
			const tagItemProto *pItemProto = pItem->GetProto();
			if(!P_VALID(pItemProto))
				return 0;

			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}
			const tagFabaoSpec *pEquipSpec = pEquip->GetFaBaoSpe();
			const tagFabaoProto *pEquipProto = (tagFabaoProto*)pEquip->GetProto();
			if( !P_VALID(pEquipSpec) || !P_VALID(pEquipProto) )
				return 0;

			if( pEquipProto->eType != EIT_FaBao )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithYaoJing")]);
				return 0;
			}
	
			if( pItemProto->nSpecFuncVal2 != GT_INVALID && pEquipProto->eFarbaoType != pItemProto->nSpecFuncVal2 && pItemProto->byLevel != 1 )
			{
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf( szTemp, X_LONG_NAME, _T("TrumpType_%d"), pItemProto->nSpecFuncVal2 );
				TCHAR szInfo[X_LONG_NAME] = {0};
				_sntprintf( szInfo, X_LONG_NAME, g_StrTable[_T("FairyMiaoType")], g_StrTable[szTemp] );
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szInfo);
				return 0;
			}

			if( pItemProto->byLevel != 1 && pEquipProto->byLevel != pItemProto->byLevel )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyLevelLow")]);
				return 0;
			}
	
			//if( pEquipSpec->n16NativeIntelligence >= pItemProto->nSpecFuncVal2 )
			//{
			//	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QualityIsTop")]);
			//	return 0;
			//}

			tagNC_FairyPowder msg;
			msg.n64Fairy = pEquip->GetItemId(); 
			msg.n64FairyPowder = pItem->GetItemId();
			m_pSession->Send(&msg);
		}
		break;
	case EISF_FairyRaiseIntelligenceMax:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;
			const tagItemProto *pItemProto = pItem->GetProto();
			if(!P_VALID(pItemProto))
				return 0;

			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}
			const tagFabaoSpec *pEquipSpec = pEquip->GetFaBaoSpe();
			const tagFabaoProto *pEquipProto = (tagFabaoProto*)pEquip->GetProto();
			if( !P_VALID(pEquipSpec) || !P_VALID(pEquipProto) )
				return 0;

			if( pEquipProto->eType != EIT_FaBao )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithYaoJing")]);
				return 0;
			}

			if( pItemProto->nSpecFuncVal2 != GT_INVALID && pEquipProto->eFarbaoType != pItemProto->nSpecFuncVal2 && pItemProto->byLevel != 1 )
			{
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf( szTemp, X_LONG_NAME, _T("TrumpType_%d"), pItemProto->nSpecFuncVal2 );
				TCHAR szInfo[X_LONG_NAME] = {0};
				_sntprintf( szInfo, X_LONG_NAME, g_StrTable[_T("FairyMiaoType")], g_StrTable[szTemp] );
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szInfo);
				return 0;
			}

			if( pItemProto->byLevel != 1 && pEquipProto->byLevel != pItemProto->byLevel )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyLevelLow")]);
				return 0;
			}


			TCHAR szTmp[X_LONG_NAME];
			if(P_VALID(GetObj("UseItemCheck")))
				KillObj("UseItemCheck");

			//if(pItemProto->nSpecFuncVal2 == 2)
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Sprite_zizhitisheng130")], pEquip->GetItemName().c_str(), pItem->GetItemName().c_str());
// 			else
// 				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("ReSetFairyQuaity")], pEquip->GetItemName().c_str(), pItem->GetItemName().c_str());
			CreateObj("UseItemCheck", "MsgBox");
			TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_UseItemCheck"));
			m_pOperSrcItem = pItem;
			m_n64OperSrcItem = pItem->GetItemId();
			m_pOperDstItem = pEquip;
		}
		break;
	case EISF_FairyDoRaiseIntelligence:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;
			const tagItemProto *pItemProto = pItem->GetProto();
			if(!P_VALID(pItemProto))
				return 0;

			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}
			const tagFabaoSpec *pEquipSpec = pEquip->GetFaBaoSpe();
			const tagFabaoProto *pEquipProto = (tagFabaoProto*)pEquip->GetProto();
			if( !P_VALID(pEquipSpec) || !P_VALID(pEquipProto) )
				return 0;

			if( pEquipProto->eType != EIT_FaBao )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithYaoJing")]);
				return 0;
			}

			if( pItemProto->nSpecFuncVal2 != GT_INVALID && pEquipProto->eFarbaoType != pItemProto->nSpecFuncVal2 && pItemProto->byLevel != 1 )
			{
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf( szTemp, X_LONG_NAME, _T("TrumpType_%d"), pItemProto->nSpecFuncVal2 );
				TCHAR szInfo[X_LONG_NAME] = {0};
				_sntprintf( szInfo, X_LONG_NAME, g_StrTable[_T("FairyMiaoType")], g_StrTable[szTemp] );
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szInfo);
				return 0;
			}

			if( pItemProto->byLevel != 1 && pEquipProto->byLevel != pItemProto->byLevel )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyLevelLow")]);
				return 0;
			}

			tagNC_FairyDoRaiseIntelligence cmd;
			cmd.n64Fairy = pEquip->GetItemId();
			cmd.n64FairyPowder = pItem->GetItemId();
			m_pSession->Send(&cmd);
		}
		break;
	case EISF_DiamondEquipGrowIM:
		{
			//不是背包里的不行
			if( pGameEvent->eTypeSrc!=EICT_Bag || (pGameEvent->eTypeDst!=EICT_Bag ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithBagItem")]);
				return 0;
			}

			if( pGameEvent->eTypeDst == EICT_Equip )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutEquipToBag")]);
				return 0;
			}
			//判断是否有该物品和装备
			Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
			if(!P_VALID(pItem))
				return 0;

			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);

			if (!P_VALID(pEquip))
				return 0;

			if( !MIsEquipment(pEquip->GetItemTypeID()) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyUsefulWithEquip")]);
				return 0;
			}

			if( pEquip->IsNotIdetified() )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyEffectOnValidEquip")]);
				return 0;
			}
			const tagEquipSpec *pEquipSpec = pEquip->GetEquip();
			if( !P_VALID(pEquipSpec) )
				return 0;

			TCHAR szTmp[X_LONG_NAME];
			if(P_VALID(GetObj("UseItemCheck")))
				KillObj("UseItemCheck");

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("UseItem_Check01")], pEquip->GetItemName().c_str(), pItem->GetItemName().c_str());
			CreateObj("UseItemCheck", "MsgBox");
			TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_UseItemCheck"));
			m_pOperSrcItem = pItem;
			m_n64OperSrcItem = pItem->GetItemId();
			m_pOperDstItem = pEquip;
		}
		break;
	default:
		{
			Item* pItemSrc = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeSrc, pGameEvent->n16IndexSrc);
			Item* pItemDst = ItemMgr::Inst()->GetConItem(pGameEvent->eTypeDst, pGameEvent->n16IndexDst);
			//--确定找到两物品
			ASSERT(P_VALID(pItemSrc) && P_VALID(pItemDst));
			ShowUseItemCheckDlg(pItemSrc, pItemDst);
		}
		break;
	}
	return 0;
}

DWORD ItemFrame::OnRolePutDownItem( tagPutDownItemEvent* pGameEvent )
{
	if(EDT_Item == pGameEvent->eType || EDT_WarBag == pGameEvent->eType)
	{
		INT16 n16Pos = (INT16)LOWORD(pGameEvent->dwParam);
		Item* pItem = NULL;
		if(EDT_Item == pGameEvent->eType)
			pItem = ItemMgr::Inst()->GetPocketItem(n16Pos);
		else if(EDT_WarBag == pGameEvent->eType)
			pItem = ItemMgr::Inst()->GetWarBag()->GetValue(n16Pos);
		if(P_VALID(pItem) && pGameEvent->dwTypeID == pItem->GetItemTypeID())
		{
			if (pItem->GetItemSpecFunc() == EISF_TreasureSeizer)
			{
				// 夺宝奇兵的宝物不可丢弃
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CantDrop")]);
				return 0;
			}

			//判断是否可丢弃
			if(pItem->IsDroppable())
			{
				TCHAR szTmp[X_LONG_NAME] = {0};
				BOOL bSendOnly = TRUE; //标志位，是否可以直接发送丢弃消息
				
				if(!pItem->IsTradeable())//判断是否可交易，不可交易的物品需要提示是否删除该物品
				{	
					tstringstream tss;
					if ( pItem->GetItemQuality()>=0 || pItem->GetItemQuality()<=6 )
						tss<<_T("RoleDeleteItem_Check")<<pItem->GetItemQuality();
					else
						tss<<_T("RoleDeleteItem_Check");
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[tss.str().c_str()], pItem->GetDisplay()->szName, pItem->GetItemQuantity());
					bSendOnly = FALSE;
				}
				else if(pItem->GetItemQuality() > 0)//判断物品评级，白品以上都提示
				{
					tstringstream tss;
					if ( pItem->GetItemQuality()<=6 )
						tss<<_T("RolePushDownItem_Check")<<pItem->GetItemQuality();
					else
						tss<<_T("RolePushDownItem_Check");
					_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[tss.str().c_str()], pItem->GetDisplay()->szName, pItem->GetItemQuantity());
					bSendOnly = FALSE;
				}

				if(bSendOnly)
				{
					SendPutDownItem(pItem->GetItemId());
				}
				else//需要弹出对话框
				{
					if(P_VALID(GetObj("UseItemCheck")))
						KillObj("UseItemCheck");

					CreateObj("UseItemCheck", "MsgBox");
					TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_PutDownCheck"));
					m_pOperSrcItem = pItem;
                    m_n64OperSrcItem = pItem->GetItemId();

					INT64 n64ItemSerial = pItem->GetItemId();
					tagEquipDropEvent event(_T("EquipDrop"), this);
					event.n64ItemID = n64ItemSerial;
					m_pMgr->SendEvent(&event);
				}
			}
			else
			{
				// 说明不可丢弃的原因
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CantDrop")]);
			}
		}
	}
	return 0;
}

DWORD ItemFrame::OnPutDownItemCheck( tagMsgBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult)
	{
		ASSERT(P_VALID(m_pOperSrcItem));
		if(P_VALID(m_pOperSrcItem))
		{
			SendPutDownItem(m_pOperSrcItem->GetItemId());
		}
	}

	m_pOperSrcItem = NULL;
    m_n64OperSrcItem = GT_INVALID;

	return 0;
}

VOID ItemFrame::SendPutDownItem( INT64 n64ItemSerial )
{
	tagNC_RolePutDownItem m;
	m.n64Serial = n64ItemSerial;
	m_pSession->Send(&m);
}


DWORD ItemFrame::OnOpenWare( tagGameEvent* pGameEvent )
{
	// 打开仓库界面
	StorageFrame* pFrame = (StorageFrame*)m_pFrameMgr->GetFrame(_T("Storage"));
	if (!P_VALID(pFrame))
	{
		pFrame = (StorageFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("Storage"), _T("StorageFrame"), 0);
		if(P_VALID(pFrame))
			this->SetCurStrategy(EIST_ItemStorage);
	}
	MallMgr::Inst()->OpenEasyBuyFrame( _T("World"), _T("StorageEasyBuyFrame"), "\\desktop\\wh_win", EBUT_storageUI, "OuterRightTop", "storageui" );
	
	//打开仓库同时打开背包
	m_pMgr->SendEvent( &tagGameEvent( _T("ForceOpen_Package_UI"), this ) );
	
	return 0;
}

DWORD ItemFrame::OnCloseWare( tagGameEvent* pGameEvent )
{
	// 关闭仓库窗口
	GameFrame* pFrame = m_pFrameMgr->GetFrame(_T("Storage"));
	if(P_VALID(pFrame))
	{
		m_pFrameMgr->AddToDestroyList(pFrame);
		m_eCurStrategy = EIST_ItemPocket;
	}
	return 0;
}

DWORD ItemFrame::OnCloseWareNow( tagGameEvent* pGameEvent )
{
	// 关闭仓库窗口
	GameFrame* pFrame = m_pFrameMgr->GetFrame(_T("Storage"));
	if(P_VALID(pFrame))
	{
		m_pFrameMgr->AddToDestroyList(pFrame);
		m_eCurStrategy = EIST_ItemPocket;
	}
	return 0;
}

VOID ItemFrame::ShowUseItemCheckDlg( Item* pSrcItem, Item* pDstItem /*= NULL*/ )
{
	if(!P_VALID(pSrcItem))
		return;

	TCHAR szTmp[X_LONG_NAME];

	if(P_VALID(GetObj("UseItemCheck")))
		KillObj("UseItemCheck");
	
	if(P_VALID(pDstItem))
	{
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("UseItem_Check01")], pDstItem->GetItemName().c_str(), pSrcItem->GetItemName().c_str());
	}
	else
	{
		INT nQuality = pSrcItem->GetItemQuality();
		if ( nQuality < 0 || nQuality > 6 )
		{
			nQuality = 255;
		}
		tstringstream tss;
		tss<<_T("UseItem_Check")<<nQuality;
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[tss.str().c_str()], pSrcItem->GetItemName().c_str());
	}
	CreateObj("UseItemCheck", "MsgBox");
	TObjRef<MsgBox>("UseItemCheck")->Init(_T(""), szTmp, _T("MsgBox_UseItemCheck"));
	m_pOperSrcItem = pSrcItem;
    m_n64OperSrcItem = pSrcItem->GetItemId();
	m_pOperDstItem = pDstItem;
}

DWORD ItemFrame::OnUseSpecItemCheck( tagMsgBoxEvent* pGameEvent )
{
    if (!P_VALID(ItemMgr::Inst()->GetConItemByID(EICT_Bag, m_n64OperSrcItem)))
        return 0;

	if(!P_VALID(m_pOperSrcItem))
		return 0;

	if(MBF_OK == pGameEvent->eResult)
	{
		switch(m_pOperSrcItem->GetItemSpecFunc())
		{
		case EISF_IdetifyEquip:// 鉴定卷
			{
				if(!P_VALID(m_pOperDstItem))
					return 0;

				tagNC_IdentifyEquip cmd;
				cmd.n64SerialReel = m_pOperSrcItem->GetItemId();
				cmd.n64SerialEquip = m_pOperDstItem->GetItemId();
				m_pSession->Send(&cmd);
			}
			break;
		case EISF_Grind:// 磨石
			{
				if(!P_VALID(m_pOperDstItem))
					return 0;

				tagNC_Abrase cmd;
				cmd.n64AbraserSerial = m_pOperSrcItem->GetItemId();
				cmd.n64WeaponSerial	 = m_pOperDstItem->GetItemId();
				m_pSession->Send(&cmd);
			}
			break;
		case EISF_Dye:// 染料
			{
				if(!P_VALID(m_pOperDstItem))
					return 0;

				// 发送染色请求
				tagNC_DyeFashion msg;
				msg.n64DyeSN        =   m_pOperSrcItem->GetItemId();
				msg.n64EquipSerial  =   m_pOperDstItem->GetItemId();
				m_pSession->Send(&msg);
			}
			break;
		case EISF_RemoveAttPt:// 洗属性
			{
				tagNC_RoleClearAttPoint m;
				m.n64ItemID = m_pOperSrcItem->GetItemId();
				m_pSession->Send(&m);
			}
			break;
		case EISF_RemoveTalentPt:// 洗技能
			{
				tagNC_ClearTalent m;
				m.n64ItemID = m_pOperSrcItem->GetItemId();
				m.eType = ETT_Blade;
				m_pSession->Send(&m);
			}
			break;
		case EISF_LearnSkill:// 学技能
			{
				OnUseItem(m_pOperSrcItem);
			}
			break;
		case EISF_PetEgg:// 宠物蛋
			{
				tagUsePetEggEvent e(_T("tagUsePetEggEvent"), this);
				e.n64ItemID      =   m_pOperSrcItem->GetItemId();
				m_pFrameMgr->SendEvent(&e);
			}
			break;
        case EISF_PetSkillBook: // 宠物技能书
            {
                DWORD dwPetID = PetManager::Inst()->GetCurrPetID();
                if (dwPetID != GT_INVALID)
                {
                    PetManager::Inst()->BeginNS_PetLearnSkill(dwPetID, m_pOperSrcItem->GetItemId());
                }
            }
            break;
		case EISF_HoleGem:// 宝石	
		case EISF_Brand:// 烙印
		case EISF_LongHunShi:// 龙附石
		case EISF_Chisel:// 凿具
		case EISF_NewChisel://新凿石
		case EISF_RockGod:	// 神石
			{
				if(!P_VALID(m_pOperDstItem))
					return 0;
						
				const tagEquipProto* pEquipProto=ItemProtoData::Inst()->FindEquipProto(m_pOperDstItem->GetItemTypeID());//add by xp
				if(!P_VALID(pEquipProto))
					return 0;
				if(pEquipProto->bOnlyScriptStrengthen==true)
					return 0;//add end

				tagOnTheRoleOfItemEvent e(_T("ItemEvent_ConsolidateItem"), NULL);
				e.eTypeSrc = m_pOperSrcItem->GetConType();
				e.n16IndexSrc = m_pOperSrcItem->GetPos();
				e.dwTypeIDSrc = m_pOperSrcItem->GetItemTypeID();
				e.eSpecFuncSrc = m_pOperSrcItem->GetItemSpecFunc();
				e.eTypeDst = m_pOperDstItem->GetConType();
				e.n16IndexDst = m_pOperDstItem->GetPos();
				e.dwTypeIDDst = m_pOperDstItem->GetItemTypeID();
				m_pFrameMgr->SendEvent(&e);
			}
			break;
		case EISF_EquipAddPotValue: // 提升装备潜力
			{
				InsuranceFrame* pFrame = (InsuranceFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Insurance"));
				if( P_VALID(pFrame) )
				{
					pFrame->ShowFrame(TRUE, EISF_EquipAddPotValue, m_pOperSrcItem->GetItemId(), m_pOperDstItem->GetItemId());
					SetStrategy(EIST_ItemInsurance);
				}
			}
		case EISF_ChestKey:// 宝箱钥匙
			{
				if(!P_VALID(m_pOperDstItem))
					return 0;
				
				tagOnTheRoleOfItemEvent e(_T("ItemEvent_UnlockChest"), NULL);
				e.eTypeSrc = m_pOperSrcItem->GetConType();
				e.n16IndexSrc = m_pOperSrcItem->GetPos();
				e.dwTypeIDSrc = m_pOperSrcItem->GetItemTypeID();
				e.eSpecFuncSrc = m_pOperSrcItem->GetItemSpecFunc();
				e.eTypeDst = m_pOperDstItem->GetConType();
				e.n16IndexDst = m_pOperDstItem->GetPos();
				e.dwTypeIDDst = m_pOperDstItem->GetItemTypeID();
				m_pFrameMgr->SendEvent(&e);
			}
			break;
		case EISF_BeGoldStone:	// 点金石
			{
				if(!P_VALID(m_pOperDstItem))
					return 0;

				tagNC_BeGoldStone cmd;
				cmd.n64SrcItemID = m_pOperSrcItem->GetItemId();
				cmd.n64DstItemID = m_pOperDstItem->GetItemId();
				m_pSession->Send(&cmd);
			}
			break;
		case EISF_RenameItem: // 易名贴
			{
				RenameFrame* pFrame = (RenameFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Rename"));
				if( !P_VALID(pFrame) )
					pFrame = (RenameFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("Rename"), _T("RenameFrame"), 0 );
				if( P_VALID(pFrame) )
					pFrame->ShowFrame( TRUE, m_pOperSrcItem->GetItemId() );
			}
			break;
        case EISF_PetPocketExtItem:
            {
                if (!P_VALID(m_pOperSrcItem))
                    return 0;

               /* tagNC_ExtendPetPocket msg;
                msg.n64ItemID = m_pOperSrcItem->GetItemId();
                m_pSession->Send(&msg);*/
            }
            break;
		case EISF_ItemLock:
			{
				if( !P_VALID(m_pOperSrcItem) || !P_VALID(m_pOperDstItem) )
					break;
				
				if( !MIsEquipment( m_pOperDstItem->GetItemTypeID() ) && m_pOperDstItem->GetItemSpecFunc() != EISF_HoleGem )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemLock_TargetIsNotEquipment")] );
					break;
				}
				if( m_pOperDstItem->IsIMLocked() )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("ItemLock_HaveLocked")] );
					break;
				}

				tagNC_LockItem cmd;
				cmd.n64Item = m_pOperSrcItem->GetItemId();
				cmd.n64Target = m_pOperDstItem->GetItemId();
				m_pSession->Send( &cmd );
			}
			break;
		case EISF_ItemUnLock:
			{
				if( !P_VALID(m_pOperSrcItem) || !P_VALID(m_pOperDstItem) )
					break;

				if( !MIsEquipment( m_pOperDstItem->GetItemTypeID() ) && m_pOperDstItem->GetItemSpecFunc() != EISF_HoleGem )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("ItemUnLock_TargetIsNotEquipment")] );
					break;
				}

				if( !m_pOperDstItem->IsIMLocked() )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("ItemUnLock_NoLocked")] );
					break;
				}
				
				tagNC_LockItem cmd;
				cmd.n64Item = m_pOperSrcItem->GetItemId();
				cmd.n64Target = m_pOperDstItem->GetItemId();
				m_pSession->Send( &cmd );
			}
			break;
		case EISF_LoadScript:
			{
				if (!P_VALID(m_pOperSrcItem))
					return 0;

				tagSpecItemEvent e(_T("tagSpecItemEvent"), NULL);
				e.n64Serial = m_pOperSrcItem->GetItemId();
				e.eSpecFuncSrc = m_pOperSrcItem->GetItemSpecFunc();
				m_pFrameMgr->SendEvent(&e);
			}
			break;
		case EISF_GuildChangeName:
			{
				tagSpecItemEvent e(_T("tagSpecItemEvent"), NULL);
				e.n64Serial = m_pOperSrcItem->GetItemId();
				e.eSpecFuncSrc = m_pOperSrcItem->GetItemSpecFunc();
				m_pFrameMgr->SendEvent(&e);
			}
			break;
		case EISF_UpgradeEquipMaxPotValue:	// 提升装备潜力上限的道具
			{
				tagNC_RaiseEquipPotval chisel;
				chisel.n64ItemID = m_pOperSrcItem->GetItemId();
				chisel.n64EquipID = m_pOperDstItem->GetItemId();
				m_pSession->Send(&chisel);
			}
			break;
		case EISF_XiHun:	// 吸魂
			{
				PlayerTradeFrame *pFrame = (PlayerTradeFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("PlayerTrade") );
				if( P_VALID(pFrame) && pFrame->IsTrading() )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CantXIHUNwhenInTrading")]);
					break;
				}

				StallShelfFrame* pStall = (StallShelfFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("StallShelf"));
				if (P_VALID(pStall))
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CantXIHUNwhenInStalling")]);
					break;
				}

				tagNC_DetachOrAttackSoul_Equip msg;
				msg.bDetachOrAttack = TRUE;
				msg.n64Stone = m_pOperSrcItem->GetItemId();
				msg.n64Equip = m_pOperDstItem->GetItemId();
				m_pSession->Send(&msg);
			}
			break;
		case EISF_FuHun:	// 附魂
			{
				PlayerTradeFrame *pFrame = (PlayerTradeFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("PlayerTrade") );
				if( P_VALID(pFrame) && pFrame->IsTrading() )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CantFUHUNwhenInTrading")]);
					break;
				}

				StallShelfFrame* pStall = (StallShelfFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("StallShelf"));
				if (P_VALID(pStall))
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CantFUHUNwhenInStalling")]);
					break;
				}

				tagNC_DetachOrAttackSoul_Equip msg;
				msg.bDetachOrAttack = FALSE;
				msg.n64Stone = m_pOperSrcItem->GetItemId();
				msg.n64Equip = m_pOperDstItem->GetItemId();
				m_pSession->Send(&msg);
			}
			break;
		case EISF_GuiYuan:	// 归元
			{
				tagNC_GuiYuan msg;
				msg.n64Stone = m_pOperSrcItem->GetItemId();
				msg.n64Equip = m_pOperDstItem->GetItemId();
				m_pSession->Send(&msg);
			}
			break;
		case EISF_FairyPowder:	// 妖精之粉
			{
				tagNC_FairyPowder msg;
				msg.n64Fairy = m_pOperDstItem->GetItemId(); 
				msg.n64FairyPowder = m_pOperSrcItem->GetItemId();
				m_pSession->Send(&msg);
			}
			break;
		case EISF_VipCard:
			{
				tagNC_UseVipCard msg;
				msg.n64ItemID = m_pOperSrcItem->GetItemId();
				m_pSession->Send(&msg);
			}
			break;
		case EISF_TreasureSeizer:
			{
				tagUseItemEvent e(_T("tagUseItemEvent"), this);
				e.dwItemTypeID = m_pOperSrcItem->GetItemTypeID();
				e.n64ItemID = m_pOperSrcItem->GetItemId();
				m_pFrameMgr->SendEvent(&e);
			}
			break;
		case EISF_FairyStuntBook:
			{
				tagNC_FairyStuntBook m;
				m.n64StuntBook = m_pOperSrcItem->GetItemId();
				m_pSession->Send(&m);
			}
			break;
		case EISF_FairyNirVana:
			{
				tagNC_FairyNirvana m;
				m.n64NirvanID = m_pOperSrcItem->GetItemId();
				m_pSession->Send(&m);
			}
			break;
		case EISF_ExpBall:
			{
				tagNC_ExpBallBind2Role m;
				m.n64ExpBallSerialID = m_pOperSrcItem->GetItemId();
				m_pSession->Send(&m);
			}
			break;
		case EISF_FairyRaiseIntelligenceMax:
			{
				tagNC_FairyRaiseIntelligenceMax cmd;
				cmd.n64Fairy = m_pOperDstItem->GetItemId();
				cmd.n64FairyPowder = m_pOperSrcItem->GetItemId();
				m_pSession->Send(&cmd);
			}
			break;
		case EISF_DiamondEquipGrowIM:
			{
				tagNC_DiamondEquipGrowIM cmd;
				cmd.n64ItemID = m_pOperSrcItem->GetItemId();
				cmd.n64EquipID = m_pOperDstItem->GetItemId();
				m_pSession->Send(&cmd);
			}
			break;
		case EISF_PerfectGrow:
			{
				tagNC_EquipPerfectGrow cmd;
				cmd.n64IMSerialID = m_pOperSrcItem->GetItemId();
				cmd.n64EquipID = m_pOperDstItem->GetItemId();
				m_pSession->Send(&cmd);
			}
			break;
		case EISF_XiStarIM:
			{
				tagNC_XiPurpleStar cmd;
				cmd.n64IMID = m_pOperSrcItem->GetItemId();
				cmd.n64EquipID = m_pOperDstItem->GetItemId();
				m_pSession->Send(&cmd);
			}
			break;
// 		case EISF_EquipReduceIM:	//不要了
// 			{
// 				tagNC_EquipGrowDec cmd;
// 				cmd.n64Equip = m_pOperDstItem->GetItemId();
// 				cmd.n64IM = m_pOperSrcItem->GetItemId();
// 				m_pSession->Send(&cmd);
// 			}
		}
	}

	m_pOperSrcItem = NULL;
    m_n64OperSrcItem = GT_INVALID;
	m_pOperDstItem = NULL;
	return 0;
}

DWORD ItemFrame::OnAddPotValue( tagMsgBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult)
	{
		ShowUseItemCheckDlg( m_pOperSrcItem, m_pOperDstItem );
	}
	else
	{
		m_pOperSrcItem = NULL;
		m_n64OperSrcItem = GT_INVALID;
		m_pOperDstItem = NULL;
	}

	return 0;
}

DWORD ItemFrame::OnEquipSignatureInput( tagMsgInputBoxEvent* pGameEvent )
{
	if( pGameEvent->eResult != MBF_OK )
		return 0;

	if( pGameEvent->strInputName.length() > 10 )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_InputTooMuch")] );
		return  0;
	}
	
	tstring strFind = FilterData::Inst()->IsContainFilter( pGameEvent->strInputName );
	if( !strFind.empty() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_CantComtainWord")], strFind.c_str() );
		return  0;
	}

	INT64 n64Src = TObjRef<MsgInputBox>("EquipSignatureInputContentMsgBox")->GetN64UserData();
	INT64 n64Dst = TObjRef<MsgInputBox>("EquipSignatureInputContentMsgBox")->GetN64UserData2();
	Item* pItemSrc = ItemMgr::Inst()->GetPocketItemByID( n64Src );
	Item* pItemDst = ItemMgr::Inst()->GetPocketItemByID( n64Dst );

	if( !P_VALID(pItemSrc) || !P_VALID(pItemDst) || !MIsEquipment(pItemDst->GetItemTypeID()) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_SearchItemError")] );
		return  0;
	}

	tagNC_EquipSignature cmd;
	ZeroMemory( cmd.szContent, sizeof(cmd.szContent) );
	cmd.n64Item = n64Src;
	cmd.n64EquipID = n64Dst;
	_tcsncpy( cmd.szContent, pGameEvent->strInputName.c_str(), sizeof(cmd.szContent)/sizeof(TCHAR) );
	m_pSession->Send( &cmd );

	return 0;
}

VOID ItemFrame::SetCurStrategy( EItemStrategy eType )
{
	// 如果不是设置为背包，则需要将上一次打开的界面关闭
	if(EIST_ItemPocket != eType && m_eCurStrategy != eType)
	{
		switch(m_eCurStrategy)
		{
		case EIST_ItemStorage://仓库
			{
				tagGameEvent event(_T("Close_Ware"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
		case EIST_ItemPlayerTrade://玩家交易
			{
				tagNC_ExchangeCancel m;
				m_pSession->Send(&m);
			}
			break;
		case EIST_ItemShopVend:// 商店售卖
			{
				tagGameEvent event(_T("Close_Shop"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
		case EIST_ItemStall:// 摆摊
			{
				tagGameEvent event(_T("StallEvent_CloseWindow"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
		case EIST_ItemSmithCompose:// 合成
			{
				tagGameEvent event(_T("Close_Compose"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
		case EIST_ItemInsurance:// 保底符
			{
				tagGameEvent event(_T("Close_Insurance"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
        case EIST_GuildStorage: // 帮派仓库
            {
                tagGameEvent event(_T("Close_GuildWare"), this);
                m_pMgr->SendEvent(&event);
            }
            break;
		case EIST_GuildUpgrade: // 帮派升级
			{
				tagGameEvent event(_T("Close_GuildUpgrade"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
        case EIST_FashionSpin:  // 时装精纺
            {
                FashionComposeFrame* pFrame = (FashionComposeFrame*)m_pFrameMgr->GetFrame(_T("FashionComposeFrame"));
                if (P_VALID(pFrame))
                {
                    pFrame->ShowWnd(FALSE);
                }
            }
            break;
		case EIST_TrumpRefine: // 法宝修炼
			{
				//tagGameEvent event(_T("Close_TrumpRefine"), this);
				//m_pMgr->SendEvent(&event);
				RefineTrumpFrame* pFrame = (RefineTrumpFrame*)m_pFrameMgr->GetFrame(_T("RefineTrump"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_MixBead: // 灵珠镶嵌
			{
				//tagGameEvent event(_T("Close_Mixbead"), this);
				//m_pMgr->SendEvent(&event);
				TrumpMixBeadFrame* pFrame = (TrumpMixBeadFrame*)m_pFrameMgr->GetFrame(_T("TrumpMixBead"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_TrumpReborn:
			{
				TrumpRebornFrame* pFrame = (TrumpRebornFrame*)m_pFrameMgr->GetFrame(_T("TrumpReborn"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_EquipStrengthen:
			{
				EquipStrengthenFrame* pFrame = (EquipStrengthenFrame*)m_pFrameMgr->GetFrame(_T("EquipStrengthen"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_GodStrengthen:
			{
				GodStrengthenFrame* pFrame = (GodStrengthenFrame*)m_pFrameMgr->GetFrame(_T("GodStrengthen"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_FairyAutoFeed:
			{
				FairyAutoFeedFrame* pFrame = (FairyAutoFeedFrame*)m_pFrameMgr->GetFrame(_T("FairyAutoFeed"));
				if (P_VALID(pFrame))
				{
					pFrame->ShowUI(FALSE);
				}
			}
			break;
		case EIST_FairyDamageTransfer:
			{
				FairyTransferFrame* pFrame = (FairyTransferFrame*)m_pFrameMgr->GetFrame(_T("FairyTransfer"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_FairyDamageUpgrade:
			{
				FairyUpgradeFrame* pFrame = (FairyUpgradeFrame*)m_pFrameMgr->GetFrame(_T("FairyUpgrade"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_RandomProduce:
			{
				FashionDesignerFrame* pFrame = (FashionDesignerFrame*)m_pFrameMgr->GetFrame(_T("FashionDesigner"));
				if (P_VALID(pFrame))
					m_pFrameMgr->DestroyFrame(pFrame);
			}
			break;
		case EIST_Clothespress:
			{
				tagGameEvent event(_T("Close_ClothesPress"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
		case EIST_GESkillRenew:
			{
				tagGameEvent event(_T("Close_GESkillOPWnd"), this);
				m_pMgr->SendEvent(&event);
			}
			break;
		case EIST_HolyCompose: //彩印
			{
				HolyManFrame* pFrame = (HolyManFrame*)m_pFrameMgr->GetFrame(_T("HolyManFrame"));
				if (P_VALID(pFrame))
					pFrame->ShowWnd(FALSE);
			}
			break;
		default:
			break;
		}
	}
	m_eCurStrategy = eType;
}

void ItemFrame::CheckItemState( Item* pItem )
{
	if( !P_VALID(pItem) )
		return;

	if( pItem->IsItemLocked() )
	{
		tagDWORDTime dwUnLockTime = pItem->GetUnLockTime();
		if( dwUnLockTime == 0 )
			return;

		tagDWORDTime dwCurTime = GetCurrentDWORDTime();
		if( CompareDwordTime( dwCurTime,dwUnLockTime ) > 0 )
		{
			pItem->SetIMLocked( false );
		}
	}
}

void ItemFrame::ProcessEquipSingnature( tagOnTheRoleOfItemEvent* pGameEvent )
{
	Item* pItemSrc = ItemMgr::Inst()->GetPocketItem( pGameEvent->n16IndexSrc );
	Item* pItemDst = ItemMgr::Inst()->GetPocketItem( pGameEvent->n16IndexDst );
	
	//不是行囊中的物品
	if( !P_VALID(pItemSrc) || !P_VALID(pItemDst) || pGameEvent->eTypeSrc != EICT_Bag || pGameEvent->eTypeDst != EICT_Bag )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_NotInBag")] );
		return;
	}

	//只能对装备进行署名操作
	if( !MIsEquipment(pGameEvent->dwTypeIDDst) || !MIsEquipment(pItemDst->GetItemTypeID()) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_TargetIsNotEquip")] );
		return;
	}

	//不是署名道具
	if( pGameEvent->eSpecFuncSrc != EISF_EquipSignature )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_IsNotEquipSigIM")] );
		return;
	}

	//装备已经被署名
	if( ((Equipment*)pItemDst)->IsSignature() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_AlredySignatrue")] );
		return;
	}

	//生产者署名将会被替换
	if( GT_VALID(pItemDst->GetItemCreatorID()) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("EquipSignature_AlredyHaveSignatrue")] );
	}

	if( P_VALID( GetObj("EquipSignatureInputContentMsgBox") ) )
		KillObj( "EquipSignatureInputContentMsgBox" );
	
	CreateObj( "EquipSignatureInputContentMsgBox", "MsgInputBox" );
	TObjRef<MsgInputBox>("EquipSignatureInputContentMsgBox")->SetN64UserData( pItemSrc->GetItemId() );
	TObjRef<MsgInputBox>("EquipSignatureInputContentMsgBox")->SetN64UserData2( pItemDst->GetItemId() );

	if( pItemSrc->GetProto()->nSpecFuncVal1 == 1 && !pItemDst->IsItemBinded() )
	{
		TObjRef<MsgInputBox>("EquipSignatureInputContentMsgBox")->Init( _T(""), g_StrTable[_T("EquipSignature_desc2")], _T("MsgBoxEvent_EquipSignatureInput"), GT_INVALID, MBIT_Name, NULL, TRUE );
	}
	else
	{
		TObjRef<MsgInputBox>("EquipSignatureInputContentMsgBox")->Init( _T(""), g_StrTable[_T("EquipSignature_desc1")], _T("MsgBoxEvent_EquipSignatureInput"), GT_INVALID, MBIT_Name, NULL, TRUE );
	}
}

BOOL ItemFrame::CheckEquipmentLimit( Equipment* pEquipment)
{
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID(pLocalPlayer)|| !P_VALID(pEquipment))
		return FALSE;
	if ( pLocalPlayer->GetOriginAtt(ERA_Physique)	<	pEquipment->GetEquipAttLimit(ERA_Physique) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_PhysiqueLim")]);
		return FALSE;
	}
	if ( pLocalPlayer->GetOriginAtt(ERA_Strength)	<	pEquipment->GetEquipAttLimit(ERA_Strength))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_StrengthLim")]);
		return FALSE;
	}
	if ( pLocalPlayer->GetOriginAtt(ERA_Pneuma)		<	pEquipment->GetEquipAttLimit(ERA_Pneuma) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_PneumaLim")]);
		return FALSE;
	}
	if ( pLocalPlayer->GetOriginAtt(ERA_InnerForce)	<	pEquipment->GetEquipAttLimit(ERA_InnerForce) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_InnerForceLim")]);
		return FALSE;
	}
	if ( pLocalPlayer->GetOriginAtt(ERA_Technique)	<	pEquipment->GetEquipAttLimit(ERA_Technique) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_TechniqueLim")]);
		return FALSE;
	}
	if ( pLocalPlayer->GetOriginAtt(ERA_Agility)	<	pEquipment->GetEquipAttLimit(ERA_Agility) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_AgilityLim")]);
		return FALSE;
	}	
	if ( pLocalPlayer->GetPlayerSex() == 0 && pEquipment->GetProto()->eSexLimit == ESL_Man )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_ManLim")]);
		return FALSE;
	}
	if ( pLocalPlayer->GetPlayerSex() == 1 && pEquipment->GetProto()->eSexLimit == ESL_Woman )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipmentError_WomenLim")]);
		return FALSE;
	}

	return TRUE;	

}

BOOL ItemFrame::CheckItemLimit( Item* pItem)
{
	if( !P_VALID(pItem))
		return FALSE;

	//物品等级限制
	INT nCurrentRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() ;
	INT nCurrentItemMinLevel = pItem->GetProto()->byMinUseLevel;
	BYTE nCurrentItemMaxLevel = pItem->GetProto()->byMaxUseLevel;
	// Jason 2010-3-12 vx.x.x增加装备特殊属性判断，都执行到这里了，就不再判断原型指针问题了
	BOOL bIsCheckLevelLimit = TRUE;
	const tagItemProto* pItemProto = pItem->GetProto();
	if( MIsEquipment(pItemProto->dwTypeID) )
	{
		const tagEquipSpec * pEquipSpec = ((Equipment*)pItem)->GetEquip();
		if( !P_VALID(pEquipSpec) )
		{
			return FALSE;
		}
		if( pItemProto->eType == EIT_FaBao )
		{
			const tagFabaoSpec *pTrumpSpec = ((Equipment*)pItem)->GetFaBaoSpe();
			nCurrentItemMinLevel = max( pTrumpSpec->nUseLevel, pItemProto->byMinUseLevel );
			nCurrentItemMaxLevel = 150;
		}
		else if( EESA_LevelLim_None == pEquipSpec->bySpecAtt )
			bIsCheckLevelLimit = FALSE;
		else if( EESA_LevelLim_Simple == pEquipSpec->bySpecAtt )
		{
			nCurrentItemMinLevel -= 5;
		}
		else if( EESA_LevelLim_Fine == pEquipSpec->bySpecAtt )
		{
			nCurrentItemMinLevel -= 10;
		}
	}
	if( bIsCheckLevelLimit )
	{
		if( nCurrentItemMinLevel < 0 )
			nCurrentItemMinLevel = 0;

		if ( nCurrentRoleLevel< nCurrentItemMinLevel )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("LessThanMinItemLevelRequire")]);
			return FALSE;
		}
		if ( nCurrentRoleLevel> nCurrentItemMaxLevel )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GreaterThanMaxItemLevelRequire")]);
			return FALSE;
		}
	}

	//性别限制
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if ( pLocalPlayer->GetPlayerSex() == 0 && pItem->GetProto()->eSexLimit == ESL_Man )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemError_ManLim")]);
		return FALSE;
	}
	if ( pLocalPlayer->GetPlayerSex() == 1 && pItem->GetProto()->eSexLimit == ESL_Woman )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemError_WomenLim")]);
		return FALSE;
	}

	//氏族声望限制
	ECLanType eT = MTRANS_ERT2ECLT(pItem->GetProto()->eClanRepute);
	const tagReputeLevel* pReputeLevel = ReputeMgr::Inst()->GetReputeLevel( eT );
	if (!P_VALID(pReputeLevel))
	{
		return TRUE;
	}
	INT repLevel = static_cast<INT>(pReputeLevel->eLevel);
	if( repLevel < pItem->GetProto()->nClanReputeVal )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemError_ClanLim")]);
		return FALSE;
	}
	return TRUE;
}

BOOL ItemFrame::CanUseItemHolyAwake( Item *pItem)
{
	if ( !P_VALID(pItem) )
		return FALSE;
	//元神状态下屏蔽变身道具使用
	BOOL bHolyAwake = HolyManMgr::Inst()->IsHolyAwake();
	if (bHolyAwake)
	{
		const tagItemProto* pProto = pItem->GetProto();
		if ( !P_VALID(pProto) )
			return FALSE;
// 		if ( pProto->eType == EIT_TransItem ) //变身不能使用
// 			return FALSE;
	}
	return TRUE;
}

DWORD ItemFrame::OnEnterWar(tagGameEvent* pGameEvent)
{
	m_bCanUseBag = FALSE;
	return 0;
}

DWORD ItemFrame::OnEnterNormalMap(tagGameEvent* pGameEvent)
{
	m_bCanUseBag = TRUE;
	return 0;
}