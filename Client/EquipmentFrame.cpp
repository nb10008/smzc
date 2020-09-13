#include "stdafx.h"
#include "EquipmentFrame.h"
#include "CharacterFrame.h"
#include "ItemButton.h"
#include "PlayerViewWindow.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "Item.h"
#include "LocalPlayer.h"
#include "ItemEvent.h"
#include "RoleEvent.h"
#include "DragBtn.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_item.h"
#include "CombatSysUtil.h"
#include "CursorMgr.h"
#include "..\WorldDefine\RoleDefine.h"
#include "BlinkStatic.h"

const TCHAR* BlinkPic= _T("data\\ui\\state\\l-state-1.bmp");

EquipmentFrame::EquipmentFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndFather = NULL;
	m_pWndEquipView = NULL;
	for (INT i=0; i<EEP_End; ++i)
	{
		if (i == EEP_FashionFace || i == EEP_FashionLegs)
		{
			continue;
		}
		m_pStcEquipBack[i] = NULL;
		m_pBtnEquip[i] = NULL;
	}

	m_bIsFashionDisplay = FALSE;
	m_pStcFashion = NULL;
	m_dwTimer = 0;
}

EquipmentFrame::~EquipmentFrame(void)
{
}

BOOL EquipmentFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateItemEvent));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_Move2Equip"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangeItemEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdataAvatarEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateAvatarEvent));
	m_pFrameMgr->RegisterEventHandle(_T("FashionEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangDispPattern));
	m_pFrameMgr->RegisterEventHandle(_T("ChangeFashionEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangeFashionPattern));
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdataAvatarDisplaySetEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdataAvatarDisplaySetEvent));
	m_pFrameMgr->RegisterEventHandle(_T("tagRehanding"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OntagRehanding));

	m_pKeyMap->Register(0, g_StrTable[_T("Hotkey_SwapWeapon")], (INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&EquipmentFrame::OnKeySwapWeapon), DIK_E);


	return bRet;
}

BOOL EquipmentFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateItemEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Equip"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangeItemEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdataAvatarEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdateAvatarEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("FashionEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangDispPattern));
	m_pFrameMgr->UnRegisterEventHandler(_T("ChangeFashionEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnChangeFashionPattern));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdataAvatarDisplaySetEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OnUpdataAvatarDisplaySetEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagRehanding"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipmentFrame::OntagRehanding));

	m_pKeyMap->SetEnable(g_StrTable[_T("Hotkey_SwapWeapon")], false);
	return bRet;
}

