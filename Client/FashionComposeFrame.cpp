#include "StdAfx.h"
#include "FashionComposeFrame.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "QuestMgr.h"
#include "RoleMgr.h"
#include "SkillProtoData.h"
#include "ItemFrame.h"
#include "Item.h"

FashionComposeFrame::FashionComposeFrame(void) :
    m_Trunk(this),
    m_pCurrFashionID(GT_INVALID),
    m_dwStartTime(0),
    m_bBegin(false),
    m_updateTimeDistance(0)
{
}

FashionComposeFrame::~FashionComposeFrame(void)
{
}

BOOL FashionComposeFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    //! 注册网络事件
    m_pCmdMgr->Register("NS_ConsolidateSpin", (NETMSGPROC)m_Trunk.sfp2(&FashionComposeFrame::OnNS_ConsolidateSpin),  _T("NS_ConsolidateSpin"));

    //! 注册游戏事件

    return bRet;
}

BOOL FashionComposeFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    //! 注销网络事件
    m_pCmdMgr->UnRegister("NS_ConsolidateSpin", (NETMSGPROC)m_Trunk.sfp2(&FashionComposeFrame::OnNS_ConsolidateSpin));

    //! 销毁窗体
    if( P_VALID(m_pWnd) )
    {
        m_pGUI->AddToDestroyList(m_pWnd);
        m_pWnd = NULL;
    }

    return bRet;
}

