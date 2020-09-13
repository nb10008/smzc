#pragma once
#include "gameframe.h"

const int TEXT_MAX_COLUMN = 12;

struct tagNS_RoleLevelChange;

//! \class  LevelUpShowFrame
//! \brief  屏显升级提示
//! \base   GameFrame
//! \author hyu
//! \date   2010-1-18
class LevelUpShowFrame :
    public GameFrame
{
public:
    LevelUpShowFrame(void);
    ~LevelUpShowFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual BOOL EscCancel();

    void ShowWnd(bool bVisible);
	BOOL IsWndInvisible(){ return m_pWnd->IsInvisible();}

    void OnLevelUp(tagNS_RoleLevelChange* pNetCmd);

private:
    DWORD EventHandler(tagGUIEvent* pEvent);

    // GUI Controls
    GUIWnd*                     m_pWnd;

    //GUIStatic*                  m_pStcMsg[TEXT_MAX_COLUMN];      //!< 信息第一行

    //GUIStatic*                  m_pStcCongratulation;            //!< 祝贺

    GUIButton*                  m_pBtExit;       //!< 关闭

    vector<tstring>             m_vecText;       //!< 升级文本缓存
    int                         m_nBeginRow;     //!< 开始行数

    DWORD                       m_dwStartShowTime;  //!< 开始刷新时的
    DWORD                       m_dwTextUpdate;     //!< 文字刷新间隔

    // tools
    TSFPTrunk<LevelUpShowFrame> m_Trunk;
    TObjRef<GUISystem>          m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;



};