BOOL EquipmentFrame::ReloadUI()
{
	// 创建
	m_pWndFather = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("rstatepic\\equipback"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&EquipmentFrame::EventHandler));
	
	// 获取控件
	// 装备预览
	m_pStcFashion = (GUIStatic*)m_pWnd->GetChild(_T("state"));
	m_pStcFashion->SetUnClickable(TRUE);
	m_pWndEquipView = (PlayerViewWindow*)m_pWnd->GetChild(_T("role"));

	for (int i = 0; i < EEP_End; i++)
	{
		m_pBlkEquip[i] = 0;
	}
	// 头部
	m_pStcEquipBack[EEP_Head] = (GUIStatic*)m_pWnd->GetChild(_T("hat"));
	m_pBtnEquip[EEP_Head] = (ItemButton*)m_pStcEquipBack[EEP_Head]->GetChild(_T("hatpic"));
	m_pBlkEquip[EEP_Head] = (BlinkStatic*)m_pBtnEquip[EEP_Head]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Head]->SetPic(BlinkPic);
	// 项链
	m_pStcEquipBack[EEP_Neck] = (GUIStatic*)m_pWnd->GetChild(_T("necklace"));
	m_pBtnEquip[EEP_Neck] = (ItemButton*)m_pStcEquipBack[EEP_Neck]->GetChild(_T("necklacepic"));
	m_pBlkEquip[EEP_Neck] = (BlinkStatic*)m_pBtnEquip[EEP_Neck]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Neck]->SetPic(BlinkPic);
	// 上身
	m_pStcEquipBack[EEP_Body] = (GUIStatic*)m_pWnd->GetChild(_T("upper"));
	m_pBtnEquip[EEP_Body] = (ItemButton*)m_pStcEquipBack[EEP_Body]->GetChild(_T("upperpic"));
	m_pBlkEquip[EEP_Body] = (BlinkStatic*)m_pBtnEquip[EEP_Body]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Body]->SetPic(BlinkPic);
	// 下身
	m_pStcEquipBack[EEP_Legs] = (GUIStatic*)m_pWnd->GetChild(_T("lower"));
	m_pBtnEquip[EEP_Legs] = (ItemButton*)m_pStcEquipBack[EEP_Legs]->GetChild(_T("lowerpic"));
	m_pBlkEquip[EEP_Legs] = (BlinkStatic*)m_pBtnEquip[EEP_Legs]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Legs]->SetPic(BlinkPic);
	// 背部
	m_pStcEquipBack[EEP_Back] = (GUIStatic*)m_pWnd->GetChild(_T("mantle"));
	m_pBtnEquip[EEP_Back] = (ItemButton*)m_pStcEquipBack[EEP_Back]->GetChild(_T("mantlepic"));
	m_pBlkEquip[EEP_Back] = (BlinkStatic*)m_pBtnEquip[EEP_Back]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Back]->SetPic(BlinkPic);
	// 手部
	m_pStcEquipBack[EEP_Wrist] = (GUIStatic*)m_pWnd->GetChild(_T("glove"));
	m_pBtnEquip[EEP_Wrist] = (ItemButton*)m_pStcEquipBack[EEP_Wrist]->GetChild(_T("glovepic"));
	m_pBlkEquip[EEP_Wrist] = (BlinkStatic*)m_pBtnEquip[EEP_Wrist]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Wrist]->SetPic(BlinkPic);
	// 脚部
	m_pStcEquipBack[EEP_Feet] = (GUIStatic*)m_pWnd->GetChild(_T("boot"));
	m_pBtnEquip[EEP_Feet] = (ItemButton*)m_pStcEquipBack[EEP_Feet]->GetChild(_T("bootpic"));
	m_pBlkEquip[EEP_Feet] = (BlinkStatic*)m_pBtnEquip[EEP_Feet]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Feet]->SetPic(BlinkPic);
	// 戒指
	m_pStcEquipBack[EEP_Finger1] = (GUIStatic*)m_pWnd->GetChild(_T("ring1"));
	m_pBtnEquip[EEP_Finger1] = (ItemButton*)m_pStcEquipBack[EEP_Finger1]->GetChild(_T("ring1pic"));
	m_pBlkEquip[EEP_Finger1] = (BlinkStatic*)m_pBtnEquip[EEP_Finger1]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Finger1]->SetPic(BlinkPic);
	// 戒指
	m_pStcEquipBack[EEP_Finger2] = (GUIStatic*)m_pWnd->GetChild(_T("ring2"));
	m_pBtnEquip[EEP_Finger2] = (ItemButton*)m_pStcEquipBack[EEP_Finger2]->GetChild(_T("ring2pic"));
	m_pBlkEquip[EEP_Finger2] = (BlinkStatic*)m_pBtnEquip[EEP_Finger2]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Finger2]->SetPic(BlinkPic);
	// 腰饰
	m_pStcEquipBack[EEP_Waist] = (GUIStatic*)m_pWnd->GetChild(_T("sash"));
	m_pBtnEquip[EEP_Waist] = (ItemButton*)m_pStcEquipBack[EEP_Waist]->GetChild(_T("sashpic"));
	m_pBlkEquip[EEP_Waist] = (BlinkStatic*)m_pBtnEquip[EEP_Waist]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Waist]->SetPic(BlinkPic);
	// 主手武器
	m_pStcEquipBack[EEP_RightHand] = (GUIStatic*)m_pWnd->GetChild(_T("mainwp"));
	m_pBtnEquip[EEP_RightHand] = (ItemButton*)m_pStcEquipBack[EEP_RightHand]->GetChild(_T("mainwppic"));
	m_pBlkEquip[EEP_RightHand] = (BlinkStatic*)m_pBtnEquip[EEP_RightHand]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_RightHand]->SetPic(BlinkPic);
	// 魂晶
	m_pStcEquipBack[EEP_LeftHand] = (GUIStatic*)m_pWnd->GetChild(_T("minorwp"));
	m_pBtnEquip[EEP_LeftHand] = (ItemButton*)m_pStcEquipBack[EEP_LeftHand]->GetChild(_T("minorwppic"));
	m_pBlkEquip[EEP_LeftHand] = (BlinkStatic*)m_pBtnEquip[EEP_LeftHand]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_LeftHand]->SetPic(BlinkPic);

	// 
	m_pStcEquipBack[EEP_HunJing] = (GUIStatic*)m_pWnd->GetChild(_T("minorwp1"));
	m_pBtnEquip[EEP_HunJing] = (ItemButton*)m_pStcEquipBack[EEP_HunJing]->GetChild(_T("minorwppic"));
	m_pBlkEquip[EEP_HunJing] = (BlinkStatic*)m_pBtnEquip[EEP_HunJing]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_HunJing]->SetPic(BlinkPic);
	
	// 法宝
	m_pStcEquipBack[EEP_Face] = (GUIStatic*)m_pWnd->GetChild(_T("talisman"));
	m_pBtnEquip[EEP_Face] = (ItemButton*)m_pStcEquipBack[EEP_Face]->GetChild(_T("talismanpic"));
	m_pBlkEquipFb  = (BlinkStatic*)m_pBtnEquip[EEP_Face]->GetChild(_T("pic"));
	m_pBlkEquipFb->SetPic(BlinkPic);
	//m_pBlkEquip[EEP_Face] = (BlinkStatic*)m_pBtnEquip[EEP_Face]->GetChild(_T("pic"));
	//m_pBlkEquip[EEP_Face]->SetPic(BlinkPic);

	// 时装头
	m_pStcEquipBack[EEP_FashionHead] = (GUIStatic*)m_pWnd->GetChild(_T("fashionhat"));
	m_pBtnEquip[EEP_FashionHead] = (ItemButton*)m_pStcEquipBack[EEP_FashionHead]->GetChild(_T("hatpic"));
	//m_pBlkEquip[EEP_FashionHead] = (BlinkStatic*)m_pBtnEquip[EEP_FashionHead]->GetChild(_T("pic"));
	//m_pBlkEquip[EEP_FashionHead]->SetPic(BlinkPic);
	// 时装上身
	m_pStcEquipBack[EEP_FashionBody] = (GUIStatic*)m_pWnd->GetChild(_T("fashioncloth"));
	m_pBtnEquip[EEP_FashionBody] = (ItemButton*)m_pStcEquipBack[EEP_FashionBody]->GetChild(_T("clothpic"));
	//m_pBlkEquip[EEP_FashionBody] = (BlinkStatic*)m_pBtnEquip[EEP_FashionBody]->GetChild(_T("pic"));
	//m_pBlkEquip[EEP_FashionBody]->SetPic(BlinkPic);
	// 时装背部
	m_pStcEquipBack[EEP_FashionBack] = (GUIStatic*)m_pWnd->GetChild(_T("fashionback"));
	m_pBtnEquip[EEP_FashionBack] = (ItemButton*)m_pStcEquipBack[EEP_FashionBack]->GetChild(_T("backpic"));
	//m_pBlkEquip[EEP_FashionBack] = (BlinkStatic*)m_pBtnEquip[EEP_FashionBack]->GetChild(_T("pic"));
	//m_pBlkEquip[EEP_FashionBack]->SetPic(BlinkPic);
	// 时装手部
	m_pStcEquipBack[EEP_FashionWrist] = (GUIStatic*)m_pWnd->GetChild(_T("fashionglove"));
	m_pBtnEquip[EEP_FashionWrist] = (ItemButton*)m_pStcEquipBack[EEP_FashionWrist]->GetChild(_T("glovepic"));
	//m_pBlkEquip[EEP_FashionWrist] = (BlinkStatic*)m_pBtnEquip[EEP_FashionWrist]->GetChild(_T("pic"));
	//m_pBlkEquip[EEP_FashionWrist]->SetPic(BlinkPic);
	// 时装足部
	m_pStcEquipBack[EEP_FashionFeet] = (GUIStatic*)m_pWnd->GetChild(_T("fashionboot"));
	m_pBtnEquip[EEP_FashionFeet] = (ItemButton*)m_pStcEquipBack[EEP_FashionFeet]->GetChild(_T("bootpic"));
	//m_pBlkEquip[EEP_FashionFeet] = (BlinkStatic*)m_pBtnEquip[EEP_FashionFeet]->GetChild(_T("pic"));
	//m_pBlkEquip[EEP_FashionFeet]->SetPic(BlinkPic);

	// 圣灵
	m_pStcEquipBack[EEP_Holy]	= (GUIStatic*)m_pWnd->GetChild(_T("sl"));
	m_pBtnEquip[EEP_Holy] = (ItemButton*)m_pStcEquipBack[EEP_Holy]->GetChild(_T("talismanpic"));
	m_pBlkEquip[EEP_Holy] = (BlinkStatic*)m_pBtnEquip[EEP_Holy]->GetChild(_T("pic"));
	m_pBlkEquip[EEP_Holy]->SetPic(BlinkPic);

	for (int i = 0; i < EEP_End; i++)
	{
		if (P_VALID(m_pBlkEquip[i] ))
		{
			m_pBlkEquip[i]->SetPic(BlinkPic);
			m_pBlkEquip[i]->SetLoopNum(16);
			m_pBlkEquip[i]->SetSize(44,44);
			m_pBlkEquip[i]->SetStartPoint(317,57);
		}
			
	}
	m_pBlkEquipFb->SetPic(BlinkPic);
	m_pBlkEquipFb->SetLoopNum(16);
	m_pBlkEquipFb->SetSize(44,44);
	m_pBlkEquipFb->SetStartPoint(317,57);
	
	
	// 隐藏装备显示图标
	UpdateAll();

	return TRUE;
}

