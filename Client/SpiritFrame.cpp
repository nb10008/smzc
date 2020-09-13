#include "stdafx.h"
#include "SpiritFrame.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CombatSysUtil.h"
#include "ItemButton.h"
#include "MallMgr.h"
#include "ItemProtoData.h"
#include "ToolTipCreator.h"
#include "IconStatic.h"
#include "ToolTipFrame.h"
#include "MallFrame.h"
#include "ActiveStatic.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_spirit.h"
#include "..\WorldDefine\mall_define.h"

const DWORD itemid1 = 1352002;//神龙鼎
const DWORD itemid0 = 1352003;//龙珠

const DWORD ExchangeItem[] = {1000802, 1000803};//可兑换的精力道具
static const TCHAR* backgroundpics[]=
{
    _T("data\\ui\\Main\\l_hongguang.dds"),
    _T("data\\ui\\Main\\l_chengguang.dds"),
    _T("data\\ui\\Main\\l_lvguang.dds"),
    _T("data\\ui\\Main\\l_languang.dds"),
    _T("data\\ui\\Main\\l_baiguang.dds")
};
SpiritFrame::SpiritFrame():
m_Trunk(this),
m_pSpiritControl(NULL),
m_pSpiritExchange(NULL),
m_pSpiritReward(NULL),
m_pSpiritShopping(NULL),
m_pBtnSpirit(NULL),
m_curbuyitem(GT_INVALID),
m_curselectedexchgitem(-1)
{
    m_SpiritRewardVal = _ttoi(g_StrTable[_T("SpiritReward")]);
    INT spiritelevels = _ttoi(g_StrTable[_T("SpiritLevelnum")]);
    m_pSpriteLevels.resize(spiritelevels);
    for (INT i = 0; i < spiritelevels; ++i)
    {
        TCHAR buff[5];
        _itot_s(i, buff, sizeof(buff)/sizeof(buff[0]), 10);
        tstring spiritelevel(_T("SpiritLevel"));
        spiritelevel += buff;
        m_pSpriteLevels[i] = _ttoi(g_StrTable[spiritelevel.c_str()]);
    }
}

