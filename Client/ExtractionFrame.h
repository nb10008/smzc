//萃取
#pragma once
#include "ExtractionFrame.h"
#include "CompClidateDefine.h"
#include "ComColidateEvent.h"
#include "ItemEvent.h"

class ComposeStatic;
struct tagNS_Extraction;

class ExtractionFrame : public GameFrame
{
private:
	enum EDeComposeMsg
	{
		EDeCompose_NULL = 0,
		EDeCompose_SendMsg,		
		EDeCompose_Waiting,
		EDeCompose_RecvMsg,
	};
public:
	ExtractionFrame(void);
	~ExtractionFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	//清除界面及保存数据
	void ClearUIandStuff();
	//判断配方是否满足
	void JudgeDecompositonFormula(DWORD dwItemID, INT64 n64ID, EEquipPos ePos);
	//读条
	void JudgeSendDecompose();
	//判断装备是否可点化
	DWORD HandleDragStuff( tagGUIEvent* pEvent );
	//重置界面状态
	void ResetStartUIState();
	//发送萃取消息
	void SendDecomposeMsg();
	//显示窗口
	void Show();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventProduceAddItem( tagItemPosChangeExEvent* pGameEvent );
	DWORD OnOpenExtractFrame(tagGameEvent* pGameEvent);

	DWORD OnNS_Extraction(tagNS_Extraction* pCmd, DWORD);

private:
	TSFPTrunk<ExtractionFrame>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*								m_pWndMain;
	GUIPatch*							m_pWndCaption;
	GUIButton*							m_pBtnExit;
	GUIButton*							m_pBtnCancel;
	GUIButton*							m_pBtnOk;
	GUIListBox*							m_pDeStuffList;			//成分出材料
	ComposeStatic*						m_pDeUnit;				//分解物品
	GUIStatic*							m_pStcPotVal;			//活力
	GUIProgress*						m_pProTime;				//进度条
	GUIStatic*							m_pStcTip;				//点化的那个提示信息
	GUIStatic*							m_pStcGoldCost;			//消耗金子
	GUIStatic*							m_pStcSilverCost;		//消耗银子

	DWORD								m_dwEquipID;						//当前萃取的装备ID
	DWORD								m_dwRecvMsgTime;					//成功\失败\完美时提示的时间
	CompConsoContainer					m_Container;						//材料容器
	EDeComposeMsg						m_eCurState;						//当前消息状态
	DWORD								m_escapeTime;
	DWORD								m_dwNPCGolobelID;		//NPC全局ID
};