VOID EquipmentFrame::Update()
{	
	if(RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_WardrobeWare ))
	{
		//m_pStcFashion->SetText(g_StrTable[_T("ClothespressDressing")]);
		m_pStcFashion->SetInvisible(FALSE);
		m_pStcFashion->SetRefresh(TRUE);
		m_pStcFashion->FlipToTop();
	}
	else
		m_pStcFashion->SetInvisible(TRUE);

	m_dwTimer += Kernel::Inst()->GetDeltaTimeDW();
	if(m_dwTimer > 3000)
	{
		UpdataBlick();
		m_dwTimer = 0;
	}
	
}

DWORD EquipmentFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			for(INT i=0; i<EEP_End; ++i)
			{
				/*if (m_bIsFashionDisplay && i > EEP_Feet && i != EEP_RightHand && i != EEP_LeftHand)
				{
					continue;
				}*/
				if ( i == EEP_FashionFace || i == EEP_FashionLegs)
				{
					continue;
				}
				if(pWnd == m_pBtnEquip[i] || pWnd == m_pBlkEquip[i])
				{
					Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));
					
					if(P_VALID(pEquip))
					{
						tagItemPosChangeExEvent event(_T("ItemEvent_Move2Bag"),this);
						event.dwTypeIDSrc = pEquip->GetItemTypeID();
						event.eTypeSrc = EICT_Equip;
						event.eTypeDst = EICT_Bag;
						event.n16IndexSrc = pEquip->GetPos();
						m_pFrameMgr->SendEvent(&event);
						
						// 没有装备则显示装备部位
						TCHAR szTmp[X_SHORT_NAME];
						_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("EquipPos%d"), GetEquipPos(i)+1);
						tstring strTmp = g_StrTable[_T("TipsDefaultFont")];
						strTmp += g_StrTable[szTmp];
						ShowTip(pWnd, strTmp.c_str());
					}
					break;
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			for(INT i=0; i<EEP_End; ++i)
			{
				/*if (m_bIsFashionDisplay && i > EEP_Feet && i != EEP_RightHand && i != EEP_LeftHand)
				{
					continue;
				}*/
				if ( i == EEP_FashionFace || i == EEP_FashionLegs)
				{
					continue;
				}
				if(pWnd == m_pBtnEquip[i]|| pWnd == m_pBlkEquip[i])
				{
					Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));
					
					if(P_VALID(pEquip))
					{
						DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
						if (P_VALID(pDrag))
						{
							pEvent->dwParam1 = EDT_ItemEquip;
							pEvent->dwParam2 = pEquip->GetPos();
							pEvent->dwParam3 = pEquip->GetItemTypeID();

							if(pWnd == m_pBlkEquip[i])
								pEvent->dwWndID = m_pBtnEquip[i]->GetID();
							pDrag->OnEvent(pEvent);
	
						}
						break;
					}
					break;
				}
			}
		}
		break;
	case EGUISCB_Drop:
		{
			for(INT i=0; i<EEP_End; ++i)
			{
				/*if (m_bIsFashionDisplay && i > EEP_Feet && i != EEP_RightHand && i != EEP_LeftHand)
				{
						continue;
				}*/

				if ( i == EEP_FashionFace || i == EEP_FashionLegs)
				{
					continue;
				}
				if(pWnd == m_pBtnEquip[i]|| pWnd == m_pBlkEquip[i])
				{
					if(EDT_Item == pEvent->dwParam1)//背包中的物品处理物品装备效果
					{
						OnEquipment((INT16)LOWORD(pEvent->dwParam2), GetEquipPos(i));
					}
					else if(EDT_ItemEquip == pEvent->dwParam1)//装备栏中的物品之间换位置
					{
						OnEquipSwap((EEquipPos)LOWORD(pEvent->dwParam2), GetEquipPos(i));
					}
					break;
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for(INT i = 0; i < EEP_End; ++i)
			{

				if ( i == EEP_FashionFace || i == EEP_FashionLegs)
				{
					continue;
				}
				if(pWnd == m_pBtnEquip[i]|| pWnd == m_pBlkEquip[i] || pWnd == m_pBlkEquipFb)
				{
					Equipment* pEquip = NULL;
					/*if (!m_bIsFashionDisplay || i <= EEP_Feet || i == EEP_RightHand || i == EEP_LeftHand)*/
					if(pWnd != m_pBlkEquipFb)
						pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));
					else
						pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Face);	//获取法宝

					if(P_VALID(pEquip))
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
					}
					else
					{
						// 没有装备则显示装备部位
						TCHAR szTmp[X_SHORT_NAME];
						_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("EquipPos%d"), GetEquipPos(i)+1);
						tstring strTmp = g_StrTable[_T("TipsDefaultFont")];
						strTmp += g_StrTable[szTmp];
						ShowTip(pWnd, strTmp.c_str());
					}
					break;
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
    case EGUISCB_UseSpecFunc:
        {
            for(INT i = 0; i < EEP_End; ++i)
            {
				if ( i == EEP_FashionFace || i == EEP_FashionLegs)
				{
					continue;
				}
                if(pWnd == (GUIWnd*)m_pBtnEquip[i] || pWnd == (GUIWnd*)m_pBlkEquip[i])
                {
                    Item* pItemDst = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));
                    if(P_VALID(pItemDst))
                    {
                        // 判断是否发出动作源对象是否是物品
                        if(pEvent->dwParam1 > ECAN_ItemStart)
                        {
                            // 处理物品特殊操作
                            Item* pItemSrc = ItemMgr::Inst()->GetPocketItem((INT16)pEvent->dwParam2);
                            if(P_VALID(pItemSrc))
                            {
                                tagOnTheRoleOfItemEvent event(_T("ItemEvent_TargetItem"), this);
                                event.eTypeSrc = pItemSrc->GetConType();
                                event.n16IndexSrc = pItemSrc->GetPos();
                                event.dwTypeIDSrc = pItemSrc->GetItemTypeID();
                                event.eSpecFuncSrc = pItemSrc->GetItemSpecFunc();
                                event.eTypeDst = pItemDst->GetConType();
                                event.n16IndexDst = pItemDst->GetPos();
                                event.dwTypeIDDst = pItemDst->GetItemTypeID();
                                m_pFrameMgr->SendEvent(&event);
                            }
                        }
                        else
                        {
                            // 处理其他特殊操作
                        }
                    }
                    break;
                }
            }
        }
        break;
		
	}
	return 0;
}


