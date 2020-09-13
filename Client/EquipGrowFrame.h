#pragma once
#include "ComposeStatic.h"
#include "ItemEvent.h"
#include "..\WorldDefine\msg_compose.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "Item.h"
#include "ActiveProgress.h"

#define MAX_GROWATT_NUM 5
struct tagItemPosChangeExEvent;

class EquipGrowFrame : public GameFrame
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
	EquipGrowFrame(void);
	~EquipGrowFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

	//清理材料
	VOID ClearUI();
	//清理继承成功后数据
	VOID ClearTransUI();
	//拖动释放
	VOID HandleDragStuff(tagGUIEvent* pEvent);
	//显示
	VOID Show();
	//隐藏
	VOID Hide();
	//搜索背包查找器魂碎片
	//VOID FindQiSoulFromBag();
	//搜索背包查找成长幸运符
	VOID FindGrowLuckSign();
	//搜索背包查找继承符
	VOID FindInheritSign();
	//根据map和索引获取物品的TypeID
	DWORD GetItemTypeIDByMap(map<DWORD, INT64>& DestMap, INT nIndex);
	INT64 GetItem64IDByMap(map<DWORD, INT64>& DestMap, INT nIndex);
	//刷新信息
	VOID RefreshInfo();
	//显示错误码
	VOID ShowErrorCode(DWORD dwErrorCode);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventProduceAddItem(tagItemPosChangeExEvent* pEvent);
	DWORD OnOpen_EquipGrow(tagGameEvent* pEvent);
	DWORD OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pGameEvent);

	DWORD OnNS_EquipGrow(tagNS_EquipGrow* pCmd, DWORD);
	DWORD OnNS_EquipGrowTrans(tagNS_EquipGrowTrans* pCmd, DWORD);
	DWORD OnNS_EquipGrowDec(tagNS_EquipGrowDec* pCmd, DWORD);

private:
	TSFPTrunk<EquipGrowFrame>				m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*									m_pWndMain;
	GUIPushButton*							m_pPBtnEquipGrow;		//装备成长PushButton
	GUIPushButton*							m_pPBtnEquipInherit;	//装备继承PushButton
	GUIButton*								m_pBtnClose;
	//--------------------装备成长-----------------------
	GUIWnd*									m_pWndEquipGrow;		//装备成长面板
	ComposeStatic*							m_pCStcGrowEquip;		//装备成长装备图标
	//IconStatic*								m_pIStcQiSoulIcon;		//器魂碎片图标
	//GUIListBox*								m_pLBoxQiSoul;			//器魂列表
	//GUIEditBox*								m_pEBoxQiSoul;	
	IconStatic*								m_pIStcLuckSign;		//幸运符图标		
	GUIListBox*								m_pLBoxLuckSign;		//幸运符列表
	GUIEditBox*								m_pEBoxLuckSign;			
	GUIStatic*								m_pStcEquipAtt[MAX_GROWATT_NUM];
	GUIEditBox*								m_pEBoxGrowNum;
	IconStatic*								m_pIStcGrowStar[EQUIP_GROW_MAX_LEVEL-1];
	ActiveProgress*							m_pProGroExp;
	GUIStatic*								m_pStcGrowExp;
	GUIButton*								m_pBtnBeginGrow;		//开始成长
	GUIButton*								m_pBtnStarReduice;		//降星按钮
	GUIStatic*								m_pStcGrowTips;			//提示差多少点升级
	//--------------------装备继承-----------------------
	GUIWnd*									m_pWndEquipInherit;		//装备继承面板
	ComposeStatic*							m_pCStcOriginalEquip;	//原始装备（带成长属性的装备）
	ComposeStatic*							m_pCStcDestEquip;		//目标装备（要增加成长属性的装备）
	IconStatic*								m_pIStcGrowInheritSign;	//成长继承符图标
	GUIListBox*								m_pLBoxGrowInheritSign;	//成长继承符列表
	GUIEditBox*								m_pEBoxGrowInheritSign;
	GUIStatic*								m_pEBoxGrowInheritAtt[MAX_GROWATT_NUM];
	IconStatic*								m_pIStcInheritStar[EQUIP_GROW_MAX_LEVEL-1];
	GUIStatic*								m_pStcInheritExp;
	GUIButton*								m_pBtnBeginInherit;		//开始继承

	std::list<Item*>						m_listQiHunItem;
	std::list<Item*>::iterator				m_QlistiHunIt;
	std::map<DWORD, INT64>					m_mapQiHunItem;			//器魂碎片map
	std::map<DWORD, INT64>					m_mapLuckSignItem;		//幸运符map
	std::map<DWORD, INT64>					m_mapInheritItem;		//装备继承道具
	std::map<DWORD, INT64>::iterator		m_Tempit;

	INT										m_nListQiHunIndex;		//器魂选中第几项
	INT										m_nListLuckSignIndex;	//幸运符选中第几项
	INT										m_nListInheritIndex;	//继承符选中第几项

	INT64									m_n64GrowEquipID;		//成长装备ID
	INT64									m_n64InheritEquipID;	//继承装备ID，带成长属性的那件
	INT64									m_n64DestEquipID;		//继承装备ID，后面那个..

	//DWORD									m_dwEquipGrowItemID;	//器魂碎片TypeID	
	DWORD									m_dwGrowValue;			//成长值
	DWORD									m_dwEquipGrowLuckSignID;//幸运符ID
	INT64									m_n64EquipInheritID;	//装备继承道具ID

	DWORD									m_updateTimeDistance;
};