#pragma once
#include "gameframe.h"
#include "SocialEvent.h"
#include "ReputeMgr.h"
#include "RoleTitleProtoData.h"
#include "RoleTitleFrame.h"
#include "TreeEx.h"
class RemoteRoleStateFrame;
#include "..\WorldDefine\msg_role_title.h"
#include "..\WorldDefine\msg_reputation.h"

class IconStatic;
struct tagNS_GetRoleVCard;

class TargetCharacterFrame :
    public GameFrame
{
public:
    TargetCharacterFrame(void);
    ~TargetCharacterFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
	virtual BOOL EscCancel();
	VOID Show(BOOL bVal) { ShowWnd(bVal); }

    tstring GetGUIFullName()
    {
        if(P_VALID(m_pWnd))
            return m_pWnd->GetFullName();
        else 
            return _T("");
    };
	
private:
    //! 处理GUI消息
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! 显示目标玩家装备,声望,称号界面
    DWORD OnDispPlayerState(tagDispPlayerState* pMsg);

    //! 显示窗体
    VOID ShowWnd(BOOL bVal);

    //! 重置声望显示
    VOID ResetRepute();
    //! 发送获得声望请求
    VOID BeginNC_GetReputation(DWORD dwRoleID);
    //! 返回声望结果
    DWORD OnNS_GetReputation(tagNS_GetReputation* pMsg, DWORD);

	//查看玩家信息请求
	DWORD OnNS_CheckRoleInfo(tagNS_CheckRemoteRoleInfoOpenType* pMsg, DWORD);
    //! 重置称号显示
    VOID ResetTitle();
    //! 发送称号获得请求
    VOID BeginNC_GetRoleTitles(DWORD dwRoleID);
    //! 返回称号结果
    DWORD OnNS_GetRoleTitles(tagNS_GetRoleTitles* pMsg, DWORD);
    //! 初始化所有称号xml数据到控件
    VOID InitTree();
    //! 跟据称号变化刷新控件
    VOID UpdateTree();
    //! 选中某一称号
    VOID SelectRoleTitle();

	void ShowErrorMsg( DWORD dwError );

	// 重置装备
	VOID ResetRemoteRoleEquip();

	// 打开声望分页事件
	DWORD OnEventOpenRepute(tagOpenReputeEvent *pEvent);
	// 打开装备分页事件
	DWORD OnEventOpenEquip(tagOpenEquipEvent *pEvent);
	// 处理不在线玩家查看装备事件
	DWORD OnEventOfflinePlayerEquip(tagGameEvent *pEvent);

	//处理获取角色名贴数据网络消息
	DWORD OnNS_GetRoleVCard(tagNS_GetRoleVCard* pMsg, DWORD dwParam);

	VOID SendGetRoleVCard();

    map<DWORD, tagTitleList>		m_mapOwnedTitles;		/*!< 保存角色已获得称号的容器 */
    map<DWORD, tagTitleTypeActivedCount> m_mapActivedCount; /*!< 此类title获得的数量 */

    TSFPTrunk<TargetCharacterFrame> m_Trunk;
    TObjRef<GUISystem>			    m_pGUI;
    TObjRef<Util>				    m_pUtil;
    TObjRef<GameFrameMgr>		    m_pFrameMgr;
    TObjRef<NetCmdMgr>				m_pCmdMgr;
    TObjRef<NetSession>				m_pSession;

    // 主窗体

    GUIWnd*						    m_pWnd;             //!< 主窗体
    GUIWnd*                         m_pEquip;           //!< 装备子窗体
    GUIWnd*                         m_pRepute;          //!< 声望窗体
    GUIWnd*                         m_pTitle;           //!< 称号窗体
	GUIWnd*                         m_pInfo;           //!< 名帖窗体
    
    GUIPushButton*                  m_pBtEquip;         //!< 进入装备
    GUIPushButton*                  m_pBtReput;         //!< 进入声望 
    GUIPushButton*                  m_pBtTitle;         //!< 进入称号
	GUIPushButton*                  m_pBtInfo;         //!< 进入名帖

    GUIButton*                      m_pBtClose;         //!< 关闭

    // 声望相关
	GUIStatic*                  m_pStcGuildExploit;     //!< 帮派功勋
	GUIStatic*                  m_pStcGuildContribute;  //!< 帮派贡献

	GUIStatic*                  m_pStcStrengthValue;    //!< 实力总值
	GUIStatic*					m_pStcLevel;			// 等级实力值
	GUIStatic*					m_pStcEquip;			// 装备实力值
	GUIStatic*					m_pStcSprite;			// 妖精实力值
	//GUIStatic*					m_pStcTitle;			// 称号实力值
	//GUIStatic*					m_pStcFriend;			// 好友实力值

	GUIStatic*                  m_pStcWuXun;            //!< 武勋
	GUIStatic*                  m_pStcWuJi;             //!< 武技
	GUIStatic*                  m_pStcTotal;	        //!< 3v3 总场数
	GUIStatic*                  m_pStcWinCount;         //!< 3v3 胜率

	GUIStatic*					m_pStcFlowerNum;		//鲜花数
	GUIStatic*					m_pStcEggNum;			//鸡蛋数

    // 称号相关
    TreeEx*                         m_pTreeTitleList;
    GUIStaticEx*                    m_pStcexDescription;

    DWORD                           m_dwCurrPlayerID;    //!< 当前查看玩家ID
	RemoteRoleStateFrame*			m_pRRSF;

    bool                            m_bneedRefreshRepute;
    DWORD                           m_timeFirst;


	GUIStatic*							m_pStcSpouse;			//配偶
	GUIStatic*							m_pStcGender;			//性别
	GUIStatic*							m_pStcConstellation;	//星座
	GUIStatic*							m_pStcCity;				//城市
	GUIStatic*							m_pStcChZodiac;			//生肖
	GUIEditBoxEx*					m_pEditBoxExPresent;	//个性签名
	GUIEditBoxEx*					m_pEditNameHistory;		//更名记录
	IconStatic*						m_pStcHeadPic;			//头像
	GUIStatic*							m_pStcHeadPicC;			//头像框

	GUIButton*						m_pBtRefresh;			//刷新
	GUIButton*						m_pBtClose2;				//关闭
};
