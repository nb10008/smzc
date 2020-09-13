#pragma once
#include "gameframe.h"
#include "MasterEvent.h"

struct tagAquireApprenticeEvent;

struct tagNS_MasterAcceptApprentice;
struct tagNS_ApprenticeNotice;
struct tagNS_ApprenticeReply;
struct tagNS_ApprenticeReplyMaster;

//! \class   ApprenticeToMaster
//! \breif   ÊÕÍ½¹ý³Ì
//! \author  hyu
//! \date    2010-1-31
//! \last    2010-1-31
class ToHaveApprentice :
    public GameFrame
{
public:
    ToHaveApprentice(void);
    ~ToHaveApprentice(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual VOID Update();

private:
    DWORD OnAquireApprenticeEvent(tagAquireApprenticeEvent* pGameEvent);

    DWORD ApprenticeNoticeEvent(tagMsgBoxEvent* pGameEvent);

    DWORD OnNS_MasterAcceptApprentice(tagNS_MasterAcceptApprentice* pMsg, DWORD);
    DWORD OnNS_ApprenticeNotice(tagNS_ApprenticeNotice* pMsg, DWORD);
    DWORD OnNS_ApprenticeReply(tagNS_ApprenticeReply* pMsg, DWORD);
    DWORD OnNS_ApprenticeReplyMaster(tagNS_ApprenticeReplyMaster* pMsg, DWORD);

    DWORD                           m_dwTargetID;

    // tools
    TSFPTrunk<ToHaveApprentice>     m_Trunk;
    TObjRef<GUISystem>              m_pGUI;
    TObjRef<Util>				    m_pUtil;
    TObjRef<NetCmdMgr>			    m_pCmdMgr;
    TObjRef<GameFrameMgr>		    m_pFrameMgr;
    TObjRef<NetSession>			    m_pSession;
};
