#pragma once
#include "gameframe.h"
#include "MasterEvent.h"

struct tagGetAquireMasterEvent;

struct tagNS_ApprenticeApplyMaster;
struct tagNS_MasterNotice;
struct tagNS_MasterReply;
struct tagNS_MasterReplyApprentice;

//! \class   ApprenticeToMaster
//! \breif   拜师过程
//! \author  hyu
//! \date    2010-1-31
//! \last    2010-1-31
class ApprenticeToMaster :
    public GameFrame
{
public:
    ApprenticeToMaster(void);
    ~ApprenticeToMaster(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual VOID Update();

private:
    // 接收请求拜师的请求
    DWORD OnGetAquireMasterEvent(tagGetAquireMasterEvent* pGameEvent);

    DWORD OnMasterNoticeEvent(tagMsgBoxEvent* pGameEvent);

    DWORD OnNS_ApprenticeApplyMaster(tagNS_ApprenticeApplyMaster* pMsg, DWORD);
    DWORD OnNS_MasterNotice(tagNS_MasterNotice* pMsg, DWORD);
    DWORD OnNS_MasterReply(tagNS_MasterReply* pMsg, DWORD);
    DWORD OnNS_MasterReplyApprentice(tagNS_MasterReplyApprentice* pMsg, DWORD);

    DWORD                           m_dwTargetID;

    // tools
    TSFPTrunk<ApprenticeToMaster>   m_Trunk;
    TObjRef<GUISystem>              m_pGUI;
    TObjRef<Util>				    m_pUtil;
    TObjRef<NetCmdMgr>			    m_pCmdMgr;
    TObjRef<GameFrameMgr>		    m_pFrameMgr;
    TObjRef<NetSession>			    m_pSession;
};