BOOL FashionComposeFrame::ReloadUI()
{
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\cloth_compose.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    // 更改控件
    m_pGUI->ChangeXml(&ele, "cloth_composewin\\cloth_composepic\\decompositionback\\itemback\\item", "ClassName", "ItemButton");
    m_pGUI->ChangeXml(&ele, "cloth_composewin\\cloth_composepic\\decompositionback\\backpic\\itemback\\item", "ClassName", "ItemButton");

    // 创建
    m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

    // 注册事件处理函数
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FashionComposeFrame::EventHandler));

    // 获得控件
    m_pIbtFashion       = static_cast<ItemButton*>(m_pWnd->GetChild(_T("cloth_composepic\\decompositionback\\itemback\\item")));
    m_pProgress         = static_cast<GUIProgress*>(m_pWnd->GetChild(_T("cloth_composepic\\decompositionback\\timepic\\time")));

    m_pIbtMateiral      = static_cast<ItemButton*>(m_pWnd->GetChild(_T("cloth_composepic\\decompositionback\\backpic\\itemback\\item")));
    m_pBtInc            = static_cast<GUIButton*>(m_pWnd->GetChild(_T("cloth_composepic\\decompositionback\\backpic\\up")));
    m_pBtDec            = static_cast<GUIButton*>(m_pWnd->GetChild(_T("cloth_composepic\\decompositionback\\backpic\\down")));
    m_pStcSucessRate    = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\decompositionback\\backpic\\word5")));

    TCHAR tchBuff[256];
    for (int i = 0; i != 4; ++i)
    {
        _sntprintf(tchBuff, sizeof(tchBuff) / sizeof(TCHAR), _T("cloth_composepic\\backpic\\frm\\opbutt%d"), i);
        m_pPbtSuit[i]   = static_cast<GUIPushButton*>(m_pWnd->GetChild(tchBuff));
        _sntprintf(tchBuff, sizeof(tchBuff) / sizeof(TCHAR), _T("cloth_composepic\\backpic\\frm\\word%d"), i + 1);
        m_pStcSuit[i]   = static_cast<GUIPushButton*>(m_pWnd->GetChild(tchBuff));
    }

    m_pStcHP[0]                 = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic0\\num0")));
    m_pStcMP[0]                 = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic0\\num1")));
    m_pStcVitality[0]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic0\\num2")));

    m_pStcSavvy[0]              = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic1\\num0")));
    m_pStcFortune[0]            = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic1\\num1")));
    m_pStcRein[0]               = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic1\\num2")));

    m_pStcPhysique[0]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic2\\num0")));
    m_pStcStrength[0]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic2\\num1")));
    m_pStcTechnique[0]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic2\\num2")));
    m_pStcPneuma[0]             = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic2\\num3")));
    m_pStcInnerForce[0]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic2\\num4")));
    m_pStcAgility[0]            = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic2\\num5")));

    m_pStcExAttack[0]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num0")));
    m_pStcExDefense[0]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num1")));
    m_pStcAttackTec[0]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num2")));
    m_pStcArmor[0]              = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num3")));
    m_pStcDefenseTec[0]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num4")));
    m_pStcInAttack[0]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num5")));
    m_pStcInDefense[0]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num6")));
    m_pStcWeaponDmgMax[0]       = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num7")));
    m_pStcExDamage[0]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num8")));
    m_pStcDerateALL[0]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num9")));
    m_pStcExDamageAbsorb[0]     = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num10")));
    m_pStcHitRate[0]            = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num11")));
    m_pStcWeaponSoul[0]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num12")));
    m_pStcDodge[0]              = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num13")));
    m_pStcCritRate[0]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num14")));
    m_pStcRegainAddtion[0]               = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num15")));
    m_pStcCritAmount[0]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num16")));
    m_pStcBlockRate[0]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic0\\backpic3\\num17")));

    m_pStcHP[1]                 = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic0\\num0")));
    m_pStcMP[1]                 = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic0\\num1")));
    m_pStcVitality[1]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic0\\num2")));

    m_pStcSavvy[1]              = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic1\\num0")));
    m_pStcFortune[1]            = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic1\\num1")));
    m_pStcRein[1]               = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic1\\num2")));

    m_pStcPhysique[1]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic2\\num0")));
    m_pStcStrength[1]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic2\\num1")));
    m_pStcTechnique[1]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic2\\num2")));
    m_pStcPneuma[1]             = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic2\\num3")));
    m_pStcInnerForce[1]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic2\\num4")));
    m_pStcAgility[1]            = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic2\\num5")));

    m_pStcExAttack[1]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num0")));
    m_pStcExDefense[1]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num1")));
    m_pStcAttackTec[1]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num2")));
    m_pStcArmor[1]              = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num3")));
    m_pStcDefenseTec[1]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num4")));
    m_pStcInAttack[1]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num5")));
    m_pStcInDefense[1]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num6")));
    m_pStcWeaponDmgMax[1]       = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num7")));
    m_pStcExDamage[1]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num8")));
    m_pStcDerateALL[1]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num9")));
    m_pStcExDamageAbsorb[1]     = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num10")));
    m_pStcHitRate[1]            = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num11")));
    m_pStcWeaponSoul[1]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num12")));
    m_pStcDodge[1]              = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num13")));
    m_pStcCritRate[1]           = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num14")));
    m_pStcRegainAddtion[1]               = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num15")));
    m_pStcCritAmount[1]         = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num16")));
    m_pStcBlockRate[1]          = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("cloth_composepic\\listpic00\\backpic3\\num17")));


    m_pBtStart              = static_cast<GUIButton*>(m_pWnd->GetChild(_T("cloth_composepic\\startbutt")));
    m_pBtClose              = static_cast<GUIButton*>(m_pWnd->GetChild(_T("cloth_composepic\\closebutt")));
    m_pBtCancel             = static_cast<GUIButton*>(m_pWnd->GetChild(_T("cloth_composepic\\breakbutt")));

    // 默认关闭窗体
    m_pProgress->SetMaxValue(6000);
    m_pProgress->SetValue(0);     
    m_pProgress->SetRefresh(TRUE);

    ShowWnd(FALSE);

    ClearUI();

    return TRUE;
}

VOID FashionComposeFrame::Update()
{
    // 当窗体打开时
    if (P_VALID(m_pWnd))
    {
        if (m_pWnd->IsInvisible())
            return;
    }

    if(Kernel::Inst()->GetAccumTimeDW() - m_updateTimeDistance > 1000)//每隔1秒检测距离目标NPC的距离
    {
        if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
        {
            ShowWnd(FALSE);
            return;
        }
    }

    if (!m_bBegin)
        return;

    // 更新进度条
    m_pProgress->SetValue(Kernel::Inst()->GetAccumTimeDW() - m_dwStartTime, true);
    m_pProgress->SetRefresh();

    // 大于6秒后完成
    if (Kernel::Inst()->GetAccumTimeDW() - m_dwStartTime > 6000)
    {
        // 开始精纺
        tagNC_ConsolidateSpin msg;
        msg.dwNPCID     = m_dwNpcId;
        msg.n64ItemID   = m_pCurrFashionID;
        msg.eSpinType   = GetSelectedFashionSuitType();
        msg.dwStuffTypeID   = m_pCurrMatearialType;
        msg.dwStuffItemNum  = m_pMatearialCount;

        m_pSession->Send(&msg);

        m_bBegin = false;
    }
}