EEquipPos EquipmentFrame::GetEquipPos( INT nIndex )
{
	EEquipPos eRet = EEP_End;
	//if(m_bIsFashionDisplay)
	//{
	//	if(nIndex < EEP_End - EEP_MaxEquip)
	//	{
	//		eRet = (EEquipPos)(nIndex + EEP_MaxEquip);
	//	}
	//	else
	//	{
	//		eRet = (EEquipPos)nIndex;
	//	}
	//}
	//else
	//{
	//	eRet = (EEquipPos)nIndex;
	//}
	eRet = (EEquipPos)nIndex;
	return eRet;
}

VOID EquipmentFrame::OnEquipment(INT16 nItemPos, EEquipPos ePos /* = EEP_End */)
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	//如果本地玩家在备战状态或使用技能不容许换装
	if( P_VALID(pLp) )
	{
		if( pLp->IsSkillState() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CombatEquip")]);
			return;
		}
		else if( pLp->IsReadyState() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_ReadyEquip")]);
			return;
		}
	}

	Item* pItem = ItemMgr::Inst()->GetPocketItem(nItemPos);
	if( P_VALID(pItem) && MIsEquipment(pItem->GetItemTypeID()) )
	{
		if (pItem->GetProto()->eType == EIT_FaBao)
		{
			const tagItemProto* pItemProto = pItem->GetProto();
			const tagFabaoSpec *pTrumpSpec = ((Equipment*)pItem)->GetFaBaoSpe();
			if (P_VALID(pItemProto) && P_VALID(pTrumpSpec))
			{
				INT nCurrentRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() ;
				int nCurrentItemMinLevel = max( pTrumpSpec->nUseLevel, pItemProto->byMinUseLevel );
				if ( nCurrentRoleLevel< nCurrentItemMinLevel )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("LessThanMinItemLevelRequire")]);
					return;
				}
			}
			
		}

		Equipment* pEquip = (Equipment*)pItem;
		if(IsEquipPosFix(pEquip->GetEquipPos(), ePos) )
		{
			tagNC_Equip m;
			m.ePosDst = ePos;
			m.n64Serial = pItem->GetItemId();
			m_pSession->Send(&m);
		}

		// 如果是法宝则关闭相关界面
		if (pItem->GetProto()->eType == EIT_FaBao)
		{
			TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("CloseAllTrumpChildFrame"), NULL ) );
		}
	}
}

