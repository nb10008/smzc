#include "StdAfx.h"
#include "LevelUpShowFrame.h"
#include "LevelUpData.h"
#include "..\WorldDefine\role_att.h"
#include "ChatFrame.h"
#include "GUIEffectMgr.h"

LevelUpShowFrame::LevelUpShowFrame(void) :
    m_Trunk(this),
    m_pWnd(0),
    m_dwStartShowTime(0),
    m_dwTextUpdate(0),
    m_nBeginRow(0)
{
}

LevelUpShowFrame::~LevelUpShowFrame(void)
{
}

BOOL LevelUpShowFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    // register game event handle

    return bRet;
}

BOOL LevelUpShowFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    // unregister game event handle

    if (P_VALID(m_pWnd))
    {
        m_pGUI->AddToDestroyList(m_pWnd);
        m_pWnd = 0;
    }

    return bRet;
}

BOOL LevelUpShowFrame::ReloadUI()
{
    // load ui resources
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\main_show.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    // create the window
    m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

    // register ui event handle
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&LevelUpShowFrame::EventHandler));

    // controls
    //TCHAR tbuffer[256] = {0};
    //for (int i = 0; i < TEXT_MAX_COLUMN; ++i)
    //{
    //    _sntprintf(tbuffer, sizeof(tbuffer) / sizeof(TCHAR), _T("main_levup_pic\\word%d"), i);
    //    m_pStcMsg[i]      = static_cast<GUIStatic*>(m_pWnd->GetChild(tbuffer));
    //}

    //m_pStcCongratulation = static_cast<GUIStatic*>(m_pWnd->GetChild(_T("main_levup_pic\\word12")));
    m_pBtExit       = static_cast<GUIButton*>(m_pWnd->GetChild(_T("main_levup_pic\\closebutt")));

    // hide the window
    ShowWnd(false);
	m_pWnd->SetUnClickable(TRUE);

    // make msg empty
    //for (int i = 0; i < TEXT_MAX_COLUMN; ++i)
    //{
    //    m_pStcMsg[i]->SetText(_T(""));
    //}

    return TRUE;
}

VOID LevelUpShowFrame::Update()
{
    if (!P_VALID(m_pWnd))
        return;

    if (m_pWnd->IsInvisible())
        return;

    //界面超过12秒自动关闭，每一秒滚动一行文本
    if (Kernel::Inst()->GetAccumTimeDW() - m_dwStartShowTime > 6000)
    {
        ShowWnd(FALSE);
    }

    if (Kernel::Inst()->GetAccumTimeDW() - m_dwTextUpdate > 500)
    {
        m_dwTextUpdate = Kernel::Inst()->GetAccumTimeDW();
        //for (int i = 0; i < TEXT_MAX_COLUMN; ++i)
        //{
        //    int currIndex = m_nBeginRow + i;
        //    if (currIndex < m_vecText.size())
        //    {
        //        m_pStcMsg[i]->SetText(m_vecText[currIndex].c_str());
        //    }
        //    else
        //    {
        //        m_pStcMsg[i]->SetText(_T(""));
        //    }

        //    m_pStcMsg[i]->SetRefresh(TRUE);
        //}
        if (m_nBeginRow < m_vecText.size())
        {
            //m_pStcMsg[m_nBeginRow]->SetText(m_vecText[m_nBeginRow].c_str());
            ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
            if (P_VALID(pFrame))
            {
                pFrame->PushInfo(m_vecText[m_nBeginRow].c_str(), ESCC_Combat);
            }
        }

        ++m_nBeginRow;
    }
}

BOOL LevelUpShowFrame::EscCancel()
{
    if (!P_VALID(m_pWnd))
        return FALSE;

    if(m_pWnd->IsInvisible())
        return FALSE;

    ShowWnd(false);
    return TRUE;
}

void LevelUpShowFrame::ShowWnd( bool bVisible )
{
    if (P_VALID(m_pWnd))
    {
        //m_pWnd->SetInvisible(!bVisible);
		m_pWnd->SetInvisible(TRUE);
    }

    if (!bVisible)
    {
        //m_pWnd->FlipToTop();
    }
    else
    {
		GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
		GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui003"),pDesktop->GetView(),pDesktop->GetSize());
    }
}

DWORD LevelUpShowFrame::EventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if (pWnd == m_pBtExit)
            {
                ShowWnd(false);
            }
        }
    }

    return 0;
}

void LevelUpShowFrame::OnLevelUp( tagNS_RoleLevelChange* pNetCmd )
{
    m_vecText.clear();
    tstringstream tss;
    int nValue;
    
    tss << g_StrTable[_T("PlayerLevelUp1")] << pNetCmd->nLevel << g_StrTable[_T("PlayerLevelUp2")];
    //m_pStcCongratulation->SetText(tss.str().c_str());
    m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_Strength, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp3")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_Physique, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp4")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_InnerForce, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp5")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_Pneuma, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp6")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_Technique, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp7")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_Agility, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp8")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_AttPoint, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp9")] << nValue << g_StrTable[_T("PlayerLevelUp10")];
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_TalentPoint, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp9")] << nValue << g_StrTable[_T("PlayerLevelUp11")];
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_HP, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp12")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_MP, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp13")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    tss.str(_T(""));
    tss.clear();
    nValue = LevelUpData::Inst()->GetRoleAttInc(ERA_Vitality, pNetCmd->nLevel);
    tss << g_StrTable[_T("PlayerLevelUp14")] << nValue;
    if (nValue > 0)
        m_vecText.push_back(tss.str());

    m_dwStartShowTime = Kernel::Inst()->GetAccumTimeDW();

    m_nBeginRow = 0;

    //for (int i = 0; i < TEXT_MAX_COLUMN; ++i)
    //{
    //    m_pStcMsg[i]->SetText(_T(""));
    //}

    ShowWnd(true);
}