VOID FashionComposeFrame::Render3D()
{

}

BOOL FashionComposeFrame::EscCancel()
{
    if( m_pWnd->IsInvisible() )
        return FALSE;

    ShowWnd(FALSE);

    return TRUE;
}

DWORD FashionComposeFrame::EventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if (pWnd == m_pBtStart)
            {
                if (m_pCurrFashionID == GT_INVALID || m_pCurrMatearialType == GT_INVALID)
                {
                    CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionCompose6")]);
                    return 0;
                }

                m_pBtStart->SetEnable(false);
                m_pBtClose->SetEnable(false);
                m_pBtCancel->SetEnable(false);

                for (int i = 0; i != 4; ++i)
                {
                    m_pPbtSuit[i]->SetEnable(false);
                }

                m_pProgress->SetMaxValue(6000);
                m_pProgress->SetSpeed(100000);
                m_pProgress->SetValue(0,true);
                m_pProgress->SetRefresh(TRUE);

                m_bBegin = true;
                m_dwStartTime = Kernel::Inst()->GetAccumTimeDW();
            }
            else if (pWnd == m_pBtClose || pWnd == m_pBtCancel)
            {
                // 关闭
                ShowWnd(FALSE);
            }
            else if (pWnd == m_pBtInc)
            {
                Item* item = ItemMgr::Inst()->GetPocketItemByID(m_pCurrMatearial);
                if (P_VALID(item))
                {
                    ++m_pMatearialCount;
                    if (item->GetItemQuantity() < m_pMatearialCount)
                    {
                        m_pMatearialCount = item->GetItemQuantity();
                    }

                    m_pIbtMateiral->RefreshItem(item->GetItemTypeID(), m_pMatearialCount, 0);
                    SetSuccessRate(GetSelectedFashionSpinRate());
                }
            }
            else if (pWnd == m_pBtDec)
            {
                Item* item = ItemMgr::Inst()->GetPocketItemByID(m_pCurrMatearial);
                if (P_VALID(item))
                {
                    --m_pMatearialCount;
                    if (m_pMatearialCount < 1)
                    {
                        m_pMatearialCount = 1;
                    }

                    m_pIbtMateiral->RefreshItem(item->GetItemTypeID(), m_pMatearialCount, 0);
                    SetSuccessRate(GetSelectedFashionSpinRate());
                } 
            }
            else if (pWnd == m_pIbtFashion)
            {
                m_pCurrFashionID = GT_INVALID;
                m_pIbtFashion->RefreshItem();
                ClearUI();
            }
            else if (pWnd == m_pIbtMateiral)
            {
                m_pCurrMatearial = GT_INVALID;
                m_pIbtMateiral->RefreshItem();
            }
        }
        break;
    case EGUISCB_Drop:
        {
            if (m_pIbtFashion == pWnd)
            {
                if (EDT_Item == pEvent->dwParam1)
                {
                    Item* item = ItemMgr::Inst()->GetPocketItem(static_cast<INT16>(LOWORD(pEvent->dwParam2)));

                    if (P_VALID(item))
                    {
                        if (MIsEquipment(item->GetItemTypeID()) && item->GetProto()->eTypeReserved == 21)
                        {
                            m_pCurrFashionID = item->GetItemId();
                            m_pIbtFashion->RefreshItem(item->GetItemTypeID(), 1, 0);

                            RefreshSuiteState();
                            RefreshSuiteAttrBegin((int)GetCurrFashionSuitType() - 1);
                            RefreshSuiteAttrEnd((int)GetSelectedFashionSuitType() - 1);

                            SetSuccessRate(GetSelectedFashionSpinRate());
                        }
                        else
                        {
                            CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionCompose7")]);
                        }

                    }
                }
            }
            else if (m_pIbtMateiral == pWnd)
            {
                if (EDT_Item == pEvent->dwParam1)
                {
                    Item* item = ItemMgr::Inst()->GetPocketItem(static_cast<INT16>(LOWORD(pEvent->dwParam2)));

                    if (P_VALID(item) && EISF_SpinFashionDressItem == item->GetItemSpecFunc())
                    {
                        m_pCurrMatearial = item->GetItemId();
                        m_pCurrMatearialType = item->GetItemTypeID();
                        int nCount = item->GetItemQuantity();
                        ESpinStep eStep = GetCurrFashionSpinSetp();
                        if (ESpinStep::ESpinStep_Null == eStep)
                        {
                            m_pMatearialCount = 10;
                        }
                        else if (ESpinStep_Basic == eStep)
                        {
                            m_pMatearialCount = 30;
                        }
                        else if (ESpinStep_Middle == eStep)
                        {
                            m_pMatearialCount = 60;
                        }
                        else
                        {
                            m_pMatearialCount = 10;
                        }

                        if (m_pMatearialCount > nCount)
                        {
                            m_pMatearialCount = nCount;
                        }

                        m_pIbtMateiral->RefreshItem(item->GetItemTypeID(), m_pMatearialCount, 0);
                        

                        SetSuccessRate(GetSelectedFashionSpinRate());
                    }
                    else
                    {
                        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionCompose8")]);
                    }
                }
            }
        }
        break;
    case EGUIE_PushButtonDown:
        {
            RefreshSuiteAttrEnd((int)GetSelectedFashionSuitType() - 1);
        }
        break;
    case EGUIE_MoveIntoCtrl:
        {
            if(m_pIbtFashion == pWnd)
            {
                Item* item = ItemMgr::Inst()->GetPocketItemByID(m_pCurrFashionID);
                if (P_VALID(item))
                {
                    ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(item).c_str());
                }
            }
            else if (m_pIbtMateiral == pWnd)
            {
                Item* item = ItemMgr::Inst()->GetPocketItemByID(m_pCurrMatearial);
                if (P_VALID(item))
                {
                    ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(item).c_str());
                }
            }
        }
        break;
    case EGUIE_MoveOutOfCtrl:
        {
            ShowTip(pWnd, NULL);
        }
        break;
    }

    return 0;
}