BOOL EquipmentFrame::IsEquipPosFix( EEquipPos eEquipPos, EEquipPos &eDstPos )
{
	// 目标栏位为默认值，则为不指定栏位，栏位根据装备栏位确定
	if(eDstPos == EEP_End)
	{
		eDstPos = eEquipPos;
		
		// 特例：如果是戒指或者武器，因为可放入两个栏位，所以需要判断栏位是否为空
		if(EEP_Finger1 == eEquipPos)
		{
			if( ItemMgr::Inst()->IsNotEquip(EEP_Finger1) || 
				( !ItemMgr::Inst()->IsNotEquip(EEP_Finger1) 
				&& !ItemMgr::Inst()->IsNotEquip(EEP_Finger2) ) )
			{
				eDstPos = EEP_Finger1;
			}
			else
			{
				eDstPos = EEP_Finger2;
			}
		}
		else if(EEP_RightHand == eEquipPos)
		{
			if( ItemMgr::Inst()->IsNotEquip(EEP_RightHand) || 
				( !ItemMgr::Inst()->IsNotEquip(EEP_RightHand) 
				&& !ItemMgr::Inst()->IsNotEquip(EEP_LeftHand) ) )
			{
				eDstPos = EEP_RightHand;
			}
			else
			{
				eDstPos = EEP_LeftHand;
			}
		}
		return TRUE;
	}
	
	// 不是默认值，则根据目标栏位判断，
	// 特例：如果是戒指或者武器，因为可放入两个栏位，所以需要判断两个栏位
	if(EEP_Finger1 == eEquipPos)
	{
		if(EEP_Finger1 == eDstPos || EEP_Finger2 == eDstPos)
		{
			return TRUE;
		}
	}
	else if(EEP_RightHand == eEquipPos)
	{
		if(EEP_RightHand == eDstPos || EEP_LeftHand == eDstPos)
		{
			return TRUE;
		}
	}
	else if(eEquipPos == eDstPos)
	{
		return TRUE;
	}
	
	return FALSE;
}

