/********************************************************************
	created:	2008/08/16
	created:	16:8:2008   17:36
	filename: 	d:\loong_client\Client\QuestActiveFrame.h
	file path:	d:\loong_client\Client
	file base:	QuestActiveFrame
	file ext:	h
	author:		leili
	
	purpose:	固定活动任务界面声明
*********************************************************************/
#pragma once

#include "QuestQuery.h"
#include "ActivityMgr.h"

class QuestActiveFrame : public GameFrame
{
public:
	QuestActiveFrame(void);
	~QuestActiveFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	VOID		 Show(void);
	VOID		 Hide(void)		{ if( P_VALID(m_pWnd) ) m_pWnd->SetInvisible(TRUE); }

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	DWORD		 GetActivityColour(ENewActivityType eType);
	VOID		 ShowQuest(void);
	VOID		 ShowDesc(UINT16 questID);
	VOID		 ShowDailyDesc(UINT16 questID); //显示日常任务说明专用
	VOID		 ShowFestival();
	bool		 CompareTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute)
	{
		if (((SrcHour * 60 + SrcMinute) - (DestHour * 60 + DestMinute)) > 0)
			return true;
		return false;
	}
	bool		 SameStartTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute)
	{
		if (((SrcHour * 60 + SrcMinute) - (DestHour * 60 + DestMinute)) == 0)
			return true;
		return false;
	}
	bool		 CompareDay(int SrcMonth, int SrcDay, int DestMonth, int DestDay)
	{
		if(SrcMonth > DestMonth
		||(SrcMonth == DestMonth && SrcDay >= DestDay )) 
			return true;

		return false;
	}

private:
	TSFPTrunk<QuestActiveFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;

	GUIWnd						*m_pWnd;				//主界面
	GUIListBox					*m_pLbActivityList;		// 节日列表
	GUIPushButton				*m_pPbToday;            // 今日活动按钮
	GUIPushButton				*m_pPbDaily;			// 日常任务按钮

	GUIPatch					*m_pStTodayDescOne;          // 第一个标题
	GUIPatch					*m_pStTodayDescTwo;          // 第二个标题
	GUIPatch					*m_pStTodayDescThree;        // 第三个标题

	GUIPatch					*m_pStDailyDescOne;          // 第一个标题
	GUIPatch					*m_pStDailyDescTwo;          // 第二个标题
	GUIPatch					*m_pStDailyDescThree;        // 第三个标题

	GUIListBox					*m_pLbTodayList;		// 今日活动列表
	GUIListBox					*m_pLbDailyList;		// 日常活动列表
	GUIStaticEx					*m_pStDesc;             // 任务描述窗口
	GUIPushButton				*m_pPshSwitchCan;		// 过滤可做活动
	GUIButton					*m_pBtnCloseWnd;		// 关闭窗口X号
	GUIButton					*m_pBtnCloseWndButton;	// 关闭窗口按钮

	BOOL						m_bIfPbTodayFlash;        //今日活动按钮是否需要闪烁判断

	ActivityMgr::ESortType		m_curSortType;
	DWORD						m_curRow;
	DWORD						m_currentRowforActivity;
	bool						m_showCan;
	ActivityMgr::ESortType		m_sortType;
	std::vector<int>			m_curShowQuestIDs; 
	std::map<int,int>           m_curShowQuestPosIDMap; // 当前任务位置和ID对照表,为今日活动列表特别制作
	std::map<int,int>           m_curShowActivityIDMap; // 当前任务位置和ID对照表,为今日活动列表特别制作
};