VOID FashionComposeFrame::ShowWnd( BOOL bVal )
{
    m_dwNpcId = QuestMgr::Inst()->GetTalkNPCGlobalID();

    m_pWnd->SetInvisible(!bVal);

    if (bVal)
    {
        m_pWnd->FlipToTop();

        ClearUI();

        ItemFrame* pFrame = (ItemFrame*)m_pFrameMgr->GetFrame(_T("Item"));
        if (P_VALID(pFrame))
            pFrame->SetCurStrategy(EIST_FashionSpin);
    }
    else
    {
        ItemFrame* pFrame = (ItemFrame*)m_pFrameMgr->GetFrame(_T("Item"));
        if (P_VALID(pFrame))
            pFrame->SetCurStrategy(EIST_ItemPocket);
    }
}

VOID FashionComposeFrame::ClearUI()
{
    ClearData();
    SetSuccessRate(0, true);
    RefreshSuiteState(true);
    RefreshSuiteAttrBegin(0, true);
    RefreshSuiteAttrEnd(0, true);
}

VOID FashionComposeFrame::SetSuccessRate( int rate, bool bClear /*= false*/ )
{
    if (bClear)
    {
        m_pStcSucessRate->SetText(_T(""));
        m_pStcSucessRate->SetRefresh(TRUE);
        return;
    }

    tstringstream tss;
    tss << (float)((float)rate / 100.0f) << _T("%");
    m_pStcSucessRate->SetText(tss.str().c_str());
    m_pStcSucessRate->SetRefresh(TRUE);
}

VOID FashionComposeFrame::RefreshSuiteState( bool bClear /*= false*/ )
{
    if (bClear)
    {
        for (int i = 0; i != 4; ++i)
        {
            m_pPbtSuit[i]->SetEnable(false);
            m_pStcSuit[i]->SetText(g_StrTable[_T("FashionCompose5")]);
            m_pStcSuit[i]->SetRefresh(TRUE);
        }

        return;
    }

    for (int i = 0; i != 4; ++i)
    {
        m_pPbtSuit[i]->SetEnable(true);
        m_pStcSuit[i]->SetRefresh(TRUE);
    }

    m_pPbtSuit[0]->SetState(EGUIBS_PushDown);

    m_pStcSuit[0]->SetText(g_StrTable[_T("FashionCompose1")]);
    m_pStcSuit[0]->SetRefresh(TRUE);
    m_pStcSuit[1]->SetText(g_StrTable[_T("FashionCompose2")]);
    m_pStcSuit[1]->SetRefresh(TRUE);
    m_pStcSuit[2]->SetText(g_StrTable[_T("FashionCompose3")]);
    m_pStcSuit[2]->SetRefresh(TRUE);
    m_pStcSuit[3]->SetText(g_StrTable[_T("FashionCompose4")]);
    m_pStcSuit[3]->SetRefresh(TRUE);
}

