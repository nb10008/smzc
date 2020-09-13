#pragma once
#include "gameframe.h"
#include "..\WorldDefine\msg_compose.h"

class IconStatic;
class ItemButton;
class Item;
/*********************
//!brief:飞升装备炼器界面
*********************/

#define  FOALT_ZERO  0.000001

struct tagNS_ItemRemove;
struct tagEquipDropEvent;

class SoarEquipFrame :
	public GameFrame
{
public:
	SoarEquipFrame(void);
	~SoarEquipFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	VOID  ShowMixFrame();
	VOID  ReceiveNPCID(DWORD dwNPCID);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD NetRecMixEquipMsg( tagNS_BuildEuipment *pNetCmd, DWORD );
	DWORD NetItemRemove(tagNS_ItemRemove *pNetCmd,DWORD);
	DWORD OnCloseSoarEquipFrame(tagGameEvent* pGameEvent);
	DWORD PocketEquipDrop( tagEquipDropEvent* pGameEvent);

	BOOL  EquipMixing();                        //炼器
	VOID  TestSoarEquip(INT nPos);     //检测待飞升装备是否合格
	VOID  TestMixEquip(INT nPos);    //检测飞升合成装备的道具是否合格
	VOID  TestMixStuff(INT nPos);    //检测飞升合成装备的装备是否合格
	VOID  MixPercentCompute();     //计算提升百分比，用以中央显示文字
	VOID  RefreshBtnUI();         //刷新练器3个道具装备栏
	VOID  RefreshProUI();		 	//刷新进度条相关UI
	VOID  DragBtnInMixFrame(ItemButton*	 btn,Item* itm);       //炼器界面dragBtn
	VOID  MixDataInit();                 //初始化炼器界面，用于取消和重置
	VOID  LockItem( bool bOperable );	// 锁定物品
	VOID  UpdateItem(Item *pItem);		// 更新物品
	VOID  SendMixInfoToServer();      //向服务器发送炼器信息
	VOID  DestroyMsgBox();
	VOID  ShowItemTips(GUIWnd* pWnd, Item *pTempItem); 

private:

	TObjRef<GUISystem>			m_pGUI;
	TSFPTrunk<SoarEquipFrame>	m_trunk;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	
	GUIWnd*						m_pWnd;
	ItemButton*					m_pBtnSoarEquip;		//待飞升装备的itembtn
	ItemButton*					m_pBtnMixEquip;
	ItemButton*					m_pBtnMixStuff;
	GUIButton*					m_pBtnConfirmMix;
	GUIButton*					m_pBtnCancel;
	GUIProgress*				m_pProMix;
	GUIPatch*					m_pPatPercentShow;
	GUIStatic*					m_pStcSoarEquip;
	GUIStatic*					m_pStcMixEquip;
	GUIStatic*					m_pStcMixStuff;
	IconStatic*					m_pStcMixResaultShow;
	Item*						m_pItemSoarEquip;		//待飞升装备的item
	Item* 						m_pItemMixEquip;	
	Item*						m_pItemMixStuff;	
	
	DWORD						m_escapeTime;		//耗时，用于判断是否离NPC过远
	BOOL						m_bIfMix;			//炼器通过基本检测
	FLOAT                     m_fSoarPercent;    //飞升百分比
	INT64						m_iSoarEquipType;	
	INT							m_iSoarEquipLevel; //待飞升装备等级
	INT							m_iSoarEquipQuality;//待飞升装备品质
	INT64						m_iEquipType;
	INT                       m_iEquipLevel;    //飞升合成道具的装备等级
	INT                       m_iEquipQuality;  //飞升合成道具装备品质

	BOOL						m_bIfProgressing;  //是否在读进度条
	BOOL						m_bSpeedNormal;   //判断读条速度是否为0.25
	DWORD						m_dwNPCID;

	//tagSaveEquipAndStuffData   m_sSaveInfo;
};
