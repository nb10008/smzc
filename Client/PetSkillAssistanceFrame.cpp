#include "StdAfx.h"
#include "PetSkillAssistanceFrame.h"
#include "PetManager.h"
#include "PetProtoData.h"
#include "PetSkillFrame.h"
#include "ItemMgr.h"
#include "Item.h"
#include "ToolTipFrame.h"
#include "ToolTipCreator.h"
#include "StaticEx.h"
#include "MallMgr.h"
#include "MallFrame.h"
#include "DragBtn.h"

PetSkillAssistanceFrame::PetSkillAssistanceFrame(void) :
    m_Trunk(this),
    m_CurrPetID(GT_INVALID),
    m_nCurrItem(0),
    m_dwUpdateTime(0)
{
    for (int i = 0; i < ESSTE_End; ++i)
    {
        m_pPbtComposeSkill[i] = NULL;
    }
}

PetSkillAssistanceFrame::~PetSkillAssistanceFrame(void)
{
}

BOOL PetSkillAssistanceFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    // register game event handle
    m_pFrameMgr->RegisterEventHandle(_T("tagPetAttrChange"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&PetSkillAssistanceFrame::OnPetAttrChange));

    return bRet;
}

BOOL PetSkillAssistanceFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    // unregister game event handle
    m_pFrameMgr->UnRegisterEventHandler(_T("tagPetAttrChange"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&PetSkillAssistanceFrame::OnPetAttrChange));

    if (P_VALID(m_pWnd))
    {
        m_pGUI->AddToDestroyList(m_pWnd);
        m_pWnd = 0;
    }

	if (P_VALID(m_pWndBuy))
    {
        m_pGUI->AddToDestroyList(m_pWndBuy);
        m_pWndBuy = 0;
    }


    return bRet;
}