VOID FashionComposeFrame::RefreshSuiteAttrBegin( int i, bool bClear /*= false*/ )
{
    if (bClear)
    {
        m_pStcHP[0]->SetText(_T(""));
        m_pStcMP[0]->SetText(_T(""));
        m_pStcVitality[0]->SetText(_T(""));

        m_pStcSavvy[0]->SetText(_T(""));
        m_pStcFortune[0]->SetText(_T(""));
        m_pStcRein[0]->SetText(_T(""));

        m_pStcPhysique[0]->SetText(_T(""));
        m_pStcStrength[0]->SetText(_T(""));
        m_pStcTechnique[0]->SetText(_T(""));
        m_pStcPneuma[0]->SetText(_T(""));
        m_pStcInnerForce[0]->SetText(_T(""));
        m_pStcAgility[0]->SetText(_T(""));

        m_pStcExAttack[0]->SetText(_T(""));
        m_pStcExDefense[0]->SetText(_T(""));
        m_pStcAttackTec[0]->SetText(_T(""));
        m_pStcArmor[0]->SetText(_T(""));
        m_pStcDefenseTec[0]->SetText(_T(""));
        m_pStcInAttack[0]->SetText(_T(""));
        m_pStcInDefense[0]->SetText(_T(""));
        m_pStcWeaponDmgMax[0]->SetText(_T(""));
        m_pStcExDamage[0]->SetText(_T(""));
        m_pStcDerateALL[0]->SetText(_T(""));
        m_pStcExDamageAbsorb[0]->SetText(_T(""));
        m_pStcHitRate[0]->SetText(_T(""));
        m_pStcWeaponSoul[0]->SetText(_T(""));
        m_pStcDodge[0]->SetText(_T(""));
        m_pStcCritRate[0]->SetText(_T(""));
        m_pStcRegainAddtion[0]->SetText(_T(""));
        m_pStcCritAmount[0]->SetText(_T(""));
        m_pStcBlockRate[0]->SetText(_T(""));


        GUIWnd* pWnd = m_pStcHP[0]->GetFather();
        if (P_VALID(pWnd))
        {
            pWnd->SetRefresh(TRUE);
        }

        return;
    }

    ESpinStep eSetp = GetCurrFashionSpinSetp();
    ESuitAttType eType = GetCurrFashionSuitType();
    SetFashionMsg(true, eSetp, eType);
}

VOID FashionComposeFrame::RefreshSuiteAttrEnd( int i, bool bClear /*= false*/ )
{
    if (bClear)
    {
        m_pStcHP[1]->SetText(_T(""));
        m_pStcMP[1]->SetText(_T(""));
        m_pStcVitality[1]->SetText(_T(""));

        m_pStcSavvy[1]->SetText(_T(""));
        m_pStcFortune[1]->SetText(_T(""));
        m_pStcRein[1]->SetText(_T(""));

        m_pStcPhysique[1]->SetText(_T(""));
        m_pStcStrength[1]->SetText(_T(""));
        m_pStcTechnique[1]->SetText(_T(""));
        m_pStcPneuma[1]->SetText(_T(""));
        m_pStcInnerForce[1]->SetText(_T(""));
        m_pStcAgility[1]->SetText(_T(""));

        m_pStcExAttack[1]->SetText(_T(""));
        m_pStcExDefense[1]->SetText(_T(""));
        m_pStcAttackTec[1]->SetText(_T(""));
        m_pStcArmor[1]->SetText(_T(""));
        m_pStcDefenseTec[1]->SetText(_T(""));
        m_pStcInAttack[1]->SetText(_T(""));
        m_pStcInDefense[1]->SetText(_T(""));
        m_pStcWeaponDmgMax[1]->SetText(_T(""));
        m_pStcExDamage[1]->SetText(_T(""));
        m_pStcDerateALL[1]->SetText(_T(""));
        m_pStcExDamageAbsorb[1]->SetText(_T(""));
        m_pStcHitRate[1]->SetText(_T(""));
        m_pStcWeaponSoul[1]->SetText(_T(""));
        m_pStcDodge[1]->SetText(_T(""));
        m_pStcCritRate[1]->SetText(_T(""));
        m_pStcRegainAddtion[1]->SetText(_T(""));
        m_pStcCritAmount[1]->SetText(_T(""));
        m_pStcBlockRate[1]->SetText(_T(""));

        GUIWnd* pWnd = m_pStcHP[1]->GetFather();
        if (P_VALID(pWnd))
        {
            pWnd->SetRefresh(TRUE);
        }

        return;
    }

    ESpinStep eSetp = (ESpinStep)((int)GetCurrFashionSpinSetp() + 1);
    if (eSetp == ESpinStep_End)
        eSetp = ESpinStep_Senior;

    ESuitAttType eType = GetSelectedFashionSuitType();
    SetFashionMsg(false, eSetp, eType);
}