SpiritFrame::~SpiritFrame()
{
}
BOOL SpiritFrame::Enabled()
{
	return TObjRef<NetSession>()->IsSpiritOn();
}
BOOL SpiritFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
    return bRet;
}
VOID SpiritFrame::Update()
{
    if(!m_pSpiritReward->IsInvisible())
    {
        if(m_RewardCountDown<clock())
        {
            //tagNC_SpiritReward msg;
            //m_pSession->Send(&msg);
            m_pSpiritReward->SetInvisible(TRUE);
        }
        else
        {
            TCHAR buff[10] = _T("");
            GUIStatic* text = (GUIStatic*)m_pSpiritReward->GetChild(_T("sprite_1_pic\\word1\\num"));
            _sntprintf(buff, sizeof(buff)/sizeof(buff[0]), _T("%d"), (m_RewardCountDown-clock())/CLOCKS_PER_SEC);
            text->SetText(buff);
            text->SetRefresh(TRUE);
        }
    }
    else if(m_SpiritRewardVal == m_spirit)
    {
        if(clock()%(CLOCKS_PER_SEC/2)<(CLOCKS_PER_SEC/4))
        {
            m_pASpiritBackground->SetPic(backgroundpics[sizeof(backgroundpics)/sizeof(backgroundpics[0])-1]);
            m_pBtnSpirit->SetRefresh(TRUE);
        }
        else
        {
            m_pASpiritBackground->SetPic(backgroundpics[sizeof(backgroundpics)/sizeof(backgroundpics[0])-2]);
            m_pBtnSpirit->SetRefresh(TRUE);
        }
    }
}
BOOL SpiritFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();
    m_pCmdMgr->UnRegister("NS_GetRoleInitState_Att", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetInitLocalPlayerAtt));
    m_pCmdMgr->UnRegister("NS_RoleAttChangeSingle", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetRoleAttChangeSingle));
    m_pCmdMgr->UnRegister("NS_SpiritReward", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetSpiritReward));
    m_pCmdMgr->UnRegister("NS_StrengthReward", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetStrengthReward));
    m_pCmdMgr->UnRegister("NS_GetStrengthDetail", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetGetStrengthDetail));
    m_pGUI->UnRegisterEventHandler(m_pSpiritControl->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::GUIEventHandler));
    m_pGUI->UnRegisterEventHandler(m_pSpiritDescription->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::DescriptionEventHandler));
    m_pGUI->UnRegisterEventHandler(m_pSpiritExchange->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::ExchangeEventHandler));
    m_pGUI->UnRegisterEventHandler(m_pSpiritShopping->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::ShoppingEventHandler));
    m_pGUI->UnRegisterEventHandler(m_pSpiritReward->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::RewardEventHandler));
    m_pMgr->UnRegisterEventHandler(_T("SpiritExchange_Event"), (FRAMEEVENTPROC)m_Trunk.sfp1(&SpiritFrame::OnExchangeInputEventHandler));
    if(P_VALID(m_pSpiritControl))
    {
        m_pGUI->AddToDestroyList(m_pSpiritControl);
        m_pSpiritControl = NULL;
    }
    if(P_VALID(m_pSpiritExchange))
    {
        m_pGUI->AddToDestroyList(m_pSpiritExchange);
        m_pSpiritExchange = NULL;
    }
    if(P_VALID(m_pSpiritReward))
    {
        m_pGUI->AddToDestroyList(m_pSpiritReward);
        m_pSpiritReward = NULL;
    }
    if(P_VALID(m_pSpiritShopping))
    {
        m_pGUI->AddToDestroyList(m_pSpiritShopping);
        m_pSpiritShopping = NULL;
    }
    if(P_VALID(m_pSpiritDescription))
    {
        m_pGUI->AddToDestroyList(m_pSpiritDescription);
        m_pSpiritDescription = NULL;
    }
    return bRet;
}
const TCHAR* SpiritFrame::ConvertErrorCode(DWORD errorcode)
{
    tstring errorname(_T("SpriteErrorCode"));
    TCHAR buff[5];
    _itot_s(errorcode, buff, sizeof(buff)/sizeof(buff[0]), 10);
    errorname += buff;
    return g_StrTable[errorname.c_str()];
}
DWORD SpiritFrame::NetStrengthReward(tagNS_StrengthReward* pNetCmd, DWORD)
{
    if (pNetCmd->dwError)
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(ConvertErrorCode(pNetCmd->dwError));
    }
    else
    {
        tagNC_GetStrengthDetail cmd;
        TObjRef<NetSession>()->Send(&cmd);
    }
    return 0;
}
DWORD SpiritFrame::NetSpiritReward(tagNS_SpiritReward* pNetCmd, DWORD)
{
    if (pNetCmd->dwError)
    {
        CombatSysUtil::Inst()->ShowScreenCenterMsgEx(ConvertErrorCode(pNetCmd->dwError));
    }
    return 0;
}
DWORD SpiritFrame::NetInitLocalPlayerAtt(tagNS_GetRoleInitState_Att *pNetCmd, DWORD)
{
    UpdateSpirit(pNetCmd->nAtt[ERA_Spirit]);
    return 0;
}
DWORD SpiritFrame::NetRoleAttChangeSingle( tagNS_RoleAttChangeSingle* pNetCmd, DWORD )
{
    if(pNetCmd->eType==ERA_Spirit)
        UpdateSpirit(pNetCmd->nValue);
    return 0;
}
DWORD SpiritFrame::NetGetStrengthDetail(tagNS_GetStrengthDetail* pNetCmd, DWORD)
{
    GUIStatic* num = (GUIStatic*)m_pSpiritExchange->GetChild(_T("sprite_pic\\sprite_back1\\num"));
    TCHAR buffer[32];
    m_constrength = pNetCmd->nConsumptiveStrength;
    _itot(m_constrength, buffer, 10);
    num->SetText(buffer);
    return 0;
}
BOOL SpiritFrame::ReloadUI()
{
    tstring spiritcontrolpath = g_strLocalPath + _T("\\ui\\sprite_state.xml");
    tstring spiritexchangepath = g_strLocalPath + _T("\\ui\\sprite_exchange.xml");
    tstring spiritshoppingpath = g_strLocalPath + _T("\\ui\\sprite_shopping.xml");
    tstring spiritrewardpath = g_strLocalPath + _T("\\ui\\sprite_reward.xml");
    tstring spritedescpath = g_strLocalPath + _T("\\ui\\msgbox3.xml");

    XmlElement element;
    m_pGUI->LoadXml(&element, "VFS_System", spiritexchangepath.c_str());
    m_pGUI->ChangeXml(&element, "sprite_win\\sprite_pic\\sprite_back\\itempic1\\img", "ClassName", "MallButton");
    m_pGUI->ChangeXml(&element, "sprite_win\\sprite_pic\\sprite_back\\itempic0\\img", "ClassName", "MallButton");
    m_pGUI->ChangeXml(&element, "sprite_win\\sprite_pic\\sprite_back\\itempic1", "ClassName", "IconStatic");
    m_pGUI->ChangeXml(&element, "sprite_win\\sprite_pic\\sprite_back\\itempic0", "ClassName", "IconStatic");

    m_pSpiritExchange = m_pGUI->CreateWnd(_T("\\desktop"), &element);
    ((GUIStatic*)m_pSpiritExchange->GetChild(_T("sprite_pic\\sprite_back\\num0")))->SetText(g_StrTable[_T("SpiritItemExchange1")]);
    ((GUIStatic*)m_pSpiritExchange->GetChild(_T("sprite_pic\\sprite_back\\num1")))->SetText(g_StrTable[_T("SpiritItemExchange2")]);
    m_exchangedprice[0] = _ttoi(g_StrTable[_T("SpiritItemExchange1")]);
    m_exchangedprice[1] = _ttoi(g_StrTable[_T("SpiritItemExchange2")]);
    m_pGUI->LoadXml(&element, "VFS_System", spritedescpath.c_str());
    m_pSpiritDescription = m_pGUI->CreateWnd(_T("\\desktop"), &element);

    m_pGUI->LoadXml(&element, "VFS_System", spiritcontrolpath.c_str());
    m_pGUI->ChangeXml(&element, "sprite_state_win\\img","ClassName", "ActiveStatic");
    m_pSpiritControl = m_pGUI->CreateWnd(_T("\\desktop"),&element);
    m_pASpiritBackground = (ActiveStatic*)m_pSpiritControl->GetChild(_T("img"));

    m_pGUI->LoadXml(&element, "VFS_System", spiritshoppingpath.c_str());
    for(char i = 0; i<6; ++i)
    {
        string nodename("sprite_2_win\\sprite_2_pic\\back0\\itempic");
        nodename+=('0'+i);
        m_pGUI->ChangeXml(&element, nodename.c_str(), "ClassName", "IconStatic");
        nodename+="\\img";
        m_pGUI->ChangeXml(&element, nodename.c_str(),"ClassName", "MallButton");
    }
    m_pSpiritShopping = m_pGUI->CreateWnd(_T("\\desktop"), &element);

    m_pGUI->LoadXml(&element, "VFS_System", spiritrewardpath.c_str());
    m_pGUI->ChangeXml(&element, "sprite_1_win\\sprite_1_pic\\itempic1\\item", "ClassName", "IconStatic");
    m_pGUI->ChangeXml(&element, "sprite_1_win\\sprite_1_pic\\itempic0\\item", "ClassName", "IconStatic");
    m_pSpiritReward = m_pGUI->CreateWnd(_T("\\desktop"), &element);

    GUIStaticEx* descriptiontext = (GUIStaticEx*)m_pSpiritDescription->GetChild(_T("back\\textback\\win\\text"));
    tstring desc;
    desc+= g_StrTable[_T("SpiritDescription")];
    desc+= g_StrTable[_T("SpiritDescription1")];
    desc+= g_StrTable[_T("SpiritDescription2")];
    desc+= g_StrTable[_T("SpiritDescription3")];
    descriptiontext->SetText(desc.c_str());
    tagPoint size = descriptiontext->GetSize();
    descriptiontext->SetResizable(TRUE);
    size.y = descriptiontext->GetTextTotalHeight();
    descriptiontext->Resize(size);
    descriptiontext->SetResizable(FALSE);

    GUIPatch* WndBack = (GUIPatch*)m_pSpiritDescription->GetChild(_T("back"));
    WndBack->SetText(g_StrTable[_T("SpiritDescriptionTitle")]);
    m_pBtnSpirit = (GUIButton*)m_pSpiritControl->GetChild(_T("butt"));
    m_pBtnSpiritDescOK = (GUIButton*)m_pSpiritDescription->GetChild(_T("back\\ok"));
    m_pBtnSpiritDescCancel = (GUIButton*)m_pSpiritDescription->GetChild(_T("back\\cancel"));


    m_pBtnSpiritExchangeBuy = (GUIButton*)m_pSpiritExchange->GetChild(_T("sprite_pic\\buybutt"));
    m_pBtnSpiritExchangeCancel = (GUIButton*)m_pSpiritExchange->GetChild(_T("sprite_pic\\canclebutt"));
    m_pBtnSpiritExchangeClose = (GUIButton*)m_pSpiritExchange->GetChild(_T("sprite_pic\\closebutt"));

    m_pBtnSpiritShoppingBuy = (GUIButton*)m_pSpiritShopping->GetChild(_T("sprite_2_pic\\buybutt"));
    m_pBtnSpiritShoppingCancel = (GUIButton*)m_pSpiritShopping->GetChild(_T("sprite_2_pic\\canclebutt"));
    m_pBtnSpiritShoppingClose = (GUIButton*)m_pSpiritShopping->GetChild(_T("sprite_2_pic\\closebutt"));

    m_pBtnSpiritRewardSure = (GUIButton*)m_pSpiritReward->GetChild(_T("sprite_1_pic\\surebutt"));
    m_pBtnSpiritRewardClose = (GUIButton*)m_pSpiritReward->GetChild(_T("sprite_1_pic\\closebutt"));
    m_pBtnSpiritRewardCancel = (GUIButton*)m_pSpiritReward->GetChild(_T("sprite_1_pic\\canclebutt"));


    m_pGUI->RegisterEventHandler(m_pSpiritControl->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::GUIEventHandler));
    m_pGUI->RegisterEventHandler(m_pSpiritDescription->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::DescriptionEventHandler));
    m_pGUI->RegisterEventHandler(m_pSpiritExchange->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::ExchangeEventHandler));
    m_pGUI->RegisterEventHandler(m_pSpiritShopping->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::ShoppingEventHandler));
    m_pGUI->RegisterEventHandler(m_pSpiritReward->GetFullName().c_str(), m_Trunk.sfp1(&SpiritFrame::RewardEventHandler));
    m_pMgr->RegisterEventHandle(_T("SpiritExchange_Event"), (FRAMEEVENTPROC)m_Trunk.sfp1(&SpiritFrame::OnExchangeInputEventHandler));
    m_pCmdMgr->Register("NS_RoleAttChangeSingle", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetRoleAttChangeSingle), _T("NS_RoleAttChangeSingle"));
    m_pCmdMgr->Register("NS_GetRoleInitState_Att", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetInitLocalPlayerAtt), _T("NS_GetRoleInitState_Att"));
    m_pCmdMgr->Register("NS_SpiritReward", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetSpiritReward), _T("NS_SpiritReward"));
    m_pCmdMgr->Register("NS_StrengthReward", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetStrengthReward), _T("NS_StrengthReward"));
    m_pCmdMgr->Register("NS_GetStrengthDetail", (NETMSGPROC)m_Trunk.sfp2(&SpiritFrame::NetGetStrengthDetail), _T("NS_GetStrengthDetail"));
    m_pSpiritControl->SetInvisible(FALSE);
    m_pSpiritExchange->SetInvisible(TRUE);
    m_pSpiritReward->SetInvisible(TRUE);
    m_pSpiritShopping->SetInvisible(TRUE);
    m_pSpiritDescription->SetInvisible(TRUE);
    return TRUE;
}

