#pragma once
#include "gameframe.h"
#include "..\WorldDefine\ItemDefine.h"

class StaticEx;
class Item;
class  ItemButton;
struct tagSpecItemEvent;
struct tagItemPosChangeExEvent;
struct tagEquipDropEvent;
struct tagRefreshMaintainValueUIEvent;
struct tagNS_ItemRemove;
struct tagNS_StarEnhance;
struct tagNS_EquipSingleChange;
struct tagNS_HolyEquipHole;				//元神装备凿孔
struct tagNS_HolyEquipInlay;				//元神装备镶嵌
struct tagNS_HolyEquipRemove;		//元神装备摘除

enum EHolyEquipCtrlType
{
	EECT_Inlay			=0,			//镶嵌
	EECT_Chisel			=1,			//凿空
	EECT_Compose	=2,			//充星

	EECT_End				=3,			//类型数目
};

class HolyEquipFrame :
	public GameFrame
{
public:
	HolyEquipFrame(void);
	~HolyEquipFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update(VOID);

	//打开界面 
	VOID ShowWnd(BOOL bVal);

private:
	//公共
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnUseItemEvent(tagSpecItemEvent* pGameEvent);
	DWORD OnEquipPosEvent(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnItemDropCheck(tagEquipDropEvent* pGameEvent);
	DWORD OnChangeItemEvent(tagItemPosChangeExEvent* pGameEvent);
	//DWORD RevRefreshFrame( tagRefreshMaintainValueUIEvent* pGameEvent);
	DWORD NetItemRemove(tagNS_ItemRemove* pMsg, DWORD pPrama);
	DWORD OnNS_StarEnhance(tagNS_StarEnhance* pMsg, DWORD pPrama);

	DWORD OnNS_HolyEquipHole(tagNS_HolyEquipHole* pMsg, DWORD pPrama);
	DWORD OnNS_HolyEquipInlay(tagNS_HolyEquipInlay* pMsg, DWORD pPrama);
	DWORD OnNS_HolyEquipRemove(tagNS_HolyEquipRemove* pMsg, DWORD pPrama);
	
	BOOL	AddItem(INT16 nPos);
	VOID	RefreshUI();
	BOOL	ClearEquip();
	VOID	ClearSubData();
	VOID	LockItem(bool bOperable,	EHolyEquipCtrlType eType=EECT_End);
	BOOL	ClearUI(EHolyEquipCtrlType eType =EECT_End);
	VOID	SwitchPage(EHolyEquipCtrlType eType =EECT_End);
	tstring	GetPicFileByQuality(INT nQuality);
	tstring  GetHoleInfo( INT index, EHolyEquipCtrlType eType,DWORD &dwQuality );

	//冲星
	BOOL	CanHolyCompose(); //能够 
	VOID	KeepHolyCompose(); //继续 
	VOID	BeginHolyCompose(); //开始 
	VOID	StopHolyCompose(); //中断 
	VOID	ChangeItemNum();
	VOID	SendHolyComposeMsg();
	VOID	RefreshComposeUI();

	//镶嵌
	DWORD	IfPosCanInlay(INT nIndex);		//位置是否可镶嵌. 该位置需已开凿且未镶嵌, index 从0开始,返回错误吗
	BOOL	ChooseInlayPos(INT nIndex);		//选中镶嵌位置
	BOOL	CanHolyInlay(); //能够
	VOID	BeginHolyInlay(); //开始镶嵌
	BOOL	CanHolyUninlay(); //能够摘除 
	VOID	BeginHolyUninlay(); //开始摘除 
	VOID	StopHolyInlay(); //中断 
	VOID	RefreshInlayUI();
	DWORD OnMsgBox_UninlayConfirm(tagMsgBoxEvent* pGameEvent);  
	VOID	SendHolyInlayMsg();
	VOID	SendHolyUninlayMsg();

	//凿孔
	BOOL	CanHolyChisel(); //能够
	VOID	BeginHolyChisel(); //开始 
	VOID   KeepHolyChisel(); //继续 
	VOID	StopHolyChisel(); //中断 
	VOID	RefreshChiselUI();
	VOID	SendHolyChiselMsg();

private:
	TSFPTrunk<HolyEquipFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>								m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	//------元神装备强化界面
	GUIWnd*							m_pWndFather;
	GUIWnd*							m_pWnd;
	ItemButton*						m_pIBEquip;
	Item*								m_pEquip;
	GUIStatic*						m_pStcSucc;	//成功
	GUIStatic*						m_pStcFail;	//失败
	EHolyEquipCtrlType		m_eCurCtrlType;
	BOOL								m_bIfProgress;
	DWORD							m_escapeTime;

	//冲星分页
	GUIPatch*						m_pPthCompose;
	ItemButton*						m_pIBComposeItem;	//辉耀石
	Item*								m_pItemCompose;
	GUIPushButton*				m_pBtnPageCompose[EECT_End];		//冲星分页3个换页按钮
	GUIStatic*						m_pStcStarLevel;
	GUIStatic*						m_pStcStarNum[MAX_STARENHANCE_LEVEL];
	GUIStatic*						m_pStcSuccRate;
	GUIStatic*						m_pStcReturnLv;
	GUIStatic*						m_pStcAddAttName;
	GUIStatic*						m_pStcAddAttValue;
	GUIStatic*						m_pStcCostGold;
	GUIStatic*						m_pStcCostSilver;
	GUIProgress*					m_pProHolyCompose;
	GUIButton*						m_pBtnStartComp;
	GUIButton*						m_pBtnCancelComp;
	GUIPushButton*				m_pPBItemNum;
	GUIPushButton*				m_pPBToLevel;
	GUIEditBox*					m_pEBItemNum;
	GUIEditBox*					m_pEBToLevel;
	INT									m_nItemCostNum;

	//镶嵌分页
	GUIPatch*						m_pPthInlay;
	ItemButton*						m_pIBInlayItem;  //神石				 
	Item*								m_pItemInlayItem;
	ItemButton*						m_pIBInlayStuff;	//老君			 
	Item*								m_pItemInlayStuff;
	GUIPushButton*				m_pBtnPageInlay[EECT_End];		//镶嵌分页3个换页按钮
	StaticEx*							m_pSteInlayHole[MAX_EQUIPHOLE_NUM];  
	GUIPushButton*				m_pBtnInlayHole[MAX_EQUIPHOLE_NUM];  
	GUIButton*						m_pBtnInlayMinStuff;
	GUIButton*						m_pBtnInlayAddStuff;
	GUIStatic*						m_pStcInlayRate;
	GUIStatic*						m_pStcInlayGold;
	GUIStatic*						m_pStcInlaySilver;
	GUIProgress*					m_pProInlayOnce;
	GUIButton*						m_pBtnInlayBegin;
	GUIButton*						m_pBtnInlayExtirpate;
	GUIButton*						m_pBtnInlayCancel;
	INT									m_nInlayStuffNum;
	INT									m_nInlayPos;			// 0 到 最大值-1
	enum EHolyInlayType
	{
		EHIT_Null			= 0,			//无操作
		EHIT_Inlay		= 1,			//镶嵌
		EHIT_Uninlay	= 2,			//摘除
	};
	EHolyInlayType				m_eHolyInlayType;

	//凿孔分页
	GUIPatch*						m_pPthChisel;
	ItemButton*						m_pIBChiselItem;//穿箭	 
	Item*								m_pItemChisel;
	GUIPushButton*				m_pBtnPageChisel[EECT_End];		//凿孔分页3个换页按钮
	StaticEx*							m_pSteChiselHole[MAX_EQUIPHOLE_NUM];  
	GUIStatic*						m_pStcHaveHoles;
	GUIStatic*						m_pStcChiselRate;
	GUIStatic*						m_pStcChiselGold;
	GUIStatic*						m_pStcChiselSilver;
	GUIProgress*					m_pProChiselOnce;
	GUIButton*						m_pBtnChiselBegin;
	GUIButton*						m_pBtnChiselCancel;

};