VOID FashionComposeFrame::ClearData()
{
    m_pCurrFashionID        = GT_INVALID;
    m_pCurrMatearial        = GT_INVALID;
    m_pCurrMatearialType    = GT_INVALID;
    m_pMatearialCount   = 0;

    m_pIbtFashion->RefreshItem();
    m_pIbtMateiral->RefreshItem();

    m_pProgress->SetValue(0, true);
}

VOID FashionComposeFrame::SetFashionMsg( bool bLeft, ESpinStep eStep, ESuitAttType eSuitType )
{
    RefreshSuiteAttrEnd(0, true);

    if (bLeft)
    {
        Item* item = ItemMgr::Inst()->GetPocketItemByID(m_pCurrFashionID);

        if (P_VALID(item))
        {
            if (MIsEquipment(item->GetItemTypeID()) && item->GetProto()->eTypeReserved == 21)
            {
                Equipment* pEquip = (Equipment*)item;
                TMap<ERoleAttribute, INT> mapEquipAttr;
                pEquip->GetEquipAttribute(mapEquipAttr);

                // 基础属性
                m_pStcHP[0]->SetText(TtoString(mapEquipAttr.Peek(ERA_MaxHP)).c_str());
                m_pStcMP[0]->SetText(TtoString(mapEquipAttr.Peek(ERA_MaxMP)).c_str());
                m_pStcVitality[0]->SetText(TtoString(mapEquipAttr.Peek(ERA_MaxVitality)).c_str());

                const tagEquipProto* pEquipProto = ItemProtoData::Inst()->FindEquipProto(pEquip->GetItemTypeID());
                if (P_VALID(pEquipProto))
                {
                    const tagSuitProtoClient* pSuitProto = ItemProtoData::Inst()->FindSuitProto(pEquipProto->dwSuitID[1]);

                    if (P_VALID(pSuitProto))
                    {
                        // 三件套
                        const tagBuffProtoClient* pBuffProto = SkillProtoData::Inst()->FindBuffProto(pSuitProto->dwBuffID[0]);
                        if (P_VALID(pBuffProto))
                        {
                            m_pStcSavvy[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Savvy)).c_str());
                            m_pStcFortune[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Fortune)).c_str());
                            m_pStcRein[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Rein)).c_str());
                        }

                        // 五件套
                        pBuffProto = SkillProtoData::Inst()->FindBuffProto(pSuitProto->dwBuffID[1]);
                        if (P_VALID(pBuffProto))
                        {
                            m_pStcPhysique[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Physique)).c_str());
                            m_pStcStrength[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Strength)).c_str());
                            m_pStcTechnique[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Technique)).c_str());
                            m_pStcPneuma[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Pneuma)).c_str());
                            m_pStcInnerForce[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_InnerForce)).c_str());
                            m_pStcAgility[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Agility)).c_str());
                        }

                        // 六件套
                        pBuffProto = SkillProtoData::Inst()->FindBuffProto(pSuitProto->dwBuffID[2]);
                        if (P_VALID(pBuffProto))
                        {
                            m_pStcExAttack[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExAttack)).c_str());
                            m_pStcExDefense[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExDefense)).c_str());
                            m_pStcAttackTec[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_AttackTec)).c_str());
                            m_pStcArmor[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Armor)).c_str());
                            m_pStcDefenseTec[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_DefenseTec)).c_str());
                            m_pStcInAttack[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_InAttack)).c_str());
                            m_pStcInDefense[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_InDefense)).c_str());
                            m_pStcWeaponDmgMax[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_WeaponDmgMax)).c_str());
                            m_pStcExDamage[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExDamage)).c_str());
                            m_pStcDerateALL[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Derate_ALL)).c_str());
                            m_pStcExDamageAbsorb[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExDamage_Absorb)).c_str());
                            m_pStcHitRate[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_HitRate)).c_str());
                            m_pStcWeaponSoul[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_WeaponSoul)).c_str());
                            m_pStcDodge[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Dodge)).c_str());
                            m_pStcCritRate[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Crit_Rate)).c_str());
                            m_pStcRegainAddtion[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Regain_Addtion)).c_str());
                            m_pStcCritAmount[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Crit_Amount) / 100).c_str());
                            m_pStcBlockRate[0]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Block_Rate) / 100).c_str());
                        }
                    }
                }
            }
        }
    }
    else
    {
        Item* item = ItemMgr::Inst()->GetPocketItemByID(m_pCurrFashionID);

        if (P_VALID(item))
        {
            DWORD equipID = ItemProtoData::Inst()->FindNextFashionSpinID(item->GetItemTypeID(), GetSelectedFashionSuitType());
            const tagEquipProto* pEquipProto = ItemProtoData::Inst()->FindEquipProto(equipID);
            if (P_VALID(pEquipProto))
            {
                for (int i = 0; i < MAX_ROLEATT_BASE_EFFECT; ++i)
                {
                    if (pEquipProto->BaseEffect[i].eRoleAtt == ERA_MaxHP)
                        m_pStcHP[1]->SetText(TtoString(pEquipProto->BaseEffect[i].nValue).c_str());
                    else if (pEquipProto->BaseEffect[i].eRoleAtt == ERA_MaxMP)
                        m_pStcMP[1]->SetText(TtoString(pEquipProto->BaseEffect[i].nValue).c_str());
                    else if (pEquipProto->BaseEffect[i].eRoleAtt == ERA_MaxVitality)
                        m_pStcVitality[1]->SetText(TtoString(pEquipProto->BaseEffect[i].nValue).c_str());
                }

                const tagSuitProtoClient* pSuitProto = ItemProtoData::Inst()->FindSuitProto(pEquipProto->dwSuitID[1]);

                if (P_VALID(pSuitProto))
                {
                    // 三件套
                    const tagBuffProtoClient* pBuffProto = SkillProtoData::Inst()->FindBuffProto(pSuitProto->dwBuffID[0]);
                    if (P_VALID(pBuffProto))
                    {
                        m_pStcSavvy[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Savvy)).c_str());
                        m_pStcFortune[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Fortune)).c_str());
                        m_pStcRein[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Rein)).c_str());
                    }

                    // 五件套
                    pBuffProto = SkillProtoData::Inst()->FindBuffProto(pSuitProto->dwBuffID[1]);
                    if (P_VALID(pBuffProto))
                    {
                        m_pStcPhysique[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Physique)).c_str());
                        m_pStcStrength[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Strength)).c_str());
                        m_pStcTechnique[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Technique)).c_str());
                        m_pStcPneuma[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Pneuma)).c_str());
                        m_pStcInnerForce[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_InnerForce)).c_str());
                        m_pStcAgility[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Agility)).c_str());
                    }

                    // 六件套
                    pBuffProto = SkillProtoData::Inst()->FindBuffProto(pSuitProto->dwBuffID[2]);
                    if (P_VALID(pBuffProto))
                    {
                        m_pStcExAttack[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExAttack)).c_str());
                        m_pStcExDefense[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExDefense)).c_str());
                        m_pStcAttackTec[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_AttackTec)).c_str());
                        m_pStcArmor[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Armor)).c_str());
                        m_pStcDefenseTec[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_DefenseTec)).c_str());
                        m_pStcInAttack[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_InAttack)).c_str());
                        m_pStcInDefense[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_InDefense)).c_str());
                        m_pStcWeaponDmgMax[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_WeaponDmgMax)).c_str());
                        m_pStcExDamage[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExDamage)).c_str());
                        m_pStcDerateALL[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Derate_ALL)).c_str());
                        m_pStcExDamageAbsorb[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_ExDamage_Absorb)).c_str());
                        m_pStcHitRate[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_HitRate)).c_str());
                        m_pStcWeaponSoul[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_WeaponSoul)).c_str());
                        m_pStcDodge[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Dodge)).c_str());
                        m_pStcCritRate[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Crit_Rate)).c_str());
                        m_pStcRegainAddtion[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Regain_Addtion)).c_str());
                        m_pStcCritAmount[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Crit_Amount) / 100).c_str());
                        m_pStcBlockRate[1]->SetText(TtoString(pBuffProto->mapRoleAttrEffect.Peek(ERA_Block_Rate) / 100).c_str());
                    }
                }
            }
        }
    }
}