BOOL SpiritFrame::EscCancel()
{
    BOOL bRet = FALSE;
    if(m_pSpiritExchange&&!m_pSpiritExchange->IsInvisible())
    {
        bRet = TRUE;
        m_pSpiritExchange->SetInvisible(TRUE);
    }
    if(m_pSpiritReward&&!m_pSpiritReward->IsInvisible())
    {
        bRet = TRUE;
        m_pSpiritReward->SetInvisible(TRUE);
    }
    if(m_pSpiritShopping&&!m_pSpiritShopping->IsInvisible())
    {
        bRet = TRUE;
        m_pSpiritShopping->SetInvisible(TRUE);
    }
    if(m_pSpiritDescription&&!m_pSpiritDescription->IsInvisible())
    {
        bRet = TRUE;
        m_pSpiritDescription->SetInvisible(TRUE);
    }
    return bRet;
}
BOOL SpiritFrame::GUIEventHandler(tagGUIEvent* pEvent)
{
    const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
    if(!P_VALID(pWnd) || m_pBtnSpirit != pWnd)
        return FALSE;
    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        if(m_spirit == m_SpiritRewardVal&&m_pSpiritReward->IsInvisible())
        {
            const tagItemProto* pItemProto1 = ItemProtoData::Inst()->FindItemProto(itemid1);
            const tagItemProto* pItemProto0 = ItemProtoData::Inst()->FindItemProto(itemid0);

            if( P_VALID(pItemProto1) && P_VALID(pItemProto1->pDisplayInfo ) )
            {
                TCHAR szPath[X_LONG_NAME] = _T("");
                _sntprintf(szPath, X_LONG_NAME, _T("data\\ui\\Icon\\%s.tga"), pItemProto1->pDisplayInfo->szMaleIcon );
                IconStatic* icon = (IconStatic*)m_pSpiritReward->GetChild(_T("sprite_1_pic\\itempic1\\item"));
                icon->SetPic( szPath );
            }

            if( P_VALID(pItemProto0) && P_VALID(pItemProto0->pDisplayInfo ) )
            {
                TCHAR szPath[X_LONG_NAME] = _T("");
                _sntprintf(szPath, X_LONG_NAME, _T("data\\ui\\Icon\\%s.tga"), pItemProto0->pDisplayInfo->szMaleIcon );
                IconStatic* icon = (IconStatic*)m_pSpiritReward->GetChild(_T("sprite_1_pic\\itempic0\\item"));
                icon->SetPic( szPath );
            }
            m_RewardCountDown = clock()+16*CLOCKS_PER_SEC;
            m_pSpiritReward->SetInvisible(FALSE);
            m_pSpiritReward->FlipToTop();
            UpdateTips();
        }
        else
        {
            m_pSpiritDescription->SetInvisible(FALSE);
            m_pSpiritDescription->FlipToTop();
        }
        break;
    default:
        break;
    }
    return TRUE;
}
BOOL SpiritFrame::DescriptionEventHandler(tagGUIEvent* pEvent)
{
    const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        if (pWnd == m_pBtnSpiritDescOK || pWnd == m_pBtnSpiritDescCancel)
        {
            m_pSpiritDescription->SetInvisible(TRUE);
        }
        break;
    case EGUIE_StaticExLinkClick:
        switch (pEvent->dwParam1)
        {
        case 1:
            {
                for(TCHAR i = 0; i<6; ++i)
                {
                    tstring nodename(_T("sprite_2_pic\\back0\\itempic"));
                    nodename += (_T('0')+i);
                    nodename += _T("\\img");

                    tstring pricename(_T("sprite_2_pic\\back0\\num"));
                    pricename += (_T('0')+i);
                    mb[i] = (MallButton*)m_pSpiritShopping->GetChild(nodename.c_str());
                    GUIStatic* price = (GUIStatic*)m_pSpiritShopping->GetChild(pricename.c_str());
                    DWORD itemid = 1000801+i;
                    tagMallItemProto* pProItem = MallMgr::Inst()->GetMallItemByTypeid(itemid);
                    if( P_VALID(pProItem))
                    {
                        const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProItem->dwTypeID);
                        mb[i]->SetTips(ToolTipCreator::Inst()->GetItemTips(itemid).c_str());
                        mb[i]->RefreshItem(pProItem->dwTypeID, 1, pItem->byQuality);
                        mb[i]->SetBtnData(ESBT_Item, pProItem->dwID, pProItem->dwTypeID, pProItem->nIndexInServer);
                        TCHAR szBuff[X_LONG_NAME];
                        if( pProItem->nSalePrice!=GT_INVALID && pProItem->nSalePrice!=pProItem->nPrice )
                            _sntprintf(szBuff, sizeof(szBuff)/sizeof(szBuff[0]), _T("%d"), pProItem->nSalePrice);
                        else 
                            _sntprintf(szBuff, sizeof(szBuff)/sizeof(szBuff[0]), _T("%d"), pProItem->nPrice);
                        price->SetText(szBuff);
                    }
                }
            m_pSpiritShopping->SetInvisible(FALSE);
            ClearShoppingSelection();
            MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(GT_INVALID);
            m_pSpiritShopping->FlipToTop();
            }
            break;
        case 2:
            {
                 mbExchange[0] = (MallButton*)m_pSpiritExchange->GetChild(_T("sprite_pic\\sprite_back\\itempic1\\img"));
                 mbExchange[1] = (MallButton*)m_pSpiritExchange->GetChild(_T("sprite_pic\\sprite_back\\itempic0\\img"));
                 for(INT i = 0; i<2; ++i)
                 {
                     tagMallItemProto* pProItem = MallMgr::Inst()->GetMallItemByTypeid(1000802+i);
                     if( P_VALID(pProItem))
                     {
                         const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProItem->dwTypeID);
                         mbExchange[i]->SetTips(ToolTipCreator::Inst()->GetItemTips(1000802+i).c_str());
                         mbExchange[i]->RefreshItem(pProItem->dwTypeID, 1, pItem->byQuality);
                         mbExchange[i]->SetBtnData(ESBT_Item, pProItem->dwID, pProItem->dwTypeID, pProItem->nIndexInServer);
                     }
                 }
                m_pSpiritExchange->SetInvisible(FALSE);
                m_pSpiritExchange->FlipToTop();
                tagNC_GetStrengthDetail cmd;
                TObjRef<NetSession>()->Send(&cmd);
                break;
            }
        }
        m_pSpiritDescription->SetInvisible(TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}
