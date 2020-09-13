#include "StdAfx.h"
#include "ToHaveApprentice.h"
#include "MasterEvent.h"
#include "MasterMgr.h"
#include "RoleMgr.h"
#include "PlayerNameTab.h"
#include "MsgBox.h"
#include "CombatSysUtil.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "ChatFrame.h"

ToHaveApprentice::ToHaveApprentice(void) :
    m_Trunk(this),
    m_dwTargetID(GT_INVALID)
{
}

ToHaveApprentice::~ToHaveApprentice(void)
{
}

BOOL ToHaveApprentice::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    // register net cmd
    m_pCmdMgr->Register("NS_MasterAcceptApprentice", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_MasterAcceptApprentice), _T("NS_MasterAcceptApprentice"));
    m_pCmdMgr->Register("NS_ApprenticeNotice", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_ApprenticeNotice), _T("NS_ApprenticeNotice"));
    m_pCmdMgr->Register("NS_ApprenticeReply", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_ApprenticeReply), _T("NS_ApprenticeReply"));
    m_pCmdMgr->Register("NS_ApprenticeReplyMaster", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_ApprenticeReplyMaster), _T("NS_ApprenticeReplyMaster"));

    // register game event
    m_pFrameMgr->RegisterEventHandle(_T("tagAquireApprenticeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ToHaveApprentice::OnAquireApprenticeEvent));
    m_pFrameMgr->RegisterEventHandle(_T("ApprenticeNoticeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ToHaveApprentice::ApprenticeNoticeEvent));

    return bRet;
}

BOOL ToHaveApprentice::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    // unregister net cmd
    m_pCmdMgr->UnRegister("NS_MasterAcceptApprentice", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_MasterAcceptApprentice));
    m_pCmdMgr->UnRegister("NS_ApprenticeNotice", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_ApprenticeNotice));
    m_pCmdMgr->UnRegister("NS_ApprenticeReply", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_ApprenticeReply));
    m_pCmdMgr->UnRegister("NS_ApprenticeReplyMaster", (NETMSGPROC)m_Trunk.sfp2(&ToHaveApprentice::OnNS_ApprenticeReplyMaster));

    // unregister game event
    m_pFrameMgr->UnRegisterEventHandler(_T("tagAquireApprenticeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ToHaveApprentice::OnAquireApprenticeEvent));
    m_pFrameMgr->UnRegisterEventHandler(_T("ApprenticeNoticeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ToHaveApprentice::ApprenticeNoticeEvent));

    return bRet;
}

VOID ToHaveApprentice::Update()
{
    GameFrame::Update();
}

DWORD ToHaveApprentice::OnAquireApprenticeEvent( tagAquireApprenticeEvent* pGameEvent )
{
    // 发送收徒审请
    tagNC_MasterAcceptApprentice msg;
    msg.dwRoleID    = pGameEvent->roleID;

    if (m_pSession->IsConnect())
        m_pSession->Send(&msg);

    return 0;
}

DWORD ToHaveApprentice::OnNS_MasterAcceptApprentice( tagNS_MasterAcceptApprentice* pMsg, DWORD )
{
    // 显示错误码
    MasterMgr::inst()->ShowErrMsg(pMsg->dwErrorCode);

    return 0;
}

DWORD ToHaveApprentice::OnNS_ApprenticeNotice( tagNS_ApprenticeNotice* pMsg, DWORD )
{
    // 显示对话框
    // 玩家玩家名字（玩家等级）希望拜您为师，您是否接受？
    if(P_VALID(GetObj("ApprenticeNotice")))
        KillObj("ApprenticeNotice");

    TCHAR szTemp[X_LONG_NAME] = {0};
    _sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("ToHaveApprenticeNotice")], PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID), pMsg->nLevel);
    CreateObj("ApprenticeNotice", "MsgBox");
    TObjRef<MsgBox>("ApprenticeNotice")->Init(_T(""), szTemp, _T("ApprenticeNoticeEvent"));

    m_dwTargetID = pMsg->dwRoleID;

    return 0;
}

DWORD ToHaveApprentice::OnNS_ApprenticeReply( tagNS_ApprenticeReply* pMsg, DWORD )
{
    // 显示错误码
    MasterMgr::inst()->ShowErrMsg(pMsg->dwErrorCode);

    return 0;
}

DWORD ToHaveApprentice::OnNS_ApprenticeReplyMaster( tagNS_ApprenticeReplyMaster* pMsg, DWORD )
{
    if (pMsg->bAccept)
    {
        if (pMsg->dwRoleID == m_dwTargetID)
        {
            // 对方是师傅
            TCHAR buffer[256] = {0};
            _sntprintf(buffer, sizeof(buffer) / sizeof(TCHAR), g_StrTable[_T("ApprenticeNotice")], PlayerNameTab::Inst()->FindNameByID(m_dwTargetID));

            // 中央屏幕
            CombatSysUtil::Inst()->ShowScreenCenterMsgEx(buffer);

            // 聊天频道显示
            ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
            if (P_VALID(pFrame))
            {
                pFrame->PushInfo(buffer, ESCC_Current);
            }

            // 设置master id
            LocalPlayer* pPlayer = (LocalPlayer*)RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pPlayer))
            {
                pPlayer->SetMasterID(m_dwTargetID);
            }

        }
        else
        {
            // 自已是师傅
            TCHAR buffer[256] = {0};
            _sntprintf(buffer, sizeof(buffer) / sizeof(TCHAR), g_StrTable[_T("MasterNotice")], PlayerNameTab::Inst()->FindNameByID(m_dwTargetID));

            // 中央屏幕
            CombatSysUtil::Inst()->ShowScreenCenterMsgEx(buffer);

            // 聊天频道显示
            ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
            if (P_VALID(pFrame))
            {
                pFrame->PushInfo(buffer, ESCC_Current);
            }

            // 设置master id
            Player* pPlayer = (Player*)RoleMgr::Inst()->FindRole(m_dwTargetID);
            if (P_VALID(pPlayer))
            {
                pPlayer->SetMasterID(RoleMgr::Inst()->GetLocalPlayerID());
            }
        }

    }
    else
    {
        MasterMgr::inst()->ShowErrMsg(pMsg->dwErrorCode);
    }

    return 0;
}

DWORD ToHaveApprentice::ApprenticeNoticeEvent( tagMsgBoxEvent* pGameEvent )
{
    // 拒绝
    if (pGameEvent->eResult != MBF_OK)
    {
        tagNC_ApprenticeReply m;
        m.dwRoleID  = m_dwTargetID;
        m.bAccept   = false;
        if (m_pSession->IsConnect())
            m_pSession->Send(&m);

        return 0;
    }

    // 同意
    tagNC_ApprenticeReply m;
    m.dwRoleID  = m_dwTargetID;
    m.bAccept   = true;
    if (m_pSession->IsConnect())
        m_pSession->Send(&m);

    return 0;
}