VOID EquipmentFrame::UpdateEquipAvatar()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
		m_pWndEquipView->UpdataAvatar(pLp);
}


VOID EquipmentFrame::UpdateEquipment( EEquipPos ePos )
{
	EEquipPos ePosTmp = ePos;
	DWORD dwTypeID = GT_INVALID;
	BYTE byQuality = 0;
	Equipment* pEquip = (Equipment*)GT_INVALID;

	//if (m_bIsFashionDisplay)
	//{
	//	if (ePos >= EEP_MaxEquip && ePos < EEP_End)
	//	{
	//		ePosTmp = (EEquipPos)(ePos - EEP_MaxEquip);
	//		pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
	//	}
	//	/*! 时装模式下仍然保留武器 */
	//	else if (EEP_RightHand == ePos || EEP_LeftHand == ePos)
	//	{
	//		ePosTmp = ePos;
	//		pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
	//	}
	//	else
	//	{
	//		ePosTmp = ePos;
	//		pEquip = ItemMgr::Inst()->GetCurEquip((EEquipPos)(EEP_MaxEquip+ePos));			
	//	}
	//}
	//else
	//{
	//	if(ePos < EEP_MaxEquip)
	//	{
	//		ePosTmp = ePos;
	//		pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
	//	}
	//}
	if ( ePos < EEP_End)
	{
		ePosTmp = ePos;
		pEquip = ItemMgr::Inst()->GetCurEquip(ePos);
	}

	if(P_VALID(pEquip))
	{
		dwTypeID = pEquip->GetItemTypeID();
		byQuality = pEquip->GetItemQuality();
	}

    // 刷新按钮
    ASSERT(ePosTmp < EEP_End);
    /*if (m_bIsFashionDisplay)
    {
        if (ePos >= EEP_MaxEquip && ePos < EEP_End || ePos == EEP_RightHand || ePos == EEP_LeftHand)
            m_pBtnEquip[ePosTmp]->RefreshItem(dwTypeID, 0, byQuality);
        else
            m_pBtnEquip[ePosTmp]->RefreshItem();
    }
    else
    {
        if (ePos < EEP_MaxEquip)
            m_pBtnEquip[ePosTmp]->RefreshItem(dwTypeID, 0, byQuality);
        else
            m_pBtnEquip[ePosTmp]->RefreshItem();
    }*/
	if(dwTypeID != GT_INVALID && ePos == EEP_Holy)
	{
		//发送显示圣灵召唤按钮事件
		tagGameEvent evt(_T("ShengLingEquipEvent"), NULL);
		m_pFrameMgr->SendEvent(&evt);
	}
	m_pBtnEquip[ePosTmp]->RefreshItem(dwTypeID, 0, byQuality);	

}