BOOL SpiritFrame::ExchangeEventHandler(tagGUIEvent* pEvent)
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if(pWnd == m_pBtnSpiritExchangeClose || pWnd == m_pBtnSpiritExchangeCancel)
                m_pSpiritExchange->SetInvisible(TRUE);
            else if(pWnd == m_pBtnSpiritExchangeBuy)
            {
                if(m_curselectedexchgitem<0)
                {
                    if( P_VALID(GetObj("SpiritExchange_Failure")) )
                        KillObj("SpiritExchange_Failure");
                    CreateObj("SpiritExchange_Failure", "MsgBox");
                    TObjRef<MsgBox>("SpiritExchange_Failure")->Init(_T(""), g_StrTable[_T("SpiritNoItemSelected")], _T("SpiritExchange_Failure"), MBF_OK, TRUE);
                }
                else
                {
                    if( P_VALID(GetObj("SpiritExchange_MsgBox")) )
                        KillObj("SpiritExchange_MsgBox");
                    CreateObj("SpiritExchange_MsgBox", "MsgInputBox");

                    TObjRef<MsgInputBox>("SpiritExchange_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade4")], 
                        _T("SpiritExchange_Event"), 100, (MsgBoxInputType)(MBIT_Number), _T(""), TRUE);
                }
            }
        }
    case EGUIE_Drag:
        if(pWnd->GetName()==_T("img"))
        {
            const TCHAR *szDeSelectItem = _T("data\\ui\\common\\l_icon.bmp");
            const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
            ((IconStatic*)mbExchange[0]->GetFather())->SetPic(szDeSelectItem);
            ((IconStatic*)mbExchange[1]->GetFather())->SetPic(szDeSelectItem);
            ((IconStatic*)pWnd->GetFather())->SetPic(szSelectItem);
            if(pWnd==mbExchange[0])
                m_curselectedexchgitem = 0;
            else
                m_curselectedexchgitem = 1;
        }
    default:break;
    }
    return TRUE;
}
void SpiritFrame::ClearShoppingSelection()
{
    m_curbuyitem = GT_INVALID;
    const TCHAR *szSelectItem = _T("data\\ui\\common\\l_icon.bmp");
    for(int i = 0; i<sizeof(mb)/sizeof(mb[0]); ++i)
    {
        if(mb[i]!=NULL)
        {
            if((IconStatic*)mb[i]->GetFather()!=NULL)
                ((IconStatic*)mb[i]->GetFather())->SetPic(szSelectItem);
        }
    }
}
BOOL SpiritFrame::ShoppingEventHandler(tagGUIEvent* pEvent)
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if(pWnd == m_pBtnSpiritShoppingClose || pWnd == m_pBtnSpiritShoppingCancel)
                m_pSpiritShopping->SetInvisible(TRUE);
            else if(pWnd == m_pBtnSpiritShoppingBuy)
            {
                if(m_curbuyitem==GT_INVALID)
                {
                    if( P_VALID(GetObj("SpiritExchange_Failure")) )
                        KillObj("SpiritExchange_Failure");
                    CreateObj("SpiritExchange_Failure", "MsgBox");
                    TObjRef<MsgBox>("SpiritExchange_Failure")->Init(_T(""), g_StrTable[_T("SpiritNoItemSelected")], _T("SpiritExchange_Failure"), MBF_OK, TRUE);
                }
                else
                {
                    MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(m_curbuyitem);
                    MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                }
            }
            else if(pWnd->GetName()==_T("img"))
            {
                const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
                MallButton* button = (MallButton*) pWnd;
                if( button->GetBtnData().GetData1() != GT_INVALID )
                {
                    ClearShoppingSelection();
                    MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(button->GetBtnData().GetData1());
                    ((IconStatic*)button->GetFather())->SetPic(szSelectItem);
                    MallMgr::Inst()->GetMallFrame()->BuyMallItem();
                }
            }
        }
        break;
    case EGUIE_Drag:
        if(pWnd->GetName()==_T("img"))
        {
            const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
            MallButton* button = (MallButton*) pWnd;
            if( button->GetBtnData().GetData1() != GT_INVALID )
            {
                ClearShoppingSelection();
                m_curbuyitem = button->GetBtnData().GetData1();
                ((IconStatic*)button->GetFather())->SetPic(szSelectItem);
            }
        }
        break;
    default:break;
    }
    return TRUE;
}
BOOL SpiritFrame::RewardEventHandler(tagGUIEvent* pEvent)
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if(pWnd == m_pBtnSpiritRewardCancel || pWnd == m_pBtnSpiritRewardClose)
            {
                m_pSpiritReward->SetInvisible(TRUE);
                UpdateTips();
            }
            else if(pWnd == m_pBtnSpiritRewardSure)
            {
                tagNC_SpiritReward msg;
                m_pSession->Send(&msg);
                m_pSpiritReward->SetInvisible(TRUE);
            }
        }
    case EGUIE_MoveIntoCtrl:
        {
            if(pWnd == m_pSpiritReward->GetChild(_T("sprite_1_pic\\itempic1\\item")))
                ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(itemid1).c_str());
            else if(pWnd == m_pSpiritReward->GetChild(_T("sprite_1_pic\\itempic0\\item")))
                ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(itemid0).c_str());
        }
    default:break;
    }
    return TRUE;
}
DWORD SpiritFrame::OnExchangeInputEventHandler(tagMsgInputBoxEvent* pGameEvent)
{
    if( MBF_OK == pGameEvent->eResult )
    {
        if(m_curselectedexchgitem<0)
        {
            if( P_VALID(GetObj("SpiritExchange_Failure")) )
                KillObj("SpiritExchange_Failure");
            CreateObj("SpiritExchange_Failure", "MsgBox");
            TObjRef<MsgBox>("SpiritExchange_Failure")->Init(_T(""), g_StrTable[_T("SpiritNoItemSelected")], _T("SpiritExchange_Failure"), MBF_OK, TRUE);
        }
        else if(m_curselectedexchgitem>=0&&m_constrength/m_exchangedprice[m_curselectedexchgitem]<pGameEvent->nInputNum)
        {
            if( P_VALID(GetObj("SpiritExchange_Failure")) )
                KillObj("SpiritExchange_Failure");
            CreateObj("SpiritExchange_Failure", "MsgBox");
            TObjRef<MsgBox>("SpiritExchange_Failure")->Init(_T(""), g_StrTable[_T("SpiritNotEnough")], _T("SpiritExchange_Failure"), MBF_OK, TRUE);
        }
        else
        {
            tagNC_StrengthReward cmd;
            cmd.nNum = pGameEvent->nInputNum;
            cmd.dwTypeID = ExchangeItem[m_curselectedexchgitem];
            m_pSession->Send(&cmd);
            return 0;
        }
    }
    return 0;
}

VOID SpiritFrame::UpdateSpirit(INT spirit)
{
    m_spirit = spirit;
    if(RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel()>=40)
    {
        int i = 0;
        while (spirit>m_pSpriteLevels[i])
        {
            ++i;
        }
        m_pASpiritBackground->SetPic(backgroundpics[i]);
    }
    UpdateTips();
}
VOID SpiritFrame::UpdateTips()
{
    if(m_spirit == m_SpiritRewardVal&&m_pSpiritReward->IsInvisible())
    {
        m_pBtnSpirit->SetTips(g_StrTable[_T("SpiritRewardTips")]);
    }
    else
    {
        tstring tips = g_StrTable[_T("Spirit")];
        TCHAR buffer[15];
        _sntprintf(buffer, sizeof(buffer)/sizeof(buffer[0]), _T("%d/%d"), m_spirit, m_pSpriteLevels[m_pSpriteLevels.size()-1]);
        tips.reserve(tips.length() + 6);
        tips += buffer;
        m_pBtnSpirit->SetTips(tips.c_str());
    }
}