BOOL PetSkillAssistanceFrame::ReloadUI()
{
    // load ui resources
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\pet_online.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic\\an_pic1\\itempic\\item", "ClassName", "PetSkillButton");
    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic\\an_pic2\\itempic\\item", "ClassName", "PetSkillButton");
    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic0\\an_pic1\\itempic\\item", "ClassName", "PetSkillButton");
    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic0\\an_pic2\\itempic\\item", "ClassName", "PetSkillButton");
    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic1\\an_pic1\\itempic\\item", "ClassName", "PetSkillButton");
    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic0\\an_pic2\\itemback1\\item", "ClassName", "ItemButton");
    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic0\\an_pic2\\itemback2\\item", "ClassName", "ItemButton");
    m_pGUI->ChangeXml(&ele, "an_win\\an_pic\\rstatepic0\\an_pic2\\itemback3\\item", "ClassName", "ItemButton");

    // create the window
    m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

    // register ui event handle
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&PetSkillAssistanceFrame::EventHandler));

    // controls

    m_pBtClose          = static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\closebutt")));
    m_pBtBuy            = static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\word2\\buybutt")));

    m_pCbPetList        = static_cast<GUIComboBox*>(m_pWnd->GetChild(_T("an_pic\\word0\\optmap")));

    m_pStcSkillLeft     = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("an_pic\\word0\\word1")));

    m_pPbtFight         = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\winopt\\optbutt1")));
    m_pPbtProduction    = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\winopt\\optbutt2")));
    m_pPbtOther         = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\winopt\\optbutt3")));
    m_pPchFight         = static_cast<GUIPatch*>(m_pWnd->GetChild(_T("an_pic\\rstatepic")));
    m_pPchProduction    = static_cast<GUIPatch*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0")));
    m_pPchOther         = static_cast<GUIPatch*>(m_pWnd->GetChild(_T("an_pic\\rstatepic1")));

    m_pPetSkillGather   = static_cast<PetSkillButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic1\\itempic\\item")));
    m_pPetLearnGather   = static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic1\\learnbutt")));

    m_pPetSkillTraining = static_cast<PetSkillButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\itempic\\item")));
    m_pPetLearnTraining = static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\learnbutt")));
    m_pPbtComposeSkill[ESSTE2_Smilt]        = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt0")));
    m_pPbtComposeSkill[ESSTE2_Artisan]      = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt4")));
    m_pPbtComposeSkill[ESSTE2_DanTraining]  = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt1")));
    m_pPbtComposeSkill[ESSTE2_Smith]        = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt5")));
    m_pPbtComposeSkill[ESSTE2_Casting]      = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt2")));
    m_pPbtComposeSkill[ESSTE2_Dressmaker]   = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt6")));
    m_pPbtComposeSkill[ESSTE2_Aechnics]     = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt3")));
    m_pPbtComposeSkill[ESSTE2_PointUp]      = static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\back\\openbutt7")));
    m_pBtTrainingConfirm= static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic\\an_pic2\\trainbutt")));

    m_pPetSkillPickUp   = static_cast<PetSkillButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic1\\itempic\\item")));
    m_pPetLearnPickUp   = static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic1\\learnbutt")));
    TCHAR tbuff[100];
    for (int i = 0; i != MAX_PICKUP_FUNC; ++i)
    {
        _sntprintf(tbuff, sizeof(tbuff) / sizeof(TCHAR), _T("an_pic\\rstatepic0\\an_pic1\\openbutt%d"), i + 1);
        m_pBtPickUpFunc[i]    =   static_cast<GUIPushButton*>(m_pWnd->GetChild(tbuff));
    }

    m_pBtPickConfirm    = static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic1\\surebutt")));
    m_pBtPickConfirm->SetInvisible(TRUE);

    m_pPetSkillMedicare   = static_cast<PetSkillButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\itempic\\item")));
    m_pPetLearnMedicare   = static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\learnbutt")));

    m_pCbRedCondition       =   static_cast<GUIComboBox*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\word4\\comboback")));      // 红药条件
    m_pPbtRedEnable         =   static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\openbutt1")));           // 红药功能开启
    m_pBtRed                =   static_cast<ItemButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\itemback1\\item")));        // 红药

    m_pCbBlueCondition      =   static_cast<GUIComboBox*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\word5\\comboback")));      // 蓝药条件
    m_pPbtBlueEnable        =   static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\openbutt2")));           // 蓝药功能开启
    m_pBtBlue               =   static_cast<ItemButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\itemback2\\item")));        // 蓝药

    m_pCbToughnessCondition =   static_cast<GUIComboBox*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\word6\\comboback")));      // 持久力条件
    m_pPbtToughnessEnable   =   static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\openbutt3")));           // 持久力功能开启
    m_pBtToughness          =   static_cast<ItemButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic0\\an_pic2\\itemback3\\item")));   

    m_pPetSkillSale         =   static_cast<PetSkillButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic1\\an_pic1\\itempic\\item")));
    m_pPetLearnSale         =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("an_pic\\rstatepic1\\an_pic1\\learnbutt")));

    // 初始化ui数据
    InitalUI();

    // hide the window
    ShowWnd(false);

    //--------------------华丽的分割线-------------------------//
    // 处理购买界面
    strPath = g_strLocalPath + _T("\\ui\\sprite_shopping.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    char buffer[256] = {0};
    for (int i = 0; i < MAX_PET_EGG_BUY; ++i)
    {
        _snprintf(buffer, sizeof(buffer) / sizeof(char), "sprite_2_win\\sprite_2_pic\\back0\\itempic%d", i);
        m_pGUI->ChangeXml(&ele, buffer, "ClassName", "StaticEx");
        _snprintf(buffer, sizeof(buffer) / sizeof(char), "sprite_2_win\\sprite_2_pic\\back0\\itempic%d\\img", i);
        m_pGUI->ChangeXml(&ele, buffer, "ClassName", "ItemButton");
    }

    // create the window
    m_pWndBuy = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

    m_pStcTitle     = (GUIStatic*)m_pWndBuy->GetChild(_T("sprite_2_pic\\new"));
    m_pBtBuyConfirm = (GUIButton*)m_pWndBuy->GetChild(_T("sprite_2_pic\\buybutt"));
    m_pBtBuyCanel   = (GUIButton*)m_pWndBuy->GetChild(_T("sprite_2_pic\\canclebutt"));
    m_pBtBuyExit    = (GUIButton*)m_pWndBuy->GetChild(_T("sprite_2_pic\\closebutt"));

    TCHAR tbuffer[256] = {0};
    for (int i = 0; i < MAX_PET_EGG_BUY; ++i)
    {
        _sntprintf(tbuffer, sizeof(tbuffer) / sizeof(TCHAR), _T("sprite_2_pic\\back0\\itempic%d"), i);
        m_pStcSelect[i] = static_cast<StaticEx*>(m_pWndBuy->GetChild(tbuffer));
        m_pIbtEgg[i] = (ItemButton*)m_pStcSelect[i]->GetChild(_T("img"));
        _sntprintf(tbuffer, sizeof(tbuffer) / sizeof(TCHAR), _T("sprite_2_pic\\back0\\num%d"), i);
        m_pStcPrice[i] = (GUIStatic*)m_pWndBuy->GetChild(tbuffer);
    }

    // register ui event handle
    m_pGUI->RegisterEventHandler(m_pWndBuy->GetFullName().c_str(), m_Trunk.sfp1(&PetSkillAssistanceFrame::BuyEventHandler));

    ShowBuyWnd(false);

    return TRUE;
}

VOID PetSkillAssistanceFrame::Update()
{
    GameFrame::Update();

    if (Kernel::Inst()->GetAccumTimeDW() - m_dwUpdateTime > 1000)
    {
        m_dwUpdateTime = Kernel::Inst()->GetAccumTimeDW();

        if (IsOpen())
        {
            RefreshPets();
            RefreshPetSkill();
        }
    }
}

BOOL PetSkillAssistanceFrame::EscCancel()
{
    if (!P_VALID(m_pWnd) || !P_VALID(m_pWndBuy))
        return FALSE;

    if(m_pWnd->IsInvisible() && m_pWndBuy->IsInvisible())
        return FALSE;

    ShowWnd(false);
    ShowBuyWnd(false);
    return TRUE;
}

void PetSkillAssistanceFrame::ShowWnd( bool bVisible )
{
    if (P_VALID(m_pWnd))
    {
        m_pWnd->SetInvisible(!bVisible);
    }

    if (bVisible)
    {
        m_pWnd->FlipToTop();

        RefreshPets();
        RefreshPetSkill();
    }
    else
    {

    }
}