VOID EquipmentFrame::OnEquipSwap( EEquipPos ePosSrc, EEquipPos ePosDst, BOOL bHotKey )
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	//如果本地玩家在战斗状态不容许换武器和装备
	if( !P_VALID(pLp) || pLp->IsCombatState() )
	{
		// 如果是换武器提示战斗状态不允许换武器
		if ( (EEP_RightHand == ePosSrc && EEP_LeftHand == ePosDst)
			|| (EEP_LeftHand == ePosSrc && EEP_RightHand == ePosDst) )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CombatSwap")]);
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_CombatEquip")]);
		}
		return;
	}
	//防止在个性动作中切换装备
	if( pLp->IsStyleActionState() )
		pLp->ChangeUpperToIdle();

	bool bDest = false; // 目标装备栏是否为空
	// 如果源装备栏为空，则需要看看目标装备栏
	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(ePosSrc);
	if(!P_VALID(pEquip))
	{
		pEquip = ItemMgr::Inst()->GetCurEquip(ePosDst);
		if(!P_VALID(pEquip))
		{
			// 如果目标装备栏为空，则不处理
			return;
		}
		else
		{
			// 否则，当做目标装备栏装备移到源装备栏处理
			EEquipPos eTmp = ePosDst;
			ePosDst = ePosSrc;
			ePosSrc = eTmp;
		}
	}
	else
	{
		Equipment *pEquip = ItemMgr::Inst()->GetCurEquip(ePosDst);
		if(!P_VALID(pEquip))
			bDest = true;
	}

	//只处理武器间的切换和戒指间的切换
	if( (EEP_RightHand == ePosSrc && EEP_LeftHand == ePosDst)
		|| (EEP_LeftHand == ePosSrc && EEP_RightHand == ePosDst) )
	{
		if( bDest && bHotKey )
			return;
		//武器间切换发切换武器
		tagNC_SwapWeapon m;
		m_pSession->Send(&m);
	}
	else if( (EEP_Finger1 == ePosSrc && EEP_Finger2 == ePosDst)
		|| (EEP_Finger2 == ePosSrc && EEP_Finger1 == ePosDst) )
	{
		//如果本地玩家在备战状态不容许换戒指
		if( pLp->IsReadyState() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemMsgError_ReadyEquip")]);
			return;
		}

		//戒指间切换发物品位置改变
		tagNC_ItemPosChange m;
		m.eConType = EICT_Equip;
		m.n16PosDst = ePosDst;
		m.n64Serial = pEquip->GetItemId();
		m_pSession->Send(&m);
	}
}

DWORD EquipmentFrame::OnUpdateItemEvent( tagUpdateLPItemEvent* pGameEvent )
{
	//只有装备栏中的物品才刷新
	if(EICT_Equip == pGameEvent->eType)
	{
		if (pGameEvent->n16Index == EEP_FashionFace || pGameEvent->n16Index == EEP_FashionLegs)
		{
			return 0;
		}

		if (pGameEvent->n16Index < EEP_End)
			UpdateEquipment((EEquipPos)pGameEvent->n16Index);
       /* if (m_bIsFashionDisplay)
        {
		    UpdateEquipment((EEquipPos)pGameEvent->n16Index);
        }
        else
        {
            if (pGameEvent->n16Index < EEP_MaxEquip)
                UpdateEquipment((EEquipPos)pGameEvent->n16Index);
        }*/
	}
	return 0;
}

DWORD EquipmentFrame::OnChangeItemEvent( tagItemPosChangeExEvent* pGameEvent )
{
	//只有背包中的物品拖到装备栏才做穿装处理
	if(EICT_Equip == pGameEvent->eTypeDst && EICT_Bag == pGameEvent->eTypeSrc)
	{
		OnEquipment(pGameEvent->n16IndexSrc);
	}
	return 0;
}

