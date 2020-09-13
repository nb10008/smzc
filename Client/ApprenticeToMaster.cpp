#include "StdAfx.h"
#include "ApprenticeToMaster.h"
#include "MasterMgr.h"
#include "RoleMgr.h"
#include "PlayerNameTab.h"
#include "MsgBox.h"
#include "CombatSysUtil.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "ChatFrame.h"

ApprenticeToMaster::ApprenticeToMaster(void) :
    m_Trunk(this),
    m_dwTargetID(GT_INVALID)
{
}

ApprenticeToMaster::~ApprenticeToMaster(void)
{
}

BOOL ApprenticeToMaster::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    // register net cmd
    m_pCmdMgr->Register("NS_ApprenticeApplyMaster", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_ApprenticeApplyMaster), _T("NS_GetMarriageReq"));
    m_pCmdMgr->Register("NS_MasterNotice", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_MasterNotice), _T("NS_MasterNotice"));
    m_pCmdMgr->Register("NS_MasterReply", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_MasterReply), _T("NS_MasterReply"));
    m_pCmdMgr->Register("NS_MasterReplyApprentice", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_MasterReplyApprentice), _T("NS_MasterReplyApprentice"));

    // register game event
    m_pFrameMgr->RegisterEventHandle(_T("tagGetAquireMasterEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ApprenticeToMaster::OnGetAquireMasterEvent));
    m_pFrameMgr->RegisterEventHandle(_T("MasterNoticeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ApprenticeToMaster::OnMasterNoticeEvent));

    return bRet;
}

BOOL ApprenticeToMaster::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    if(P_VALID(GetObj("MasterNotice")))
        KillObj("MasterNotice");

    // unregister net cmd
    m_pCmdMgr->UnRegister("NS_ApprenticeApplyMaster", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_ApprenticeApplyMaster));
    m_pCmdMgr->UnRegister("NS_MasterNotice", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_MasterNotice));
    m_pCmdMgr->UnRegister("NS_MasterReply", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_MasterReply));
    m_pCmdMgr->UnRegister("NS_MasterReplyApprentice", (NETMSGPROC)m_Trunk.sfp2(&ApprenticeToMaster::OnNS_MasterReplyApprentice));


    // unregister game event
    m_pFrameMgr->UnRegisterEventHandler(_T("tagGetAquireMasterEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ApprenticeToMaster::OnGetAquireMasterEvent));
    m_pFrameMgr->UnRegisterEventHandler(_T("MasterNoticeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ApprenticeToMaster::OnMasterNoticeEvent));

    return bRet;
}

VOID ApprenticeToMaster::Update()
{
    GameFrame::Update();
}

DWORD ApprenticeToMaster::OnGetAquireMasterEvent( tagGetAquireMasterEvent* pGameEvent )
{
    // 发送拜师审请
    tagNC_ApprenticeApplyMaster msg;
    msg.dwRoleID    = pGameEvent->roleID;

    if (m_pSession->IsConnect())
        m_pSession->Send(&msg);

    return 0;
}

DWORD ApprenticeToMaster::OnNS_ApprenticeApplyMaster( tagNS_ApprenticeApplyMaster* pMsg, DWORD )
{
    // 显示错误码
    MasterMgr::inst()->ShowErrMsg(pMsg->dwErrorCode);

    return 0;
}

DWORD ApprenticeToMaster::OnNS_MasterNotice( tagNS_MasterNotice* pMsg, DWORD )
{
    // 显示对话框
    // 玩家玩家名字（玩家等级）希望拜您为师，您是否接受？
    if(P_VALID(GetObj("MasterNotice")))
        KillObj("MasterNotice");

    TCHAR szTemp[X_LONG_NAME] = {0};
    _sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("ApprenticeToMasterNotice")], PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID), pMsg->nLevel);
    CreateObj("MasterNotice", "MsgBox");
    TObjRef<MsgBox>("MasterNotice")->Init(_T(""), szTemp, _T("MasterNoticeEvent"));

    m_dwTargetID = pMsg->dwRoleID;

    return 0;
}

DWORD ApprenticeToMaster::OnNS_MasterReply( tagNS_MasterReply* pMsg, DWORD )
{
    // 显示错误码
    MasterMgr::inst()->ShowErrMsg(pMsg->dwErrorCode);

    return 0;
}

DWORD ApprenticeToMaster::OnNS_MasterReplyApprentice( tagNS_MasterReplyApprentice* pMsg, DWORD )
{
    if (pMsg->bAccept)
    {
        if (pMsg->dwRoleID == m_dwTargetID)
        {
            // 对方是徒弟
            TCHAR buffer[256] = {0};
            _sntprintf(buffer, sizeof(buffer) / sizeof(TCHAR), g_StrTable[_T("MasterNotice")], PlayerNameTab::Inst()->FindNameByID(m_dwTargetID));

            // 中央屏显
            CombatSysUtil::Inst()->ShowScreenCenterMsgEx(buffer);

            // 聊天频道显示
            ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
            if (P_VALID(pFrame))
            {
                pFrame->PushInfo(buffer, ESCC_Current);
            }

            // 设置masterid
            Player* pPlayer = (Player*)RoleMgr::Inst()->FindRole(m_dwTargetID);
            if (P_VALID(pPlayer))
            {
                pPlayer->SetMasterID(RoleMgr::Inst()->GetLocalPlayerID());
            }
        }
        else
        {
            // 自已是徒弟
            TCHAR buffer[256] = {0};
            _sntprintf(buffer, sizeof(buffer) / sizeof(TCHAR), g_StrTable[_T("ApprenticeNotice")], PlayerNameTab::Inst()->FindNameByID(m_dwTargetID));

            // 中央屏显
            CombatSysUtil::Inst()->ShowScreenCenterMsgEx(buffer);

            // 聊天频道显示
            ChatFrame* pFrame = (ChatFrame*)m_pFrameMgr->GetFrame(_T("Chat"));
            if (P_VALID(pFrame))
            {
                pFrame->PushInfo(buffer, ESCC_Current);
            }

            // 设置masterid
            LocalPlayer* pPlayer = (LocalPlayer*)RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pPlayer))
            {
                pPlayer->SetMasterID(pMsg->dwRoleID);
            }
        }

    }
    else
    {
        MasterMgr::inst()->ShowErrMsg(pMsg->dwErrorCode);
    }
    return 0;
}

DWORD ApprenticeToMaster::OnMasterNoticeEvent( tagMsgBoxEvent* pGameEvent )
{
    // 拒绝
    if (pGameEvent->eResult != MBF_OK)
    {
        tagNC_MasterReply m;
        m.dwRoleID  = m_dwTargetID;
        m.bAccept   = false;
        if (m_pSession->IsConnect())
            m_pSession->Send(&m);

        return 0;
    }

    // 同意
    tagNC_MasterReply m;
    m.dwRoleID  = m_dwTargetID;
    m.bAccept   = true;
    if (m_pSession->IsConnect())
        m_pSession->Send(&m);

    return 0;
}