DWORD PetSkillAssistanceFrame::EventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if (pWnd == m_pBtClose)
            {
                ShowWnd(false);
            }
            else if (pWnd == m_pBtBuy)
            {
                ShowBuyWnd(true);
            }
            else if (pWnd == m_pBtRed)
            {
                m_pBtRed->RefreshItem();
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.dwHPItemID   =   GT_INVALID;
                }
            }
            else if (pWnd == m_pBtBlue)
            {
                m_pBtBlue->RefreshItem();
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.dwMPItemID   =   GT_INVALID;
                }
            }
            else if (pWnd == m_pBtToughness)
            {
                m_pBtToughness->RefreshItem();
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.dwTouItemID   =   GT_INVALID;
                }
            }
            else if (pWnd == m_pBtTrainingConfirm)
            {
                PetSkillFrame* pFrame = (PetSkillFrame*)m_pFrameMgr->GetFrame(_T("PetSkillFrame"));
                if (!P_VALID(pFrame))
                    return 0;

                // 确定
                for (int i = 0; i < ESSTE_End; ++i)
                {
                    if (P_VALID(m_pPbtComposeSkill[i])
                        && EGUIBS_PushDown == m_pPbtComposeSkill[i]->GetState())
                    {
                        const tagPetSkillData* pSkillData = m_pPetSkillTraining->GetSkillData();
                        if (P_VALID(pSkillData))
                        {
                            PetManager::Inst()->TryToTraining(m_CurrPetID, pFrame->GetComposeSkill((ESkillTypeEx2)i), pSkillData->dwPetSkillID);
                        }

                        return 0;
                    }
                }
            }
            else if (m_pPetSkillGather == pWnd)
            {
                const tagPetSkillData* pSkillData = m_pPetSkillGather->GetSkillData();
                if (P_VALID(pSkillData))
                {
                    PetManager::Inst()->TryToUsePetSkill(m_CurrPetID, pSkillData->dwPetSkillID);
                }
                break;
            }
            else if (m_pPetSkillSale == pWnd)
            {
                const tagPetSkillData* pSkillData = m_pPetSkillSale->GetSkillData();
                if (P_VALID(pSkillData))
                {
                    PetManager::Inst()->TryToUsePetSkill(m_CurrPetID, pSkillData->dwPetSkillID);
                }
                break;
            }
            else if (m_pPetLearnGather == pWnd)
            {
                if (m_pPetLearnGather->GetText() == g_StrTable[_T("PetAssistantMsgLearn")])
                {
                    const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220501);
                    if (P_VALID(pItem))
                    {
                        MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                        MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                    }
                }
                else
                {
                    const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220502);
                    if (P_VALID(pItem))
                    {
                        MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                        MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                    }
                }
            }
            else if (m_pPetLearnTraining == pWnd)
            {
                if (m_pPetLearnTraining->GetText() == g_StrTable[_T("PetAssistantMsgLearn")])
                {
                    const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220601);
                    if (P_VALID(pItem))
                    {
                        MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                        MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                    }
                }
                else
                {
                    const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220602);
                    if (P_VALID(pItem))
                    {
                        MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                        MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                    }
                }
            }
            else if (m_pPetLearnPickUp == pWnd)
            {
                if (m_pPetLearnPickUp->GetText() == g_StrTable[_T("PetAssistantMsgLearn")])
                {
                    const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220301);
                    if (P_VALID(pItem))
                    {
                        MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                        MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                    }
                }
                else
                {
                    const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220302);
                    if (P_VALID(pItem))
                    {
                        MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                        MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                    }
                }
            }
            else if (m_pPetLearnMedicare == pWnd)
            {
                const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220401);
                if (P_VALID(pItem))
                {
                    MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                    MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                }
            }
            else if (m_pPetLearnSale == pWnd)
            {
                const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByTypeid(4220701);
                if (P_VALID(pItem))
                {
                    MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pItem->dwID);
                    MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                }
            }
        }
        break;
    case EGUIE_PushButtonDown:
        {
            if (pWnd == m_pPbtFight)
            {
                m_pPchFight->SetInvisible(FALSE);
                m_pPchProduction->SetInvisible(TRUE);
                m_pPchOther->SetInvisible(TRUE);
            }
            else if (pWnd == m_pPbtProduction)
            {
                m_pPchFight->SetInvisible(TRUE);
                m_pPchProduction->SetInvisible(FALSE);
                m_pPchOther->SetInvisible(TRUE);
            }
            else if (pWnd == m_pPbtOther)
            {
                m_pPchFight->SetInvisible(TRUE);
                m_pPchProduction->SetInvisible(TRUE);
                m_pPchOther->SetInvisible(FALSE);
            }
            else if (pWnd == m_pPbtRedEnable)
            {
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.bHPEnabled = (m_pPbtRedEnable->GetState() == EGUIBS_PushDown);
                }
            }
            else if (pWnd == m_pPbtBlueEnable)
            {
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.bisMPEnabled = (m_pPbtBlueEnable->GetState() == EGUIBS_PushDown);
                }
            }
            else if (pWnd == m_pPbtToughnessEnable)
            {
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.bisTouEnabled = (m_pPbtToughnessEnable->GetState() == EGUIBS_PushDown);
                }
            }

            tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
            if (P_VALID(pPet))
            {
                if (pWnd == m_pBtPickUpFunc[0])
                {
                    pPet->skillPickupCfg.bMoney = (m_pBtPickUpFunc[0]->GetState() == EGUIBS_PushDown);
                }
                else if (pWnd == m_pBtPickUpFunc[1])
                {
                    pPet->skillPickupCfg.bTrophy = (m_pBtPickUpFunc[1]->GetState() == EGUIBS_PushDown);
                }
                else if (pWnd == m_pBtPickUpFunc[2])
                {
                    pPet->skillPickupCfg.bMaterial = (m_pBtPickUpFunc[2]->GetState() == EGUIBS_PushDown);
                }
                else if (pWnd == m_pBtPickUpFunc[3])
                {
                    pPet->skillPickupCfg.bEquip = (m_pBtPickUpFunc[3]->GetState() == EGUIBS_PushDown);
                }
                else if (pWnd == m_pBtPickUpFunc[4])
                {
                    pPet->skillPickupCfg.bMedicine = (m_pBtPickUpFunc[4]->GetState() == EGUIBS_PushDown);
                }
                else if (pWnd == m_pBtPickUpFunc[5])
                {
                    pPet->skillPickupCfg.bOthers = (m_pBtPickUpFunc[5]->GetState() == EGUIBS_PushDown);
                }
            }
        }
        break;
    case EGUIE_ItemClick:
        {
            if (pWnd == m_pCbPetList->GetListBox())
            {
                int m_nSelectedRow = pEvent->dwParam1;
                map<int, DWORD>::iterator it = m_mapRow2Pet.find(m_nSelectedRow);
                if (it != m_mapRow2Pet.end())
                {
                    m_CurrPetID = it->second;

                    // 召唤
                    PetManager::Inst()->BeginSetPetState(m_CurrPetID, EPS_Called, true);

                    // 刷新当前技能列表
                    RefreshPetSkill();
                }
            }
            if (pWnd == m_pCbRedCondition->GetListBox())
            {
                int row = pEvent->dwParam1;
                int nlimit = 10;
                tstring str = m_pCbRedCondition->GetListBox()->GetText(row, 0);
                tstringstream tsstream;
                tsstream << str.substr(0, 2);
                tsstream >> nlimit;

                // save config
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.u8MinHP = nlimit;
                }
            }
            else if (pWnd == m_pCbBlueCondition->GetListBox())
            {
                int row = pEvent->dwParam1;
                int nlimit = 10;
                tstring str = m_pCbBlueCondition->GetListBox()->GetText(row, 0);
                tstringstream tsstream;
                tsstream << str.substr(0, 2);
                tsstream >> nlimit;

                // save config
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.u8MinMP = nlimit;
                }
            }
            else if (pWnd == m_pCbToughnessCondition->GetListBox())
            {
                int row = pEvent->dwParam1;
                int nlimit = 10;
                tstring str = m_pCbToughnessCondition->GetListBox()->GetText(row, 0);
                tstringstream tsstream;
                tsstream << str.substr(0, 2);
                tsstream >> nlimit;

                // save config
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    pPet->skillMedicineFeedCfg.u8Toughness = nlimit;
                }
            }
        }
        break;
    case EGUISCB_Drop:
        {
            PetSkillFrame* pFrame = (PetSkillFrame*)m_pFrameMgr->GetFrame(_T("PetSkillFrame"));
            if (!P_VALID(pFrame))
                return 0;

            if (pWnd == m_pBtRed)
            {
                Item* item = ItemMgr::Inst()->GetPocketItem(static_cast<INT16>(LOWORD(pEvent->dwParam2)));
                if (P_VALID(item) && item->GetProto()->nPrepareTime == 0)
                {
                    if (pFrame->IfCanUseHPItem(item))
                    {
                        m_pBtRed->RefreshItem(item->GetItemTypeID(), item->GetItemQuantity(), item->GetItemQuality());
                        tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                        if (P_VALID(pPet))
                        {
                            pPet->skillMedicineFeedCfg.dwHPItemID   =   item->GetItemId();
                        }
                    }
                }
            }
            else if (pWnd == m_pBtBlue)
            {
                Item* item = ItemMgr::Inst()->GetPocketItem(static_cast<INT16>(LOWORD(pEvent->dwParam2)));
                if (P_VALID(item) && item->GetProto()->nPrepareTime == 0)
                {
                    if (pFrame->IfCanUseMPItem(item))
                    {
                        m_pBtBlue->RefreshItem(item->GetItemTypeID(), item->GetItemQuantity(), item->GetItemQuality());
                        tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                        if (P_VALID(pPet))
                        {
                            pPet->skillMedicineFeedCfg.dwMPItemID   =   item->GetItemId();
                        }
                    }
                }
            }
            else if (pWnd == m_pBtToughness)
            {
                Item* item = ItemMgr::Inst()->GetPocketItem(static_cast<INT16>(LOWORD(pEvent->dwParam2)));
                if (P_VALID(item) && item->GetProto()->nPrepareTime == 0)
                {
                    if (pFrame->IfCanUseToughnessItem(item))
                    {
                        m_pBtToughness->RefreshItem(item->GetItemTypeID(), item->GetItemQuantity(), item->GetItemQuality());
                        tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                        if (P_VALID(pPet))
                        {
                            pPet->skillMedicineFeedCfg.dwTouItemID   =   item->GetItemId();
                        }
                    }
                }
            }
        }
        break;
    case EGUIE_MoveIntoCtrl:
        {
            PetSkillButton* pSkillButton = NULL;

            if (m_pPetSkillGather == pWnd)
                pSkillButton = m_pPetSkillGather;
            else if (m_pPetSkillTraining == pWnd)
                pSkillButton = m_pPetSkillTraining;
            else if (m_pPetSkillPickUp == pWnd)
                pSkillButton = m_pPetSkillPickUp;
            else if (m_pPetSkillMedicare == pWnd)
                pSkillButton = m_pPetSkillMedicare;
            else if (m_pPetSkillSale == pWnd)
                pSkillButton = m_pPetSkillSale;
            else if(m_pBtRed == pWnd)
            {
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    Item* pItem = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pPet->skillMedicineFeedCfg.dwHPItemID);
                    if (P_VALID(pItem))
                    {
                        ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
                    }
                }
            }
            else if(m_pBtBlue == pWnd)
            {
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    Item* pItem = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pPet->skillMedicineFeedCfg.dwMPItemID);
                    if (P_VALID(pItem))
                    {
                        ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
                    }
                }
            }
            else if(m_pBtToughness == pWnd)
            {
                tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
                if (P_VALID(pPet))
                {
                    Item* pItem = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pPet->skillMedicineFeedCfg.dwTouItemID);
                    if (P_VALID(pItem))
                    {
                        ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
                    }
                }
            }

            if (P_VALID(pSkillButton))
            {
                const tagPetSkillData* pSkillData = pSkillButton->GetSkillData();
                if (P_VALID(pSkillData))
                {
                    ShowTip(pWnd, ToolTipCreator::Inst()->GetPetSkillTips(pSkillData->dwPetSkillID).c_str());
                }
                break;
            }
        }
        break;
    case EGUIE_MoveOutOfCtrl:
        {
            ShowTip(pWnd, NULL);
        }
        break;
    case EGUIE_Active:
        if (pWnd == m_pCbPetList->GetEditBox())
            m_pGUI->SetActive( m_pCbPetList->GetListBox());
        else if( pWnd == m_pCbRedCondition->GetEditBox() )
            m_pGUI->SetActive( m_pCbRedCondition->GetListBox() );
        else if( pWnd == m_pCbBlueCondition->GetEditBox() )
            m_pGUI->SetActive( m_pCbBlueCondition->GetListBox() );
        else if( pWnd == m_pCbToughnessCondition->GetEditBox() )
            m_pGUI->SetActive( m_pCbToughnessCondition->GetListBox() );
        break;
    case EGUIE_Drag:
        if (m_pPetSkillGather == pWnd
            || m_pPetSkillTraining == pWnd
            || m_pPetSkillPickUp == pWnd
            || m_pPetSkillMedicare == pWnd
            || m_pPetSkillSale == pWnd)
        {
            PetSkillButton* pSkillButton = (PetSkillButton*)pWnd;
            DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
            if (P_VALID(pDrag))
            {
                const tagPetSkillData* pSkillData = pSkillButton->GetSkillData();
                if (P_VALID(pSkillData) && EPCT_Active == pSkillData->eCastType)
                {
                    //发送游戏事件
                    DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
                    if (P_VALID(pDrag))
                    {
                        tagGUIEvent event(pSkillButton, EGUIE_Drag, NULL);
                        POINT pt;
                        ::GetCursorPos(&pt);
                        event.dwParam1 = EDT_PetSkill;
                        event.dwParam2 = m_CurrPetID;
                        event.dwParam3 = pSkillData->dwPetSkillID;
                        event.dwParam4 = MAKELONG(pt.x, pt.y);
                        pDrag->OnEvent(&event);
                    }
                }
            }
        }
        break;
    }

    return 0;
}