ESpinStep FashionComposeFrame::GetCurrFashionSpinSetp()
{
    Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pCurrFashionID);
    if (P_VALID(pItem))
    {
        return ItemProtoData::Inst()->FindFashionSpinStep(pItem->GetItemTypeID());
    }

    return ESpinStep_Null;
}

ESuitAttType FashionComposeFrame::GetSelectedFashionSuitType()
{
    ESuitAttType eType = ESSAT_Null;

    for (int i = 0; i != 4; ++i)
    {
        if (m_pPbtSuit[i]->GetState() == EGUIBS_PushDown)
        {
            eType = (ESuitAttType)(i + 1);

            return eType;
        }
    }

    return eType;
}

ESuitAttType FashionComposeFrame::GetCurrFashionSuitType()
{
    Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pCurrFashionID);
    if (P_VALID(pItem))
    {
        return ItemProtoData::Inst()->FindFashionSuitType(pItem->GetItemTypeID());
    }

    return ESSAT_Null;
}

DWORD FashionComposeFrame::OnNS_ConsolidateSpin( tagNS_ConsolidateSpin* pMsg, DWORD )
{
    m_pBtStart->SetEnable(true);
    m_pBtClose->SetEnable(true);
    m_pBtCancel->SetEnable(true);
    for (int i = 0; i != 4; ++i)
    {
        m_pPbtSuit[i]->SetEnable(true);
    }
    m_pPbtSuit[0]->SetState(EGUIBS_PushDown);

    if (pMsg->dwErrorCode == E_Compose_Consolidate_Success)
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionCompose9")]);
    }
    else
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionCompose10")]);
    }

    ClearUI();

    return 0;
}

