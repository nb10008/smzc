#pragma once
#include "gameframe.h"

/**
	\brief 神龙赐福 是新手登录游戏，以10分钟为单位向玩家赠送礼物
		   主要是为了防止新手的流失
    \auth  dyb
*/
class IconStatic;
class ServerTime;
class GlintStatic;
class FlashButton;
struct tagNS_GetLoongBenediction;
struct tagNS_SetNextBenediction;

class LoongBenedictionFrame : public GameFrame
{
	enum ELoongBenedictionStep
	{
		EBeneStep_Null = -1,
		EBeneStep_Count,			//按照次数领
		EBeneStep_Time				//按照每日在线时间领
	};

public:
	LoongBenedictionFrame(void);
	~LoongBenedictionFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	
	void SetBenedictionStep( INT nType )
	{
		m_step = (ELoongBenedictionStep)nType;
		if( P_VALID(m_pBtnClose) )
		{
			if( m_step == EBeneStep_Time )
				m_pBtnClose->SetInvisible( FALSE );
			else
				m_pBtnClose->SetInvisible( TRUE );
		}
	}

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	VOID SafeDestroyUI();

	/** 获得奖励
	*/
	VOID GetBenediction();
	
	/** 服务器针对获取奖励的返回事件
	*/
	DWORD OnNS_GetBenediction( tagNS_GetLoongBenediction* pMsg, DWORD dwParam );

	/** 服务器发送消息设置下一次赐福时间，以及物品信息
	*/
	DWORD OnNS_SetNextBenediction( tagNS_SetNextBenediction* pMsg, DWORD dwParam );

	/** 判断当前窗口是否是展开的
	*/
	BOOL IsWindowExpand();

	VOID ShowWnd( GUIWnd* pWnd, BOOL bShow );

	VOID EnableWnd( GUIButton* pBtn, bool bEnable );

	void BeginGlint();

	void EndGlint();

private:
	TSFPTrunk<LoongBenedictionFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	GUIWnd*							m_pWndExpand;
	GUIStatic*						m_pStcWaitTime;
	FlashButton*					m_pBtnGetBenediction;
	GUIButton*						m_pBtnArrow;			//折叠
	//GUIStatic*						m_pStcExprence;			//获得的经验

	IconStatic*						m_pStcItem;				//获得的道具
	GUIButton*						m_pBtnClose;			//关闭按钮

	GUIWnd*							m_pWndFold;					//折叠起来之后的界面
	//GlintStatic*					m_pStcFoldWaitTime;
	FlashButton*					m_pBtnFoldGetBenediction;
	GUIButton*						m_pBtnFoldArrow;			//折叠
	GUIButton*						m_pBtnFoldClose;			//关闭按钮
	GUIWnd*							m_pWndCurDisplay;			//得到当前正在显示的控件

	DWORD							m_nItemTypeId;				//将要获得的道具的ID
	INT								m_nItemCount;				//奖励物品的个数

	DWORD							m_dwNeedTime;				//到下一次奖励需要的秒数
	float							m_fStepTime;				//倒计时
	DWORD							m_dwStartTimer;				//计时器

	BOOL							m_bCanUpdate;				//能否更新，在没有收到服务器发来的 设置下一次赐福时间 之前是不能更新的

	ELoongBenedictionStep			m_step;
	BOOL							m_bSendAtZero;				//零点后，会和服务器同步一次时间，方便数据库计时，发送一次就行了

};