void PetSkillAssistanceFrame::RefreshPets()
{
    // 清空combobx
    m_pCbPetList->GetListBox()->Clear();
    m_nSelectedRow  = GT_INVALID;
    m_CurrPetID     = GT_INVALID;

    // 清空对照表
    m_mapRow2Pet.clear();

    // 得到所有灵兽，填充到combobox
    const PetManager::_id2pet* pPetList = PetManager::Inst()->GetPetList();
    if (P_VALID(pPetList))
    {
        int i = 0;

        for (PetManager::_id2pet::const_iterator it = pPetList->begin();
            it != pPetList->end(); ++it)
        {
            m_pCbPetList->GetListBox()->SetText(i, 0, it->second->strName.c_str());
            m_mapRow2Pet.insert(make_pair(i, it->first));

            if (it->second->state.bisCalled)
            {
                m_nSelectedRow  = i;
                m_CurrPetID     = it->first;
                m_pCbPetList->GetEditBox()->SetText(it->second->strName.c_str());
            }
            ++i;
        }

        // 调整大小
        ResizeCombobox(m_pCbPetList, pPetList->size());
    }
}

void PetSkillAssistanceFrame::RefreshPetSkill()
{
    // 清空技能
    ClearSkill();

    // 按类型取得所有灵兽技能

    int i = 0;

    // 采集
    const tagPetSkill* pPetSkill = PetManager::Inst()->FindPetSkillbyType(m_CurrPetID, EPT_Gather);
    if (P_VALID(pPetSkill))
    {
        m_pPetSkillGather->RefrashSkill(m_CurrPetID, pPetSkill->dwTypeID);

        TurnOnSkillUI(EPT_Gather, pPetSkill->dwTypeID, true);

        ++i;
    }

    // 训练
    pPetSkill = PetManager::Inst()->FindPetSkillbyType(m_CurrPetID, EPT_Experience);
    if (P_VALID(pPetSkill))
    {
        m_pPetSkillTraining->RefrashSkill(m_CurrPetID, pPetSkill->dwTypeID);

        TurnOnSkillUI(EPT_Experience, pPetSkill->dwTypeID, true);

        ++i;
    }

    // 自动拾取
    pPetSkill = PetManager::Inst()->FindPetSkillbyType(m_CurrPetID, EPT_PickUp);
    if (P_VALID(pPetSkill))
    {
        m_pPetSkillPickUp->RefrashSkill(m_CurrPetID, pPetSkill->dwTypeID);

        TurnOnSkillUI(EPT_PickUp, pPetSkill->dwTypeID, true);

        ++i;
    }

    // 护理
    pPetSkill = PetManager::Inst()->FindPetSkillbyType(m_CurrPetID, EPT_MedicineFeed);
    if (P_VALID(pPetSkill))
    {
        m_pPetSkillMedicare->RefrashSkill(m_CurrPetID, pPetSkill->dwTypeID);

        TurnOnSkillUI(EPT_MedicineFeed, pPetSkill->dwTypeID, true);

        ++i;
    }

    // 贩卖
    pPetSkill = PetManager::Inst()->FindPetSkillbyType(m_CurrPetID, EPT_Sale);
    if (P_VALID(pPetSkill))
    {
        m_pPetSkillSale->RefrashSkill(m_CurrPetID, pPetSkill->dwTypeID);

        TurnOnSkillUI(EPT_Sale, pPetSkill->dwTypeID, true);

        ++i;
    }

    tstringstream tss;
    tss << MAX_PET_ONLINE_SKILL - i;
    m_pStcSkillLeft->SetText(tss.str().c_str());

}