int FashionComposeFrame::GetSelectedFashionSpinRate()
{
    return ItemProtoData::Inst()->FindFashionSpinRate(m_pMatearialCount, (ESpinStep)((int)GetCurrFashionSpinSetp() + 1));
}

VOID FashionComposeFrame::SetItem( Item* pItem )
{
    if (P_VALID(pItem))
    {
        if (MIsEquipment(pItem->GetItemTypeID()) && pItem->GetProto()->eTypeReserved == 21)
        {
            m_pCurrFashionID = pItem->GetItemId();
            m_pIbtFashion->RefreshItem(pItem->GetItemTypeID(), 1, 0);

            RefreshSuiteState();
            RefreshSuiteAttrBegin((int)GetCurrFashionSuitType() - 1);
            RefreshSuiteAttrEnd((int)GetSelectedFashionSuitType() - 1);

            SetSuccessRate(GetSelectedFashionSpinRate());

            return;
        }
        else if (pItem->GetItemSpecFunc() == EISF_SpinFashionDressItem)
        {
            m_pCurrMatearial = pItem->GetItemId();
            m_pCurrMatearialType = pItem->GetItemTypeID();
            int nCount = pItem->GetItemQuantity();
            ESpinStep eStep = GetCurrFashionSpinSetp();
            if (ESpinStep::ESpinStep_Null == eStep)
            {
                m_pMatearialCount = 10;
            }
            else if (ESpinStep_Basic == eStep)
            {
                m_pMatearialCount = 30;
            }
            else if (ESpinStep_Middle == eStep)
            {
                m_pMatearialCount = 60;
            }
            else
            {
                m_pMatearialCount = 10;
            }

            if (m_pMatearialCount > nCount)
            {
                m_pMatearialCount = nCount;
            }

            m_pIbtMateiral->RefreshItem(pItem->GetItemTypeID(), m_pMatearialCount, 0);

            SetSuccessRate(GetSelectedFashionSpinRate());

            return;
        }
    }

    CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionCompose11")]);
}