DWORD EquipmentFrame::OnUpdateAvatarEvent( tagUpdataAvatarEvent* pGameEvent )
{
	//本地玩家装备变化更新Avatar
	if(pGameEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
		UpdateEquipAvatar();

	return 0;
}

VOID EquipmentFrame::Render3D()
{
	if( P_VALID( m_pWndEquipView ) )
		m_pWndEquipView->Render3D();
	GameFrame::Render3D();
}

DWORD EquipmentFrame::OnKeySwapWeapon( DWORD dwID, BOOL bDown )
{
	if(bDown)
	{
		//主副手切换
		OnEquipSwap(EEP_RightHand, EEP_LeftHand, bDown);
	}

	return 0;
}

DWORD EquipmentFrame::OnChangDispPattern( tagFashionEvent *pGameEvent )
{

	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
	{
		return 0;
	}
	//防止在个性动作中切换装备
	if( pLp->IsStyleActionState() )
		pLp->ChangeUpperToIdle();
    /*! 发送网络消息 */
    tagNC_Fashion e;
    e.bFashion = pGameEvent->bDisFashion;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);

	/*! 设置装备/时装显示模式 */
	m_bIsFashionDisplay = pGameEvent->bDisFashion;
	UpdateAll();

	/*! 屏显提示 */
	tstring str = pGameEvent->bDisFashion ? g_StrTable[_T("PatternFashion")] : g_StrTable[_T("PatternEquip")];
	//CombatSysUtil::Inst()->ShowScreenCenterMsgEx(str.c_str());

	return 0;
}

DWORD EquipmentFrame::OnChangeFashionPattern( tagChangeFashionEvent *pGameEvent )
{
	
	/*! 设置装备/时装显示模式 */
	m_bIsFashionDisplay = pGameEvent->bDisFashion;
	UpdateAll();

	/*! 屏显提示 */
	tstring str = pGameEvent->bDisFashion ? g_StrTable[_T("PatternFashion")] : g_StrTable[_T("PatternEquip")];
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(str.c_str());

	return 0;
}


VOID EquipmentFrame::UpdateAll()
{
    TurnFashionPos(!m_bIsFashionDisplay);

	for (INT i = 0; i < EEP_End; ++i)
	{
		if (i == EEP_FashionFace || i == EEP_FashionLegs)
		{
			continue;
		}
		UpdateEquipment((EEquipPos)i);

	}


// 	if (m_bIsFashionDisplay)
// 	{
// 		for (INT i = 0; i < EEP_End; ++i)
// 		{
// 			UpdateEquipment((EEquipPos)i);
// 		}
// 	}
// 	else
// 	{
// 		for (INT i=0; i<EEP_MaxEquip; ++i)
// 		{
// 			UpdateEquipment((EEquipPos)i);
// 		}
// 	}
}

DWORD EquipmentFrame::OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pWndEquipView->UpdataDisplaySet( RoleMgr::Inst()->GetLocalPlayer()->GetDisplaySet() );
	}
	return 0;
}

DWORD EquipmentFrame::OntagRehanding(tagRehanding* pGameEvent)
{
	if(pGameEvent->dwError == E_Success && m_bIsFashionDisplay)
	{
		//m_pStcFashion->SetText(g_StrTable[_T("ClothespressDressing")]);
		m_pStcFashion->SetInvisible(FALSE);
		m_pStcFashion->FlipToTop();
	}
	else
		m_pStcFashion->SetInvisible(TRUE);
	m_pStcFashion->SetRefresh(TRUE);
	return 0;
}

void EquipmentFrame::TurnFashionPos( bool on )
{
    /*m_pStcEquipBack[EEP_Face]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Finger1]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Finger2]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Waist]->SetInvisible(!on);
    m_pStcEquipBack[EEP_Neck]->SetInvisible(!on);*/
}

void EquipmentFrame::UpdataBlick()
{
	for(INT i=0; i<EEP_End; ++i)
	{
		if ( i == EEP_FashionFace || i == EEP_FashionLegs)
			continue;
		
		Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(i));
		if(P_VALID(pEquip) && pEquip->GetEquipStrengthLevel() >= 12 )
		{
			if (P_VALID(m_pBlkEquip[i]))
			{
				if(20 <= pEquip->GetEquipStrengthLevel())
					m_pBlkEquip[i]->SetStartPoint(284,244);
				else
					m_pBlkEquip[i]->SetStartPoint(284,56);
				
				m_pBlkEquip[i]->StartBlink();
			}
		}
	}

	Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(GetEquipPos(EEP_Face));
	if(P_VALID(pEquip) )
	{
		const tagFabaoSpec *pTrumpSpe = pEquip->GetFaBaoSpe();
		if (P_VALID(pTrumpSpe) && pTrumpSpe->n16NativeIntelligence >= 80)
		{
			if(pTrumpSpe->n16NativeIntelligence > 100)
				m_pBlkEquipFb->SetStartPoint(284,244);
			else if (pTrumpSpe->n16NativeIntelligence >= 80)
				m_pBlkEquipFb->SetStartPoint(284,56);

			m_pBlkEquipFb->StartBlink();
		}
	}
}