void PetSkillAssistanceFrame::InitalUI()
{
    // 填充combobox
    for (int i = 0; i < 9; ++i)
    {
        tstringstream tsstream;
        tsstream << (i + 1) * 10 << _T('%');
        m_pCbRedCondition->GetListBox()->SetText(i, 0, tsstream.str().c_str());
        m_pCbBlueCondition->GetListBox()->SetText(i, 0, tsstream.str().c_str());
        m_pCbToughnessCondition->GetListBox()->SetText(i, 0, tsstream.str().c_str());
    }

    // 调整大小
    ResizeCombobox(m_pCbRedCondition, 9);
    ResizeCombobox(m_pCbBlueCondition, 9);
    ResizeCombobox(m_pCbToughnessCondition, 9);

    // 默认打开第一个选项卡
    m_pPbtFight->SetState(EGUIBS_PushDown);

    // 清空技能
    ClearSkill();
}

void PetSkillAssistanceFrame::ResizeCombobox( GUIComboBox* pComboBox, int rowCount )
{
    tagPoint ptSize	= pComboBox->GetListBox()->GetSize();

    ptSize.x = pComboBox->GetEditBox()->GetSize().x;
    ptSize.y = pComboBox->GetListBox()->GetRowHeight() * rowCount;

    pComboBox->GetListBox()->SetResizable(TRUE);
    pComboBox->GetListBox()->SetColWidth(0, ptSize.x);
    pComboBox->GetListBox()->Resize(ptSize);
    pComboBox->GetListBox()->SetResizable(FALSE);
}

