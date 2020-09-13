#pragma once
#include "gameframe.h"

class StaticEx;
class IconStatic;

//! \class  PetStateFrame
//! \brief  本地玩家宠物状态显示
//! \author hyu
//! \base   GameFrame
//! \sa     PetManager
class PetStateFrame :
    public GameFrame
{
public:
    PetStateFrame(void);
    ~PetStateFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
    virtual BOOL EscCancel();
	VOID SetFairyMoodPic(INT nMood);

private:
    //! 处理GUI消息
    DWORD EventHandler(tagGUIEvent* pEvent);

    void ShowWnd(bool bVisable);

    //! 更新宠物信息
    void UpdatePet();

	tstring GetFairyMoodState();

    TSFPTrunk<PetStateFrame>    m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;

    DWORD           m_dwStartTime;          //!< 起始更新时间点

    //GUI控件
    GUIWnd*						m_pWnd;
    //GUIStatic*                  m_pStcName;     //!< 名字
    StaticEx*                   m_pStcIcon;     //!< 图标
    GUIProgress*                m_pPgssEnergy;  //!< 精气 // 心情
    GUIProgress*                m_pPgssExp;     //!< 经验 // 经验
	GUIStatic*					m_pStcSaveExp;
	IconStatic *m_pIconFeed;
	IconStatic *m_pIconMood;
	IconStatic *m_pIconLearn;

	INT16 m_nFairyMood;
	INT16 m_nFairyRealMood;
};