DWORD PetSkillAssistanceFrame::OnPetAttrChange( tagPetAttrChange* pEvent )
{
    switch (pEvent->eType)
    {
    case EPACT_Init:
        RefreshPets();
        break;
    case EPACT_Remove:
        RefreshPets();
        break;
    case EPACT_PetValue:
        RefreshPets();
        break;
    case EPACT_PetState:
        RefreshPets();
        break;
    case EPACT_PetDisp:
        RefreshPets();
        break;
    case EPACT_PetEquip:
        RefreshPets();
        break;
    case EPACT_PetUnEquip:
        RefreshPets();
        break;
    case EPACT_PetSkill:
        RefreshPets();
        break;
    }

    return 0;
}

void PetSkillAssistanceFrame::ClearSkill()
{
    m_pPetSkillGather->RefrashSkill(GT_INVALID, GT_INVALID);
    m_pPetSkillTraining->RefrashSkill(GT_INVALID, GT_INVALID);
    m_pPetSkillPickUp->RefrashSkill(GT_INVALID, GT_INVALID);
    m_pPetSkillMedicare->RefrashSkill(GT_INVALID, GT_INVALID);
    m_pPetSkillSale->RefrashSkill(GT_INVALID, GT_INVALID);

    m_pBtRed->RefreshItem();
    m_pBtBlue->RefreshItem();
    m_pBtToughness->RefreshItem();

    TurnOnSkillUI(EPT_Gather, GT_INVALID, false);
    TurnOnSkillUI(EPT_Experience, GT_INVALID, false);
    TurnOnSkillUI(EPT_PickUp, GT_INVALID, false);
    TurnOnSkillUI(EPT_MedicineFeed, GT_INVALID, false);
    TurnOnSkillUI(EPT_Sale, GT_INVALID, false);
}

DWORD PetSkillAssistanceFrame::BuyEventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if (pWnd == m_pBtBuyExit || pWnd == m_pBtBuyCanel)
            {
                ShowBuyWnd(false);
            }
            else if (pWnd == m_pBtBuyConfirm)
            {
                // 购买
                MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(m_dwItemID[m_nCurrItem]);
                MallMgr::Inst()->GetMallFrame()->BuyMallItem();
            }
            else
            {
                for (int i = 0; i < MAX_PET_EGG_BUY; ++i)
                {
                    if (pWnd == m_pIbtEgg[i])
                    {
                        m_nCurrItem = i;
                        RefreshItemBuy();
                        
                        return 0;
                    }
                }
            }
        }
        break;
    case EGUIE_Drag:
        {
            for (int i = 0; i < MAX_PET_EGG_BUY; ++i)
            {
                if (pWnd == m_pIbtEgg[i])
                {
                    m_nCurrItem = i;
                    RefreshItemBuy();

                    return 0;
                }
            }
        }
        break;
    }

    return 0;
}

void PetSkillAssistanceFrame::ShowBuyWnd( bool visable )
{
    m_pWndBuy->SetInvisible(!visable);

    if (visable)
    {
        m_pWndBuy->FlipToTop();
        LoadPets();
        RefreshItemBuy();
    }
    else
    {

    }
}

void PetSkillAssistanceFrame::LoadPets()
{
    // 清空所有
    ClearBuyUI();

    const vector<DWORD>& vecItems = PetProtoData::Inst()->GetPetShopList();
    for (int i = 0; i < vecItems.size(); ++i)
    {
        // 是否在商城中
        const tagMallItemProto* pItemProto = MallMgr::Inst()->GetMallItemByTypeid(vecItems[i]);
        if (P_VALID(pItemProto))
        {
            m_pIbtEgg[i]->RefreshItem(vecItems[i]);
            m_dwItemID[i] = pItemProto->dwID;

            // 价格
            tstringstream tss;
            tss << pItemProto->nPrice;
            m_pStcPrice[i]->SetText(tss.str().c_str());
        }
    }
}

void PetSkillAssistanceFrame::ClearBuyUI()
{
    m_pStcTitle->SetText(g_StrTable[_T("PetAssistantMsgSelect")]);

    for (int i = 0; i < MAX_PET_EGG_BUY; ++i)
    {
        m_pIbtEgg[i]->RefreshItem();
        m_pStcPrice[i]->SetText(_T(""));
        m_dwItemID[i] = GT_INVALID;
    }
}

void PetSkillAssistanceFrame::TurnOnSkillUI( EPetskillType eType, DWORD dwTypeID, bool bisOpen )
{
    switch (eType)
    {
    case EPT_Gather:
        {
            // 采集
            if (bisOpen)
            {
                if (dwTypeID == 1300801)
                {
                    m_pPetLearnGather->SetText(g_StrTable[_T("PetAssistantMsgLevelUp")]);
                    m_pPetLearnGather->SetEnable(true);
                }
                else
                {
                    m_pPetLearnGather->SetText(g_StrTable[_T("PetAssistantMsgLearned")]);
                    m_pPetLearnGather->SetEnable(false);
                }
            }
            else
            {
                m_pPetLearnGather->SetText(g_StrTable[_T("PetAssistantMsgLearn")]);
                m_pPetLearnGather->SetEnable(true);
            }
        }
        break;
    case EPT_Experience:
        {
            // 训练
            m_pPbtComposeSkill[ESSTE2_Smilt]->SetEnable(bisOpen);
            m_pPbtComposeSkill[ESSTE2_Artisan]->SetEnable(bisOpen);
            m_pPbtComposeSkill[ESSTE2_DanTraining]->SetEnable(bisOpen);
            m_pPbtComposeSkill[ESSTE2_Smith]->SetEnable(bisOpen);
            m_pPbtComposeSkill[ESSTE2_Casting]->SetEnable(bisOpen);
            m_pPbtComposeSkill[ESSTE2_Dressmaker]->SetEnable(bisOpen);
            m_pPbtComposeSkill[ESSTE2_Aechnics]->SetEnable(bisOpen);
            m_pPbtComposeSkill[ESSTE2_PointUp]->SetEnable(bisOpen);

            m_pBtTrainingConfirm->SetEnable(bisOpen);

            if (bisOpen)
            {
                if (dwTypeID == 1300401)
                {
                    m_pPetLearnTraining->SetText(g_StrTable[_T("PetAssistantMsgLevelUp")]);
                    m_pPetLearnTraining->SetEnable(true);
                }
                else
                {
                    m_pPetLearnTraining->SetText(g_StrTable[_T("PetAssistantMsgLearned")]);
                    m_pPetLearnTraining->SetEnable(false);
                }
            }
            else
            {
                m_pPetLearnTraining->SetText(g_StrTable[_T("PetAssistantMsgLearn")]);
                m_pPetLearnTraining->SetEnable(true);
            }
        }
    	break;
    case EPT_PickUp:
        {
            // 拾取
            for (int i = 0; i < MAX_PICKUP_FUNC; ++i)
            {
                m_pBtPickUpFunc[i]->SetEnable(bisOpen);
            }

            if (bisOpen)
            {
                if (dwTypeID == 1300101)
                {
                    m_pPetLearnPickUp->SetText(g_StrTable[_T("PetAssistantMsgLevelUp")]);
                    m_pPetLearnPickUp->SetEnable(true);
                }
                else
                {
                    m_pPetLearnPickUp->SetText(g_StrTable[_T("PetAssistantMsgLearned")]);
                    m_pPetLearnPickUp->SetEnable(false);
                }
            }
            else
            {
                m_pPetLearnPickUp->SetText(g_StrTable[_T("PetAssistantMsgLearn")]);
                m_pPetLearnPickUp->SetEnable(true);
            }

            LoadPetSkillCfg(EPT_PickUp);
        }
        break;
    case EPT_MedicineFeed:
        {
            // 护理
            m_pCbRedCondition->GetButton()->SetEnable(bisOpen);
            m_pPbtRedEnable->SetEnable(bisOpen);
            m_pBtRed->SetEnable(bisOpen);

            m_pCbBlueCondition->GetButton()->SetEnable(bisOpen);
            m_pPbtBlueEnable->SetEnable(bisOpen);
            m_pBtBlue->SetEnable(bisOpen);

            m_pCbToughnessCondition->GetButton()->SetEnable(bisOpen);
            m_pPbtToughnessEnable->SetEnable(bisOpen);
            m_pBtToughness->SetEnable(bisOpen);

            if (bisOpen)
            {
                m_pPetLearnMedicare->SetText(g_StrTable[_T("PetAssistantMsgLearned")]);
                m_pPetLearnMedicare->SetEnable(false);
            }
            else
            {
                m_pPetLearnMedicare->SetText(g_StrTable[_T("PetAssistantMsgLearn")]);
                m_pPetLearnMedicare->SetEnable(true);
            }

            LoadPetSkillCfg(EPT_MedicineFeed);
        }
        break;
    case EPT_Sale:
        {
            // 贩卖

            if (bisOpen)
            {
                m_pPetLearnSale->SetText(g_StrTable[_T("PetAssistantMsgLearned")]);
                m_pPetLearnSale->SetEnable(false);
            }
            else
            {
                m_pPetLearnSale->SetText(g_StrTable[_T("PetAssistantMsgLearn")]);
                m_pPetLearnSale->SetEnable(true);
            }
        }
        break;
    }
}

void PetSkillAssistanceFrame::RefreshItemBuy()
{
    for (int i = 0; i < MAX_PET_EGG_BUY; ++i)
    {
        if (i == m_nCurrItem)
        {
            m_pStcSelect[i]->SetPicFile(STR_SELECT_GOOD);
        }
        else
        {
            m_pStcSelect[i]->SetPicFile(STR_BACK_GOOD);
        }
    }
}

void PetSkillAssistanceFrame::LoadPetSkillCfg( EPetskillType ePetSkillType )
{
    switch (ePetSkillType)
    {
    case EPT_PickUp:
        {
            // 拾取
            tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
            if (P_VALID(pPet))
            {
                m_pBtPickUpFunc[0]->SetState(pPet->skillPickupCfg.bMoney      ? EGUIBS_PushDown : EGUIBS_PopUp);
                m_pBtPickUpFunc[1]->SetState(pPet->skillPickupCfg.bTrophy     ? EGUIBS_PushDown : EGUIBS_PopUp);
                m_pBtPickUpFunc[2]->SetState(pPet->skillPickupCfg.bMaterial   ? EGUIBS_PushDown : EGUIBS_PopUp);
                m_pBtPickUpFunc[3]->SetState(pPet->skillPickupCfg.bEquip      ? EGUIBS_PushDown : EGUIBS_PopUp);
                m_pBtPickUpFunc[4]->SetState(pPet->skillPickupCfg.bMedicine   ? EGUIBS_PushDown : EGUIBS_PopUp);
                m_pBtPickUpFunc[5]->SetState(pPet->skillPickupCfg.bOthers     ? EGUIBS_PushDown : EGUIBS_PopUp);
            }
        }
        break;
    case EPT_MedicineFeed:
        {
            // 喂药
            tagPet* pPet = const_cast<tagPet*>(PetManager::Inst()->GetPet(m_CurrPetID));
            if (P_VALID(pPet))
            {
                // 开关
                m_pPbtRedEnable->SetState(pPet->skillMedicineFeedCfg.bHPEnabled ? EGUIBS_PushDown : EGUIBS_PopUp);
                m_pPbtBlueEnable->SetState(pPet->skillMedicineFeedCfg.bisMPEnabled ? EGUIBS_PushDown : EGUIBS_PopUp);
                m_pPbtToughnessEnable->SetState(pPet->skillMedicineFeedCfg.bisTouEnabled ? EGUIBS_PushDown : EGUIBS_PopUp);

                // 体力
                Item* pItem = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pPet->skillMedicineFeedCfg.dwHPItemID);
                if (P_VALID(pItem))
                {
                    m_pBtRed->RefreshItem(pItem->GetItemTypeID(), pItem->GetItemQuantity(), pItem->GetItemQuality());
                }
                else
                {
                    m_pBtRed->RefreshItem();
                }
                // 真气
                pItem = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pPet->skillMedicineFeedCfg.dwMPItemID);
                if (P_VALID(pItem))
                {
                    m_pBtBlue->RefreshItem(pItem->GetItemTypeID(), pItem->GetItemQuantity(), pItem->GetItemQuality());
                }
                else
                {
                    m_pBtBlue->RefreshItem();
                }
                // 持久力
                pItem = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pPet->skillMedicineFeedCfg.dwTouItemID);
                if (P_VALID(pItem))
                {
                    m_pBtToughness->RefreshItem(pItem->GetItemTypeID(), pItem->GetItemQuantity(), pItem->GetItemQuality());
                }
                else
                {
                    m_pBtToughness->RefreshItem();
                }
                // 限制
                if (pPet->skillMedicineFeedCfg.u8MinHP != 0)
                {
                    tstringstream tsstream;
                    tsstream << pPet->skillMedicineFeedCfg.u8MinHP << _T('%');
                    m_pCbRedCondition->GetEditBox()->SetText(tsstream.str().c_str());
                }
                else
                {
                    m_pCbRedCondition->GetEditBox()->SetText(_T(""));
                }

                if (pPet->skillMedicineFeedCfg.u8MinMP != 0)
                {
                    tstringstream tsstream;
                    tsstream << pPet->skillMedicineFeedCfg.u8MinMP << _T('%');
                    m_pCbBlueCondition->GetEditBox()->SetText(tsstream.str().c_str());
                }
                else
                {
                    m_pCbBlueCondition->GetEditBox()->SetText(_T(""));
                }

                if (pPet->skillMedicineFeedCfg.u8Toughness != 0)
                {
                    tstringstream tsstream;
                    tsstream << pPet->skillMedicineFeedCfg.u8Toughness << _T('%');
                    m_pCbToughnessCondition->GetEditBox()->SetText(tsstream.str().c_str());
                }
                else
                {
                    m_pCbToughnessCondition->GetEditBox()->SetText(_T(""));
                }
            }
        }
        break;
    }
}

bool PetSkillAssistanceFrame::IsOpen()
{
    if (!P_VALID(m_pWnd))
        return false;

    return !m_pWnd->IsInvisible();
}